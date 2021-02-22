#include<string>

#include <Poco/Logger.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSessionFactory.h>

#include "URLParser.h"
#include "HTTPSession.h"

#include "LoggerUtil.h"

using namespace Poco;
using namespace Poco::Net;

static const bool rcForRedirectionCheck(short rc) {
  return rc == HTTPResponse::HTTP_MOVED_PERMANENTLY || rc == HTTPResponse::HTTP_FOUND || rc == HTTPResponse::HTTP_TEMPORARY_REDIRECT || rc == HTTPResponse::HTTP_PERMANENT_REDIRECT;
}


static const HTTP_Session::CustomHttpResponse getRequest(URI prevURI)
{
    //GET request
    Logger& fileLogger =  LoggerUtil::getCurrentThreadLogger();
    // Global default HTTPSession Factory
    HTTPSessionFactory &defaultFactory = HTTPSessionFactory::defaultFactory();
    
    // Poco request and response objects
    HTTPRequest request(HTTPMessage::HTTP_1_1);
    // HTTPResponse response;
    
    // Custom HTTP Struct
    HTTP_Session::CustomHttpResponse finalResponse;
    
    // Stream to hold data of response body
    std::stringstream body_stream;
    
    // currentURI
    URI currentURI = prevURI;
    
    // Starting new Session
    HTTPClientSession *session = defaultFactory.createClientSession(currentURI);
    
    // Flag to avoid redirection to same URL
    bool notFirstRequest = false;
    
    while(1)
    {   
        fileLogger.information("Requesting: " + currentURI.toString());
        // Scheme changes so create new session
        if(prevURI.getScheme() != currentURI.getScheme())
        {
            session->reset();
            session = defaultFactory.createClientSession(currentURI);
        }
        // Host or port changes
        else if(session->getHost() != currentURI.getHost() || session->getPort() != currentURI.getPort())
        {
            session->reset();
            session->setHost(currentURI.getHost());
            session->setPort(currentURI.getPort());
        }
        
        prevURI = currentURI;
        request.setURI(currentURI.toString());
        session->sendRequest(request);
        std::istream& result = session->receiveResponse(finalResponse.header);
        if(finalResponse.header.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED)
        {
            // Unauthorized response
            throw ApplicationException("401 Unauthorized");
        }
        else if(rcForRedirectionCheck(finalResponse.header.getStatus()))
        {
            // 300/301/307/308 response header
            if(finalResponse.header.has("Location"))
            {
                currentURI.resolve(finalResponse.header.get("Location"));
                
                if(notFirstRequest)
                {
                    if(currentURI == prevURI)
                    {
                        // Redirect to prevURI
                        throw ApplicationException("Redirected to the same previous URL");
                    }
                }
                else
                {
                    notFirstRequest = true;
                }
            }
            else
            {
                // No redirection url provided in response header
                throw ApplicationException("Redirect Location was not provided in the response header");
            }
            
        }
        else if(finalResponse.header.getStatus() == HTTPResponse::HTTP_OK)
        {
            // Final response
            StreamCopier::copyStream(result, body_stream);
            finalResponse.body = body_stream.str();
            return finalResponse;
        }
        else
        {
            // Rest of response codes not implemented
            throw ApplicationException("Status Code method not implemented yet");
        }
    }
    
}

const HTTP_Session::CustomHttpResponse HTTP_Session::sendAndReceive(std::string url)
{
    // URL parsing
    URLParser url_(url);
    URI uri(url_.beautifyURL());
    return getRequest(uri);
}





