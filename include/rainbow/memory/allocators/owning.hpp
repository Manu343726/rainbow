#ifndef RAINBOW_MEMORY_ALLOCATORS_OWNING_HPP
#define RAINBOW_MEMORY_ALLOCATORS_OWNING_HPP

#include <optional>
#include <rainbow/memory/allocator.hpp>
#include <rainbow/memory/allocators/malloc.hpp>
#include <rainbow/unique_ptr.hpp>

namespace rainbow::memory::allocators
{

template<typename UserAllocator>
class Owning final : public rainbow::memory::Allocator
{
public:
    static constexpr Features FEATURES =
        UserAllocator::FEATURES | Features::OwningAllocator;

    template<typename... Args>
    Owning(
        rainbow::memory::Allocator& parentAllocator,
        rainbow::memory::Block      storage,
        const std::size_t           storageAlignment,
        const Args&... args)
        : _parentAllocator{&parentAllocator},
          _storage{parentAllocator, std::move(storage)},
          _allocator{_storage->aligned(storageAlignment), args...}
    {
    }
    Features features() const override
    {
        return FEATURES;
    }

    Info info() const override
    {
        auto result   = _allocator.info();
        result.parent = _parentAllocator;

        return result;
    }

    rainbow::memory::Allocation allocate(const std::size_t bytes) override
    {
        return _allocator.allocate(bytes);
    }

    rainbow::memory::Allocation allocateAligned(
        const std::size_t bytes, const std::size_t boundary) override
    {
        return _allocator.allocateAligned(bytes, boundary);
    }

    rainbow::memory::Allocation reallocate(
        const rainbow::memory::Block& original,
        const std::size_t             bytes) override
    {
        return _allocator.reallocate(original, bytes);
    }

    rainbow::memory::Allocation reallocateAligned(
        const rainbow::memory::Block& original,
        const std::size_t             bytes,
        const std::size_t             boundary) override
    {
        return reallocateAligned(original, bytes, boundary);
    }

    rainbow::memory::Deallocation
        free(const rainbow::memory::Block& block) override
    {
        return _allocator.free(block);
    }

private:
    rainbow::memory::Allocator* _parentAllocator;
    rainbow::raw::UniquePtr     _storage;
    UserAllocator               _allocator;
};

namespace detail
{

template<typename Allocator, typename... Args>
rainbow::UniquePtrAllocation<Owning<Allocator>> makeOwning(
    rainbow::memory::Allocator&   uniquePtrAllocator,
    rainbow::memory::Allocator&   parentAllocator,
    const AllocationRequirements& storageRequirements,
    const Args&... args)
{
    RAINBOW_RESULT_TRY(storage, parentAllocator.allocate(storageRequirements));

    return rainbow::makeUnique<Owning<Allocator>>(
        uniquePtrAllocator,
        parentAllocator,
        std::move(storage),
        storageRequirements.alignment,
        args...);
}
} // namespace detail

template<typename Allocator, typename... Args>
rainbow::UniquePtrAllocation<Owning<Allocator>>
    makeOwningConstrainedThroughManager(
        rainbow::memory::Allocator& uniquePtrAllocator,
        rainbow::memory::Allocator& parentAllocator,
        rainbow::memory::Allocator& managerAllocator,
        const Args&... args)
{
    return rainbow::memory::allocators::detail::makeOwning<Allocator>(
        uniquePtrAllocator,
        parentAllocator,
        Allocator::storageRequirements(
            args..., &parentAllocator, &managerAllocator),
        args...);
}

template<typename Allocator, typename... Args>
rainbow::UniquePtrAllocation<Owning<Allocator>> makeOwningConstrained(
    rainbow::memory::Allocator& uniquePtrAllocator,
    rainbow::memory::Allocator& parentAllocator,
    const Args&... args)
{
    return rainbow::memory::allocators::detail::makeOwning<Allocator>(
        uniquePtrAllocator,
        parentAllocator,
        Allocator::storageRequirements(args..., &parentAllocator),
        args...);
}

template<typename Allocator, typename... Args>
rainbow::UniquePtrAllocation<Owning<Allocator>> makeOwningUnconstrained(
    rainbow::memory::Allocator&                    uniquePtrAllocator,
    rainbow::memory::Allocator&                    parentAllocator,
    const rainbow::memory::AllocationRequirements& storageRequirements)
{
    return rainbow::memory::allocators::detail::makeOwning<Allocator>(
        uniquePtrAllocator, parentAllocator, storageRequirements);
}

template<typename Allocator, typename... Args>
rainbow::UniquePtrAllocation<Owning<Allocator>> makeOwning(
    rainbow::memory::Allocator& uniquePtrAllocator,
    rainbow::memory::Allocator& parentAllocator,
    const Args&... args)
{
    if constexpr(
        rainbow::memory::features<Allocator>() &
        rainbow::memory::AllocatorFeatures::ConstrainedAllocation)
    {
        return rainbow::memory::allocators::makeOwningConstrained<Allocator>(
            uniquePtrAllocator, parentAllocator, args...);
    }
    else
    {
        return rainbow::memory::allocators::makeOwningUnconstrained<Allocator>(
            uniquePtrAllocator, parentAllocator, args...);
    }
}

template<typename Allocator, typename... Args>
rainbow::UniquePtrAllocation<Owning<Allocator>> makeOwningThroughManager(
    rainbow::memory::Allocator& uniquePtrAllocator,
    rainbow::memory::Allocator& parentAllocator,
    rainbow::memory::Allocator& managerAllocator,
    const Args&... args)
{
    if constexpr(
        rainbow::memory::features<Allocator>() &
        rainbow::memory::AllocatorFeatures::ConstrainedAllocation)
    {
        return rainbow::memory::allocators::makeOwningConstrainedThroughManager<
            Allocator>(
            uniquePtrAllocator, parentAllocator, managerAllocator, args...);
    }
    else
    {
        return rainbow::memory::allocators::makeOwningUnconstrained<Allocator>(
            uniquePtrAllocator, parentAllocator, args...);
    }
}

} // namespace rainbow::memory::allocators

#endif // RAINBOW_MEMORY_ALLOCATORS_OWNING_HPP
