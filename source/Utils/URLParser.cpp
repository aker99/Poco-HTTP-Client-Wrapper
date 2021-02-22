#include "Utils/URLParser.h"

uint16_t getPortFromProtocol(const std::string protocol){
    if(protocol == HTTP_)
        return HTTP_PORT_;
    if(protocol == HTTPS_)
        return HTTPS_PORT_;

    return 0; //error
}

size_t findColonPos(std::string str){
    size_t i= 0;
    while(i<str.length()){
        if(str[i]==':')
            return i;
        else
            i++;
    }
    return i;
}

URLParser::URLParser(const std::string &URL): url(URL){
    parser();
}

void URLParser::parser(){
    size_t url_length = url.length();
    size_t it = 0; // Iterator index


    // Finding Protocol
    try{
        std::string httpURLStart = "http://";
        std::string httpsURLStart = "https://";

        // If URL starts with http://
        if(!url.compare(it, httpURLStart.size(), httpURLStart)){
            protocol = HTTP_;
            it += httpURLStart.size();
        }
        // If URL starts with https://
        else if (!url.compare(it, httpsURLStart.size(), httpsURLStart)){
            protocol = HTTPS_;
            it += httpsURLStart.size();
        }
        else{
            protocol = DEFAULT_PROTOCOL;
        }
    }
    catch(...){
        throw "URLParser::parser -> error in finding protocol";
    }


    //Finding Host and Port
    try{
        size_t it2 = it;
        for(; url[it2+1] != '/' && it2+1<url_length; it2++);

        size_t colonPos = findColonPos(url.substr(it, it2-it+1)) + it;

        // If colon found
        if(colonPos != it2+1){
            host = url.substr(it, colonPos-it);
            port = stoi(url.substr(colonPos+1, it2 - colonPos));
           
        }
        else{
            host = url.substr(it, it2-it+1);
            port = getPortFromProtocol(protocol);
        }
        it = it2+1;
        
    }
    catch(...){
        throw "URLParser::parser -> error in finding Host and Port";
    }


    //Finding Path
    try{
        if(it >= url_length)
            path = "/";
        else
            path = url.substr(it);
    }
    catch(...){
        throw "URLParser::parser -> error in finding path";
    }
 
    
}

std::string URLParser::getProtocol(){
    return protocol;
}

std::string URLParser::getHost(){
    return host;
}

uint16_t URLParser::getPort(){
    return port;
}

std::string URLParser::getPath(){
    return path;
}

std::string URLParser::beautifyURL(){
    return protocol + "://" + host+ ":" + std::to_string(port) + path;
}

URLParser::~URLParser(){ 
}

