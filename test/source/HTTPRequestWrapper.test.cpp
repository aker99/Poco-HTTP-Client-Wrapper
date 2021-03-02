//
//  test-file.cpp
//  poco-practice-program
//
//  Created by Bhaskar Mahajan on 25/02/21.
//

#include "test-file.hpp"
#include <gtest/gtest.h>


#include "CustomHttpSessionFactory.h"
#include "HTTPRequestWrapper.h"
#include <Poco/URI.h>
#include <Poco/Exception.h>


//Logging functions should not be called while testing HTTP get function as getThreadID function will not run.
TEST(HTTPRequestTest, WorkingURLS) {
    CustomHTTPSessionFactory::registerHTTP();
    CustomHTTPSessionFactory::registerHTTPS();
    
    EXPECT_EQ ("OK", HTTPRequestWrapper::get("google.com").header.getReason());
    EXPECT_EQ ("OK", HTTPRequestWrapper::get(Poco::URI("https://example.com")).header.getReason());
}

TEST(HTTPRequestTest, NotWorkingURLS){
    CustomHTTPSessionFactory::registerHTTP();
    CustomHTTPSessionFactory::registerHTTPS();
    
    try {
        ASSERT_EQ ("OK", HTTPRequestWrapper::get("askgfkas.com").header.getReason());
    } catch (Poco::Exception& excp) {
        EXPECT_EQ ("Host not found", (std::string)excp.name());
    }
    
    try {
        ASSERT_EQ ("OK", HTTPRequestWrapper::get("ftp://google.com").header.getReason());
    } catch (Poco::Exception& excp) {
        EXPECT_EQ ("Host not found", (std::string)excp.name());
    }
    catch(const char* excp){
        EXPECT_EQ ("URLParser::parser -> error in finding Host and Port", excp);
    }
   
}


void testingOn(){
    
   
    ::testing::InitGoogleTest();
//    ::testing::GTEST_FLAG(filter) = "*WorkingURLS";
    RUN_ALL_TESTS();
    std::cout<<"Hello World\n";
    exit(0);
}
