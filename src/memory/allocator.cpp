#include <cstddef>
#include <limits>
#include <rainbow/memory/aligned.hpp>
#include <rainbow/memory/allocator.hpp>

using namespace rainbow::memory;

AllocationRequirements Allocator::minimalAllocationRequirements() const
{
    return {};
}

AllocationRequirements Allocator::maxAllocationRequirements() const
{
    auto result      = minimalAllocationRequirements();
    result.size      = std::numeric_limits<std::size_t>::max();
    result.alignment = alignof(std::max_align_t);

    return result;
}

bool Allocator::fits(
    const rainbow::memory::AllocationRequirements& requirements) const
{
    return minimalAllocationRequirements() >= requirements and
           requirements <= maxAllocationRequirements();
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
