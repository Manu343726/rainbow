#include <rainbow/type.hpp>

using namespace rainbow;

void Type::defaultConstruct(const rainbow::memory::Block& object) const
{
    assert(isDefaultConstructible());

    _specialFunctions.defaultConstruct(object);
}

void Type::copy(
    const rainbow::memory::Block& from, const rainbow::memory::Block& to) const
{
    assert(isCopyConstructible());

    _specialFunctions.copy(from, to);
}

void Type::move(
    const rainbow::memory::Block& from, const rainbow::memory::Block& to) const
{
    assert(isCopyConstructible());

    _specialFunctions.copy(from, to);
}

void Type::copyAssign(
    const rainbow::memory::Block& from, const rainbow::memory::Block& to) const
{
    assert(isCopyAssignable());

    _specialFunctions.copyAssign(from, to);
}

void Type::moveAssign(
    const rainbow::memory::Block& from, const rainbow::memory::Block& to) const
{
    assert(isMoveAssignable());

    _specialFunctions.moveAssign(from, to);
}

void Type::destroy(const rainbow::memory::Block& object) const
{
    _specialFunctions.destroy(object);
}

void Type::defaultConstructArray(
    const rainbow::memory::Block& array, const std::size_t count) const
{
    assert(array.size() >= size() * count);

    for(std::size_t i = 0; i < count; ++i)
    {
        defaultConstruct(array(i * size(), (i + 1) * size()));
    }
}

void Type::copyArray(
    const rainbow::memory::Block& from,
    const rainbow::memory::Block& to,
    const std::size_t             count) const
{
    assert(from.size() >= size() * count);
    assert(to.size() >= size() * count);

    for(std::size_t i = 0; i < count; ++i)
    {
        copy(
            from(i * size(), (i + 1) * size()),
            to(i * size(), (i + 1) * size()));
    }
}

void Type::moveArray(
    const rainbow::memory::Block& from,
    const rainbow::memory::Block& to,
    const std::size_t             count) const
{
    assert(from.size() >= size() * count);
    assert(to.size() >= size() * count);

    for(std::size_t i = 0; i < count; ++i)
    {
        move(
            from(i * size(), (i + 1) * size()),
            to(i * size(), (i + 1) * size()));
    }
}

void Type::copyAssignArray(
    const rainbow::memory::Block& from,
    const rainbow::memory::Block& to,
    const std::size_t             count) const
{
    assert(from.size() >= size() * count);
    assert(to.size() >= size() * count);

    for(std::size_t i = 0; i < count; ++i)
    {
        copyAssign(
            from(i * size(), (i + 1) * size()),
            to(i * size(), (i + 1) * size()));
    }
}

void Type::moveAssignArray(
    const rainbow::memory::Block& from,
    const rainbow::memory::Block& to,
    const std::size_t             count) const
{
    assert(from.size() >= size() * count);
    assert(to.size() >= size() * count);

    for(std::size_t i = 0; i < count; ++i)
    {
        moveAssign(
            from(i * size(), (i + 1) * size()),
            to(i * size(), (i + 1) * size()));
    }
}

void Type::destroyArray(
    const rainbow::memory::Block& array, const std::size_t count) const
{
    assert(array.size() >= size() * count);

    for(std::size_t i = 0; i < count; ++i)
    {
        destroy(array(i * size(), (i + 1) * size()));
    }
}
