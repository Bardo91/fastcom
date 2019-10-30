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

#include <gtest/gtest.h>

#include <atomic>
#include <thread>
#include <chrono>

TEST(FloatTest, FloatTest)  {
    struct floatStruct{
        float data;
    };

    fastcom::Publisher<floatStruct> publisher(8888);
    fastcom::Subscriber<floatStruct> subscriber("127.0.0.1", 8888);

    std::atomic<bool> flagRecv(false); 
    float expectedValue = 1.61803398875f;
    subscriber.attachCallback([&](floatStruct &_data){
        std::cout << _data.data << std::endl;
        ASSERT_EQ(_data.data, expectedValue);
        flagRecv = true;
    });

    floatStruct msg;
    msg.data = expectedValue;
        std::cout <<
        "----" << msg.data << std::endl;
    publisher.publish(msg);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));   
    ASSERT_TRUE(flagRecv);

    // ASSERT_EQ(-1.0, squareRoot(-15.0));
    // ASSERT_EQ(-1.0, squareRoot(-0.2));
}

TEST(StringTest, StringTest)  {

}
 
int main(int _argc, char **_argv)  {
    testing::InitGoogleTest(&_argc, _argv);
    return RUN_ALL_TESTS();
}