#ifndef RAINBOW_MEMORY_ALLOCATORS_HIERARCHICAL_HPP
#define RAINBOW_MEMORY_ALLOCATORS_HIERARCHICAL_HPP

#include <functional>
#include <rainbow/containers/vector.hpp>
#include <rainbow/memory/allocator.hpp>
#include <rainbow/memory/allocators/malloc.hpp>
#include <rainbow/memory/cast.hpp>
#include <rainbow/unique_ptr.hpp>

namespace rainbow::memory::allocators
{

class Hierarchical final : public rainbow::memory::Allocator
{
public:
    using ChildAllocatorFactory =
        std::function<rainbow::UniquePtr<rainbow::memory::Allocator>(
            rainbow::memory::Allocator& /* uniquePtrAllocator */,
            rainbow::memory::Allocator& /* parentAllocator */)>;

    Hierarchical(
        rainbow::memory::Allocator& parentAllocator,
        ChildAllocatorFactory       childAllocatorFactory,
        rainbow::memory::Allocator& metadataVectorAllocator =
            rainbow::memory::allocators::malloc(),
        rainbow::memory::Allocator& metadataNodeAllocator =
            rainbow::memory::allocators::malloc());

    Features               features() const override;
    Info                   info() const override;
    rainbow::memory::Block allocate(const std::size_t bytes) override;
    rainbow::memory::Block allocateAligned(
        const std::size_t bytes, const std::size_t boundary) override;
    rainbow::memory::Block reallocate(
        const rainbow::memory::Block& original,
        const std::size_t             bytes) override;

    rainbow::memory::Block reallocateAligned(
        const rainbow::memory::Block& original,
        const std::size_t             bytes,
        const std::size_t             boundary) override;
    bool free(const rainbow::memory::Block& block) override;

private:
    rainbow::containers::Vector<rainbow::UniquePtr<rainbow::memory::Allocator>>
                                _childrenAllocators;
    rainbow::memory::Allocator* _parentAllocator;
    rainbow::memory::Allocator* _uniquePtrAllocator;
    ChildAllocatorFactory       _childAllocatorFactory;

    static rainbow::memory::Block
        userBlock(const rainbow::memory::Block& allocatedBlock);
    static rainbow::memory::Block
        allocatedBlock(const rainbow::memory::Block& userBlock);

    bool grow();
};

} // namespace rainbow::memory::allocators

#endif // RAINBOW_MEMORY_ALLOCATORS_HIERARCHICAL_HPP
