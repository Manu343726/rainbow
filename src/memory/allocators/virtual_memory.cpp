#include <rainbow/memory/allocators/virtual_memory.hpp>
#include <rainbow/memory/virtual.hpp>

using namespace rainbow::memory;
using namespace rainbow::memory::allocators;

Allocator::Info VirtualMemory::info() const
{
    return {};
}

Allocator::Features VirtualMemory::features() const
{
    return FEATURES;
}

Allocation VirtualMemory::allocate(const std::size_t size)
{
    return rainbow::memory::allocateVirtualMemory(size);
}

Deallocation VirtualMemory::free(const Block& block)
{
    return rainbow::memory::freeVirtualMemory(block);
}

namespace rainbow::memory::allocators
{
VirtualMemory& virtualMemory()
{
    static VirtualMemory allocator;
    return allocator;
}
} // namespace rainbow::memory::allocators
