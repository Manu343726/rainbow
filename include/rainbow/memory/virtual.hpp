#ifndef RAINBOW_MEMORY_VIRTUAL_HPP_INCLUDED
#define RAINBOW_MEMORY_VIRTUAL_HPP_INCLUDED

#include <rainbow/memory/allocation.hpp>
#include <rainbow/memory/allocation_requirements.hpp>
#include <rainbow/memory/block.hpp>
#include <rainbow/memory/deallocation.hpp>

namespace rainbow::memory
{

rainbow::memory::Allocation reserveVirtualMemory(const std::size_t size);
rainbow::memory::Deallocation
    releaseVirtualMemory(const rainbow::memory::Block& block);
rainbow::memory::Allocation
    commitVirtualMemory(const rainbow::memory::Block& block);
rainbow::memory::Deallocation
    unCommitVirtualMemory(const rainbow::memory::Block& block);
rainbow::memory::Allocation allocateVirtualMemory(const std::size_t size);
rainbow::memory::Deallocation
            freeVirtualMemory(const rainbow::memory::Block& block);
std::size_t pageSize();
std::size_t pageAlignment();
rainbow::memory::AllocationRequirements pageAlignedRequirements();

} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_VIRTUAL_HPP_INCLUDED
