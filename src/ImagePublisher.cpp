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

#include <fastcom/ImagePublisher.h>


#ifdef FASTCOM_HAS_OPENCV

namespace fastcom{
    ImagePublisher::ImagePublisher(int _port) {
        mPublisher = new Publisher<ImageDataPacket>(_port);
    }

    void ImagePublisher::publish(cv::Mat &_image, int _compression){
        std::vector<int> params;
        params.push_back( cv::IMWRITE_JPEG_QUALITY);
        params.push_back(30); //image quality
        std::vector<uchar> buffer;

        cv::imencode(".jpg", _image, buffer, params);
        auto bufferPtr = buffer.begin();

        //std::chrono::time_point<std::chrono::system_clock> timeStamp = std::chrono::system_clock::now();

        int numPackets = buffer.size()/IMAGE_PACKET_SIZE + 1;
        for(unsigned i = 0; i < numPackets ; i++){
            ImageDataPacket packet;
            //packet.timeStamp = timeStamp;
            packet.packetId = i;
            packet.isFirst = i == 0;
            packet.numPackets = numPackets;
            packet.totalSize = buffer.size();

            int endSize = (i == numPackets-1)? buffer.size() % IMAGE_PACKET_SIZE : IMAGE_PACKET_SIZE;
            packet.packetSize = endSize;

            std::copy(  bufferPtr, 
                        bufferPtr + endSize, 
                        packet.buffer);

            bufferPtr += endSize;

            mPublisher->publish(packet);
        }
    }
}

#endif
