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

""" Subscriber class.
This class that receives data from publishers
"""
class Subscriber:
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
        
        # Send one byte to notify publisher
        self.sock.sendto(b'1', self.server_address)

        # Start listening for publisher to call callbacks
        self.listen_thread = threading.Thread(target=self.__callbackListen)
        self.listen_thread.start()

    """ Basic destructor
    """
    def __del__(self):
        self.run = False

    """ Publish data to publisher. 
        Data is an array of bytes to be sent.
    """
    def appendCallback(self, callback):
        self.guard_list.acquire()
        self.callbacks_list.append(callback)
        self.guard_list.release()

    def __callbackListen(self):
        while True:
            data, address = self.sock.recvfrom(4096)
            self.guard_list.acquire()
            for call in self.callbacks_list:
                call(data)
            self.guard_list.release()
