#ifndef RAINBOW_MEMORY_ALLOCATORS_MALLOC_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATORS_MALLOC_HPP_INCLUDED

#include <rainbow/memory/allocator.hpp>

namespace rainbow::memory::allocators
{

class Malloc final : public rainbow::memory::Allocator
{
public:
    static constexpr Features FEATURES =
        Features::GrowingAllocator | Features::Allocate | Features::Reallocate |
        Features::AllocateAligned | Features::Free;

    Features features() const override;
    Info     info() const override;

    rainbow::memory::Allocation allocate(const std::size_t bytes) override;
    rainbow::memory::Allocation allocateAligned(
        const std::size_t bytes, const std::size_t boundary) override;
    rainbow::memory::Allocation reallocate(
        const rainbow::memory::Block& original,
        const std::size_t             bytes) override;
    rainbow::memory::Deallocation
        free(const rainbow::memory::Block& block) override;
};

Malloc& malloc();

} // namespace rainbow::memory::allocators

#endif // RAINBOW_MEMORY_ALLOCATORS_MALLOC_HPP_INCLUDED
