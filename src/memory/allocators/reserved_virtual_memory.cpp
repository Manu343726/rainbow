#include <rainbow/memory/allocators/reserved_virtual_memory.hpp>
#include <rainbow/memory/virtual.hpp>

using namespace rainbow::memory;
using namespace rainbow::memory::allocators;

Allocator::Info ReservedVirtualMemory::info() const
{
    return {};
}

Allocator::Features ReservedVirtualMemory::features() const
{
    return FEATURES;
}

Allocation ReservedVirtualMemory::allocate(const std::size_t size)
{
    return rainbow::memory::reserveVirtualMemory(size);
}

Deallocation ReservedVirtualMemory::free(const Block& block)
{
    return rainbow::memory::releaseVirtualMemory(block);
}

namespace rainbow::memory::allocators
{

ReservedVirtualMemory& reservedVirtualMemory()
{
    static ReservedVirtualMemory allocator;
    return allocator;
}

} // namespace rainbow::memory::allocators
