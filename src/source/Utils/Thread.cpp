#include <string>

#include <Poco/Thread.h>

#include <Utils/Thread.h>

const int Utility::Thread::getCurrentThreadId() {
    return Poco::Thread::current()->id();
}

const std::string Utility::Thread::getCurrentThreadCustomName() {
    return std::to_string(Utility::Thread::getCurrentThreadId());
}

const void Utility::Thread::sleep(const long ms) {
    Poco::Thread::sleep(ms);
}
