#ifndef RAINBOW_VIEWS_VECTOR_HPP
#define RAINBOW_VIEWS_VECTOR_HPP

#include <rainbow/memory/block.hpp>
#include <rainbow/object.hpp>
#include <rainbow/object/iterator.hpp>
#include <rainbow/type.hpp>
#include <utility>

namespace rainbow::views
{

namespace raw
{

class Vector
{
public:
    Vector(const rainbow::Type* type, rainbow::memory::Block storage);

    std::size_t                      size() const;
    rainbow::memory::Block::Iterator begin() const;
    rainbow::memory::Block::Iterator end() const;
    rainbow::memory::Block           at(const std::size_t i) const;
    rainbow::memory::Block           operator[](const std::size_t i) const;
    rainbow::memory::Block           front() const;
    rainbow::memory::Block           back() const;
    bool                             empty() const;

private:
    const rainbow::Type*   _type;
    rainbow::memory::Block _storage;
};

} // namespace raw

template<typename T>
class Vector : public raw::Vector
{
public:
    Vector(rainbow::memory::Block storage)
        : raw::Vector{rainbow::type_of<T>(), std::move(storage)}
    {
    }

    const T& at(const std::size_t i) const
    {
        return *rainbow::Object<T>{raw::Vector::at(i)}.get();
    }

    T& at(const std::size_t i)
    {
        return *rainbow::Object<T>{raw::Vector::at(i)}.get();
    }

    const T& operator[](const std::size_t i) const
    {
        return at(i);
    }

    T& operator[](const std::size_t i)
    {
        return at(i);
    }

    rainbow::object::Iterator<T> begin() const
    {
        return {raw::Vector::begin()};
    }

    rainbow::object::Iterator<T> end() const
    {
        return {raw::Vector::end()};
    }
};

} // namespace rainbow::views

#endif // RAINBOW_VIEWS_VECTOR_HPP
