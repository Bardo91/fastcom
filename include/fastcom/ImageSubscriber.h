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

#ifndef _FASTCOM_IMAGESUBSCRIBER_H_
#define _FASTCOM_IMAGESUBSCRIBER_H_

#include <fastcom/ImagePublisher.h>
#include <fastcom/Subscriber.h>

#ifdef FASTCOM_HAS_OPENCV

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace fastcom{
    /// Subscriber specialized for image transmission
    class ImageSubscriber {
        public:
            /// Constructor to subscribe to ImagePublishers in different computers.
            /// \param _ip: IP where the ImagePublisher is located
            /// \param _port: Specific port in the IP to bind
            ImageSubscriber(std::string _ip, int _port);

            /// Constructor to subscribe to ImagePublishers. It assumes that the publisher is in the same computer
            /// \param _port: Specific port in the IP to bind
            ImageSubscriber(int _port);

            /// Attach a callback to a subscription. Each time an image arrives the attached callback are called
            void attachCallback(std::function<void(cv::Mat &)> &_callback);
            
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
