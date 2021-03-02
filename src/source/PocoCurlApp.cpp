#include <regex>
#include <sstream>

#include <Poco/Util/HelpFormatter.h>
#include "PocoCurlApp.h"

#include "HttpRequestPool.h"
#include "CustomHttpSessionFactory.h"
#include "Utils/Logger.h"

using namespace Poco::Util;


//Contructor
PocoCurlApp::PocoCurlApp(): _helpRequested(false), _proxySet(false), _proxyCredSet(false), _error(false)
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
		Option("url", "u", "loads the given url/ip or multiple urls/ips within \" \" separated by space") 
			.required(true)
			.repeatable(false)
			.argument("value")
			.callback(OptionCallback<PocoCurlApp>(this, &PocoCurlApp::handleURL)));
			
	options.addOption(
		Option("proxy", "p", "sets the proxy given in the format Proxy_Host:Proxy_Port")
			.required(false)
			.repeatable(false)
			.argument("value")
			.callback(OptionCallback<PocoCurlApp>(this, &PocoCurlApp::handleProxy)));
	
	options.addOption(
		Option("proxy-cred", "c", "sets the proxy credentials given in the format \"username password\" for given proxy")
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
	//regular expression for url validation
	std::regex urlRegex("((http|https)(://))?(www\\.)?(\\w+/?)(\\.\\w+)");  
	std::regex ipv4Regex("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
	std::regex ipv6Regex("((([0-9a-fA-F]){1,4})\\:){7}([0-9a-fA-F]){1,4}");

	//Splitting the urls into single url and adding to the userValidURLs
	std::stringstream inputStream(value);
	std::string url;		

	while(getline(inputStream, url, ' ')){
		if((std::regex_match(url, urlRegex)) || (std::regex_match(url, ipv4Regex)) || std::regex_match(url, ipv6Regex))
		{
			userValidURLs.push_back(url);
		}

		else
		{
			userInvalidURLs.push_back(url);
		}
	}

}


void PocoCurlApp::handleProxy(const std::string& name, const std::string& value)
{
	//regular expression for proxy
	std::regex proxyRegex("[\\w\\.]+(:\\d+)?");
	
	
	if(std::regex_match(value, proxyRegex))
	{
		std::stringstream inputStream(value);
		std::string port;

		getline(inputStream, proxyConf.host, ':');
		getline(inputStream, port);
	
		if (port == "\0"){
			proxyConf.port = 80;
		}

		else{
			proxyConf.port = stoi(port);
		}
		
		_proxySet = true;
	}

	else{
		handleInvalidCommand("proxy flag","Invalid proxy format");
	}
}


void PocoCurlApp::handleProxyCred(const std::string& name, const std::string& value)
{
	//Regular Expression for Proxy Credentials
	std::regex proxyCredRegex("(^[\\w_]+\\s+.+$)");	

	if(std::regex_match(value, proxyCredRegex))
	{
		std::stringstream inputStream(value);

		getline(inputStream, proxyConf.username, ' ');
		getline(inputStream, proxyConf.password, ' ');

		_proxyCredSet = true;
	}

	else{
		handleInvalidCommand("proxy credentials flag","Invalid Credentials Format");
	}
}


void PocoCurlApp::handleInvalidCommand(const std::string &flag, const std::string &error) {
	std::cerr<<"Error occured near "<<flag<<" : "<<error<<std::endl;
	_error = true;
	_helpRequested = true;
	stopOptionsProcessing();
}

	
void PocoCurlApp::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.setHeader("Options available for this program:");
	helpFormatter.format(std::cout);
}

int PocoCurlApp::main(const ArgVec& args)
{
	if(!_helpRequested){
		
		// if(userInvalidURLs.size()) 
		// {
		// 	std::cout<<"Invalid URL(s) found. Skipping the following URL(s):"<<std::endl;
		// 	for(const std::string &s : userInvalidURLs){
		// 		std::cout<<s<<std::endl;
		// 	}
		// }

		
		if(userValidURLs.size()){

			if(_proxyCredSet && !_proxySet){
				handleInvalidCommand("proxy credential flag","Credentials are given without proxy");
			}


			else{
				
				// std::cout<<"\n\nValid URLS : "<<std::endl;
				// for(const std::string &s : userValidURLs){
				// 	std::cout<<s<<std::endl;
				// }
				

				CustomHTTPSessionFactory::registerHTTP();
				CustomHTTPSessionFactory::registerHTTPS();

				if (proxyConf.host.length()) {
					// std::cout<<"\n\nProxy : \n";
					// std::cout<<"Proxy Host : "<<proxyConf.host<<"\nProxy Port :"<<proxyConf.port<<std::endl;
					
					CustomHTTPSessionFactory::setProxy(proxyConf.host, proxyConf.port);


					if(proxyConf.username.length()) {
						// std::cout<<"Proxy Username : "<<proxyConf.username<<std::endl;
						// std::cout<<"Proxy Password : "<<proxyConf.password <<std::endl;
						
						CustomHTTPSessionFactory::setProxyCredentials(proxyConf.username, proxyConf.password);
					}
				}


				Utility::Logger::setDefaultConfig();
				
				HttpRequestPool pool(userValidURLs);
				pool.start();
				pool.join();

			}
		}

		else{
			handleInvalidCommand("url flag","No Valid URLs are left for process");
		}

	}
	
	if(_error){
		std::cout<<"Please reffer to the -h or --help flag"<<std::endl;
	}

	return Application::EXIT_OK;
}
