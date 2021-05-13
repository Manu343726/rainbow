#ifndef RAINBOW_OBJECT_ITERATOR_HPP_INCLUDED
#define RAINBOW_OBJECT_ITERATOR_HPP_INCLUDED

#include <rainbow/memory/block.hpp>
#include <rainbow/object.hpp>
#include <utility>

namespace rainbow::object
{

template<typename T>
class Iterator : public rainbow::memory::Block::Iterator
{
private:
    using Base = rainbow::memory::Block::Iterator;

public:
    Iterator(Base it) : Base{std::move(it)} {}

    T* operator->() const
    {
        return rainbow::Object<T>{Base::operator*()}.get();
    }

    T& operator*() const
    {
        return *operator->();
    }

    Iterator& operator++()
    {
        Base::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        Iterator result = *this;
        ++(*this);
        return result;
    }

    Iterator& operator--()
    {
        Base::operator--();
        return *this;
    }

    Iterator operator--(int)
    {
        Iterator result = *this;
        --(*this);
        return result;
    }
};
} // namespace rainbow::object

#endif // RAINBOW_OBJECT_ITERATOR_HPP_INCLUDED
