#ifndef RAINBOW_UNIQUEPTR_HPP_INCLUDED
#define RAINBOW_UNIQUEPTR_HPP_INCLUDED

#include <memory>
#include <rainbow/memory/allocator.hpp>
#include <rainbow/object.hpp>
#include <rainbow/object/new.hpp>
#include <rainbow/result.hpp>
#include <rainbow/type.hpp>
#include <type_traits>

namespace rainbow
{

namespace raw
{

class UniquePtr
{
public:
    UniquePtr() = default;
    UniquePtr(std::nullptr_t);
    UniquePtr(
        rainbow::memory::Allocator& allocator, rainbow::raw::Object object);
    UniquePtr(
        rainbow::memory::Allocator& allocator,
        rainbow::memory::Block      rawMemory);

    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr(UniquePtr&& other);

    UniquePtr& operator=(const UniquePtr& other) = delete;
    UniquePtr& operator                          =(UniquePtr&& other);

    virtual ~UniquePtr();

    const rainbow::Type*        type() const;
    rainbow::memory::Allocator& allocator() const;
    const rainbow::raw::Object& get() const;
    const rainbow::raw::Object& operator*() const;
    const rainbow::raw::Object* operator->() const;
    rainbow::raw::Object        release();
    bool                        isNull() const;

    bool operator==(const UniquePtr& other) const;
    bool operator!=(const UniquePtr& other) const;
    bool operator==(void* other) const;
    bool operator!=(void* other) const;

    explicit operator bool() const;

private:
    rainbow::raw::Object        _object    = nullptr;
    rainbow::memory::Allocator* _allocator = nullptr;
};

} // namespace raw

template<typename T>
class UniquePtr : public rainbow::raw::UniquePtr
{
public:
    UniquePtr(rainbow::memory::Allocator& allocator, rainbow::Object<T> object)
        : rainbow::raw::UniquePtr{allocator, std::move(object)}
    {
    }

    UniquePtr() = default;
    UniquePtr(std::nullptr_t) : UniquePtr{} {}
    UniquePtr(UniquePtr&& other) : rainbow::raw::UniquePtr{std::move(other)} {}

    template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
    UniquePtr(UniquePtr<U>&& pointer)
        : UniquePtr{pointer.allocator(), pointer.release().template cast<T>()}
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
    UniquePtr& operator=(UniquePtr<U>&& other)
    {
        rainbow::raw::UniquePtr::operator=(UniquePtr{std::move(other)});
    }

    UniquePtr& operator=(UniquePtr&& other)
    {
        rainbow::raw::UniquePtr::operator=(std::move(other));
        return *this;
    }

    rainbow::Object<T> object() const
    {
        return rainbow::raw::UniquePtr::get();
    }

    rainbow::Object<T> release()
    {
        return rainbow::raw::UniquePtr::release();
    }

    T* get() const
    {
        return object().get();
    }

    T* operator->() const
    {
        return get();
    }
};

template<typename T>
using UniquePtrAllocation =
    rainbow::Result<rainbow::UniquePtr<T>, rainbow::memory::AllocationResult>;

template<typename T, typename... Args>
UniquePtrAllocation<T>
    makeUnique(rainbow::memory::Allocator& allocator, Args&&... args)
{
    RAINBOW_RESULT_TRY(
        allocation,
        rainbow::object::new_<T>(allocator, std::forward<Args>(args)...));

    return UniquePtr<T>{allocator, std::move(allocation)};
}

} // namespace rainbow

#endif // RAINBOW_UNIQUEPTR_HPP_INCLUDED
