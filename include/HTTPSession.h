#pragma once

#include "Poco/Net/HTTPResponse.h"

namespace HTTP_Session
{
    // Custom Response Struct
    struct CustomHttpResponse
    {
        Poco::Net::HTTPResponse header;
        std::string body;
    };
    // Returns result of get request in Custom Response Struct
    const CustomHttpResponse sendAndReceive(std::string url);
}




