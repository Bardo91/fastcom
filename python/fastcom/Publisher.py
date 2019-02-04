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

""" Publisher class.
This class communicates data to subscribers
"""
class Publisher:
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

    """ Publish data to publisher. 
        Data is an array of bytes to be sent.
    """
    def publish(self, data):
        self.guard_list.acquire()
        for add in self.client_list:
            sent = self.sock.sendto(data, add)
        self.guard_list.release()

    def __callbackListen(self):
        while self.run:
            print("Waiting for connection")
            data, address = self.sock.recvfrom(1)
            print("Received new connection from: ", address)
            self.sock.sendto(b'1', address)
            self.guard_list.acquire()
            addCon = True
            for addr in self.client_list:
                if addr == address:
                    addCon = False
            if addCon:
                self.client_list.append(address)
            self.guard_list.release()

