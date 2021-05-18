#include <cassert>
#include <rainbow/memory/aligned.hpp>
#include <rainbow/memory/allocators/pool.hpp>
#include <rainbow/memory/cast.hpp>
#include <utility>

using namespace rainbow::memory;
using namespace rainbow::memory::allocators;

Pool::Pool(Block storage, const Parameters& parameters)
    : _nextBlock{storage.begin(), parameters.maxSize},
      _nodeRequirements{nodeRequirements(parameters)},
      _storage{std::move(storage)}
{
    char* current = arithmeticPointer(_nextBlock.begin());
    char* next    = current + blockSize();

    for(std::size_t i = 1; i < parameters.count; ++i)
    {
        bit_write(next, current);
        current = next;
        next += blockSize();
    }

    bit_write(nullptr, current);
}

AllocationRequirements Pool::minimalAllocationRequirements() const
{
    AllocationRequirements result;

    result.alignment = alignof(void*);
    result.extraSize = sizeof(void*);

    return result;
}

AllocationRequirements Pool::maxAllocationRequirements() const
{
    AllocationRequirements result;

    result.alignment = _nodeRequirements.alignment;
    result.extraSize = sizeof(void*);
    result.size      = maxItemSize();

    return result;
}

AllocationRequirements Pool::Pool::storageRequirements(
    const Parameters& parameters,
    const Allocator*  parentAllocator,
    const Allocator*  managerAllocator)
{
    AllocationRequirements result;
    const auto             nodeRequirements =
        Pool::nodeRequirements(parameters, parentAllocator, managerAllocator);
    result.size      = nodeRequirements.totalSize() * parameters.count;
    result.alignment = nodeRequirements.alignment;

    return result;
}

AllocationRequirements Pool::Pool::nodeRequirements(
    const Parameters& parameters,
    const Allocator*  parentAllocator,
    const Allocator*  managerAllocator)
{
    if(not parameters.satisfiesExternalRequirements)
    {
        return nodeRequirements(adaptParametersToRequirements(
            parameters, parentAllocator, managerAllocator));
    }

    AllocationRequirements result;
    result.size      = parameters.maxSize;
    result.alignment = parameters.maxAlignment;

    return result;
}

Pool::Parameters Pool::Pool::adaptParametersToRequirements(
    Parameters       parameters,
    const Allocator* parentAllocator,
    const Allocator* managerAllocator)
{
    if(parameters.satisfiesExternalRequirements)
    {
        return parameters;
    }

    AllocationRequirements externalRequirements;

    if(parentAllocator != nullptr)
    {
        externalRequirements +=
            parentAllocator->minimalAllocationRequirements();
    }

    if(managerAllocator != nullptr)
    {
        externalRequirements +=
            managerAllocator->minimalAllocationRequirements();
    }

    parameters.maxAlignment = std::max(
        std::max(parameters.maxAlignment, externalRequirements.alignment),
        alignof(void*));

    parameters.maxSize = rainbow::memory::nextAlignedSize(
        parameters.maxSize + externalRequirements.extraSize + sizeof(void*),
        parameters.maxAlignment);

    parameters.satisfiesExternalRequirements = true;

    return parameters;
}

std::size_t Pool::blockSize() const
{
    return _nodeRequirements.totalSize();
}

std::size_t Pool::blockAlignment() const
{
    return _nodeRequirements.alignment;
}

Allocator::Features Pool::features() const
{
    return FEATURES;
}

Allocator::Info Pool::info() const
{
    Info result;

    result.storage = _storage;

    return result;
}

std::size_t Pool::maxItemSize() const
{
    return blockSize() - sizeof(void*);
}

Allocation Pool::allocate(const std::size_t bytes)
{
    if(_nextBlock == nullptr)
    {
        return AllocationResult::NoSpaceLeft;
    }

    if(bytes > maxItemSize())
    {
        return AllocationResult::NoSpaceInNode;
    }

    const auto result = _nextBlock;
    _nextBlock        = Block{bit_cast<void*>(_nextBlock.begin()), blockSize()};
    return result;
}

Allocation Pool::allocateAligned(
    const std::size_t bytes, [[maybe_unused]] const std::size_t boundary)
{
    if(boundary > blockAlignment())
    {
        return AllocationResult::CannotAlign;
    }

    return allocate(bytes);
}

Deallocation Pool::free(const rainbow::memory::Block& block)
{
    if(not _storage.contains(block))
    {
        return DeallocationResult::UnknownNode;
    }

    bit_write(_nextBlock.begin(), block.begin());
    _nextBlock = block;

    return DeallocationResult::Ok;
}
