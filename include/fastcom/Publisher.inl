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

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>


namespace fastcom {
    template<typename DataType_>
    Publisher<DataType_>::Publisher(int _port){
        mPort = _port;
        mRun = true;
		mBroadcastEndpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), 8888);
		mServerSocket = new boost::asio::ip::udp::socket(io_service);
		mServerSocket->open(boost::asio::ip::udp::v4());
		boost::asio::ip::udp::endpoint remote_endpoint;
		mServerSocket->connect(remote_endpoint);
		mServerSocket->set_option(boost::asio::socket_base::broadcast(true));
		
	}

    //-----------------------------------------------------------------------------------------------------------------
    template<typename DataType_>
    void Publisher<DataType_>::publish(DataType_ &_data){
		boost::system::error_code ignored_error;
		boost::array<char, sizeof(DataType_)> send_buffer;
		memcpy(&send_buffer[0], &_data, sizeof(DataType_));

		try {
			mServerSocket->send_to(boost::asio::buffer(send_buffer),mBroadcastEndpoint, 0, ignored_error);
		}
		catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
    }
} 