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

#ifndef FASTCOM_CONNECTIONMANAGER_H_
#define FASTCOM_CONNECTIONMANAGER_H_

#include <functional>
#include <thread>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

namespace fastcom{

    class ConnectionManager{
    public:
        static ConnectionManager &get();

        void registerUri(const std::string &_ip, const uint16_t &_port, const std::string &_uri);

        void unregisterUri(const std::string &_ip, const uint16_t &_port, const std::string &_uri);

    private:
        // singletone!
        ConnectionManager();
        static ConnectionManager *instance_;
    
        typedef websocketpp::client<websocketpp::config::asio_client> Client;
        Client *connectionWithMole_;
        Client::connection_ptr *connectionHandler_;
        std::thread connectionThread_;

        std::mutex connectionGuard_;
        std::condition_variable connectionCV_;
        bool connectToMole();
        bool initMole();
        bool isConnectedToMole();
        bool isConnected_ = false;

    private:

    };

}

#endif