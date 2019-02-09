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
#include <boost/bind.hpp>
#include <iostream>

namespace fastcom{
    //---------------------------------------------------------------------------------------------------------------------
    template<typename DataType_>
    Subscriber<DataType_>::Subscriber(std::string _ip, int _port): mDeadlineTimout(io_service) {
        mDeadlineTimout.expires_at(boost::posix_time::pos_infin);

        mEndpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(_ip), _port);
        mSocket = new boost::asio::ip::udp::socket(io_service);
        mSocket->open(boost::asio::ip::udp::v4());

        checkDeadline();

        // std::cout << "Trying to connect to " + std::to_string(_port) << std::endl;
        mConnectionThread = std::thread([&](){
            try {	
				for(;;){
                    // Send query to publisher
                    boost::array<char, 1> send_buf = { { 0 } };
                    mSocket->send_to(boost::asio::buffer(send_buf), mEndpoint);
                    
                    // Set timeout
                    mDeadlineTimout.expires_from_now(boost::posix_time::milliseconds(200));

                    // Wait for response
                    boost::system::error_code ec = boost::asio::error::would_block;
                    std::size_t length = 0;

					boost::array<char, 1> recv_buf;
					mSocket->async_receive_from( boost::asio::buffer(recv_buf), 
                                                    mEndpoint, 
                                                    boost::bind(
                                                            &Subscriber<DataType_>::asyncConnectionHandle, this,
                                                            boost::asio::placeholders::error,
                                                            boost::asio::placeholders::bytes_transferred)
                                                    );

					io_service.run_one();					
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    
                    if(mReceivedConnectionNotification){
                        mRun = true;
                        mListenThread = std::thread([&](){
                            while(mRun){
                                boost::array<char, sizeof(DataType_)> recv_buf;
                                boost::asio::ip::udp::endpoint sender_endpoint;
                                size_t len = mSocket->receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
                                
                                mLastStamp = std::chrono::system_clock::now();

                                DataType_ packet;
                                memcpy(&packet, &recv_buf[0], sizeof(DataType_));

                                mCallbackGuard.lock();
                                for(auto &callback: mCallbacks){
                                    callback(packet);
                                }
                                mCallbackGuard.unlock();
                            }
                        });

                        break;
                    }
					io_service.reset();
				}
			}catch (std::exception &e) {
				std::cerr << e.what() << std::endl;
			}
			// std::cout << "Closing reading of new connections" << std::endl;
        });
        
        mLastStamp = std::chrono::system_clock::now();
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

    //---------------------------------------------------------------------------------------------------------------------
    template<typename DataType_>
    void Subscriber<DataType_>::asyncConnectionHandle(const boost::system::error_code & error,  std::size_t length){
        if(length == 1)
        {
            mReceivedConnectionNotification = true;
        }
    }

    //---------------------------------------------------------------------------------------------------------------------
    template<typename DataType_>
    void Subscriber<DataType_>::checkDeadline() {
        if (mDeadlineTimout.expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
            mSocket->cancel();
            mDeadlineTimout.expires_at(boost::posix_time::pos_infin);
        }
        mDeadlineTimout.async_wait(boost::bind(&Subscriber<DataType_>::checkDeadline, this));
    }

}