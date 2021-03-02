#include <string>

#include <Poco/Thread.h>

#include <Utils/Thread.h>

#include <iostream>
const int Utility::Thread::getCurrentThreadId() {
    return Poco::Thread::current()->id();
}

const std::string Utility::Thread::getCurrentThreadCustomName() {
    #ifdef TESTING
        return "TestThread";
    #endif
    std::cout<<"Try Thread";
    return std::to_string(Utility::Thread::getCurrentThreadId());
}

const void Utility::Thread::sleep(const long ms) {
    Poco::Thread::sleep(ms);
}
