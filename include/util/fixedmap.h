#ifndef UTIL_FIXEDMAP_H_
#define UTIL_FIXEDMAP_H_

#include <cstdint>

#include "util/assert.h"

namespace util {

/*
TODO(theimer)
*/
template <typename V>
class FixedMap {
 public:
    FixedMap(std::size_t max_size) : max_size_(max_size) {
        slots_ = new FixedMapSlot[max_size];
    }

    ~FixedMap() {
        delete[] slots_;
    }

    V* end() const {
        return reinterpret_cast<V*>(slots_ + max_size_);
    }

    V* find(std::size_t key) const {
        std::size_t index = getIndex(key);
        FixedMapSlot& slot = slots_[index];
        if (slot.present && (slot.key == key)) {
            return &slot.value;
        } else {
            return end();
        }
    }

    void set(std::size_t key, const V& value) {
        std::size_t index = getIndex(key);
        FixedMapSlot& slot = slots_[index];
        slot.present = true;
        slot.value = value;
        slot.key = key;
    }

 private:
    /*
    A "slot" in a FixedMap.
    */
    struct FixedMapSlot {
        bool present = false;
        std::size_t key;
        V value;
    };

    std::size_t max_size_;
    FixedMapSlot* slots_;

    std::size_t getIndex(std::size_t key) const {
        std::size_t index =  key % max_size_;
        ASSERT(index < max_size_, "invalid index: " + std::to_string(index));
        return index;
    }
};

}

#endif  // UTIL_FIXEDMAP_H_
