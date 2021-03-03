#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/Logger.h>

#include "Utils/Logger.h"
#include "Utils/Thread.h"


using namespace Poco;
// using namespace LoggerUtil

//Poco::Logger &Utility::Logger::create(const std::string &name, Message::Priority priority) {
//
//    Poco::FormattingChannel* pFCFile = new Poco::FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"), new Poco::FileChannel("request.log"));
//    // pFCFile->isop
////    pFCFile->open();
//
//	return Poco::Logger::create(name, pFCFile, priority);
//
//}

Poco::Logger &Utility::Logger::getCurrentThreadLogger() {
    return Poco::Logger::get(Utility::Thread::getCurrentThreadCustomName());
}

//Poco::Logger &Utility::Logger::createCurrentThreadLogger() {
//    return Utility::Logger::create(std::to_string(Utility::Thread::getCurrentThreadId()));
//}

Poco::Logger &Utility::Logger::getLoggerByName(const std::string &name) {
    return Poco::Logger::get(name);
}

void Utility::Logger::setDefaultConfig()
{
    Poco::FormattingChannel* pFCFile = new Poco::FormattingChannel(new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"), new Poco::SimpleFileChannel("request.log"));
    Poco::Logger::root().setChannel(pFCFile);
}