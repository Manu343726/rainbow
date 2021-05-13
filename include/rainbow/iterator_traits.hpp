#ifndef RAINBOW_ITERATORTRAITS_HPP_INCLUDED
#define RAINBOW_ITERATORTRAITS_HPP_INCLUDED

#include <cstddef>
#include <iterator>
#include <rainbow/memory/block.hpp>
#include <rainbow/object/iterator.hpp>

namespace std
{

template<>
struct iterator_traits<rainbow::memory::Block::Iterator>
{
    using value_type        = rainbow::memory::Block;
    using reference         = rainbow::memory::Block;
    using pointer           = rainbow::memory::Block*;
    using diffrence_type    = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
};

template<typename T>
struct iterator_traits<rainbow::object::Iterator<T>>
{
    using value_type        = T;
    using reference         = T&;
    using pointer           = T*;
    using diffrence_type    = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
};

} // namespace std

#endif // RAINBOW_ITERATORTRAITS_HPP_INCLUDED
