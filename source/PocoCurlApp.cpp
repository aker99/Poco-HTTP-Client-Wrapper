#include <regex>
#include <sstream>

#include <Poco/Util/HelpFormatter.h>

#include "PocoCurlApp.h"
#include "HttpRequestPool.h"
#include "CustomHttpSessionFactory.h"

using namespace Poco::Util;

//Contructor
PocoCurlApp::PocoCurlApp(): _helpRequested(false)
{
	std::cout<<std::endl;
}


//Destructor
PocoCurlApp::~PocoCurlApp() {}

void PocoCurlApp::defineOptions(OptionSet& options)
{
	Application::defineOptions(options);

	options.addOption(
		Option("help", "h", "Display help information on command line arguments")
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<PocoCurlApp>(this, &PocoCurlApp::handleHelp)));

	options.addOption(
		Option("url", "u", "loads the given urls")
			.required(true)
			.repeatable(false)
			.argument("value")
			.callback(OptionCallback<PocoCurlApp>(this, &PocoCurlApp::handleURL)));
			
	options.addOption(
		Option("proxy", "p", "sets the proxy ip/domain")
			.required(false)
			.repeatable(false)
			.argument("value")
			.callback(OptionCallback<PocoCurlApp>(this, &PocoCurlApp::handleProxy)));
	
	options.addOption(
		Option("proxy-cred", "c", "sets the proxy username and password")
			.required(false)
			.repeatable(false)
			.argument("value")
			.callback(OptionCallback<PocoCurlApp>(this, &PocoCurlApp::handleProxyCred)));
}


void PocoCurlApp::handleHelp(const std::string& name, const std::string& value)
{
	_helpRequested = true;
	displayHelp();
	stopOptionsProcessing();
}


void PocoCurlApp::handleURL(const std::string& name, const std::string& value)
{
	//Splitting the urls into single url and adding to the userURLs
	std::stringstream inputStream(value);

	std::string url;	
	std::regex urlRegex("((http|https)(://))?(www\\.)?(\\w+/?)(\\.\\w+)");

	while(getline(inputStream, url, ' ')){
		if(std::regex_match(url, urlRegex))
			userValidURLs.push_back(url);
		else
			userInvalidURLs.push_back(url);
	}

}


void PocoCurlApp::handleProxy(const std::string& name, const std::string& value)
{
	std::stringstream inputStream(value);
	std::string port;

	getline(inputStream, proxyConf.host, ':');
	getline(inputStream, port);

	if (proxyConf.host != "\0"){
		handleInvalidCommand("Invalid Proxy");
	}

	if (port == "\0")
		proxyConf.port = 80;
	else
		proxyConf.port = stoi(port);
}

void PocoCurlApp::handleProxyCred(const std::string& name, const std::string& value)
{
	std::stringstream inputStream(value);
	std::string temp;

	getline(inputStream, proxyConf.username, ' ');
	getline(inputStream, proxyConf.password, ' ');
	getline(inputStream, temp, ' ');
	
	if(temp != "\0" || (proxyConf.username != "\0" && proxyConf.password == "\0")) {
		handleInvalidCommand("Invalid Credentials");
	}
}

	
void PocoCurlApp::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.setHeader("Options available for this program:");
	helpFormatter.format(std::cout);
}

void PocoCurlApp::handleInvalidCommand(const std::string &error) {
	std::cerr<<"Invalid arguments: "<<error<<std::endl;
	_helpRequested = true;
	displayHelp();
	stopOptionsProcessing();
}

int PocoCurlApp::main(const ArgVec& args)
{
	if(!_helpRequested){
		if(!userValidURLs.size())
			std::cout<<"No valid urls found";

		if(userInvalidURLs.size()) {
			std::cout<<"Invalid URL(s) found. Skipping the following URL(s):"<<std::endl;
			for(const std::string &s : userInvalidURLs)
				std::cout<<s<<std::endl;
		}

		//Write Your Code Here
		if(userValidURLs.size()){
			// 	std::cout<<"\n\nValid URLS : "<<std::endl;
			// 	for(const std::string &s : userValidURLs)
			// 		std::cout<<s<<std::endl;
				

				// std::cout<<"\n\nProxy : \n";
				// std::cout<<"Proxy IP/Domain : "<<Poco::Net::HTTPSessionFactory::defaultFactory().proxyHost()<<":"<<Poco::Net::HTTPSessionFactory::defaultFactory().proxyPort()<<std::endl;
				
			CustomHTTPSessionFactory::registerHTTP();
			CustomHTTPSessionFactory::registerHTTPS();

			if (proxyConf.host.length()) {
				CustomHTTPSessionFactory::setProxy(proxyConf.host, proxyConf.port);
				if(proxyConf.username.length()) {
					CustomHTTPSessionFactory::setProxyCredentials(proxyConf.username, proxyConf.password);
				}
			}

			// if(!(Poco::Net::HTTPSessionFactory::defaultFactory().proxyUsername() == "\0") && !(Poco::Net::HTTPSessionFactory::defaultFactory().proxyPassword() == "\0")){
			// 	std::cout<<"Proxy Username : "<<Poco::Net::HTTPSessionFactory::defaultFactory().proxyUsername()<<std::endl;
			// 	std::cout<<"Proxy Password : "<<Poco::Net::HTTPSessionFactory::defaultFactory().proxyPassword()<<std::endl;
			// }

			HttpRequestPool pool(userValidURLs);
			pool.start();
			pool.join();
		}

	}

	return Application::EXIT_OK;
}