#ifndef RAINBOW_MEMORY_OBJECTOPERATIONS_HPP_INCLUDED
#define RAINBOW_MEMORY_OBJECTOPERATIONS_HPP_INCLUDED

#include <cassert>
#include <rainbow/memory/block.hpp>
#include <rainbow/memory/cast.hpp>
#include <type_traits>
#include <utility>

namespace rainbow::memory
{

template<typename T>
void defaultConstruct(const rainbow::memory::Block& object)
{
    if constexpr(std::is_default_constructible_v<T>)
    {
        assert(object.isAligned(alignof(T)));
        assert(object.size() >= sizeof(T));

        new(object.begin()) T{};
    }
}

template<typename T, typename... Args>
void construct(const rainbow::memory::Block& object, Args&&... args)
{
    if constexpr(std::is_constructible_v<
                     T,
                     decltype(std::forward<Args>(args))...>)
    {
        assert(object != nullptr);
        assert(object.isAligned(alignof(T)));
        assert(object.size() >= sizeof(T));

        new(object.begin()) T{std::forward<Args>(args)...};
    }
}

template<typename T>
void copy(const rainbow::memory::Block& from, const rainbow::memory::Block& to)
{
    if constexpr(std::is_copy_constructible_v<T>)
    {
        assert(from != nullptr);
        assert(to != nullptr);

        rainbow::memory::construct<T>(
            to, *rainbow::memory::object_cast<T>(from));
    }
}

template<typename T>
void move(const rainbow::memory::Block& from, const rainbow::memory::Block& to)
{
    if constexpr(std::is_move_constructible_v<T>)
    {
        assert(from != nullptr);
        assert(to != nullptr);

        rainbow::memory::construct<T>(
            to, std::move(*rainbow::memory::object_cast<T>(from)));
    }
}

template<typename T>
void copyAssign(
    const rainbow::memory::Block& from, const rainbow::memory::Block& to)
{
    if constexpr(std::is_copy_assignable_v<T>)
    {
        assert(from != nullptr);
        assert(to != nullptr);

        *rainbow::memory::object_cast<T>(to) =
            *rainbow::memory::object_cast<T>(from);
    }
}

template<typename T>
void moveAssign(
    const rainbow::memory::Block& from, const rainbow::memory::Block& to)
{
    if constexpr(std::is_move_assignable_v<T>)
    {
        assert(from != nullptr);
        assert(to != nullptr);

        *rainbow::memory::object_cast<T>(to) =
            std::move(*rainbow::memory::object_cast<T>(from));
    }
}

template<typename T>
void destroy(const rainbow::memory::Block& object)
{
    assert(object != nullptr);
    rainbow::memory::object_cast<T>(object)->~T();
}
} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_OBJECTOPERATIONS_HPP_INCLUDED
