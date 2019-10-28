//---------------------------------------------------------------------------------------------------------------------
//  FASTCOM
//---------------------------------------------------------------------------------------------------------------------
//  Copyright 2019 - Pablo Ramon Soria (a.k.a. Bardo91) 
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

#include <fastcom/fastcom_version.h>
#include <iostream>

namespace fastcom{

    template<typename RequestType_, typename ResponseType_>
    ServiceServer<RequestType_, ResponseType_>::ServiceServer(int _port, std::function<void (RequestType_ &, ResponseType_ &)> _callback): 
                                serverSocket_(  ioService_, 
                                                boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), 
                                                _port )
                                            ){
            callback_ = _callback;
            port_ = _port;
            acceptingConnections_ = true;

            acceptingThread_ = std::thread([&](){

                while(acceptingConnections_){
                    boost::asio::ip::tcp::socket *clientSocket = new boost::asio::ip::tcp::socket(ioService_);

                    serverSocket_.accept(*clientSocket);

                    std::thread clientThread([&](boost::asio::ip::tcp::socket *_client){
                        // Send boost version to check compatibility.
                        boost::asio::write(*_client, boost::asio::buffer(fastcom::FASTCOM_VERSION+"\n") );
                        
                        // Receive hash code of class.
                        size_t hashCodeRequest = retreiveHashCode(_client);
                        RequestType_ request;
                        if(request.checkType(hashCodeRequest)){
                            // Good request type, send query for data.
                            boost::asio::write(*_client, boost::asio::buffer("good_request\n") );

                            // Get request data
                            request = receiveType<RequestType_>(_client);

                            // process response
                            ResponseType_ response;
                            callback_(request, response);

                            // Send response
                            const char * responsePtr = reinterpret_cast<const char*>(&response);
                            auto len = boost::asio::write(*_client, boost::asio::buffer(responsePtr, sizeof(ResponseType_)) );

                            // Wait to finish
                            boost::asio::streambuf sb;
                            boost::asio::read_until(*_client, sb, "\n");
                            std::string finalAnswer = boost::asio::buffer_cast<const char*>(sb.data());
                        }else{
                            // Bad request type, send error msg.
                            std::cout << "\033[1;31m [ERROR]    Hash code of request is not the same than the expected. Received: "+std::to_string(hashCodeRequest)+"; but expected: "+std::to_string(typeid(RequestType_).hash_code())+" \033[0m\n";
                            boost::asio::write(*_client, boost::asio::buffer("bad_request_type\n") );
                        }

                        _client->close();

                        delete _client;
                    }, clientSocket);

                    clientThread.detach();
                }
            });
    }


    template<typename RequestType_, typename ResponseType_>
    ServiceServer<RequestType_, ResponseType_>::~ServiceServer(){
        acceptingConnections_ = false;

        boost::asio::ip::tcp::socket socket(ioService_);
        socket.connect( boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string("0.0.0.0"), port_ ));

        if(acceptingThread_.joinable())
            acceptingThread_.join();
    }


    template<typename RequestType_, typename ResponseType_>
    size_t ServiceServer<RequestType_, ResponseType_>::retreiveHashCode(boost::asio::ip::tcp::socket *_client){
        boost::asio::streambuf sb;
        boost::asio::read(*_client, sb, boost::asio::transfer_exactly(sizeof(size_t)));
        boost::asio::streambuf::const_buffers_type bufs = sb.data();
        std::string hashCodeRequestStr(     boost::asio::buffers_begin(bufs),
                                            boost::asio::buffers_begin(bufs) + sizeof(size_t));

        size_t hashCodeRequest;
        memcpy(&hashCodeRequest, hashCodeRequestStr.c_str(), sizeof(size_t));
        return hashCodeRequest;
    }


    template<typename RequestType_, typename ResponseType_>
    template<typename T_>
    T_ ServiceServer<RequestType_, ResponseType_>::receiveType(boost::asio::ip::tcp::socket *_client){
        boost::asio::streambuf sb;
        boost::asio::read(*_client, sb, boost::asio::transfer_exactly(sizeof(T_)));
        boost::asio::streambuf::const_buffers_type bufs = sb.data();
        std::string objStr(     boost::asio::buffers_begin(bufs),
                                            boost::asio::buffers_begin(bufs) + sizeof(T_));

        T_ obj;
        memcpy(&obj, objStr.c_str(), sizeof(T_));
        return obj;
    }



}
