#include <cassert>
#include <rainbow/memory/virtual.hpp>
#include <sys/mman.h>

namespace rainbow::memory
{

Allocation allocateVirtualMemory(const std::size_t size)
{
    RAINBOW_RESULT_TRY(allocation, reserveVirtualMemory(size));

    return commitVirtualMemory(allocation);
}

Allocation reserveVirtualMemory(const std::size_t size)
{
    if(size == 0)
    {
        return AllocationResult::RequirementsFailed_MinSize;
    }

    if(void* result =
           ::mmap(nullptr, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
       result != MAP_FAILED)
    {
        return Block{result, size};
    }

    return AllocationResult::NoSpaceLeft;
}

Allocation commitVirtualMemory(const Block& block)
{
    if(::mprotect(block.begin(), block.size(), PROT_READ | PROT_WRITE) == -1)
    {
        return AllocationResult::CannotCommit;
    }

    return block;
}

Deallocation unCommitVirtualMemory(const Block& block)
{
    if(::mprotect(block.begin(), block.size(), PROT_NONE) == -1)
    {
        return DeallocationResult::CannotUncommit;
    }

    return DeallocationResult::Ok;
}

Deallocation releaseVirtualMemory(const Block& block)
{
    if(::munmap(block.begin(), block.size()) == -1)
    {
        return DeallocationResult::UnknownNode;
    }

    return DeallocationResult::Ok;
}

Deallocation freeVirtualMemory(const Block& block)
{
    // Uncommit is not needed, handled by munmap() itself.
    return releaseVirtualMemory(block);
}

} // namespace rainbow::memory
