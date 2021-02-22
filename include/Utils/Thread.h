#pragma once

namespace Utility{
    namespace Thread {
        const int getCurrentThreadId();
        const std::string getCurrentThreadCustomName();
        const void sleep(const long ms);
    }
}