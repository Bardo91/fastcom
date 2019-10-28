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

#ifndef _FASTCOM_SERVICESERVER_H_
#define _FASTCOM_SERVICESERVER_H_

#include <boost/asio.hpp>
#include<functional>
#include <thread>

namespace fastcom{
    #define SERVICE_MESSAGE_TYPE \
    public: \
	bool checkType(size_t _hash){ \
		return this->type() == _hash; \
	} \
	size_t type(){ \
		return 0; \
	}

    /// Service Server class
    template<typename RequestType_, typename ResponseType_>
    class ServiceServer{
    public:
        /// Basic constructor. Uses given port to accept connections
        /// \param _port: port to accept connections
        ServiceServer(int _port, std::function<void (RequestType_ &, ResponseType_ &)> _callback);

        /// Basic destructor. Stop and disable the server.
        ~ServiceServer();

    private:
        size_t retreiveHashCode(boost::asio::ip::tcp::socket *_client);
        
        template<typename T_>
        T_ receiveType(boost::asio::ip::tcp::socket *_client);

    private:
        int port_;

        boost::asio::io_service ioService_;
        boost::asio::ip::tcp::acceptor serverSocket_;

        bool acceptingConnections_;
        std::thread acceptingThread_;

        std::function<void (RequestType_ &, ResponseType_ &)> callback_;
    };
}

#include <fastcom/ServiceServer.inl>

#endif