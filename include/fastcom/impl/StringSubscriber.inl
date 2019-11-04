
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
    template<>
    bool Subscriber<std::string>::listenCallback_impl(std::string &_packet){
        boost::asio::ip::udp::endpoint sender_endpoint;
        
		size_t nPackets = 0;
        
        boost::array<char, sizeof(size_t)> recv_buf;
        size_t len = mSocket->receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        if(len != sizeof(size_t)){
            return false;
        }
        memcpy(&nPackets, &recv_buf[0], sizeof(size_t));
    
        std::vector<char> stdBuffer(nPackets);
        len = mSocket->receive_from(boost::asio::buffer(stdBuffer), sender_endpoint);
        if(len != nPackets){
            return false;
        }

        _packet = std::string(stdBuffer.begin(), stdBuffer.end());
        return true;
    }

}
