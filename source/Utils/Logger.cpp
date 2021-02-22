#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/Logger.h>

#include "Utils/Logger.h"
#include "Utils/Thread.h"


using namespace Poco;
// using namespace LoggerUtil

Logger &LoggerUtil::create(const std::string &name, Message::Priority priority) {

    static FormattingChannel* pFCFile = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"), new FileChannel("sample.log"));
    // pFCFile->isop
    pFCFile->open();

	return Logger::create(name, pFCFile, priority);

}

Poco::Logger &LoggerUtil::getCurrentThreadLogger() {
    return Logger::get(std::to_string(ThreadUtil::getCurrentThreadId()));
}