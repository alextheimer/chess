// Copyright 2021 Alex Theimer

#ifndef UTIL_MATH_H_
#define UTIL_MATH_H_

#include <cstdint>

namespace util {

/*
Returns true iff `val` is an exact power of two.
@param val must be >= 0
*/
bool isPow2(std::size_t val);

/*
Returns the ceiling of the log2 function.
@param val must be > 0
*/
std::size_t log2Ceil(std::size_t val);

}  // namespace util

#endif  // UTIL_MATH_H_
