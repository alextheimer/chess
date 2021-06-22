#include <iostream>

// (mostly) stolen from: https://stackoverflow.com/a/3767883
#ifdef DEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "assertion fail:" << std::endl \
                << #condition << std::endl \
                << "(" << __FILE__ << ", " << __LINE__ << "): " \
                << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif
