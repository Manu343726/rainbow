#ifndef RAINBOW_OBJECT_HPP_INCLUDED
#define RAINBOW_OBJECT_HPP_INCLUDED

#include <cassert>
#include <rainbow/memory/allocator.hpp>
#include <rainbow/type.hpp>
#include <utility>

namespace rainbow
{

template<typename T>
class Object;

namespace raw
{

class Object : public rainbow::memory::Block
{
public:
    Object();
    Object(std::nullptr_t);
    Object(
        const rainbow::memory::Block& block,
        const rainbow::Type*          type = rainbow::trivialType());

    template<typename T>
    Object(const rainbow::Object<T>& object) : Object{object, object.type()}
    {
    }

    const rainbow::Type*   type() const;
    rainbow::memory::Block get() const;

private:
    const rainbow::Type* _type;
};

} // namespace raw

template<typename T>
class Object : public rainbow::raw::Object
{
public:
    Object() : Object{nullptr} {}
    Object(std::nullptr_t) : rainbow::raw::Object{nullptr} {}
    Object(const rainbow::raw::Object& object) : rainbow::raw::Object{object}
    {
        assert(object == nullptr or object.type() == rainbow::type_of<T>());
    }
    Object(const rainbow::memory::Block& block)
        : rainbow::raw::Object{block, rainbow::type_of<T>()}
    {
    }

    template<
        typename U,
        typename = std::enable_if_t<
            std::is_base_of_v<T, U> or std::is_base_of_v<U, T>>>
    Object<U> cast() const
    {
        return {static_cast<const rainbow::memory::Block&>(*this)};
    }

    T* get() const
    {
        return reinterpret_cast<T*>(rainbow::raw::Object::get().begin());
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

template<typename T, typename... Args>
Object<T> construct(const rainbow::memory::Block& block, Args&&... args)
{
    rainbow::memory::construct<T>(block, std::forward<Args>(args)...);
    return {block};
}

void destroy(const rainbow::raw::Object& object);

} // namespace rainbow

#endif // RAINBOW_OBJECT_HPP_INCLUDED
