#ifndef RAINBOW_TYPE_HPP
#define RAINBOW_TYPE_HPP

#include <cassert>
#include <cstdint>
#include <rainbow/memory/block.hpp>
#include <rainbow/memory/object_operations.hpp>
#include <type_traits>
#include <utility>

namespace rainbow
{

class Type
{
public:
    using DefaultConstruct = void (*)(const rainbow::memory::Block&);
    using Copy             = void (*)(
        const rainbow::memory::Block& /* from */,
        const rainbow::memory::Block& /* to */);
    using Move = void (*)(
        const rainbow::memory::Block& /* from */,
        const rainbow::memory::Block& /* to */);
    using CopyAssign = void (*)(
        const rainbow::memory::Block& /* from */,
        const rainbow::memory::Block& /* to */);
    using MoveAssign = void (*)(
        const rainbow::memory::Block& /* from */,
        const rainbow::memory::Block& /* to */);
    using Destroy = void (*)(const rainbow::memory::Block&);

    struct SpecialFunctions
    {
        DefaultConstruct defaultConstruct;
        Copy             copy;
        Move             move;
        CopyAssign       copyAssign;
        MoveAssign       moveAssign;
        Destroy          destroy;
    };

    constexpr Type(
        const std::size_t size,
        const std::size_t alignment,
        SpecialFunctions  specialFunctions)
        : _size{size},
          _alignment{alignment},
          _specialFunctions{std::move(specialFunctions)}
    {
    }

    constexpr std::size_t size() const
    {
        return _size;
    }

    constexpr std::size_t alignment() const
    {
        return _alignment;
    }

    constexpr bool isDefaultConstructible() const
    {
        return _specialFunctions.defaultConstruct != nullptr;
    }

    constexpr bool isCopyConstructible() const
    {
        return _specialFunctions.copy != nullptr;
    }

    constexpr bool isMoveConstructible() const
    {
        return _specialFunctions.move != nullptr;
    }

    constexpr bool isCopyAssignable() const
    {
        return _specialFunctions.copyAssign != nullptr;
    }

    constexpr bool isMoveAssignable() const
    {
        return _specialFunctions.moveAssign != nullptr;
    }

    void defaultConstruct(const rainbow::memory::Block& object) const;
    void copy(
        const rainbow::memory::Block& from,
        const rainbow::memory::Block& to) const;
    void move(
        const rainbow::memory::Block& from,
        const rainbow::memory::Block& to) const;
    void copyAssign(
        const rainbow::memory::Block& from,
        const rainbow::memory::Block& to) const;
    void moveAssign(
        const rainbow::memory::Block& from,
        const rainbow::memory::Block& to) const;
    void destroy(const rainbow::memory::Block& object) const;

    void defaultConstructArray(
        const rainbow::memory::Block& array, const std::size_t count) const;
    void copyArray(
        const rainbow::memory::Block& from,
        const rainbow::memory::Block& to,
        const std::size_t             count) const;
    void moveArray(
        const rainbow::memory::Block& from,
        const rainbow::memory::Block& to,
        const std::size_t             count) const;
    void copyAssignArray(
        const rainbow::memory::Block& from,
        const rainbow::memory::Block& to,
        const std::size_t             count) const;
    void moveAssignArray(
        const rainbow::memory::Block& from,
        const rainbow::memory::Block& to,
        const std::size_t             count) const;
    void destroyArray(
        const rainbow::memory::Block& array, const std::size_t count) const;

private:
    std::size_t      _size;
    std::size_t      _alignment;
    SpecialFunctions _specialFunctions;
};

template<typename T>
constexpr Type type()
{
    return {
        sizeof(T),
        alignof(T),
        {(std::is_default_constructible_v<T>
              ? rainbow::memory::defaultConstruct<T>
              : nullptr),
         (std::is_copy_constructible_v<T> ? rainbow::memory::copy<T> : nullptr),
         (std::is_move_constructible_v<T> ? rainbow::memory::move<T> : nullptr),
         (std::is_copy_assignable_v<T> ? rainbow::memory::copyAssign<T>
                                       : nullptr),
         (std::is_move_assignable_v<T> ? rainbow::memory::moveAssign<T>
                                       : nullptr),
         rainbow::memory::destroy<T>}};
}

template<typename T>
const Type* type_of()
{
    static constexpr Type type = rainbow::type<T>();
    return &type;
}

const Type* trivialType();

} // namespace rainbow

#endif // RAINBOW_TYPE_HPP
