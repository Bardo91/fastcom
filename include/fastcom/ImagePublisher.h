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

#ifndef _FASTCOM_IMAGEPUBLISHER_H_
#define _FASTCOM_IMAGEPUBLISHER_H_

#include <fastcom/Publisher.h>
#include <opencv2/opencv.hpp>
#include <chrono>

#ifdef FASTCOM_HAS_OPENCV

namespace fastcom{
    struct ImageDataPacket{   
        static const int PACKET_SIZE = 1024;
        bool isFirst = false;
        std::chrono::time_point<std::chrono::system_clock> timeStamp;
        int packetId = 0;
        int numPackets = 0;
        int totalSize = 0;
        int packetSize = 0;
        char buffer[PACKET_SIZE];
    };

    class ImagePublisher {
        public:
            ImagePublisher(int _name);

            void publish(cv::Mat &_image, int _compression = 50);
        private:
            Publisher<ImageDataPacket> *mPublisher;
    };
}

#endif

#endif