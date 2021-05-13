#ifndef RAINBOW_CONTAINERS_VECTOR_HPP
#define RAINBOW_CONTAINERS_VECTOR_HPP

#include <rainbow/memory/allocator.hpp>
#include <rainbow/object.hpp>
#include <rainbow/object/allocation.hpp>
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
    Vector(const Vector&) = delete;
    Vector(Vector&&)      = delete;
    Vector& operator=(const Vector&) = delete;
    Vector& operator=(Vector&&) = delete;

    virtual ~Vector();

    rainbow::memory::Allocator& allocator() const;

    std::size_t                       size() const;
    bool                              empty() const;
    std::size_t                       capacity() const;
    rainbow::memory::AllocationResult reserve(const std::size_t size);
    rainbow::memory::AllocationResult resize(const std::size_t size);

    rainbow::memory::Allocation push_back();
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
    rainbow::object::Allocation<T> push_back(Args&&... args)
    {
        RAINBOW_RESULT_TRY(pushBackResult, raw::Vector::push_back());

        return rainbow::construct<T>(
            pushBackResult, std::forward<Args>(args)...);
    }

    rainbow::views::Vector<T> view() const
    {
        return {storageInUse()};
    }
};

} // namespace rainbow::containers

#endif // RAINBOW_CONTAINERS_VECTOR_HPP
