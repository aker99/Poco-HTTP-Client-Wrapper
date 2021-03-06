#pragma once

#include <Poco/Runnable.h>

class HttpRequestPoolRunnable: public Poco::Runnable{
    virtual void run();

    public: 

        HttpRequestPoolRunnable(const std::string& url);
        // HttpRequestPoolRunnable(HttpRequestPool& ref);
        ~HttpRequestPoolRunnable();

    private:
        const std::string url;
        // HttpRequestPool& pRef;
};