#include <rainbow/unique_ptr.hpp>
#include <utility>

using namespace rainbow::raw;
using namespace rainbow::memory;

UniquePtr::UniquePtr(std::nullptr_t) : UniquePtr{} {}
UniquePtr::UniquePtr(Allocator& allocator, Object object)
    : _object{std::move(object)}, _allocator{&allocator}
{
}

UniquePtr::UniquePtr(Allocator& allocator, Block memory)
    : _object{memory}, _allocator{&allocator}
{
}

UniquePtr::UniquePtr(UniquePtr&& other)
    : _object{std::exchange(other._object, nullptr)},
      _allocator{std::exchange(other._allocator, nullptr)}
{
}

UniquePtr& UniquePtr::operator=(UniquePtr&& other)
{
    std::swap(_object, other._object);
    std::swap(_allocator, other._allocator);

    return *this;
}

UniquePtr::~UniquePtr()
{
    if(_allocator != nullptr and _object != nullptr)
    {
        _object.type()->destroy(_object.get());
        _allocator->free(_object);

        _object    = nullptr;
        _allocator = nullptr;
    }
}

const rainbow::Type* UniquePtr::type() const
{
    return _object.type();
}

Allocator& UniquePtr::allocator() const
{
    return *_allocator;
}

Object UniquePtr::release()
{
    _allocator = nullptr;
    return std::exchange(_object, nullptr);
}

const Object& UniquePtr::get() const
{
    return _object;
}

const Object& UniquePtr::operator*() const
{
    return get();
}

const Object* UniquePtr::operator->() const
{
    return &_object;
}

bool UniquePtr::operator==(const UniquePtr& other) const
{
    return _object == other._object;
}

bool UniquePtr::operator!=(const UniquePtr& other) const
{
    return !(*this == other);
}

bool UniquePtr::operator==(void* other) const
{
    return get().begin() == other;
}

bool UniquePtr::operator!=(void* other) const
{
    return !(*this == other);
}

bool UniquePtr::isNull() const
{
    return _object == nullptr;
}

UniquePtr::operator bool() const
{
    return not isNull();
}
