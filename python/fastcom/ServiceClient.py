#!/usr/bin/env python3

##---------------------------------------------------------------------------------------------------------------------
##  FASTCOM
##---------------------------------------------------------------------------------------------------------------------
##  Copyright 2019 - Pablo Ramon Soria (a.k.a. Bardo91) 
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
import fastcom.fastcom_version as fastcom_version
import struct
import ctypes

""" Subscriber class.
This class that receives data from publishers
"""
class ServiceClient:
    """ Basic initializer
    """
    def __init__(self, _host, _port):
        self.host_ = _host
        self.port_ = _port

    def call(self, _req, _res):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((self.host_, self.port_))

        # Receive fastcom version
        hasFinished = False
        fastcomVersion = self.__receiveUntil(sock, '\n')
        
        if fastcomVersion == fastcom_version.FASTCOM_VERSION:
            # Sending hash code of request
            sock.send(struct.pack("Q",0)) # 666 TODO solve hash

            # Check if request type is ok
            typeResponse = self.__receiveUntil(sock, '\n')
            if(typeResponse == "good_request"):
                # Send request
                sock.send(_req.pack())

                #receiveResponse
                data = self.__receiveBytes(sock, _res.size())
                _res.unpack(data)

                sock.send(b"end\n")
            else:
                print("\033[1;31m [ERROR]    Hash code of request inconsistent with server version\033[0m\n")
            

        else:
            print("\033[1;31m [ERROR]    Inconsistent fastcom version. Expected "+fastcom_version.FASTCOM_VERSION+" but received "+fastcomVersion+"\033[0m\n")

    def __receiveUntil(self, _socket, _endChar):
        hasFinished = False
        chunks = []
        while not hasFinished:
            chunk = _socket.recv(1)
            if chunk == b'\n':
                hasFinished = True
            else:
                chunks.append(chunk)
        return (b"".join(chunks)).decode("utf-8") 

    def __receiveBytes(self, _socket, _bytes):
        _socket.recv(1) # 666 Ignoring chunk that for some reason is affecting the python code and do not the c++ code
        return _socket.recv(_bytes)



