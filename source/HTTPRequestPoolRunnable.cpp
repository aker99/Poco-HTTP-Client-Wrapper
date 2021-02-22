// #include <iostream>
#include <string>
#include <chrono>

#include <Poco/Thread.h>

#include "HttpRequestPoolRunnable.h"
#include "HTTPRequestWrapper.h"
#include "Utils/Logger.h"

HttpRequestPoolRunnable::HttpRequestPoolRunnable(const std::string &url): url(url) {}
HttpRequestPoolRunnable::~HttpRequestPoolRunnable() {}

void  HttpRequestPoolRunnable::run() {

    Poco::Logger& fileLogger = Utility::Logger::createCurrentThreadLogger();

    try {
        typedef std::chrono::high_resolution_clock Clock;

        fileLogger.information("Requesting to URL: " + url);
        
        auto startTime = Clock::now();
        const HTTPRequestWrapper::CustomHttpResponse obj = HTTPRequestWrapper::get(url);
        auto endTime = Clock::now();

        auto duration = (endTime - startTime).count() / 1000000000.0;

        fileLogger.information("Response Status of URL: " + url + obj.header.getReason() + " | Total Time Taken: " + std::to_string(duration));
        //Print Response Headers
        // for(auto it = obj.header.begin(); it!= obj.header.end(); it++)
        //     std::cout<< it->first << " " << it->second << "\n";

        
        // //Print Body
        // std::cout<<"\n\n------------- Body --------------\n\n";
        // std::cout<<"\n"<<obj.body<<"\n";


    } catch (Poco::Exception& excp) {
        fileLogger.error("Request to URL: " + url + " returned with following error " + excp.displayText());
    } catch (std::exception ex) {
        fileLogger.error("Request to URL: " + url + " returned with following error " + ex.what());
    } catch(const char *err)
    {
        fileLogger.error("Request to URL: " + url + " returned with following error " + err);
    }
}
