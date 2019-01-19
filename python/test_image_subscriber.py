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

import time
import struct
import sys
# internal trick by now....
for idx, dir in enumerate(sys.path):
    if "ros" in dir:
        sys.path[idx] = "/home/bardo91/.local/lib/python3.5/site-packages"



import ImageSubscriber
import cv2

pub = ImageSubscriber.ImageSubscriber("0.0.0.0", 8888)

def showImage(_image):
    cv2.imshow("image", _image)
    cv2.waitKey(3)

pub.appendCallback(showImage)

while True:
    time.sleep(1.0)