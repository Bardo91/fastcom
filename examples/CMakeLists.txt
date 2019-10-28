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

add_executable(example_sender "sender.cpp")
target_include_directories(example_sender PRIVATE "../include")
target_link_libraries(example_sender LINK_PUBLIC fastcom)

add_executable(example_receiver "receiver.cpp")
target_include_directories(example_receiver PRIVATE "../include")
target_link_libraries(example_receiver LINK_PUBLIC fastcom)


add_executable(example_sender_image "sender_image.cpp")
target_include_directories(example_sender_image PRIVATE "../include")
target_link_libraries(example_sender_image LINK_PUBLIC fastcom)

add_executable(example_receiver_image "receiver_image.cpp")
target_include_directories(example_receiver_image PRIVATE "../include")
target_link_libraries(example_receiver_image LINK_PUBLIC fastcom)

add_executable(example_destruction "destruction.cpp")
target_include_directories(example_destruction PRIVATE "../include")
target_link_libraries(example_destruction LINK_PUBLIC fastcom)


add_executable(example_services "example_services.cpp")
target_include_directories(example_services PRIVATE "../include")
target_link_libraries(example_services LINK_PUBLIC fastcom)