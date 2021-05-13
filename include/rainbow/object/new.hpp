#ifndef RAINBOW_OBJECT_NEW_HPP
#define RAINBOW_OBJECT_NEW_HPP

#include <rainbow/memory/allocator.hpp>
#include <rainbow/object.hpp>
#include <rainbow/object/allocation.hpp>

namespace rainbow::object
{

template<typename T, typename... Args>
rainbow::object::Allocation<T>
    new_(rainbow::memory::Allocator& allocator, Args&&... args)
{
    RAINBOW_RESULT_TRY(
        allocation, allocator.allocateAligned(sizeof(T), alignof(T)));

    return rainbow::construct<T>(
        allocation.aligned(alignof(T)), std::forward<Args>(args)...);
}

template<typename T>
rainbow::memory::Deallocation delete_(
    rainbow::memory::Allocator& allocator, const rainbow::Object<T>& object)
{
    rainbow::destroy(object);
    return allocator.free(object);
}

} // namespace rainbow::object

#endif // RAINBOW_OBJECT_NEW_HPP
