#ifndef RAINBOW_MEMORY_ALLOCATORS_FIXEDSIZE_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATORS_FIXEDSIZE_HPP_INCLUDED

#include <rainbow/memory/allocator.hpp>

namespace rainbow::memory::allocators
{

class FixedSize final : public rainbow::memory::Allocator
{
public:
    FixedSize(rainbow::memory::Block storage);
    FixedSize(
        rainbow::memory::Allocator& parent, const std::size_t storageSize);

    static constexpr Features FEATURES =
        Features::Allocate | Features::AllocateAligned | Features::Free;

    Features features() const override;
    Info     info() const override;

    rainbow::memory::Block allocate(const std::size_t bytes) override;
    rainbow::memory::Block allocateAligned(
        const std::size_t bytes, const std::size_t boundary) override;
    bool free(const rainbow::memory::Block& block) override;

private:
    rainbow::memory::Block      _storage;
    std::size_t                 _used;
    rainbow::memory::Allocator* _parent;

    std::size_t            freeSpace() const;
    rainbow::memory::Block freeStorage() const;
};

} // namespace rainbow::memory::allocators

#endif // RAINBOW_MEMORY_ALLOCATORS_FIXEDSIZE_HPP_INCLUDED
