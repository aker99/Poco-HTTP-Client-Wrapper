#pragma once

#include <Poco/URI.h>
#include <Poco/Net/HTTPResponse.h>

namespace HTTPRequestWrapper
{
    // Custom Response Struct
    struct CustomHttpResponse
    {
        Poco::Net::HTTPResponse header;
        std::string body;
    };
    
    const CustomHttpResponse get(Poco::URI url);
    const CustomHttpResponse get(std::string &url);
    const bool isRedirectionStatus(Poco::Net::HTTPResponse::HTTPStatus status);
}




