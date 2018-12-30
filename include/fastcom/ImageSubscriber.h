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

#ifndef _FASTCOM_IMAGESUBSCRIBER_H_
#define _FASTCOM_IMAGESUBSCRIBER_H_

#include <fastcom/ImagePublisher.h>
#include <fastcom/Subscriber.h>
#include <opencv2/opencv.hpp>

#ifdef FASTCOM_HAS_OPENCV

namespace fastcom{
    class ImageSubscriber {
        public:
            ImageSubscriber(std::string _ip, int _port);
            ImageSubscriber(int _port);

            void appendCallback(std::function<void(cv::Mat &)> &_callback);
            
        private:
            void coreCallback(ImageDataPacket &_data);

        private:
            Subscriber<ImageDataPacket> *mSubscriber;
            
            std::mutex mCallbackGuard;
            std::vector<std::function<void(cv::Mat &)>> mCallbacks;


            bool isFirst = true;
            int packetId = -1;
            std::vector<char> totalBuffer;
    };
}

#endif

#endif