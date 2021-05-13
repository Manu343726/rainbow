#ifndef RAINBOW_MEMORY_DEALLOCATION_HPP
#define RAINBOW_MEMORY_DEALLOCATION_HPP

#include <rainbow/result.hpp>
#include <string_view>

namespace rainbow::memory
{

enum class DeallocationResult
{
    Ok,
    NotCurrentNode,
    UnknownNode,
    CannotUncommit,
    NotSupported
};

using Deallocation = rainbow::Result<void, DeallocationResult>;

std::string_view toString(const DeallocationResult resultCode);

} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_DEALLOCATION_HPP
