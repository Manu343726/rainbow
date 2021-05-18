#ifndef RAINBOW_MEMORY_ALLOCATOR_FEATURES_HPP_INCLUDED
#define RAINBOW_MEMORY_ALLOCATOR_FEATURES_HPP_INCLUDED

#include <type_traits>

namespace rainbow::memory
{

enum class AllocatorFeatures
{
    None                  = 0b0,
    Allocate              = 0b1,
    AllocateAligned       = 0b10,
    Reallocate            = 0b100,
    ReallocateAligned     = 0b1000,
    Free                  = 0b10000,
    ConstrainedAllocation = 0b100000,
    GrowingAllocator      = 0b1000000,
    OwningAllocator       = 0b10000000,
    InPlaceBookKeeping    = 0b100000000
};


constexpr AllocatorFeatures
    operator|(const AllocatorFeatures lhs, const AllocatorFeatures rhs)
{
    return static_cast<AllocatorFeatures>(
        static_cast<std::underlying_type_t<AllocatorFeatures>>(lhs) |
        static_cast<std::underlying_type_t<AllocatorFeatures>>(rhs));
}

constexpr bool
    operator&(const AllocatorFeatures lhs, const AllocatorFeatures rhs)
{
    return static_cast<bool>(
        static_cast<std::underlying_type_t<AllocatorFeatures>>(lhs) &
        static_cast<std::underlying_type_t<AllocatorFeatures>>(rhs));
}

namespace detail
{

template<typename Allocator, typename = void>
struct IsStaticallyFeaturedAllocator : std::false_type
{
};

template<typename Allocator>
struct IsStaticallyFeaturedAllocator<
    Allocator,
    std::void_t<decltype(Allocator::FEATURES)>>
    : std::is_same<
          std::decay_t<decltype(Allocator::FEATURES)>,
          rainbow::memory::AllocatorFeatures>
{
};

} // namespace detail

namespace traits
{

template<typename Allocator>
constexpr bool IsStaticallyFeaturedAllocator =
    rainbow::memory::detail::IsStaticallyFeaturedAllocator<Allocator>::value;
}

template<typename Allocator>
constexpr AllocatorFeatures features()
{
    if constexpr(rainbow::memory::traits::IsStaticallyFeaturedAllocator<
                     Allocator>)
    {
        return Allocator::FEATURES;
    }
    else
    {
        return AllocatorFeatures::None;
    }
}

} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_ALLOCATOR_FEATURES_HPP_INCLUDED
