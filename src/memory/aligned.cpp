#include <rainbow/memory/aligned.hpp>

namespace rainbow::memory
{

void* alignedTo(void* baseAddress, const std::size_t boundary)
{
    const auto base = arithmeticPointer(baseAddress);
    const auto distance =
        rainbow::memory::distanceToNextAligned(baseAddress, boundary);

    if(distance == static_cast<std::ptrdiff_t>(boundary))
    {
        return baseAddress;
    }
    else
    {
        return pointer(base + distance);
    }
}

std::ptrdiff_t misalignment(void* pointer, const std::size_t boundary)
{
    return static_cast<std::ptrdiff_t>(
        reinterpret_cast<std::uintptr_t>(pointer) % boundary);
}

std::ptrdiff_t distanceToNextAligned(void* pointer, const std::size_t boundary)
{
    return static_cast<std::ptrdiff_t>(
        boundary - reinterpret_cast<std::uintptr_t>(pointer) % boundary);
}

bool isAligned(void* pointer, const std::size_t boundary)
{
    return misalignment(pointer, boundary) == 0;
}

std::size_t nextAlignedSize(const std::size_t size, const std::size_t boundary)
{
    const std::size_t diff = size % boundary;

    if(diff == 0)
    {
        return size;
    }

    return size + (boundary - diff);
}

} // namespace rainbow::memory
