#include <rainbow/containers/vector.hpp>
#include <utility>

using namespace rainbow::containers::raw;

Vector::Vector(const rainbow::Type* type, rainbow::memory::Allocator& allocator)
    : _size{0}, _type{std::move(type)}, _allocator{&allocator}
{
}

rainbow::memory::Allocator& Vector::allocator() const
{
    return *_allocator;
}

rainbow::memory::Block Vector::alignedStorage() const
{
    return _storage.aligned(_type->alignment());
}

std::size_t Vector::size() const
{
    return _size;
}

bool Vector::empty() const
{
    return _size == 0;
}

std::size_t Vector::capacity() const
{
    return alignedStorage().size() / _type->size();
}

rainbow::memory::Block Vector::storageInUse() const
{
    return alignedStorage().left(_size * _type->size());
}

bool Vector::reserve(const std::size_t size)
{
    if(size <= capacity())
    {
        return true;
    }

    float growRate = 1.5f;

    if(size >= capacity() * growRate)
    {
        growRate = 1.0f;
    }

    if(const auto newStorage = _allocator->allocateAligned(
           growRate * size * _type->size(), _type->alignment()))
    {
        if(const auto oldStorage = std::exchange(_storage, newStorage))
        {
            _type->moveArray(
                oldStorage.aligned(_type->alignment()),
                newStorage.aligned(_type->alignment()),
                _size);

            return _allocator->free(oldStorage);
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool Vector::resize(const std::size_t size)
{
    if(size < _size and _storage)
    {
        _type->destroyArray(
            alignedStorage().right(size * _type->size()), _size - size);

        _size = size;

        return true;
    }
    else if(size == _size)
    {
        return true;
    }
    else
    {
        if(not reserve(size))
        {
            return false;
        }

        _type->defaultConstructArray(
            alignedStorage().right((size - _size) * _type->size()),
            size - _size);

        _size = size;

        return true;
    }
}

rainbow::memory::Block Vector::push_back()
{
    if(resize(_size + 1))
    {
        return view().back();
    }
    else
    {
        return nullptr;
    }
}

bool Vector::pop_back()
{
    return not empty() and resize(_size - 1);
}

rainbow::views::raw::Vector Vector::view() const
{
    return {_type, storageInUse()};
}

Vector::~Vector()
{
    if(_storage)
    {
        _type->destroyArray(storageInUse(), size());
        _allocator->free(_storage);
        _storage = nullptr;
    }
}
