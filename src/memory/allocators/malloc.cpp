#include "rainbow/memory/aligned.hpp"
#include <cassert>
#include <cstdlib>
#include <rainbow/memory/allocators/malloc.hpp>

using namespace rainbow::memory;
using namespace rainbow::memory::allocators;


namespace rainbow::memory::allocators
{
Malloc& malloc()
{
    thread_local Malloc malloc;
    return malloc;
}
} // namespace rainbow::memory::allocators

Allocation Malloc::allocate(const std::size_t bytes)
{
    if(Block block{std::malloc(bytes), bytes})
    {
        return block;
    }

    return AllocationResult::NoSpaceLeft;
}

Allocator::Info Malloc::info() const
{
    return {};
}

Allocation
    Malloc::allocateAligned(const std::size_t bytes, const std::size_t boundary)
{
    if(Block result{std::aligned_alloc(boundary, bytes), bytes})
    {
        if(not result.isAligned(boundary))
        {
            return AllocationResult::CannotAlign;
        }

        return result;
    }

    return AllocationResult::NoSpaceLeft;
}

Allocation Malloc::reallocate(const Block& original, const std::size_t bytes)
{
    if(Block block{std::realloc(original.begin(), bytes), bytes})
    {
        return block;
    }

    return AllocationResult::NoSpaceLeft;
}

Deallocation Malloc::free(const rainbow::memory::Block& block)
{
    if(block.isNull())
    {
        return DeallocationResult::UnknownNode;
    }

    std::free(block.begin());

    return DeallocationResult::Ok;
}

Allocator::Features Malloc::features() const
{
    return FEATURES;
}
