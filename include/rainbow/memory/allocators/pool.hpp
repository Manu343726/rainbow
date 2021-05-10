#ifndef RAINBOW_MEMORY_ALLOCATORS_POOL_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATORS_POOL_HPP_INCLUDED

#include <rainbow/memory/allocator.hpp>

namespace rainbow::memory::allocators
{

class Pool final : public rainbow::memory::Allocator
{
public:
    static constexpr Features FEATURES =
        Features::Allocate | Features::AllocateAligned | Features::Free;

    struct Parameters
    {
        std::size_t maxSize;
        std::size_t maxAlignment;
        std::size_t count;
    };

    Pool(rainbow::memory::Block storage, const Parameters& parameters);

    static rainbow::memory::AllocationRequirements
        storageRequirements(const Parameters& parameters);

    Features features() const override;
    Info     info() const override;

    rainbow::memory::Block allocate(const std::size_t bytes) override;
    rainbow::memory::Block allocateAligned(
        const std::size_t bytes, const std::size_t boundary) override;
    bool free(const rainbow::memory::Block& block) override;

private:
    rainbow::memory::Block _nextBlock;
    std::size_t            _blockSize;
    std::size_t            _maxAlignment;
    rainbow::memory::Block _storage;

    std::size_t maxItemSize() const;
};

} // namespace rainbow::memory::allocators

#endif // RAINBOW_MEMORY_ALLOCATORS_POOL_HPP_INCLUDED
