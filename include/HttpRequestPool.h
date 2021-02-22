#pragma once

#include <queue>
#include <vector>

#include <Poco/ThreadPool.h>

#include "HttpRequestPoolRunnable.h"

class  HttpRequestPool {
    
    
    public:

        HttpRequestPool(const std::queue<std::string> &reqUrl, const int &concurrentRequestLimit = 5);
        HttpRequestPool(const std::vector<std::string> &reqUrl, const int &concurrentRequestLimit = 5);

        ~HttpRequestPool();

        void addRequestUrl(const std::string &url);

        void start();

        void join();

        int getConcurrentRequestLimit() const;

    private:

        std::queue<std::string> reqUrls;
        std::vector<HttpRequestPoolRunnable> runnableObjs;
        Poco::ThreadPool pool;

        HttpRequestPool();

};