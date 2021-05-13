#ifndef RAINBOW_MEMORY_ALLOCATORS_COMMITTEDVIRTUALMEMORY_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATORS_COMMITTEDVIRTUALMEMORY_HPP_INCLUDED

#include <optional>
#include <rainbow/memory/allocator.hpp>
#include <rainbow/memory/allocators/owning.hpp>
#include <rainbow/memory/allocators/reserved_virtual_memory.hpp>
#include <rainbow/memory/virtual.hpp>

namespace rainbow::memory::allocators
{

template<typename CommitAllocator>
class CommittedVirtualMemory final : public rainbow::memory::Allocator
{
public:
    static_assert(
        not(CommitAllocator::FEATURES & Features::GrowingAllocator),
        "The allocator used for the commit strategy must not be a growing allocator");
    static_assert(
        not(CommitAllocator::FEATURES & Features::OwningAllocator),
        "The allocator used for the commit strategy must not be an owning allocator");
    static_assert(
        not(CommitAllocator::FEATURES & Features::InPlaceBookKeeping),
        "The allocator used for the commit strategy must not use managed memory to store bookkeeping data");

    static constexpr Features FEATURES =
        CommitAllocator::FEATURES | Features::OwningAllocator;

    template<typename... Args>
    CommittedVirtualMemory(
        rainbow::memory::Block reservedVirtualMemory, const Args&... args)
        : _commitAllocator{
              rainbow::memory::allocators::reservedVirtualMemory(),
              std::move(reservedVirtualMemory),
              rainbow::memory::allocators::reservedVirtualMemory()
                  .minimalAllocationRequirements()
                  .alignment,
              args...}
    {
    }

    Features features() const override
    {
        return FEATURES;
    }

    Info info() const override
    {
        return _commitAllocator.info();
    }

    rainbow::memory::Allocation allocate(const std::size_t bytes) override
    {
        RAINBOW_RESULT_TRY(result, _commitAllocator.allocate(bytes));

        if(not rainbow::memory::commitVirtualMemory(result))
        {
            return AllocationResult::CannotCommit;
        }

        return result;
    }

    rainbow::memory::Allocation allocateAligned(
        const std::size_t bytes, const std::size_t boundary) override
    {
        RAINBOW_RESULT_TRY(
            result, _commitAllocator.allocateAligned(bytes, boundary));

        if(not rainbow::memory::commitVirtualMemory(result))
        {
            return AllocationResult::CannotCommit;
        }

        return result;
    }

    rainbow::memory::Deallocation
        free(const rainbow::memory::Block& block) override
    {
        RAINBOW_RESULT_TRY(rainbow::memory::unCommitVirtualMemory(block));
        return _commitAllocator.free(block);
    }

private:
    rainbow::memory::allocators::Owning<CommitAllocator> _commitAllocator;
};

} // namespace rainbow::memory::allocators

#endif // RAINBOW_MEMORY_ALLOCATORS_COMMITTEDVIRTUALMEMORY_HPP_INCLUDED
