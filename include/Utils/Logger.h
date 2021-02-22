#include <Poco/Logger.h>

namespace LoggerUtil {
    Poco::Logger &create(const std::string &name, Poco::Message::Priority priority = Poco::Message::Priority::PRIO_TRACE);
    Poco::Logger &getCurrentThreadLogger();
}