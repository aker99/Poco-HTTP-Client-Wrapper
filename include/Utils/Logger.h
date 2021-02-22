#pragma once

#include <Poco/Logger.h>

namespace Utility {
    namespace Logger {
        Poco::Logger &create(const std::string &name, Poco::Message::Priority priority = Poco::Message::Priority::PRIO_TRACE);
        Poco::Logger &getCurrentThreadLogger();
        Poco::Logger &createCurrentThreadLogger();
    }
}