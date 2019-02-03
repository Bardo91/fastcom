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


#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>

namespace fastcom{
    //---------------------------------------------------------------------------------------------------------------------
    template<typename DataType_>
    Subscriber<DataType_>::Subscriber(std::string _ip, int _port){
        mEndpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(_ip), _port);
        mHomeDir = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), _port);

        mSocket = new boost::asio::ip::udp::socket(io_service);
        mSocket->open(boost::asio::ip::udp::v4());
        
        mSocket->set_option(boost::asio::ip::udp::socket::reuse_address(true));
        // mSocket->set_option(boost::asio::socket_base::broadcast(true));
        mSocket->bind(mHomeDir);

        mRun = true;
        mListenThread = std::thread([&](){
            while(mRun){
                boost::array<char, sizeof(DataType_)> recv_buf;
                boost::asio::ip::udp::endpoint sender_endpoint;
                size_t len = mSocket->receive(boost::asio::buffer(recv_buf));

                DataType_ packet;
                memcpy(&packet, &recv_buf[0], sizeof(DataType_));

                mCallbackGuard.lock();
                for(auto &callback: mCallbacks){
                    callback(packet);
                }
                mCallbackGuard.unlock();
            }
        });
    }

    //---------------------------------------------------------------------------------------------------------------------
    template<typename DataType_>
    Subscriber<DataType_>::Subscriber(int _port): Subscriber("0.0.0.0", _port) {  }

    //---------------------------------------------------------------------------------------------------------------------
    template<typename DataType_>
    void Subscriber<DataType_>::attachCallback(std::function<void(DataType_ &)> _callback){
        mCallbackGuard.lock();
        mCallbacks.push_back(_callback);
        mCallbackGuard.unlock();
    }

}