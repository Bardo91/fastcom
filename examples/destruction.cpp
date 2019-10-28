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

#include <fastcom/Publisher.h>
#include <fastcom/Subscriber.h>

#include <thread>
#include <iostream>
#include <chrono>

struct SimpleFloat{
    float a;
    float b;
    float c;
};

int main(){

    fastcom::Publisher<SimpleFloat> *publisher;
    fastcom::Subscriber<SimpleFloat> *subscriber;

    for(unsigned iter = 0; iter < 10; iter++){
		publisher = new fastcom::Publisher<SimpleFloat>(8888);
		subscriber = new fastcom::Subscriber<SimpleFloat>("0.0.0.0", 8888);

	    subscriber->attachCallback([&](SimpleFloat &_data){
		std::cout << _data.a << std::endl;
	    });

		
		std::this_thread::sleep_for(std::chrono::milliseconds(500));   
	    SimpleFloat data;
	    data.a = 0;
	    for(int i = 1;i<10;i++){
		std::this_thread::sleep_for(std::chrono::milliseconds(100));   
		publisher->publish(data);
		data.a +=1;
	    }

	    delete publisher;
	    delete subscriber;
	    std::cout << "start again" << std::endl;
    }
}
