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

TEST(IntTest, IntTest)  {

    fastcom::Publisher<int> publisher(8888);
    fastcom::Subscriber<int> subscriber("127.0.0.1", 8888);
    while(!subscriber.isConnected()){
        std::this_thread::sleep_for(std::chrono::milliseconds(30));   
    }
 
    int expectedValue = 1;
    subscriber.attachCallback([&](int &_data){
        ASSERT_EQ(_data, expectedValue);
        expectedValue++;
    });

    for(int msg = 1; msg < 10; msg++){
        publisher.publish(msg);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));   

}

TEST(FloatTest, FloatTest)  {

    fastcom::Publisher<float> publisher(8888);
    fastcom::Subscriber<float> subscriber("127.0.0.1", 8888);
    while(!subscriber.isConnected()){
        std::this_thread::sleep_for(std::chrono::milliseconds(30));   
    }
 
    float expectedValue = 1.0f;
    subscriber.attachCallback([&](float &_data){
        ASSERT_EQ(_data, expectedValue);
        expectedValue += 1.0f;
    });

    for(int msg = 1; msg < 10; msg = msg+1.0f){
        publisher.publish(msg);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));   

}

TEST(StringTest, StringTest)  {
    fastcom::Publisher<std::string> publisher(8888);
    fastcom::Subscriber<std::string> subscriber("127.0.0.1", 8888);
    while(!subscriber.isConnected()){
        std::this_thread::sleep_for(std::chrono::milliseconds(30));   
    }
 
    std::string expectedValue = "WORKING";
    subscriber.attachCallback([&](std::string &_data){
        ASSERT_STREQ("WORKING", _data.c_str());
    });

    publisher.publish(expectedValue);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));   
}


TEST(VectorTest, VectorTest)  {
    fastcom::Publisher<std::vector<int>> publisher(8888);
    fastcom::Subscriber<std::vector<int>> subscriber("127.0.0.1", 8888);
    while(!subscriber.isConnected()){
        std::this_thread::sleep_for(std::chrono::milliseconds(30));   
    }
 
    std::vector<int> expectedValue = {1,2,3,4};
    subscriber.attachCallback([&](std::vector<int> &_data){
        ASSERT_EQ(1, _data[0]);
        ASSERT_EQ(2, _data[1]);
        ASSERT_EQ(3, _data[2]);
        ASSERT_EQ(4, _data[3]);
    });

    publisher.publish(expectedValue);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));   
}
 
int main(int _argc, char **_argv)  {
    testing::InitGoogleTest(&_argc, _argv);
    return RUN_ALL_TESTS();
}