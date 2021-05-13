#include <rainbow/memory/aligned.hpp>
#include <rainbow/memory/allocator.hpp>

using namespace rainbow::memory;

std::size_t AllocationRequirements::totalSize() const
{
    return nextAlignedSize(size, alignment) + extraSize;
}

AllocationRequirements&
    AllocationRequirements::operator+=(const AllocationRequirements& other)
{
    size += other.size;
    extraSize += other.extraSize;
    alignment = std::max(alignment, other.alignment);

    return *this;
}

AllocationRequirements Allocator::minimalAllocationRequirements() const
{
    return {};
}

Allocation Allocator::allocate(const AllocationRequirements& requirements)
{
    if(requirements.alignment > 1)
    {
        return allocateAligned(
            requirements.totalSize(), requirements.alignment);
    }
    else
    {
        return allocate(requirements.totalSize());
    }
}

Allocation Allocator::allocate()
{
    return AllocationResult::NotSupported;
}

Allocation Allocator::allocate([[maybe_unused]] const std::size_t bytes)
{
    return AllocationResult::NotSupported;
}

Allocation Allocator::allocateAligned(
    [[maybe_unused]] const std::size_t bytes,
    [[maybe_unused]] const std::size_t boundary)
{
    return allocate(bytes + alignof(std::max_align_t) - boundary);
}

Allocation Allocator::reallocate(
    [[maybe_unused]] const Block&      originalBlock,
    [[maybe_unused]] const std::size_t bytes)
{
    return AllocationResult::NotSupported;
}

Allocation Allocator::reallocateAligned(
    [[maybe_unused]] const Block&      originalBlock,
    [[maybe_unused]] const std::size_t bytes,
    [[maybe_unused]] const std::size_t boundary)
{
    return AllocationResult::NotSupported;
}

Deallocation Allocator::free([[maybe_unused]] const Block& block)
{
    return DeallocationResult::NotSupported;
}
