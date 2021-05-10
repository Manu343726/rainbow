#ifndef RAINBOW_MEMORY_ALLOCATOR_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATOR_HPP_INCLUDED

#include <cstdint>
#include <optional>
#include <rainbow/memory/aligned.hpp>
#include <rainbow/memory/block.hpp>
#include <type_traits>

namespace rainbow::memory
{

struct AllocationRequirements
{
    std::size_t                size;
    std::optional<std::size_t> alignment;
};

class Allocator
{
public:
    enum class Features
    {
        Allocate          = 0b1,
        AllocateAligned   = 0b10,
        Reallocate        = 0b100,
        ReallocateAligned = 0b1000,
        Free              = 0b10000,
        AllocateFixed     = 0b100000,
        GrowingAllocator  = 0b1000000
    };

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

    rainbow::memory::Block
        allocate(const rainbow::memory::AllocationRequirements& requirements);
    virtual rainbow::memory::Block allocate();
    virtual rainbow::memory::Block allocate(const std::size_t bytes);
    virtual rainbow::memory::Block
        allocateAligned(const std::size_t bytes, const std::size_t boundary);
    virtual rainbow::memory::Block reallocate(
        const rainbow::memory::Block& originalBlock, const std::size_t bytes);
    virtual rainbow::memory::Block reallocateAligned(
        const rainbow::memory::Block& originalBlock,
        const std::size_t             bytes,
        const std::size_t             boundary);

    virtual bool free(const rainbow::memory::Block& block);
};


constexpr Allocator::Features
    operator|(const Allocator::Features lhs, const Allocator::Features rhs)
{
    return static_cast<Allocator::Features>(
        static_cast<std::underlying_type_t<Allocator::Features>>(lhs) |
        static_cast<std::underlying_type_t<Allocator::Features>>(rhs));
}

constexpr Allocator::Features
    operator&(const Allocator::Features lhs, const Allocator::Features rhs)
{
    return static_cast<Allocator::Features>(
        static_cast<std::underlying_type_t<Allocator::Features>>(lhs) &
        static_cast<std::underlying_type_t<Allocator::Features>>(rhs));
}

} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_ALLOCATOR_HPP_INCLUDED
