#ifndef RAINBOW_MEMORY_ALLOCATORS_POOL_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATORS_POOL_HPP_INCLUDED

#include <rainbow/memory/allocator.hpp>

namespace rainbow::memory::allocators
{

class Pool final : public rainbow::memory::Allocator
{
public:
    static constexpr Features FEATURES =
        Features::Allocate | Features::AllocateAligned | Features::Free |
        Features::InPlaceBookKeeping | Features::ConstrainedAllocation;

    struct Parameters
    {
        std::size_t maxSize;
        std::size_t maxAlignment;
        std::size_t count;
        bool        satisfiesExternalRequirements = false;
    };

    Pool(rainbow::memory::Block storage, const Parameters& parameters);

    static rainbow::memory::AllocationRequirements storageRequirements(
        const Parameters&                 parameters,
        const rainbow::memory::Allocator* parentAllocator  = nullptr,
        const rainbow::memory::Allocator* managerAllocator = nullptr);

    static rainbow::memory::AllocationRequirements nodeRequirements(
        const Parameters&                 parameters,
        const rainbow::memory::Allocator* parentAllocator  = nullptr,
        const rainbow::memory::Allocator* managerAllocator = nullptr);

    static Parameters adaptParametersToRequirements(
        Parameters                        parameters,
        const rainbow::memory::Allocator* parentAllocator  = nullptr,
        const rainbow::memory::Allocator* managerAllocator = nullptr);

    Features               features() const override;
    Info                   info() const override;
    AllocationRequirements minimalAllocationRequirements() const override;
    AllocationRequirements maxAllocationRequirements() const override;

    rainbow::memory::Allocation allocate(const std::size_t bytes) override;
    rainbow::memory::Allocation allocateAligned(
        const std::size_t bytes, const std::size_t boundary) override;
    rainbow::memory::Deallocation
        free(const rainbow::memory::Block& block) override;

private:
    rainbow::memory::Block                  _nextBlock;
    rainbow::memory::AllocationRequirements _nodeRequirements;
    rainbow::memory::Block                  _storage;

    std::size_t maxItemSize() const;
    std::size_t blockSize() const;
    std::size_t blockAlignment() const;
};

} // namespace rainbow::memory::allocators

#endif // RAINBOW_MEMORY_ALLOCATORS_POOL_HPP_INCLUDED
