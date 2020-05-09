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
#include <fastcom/Mole.h>

#include <functional>

#include <condition_variable>

namespace fastcom{
    ConnectionManager *ConnectionManager::instance_ = nullptr;

    ConnectionManager &ConnectionManager::get(){
        if(instance_ == nullptr){
            instance_ = new ConnectionManager;
        }
        return *instance_;
    }



    void ConnectionManager::registerUri(const std::string &_ip, const uint16_t &_port, const std::string &_uri){
        websocketpp::lib::error_code ec;
        std::string data = "register,"+_ip+":"+std::to_string(_port)+_uri;
        connectionWithMole_->send(*connectionHandler_, data, websocketpp::frame::opcode::text, ec);
    }

    void ConnectionManager::unregisterUri(const std::string &_ip, const uint16_t &_port, const std::string &_uri){
        websocketpp::lib::error_code ec;
        std::string data = "unregister,"+_ip+":"+std::to_string(_port)+_uri;
        connectionWithMole_->send(*connectionHandler_, data, websocketpp::frame::opcode::text, ec);
    }


    void ConnectionManager::queryListPublishers(const std::string &_uri, std::function<void(std::vector<std::string>)> _responseCb){
        updateSubTable_[_uri].push_back(_responseCb);

        websocketpp::lib::error_code ec;
        std::string data = "query_publishers,"+_uri;
        connectionWithMole_->send(*connectionHandler_, data, websocketpp::frame::opcode::text, ec);
    }

    ConnectionManager::ConnectionManager(){
        while(!connectToMole()){
            initMole();
            //std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        isConnected_ = true;
    }


    bool ConnectionManager::connectToMole(){
        bool connectionExist = false;
        std::unique_lock<std::mutex> lock(connectionGuard_);

        connectionThread_ = std::thread([&]{
            std::string hostIP;
            if(getenv("FASTCOM_MOLE_IP"))
                hostIP = getenv("FASTCOM_MOLE_IP"); // In the future look for it in the net.
            else 
                hostIP = "localhost";
                
            std::string uri = "ws://"+hostIP+":"+std::to_string(Mole::port());
            connectionWithMole_ = new Client;
            size_t runResult = 0;
            try {
                connectionWithMole_->set_access_channels(websocketpp::log::alevel::none);
                connectionWithMole_->set_error_channels(websocketpp::log::alevel::none);
                // Initialize ASIO
                connectionWithMole_->init_asio();

                connectionWithMole_->set_message_handler(std::bind(&ConnectionManager::moleMessage,this,std::placeholders::_1,std::placeholders::_2));

                websocketpp::lib::error_code ec;
                auto con =  connectionWithMole_->get_connection(uri, ec);
                connectionHandler_ = &con; // 666 ufff
                
                connectionWithMole_->connect(*connectionHandler_);

                // Start the ASIO io_service run loop
                // this will cause a single connection to be made to the server. connectionWithMole_->run()
                // will exit when this connection is closed.
                connectionExist = true;
                runResult = connectionWithMole_->run();
            } catch (websocketpp::exception const & e) {
                std::cout << e.what() << std::endl;
                connectionExist = false;
                connectionCV_.notify_all();
            }
            // Exited for some reason
            connectionExist = false;
            connectionCV_.notify_all();
            
        });

        connectionCV_.wait(lock);
        
        if(!connectionExist){
            connectionThread_.join();
        }
        return connectionExist;
    }

    bool ConnectionManager::initMole(){
        return Mole::init();
    }

    bool ConnectionManager::isConnectedToMole(){
        return isConnected_;
    }


    std::vector<std::string> split(const std::string& s, char delimiter){
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    void ConnectionManager::moleMessage(websocketpp::connection_hdl _hdl, Client::message_ptr _msg){
        // Parse message
        std::string rawMsg = _msg->get_payload();

        // decode action
        std::string cmd = rawMsg.substr(0, rawMsg.find_first_of("@"));
        rawMsg = rawMsg.substr(rawMsg.find_first_of("@")+1, rawMsg.size());

        if (cmd == "connection_etablished"){
            connectionCV_.notify_all();
        } else if(cmd == "new_publisher"){
            std::string publisherInfo = rawMsg.substr(0, rawMsg.find_first_of("/"));
            std::string stream = rawMsg.substr(rawMsg.find_first_of("/"), rawMsg.size());
            for(auto &subscriber : updateSubTable_[stream]){
                subscriber({publisherInfo});
            }
        }else if(cmd == "list"){
            // Now we only have one action, notify new uris.
            std::string targetUri = rawMsg.substr(0, rawMsg.find_first_of("|"));
            rawMsg = rawMsg.substr(rawMsg.find_first_of("|")+1, rawMsg.size());
        
            // Decode list of publishers.
            std::vector<std::string> list = split(rawMsg, ',');

            // Update subscribers.
            for(auto &subscriber : updateSubTable_[targetUri]){
                subscriber(list);
            }

        }
    }
}