#include <Poco/Thread.h>

#include "HttpRequestPool.h"
#include "HttpRequestPoolRunnable.h"
#include "Utils/Thread.h"

HttpRequestPool::HttpRequestPool(const std::queue<std::string> &reqUrl, const int &concurrentRequestLimit) : 
    reqUrls(reqUrl), pool(1, concurrentRequestLimit) {
        runnableObjs.reserve(reqUrl.size());
    }

HttpRequestPool::HttpRequestPool(const std::vector<std::string> &reqUrl, const int &concurrentRequestLimit) : 
    pool(1, concurrentRequestLimit) {
        runnableObjs.reserve(reqUrl.size());
        for (const auto &i : reqUrl)
        {
            reqUrls.push(i);
        }
    }

HttpRequestPool::~HttpRequestPool() {}

void HttpRequestPool::start() {

    while (!reqUrls.empty())
    {
        // HttpRequestPoolRunnable r(reqUrls.front());
        if (pool.available()) {
            runnableObjs.__emplace_back(reqUrls.front());
            pool.start(runnableObjs.back());
            reqUrls.pop();
        } else ThreadUtil::sleep(1000);
    }
}

void HttpRequestPool::join() {
    pool.joinAll();
}

int HttpRequestPool::getConcurrentRequestLimit() const {
    return pool.capacity();
}
