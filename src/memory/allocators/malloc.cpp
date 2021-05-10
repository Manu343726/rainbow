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

Block Malloc::allocate(const std::size_t bytes)
{
    return {std::malloc(bytes), bytes};
}

Allocator::Info Malloc::info() const
{
    return {};
}

Block Malloc::allocateAligned(
    const std::size_t bytes, const std::size_t boundary)
{
    Block result{std::aligned_alloc(boundary, bytes), bytes};

    assert(result.isNull() xor result.isAligned(boundary));
    assert(result.isNull() xor (result == result.aligned(boundary, bytes)));

    return result;
}

Block Malloc::reallocate(const Block& original, const std::size_t bytes)
{
    return {std::realloc(original.begin(), bytes), bytes};
}

bool Malloc::free(const rainbow::memory::Block& block)
{
    std::free(block.begin());
    return not block.isNull();
}

Allocator::Features Malloc::features() const
{
    return FEATURES;
}
