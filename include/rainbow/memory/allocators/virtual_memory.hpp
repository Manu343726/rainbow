#ifndef RAINBOW_MEMORY_ALLOCATORS_VIRTUALMEMORY_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATORS_VIRTUALMEMORY_HPP_INCLUDED

#include <optional>
#include <rainbow/memory/allocator.hpp>
#include <rainbow/unique_ptr.hpp>

namespace rainbow::memory::allocators
{

class VirtualMemory final : public rainbow::memory::Allocator
{
public:
    static constexpr Features FEATURES = Features::Allocate | Features::Free |
                                         Features::OwningAllocator |
                                         Features::GrowingAllocator;

    Features features() const override;
    Info     info() const override;

    rainbow::memory::Allocation allocate(const std::size_t bytes) override;
    rainbow::memory::Deallocation
        free(const rainbow::memory::Block& block) override;
};

rainbow::memory::allocators::VirtualMemory& virtualMemory();

} // namespace rainbow::memory::allocators

#endif // RAINBOW_MEMORY_ALLOCATORS_VIRTUALMEMORY_HPP_INCLUDED
