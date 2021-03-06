#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/HTTPSessionFactory.h>
#include <Poco/Net/HTTPSessionInstantiator.h>
#include <Poco/Net/HTTPSSessionInstantiator.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Util/AbstractConfiguration.h>

#include "CustomHttpSessionFactory.h"

using namespace Poco::Net;

void CustomHTTPSessionFactory::registerHTTP() {
    HTTPSessionFactory::defaultFactory().registerProtocol("http", new HTTPSessionInstantiator);
}

void CustomHTTPSessionFactory::registerHTTPS() {

		HTTPSessionFactory::defaultFactory().registerProtocol("https", new HTTPSSessionInstantiator);
    Poco::SharedPtr<InvalidCertificateHandler> ptrCert = new AcceptCertificateHandler(true);
    Context::Ptr context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_STRICT, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    SSLManager::instance().initializeClient(0, ptrCert, context);
}

void CustomHTTPSessionFactory::setProxy(const std::string& host, const short port) {
    // Poco::UInt16 proxyPort = static_cast<Poco::UInt16>(port);
    HTTPSessionFactory::defaultFactory().setProxy(host, port);
}

void CustomHTTPSessionFactory::setProxyCredentials(const std::string& username, const std::string& password) {
    HTTPSessionFactory::defaultFactory().setProxyCredentials(username, password);
}

void CustomHTTPSessionFactory::setProxy(const std::string& host, const short port, const std::string& username, const std::string& password) {
    CustomHTTPSessionFactory::setProxy(host, port);
    CustomHTTPSessionFactory::setProxyCredentials(host, password);
}

HTTPClientSession* CustomHTTPSessionFactory::createSession(const Poco::URI& uri){
    return HTTPSessionFactory::defaultFactory().createClientSession(uri);
}