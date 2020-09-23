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

#ifndef FASTCOM_SUBSCRIBER_H_
#define FASTCOM_SUBSCRIBER_H_

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include <functional>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

namespace fastcom{
    template<typename SerializableObject_>
    class Subscriber{
    public:
        typedef std::function<void(const SerializableObject_)> Callback;
        Subscriber(const std::string &_resourceName);

        void addCallback(Callback _cb);
    private:
        void deserializeData(const std::string &_data, SerializableObject_ &_out);

        typedef websocketpp::client<websocketpp::config::asio_client> Client;
        void on_message(websocketpp::connection_hdl hdl, Client::message_ptr msg);
        void pubListUpdatedCb(std::vector<std::string> _list);
        void addConnection(std::string _uri);

        std::map<std::string, Client *> connectionWithPubs_;

    private:
        std::vector<Callback> callbacks_;
        std::mutex cbListLock_;

        std::string resourceName_;
    };

}

#include <fastcom/Subscriber.inl>
#include <fastcom/impl/SubscriberDeserializers.inl>

#endif