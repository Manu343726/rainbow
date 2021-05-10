#ifndef RAINBOW_CONTAINERS_VECTOR_HPP
#define RAINBOW_CONTAINERS_VECTOR_HPP

#include <rainbow/memory/allocator.hpp>
#include <rainbow/object.hpp>
#include <rainbow/type.hpp>
#include <rainbow/views/vector.hpp>

namespace rainbow::containers
{

namespace raw
{

class Vector
{
public:
    Vector(const rainbow::Type* type, rainbow::memory::Allocator& allocator);
    ~Vector();

    rainbow::memory::Allocator& allocator() const;

    std::size_t size() const;
    bool        empty() const;
    std::size_t capacity() const;
    bool        reserve(const std::size_t size);
    bool        resize(const std::size_t size);

    rainbow::memory::Block      push_back();
    bool                        pop_back();
    rainbow::views::raw::Vector view() const;

    operator rainbow::views::raw::Vector() const;

protected:
    rainbow::memory::Block storageInUse() const;

private:
    std::size_t                 _size;
    rainbow::memory::Block      _storage;
    const rainbow::Type*        _type;
    rainbow::memory::Allocator* _allocator;

    rainbow::memory::Block alignedStorage() const;
};

} // namespace raw

template<typename T>
class Vector : public raw::Vector
{
public:
    Vector(rainbow::memory::Allocator& allocator)
        : raw::Vector{rainbow::type_of<T>(), allocator}
    {
    }

    template<typename... Args>
    T* push_back(Args&&... args)
    {
        if(const auto block = raw::Vector::push_back())
        {
            return rainbow::construct<T>(block, std::forward<Args>(args)...)
                .get();
        }
        else
        {
            return nullptr;
        }
    }

    rainbow::views::Vector<T> view() const
    {
        return {storageInUse()};
    }
};

} // namespace rainbow::containers

#endif // RAINBOW_CONTAINERS_VECTOR_HPP
