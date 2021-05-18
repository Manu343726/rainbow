#include <numeric>
#include <rainbow/memory/aligned.hpp>
#include <rainbow/memory/allocation_requirements.hpp>
#include <tuple>

using namespace rainbow::memory;

std::size_t AllocationRequirements::totalSize() const
{
    return nextAlignedSize(size, alignment) + extraSize;
}

AllocationRequirements&
    AllocationRequirements::operator+=(const AllocationRequirements& other)
{
    size += other.size;
    extraSize += other.extraSize;
    alignment      = std::max(alignment, other.alignment);
    sizeMultipleOf = std::lcm(sizeMultipleOf, other.sizeMultipleOf);

    return *this;
}

bool AllocationRequirements::operator==(
    const AllocationRequirements& other) const
{
    return std::tie(size, extraSize, alignment, sizeMultipleOf) ==
           std::tie(
               other.size, other.extraSize, other.alignment, sizeMultipleOf);
}

bool AllocationRequirements::operator!=(
    const AllocationRequirements& other) const
{
    return !(*this == other);
}

bool AllocationRequirements::operator<(
    const AllocationRequirements& other) const
{
    return std::make_tuple(totalSize(), alignment, sizeMultipleOf) <
           std::make_tuple(totalSize(), other.alignment, other.sizeMultipleOf);
}

bool AllocationRequirements::operator>(
    const AllocationRequirements& other) const
{
    return std::make_tuple(totalSize(), alignment, sizeMultipleOf) >
           std::make_tuple(totalSize(), other.alignment, other.sizeMultipleOf);
}

bool AllocationRequirements::operator<=(
    const AllocationRequirements& other) const
{
    return *this == other or (*this < other);
}

bool AllocationRequirements::operator>=(
    const AllocationRequirements& other) const
{
    return *this == other or (*this > other);
}

AllocationRequirements AllocationRequirements::simple(const std::size_t size)
{
    AllocationRequirements result;
    result.size = size;

    return result;
}
