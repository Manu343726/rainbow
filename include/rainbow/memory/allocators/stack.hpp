#ifndef RAINBOW_MEMORY_ALLOCATORS_STACK_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATORS_STACK_HPP_INCLUDED

#include <rainbow/memory/allocator.hpp>

namespace rainbow::memory::allocators
{

class Stack final : public rainbow::memory::Allocator
{
public:
    Stack(rainbow::memory::Block storage);

    static constexpr Features FEATURES =
        Features::Allocate | Features::AllocateAligned | Features::Free;

    Features features() const override;
    Info     info() const override;

    rainbow::memory::Allocation allocate(const std::size_t bytes) override;
    rainbow::memory::Allocation allocateAligned(
        const std::size_t bytes, const std::size_t boundary) override;
    rainbow::memory::Deallocation
        free(const rainbow::memory::Block& block) override;

private:
    rainbow::memory::Block _storage;
    std::size_t            _used;

    std::size_t            freeSpace() const;
    rainbow::memory::Block freeStorage() const;
    rainbow::memory::Block usedStorage() const;
};

} // namespace rainbow::memory::allocators

#endif // RAINBOW_MEMORY_ALLOCATORS_STACK_HPP_INCLUDED
