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
    T& get(std::size_t index) {
        assert(index < size);
        T * ptr = reinterpret_cast<T*>(arr_);
        return ptr[index];
    }

    void set(std::size_t index, T& elt) {
        assert(index < size);
        T * ptr = reinterpret_cast<T*>(arr_);
        ptr[index] = elt;
    }

    T * __getPtr__() {  // TODO(theimer): remove this when replaced all T* args
        return reinterpret_cast<T*>(arr_);
    }
};

}
