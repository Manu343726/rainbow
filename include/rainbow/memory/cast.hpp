#ifndef RAINBOW_MEMORY_CAST_HPP_INCLUDED
#define RAINBOW_MEMORY_CAST_HPP_INCLUDED

#include <cassert>
#include <cstring>
#include <rainbow/memory/aligned.hpp>

namespace rainbow::memory
{

template<typename T>
T bit_cast(void* pointer)
{
    assert(rainbow::memory::isAligned(pointer, alignof(T)));

    T result;
    std::memcpy(&result, pointer, sizeof(T));
    return result;
}

template<typename T, typename U>
T bit_cast(U value)
{
    return bit_cast<T>(static_cast<void*>(&value));
}

template<typename T>
void bit_write(const T& from, void* to)
{
    assert(rainbow::memory::isAligned(to, alignof(T)));

    std::memcpy(to, &from, sizeof(T));
}

template<typename T, typename U>
void bit_assign(const T& from, const U& to)
{
    bit_write(static_cast<void*>(&from), to);
}

} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_CAST_HPP_INCLUDED
