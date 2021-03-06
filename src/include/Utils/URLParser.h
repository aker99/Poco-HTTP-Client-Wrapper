#pragma once

#include <string>

#define DEFAULT_PROTOCOL "http"

#define HTTP_ "http"
#define HTTPS_ "https"

#define HTTP_PORT_ 80
#define HTTPS_PORT_ 443 

// namespace Util {

class URLParser {
    // Parse HTTP and HTTPS urls only.
    // Parse into protocol, host, port and path.
    // if protocol not specified in url, DEFAULT_PROTOCOL is used.
    // if port not specified in url, DEFAULT_PORT is used.
    public:
        URLParser(const std::string& url);
        ~URLParser();

        std::string getProtocol();
        std::string getHost();
        uint16_t getPort();
        std::string getPath();
        std::string beautifyURL();


    private:

        void parser();

        const std::string url;
        std::string protocol;
        std::string host;
        uint16_t port;
        std::string path;
};

// }