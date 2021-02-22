#include <iostream>
#include <string>

#include <Poco/Thread.h>

#include "HttpRequestPoolRunnable.h"
#include "HTTPSession.h"
#include "Utils/Logger.h"

using namespace Poco;

HttpRequestPoolRunnable::HttpRequestPoolRunnable(const std::string &url): url(url) {}
HttpRequestPoolRunnable::~HttpRequestPoolRunnable() {}

void  HttpRequestPoolRunnable::run(){
      try {
        
        // MARK: - Directly work here Himanshu
        Logger& fileLogger = LoggerUtil::create(std::to_string(Thread::current()->id()));
        const HTTP_Session::CustomHttpResponse obj = HTTP_Session::sendAndReceive(url);


        //Print Response Headers
        for(auto it = obj.header.begin(); it!= obj.header.end(); it++)
            std::cout<< it->first << " " << it->second << "\n";

        
        // //Print Body
        // std::cout<<"\n\n------------- Body --------------\n\n";
        // std::cout<<"\n"<<obj.body<<"\n";
    } catch (Poco::Exception& excp) {
        std::cerr<<"Some error occured --> "<<excp.displayText()<<std::endl;
        // exit(1);
    } catch(const char * err)
    {
        std::cout<<err;
    }
    
    Poco::Thread::sleep(1000);
}
