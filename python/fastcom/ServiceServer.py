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
import fastcom.fastcom_version
import struct
import ctypes

""" Subscriber class.
This class that receives data from publishers
"""
class ServiceServer:
    """ Basic initializer
    """
    def __init__(self, _port, _callback):
        self.port_ = _port
        self.acceptingConnections_ = True
        self.threadVector = []
        self.acceptThread = threading.Thread(target = self.__acceptThread)
        self.acceptThread.start()

    def __threadFunction(self, _conn):
        # Send boost version to check compatibility.
        _conn.send(fastcom.fastcom_version.FASTCOM_VERSION)
        _conn.send("\n")
        
        # Receive hash code of class.
        hashcodeData = self.__receiveBytes(_conn, 4)
        hashcode = struct.unpack("Q", hashcodeData)
        print("received hash")
        if(hashcode == 0):
            # Good request type, send query for data.
            _conn.send("good_request\n")
            print("good request")

            # Get request data
            # request = receiveType<RequestType_>(_client);

            # process response
            # ResponseType_ response;
            # callback_(request, response);

            # Send response
            # const char * responsePtr = reinterpret_cast<const char*>(&response);
            # auto len = boost::asio::write(*_client, boost::asio::buffer(responsePtr, sizeof(ResponseType_)) );
            # std::cout << "sent: " << len << " bytes\n";
            # std::cout << int(responsePtr[0]) << int(responsePtr[1]) << int(responsePtr[2]) << int(responsePtr[3]) <<std::endl;
            # Wait to finish
            # boost::asio::streambuf sb;
            # boost::asio::read_until(*_client, sb, "\n");
            # std::string finalAnswer = boost::asio::buffer_cast<const char*>(sb.data());
        else:
            # Bad request type, send error msg.
            print("\033[1;31m [ERROR]    Hash code of request is not the same than the expected. \033[0m\n");
            _conn.send("bad_request_type\n")
        
        _conn.close()

    def __acceptThread(self):
        self.serverSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.serverSock.bind(("0.0.0.0", self.port_))
        self.serverSock.listen()
        while self.acceptingConnections_:
            conn, addr = self.serverSock.accept()

            thread = threading.Thread(target = self.__threadFunction, args=(conn,))
            thread.start()
            thread.join()   # 666 make it asyncronous
        

    def __receiveUntil(self, _socket, _endChar):
        hasFinished = False
        string = ""
        while not hasFinished:
            chunk = _socket.recv(1)
            if chunk == '\n':
                hasFinished = True
            else:
                string = string+chunk
        return string

    def __receiveBytes(self, _socket, _bytes):
        data = b''
        total = 0
        while total < _bytes:
            chunk = _socket.recv(1)
            data = data+chunk
            total = total + len(chunk)
            return data