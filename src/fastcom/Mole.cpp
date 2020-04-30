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

#include <fastcom/Mole.h>

#include <functional>


namespace fastcom{

    Mole *Mole::instance_ = nullptr;
    bool Mole::isInit_ = false;

    uint16_t Mole::port(){
        return port_;
    }

    bool Mole::init(){
        if(!instance_){
            instance_ = new Mole(); // 666 how to check if true.
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if(!isInit_){
                delete instance_;
            }
        }
    }

    Mole::~Mole(){
        listenThread_.join();
    }

    Mole::Mole(){
        listenThread_ = std::thread([&](){
            try {
                server_ = new Server;
                // Disable all loggings
                server_->set_access_channels(websocketpp::log::alevel::none);
                server_->set_error_channels(websocketpp::log::alevel::none);

                // Initialize Asio
                server_->init_asio();

                // Register our message handler
                server_->set_message_handler(std::bind(&Mole::onMessage, this, std::placeholders::_1,  std::placeholders::_2));
                server_->set_open_handler(std::bind(&Mole::onOpen, this,std::placeholders::_1));
                server_->set_close_handler(std::bind(&Mole::onClose, this,std::placeholders::_1));

                // Listen on port 
                server_->listen(port_);

                // Start the server accept loop
                server_->start_accept();
                isInit_ = true;
                server_->run();
            } catch (websocketpp::exception const & e) {
                std::cout << e.what() << std::endl;
            } catch (...) {
                std::cout << "other exception" << std::endl;
            }
            isInit_ = false;
        });
    }

    // Define a callback to handle incoming messages
    void Mole::onMessage(websocketpp::connection_hdl hdl, Server::message_ptr msg) {

        // Message from connection managers. Options are:
        //      1) Register a new uri.      From publisher
        //      2) Unregister uri.          From publisher
        //      3) Close mole.              From any
        //      4) Query list publishers.   From subscriber
        Server::connection_ptr con = server_->get_con_from_hdl(hdl);
        
        std::string data = msg->get_payload();
        std::string action = data.substr(0, data.find_first_of(","));
        std::string uri = data.substr(data.find_first_of(",")+1, data.size());

        MessageType connectionType = decodeAction(action);

        doAction(con, connectionType, uri);
    }

    void Mole::onOpen(websocketpp::connection_hdl hdl) {
        serverGuard_.lock();
        // Connection manager connected, send message back
        websocketpp::lib::error_code ec;
        server_->send(hdl, "connection_etablished@", websocketpp::frame::opcode::binary, ec);
        serverGuard_.unlock();
    }

    void Mole::onClose(websocketpp::connection_hdl hdl) {
        serverGuard_.lock();
        //publishersTable_.erase(hdl.lock());
        serverGuard_.unlock();
    }


    Mole::MessageType Mole::decodeAction(const std::string &_action){
        if(_action == "register"){
            return Mole::MessageType::RegisterUri;
        } else if(_action == "unregister"){
            return Mole::MessageType::UnregisterUri;
        } else if(_action == "close_mole"){
            return Mole::MessageType::CloseMole;
        } else if(_action == "query_publishers"){
            return Mole::MessageType::QueryPublishers;
        } else{
            return Mole::MessageType::Unknown;
        }
    }


    void Mole::doAction(Server::connection_ptr _con, MessageType _type, std::string _uri){
        switch (_type) {
        case MessageType::RegisterUri:
            return registerUri(_uri);
        case MessageType::UnregisterUri:
            return unregisterUri(_uri);
        case MessageType::CloseMole:
            return closeMole();
        case MessageType::QueryPublishers:
            return sendListPublishers(_con, _uri);
        }
    }


    void Mole::registerUri(std::string _uri){
        serverGuard_.lock();
        std::string publisherInfo = _uri.substr(0, _uri.find_first_of("/"));
        std::string stream = _uri.substr(_uri.find_first_of("/"), _uri.size());
        publishersTable_[publisherInfo].push_back(stream);

        if(subscriberNotificationTable_.find(stream) != subscriberNotificationTable_.end()){
            for(auto &con:subscriberNotificationTable_[stream]){
                websocketpp::lib::error_code ec;
                server_->send(con, "new_publisher@"+_uri, websocketpp::frame::opcode::binary, ec);
            }
        }
        serverGuard_.unlock();
    }

    void Mole::unregisterUri(std::string _uri){
        serverGuard_.lock();
        std::string publisherInfo = _uri.substr(0, _uri.find_first_of("/"));
        std::string stream = _uri.substr(_uri.find_first_of("/"), _uri.size());
        if(auto iter = std::find(publishersTable_[publisherInfo].begin(), publishersTable_[publisherInfo].end(), stream) != publishersTable_[publisherInfo].end()){
            // publishersTable_[publisherInfo].erase(iter); 666 TODO
        }
        serverGuard_.unlock();
    }

    void Mole::closeMole(){
        // Not implemented yet
    }

    void Mole::sendListPublishers(Server::connection_ptr _con, std::string _uri){
        serverGuard_.lock();
        
        if(std::find(subscriberNotificationTable_[_uri].begin(), subscriberNotificationTable_[_uri].end(), _con) == subscriberNotificationTable_[_uri].end()){
            subscriberNotificationTable_[_uri].push_back(_con);
        }

        /// Need to store in uriTable also Ips and ports to be connected to.
        std::string listPublishers = "list@"+_uri+"|";
        for(auto &[publisher, topics]: publishersTable_){
            for(auto &topic: topics){
                if(topic == _uri){
                    listPublishers += publisher + ",";
                }
            }
        }
        serverGuard_.unlock();
        if(listPublishers != ""){
            listPublishers = listPublishers.substr(0, listPublishers.size()-1);
            websocketpp::lib::error_code ec;
            server_->send(_con, listPublishers, websocketpp::frame::opcode::binary, ec);
        }
    }


}