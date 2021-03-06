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

#include "CustomHttpSessionFactory.h"
#include "HTTPRequestWrapper.h"
#include "Utils/URLParser.h"
#include "Utils/Logger.h"

using namespace Poco;
// using namespace Poco::Net;

const bool HTTPRequestWrapper::isRedirectionStatus(Net::HTTPResponse::HTTPStatus rc) {
    using namespace Poco::Net;
    return rc == HTTPResponse::HTTP_MOVED_PERMANENTLY || rc == HTTPResponse::HTTP_FOUND || rc == HTTPResponse::HTTP_TEMPORARY_REDIRECT || rc == HTTPResponse::HTTP_PERMANENT_REDIRECT;
}

const HTTPRequestWrapper::CustomHttpResponse HTTPRequestWrapper::get(const URI& uriOrg, const std::string& loggerName)
{
    uint8_t redirectionLeft = HTTPRequestWrapper::maxRedirection;

    const URI* uri = &uriOrg;

    bool enableLogging = loggerName.length();
    Logger* fileLogger; 
    if (enableLogging) {
        fileLogger = &Utility::Logger::getLoggerByName(loggerName);
    }
    
    // Poco request and response objects
    Net::HTTPRequest request(Net::HTTPMessage::HTTP_1_1);
    // request.setURI(uri->toString());
    
    // Custom HTTP Struct
    HTTPRequestWrapper::CustomHttpResponse finalResponse;
    
    // Stream to hold data of response body
    // currentURI
    // URI currentURI = prevURI;
    
    // Starting new Session
    Net::HTTPClientSession* session = CustomHTTPSessionFactory::createSession(*uri);
    
    // Flag to avoid redirection to same URL
    bool notFirstRequest = false;
    
    while(redirectionLeft--) { 

        request.setURI(uri->toString());
        session->sendRequest(request);
        std::istream& resBody = session->receiveResponse(finalResponse.header);

        if(finalResponse.header.getStatus() == Net::HTTPResponse::HTTP_UNAUTHORIZED) {
            // Unauthorized response
            throw ApplicationException("401 Unauthorized");
        } else if(HTTPRequestWrapper::isRedirectionStatus(finalResponse.header.getStatus())) {
            // 300/301/307/308 response header
            if(finalResponse.header.has("Location")) {
                const Poco::URI* newUri = new Poco::URI(finalResponse.header.get("Location"));
                if(*uri == newUri) {
                    throw ApplicationException("Redirected to the same previous URL");
                } else {
                    if (!redirectionLeft) {
                        throw ApplicationException("No more redirection allowed");
                    }
                    if (enableLogging) {
                        fileLogger->information("Redirecting: " + uri->toString() + " --> " + newUri.toString());
                    }

                    // Scheme changes so create new session
                    if(uri->getScheme() != newUri.getScheme()) {
                        session->reset();
                        session =  CustomHTTPSessionFactory::createSession(newUri);
                    } else if(session->getHost() != newUri.getHost() || session->getPort() != newUri.getPort()) {
                        session->reset();
                        session->setHost(newUri.getHost());
                        session->setPort(newUri.getPort());
                    }
                    uri = (Poco::URI*)&newUri;
                }
            } else {
                // No redirection url provided in response header
                throw ApplicationException("Redirect Location was not provided in the response header");
            }
        } else if(finalResponse.header.getStatus() == Net::HTTPResponse::HTTP_OK) {
            // Final response
            std::stringstream bodyStream;
            StreamCopier::copyStream(resBody, bodyStream);
            finalResponse.body = bodyStream.str();
            return finalResponse;
        } else {
            // Rest of response codes not implemented
            throw ApplicationException("Status Code method not implemented yet, status code is : "+std::to_string(finalResponse.header.getStatus()));
        }
    }
}


// const HTTPRequestWrapper::CustomHttpResponse HTTPRequestWrapper::get(URI prevURI, const std::string &loggerName)
// {
//     //MARK: - Latest changes
//     bool enableLogging = loggerName.length();
//     Logger* fileLogger; 
//     if (enableLogging) {
//         fileLogger = &Utility::Logger::getCurrentThreadLogger();
//     }
//     //Mark: -
    
//     // Global default HTTPSession Factory
//     Net::HTTPSessionFactory &defaultFactory = Net::HTTPSessionFactory::defaultFactory();
    
//     // Poco request and response objects
//     Net::HTTPRequest request(Net::HTTPMessage::HTTP_1_1);
//     // HTTPResponse response;
    
//     // Custom HTTP Struct
//     HTTPRequestWrapper::CustomHttpResponse finalResponse;
    
//     // Stream to hold data of response body
//     std::stringstream body_stream;
    
//     // currentURI
//     URI currentURI = prevURI;
    
//     // Starting new Session
//     Net::HTTPClientSession *session = defaultFactory.createClientSession(currentURI);
    
//     // Flag to avoid redirection to same URL
//     bool notFirstRequest = false;
    
//     while(1)
//     {   
//         // Scheme changes so create new session
//         if(prevURI.getScheme() != currentURI.getScheme())
//         {
//             session->reset();
//             session = defaultFactory.createClientSession(currentURI);
//         }
//         // Host or port changes
//         else if(session->getHost() != currentURI.getHost() || session->getPort() != currentURI.getPort())
//         {
//             session->reset();
//             session->setHost(currentURI.getHost());
//             session->setPort(currentURI.getPort());
//         }
        
//         prevURI = currentURI;
//         request.setURI(currentURI.toString());
//         session->sendRequest(request);
//         std::istream& result = session->receiveResponse(finalResponse.header);
//         if(finalResponse.header.getStatus() == Net::HTTPResponse::HTTP_UNAUTHORIZED)
//         {
//             // Unauthorized response
//             throw ApplicationException("401 Unauthorized");
//         }
//         else if(HTTPRequestWrapper::isRedirectionStatus(finalResponse.header.getStatus()))
//         {
//             // 300/301/307/308 response header
//             if(finalResponse.header.has("Location"))
//             {
//                 currentURI.resolve(finalResponse.header.get("Location"));
                
//                 if(notFirstRequest)
//                 {
//                     if(currentURI == prevURI)
//                     {
//                         // Redirect to prevURI
//                         throw ApplicationException("Redirected to the same previous URL");
//                     }
//                 }
//                 else
//                 {
//                     notFirstRequest = true;
//                     if (enableLogging) {
//                         fileLogger->information("Redirecting: " + prevURI.toString() + " --> " + currentURI.toString());
//                     }
//                 }
//             }
//             else
//             {
//                 // No redirection url provided in response header
//                 throw ApplicationException("Redirect Location was not provided in the response header");
//             }
            
//         }
//         else if(finalResponse.header.getStatus() == Net::HTTPResponse::HTTP_OK)
//         {
//             // Final response
//             StreamCopier::copyStream(result, body_stream);
//             finalResponse.body = body_stream.str();
//             return finalResponse;
//         }
//         else
//         {
//             // Rest of response codes not implemented
//             throw ApplicationException("Status Code method not implemented yet, status code is : "+std::to_string(finalResponse.header.getStatus()));
//         }
//     }
    
// }

const HTTPRequestWrapper::CustomHttpResponse HTTPRequestWrapper::get(const std::string &url, const std::string &loggerName)
{
    // URL parsing
    URLParser url_(url);
    URI uri(url_.beautifyURL());
    return HTTPRequestWrapper::get(uri, loggerName);
}





