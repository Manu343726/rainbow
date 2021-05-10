#include <cassert>
#include <rainbow/memory/block.hpp>
#include <utility>

using namespace rainbow::memory;

Block::Block() : Block{nullptr} {}

Block::Block(std::nullptr_t) : Block{nullptr, 0} {}

Block::Block(void* begin, const std::size_t size)
    : _begin{arithmeticPointer(begin)}, _size{size}
{
}

Block::Block(Block&& other)
    : _begin{std::exchange(other._begin, nullptr)},
      _size{std::exchange(other._size, 0)}
{
}

Block& Block::operator=(Block&& other)
{
    _begin = std::exchange(other._begin, nullptr);
    _size  = std::exchange(other._size, 0);

    return *this;
}

void* Block::begin() const
{
    return pointer(_begin);
}

void* Block::end() const
{
    return pointer(_begin + _size);
}

std::size_t Block::size() const
{
    return _size;
}

bool Block::isNull() const
{
    return _begin == nullptr;
}

Block::operator bool() const
{
    return not isNull();
}

bool Block::empty() const
{
    return _size == 0;
}

bool Block::operator==(const std::nullptr_t) const
{
    return isNull();
}

bool Block::operator!=(const std::nullptr_t) const
{
    return not isNull();
}

bool Block::operator==(const Block& other) const
{
    return _begin == other._begin and _size == other._size;
}

bool Block::operator!=(const Block& other) const
{
    return !(*this == other);
}

Block Block::operator+(const std::ptrdiff_t offset) const
{
    return {_begin + offset, _size};
}

Block Block::operator-(const std::ptrdiff_t offset) const
{
    return {_begin - offset, _size};
}

bool Block::contains(void* address) const
{
    return begin() <= address and address <= end();
}

bool Block::contains(const Block& other) const
{
    return contains(other.begin()) and _size < other._size;
}

bool Block::isAligned(const std::size_t boundary) const
{
    return rainbow::memory::isAligned(begin(), boundary);
}

Block Block::shrinkFromBegin(const std::size_t bytesToRemove) const
{
    if(bytesToRemove > _size)
    {
        return nullptr;
    }

    return {_begin + bytesToRemove, _size - bytesToRemove};
}

Block Block::shrinkFromEnd(const std::size_t bytesToRemove) const
{
    if(bytesToRemove > _size)
    {
        return nullptr;
    }

    return {_begin, _size - bytesToRemove};
}

Block Block::left(const std::size_t bytes) const
{
    if(bytes > _size)
    {
        return nullptr;
    }

    return shrinkFromEnd(_size - bytes);
}

Block Block::right(const std::size_t bytes) const
{
    if(bytes > _size)
    {
        return nullptr;
    }

    return shrinkFromBegin(_size - bytes);
}

Block Block::aligned(const std::size_t boundary) const
{
    if(isAligned(boundary))
    {
        return *this;
    }

    return shrinkFromBegin(
        rainbow::memory::distanceToNextAligned(begin(), boundary));
}

Block Block::aligned(const std::size_t boundary, const std::size_t size) const
{
    assert(size <= _size);

    if(isAligned(boundary))
    {
        return shrinkFromEnd(_size - size);
    }

    const auto begin = _begin + rainbow::memory::distanceToNextAligned(
                                    this->begin(), boundary);

    if(begin + size > end())
    {
        return nullptr;
    }

    return {begin, size};
}

Block::Range Block::iterate(const std::size_t subBlockSize) const
{
    return {*this, subBlockSize};
}

Block::Range::Range(const Block& begin, const Block& end)
    : _begin{begin}, _end{end}
{
}

Block::Range::Range(const Block& storage, const std::size_t subBlockSize)
    : Range{{storage.begin(), subBlockSize}, {storage.end(), subBlockSize}}
{
    assert(storage.size() % subBlockSize == 0);
}

Block::Iterator Block::Range::begin() const
{
    return _begin;
}

Block::Iterator Block::Range::end() const
{
    return _end;
}

Block::Iterator::Iterator(const Block& subBlock)
    : _begin{rainbow::memory::arithmeticPointer(subBlock.begin())},
      _size{subBlock.size()}
{
}

Block::Iterator& Block::Iterator::operator++()
{
    _begin += _size;
    return *this;
}

Block::Iterator Block::Iterator::operator++(int)
{
    auto result = *this;
    ++(*this);
    return result;
}

Block::Iterator& Block::Iterator::operator--()
{
    _begin -= _size;
    return *this;
}

Block::Iterator Block::Iterator::operator--(int)
{
    auto result = *this;
    --(*this);
    return result;
}

Block::Iterator Block::Iterator::operator+(const std::int64_t n) const
{
    return {Block{_begin + (_size * n), _size}};
}

Block::Iterator Block::Iterator::operator-(const std::int64_t n) const
{
    return (*this) + (-n);
}

Block Block::Iterator::operator*() const
{
    return {_begin, _size};
}

bool Block::Iterator::operator==(const Block::Iterator& other) const
{
    return *(*this) == *other;
}

bool Block::Iterator::operator!=(const Block::Iterator& other) const
{
    return !(*this == other);
}

Block Block::subBlock(const std::size_t begin, const std::size_t end) const
{
    assert(begin <= end);
    assert(begin <= _size);
    assert(end <= _size);

    return {_begin + begin, end - begin};
}

Block Block::operator()(const std::size_t begin, const std::size_t end) const
{
    return subBlock(begin, end);
}

Block Block::operator()(const std::size_t begin) const
{
    return (*this)(begin, _size);
}
