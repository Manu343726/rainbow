#ifndef RAINBOW_MEMORY_ALLOCATORS_OWNING_HPP
#define RAINBOW_MEMORY_ALLOCATORS_OWNING_HPP

#include <rainbow/memory/allocator.hpp>
#include <rainbow/memory/allocators/malloc.hpp>
#include <rainbow/unique_ptr.hpp>

namespace rainbow::memory::allocators
{

template<typename UserAllocator>
class Owning final : public rainbow::memory::Allocator
{
public:
    template<typename... Args>
    Owning(
        rainbow::memory::Allocator& parentAllocator,
        rainbow::memory::Block      storage,
        const Args&... args)
        : _parentAllocator{&parentAllocator},
          _storage{parentAllocator, std::move(storage)},
          _allocator{_storage.get(), args...}
    {
    }
    Features features() const override
    {
        return _allocator.features();
    }

    Info info() const override
    {
        auto result   = _allocator.info();
        result.parent = _parentAllocator;

        return result;
    }

    rainbow::memory::Block allocate(const std::size_t bytes) override
    {
        return _allocator.allocate(bytes);
    }

    rainbow::memory::Block allocateAligned(
        const std::size_t bytes, const std::size_t boundary) override
    {
        return _allocator.allocateAligned(bytes, boundary);
    }

    rainbow::memory::Block reallocate(
        const rainbow::memory::Block& original,
        const std::size_t             bytes) override
    {
        return _allocator.reallocate(original, bytes);
    }

    rainbow::memory::Block reallocateAligned(
        const rainbow::memory::Block& original,
        const std::size_t             bytes,
        const std::size_t             boundary) override
    {
        return reallocateAligned(original, bytes, boundary);
    }

    bool free(const rainbow::memory::Block& block) override
    {
        return _allocator.free(block);
    }

private:
    rainbow::memory::Allocator* _parentAllocator;
    rainbow::raw::UniquePtr     _storage;
    UserAllocator               _allocator;
};

template<typename Allocator, typename... Args>
rainbow::UniquePtr<Owning<Allocator>> makeOwning(
    rainbow::memory::Allocator& uniquePtrAllocator,
    rainbow::memory::Allocator& parentAllocator,
    const Args&... args)
{
    if(auto storage =
           parentAllocator.allocate(Allocator::storageRequirements(args...)))
    {
        return rainbow::makeUnique<Owning<Allocator>>(
            uniquePtrAllocator, parentAllocator, std::move(storage), args...);
    }

    return nullptr;
}

template<typename Allocator, typename... Args>
rainbow::UniquePtr<Owning<Allocator>>
    makeOwning(rainbow::memory::Allocator& parentAllocator, const Args&... args)
{
    return makeOwning(
        rainbow::memory::allocators::malloc(), parentAllocator, args...);
}

} // namespace rainbow::memory::allocators

#endif // RAINBOW_MEMORY_ALLOCATORS_OWNING_HPP
