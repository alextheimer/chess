#ifndef UTIL_FIXEDMAP_H_
#define UTIL_FIXEDMAP_H_

#include <cstdint>

#include "util/assert.h"

namespace util {

/*
Hash table of fixed size.
Collisions are simply replaced with the most-recent value.
*/
template <typename K, typename V>
class FixedSizeMap {
 public:
    FixedSizeMap(std::size_t size) :
            size_(size),
            slots_(new FixedSizeMapSlot[size]) {
       // intentionally blank
    }

    ~FixedSizeMap() {
        delete[] slots_;
    }

    /*
    Returns an iterator referring to the "past-the-end" element.

    The iterator doesn't point to any element and should not be dereferenced.
    */
    V* end() const {
        return reinterpret_cast<V*>(slots_ + size_);
    }

    /*
    Returns a pointer to the value if `key` exists in the map.
    Otherwise, returns the end iterator (i.e. the return value of end()).
    */
    V* find(const K& key) const {
        std::size_t index = getIndex(key);
        FixedSizeMapSlot& slot = slots_[index];
        // TODO(theimer): worth assuming enough key randomness that the
        //     slot.present check isn't necessary?
        if (slot.present && (slot.key == key)) {
            return &slot.value;
        } else {
            return end();
        }
    }

    /*
    Sets the value at the key.
    ***Overwrites any existing value.***
    */
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

    const std::size_t size_;
    FixedSizeMapSlot* const slots_;

    /*
    Given a key, returns a matching slot index.
    */
    std::size_t getIndex(const K& key) const {
        std::size_t hash = std::hash<K>{}(key);
        // TODO(theimer): faster / more random method?
        std::size_t index =  hash % size_;
        ASSERT(index < size_, "invalid index: " + std::to_string(index));
        return index;
    }
};

}  // namespace util

#endif  // UTIL_FIXEDMAP_H_
