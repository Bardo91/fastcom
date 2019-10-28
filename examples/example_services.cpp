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


#include <fastcom/ServiceServer.h>
#include <fastcom/ServiceClient.h>

struct RequestInt{
    SERVICE_MESSAGE_TYPE
    int a;
};

struct ResponseInt{
    SERVICE_MESSAGE_TYPE
    int a;
};

int main(void){
    ResponseInt res;
    std::cout << res.type() << std::endl;
    fastcom::ServiceServer<RequestInt, ResponseInt> server(9999, [&](RequestInt &_req, ResponseInt &_res){
            std::cout <<"Received call: " << _req.a <<std::endl;
            _res.a = _req.a+1;
    });


    fastcom::ServiceClient<RequestInt, ResponseInt> client("0.0.0.0", 9999);

    std::cout << "sending calls" << std::endl;
    RequestInt req;
    for( unsigned i = 0; i < 10;i++){
        req.a = i*4;
        auto res = client.call(req);
        std::cout << "Response: " << res.a << std::endl;
    }

    std::cout << "Finished calls" << std::endl;
}