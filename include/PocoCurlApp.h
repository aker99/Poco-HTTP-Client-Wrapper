#pragma once

#include <Poco/Util/Application.h>
#include <Poco/Util/OptionSet.h>


struct ProxyConfig {
	std::string host;
	unsigned short port;
	std::string username;
	std::string password;
};

class PocoCurlApp: public Poco::Util::Application
{	

private:
	bool _helpRequested, _proxySet, _proxyCredSet, _error;
	std::vector<std::string> userValidURLs;
	std::vector<std::string> userInvalidURLs;
	ProxyConfig  proxyConf;

public:

	PocoCurlApp();

	~PocoCurlApp();


protected:	
	
	void defineOptions(Poco::Util::OptionSet& options);
	
	void handleHelp(const std::string& name, const std::string& value);
	
	void handleURL(const std::string& name, const std::string& value);
	
	void handleProxy(const std::string& name, const std::string& value);

	void handleProxyCred(const std::string& name, const std::string& value);
		
	void displayHelp();

	void handleInvalidCommand(const std::string &flag, const std::string &error);

	int main(const ArgVec & args);
};
