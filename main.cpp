#include <iostream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include "CustomHttpClient.h"

void test(const std::string path) {
  static CustomHttpClient client("localhost", 8080);
  try {
    Poco::Net::HTTPResponse response = client.request(path, Poco::Net::HTTPRequest::HTTP_GET);
    std::cout << "Response is : " << response.getStatus() << "\t\t Reason is : " << response.getReason() << std::endl;  
  } catch (const std::exception &e) {
    std::cerr <<"Error Occured : "<< e.what() << '\n';
  }
}

int main(int argc, char const *argv[]) {
  test("/redirect");
  test("/1");
  return 0;
}
