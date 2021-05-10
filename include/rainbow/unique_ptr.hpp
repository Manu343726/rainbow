#ifndef RAINBOW_UNIQUEPTR_HPP_INCLUDED
#define RAINBOW_UNIQUEPTR_HPP_INCLUDED

#include <memory>
#include <rainbow/memory/allocator.hpp>
#include <rainbow/object.hpp>
#include <type_traits>

namespace rainbow
{

template<typename T>
class UniquePtr
{
public:
    UniquePtr(rainbow::memory::Allocator& allocator, rainbow::Object<T> object)
        : _object{std::move(object)}, _allocator{&allocator}
    {
    }

    UniquePtr() = default;
    UniquePtr(std::nullptr_t) : UniquePtr{} {}

    template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
    UniquePtr(UniquePtr<U>&& pointer)
        : _object{rainbow::Object<T>{pointer.object()}},
          _allocator{&pointer.allocator()}
    {
        pointer.release();
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
    UniquePtr& operator=(UniquePtr<U>&& other)
    {
        release();

        _allocator = &other.allocator();
        _object    = rainbow::Object<T>(other.release());

        return *this;
    }

    UniquePtr& operator=(UniquePtr&& other)
    {
        release();

        _object    = std::exchange(other._object, nullptr);
        _allocator = std::exchange(other._allocator, nullptr);

        return *this;
    }

    ~UniquePtr()
    {
        release();
    }

    rainbow::Object<T> object() const
    {
        return _object;
    }

    rainbow::memory::Allocator& allocator() const
    {
        return *_allocator;
    }

    T* get() const
    {
        return _object.get();
    }

    T* operator->() const
    {
        return get();
    }

    operator bool() const
    {
        return not _object.isNull();
    }

    friend bool operator==(const UniquePtr<T>& lhs, const UniquePtr<T>& rhs)
    {
        return lhs._object == rhs._object;
    }

    friend bool operator!=(const UniquePtr<T>& lhs, const UniquePtr<T>& rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator==(const UniquePtr<T>& lhs, const Object<T>& rhs)
    {
        return lhs._object == rhs;
    }

    friend bool operator!=(const UniquePtr<T>& lhs, const Object<T>& rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator==(const UniquePtr<T>& lhs, T* rhs)
    {
        return lhs._object.aligned(alignof(T)).begin() == rhs;
    }

    friend bool operator!=(const UniquePtr<T>& lhs, T* rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator==(const UniquePtr<T>& lhs, std::nullptr_t)
    {
        return lhs._object == nullptr;
    }

    friend bool operator!=(const UniquePtr<T>& lhs, std::nullptr_t rhs)
    {
        return !(lhs == rhs);
    }

    Object<T> release()
    {
        _allocator = nullptr;

        return std::exchange(_object, nullptr);
    }

    void reset()
    {
        if(_object != nullptr and _allocator != nullptr)
        {
            rainbow::delete_(*_allocator, _object);
            release();
        }
    }

private:
    rainbow::Object<T>          _object    = nullptr;
    rainbow::memory::Allocator* _allocator = nullptr;
};

template<typename T, typename... Args>
UniquePtr<T> makeUnique(rainbow::memory::Allocator& allocator, Args&&... args)
{
    return {
        allocator, rainbow::new_<T>(allocator, std::forward<Args>(args)...)};
}

} // namespace rainbow

#endif // RAINBOW_UNIQUEPTR_HPP_INCLUDED
