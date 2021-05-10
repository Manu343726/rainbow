#include <cassert>
#include <rainbow/memory/aligned.hpp>
#include <rainbow/memory/allocators/pool.hpp>
#include <rainbow/memory/cast.hpp>
#include <utility>

using namespace rainbow::memory;
using namespace rainbow::memory::allocators;

Pool::Pool(
    Block                              storage,
    const std::size_t                  maxSize,
    [[maybe_unused]] const std::size_t maxAlignment,
    const std::size_t                  count)
    : _nextBlock{storage.begin(), maxSize},
      _blockSize{maxSize + sizeof(void*)},
      _maxAlignment{maxAlignment},
      _storage{std::move(storage)}
{
    assert(_storage.size() >= _blockSize * count);

    char* current = arithmeticPointer(_nextBlock.begin());
    char* next    = current + _blockSize;

    for(std::size_t i = 1; i < count; ++i)
    {
        bit_write(next, current);
        current = next;
        next += _blockSize;
    }

    bit_write(nullptr, current);
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
