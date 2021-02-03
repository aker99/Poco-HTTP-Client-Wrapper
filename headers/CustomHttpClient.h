#pragma once

// #include <string>

#include <Poco/Net/HTTPClientSession.h>

using namespace Poco::Net;

class CustomHttpClient
{
private:
    const std::string host_names;
    const short port;
    HTTPClientSession session; 

    static const std::vector<int> rcForRedirection;
    static bool rcForRedirectionCheck(const short rc);

public:
    CustomHttpClient(std::string host_name, const short portNo = 80);
    ~CustomHttpClient();

    const HTTPResponse request(std::string path, const std::string req_type, int redirection_limit = 0);

    const HTTPResponse printResponseData(const HTTPResponse res);
};
