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
    Subscriber<SerializableObject_>::Subscriber(const std::string &_resourceName){
        resourceName_ = _resourceName;
        auto &cm = fastcom::ConnectionManager::get();

        cm.queryListPublishers(_resourceName, std::bind(&Subscriber<SerializableObject_>::pubListUpdatedCb, this, std::placeholders::_1));
        
    }

    template<typename SerializableObject_>
    void Subscriber<SerializableObject_>::addCallback(Callback _cb){

    }

    template<typename SerializableObject_>
    void Subscriber<SerializableObject_>::on_message(websocketpp::connection_hdl hdl, Client::message_ptr msg) {
        std::cout << msg->get_payload() << std::endl;
    }

    template<typename SerializableObject_>
    void Subscriber<SerializableObject_>::pubListUpdatedCb(std::vector<std::string> _list){
        for(auto &pubUri: _list){
            if(connectionWithPubs_.find(pubUri) == connectionWithPubs_.end()){
                addConnection(pubUri);
            }
        }
    }

    template<typename SerializableObject_>
    void Subscriber<SerializableObject_>::addConnection(std::string _uri){
        std::thread clientThread([&](std::string _uri){
            std::string uri = "ws://"+_uri+resourceName_;
            try {
                Client *client = new Client;
                connectionWithPubs_[_uri] = client; 
                client->set_access_channels(websocketpp::log::alevel::none);
                client->set_error_channels(websocketpp::log::alevel::none);
                // Initialize ASIO
                client->init_asio();

                // Register our message handler
                client->set_message_handler(std::bind(&Subscriber::on_message,this,std::placeholders::_1,std::placeholders::_2));

                websocketpp::lib::error_code ec;
                Client::connection_ptr con = client->get_connection(uri, ec);
                if (ec) {
                    std::cout << "could not create connection because: " << ec.message() << std::endl;
                    return;
                }

                client->connect(con);
                client->run();
            } catch (websocketpp::exception const & e) {
                std::cout << e.what() << std::endl;
            }
        },_uri);
        clientThread.detach();
    }
}