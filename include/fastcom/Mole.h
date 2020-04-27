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

#ifndef FASTCOM_MOLE_H_
#define FASTCOM_MOLE_H_

#include <unordered_map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>


#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

namespace fastcom{

    class Mole{
    public:
        typedef websocketpp::server<websocketpp::config::asio> Server;

        static uint16_t port();

        static bool init();

    private:
        Mole();
        ~Mole();
        static Mole *instance_;

        void onMessage(websocketpp::connection_hdl hdl, Server::message_ptr msg);
        void onOpen(websocketpp::connection_hdl hdl);
        void onClose(websocketpp::connection_hdl hdl);

        enum class MessageType {Unknown, RegisterUri, UnregisterUri, CloseMole, QueryPublishers};
        MessageType decodeAction(const std::string &_uri);
        void doAction(Server::connection_ptr hdl, MessageType _type, std::string _data);

        void registerUri(std::string _uri);
        void unregisterUri(std::string _uri);
        void closeMole();
        void sendListPublishers(std::string _uri);

    private: // Comm related membersXD
        static const uint16_t port_ = 22322;
        std::thread listenThread_;
        Server *server_;

        // Map with connections and uris.
        std::unordered_map<std::string, std::vector<std::string>> uriTable_;
        std::mutex serverGuard_;
        static bool isInit_;
    };

}

#endif