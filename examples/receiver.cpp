//---------------------------------------------------------------------------------------------------------------------
//  FASTCOM
//---------------------------------------------------------------------------------------------------------------------
//  Copyright 2020 -    Manuel Perez Jimenez (a.k.a. manuoso)
//                      Marco A. Montes Grova (a.k.a. mgrova) 
//                      Pablo Ramon Soria (a.k.a. Bardo91)
//                      Ricardo Lopez Lopez (a.k.a. ric92)
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


#include <fastcom/Publisher.h>
#include <fastcom/Subscriber.h>

#include <fastcom/ConnectionManager.h>

#include <string>
#include <thread>
#include <chrono>
#include <iostream>

#include "SerializableVector.h"

int main(){

    fastcom::Subscriber<float> s1("/pepe");
    fastcom::Subscriber<std::string> s2("/jojo");
    fastcom::Subscriber<SerializableVector<float>> s3("/custom");
    
    s1.addCallback([&](const int &_msg){
        std::cout << _msg << std::endl;
    });

    s2.addCallback([&](const std::string &_msg){
        std::cout << _msg << std::endl;
    });

    s3.addCallback([&](const SerializableVector<float> &_msg){
        for(auto elem: _msg)
            std::cout << elem << ", "<< std::endl;
    });


    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    

}