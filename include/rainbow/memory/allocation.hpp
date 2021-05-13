#ifndef RAINBOW_MEMORY_ALLOCATION_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATION_HPP_INCLUDED

#include <rainbow/memory/block.hpp>
#include <rainbow/result.hpp>
#include <string_view>

namespace rainbow::memory
{

enum class AllocationResult
{
    Ok,
    CannotAlign,
    CannotCommit,
    NoSpaceLeft,
    NoSpaceInNode,
    NotSupported,
    CannotFreeOld,
    RequirementsFailed_Alignment,
    RequirementsFailed_MinSize
};

using Allocation =
    rainbow::Result<rainbow::memory::Block, rainbow::memory::AllocationResult>;

std::string_view toString(const rainbow::memory::AllocationResult result);

} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_ALLOCATION_HPP_INCLUDED
