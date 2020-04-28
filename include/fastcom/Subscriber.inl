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
    Subscriber<SerializableObject_>::Subscriber(const std::string &_uri){
        auto &cm = fastcom::ConnectionManager::get();

        cm.queryListPublishers(_uri);
        
    }

    template<typename SerializableObject_>
    void Subscriber<SerializableObject_>::addCallback(Callback _cb){

    }

    // template<typename SerializableObject_>
    // void Subscriber<SerializableObject_>::addConnection(std::string _uri){
    //     try {
    //         // Set logging to be pretty verbose (everything except message payloads)
    //         // client_.set_access_channels(websocketpp::log::alevel::all);
    //         // client_.clear_access_channels(websocketpp::log::alevel::frame_payload);

    //         client_.set_access_channels(websocketpp::log::alevel::none);
    //         // Initialize ASIO
    //         client_.init_asio();

    //         // Register our message handler
    //         client_.set_message_handler(std::bind(&Subscriber::on_message,this,::_1,::_2));

    //         websocketpp::lib::error_code ec;
    //         client::connection_ptr con = client_.get_connection(uri, ec);
    //         if (ec) {
    //             std::cout << "could not create connection because: " << eclient_.message() << std::endl;
    //             return;
    //         }

    //         // Note that connect here only requests a connection. No network messages are
    //         // exchanged until the event loop starts running in the next line.
    //         client_.connect(con);

    //         // Start the ASIO io_service run loop
    //         // this will cause a single connection to be made to the server. client_.run()
    //         // will exit when this connection is closed.
    //         client_.run();
    //     } catch (websocketpp::exception const & e) {
    //         std::cout << e.what() << std::endl;
    //     }
    // }
}