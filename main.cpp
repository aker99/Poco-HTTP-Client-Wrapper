#include <iostream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include "CustomHttpClient.h"

void test() {
  CustomHttpClient client("localhost", 8080);
  try {
    Poco::Net::HTTPResponse response = client.request("/", Poco::Net::HTTPRequest::HTTP_GET);
    std::cout << "Response is : " << response.getStatus() << "\t\t Reason is : " << response.getReason() << std::endl;  
  } catch (const Poco::Net::Exce) 
  catch (const std::exception &e) {
    std::cerr <<"Error Occured : "<< e.what() << '\n';
  }
}

int main(int argc, char const *argv[]) {
  test();
  return 0;
}
