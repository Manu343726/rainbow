#ifndef RAINBOW_OBJECT_HPP_INCLUDED
#define RAINBOW_OBJECT_HPP_INCLUDED

#include <cassert>
#include <rainbow/memory/allocator.hpp>
#include <utility>

namespace rainbow
{

template<typename T>
class Object : public rainbow::memory::Block
{
public:
    Object() : Object{nullptr} {}
    Object(std::nullptr_t) : rainbow::memory::Block{nullptr} {}
    Object(const rainbow::memory::Block& block)
        : rainbow::memory::Block{block.begin(), block.size()}
    {
    }

    T* get() const
    {
        return reinterpret_cast<T*>(aligned(alignof(std::decay_t<T>)).begin());
    }

    T* operator->() const
    {
        return get();
    }

    operator T&() const
    {
        return *get();
    }
};

template<typename T>
class ObjectIterator : public rainbow::memory::Block::Iterator
{
private:
    using Base = rainbow::memory::Block::Iterator;

public:
    ObjectIterator(Base it) : Base{std::move(it)} {}

    T* operator->() const
    {
        return Object<T>{Base::operator*()}.get();
    }

    T& operator*() const
    {
        return *operator->();
    }

    ObjectIterator& operator++()
    {
        Base::operator++();
        return *this;
    }

    ObjectIterator operator++(int)
    {
        ObjectIterator result = *this;
        ++(*this);
        return result;
    }

    ObjectIterator& operator--()
    {
        Base::operator--();
        return *this;
    }

    ObjectIterator operator--(int)
    {
        ObjectIterator result = *this;
        --(*this);
        return result;
    }
};

template<typename T>
class ObjectRange
{
public:
    ObjectRange(rainbow::memory::Block::Range range) : _range{std::move(range)}
    {
    }

    ObjectIterator<T> begin() const
    {
        return {_range.begin()};
    }

    ObjectIterator<T> end() const
    {
        return {_range.end()};
    }

private:
    rainbow::memory::Block::Range _range;
};

template<typename T, typename... Args>
Object<T> construct(const rainbow::memory::Block& block, Args&&... args)
{
    assert(block.isAligned(alignof(T)));
    assert(block.size() == sizeof(T));

    new(block.begin()) T{std::forward<Args>(args)...};

    return {block};
}

template<typename T>
void destroy(const Object<T>& object)
{
    object->~T();
}

template<typename T, typename... Args>
Object<T> new_(rainbow::memory::Allocator& allocator, Args&&... args)
{
    if(auto block = allocator.allocateAligned(sizeof(T), alignof(T)))
    {
        rainbow::construct<T>(
            block.aligned(alignof(T)), std::forward<Args>(args)...);

        return {block};
    }

    return {nullptr};
}

template<typename T>
bool delete_(rainbow::memory::Allocator& allocator, const Object<T>& object)
{
    rainbow::destroy(object);
    return allocator.free(object);
}

} // namespace rainbow

#endif // RAINBOW_OBJECT_HPP_INCLUDED
