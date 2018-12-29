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

#ifndef _FASTCOM_SUBSCRIBER_H_
#define _FASTCOM_SUBSCRIBER_H_

#include <boost/asio.hpp>
#include <thread>
#include <mutex>

namespace fastcom{
    template<typename DataType_>
    class Subscriber{
        public:
            Subscriber(int _name);

            void appendCallback(std::function<void(DataType_ &)> _callback);

            bool isConnected();
        private:
            boost::asio::ip::udp::endpoint mEndpoint;
            boost::asio::ip::udp::socket *mSocket;
            boost::asio::io_service io_service;

            std::mutex mCallbackGuard;
            std::vector<std::function<void(DataType_ &)>> mCallbacks;

            std::thread mListenThread;
            bool mRun = false;
    };
}

#include <fastcom/Subscriber.inl>

#endif