#ifndef UTIL_FIXEDMAP_H_
#define UTIL_FIXEDMAP_H_

#include <cstdint>

#include "util/assert.h"

namespace util {

/*
TODO(theimer)
*/
template <typename K, typename V>
class FixedSizeMap {
 public:
    FixedSizeMap(std::size_t max_size) : max_size_(max_size) {
        slots_ = new FixedSizeMapSlot[max_size];
    }

    ~FixedSizeMap() {
        delete[] slots_;
    }

    V* end() const {
        return reinterpret_cast<V*>(slots_ + max_size_);
    }

    V* find(const K& key) const {
        std::size_t index = getIndex(key);
        FixedSizeMapSlot& slot = slots_[index];
        if (slot.present && (slot.key == key)) {
            return &slot.value;
        } else {
            return end();
        }
    }

    void set(const K& key, const V& value) {
        std::size_t index = getIndex(key);
        FixedSizeMapSlot& slot = slots_[index];
        slot.present = true;
        slot.value = value;
        slot.key = key;
    }

 private:
    /*
    A "slot" in a FixedSizeMap.
    */
    struct FixedSizeMapSlot {
        bool present = false;
        std::size_t key;
        V value;
    };

    std::size_t max_size_;
    FixedSizeMapSlot* slots_;

    std::size_t getIndex(const K& key) const {
        std::size_t hash = std::hash<K>{}(key);
        std::size_t index =  hash % max_size_;
        ASSERT(index < max_size_, "invalid index: " + std::to_string(index));
        return index;
    }
};

}

#endif  // UTIL_FIXEDMAP_H_
