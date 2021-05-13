#ifndef RAINBOW_OBJECTS_ALLOCATION_HPP
#define RAINBOW_OBJECTS_ALLOCATION_HPP

#include <rainbow/memory/allocation.hpp>
#include <rainbow/object.hpp>
#include <rainbow/result.hpp>

namespace rainbow::object
{

template<typename T>
using Allocation =
    rainbow::Result<rainbow::Object<T>, rainbow::memory::AllocationResult>;

}
#endif // RAINBOW_OBJECTS_ALLOCATION_HPP
