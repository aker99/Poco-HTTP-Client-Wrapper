#include <string>

#include <Poco/Thread.h>

#include <Utils/Thread.h>

// #define TESTING 1
#ifndef TESTING
#define TESTING 0
#endif

const int Utility::Thread::getCurrentThreadId() {
    return Poco::Thread::current()->id();
}

const std::string Utility::Thread::getCurrentThreadCustomName() {
    if (TESTING == 1) {
        return "TestThread";
    } 
    return std::to_string(Utility::Thread::getCurrentThreadId());
}

const void Utility::Thread::sleep(const long ms) {
    Poco::Thread::sleep(ms);
}
