#pragma once
#include <deque>
namespace sjtu {
    template <typename T, typename Alloc = std::allocator<T>>
    using deque = std::deque<T, Alloc>;
}
