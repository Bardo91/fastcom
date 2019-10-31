//---------------------------------------------------------------------------------------------------------------------
//  FASTCOM
//---------------------------------------------------------------------------------------------------------------------
//  Copyright 2019 - Pablo Ramon Soria (a.k.a. Bardo91) 
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

#include <fastcom/fastcom.h>

#include <thread>
#include <iostream>
#include <chrono>



int main(int _argc, char **_argv){

    fastcom::Publisher<int> publisher1(8889);
    fastcom::Subscriber<int> subscriber1("127.0.0.1", 8889);
    while(!subscriber1.isConnected()){
        std::this_thread::sleep_for(std::chrono::milliseconds(30));   
    }
 
    int expectedValue1 = 5;
    subscriber1.attachCallback([&](int &_data){
        std::cout<<_data << std::endl;
    });

    publisher1.publish(expectedValue1);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));     


    fastcom::Publisher<std::string> publisher2(8887);
    fastcom::Subscriber<std::string> subscriber2("127.0.0.1", 8887);
    while(!subscriber2.isConnected()){
        std::this_thread::sleep_for(std::chrono::milliseconds(30));   
    }
 
    std::string expectedValue2 = "YEAH BITCH";
    subscriber2.attachCallback([&](std::string &_data){
        std::cout<<_data << std::endl;
    });

    publisher2.publish(expectedValue2);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));     


    // fastcom::Publisher<std::vector<int>> publisher(8888);
    // fastcom::Subscriber<std::vector<int>> subscriber("127.0.0.1", 8888);
    // while(!subscriber.isConnected()){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(30));   
    // }
 
    // std::vector<int> expectedValue = {1,2,3,4};
    // subscriber.attachCallback([&](std::vector<int> &_data){
    //     std::cout<<_data[0] << std::endl;
    //     std::cout<<_data[1] << std::endl;
    //     std::cout<<_data[2] << std::endl;
    //     std::cout<<_data[3] << std::endl;
    // });

    // publisher.publish(expectedValue);

    // std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    

}