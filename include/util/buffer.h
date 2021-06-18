#include <cstdint>

namespace util {

// TODO(theimer): overuse of this will bloat the binary, no?
//    Do any better solutions exist in C++??

/**
 * Array that does not initialize its elements at its own initialization.
 */
template <typename T, std::size_t size>
class Buffer {
 private:
    uint8_t arr_[size * sizeof(T)];
 public:
    T& get(std::size_t index);
    void set(std::size_t index, T& elt);
};

}
