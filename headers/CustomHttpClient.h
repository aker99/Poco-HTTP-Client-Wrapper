#pragma once

#include <Poco/Net/HTTPClientSession.h>

#include "CustomDef.h"

using namespace Poco::Net;

struct CustomHttpResponse {
    HTTPResponse header;
    String body;
};

class CustomHttpClient {
private:
    const String host_names;
    const short port;
    HTTPClientSession session; 

    static const std::vector<int> rcForRedirection;
    static bool rcForRedirectionCheck(const short rc);

public:
    CustomHttpClient(String host_name, const short portNo = 80);
    ~CustomHttpClient();

    const CustomHttpResponse request(String path, const String req_type, int redirection_limit = 0);
};
