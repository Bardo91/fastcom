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

#include <fastcom/ImageSubscriber.h>

#ifdef FASTCOM_HAS_OPENCV

namespace fastcom{
    ImageSubscriber::ImageSubscriber(std::string _ip, int _port){
        mSubscriber = new Subscriber<ImageDataPacket>(_ip, _port);
        mSubscriber->appendCallback(
            [&](ImageDataPacket &_data){
                this->coreCallback(_data);
            }
        );
    }

    ImageSubscriber::ImageSubscriber(int _port): ImageSubscriber("0.0.0.0", _port){  }

    void ImageSubscriber::appendCallback(std::function<void(cv::Mat &)> &_callback){
        mCallbackGuard.lock();
        mCallbacks.push_back(_callback);
        mCallbackGuard.unlock();
    }

    void ImageSubscriber::coreCallback(ImageDataPacket &_data){
        if(isFirst == _data.isFirst){
            if(isFirst)
                std::cout << _data.numPackets << ": ";

            std::cout << _data.packetId << "|";
            std::cout.flush();
            
            isFirst = false;
            if(packetId + 1 == _data.packetId){
                totalBuffer.insert(totalBuffer.end(), _data.buffer, _data.buffer+_data.packetSize);
                packetId++;
                if(_data.packetId == _data.numPackets-1){
                    if(totalBuffer.size() == _data.totalSize){
                        // decode image
                        cv::Mat image = cv::imdecode(totalBuffer, 1);

                        mCallbackGuard.lock();
                        for(auto &callback: mCallbacks){
                            callback(image);
                        }
                        mCallbackGuard.unlock();

                        // reset data
                        isFirst = true;
                        packetId = -1;
                        totalBuffer.clear();
                        int timeDiff = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now() - _data.timeStamp).count() ;
                        std::cout << "Speed transmission: " << timeDiff << std::endl;
                    }
                }
            }
        }else{
            std::cout << std::endl;
        }
    }
}


#endif