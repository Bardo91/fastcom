//---------------------------------------------------------------------------------------------------------------------
//  FASTCOM
//---------------------------------------------------------------------------------------------------------------------
//  Copyright 2020 -    Manuel Perez Jimenez (a.k.a. manuoso)
//                      Marco A. Montes Grova (a.k.a. mgrova) 
//                      Pablo Ramon Soria (a.k.a. Bardo91)
//                      Ricardo Lopez Lopez (a.k.a. ric92)
//---------------------------------------------------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
//  and associated documentation files (the "Software"), to deal in the Software without restriction, 
//  including without limitation the rights to use, copy, modify, merge, publish, distribute, 
//  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all copies or substantial 
//  portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
//  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES 
//  OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//---------------------------------------------------------------------------------------------------------------------

#include <fastcom/ConnectionManager.h>

namespace fastcom{
    template<typename SerializableObject_>
    Publisher<SerializableObject_>::Publisher(const std::string &_uri){
        uri_ = _uri;
        auto &cm = fastcom::ConnectionManager::get();

        magicInitOfData();
        initServer();
        cm.registerUri(ip_, port_, "/integer_count");
    }

    template<typename SerializableObject_>
    void Publisher<SerializableObject_>::publish(SerializableObject_ _msg){
        websocketpp::lib::error_code ec;
        std::string serializedMsg;
        _msg >> serializedMsg;
        
        lock.lock();
        for(auto con:subscribers_){
            internalServer_.send(con, serializedMsg, websocketpp::frame::opcode::binary, ec);
        }
        lock.unlock();
    }

    template<typename SerializableObject_>
    void Publisher<SerializableObject_>::magicInitOfData(){
        try {
            boost::asio::io_service netService;
            boost::asio::ip::udp::resolver   resolver(netService);
            boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), "google.com", "");
            boost::asio::ip::udp::resolver::iterator endpoints = resolver.resolve(query);
            boost::asio::ip::udp::endpoint ep = *endpoints;
            boost::asio::ip::udp::socket socket(netService);
            socket.connect(ep);
            boost::asio::ip::address addr = socket.local_endpoint().address();
            ip_ = addr.to_string();
            port_ = socket.local_endpoint().port();
            socket.close(); // close socket! and reuse ip and port.
        } catch (std::exception& e){
            std::cerr << "Could not deal with socket. Exception: " << e.what() << std::endl;

        }
    }

    template<typename SerializableObject_>
    void Publisher<SerializableObject_>::initServer(){
        listenThread_ = std::thread([&]{

            try {
                // Set logging settings
                // internalServer_.set_access_channels(websocketpp::log::alevel::all);
                // internalServer_.clear_access_channels(websocketpp::log::alevel::frame_payload);
                internalServer_.set_access_channels(websocketpp::log::alevel::none);

                // Initialize Asio
                internalServer_.init_asio();

                internalServer_.set_open_handler(std::bind(&Publisher::on_open, this, std::placeholders::_1));
                internalServer_.set_close_handler(std::bind(&Publisher::on_close, this,std::placeholders::_1));

                // Listen on port 9002
                internalServer_.listen(port_);

                // Start the server accept loop
                internalServer_.start_accept();

                // Start the ASIO io_service run loop
                std::cout << "ready to listen in port " << port_ << std::endl;
                internalServer_.run();
            } catch (websocketpp::exception const & e) {
                std::cout << e.what() << std::endl;
            } catch (...) {
                std::cout << "other exception" << std::endl;
            }
        });
    }


    template<typename SerializableObject_>
    void Publisher<SerializableObject_>::on_open(websocketpp::connection_hdl hdl) {
        lock.lock();
        subscribers_.insert(hdl);
        Server::connection_ptr con = internalServer_.get_con_from_hdl(hdl);
        std::cout << con->get_resource() << std::endl;
        lock.unlock();
    }

    template<typename SerializableObject_>
    void Publisher<SerializableObject_>::on_close(websocketpp::connection_hdl hdl) {
        lock.lock();
        subscribers_.erase(hdl);
        lock.unlock();
    }

}