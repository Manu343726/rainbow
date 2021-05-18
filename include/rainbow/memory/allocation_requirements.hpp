#ifndef RAINBOW_MEMORY_ALLOCATIONREQUIREMENTS_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATIONREQUIREMENTS_HPP_INCLUDED

#include <cstdint>

namespace rainbow::memory
{

struct AllocationRequirements
{
    std::size_t size           = 0;
    std::size_t extraSize      = 0;
    std::size_t alignment      = 1;
    std::size_t sizeMultipleOf = 1;

    std::size_t totalSize() const;

    AllocationRequirements& operator+=(const AllocationRequirements& other);
    bool operator<(const AllocationRequirements& other) const;
    bool operator>(const AllocationRequirements& other) const;
    bool operator<=(const AllocationRequirements& other) const;
    bool operator>=(const AllocationRequirements& other) const;
    bool operator==(const AllocationRequirements& other) const;
    bool operator!=(const AllocationRequirements& other) const;

    static AllocationRequirements simple(const std::size_t size);
};

} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_ALLOCATIONREQUIREMENTS_HPP_INCLUDED
