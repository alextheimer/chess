#ifndef MATH_H_
#define MATH_H_

#include <cstdint>

namespace math {

/**
 * Returns true iff `val` is an exact power of two.
 *
 * @param val must be >= 0
 */
bool isPow2(std::size_t val);

/**
 * Returns the ceiling of the log2 function.
 *
 * @param val must be > 0
 */
std::size_t log2Ceil(std::size_t val);

}  // namespace math

#endif  // MATH_H_
