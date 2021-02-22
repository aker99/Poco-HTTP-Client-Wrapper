#include <Poco/Thread.h>

#include "HttpRequestPool.h"
#include "HttpRequestPoolRunnable.h"
#include "Utils/Thread.h"

HttpRequestPool::HttpRequestPool() {
    runnableObjs.reserve(reqUrls.size());
}

HttpRequestPool::HttpRequestPool(const std::queue<std::string> &reqUrl, const int &concurrentRequestLimit) : 
    reqUrls(reqUrl), pool(1, concurrentRequestLimit) {
        HttpRequestPool();
    }

HttpRequestPool::HttpRequestPool(const std::vector<std::string> &reqUrl, const int &concurrentRequestLimit) : 
    pool(1, concurrentRequestLimit) {
        for (const auto &i : reqUrl)
        {
            reqUrls.push(i);
        }
        // HttpRequestPool();
    }

HttpRequestPool::~HttpRequestPool() {}

void HttpRequestPool::start() {

    while (!reqUrls.empty())
    {
        if (pool.available()) {
            runnableObjs.__emplace_back(reqUrls.front());
            pool.start(runnableObjs.back());
            reqUrls.pop();
        } else Utility::Thread::sleep(1000);
    }
}

void HttpRequestPool::join() {
    pool.joinAll();
}

int HttpRequestPool::getConcurrentRequestLimit() const {
    return pool.capacity();
}
