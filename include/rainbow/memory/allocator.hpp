#ifndef RAINBOW_MEMORY_ALLOCATOR_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATOR_HPP_INCLUDED

#include <cstdint>
#include <optional>
#include <rainbow/memory/aligned.hpp>
#include <rainbow/memory/allocation.hpp>
#include <rainbow/memory/allocation_requirements.hpp>
#include <rainbow/memory/allocator_features.hpp>
#include <rainbow/memory/block.hpp>
#include <rainbow/memory/deallocation.hpp>
#include <type_traits>

namespace rainbow::memory
{

class Allocator
{
public:
    using Features = rainbow::memory::AllocatorFeatures;

    struct Info
    {
        std::optional<std::size_t>            used;
        std::optional<std::size_t>            free;
        std::optional<std::size_t>            reserved;
        std::optional<rainbow::memory::Block> storage;
        Allocator*                            parent;
    };

    virtual ~Allocator() = default;

    virtual Features features() const = 0;
    virtual Info     info() const     = 0;
    virtual rainbow::memory::AllocationRequirements
        minimalAllocationRequirements() const;
    virtual rainbow::memory::AllocationRequirements
        maxAllocationRequirements() const;
    virtual bool
        fits(const rainbow::memory::AllocationRequirements& requirements) const;

    rainbow::memory::Allocation
        allocate(const rainbow::memory::AllocationRequirements& requirements);
    virtual rainbow::memory::Allocation allocate();
    virtual rainbow::memory::Allocation allocate(const std::size_t bytes);
    virtual rainbow::memory::Allocation
        allocateAligned(const std::size_t bytes, const std::size_t boundary);
    virtual rainbow::memory::Allocation reallocate(
        const rainbow::memory::Block& originalBlock, const std::size_t bytes);
    virtual rainbow::memory::Allocation reallocateAligned(
        const rainbow::memory::Block& originalBlock,
        const std::size_t             bytes,
        const std::size_t             boundary);

    virtual rainbow::memory::Deallocation
        free(const rainbow::memory::Block& block);
};

} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_ALLOCATOR_HPP_INCLUDED
