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
import fastcom.ImagePublisher
import struct
import numpy
from PIL import Image
import io

""" Subscriber class.
This class that receives images from image publishers
"""
class ImageSubscriber:
    """ Basic initializer
    """
    def __init__(self, _host, _port):
        # Some init variables
        self.run = True
        self.guard_list = threading.Lock()
        self.callbacks_list = []

        # Create socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_address = (_host, _port)
        self.sock.bind(self.server_address)
        
        # Start listening for publisher to call callbacks
        self.listen_thread = threading.Thread(target=self.__callbackListen)
        self.listen_thread.start()

    """ Basic destructor
    """
    def __del__(self):
        self.run = False

    """ Append a custom callback to be run when an image is received. 
        Input is expected to be a numpy image
    """
    def appendCallback(self, callback):
        self.guard_list.acquire()
        self.callbacks_list.append(callback)
        self.guard_list.release()

    def __callbackListen(self):
        while True:
            isFirstPacket = True
            currPacketId = -1
            dataBuffer = numpy.array([],dtype=numpy.uint8)
            while True:
                # Get and decode data
                expectedRecvSize = struct.calcsize("i?iiii"+str(fastcom.ImagePublisher.ImageDataPacket().PACKET_SIZE)+"B")
                data, address = self.sock.recvfrom(expectedRecvSize)
                recvData = struct.unpack("i?iiii"+str(fastcom.ImagePublisher.ImageDataPacket().PACKET_SIZE)+"B", data)
                recvPacket = fastcom.ImagePublisher.ImageDataPacket()
                recvPacket.PACKET_SIZE = recvData[0]
                recvPacket.isFirst = recvData[1]
                recvPacket.packetId = recvData[2]
                recvPacket.numPackets = recvData[3]
                recvPacket.totalSize = recvData[4]
                recvPacket.packetSize = recvData[5]
                recvPacket.buffer =  recvData[6:6+recvPacket.packetSize]

                # Check State of transmission
                if isFirstPacket == recvPacket.isFirst:
                    isFirstPacket = False
                    # Check id packet
                    if(recvPacket.packetId == currPacketId+1):
                        dataBuffer = numpy.append(dataBuffer, recvPacket.buffer)
                        currPacketId = currPacketId+1
                        # If last packet
                        if(recvPacket.packetId == recvPacket.numPackets-1):
                            if(len(dataBuffer) == recvPacket.totalSize):
                                # Decode img
                                ioBuffer = io.BytesIO(numpy.uint8(dataBuffer).tobytes())
                                ioBuffer.seek(0)
                                decodedImg = Image.open(ioBuffer)
                                # Call callbacks with img
                                self.guard_list.acquire()
                                for call in self.callbacks_list:
                                    call(numpy.array(decodedImg))
                                self.guard_list.release()

                                # Restore default variables
                                isFirstPacket = True
                                currPacketId = -1
                                dataBuffer = []
