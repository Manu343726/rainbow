#ifndef RAINBOW_MEMORY_BLOCK_HPP_INCLUDED
#define RAINBOW_MEMORY_BLOCK_HPP_INCLUDED

#include <cstddef>
#include <cstdint>
#include <rainbow/memory/aligned.hpp>

namespace rainbow::memory
{

class Block
{
public:
    virtual ~Block() = default;

    Block();
    Block(std::nullptr_t);
    Block(void* begin, const std::size_t size);
    Block(Block&& other);
    Block(const Block& other) = default;
    Block& operator           =(Block&& other);
    Block& operator=(const Block& other) = default;

    template<std::size_t Size>
    Block(char (&buffer)[Size]) : Block{&buffer[0], Size}
    {
    }

    bool        isNull() const;
    std::size_t size() const;
    bool        empty() const;
    void*       begin() const;
    void*       end() const;

    bool contains(void* address) const;
    bool contains(const Block& other) const;
    bool isAligned(const std::size_t boundary) const;

    explicit operator bool() const;
    bool     operator==(const std::nullptr_t) const;
    bool     operator!=(const std::nullptr_t) const;
    bool     operator==(const Block& other) const;
    bool     operator!=(const Block& other) const;
    Block    operator+(const std::ptrdiff_t offset) const;
    Block    operator-(const std::ptrdiff_t offset) const;

    Block shrinkFromBegin(const std::size_t bytesToRemove) const;
    Block shrinkFromEnd(const std::size_t bytesToRemove) const;
    Block left(const std::size_t bytes) const;
    Block right(const std::size_t bytes) const;
    Block aligned(const std::size_t boundary) const;
    Block aligned(const std::size_t boundary, const std::size_t size) const;

    class Iterator
    {
    public:
        Iterator(const Block& subblock);

        Iterator& operator++();
        Iterator  operator++(int);
        Iterator& operator--();
        Iterator  operator--(int);
        Iterator  operator+(const std::int64_t n) const;
        Iterator  operator-(const std::int64_t n) const;

        Block operator*() const;

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        char*       _begin;
        std::size_t _size;
    };

    class Range
    {
    public:
        Range(const Block& fullBlock, const std::size_t subBlockSize);

        Iterator begin() const;
        Iterator end() const;

    private:
        Iterator _begin;
        Iterator _end;

        Range(const Block& begin, const Block& end);
    };

    Range iterate(const std::size_t subBlockSize) const;
    Block subBlock(const std::size_t begin, const std::size_t end) const;
    Block operator()(const std::size_t begin, const std::size_t end) const;
    Block operator()(const std::size_t begin) const;

private:
    char*       _begin;
    std::size_t _size;
};

} // namespace rainbow::memory

#endif // RAINBOW_MEMORY_BLOCK_HPP_INCLUDED
