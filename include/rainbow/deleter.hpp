#ifndef RAINBOW_DELETER_HPP_INCLUDED
#define RAINBOW_DELETER_HPP_INCLUDED

#include <rainbow/memory/allocator.hpp>
#include <rainbow/object.hpp>

namespace rainbow
{

template<typename T>
class Deleter
{
public:
    Deleter(rainbow::memory::Allocator* allocator) : _allocator{allocator} {}

    void operator()(const Object<T>& object)
    {
        rainbow::delete_(*_allocator, object);
    }

private:
    rainbow::memory::Allocator* _allocator;
};

} // namespace rainbow

#endif // RAINBOW_DELETER_HPP_INCLUDED
