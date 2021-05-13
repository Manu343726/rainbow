#include "rainbow/result.hpp"
#include <rainbow/memory/allocators/hierarchical.hpp>
#ifndef NDEBUG
#include <rainbow/iterator_traits.hpp>
#endif // NDEBUG

using namespace rainbow::memory;
using namespace rainbow::memory::allocators;

Hierarchical::Hierarchical(
    Allocator&            parentAllocator,
    ChildAllocatorFactory childAllocatorFactory,
    Allocator&            metadataVectorAllocator,
    Allocator&            metadataNodeAllocator)
    : _childrenAllocators{metadataVectorAllocator},
      _parentAllocator{&parentAllocator},
      _uniquePtrAllocator{&metadataNodeAllocator},
      _childAllocatorFactory{std::move(childAllocatorFactory)}
{
}

AllocationRequirements Hierarchical::minimalAllocationRequirements() const
{
    AllocationRequirements result;

    result.alignment = alignof(void*);
    result.extraSize = sizeof(void*);

    return result;
}

Allocator::Features Hierarchical::features() const
{
    Features result{};

    for(const auto& allocator : _childrenAllocators.view())
    {
        result = result | allocator->features();
    }

    return result;
}

namespace detail
{
std::optional<std::size_t>
    add(const std::optional<std::size_t>& lhs,
        const std::optional<std::size_t>  rhs)
{
    if(lhs and rhs)
    {
        return *lhs + *rhs;
    }
    else if(lhs)
    {
        return lhs;
    }
    else if(rhs)
    {
        return rhs;
    }
    else
    {
        return std::nullopt;
    }
}

} // namespace detail

Allocator::Info Hierarchical::info() const
{
    Info result;
    result.parent = _parentAllocator;

    for(const auto& allocator : _childrenAllocators.view())
    {
        const Info info = allocator->info();

        result.free     = ::detail::add(result.free, info.free);
        result.reserved = ::detail::add(result.reserved, info.reserved);
        result.used     = ::detail::add(result.used, info.used);
    }

    return result;
}

Allocation Hierarchical::allocate(const std::size_t bytes)
{
    for(auto& allocator : _childrenAllocators.view())
    {
        if(const auto block = allocator->allocate(bytes + sizeof(void*)))
        {
            rainbow::memory::bit_write(allocator.get(), block.begin());
            return userBlock(block);
        }
        else if(block.result() != AllocationResult::NoSpaceLeft)
        {
            return block.result();
        }
    }


    RAINBOW_RESULT_TRY(grow());

    assert(_childrenAllocators.view().back() != nullptr);

    return allocate(bytes);
}

Allocation Hierarchical::allocateAligned(
    const std::size_t bytes, const std::size_t boundary)
{
    for(auto& allocator : _childrenAllocators.view())
    {
        if(const auto block =
               allocator->allocateAligned(bytes + sizeof(void*), boundary))
        {
            rainbow::memory::bit_write(allocator.get(), block.begin());
            return userBlock(block);
        }
        else if(block.result() != AllocationResult::NoSpaceLeft)
        {
            return block.result();
        }
    }

    RAINBOW_RESULT_TRY(grow());

    assert(_childrenAllocators.view().back() != nullptr);

    return allocateAligned(bytes, boundary);
}

Allocation Hierarchical::reallocate(
    const rainbow::memory::Block& original, const std::size_t bytes)
{
    for(auto& allocator : _childrenAllocators.view())
    {
        if(const auto block =
               allocator->reallocate(original, bytes + sizeof(void*)))
        {
            rainbow::memory::bit_write(allocator.get(), block.begin());
            return userBlock(block);
        }
        else if(block.result() != AllocationResult::NoSpaceLeft)
        {
            return block.result();
        }
    }

    RAINBOW_RESULT_TRY(grow());

    assert(_childrenAllocators.view().back() != nullptr);

    return reallocate(original, bytes);
}

Allocation Hierarchical::reallocateAligned(
    const rainbow::memory::Block& original,
    const std::size_t             bytes,
    const std::size_t             boundary)
{
    for(auto& allocator : _childrenAllocators.view())
    {
        if(const auto block = allocator->reallocateAligned(
               original, bytes + sizeof(void*), boundary))
        {
            rainbow::memory::bit_write(allocator.get(), block.begin());
            return userBlock(block);
        }
        else if(block.result() != AllocationResult::NoSpaceLeft)
        {
            return block.result();
        }
    }

    RAINBOW_RESULT_TRY(grow());

    assert(_childrenAllocators.view().back() != nullptr);

    return reallocateAligned(original, bytes, boundary);
}

Deallocation Hierarchical::free(const rainbow::memory::Block& userBlock)
{
    assert(userBlock != nullptr);
    const auto block     = allocatedBlock(userBlock);
    auto       allocator = rainbow::memory::bit_cast<Allocator*>(block.begin());

    assert(std::any_of(
        _childrenAllocators.view().begin(),
        _childrenAllocators.view().end(),
        [=](const auto& allocatorPtr) {
            return allocatorPtr.get() == allocator;
        }));

    return allocator->free(block);
}

rainbow::memory::AllocationResult Hierarchical::grow()
{
    RAINBOW_RESULT_TRY(
        newAllocatorAllocation,
        _childAllocatorFactory(*_uniquePtrAllocator, *_parentAllocator, *this));

    return _childrenAllocators.push_back(std::move(newAllocatorAllocation))
        .result();
}

Block Hierarchical::userBlock(const Block& allocatedBlock)
{
    return allocatedBlock.shrinkFromBegin(sizeof(void*));
}

Block Hierarchical::allocatedBlock(const Block& userBlock)
{
    return {
        arithmeticPointer(userBlock.begin()) - sizeof(void*),
        userBlock.size() + sizeof(void*)};
}
