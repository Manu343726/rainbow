#ifndef RAINBOW_MEMORY_ALIGNED_HPP_INCLUDED
#define RAINBOW_MEMORY_ALIGNED_HPP_INCLUDED

#include <cstddef>
#include <cstdint>

namespace rainbow::memory
{

constexpr void* pointer(char* arithmeticPointer)
{
    return static_cast<void*>(arithmeticPointer);
}

constexpr char* arithmeticPointer(void* pointer)
{
    return static_cast<char*>(pointer);
}

std::ptrdiff_t misalignment(void* pointer, const std::size_t boundary);
std::ptrdiff_t distanceToNextAligned(void* pointer, const std::size_t boundary);
bool           isAligned(void* pointer, const std::size_t boundary);
void*          alignedTo(void* baseAddress, const std::size_t boundary);

} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_ALIGNED_HPP_INCLUDED
