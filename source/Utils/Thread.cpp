#include <Poco/Thread.h>

#include <Utils/Thread.h>

using namespace Poco;

const int ThreadUtil::getCurrentThreadId() {
    return Thread::current()->id();
}

const void ThreadUtil::sleep(const long ms) {
    Thread::sleep(ms);
}
