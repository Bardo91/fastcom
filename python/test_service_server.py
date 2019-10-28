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

import fastcom.ServiceServer
import struct
import ctypes
import time 
import sys

class RequestInt:
    a = 1
    def pack(self):
        return struct.pack("i", self.a)

    def size(self):
        return ctypes.sizeof(ctypes.c_int)


class ResponseInt:
    a = 1
    def unpack(self, data):
        self.a = struct.unpack("i", data)
    
    def size(self):
        return ctypes.sizeof(ctypes.c_int)

def serverCallback(_req, _res):
    _res.a = _req.a
    return True

server = fastcom.ServiceServer.ServiceServer(int(sys.argv[1]), serverCallback)

while True:
    time.sleep(1)