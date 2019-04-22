//---------------------------------------------------------------------------------------------------------------------
//  FASTCOM
//---------------------------------------------------------------------------------------------------------------------
//  Copyright 2018 - Pablo Ramon Soria (a.k.a. Bardo91) 
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
#include <chrono>
namespace fastcom{
    
    template<typename RequestType_, typename ResponseType_>
    ServiceClient<RequestType_, ResponseType_>::ServiceClient(std::string _ip, int _port){
        ip_ = _ip;
        port_ = _port;        
    }

    template<typename RequestType_, typename ResponseType_>
    ResponseType_ ServiceClient<RequestType_, ResponseType_>::call(RequestType_ &_req){
        // Connect to server
        boost::asio::ip::tcp::socket socket(ioService_);
        socket.connect( boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string(ip_), port_ ));

        // Receive fastcom version
        boost::asio::streambuf buf;
        boost::asio::read_until(socket, buf, "\n");
        std::string fastcomVersion = boost::asio::buffer_cast<const char*>(buf.data());
        
        ResponseType_ response;
        if(fastcomVersion == ("fastcom_"+fastcom::FASTCOM_VERSION+"\n")){
            // Sending hash code of request
            size_t hashCode = typeid(_req).hash_code();
            const char * hashCodePtr = reinterpret_cast<const char*>(&hashCode);
            boost::asio::write( socket, boost::asio::buffer(hashCodePtr, sizeof(size_t)) );

            // Check if request type is ok
            boost::asio::streambuf buf2;
            boost::asio::read_until(socket, buf2, "\n");
            std::string typeResponse = boost::asio::buffer_cast<const char*>(buf2.data());
            if(typeResponse == "good_request\n"){
                // Send request
                const char * requestPtr = reinterpret_cast<const char*>(&_req);
                boost::asio::write( socket, boost::asio::buffer(requestPtr, sizeof(RequestType_)) );

                response = receiveType<ResponseType_>(&socket);

                boost::asio::write(socket, boost::asio::buffer("end\n") );

            }else{
                std::cout << "\033[1;31m [ERROR]    Hash code of request inconsistent with server version\033[0m\n";
            }

        }else{
            // Bad fastcom version.
            std::cout << "\033[1;31m [ERROR]    Inconsistent fastcom version. Expected"+fastcom::FASTCOM_VERSION+" but \
                                                received "+fastcomVersion+"\033[0m\n";
        }
        
        return response;
        socket.close();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }



    template<typename RequestType_, typename ResponseType_>
    template<typename T_>
    T_ ServiceClient<RequestType_, ResponseType_>::receiveType(boost::asio::ip::tcp::socket *_socket){
        boost::asio::streambuf sb;
        boost::asio::read(*_socket, sb, boost::asio::transfer_exactly(sizeof(T_)));
        boost::asio::streambuf::const_buffers_type bufs = sb.data();
        std::string objStr(     boost::asio::buffers_begin(bufs),
                                            boost::asio::buffers_begin(bufs) + sizeof(T_));

        T_ obj;
        memcpy(&obj, objStr.c_str(), sizeof(T_));
        return obj;
    }

}