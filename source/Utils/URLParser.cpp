#include "Utils/URLParser.h"

uint16_t getPortFromProtocol(const std::string protocol) {
	if (protocol == HTTP_)
		return HTTP_PORT_;
	if (protocol == HTTPS_)
		return HTTPS_PORT_;

	return 0; //error
}

size_t findColonPos(std::string str) {
	size_t i = 0;
	while (i < str.length()) {
		if (str[i] == ':')
			return i;
		else
			i++;
	}
	return i;
}

bool isValidCharInHost(const std::string& host) {
	if (host[0] == '-' || host[host.length()-1] == '-') {
		return false;
	}
	for (auto it : host) {
		if ((it < 'a' || it > 'z') && (it < 'A' || it > 'Z') && (it < '0' || it > '9') && (it != '-') && (it != '.')) {
			return false;
		}
	}
	return true;
}

URLParser::URLParser(const std::string &URL) : url(URL) {
	parser();
}

void URLParser::parser() {
	size_t url_length = url.length();
	size_t it = 0; // Iterator index


	// Finding Protocol
	try {
		size_t found = url.find("://");
		if (found != std::string::npos) {
			std::string prot_str = url.substr(0, found);
			// If URL starts with http://
			if (!prot_str.compare(HTTP_)) {
				protocol = HTTP_;
				it += found + 3;
			}
			// If URL starts with https://
			else if (!prot_str.compare(HTTPS_)) {
				protocol = HTTPS_;
				it += found + 3;
			}
			else {
				throw std::string("URLParser::parser  ->" + prot_str + "protocol not supported");
			}
		}
		else {
			protocol = DEFAULT_PROTOCOL;
		}
	}
	catch (std::string err) {
		throw err;
	}
	catch (...) {
		throw "URLParser::parser -> error in finding protocol";
	}


	//Finding Host and Port
	try {
		size_t it2 = it;
		for (; url[it2 + 1] != '/' && it2 + 1 < url_length; it2++);

		size_t colonPos = findColonPos(url.substr(it, it2 - it + 1)) + it;

		// If colon found
		if (colonPos != it2 + 1) {
			host = url.substr(it, colonPos - it);
			port = stoi(url.substr(colonPos + 1, it2 - colonPos));

		}
		else {
			host = url.substr(it, it2 - it + 1);
			port = getPortFromProtocol(protocol);
		}
		
		if (!isValidCharInHost(host)) {
			throw std::string("URLParse::parser -> Invalid Host");
		}
		it = it2 + 1;

	}
	catch (std::string err) {
		throw err;
	}
	catch (...) {
		throw "URLParser::parser -> error in finding Host and Port";
	}


	//Finding Path
	try {
		if (it >= url_length)
			path = "/";
		else
			path = url.substr(it);
	}
	catch (...) {
		throw "URLParser::parser -> error in finding path";
	}


}


std::string URLParser::getProtocol() {
	return protocol;
}

std::string URLParser::getHost() {
	return host;
}

uint16_t URLParser::getPort() {
	return port;
}

std::string URLParser::getPath() {
	return path;
}

std::string URLParser::beautifyURL() {
	return protocol + "://" + host + ":" + std::to_string(port) + path;
}

URLParser::~URLParser() {
}

