#include <cassert>
#include <rainbow/memory/aligned.hpp>
#include <rainbow/memory/allocators/pool.hpp>
#include <rainbow/memory/cast.hpp>
#include <utility>

using namespace rainbow::memory;
using namespace rainbow::memory::allocators;

Pool::Pool(Block storage, const Parameters& parameters)
    : _nextBlock{storage.begin(), parameters.maxSize},
      _blockSize{parameters.maxSize + sizeof(void*)},
      _maxAlignment{parameters.maxAlignment},
      _storage{std::move(storage)}
{
    assert(_storage.size() >= storageRequirements(parameters).size);
    assert(
        not storageRequirements(parameters).alignment or
        _storage.isAligned(*storageRequirements(parameters).alignment));

    char* current = arithmeticPointer(_nextBlock.begin());
    char* next    = current + _blockSize;

    for(std::size_t i = 1; i < parameters.count; ++i)
    {
        bit_write(next, current);
        current = next;
        next += _blockSize;
    }

    bit_write(nullptr, current);
}

AllocationRequirements
    Pool::Pool::storageRequirements(const Parameters& parameters)
{
    AllocationRequirements result;
    result.size      = (parameters.maxSize + sizeof(void*)) * parameters.count;
    result.alignment = parameters.maxAlignment;

    return result;
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
    return _blockSize - sizeof(void*);
}

Block Pool::allocate(const std::size_t bytes)
{
    if(_nextBlock == nullptr or bytes > maxItemSize())
    {
        return nullptr;
    }

    const auto result = _nextBlock;
    _nextBlock        = Block{bit_cast<void*>(_nextBlock.begin()), _blockSize};
    return result;
}

Block Pool::allocateAligned(
    const std::size_t bytes, [[maybe_unused]] const std::size_t boundary)
{
    if(boundary > _maxAlignment)
    {
        return nullptr;
    }

    return allocate(bytes);
}

bool Pool::free(const rainbow::memory::Block& block)
{
    if(not _storage.contains(block))
    {
        return false;
    }

    bit_write(_nextBlock.begin(), block.begin());
    _nextBlock = block;

    return true;
}
