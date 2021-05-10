#include <rainbow/memory/allocator.hpp>

using namespace rainbow::memory;

Block Allocator::allocate()
{
    return nullptr;
}

Block Allocator::allocate([[maybe_unused]] const std::size_t bytes)
{
    return nullptr;
}

Block Allocator::allocateAligned(
    [[maybe_unused]] const std::size_t bytes,
    [[maybe_unused]] const std::size_t boundary)
{
    return nullptr;
}

Block Allocator::reallocate(
    [[maybe_unused]] const Block&      originalBlock,
    [[maybe_unused]] const std::size_t bytes)
{
    return nullptr;
}

Block Allocator::reallocateAligned(
    [[maybe_unused]] const Block&      originalBlock,
    [[maybe_unused]] const std::size_t bytes,
    [[maybe_unused]] const std::size_t boundary)
{
    return nullptr;
}

bool Allocator::free([[maybe_unused]] const Block& block)
{
    return false;
}
