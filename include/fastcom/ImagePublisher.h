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

#ifndef _FASTCOM_IMAGEPUBLISHER_H_
#define _FASTCOM_IMAGEPUBLISHER_H_

#ifdef FASTCOM_HAS_OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#endif

#include <chrono>

#ifdef FASTCOM_HAS_OPENCV

namespace fastcom{
    template<typename T_>
    class Publisher;

    /// Maximum packet size used to send images
    static const int IMAGE_PACKET_SIZE = 1024;

    /// Data packet used by ImagePublisher to split images into packets.
    struct ImageDataPacket{   
        int PACKET_SIZE = IMAGE_PACKET_SIZE;
        bool isFirst = false;
        int packetId = 0;
        int numPackets = 0;
        int totalSize = 0;
        int packetSize = 0;
        char buffer[IMAGE_PACKET_SIZE];
    };

    /// Specialized Publisher to publish images to subscribers
    class ImagePublisher {
        public:
            /// Basic constructor. It binds to an specific port and sends information to subscriber.
            /// \param _port: ports to be binded.
            ImagePublisher(int _port);

            /// Publish an image to the subscribers.
            /// \param _image: image to be published.
            /// \param _compression: JPG compression rate.
            void publish(cv::Mat _image, int _compression = 50);
        private:
            Publisher<ImageDataPacket> *mPublisher;
			std::vector<int> params;
			std::vector<uchar> buffer;
    };
}

#endif

#endif
