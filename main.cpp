#include <iostream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include "CustomDef.h"
#include "CustomHttpClient.h"

void test(const String path) {
  static CustomHttpClient client("localhost", 8080);
  std::cout<<"Requesting to localhost:8080"<<path<<std::endl;
  try {
    const CustomHttpResponse response = client.request(path, Poco::Net::HTTPRequest::HTTP_GET);
    std::cout << "Response: \nCode :" << response.header.getStatus() << "\t\t Message : " << response.header.getReason() << "\nBody: "<<std::endl<<response.body<<std::endl;  
  } catch (const std::exception &e) {
    std::cerr <<"Error Occured : "<< e.what() << std::endl;
  } catch (const char* e) {
    std::cerr << "Custom Error Occured : " << e << std::endl;
  }
  std::cout<<std::endl;
}

int main(int argc, char const *argv[]) {
  test("/redirect");
  test("/1");
  return 0;
}
