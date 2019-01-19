##---------------------------------------------------------------------------------------------------------------------
##  FASTCOM
##---------------------------------------------------------------------------------------------------------------------
##  Copyright 2018 - Pablo Ramon Soria (a.k.a. Bardo91) 
##---------------------------------------------------------------------------------------------------------------------
##  Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
##  and associated documentation files (the "Software"), to deal in the Software without restriction, 
##  including without limitation the rights to use, copy, modify, merge, publish, distribute, 
##  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
##  furnished to do so, subject to the following conditions:
##
##  The above copyright notice and this permission notice shall be included in all copies or substantial 
##  portions of the Software.
##
##  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
##  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
##  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES 
##  OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
##  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
##---------------------------------------------------------------------------------------------------------------------

import socket
import sys
import threading
import struct
import cv2
import array 


IMAGE_PACKET_SIZE = 1024
class ImageDataPacket:
    PACKET_SIZE = IMAGE_PACKET_SIZE
    isFirst = True
    packetId = 0
    numPackets = 0
    totalSize = 0
    packetSize = 0
    buffer =  [0] * PACKET_SIZE

""" ImagePublisher class.
This class is used to share images to subscribers
"""
class ImagePublisher:
    """ Basic initializer
    """
    def __init__(self, _port):
        # Some init variables
        self.run = True
        self.guard_list = threading.Lock()
        self.client_list = []

        # Create socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_address = ('0.0.0.0', _port)
        self.sock.bind(server_address)

        # Start listening for connections
        self.listen_thread = threading.Thread(target=self.__callbackListen)
        self.listen_thread.start()

    """ Basic destructor
    """
    def __del__(self):
        self.run = False

    """ Send images to subscribers data to publisher. 
        Images should be in cv2 format.
    """
    def publish(self, _image, _compression=50):
        # Encode image
        encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), _compression]
        result, encoded_image = cv2.imencode('.jpg', _image, encode_param)

        # Calculate packets
        nPackets = int (len(encoded_image)/ImageDataPacket().PACKET_SIZE + 1)
        
        encoded_data = encoded_image.transpose().tolist()

        # Publish packets to subscribers
        self.guard_list.acquire()
        for p_idx in range(nPackets):
            # Create packet idx
            packet = ImageDataPacket()
            packet.timeStamp = 0.0
            packet.packetId = p_idx
            packet.isFirst = (p_idx == 0)
            packet.numPackets = nPackets
            packet.totalSize = len(encoded_image)

            endSize = ImageDataPacket().PACKET_SIZE
            if(p_idx == nPackets-1):
                endSize = len(encoded_image) % ImageDataPacket().PACKET_SIZE

            packet.packetSize = endSize
            packet.buffer[0:endSize] = encoded_data[0][ImageDataPacket().PACKET_SIZE*p_idx:ImageDataPacket().PACKET_SIZE*p_idx+endSize]


            # Pack data
            bytesPack = struct.pack(    "i?iiii"+str(packet.PACKET_SIZE)+"B",  
                                        packet.PACKET_SIZE,
                                        packet.isFirst,
                                        packet.packetId,
                                        packet.numPackets,
                                        packet.totalSize,
                                        packet.packetSize,
                                        *packet.buffer
                                    )

            for add in self.client_list:
                sent = self.sock.sendto(bytesPack, add)

        self.guard_list.release()

    def __callbackListen(self):
        while self.run:
            print("Waiting for connection")
            data, address = self.sock.recvfrom(1)
            print("Received new connection from: ", address)
            self.guard_list.acquire()
            self.client_list.append(address)
            self.guard_list.release()

