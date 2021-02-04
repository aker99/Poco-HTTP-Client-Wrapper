#include <iostream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include "CustomDef.h"
#include "CustomHttpClient.h"

void test(const String path) {
  static CustomHttpClient client("localhost", 8080);
  try {
    const CustomHttpResponse response = client.request(path, Poco::Net::HTTPRequest::HTTP_GET);
    std::cout << "Response is : " << response.header.getStatus() << "\t\t Reason is : " << response.header.getReason() << std::endl << "Body: "<<std::endl<<response.body<<std::endl;  
  } catch (const std::exception &e) {
    std::cerr <<"Error Occured : "<< e.what() << '\n';
  }
}

int main(int argc, char const *argv[]) {
  test("/redirect");
  test("/1");
  return 0;
}
