#ifndef MATH_H_
#define MATH_H_

#include <cstdint>

using std::size_t;

namespace math {
    bool isPow2(size_t val);
    size_t log2Ceil(size_t val);
}

#endif  // MATH_H_