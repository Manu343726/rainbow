#include <cassert>
#include <rainbow/views/vector.hpp>
#include <utility>

namespace rainbow::views::raw
{

Vector::Vector(const rainbow::Type* type, rainbow::memory::Block storage)
    : _type{std::move(type)}, _storage{std::move(storage)}
{
    assert(_storage.size() % _type->size() == 0);
    assert(_storage.isAligned(_type->alignment()));
}

std::size_t Vector::size() const
{
    return _storage.size() / _type->size();
}

bool Vector::empty() const
{
    return _storage.empty();
}

rainbow::memory::Block::Iterator Vector::begin() const
{
    return _storage.iterate(_type->size()).begin();
}

rainbow::memory::Block::Iterator Vector::end() const
{
    return _storage.iterate(_type->size()).end();
}

rainbow::memory::Block Vector::at(const std::size_t i) const
{
    return *(begin() + i);
}

rainbow::memory::Block Vector::operator[](const std::size_t i) const
{
    return at(i);
}

rainbow::memory::Block Vector::front() const
{
    return at(0);
}

rainbow::memory::Block Vector::back() const
{
    assert(not empty());
    return at(size() - 1);
}

} // namespace rainbow::views::raw
