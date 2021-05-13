#include <rainbow/containers/vector.hpp>
#include <utility>

using namespace rainbow::containers::raw;

Vector::Vector(const rainbow::Type* type, rainbow::memory::Allocator& allocator)
    : _size{0}, _type{std::move(type)}, _allocator{&allocator}
{
    assert(
        (_allocator->features() &
         memory::Allocator::Features::AllocateAligned) and
        _allocator->features() & memory::Allocator::Features::Free);
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

rainbow::memory::AllocationResult Vector::reserve(const std::size_t size)
{
    if(size <= capacity())
    {
        return rainbow::memory::AllocationResult::Ok;
    }

    float growRate = 1.5f;

    if(size >= capacity() * growRate)
    {
        growRate = 1.0f;
    }

    RAINBOW_RESULT_TRY(
        newStorageAllocation,
        _allocator->allocateAligned(
            growRate * size * _type->size(), _type->alignment()));

    if(const auto oldStorage = std::exchange(_storage, newStorageAllocation))
    {
        _type->moveArray(
            oldStorage.aligned(_type->alignment()),
            newStorageAllocation.aligned(_type->alignment()),
            _size);

        return _allocator->free(oldStorage)
                   ? rainbow::memory::AllocationResult::Ok
                   : rainbow::memory::AllocationResult::CannotFreeOld;
    }

    return rainbow::memory::AllocationResult::Ok;
}

rainbow::memory::AllocationResult Vector::resize(const std::size_t size)
{
    if(size < _size and _storage)
    {
        _type->destroyArray(
            alignedStorage().right(size * _type->size()), _size - size);

        _size = size;

        return rainbow::memory::AllocationResult::Ok;
    }
    else if(size == _size)
    {
        return rainbow::memory::AllocationResult::Ok;
    }
    else
    {
        RAINBOW_RESULT_TRY(reserve(size));

        _type->defaultConstructArray(
            alignedStorage().right((size - _size) * _type->size()),
            size - _size);

        _size = size;

        return rainbow::memory::AllocationResult::Ok;
    }
}

rainbow::memory::Allocation Vector::push_back()
{
    RAINBOW_RESULT_TRY(resize(_size + 1));

    return view().back();
}

bool Vector::pop_back()
{
    return not empty() and
           resize(_size - 1) == rainbow::memory::AllocationResult::Ok;
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
