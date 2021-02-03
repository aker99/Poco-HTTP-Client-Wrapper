#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>

#include "CustomHttpClient.h"

using namespace Poco::Net;

bool CustomHttpClient::rcForRedirectionCheck(short rc) {
  return rc == HTTPResponse::HTTP_MOVED_PERMANENTLY || rc == HTTPResponse::HTTP_FOUND || rc == HTTPResponse::HTTP_TEMPORARY_REDIRECT || rc == HTTPResponse::HTTP_PERMANENT_REDIRECT;
}

CustomHttpClient::CustomHttpClient(std::string hostName, const short portNo)
 : host_names(hostName), port(portNo), session(hostName, port) {}

CustomHttpClient::~CustomHttpClient() {}

const HTTPResponse CustomHttpClient::request(const std::string path, const std::string req_type,const int redirection_limit) {
  HTTPRequest obj(req_type, path);
  HTTPResponse response;
  this->session.sendRequest(obj);
  this->session.receiveResponse(response);
  short rc = response.getStatus();
  if (rc < 300) {
    return response;
  }
  if (rcForRedirectionCheck(rc)) {
    if (redirection_limit > -1) {
      const char* key = "location";
      if (response.has(key)) {
        const std::string location = response.get(key);
        const Poco::URI uri(location);
        const std::string host = uri.getHost();
        const short port = uri.getPort();
        std::cout<<"Redirecting to Location: "<<location<<std::endl;
        
        if (this->port == port && this->host_names.compare(host)) {
          return this->request(uri.getPathAndQuery(), req_type, redirection_limit - 1);
        }
        CustomHttpClient obj(host, port);
        return obj.request(uri.getPathAndQuery(), req_type, redirection_limit - 1);
      } else throw "Response requsted for redirection (rc = " + std::to_string(rc) + ")! But redirection location not provided";
    } else throw "Redirection limit exceeded!"; 
  }
}

// const HTTPResponse printResponseData(const HTTPResponse res) {
//   thi
// }