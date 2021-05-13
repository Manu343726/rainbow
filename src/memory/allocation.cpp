#include <cassert>
#include <rainbow/memory/allocation.hpp>
#include <utility>

namespace rainbow::memory
{

std::string_view toString(const AllocationResult result)
{
    switch(result)
    {
    case AllocationResult::Ok:
        return "Ok";
    case AllocationResult::NoSpaceLeft:
        return "No space left on storage";
    case AllocationResult::CannotAlign:
        return "Cannot meet the requested alignment";
    case AllocationResult::CannotCommit:
        return "Cannot commit the given uncommited block";
    case AllocationResult::NoSpaceInNode:
        return "No enough space in fixed storage node";
    case AllocationResult::NotSupported:
        return "Operation not supported";
    case AllocationResult::CannotFreeOld:
        return "Cannot free old block";
    case AllocationResult::RequirementsFailed_Alignment:
        return "Less than minimal alignment requested";
    case AllocationResult::RequirementsFailed_MinSize:
        return "Less than minimal size requested";
    }

    // unreachable
    return "";
}

} // namespace rainbow::memory
