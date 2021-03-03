#pragma once

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/URI.h>

namespace CustomHTTPSessionFactory {
    
    void registerHTTP();
    void registerHTTPS();

    void setProxy(const std::string &host, const short port);
    void setProxyCredentials(const std::string &username, const std::string &password);
    void setProxy(const std::string &host, const short port, const std::string &username, const std::string &password);

    Poco::Net::HTTPClientSession* createSession(const Poco::URI &uri);
}