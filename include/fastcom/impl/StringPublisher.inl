
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

#include <string>

namespace fastcom{
    template<>
    void Publisher<std::string>::publish(const std::string &_data){
        if(mRun && mServerSocket){
            mSafeGuard.lock();
            for (auto &con : mUdpConnections) {
                boost::system::error_code error;
                boost::system::error_code ignored_error;

                int nPackets = _data.size();
                // std::vectors have a more complex structure. It is dangeorous because packets may get lost! 666
                // Send Number of packets
                {
                    boost::array<char, sizeof(int)> send_buffer;
                    memcpy(&send_buffer[0], &nPackets, sizeof(int));
                    try {
                        mServerSocket->send_to(boost::asio::buffer(send_buffer), *con, 0, ignored_error);
                    }
                    catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                std::vector<char> stdBuffer(_data.begin(), _data.end());
                try {
                    mServerSocket->send_to(boost::asio::buffer(stdBuffer), *con, 0, ignored_error);
                }
                catch (std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            }
            mSafeGuard.unlock();
        }    
    }
}
