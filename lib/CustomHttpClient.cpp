#include <string>
#include <sstream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>

#include "CustomDef.h"
#include "CustomHttpClient.h"

using namespace Poco::Net;

// Static Methods

bool CustomHttpClient::rcForRedirectionCheck(short rc) {
  return rc == HTTPResponse::HTTP_MOVED_PERMANENTLY || rc == HTTPResponse::HTTP_FOUND || rc == HTTPResponse::HTTP_TEMPORARY_REDIRECT || rc == HTTPResponse::HTTP_PERMANENT_REDIRECT;
}

// Constructors
CustomHttpClient::CustomHttpClient(String hostName, const short portNo)
 : host_names(hostName), port(portNo), session(hostName, port) {}

CustomHttpClient::~CustomHttpClient() {}

// Methods

const CustomHttpResponse CustomHttpClient::request(const String path, const String req_type,const int redirection_limit) {
  HTTPRequest obj(req_type, path);
  HTTPResponse response;

  this->session.sendRequest(obj);
  std::istream& bodyStream = this->session.receiveResponse(response);

  std::stringstream s;
  Poco::StreamCopier::copyStream(bodyStream, s);

  short rc = response.getStatus();
  if (rcForRedirectionCheck(rc)) {
    if (redirection_limit > -1) {
      const char* key = "location";
      if (response.has(key)) {
        const String location = response.get(key);
        const Poco::URI uri(location);
        const String host = uri.getHost();
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
  CustomHttpResponse res;
  res.header = response;
  res.body = s.str();
  return res;
}

// const String printResponseData(const HTTPResponse res) {
//   std:: stringstream body;
//         istream& result = session.receiveResponse(response);
// }