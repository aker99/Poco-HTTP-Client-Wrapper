#include <gtest/gtest.h>
#include "PocoCurlApp.h"


//Handling URLs
TEST(URLTest, HandleValidURLs){
    PocoCurlApp app;
    app.setHandleURLs("-u", "http://google.com https://google.com google.com 127.0.0.1");
    EXPECT_EQ(app.getValidURLSize(), 4);
}

TEST(URLTest, HandleInvalidURLs){
    PocoCurlApp app;
    app.setHandleURLs("-u", "ftp://google.com https:/google.com google.com 127.0.0.1.90.80");
    EXPECT_EQ(app.getInvalidURLSize(), 3);
}



//Handling Proxy
TEST(ProxyTest, HandleValidProxy){
    PocoCurlApp app;
    app.setHandleProxy("-p", "gateway.zscalerbeta.net:443");
    EXPECT_EQ(app.isProxySet(), true);
}

TEST(ProxyTest, HandleInvalidProxy){
    PocoCurlApp app;
    app.setHandleProxy("-p", "xyz??123");
    EXPECT_EQ(app.isProxySet(), false);
}



//Handling Proxy Credentials
TEST(ProxyCredTest, HandleValidCred){
    PocoCurlApp app;
    app.setHandleProxyCred("-c", "Shubham 123");
    EXPECT_EQ(app.isProxyCredSet(), true);
}

TEST(ProxyCredTest, HandleInvalidCred){
    PocoCurlApp app;
    app.setHandleProxyCred("-c", "Shubham");
    EXPECT_EQ(app.isProxyCredSet(), false);
}



//Testing If Credentials are given then Proxy is given or not
TEST(ProxyAndCredTest, HandleProxyGiven){
    PocoCurlApp app;
    app.setHandleProxy("-p", "gateway.zscalerbeta.net:443");
    app.setHandleProxyCred("-c", "Shubham 123");
    EXPECT_EQ((app.isProxyCredSet() && app.isProxySet()), true);
}

TEST(ProxyAndCredTest, HandleProxyNotGiven){
    PocoCurlApp app;
    app.setHandleProxyCred("-c", "Shubham 123");
    EXPECT_EQ((app.isProxyCredSet() && app.isProxySet()), false);
}