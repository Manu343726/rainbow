#include <cstddef>
#include <cstring>
#include <iostream>
#include <rainbow/containers/vector.hpp>
#include <rainbow/memory/allocators/fixed_size.hpp>
#include <rainbow/memory/allocators/hierarchical.hpp>
#include <rainbow/memory/allocators/malloc.hpp>
#include <rainbow/memory/allocators/owning.hpp>
#include <rainbow/memory/allocators/pool.hpp>
#include <rainbow/object.hpp>
#include <rainbow/shared_ptr.hpp>
#include <rainbow/unique_ptr.hpp>

class Class
{
public:
    Class()
    {
        std::strcpy(&_hello[0], "hello, ");

        std::cout << _hello;
    }

    void world()
    {
        std::cout << _world;
    }

    ~Class()
    {
        std::cout << "!" << std::endl;
    }

    char        _hello[8];
    std::string _world = "world";
};

void play(rainbow::memory::Allocator& allocator)
{
    const auto object = rainbow::new_<Class>(allocator);
    object->world();
    rainbow::delete_(allocator, object);
}

int main()
{
    char                                   storage[256];
    rainbow::memory::allocators::FixedSize fixedSize{
        rainbow::memory::Block{storage}};

    rainbow::memory::allocators::Hierarchical hierarchical{
        rainbow::memory::allocators::malloc(),
        [](rainbow::memory::Allocator& uniquePtrAllocator,
           rainbow::memory::Allocator& parentAllocator)
            -> rainbow::UniquePtr<rainbow::memory::Allocator> {
            rainbow::memory::allocators::Pool::Parameters params;
            params.count        = 1024;
            params.maxAlignment = alignof(std::max_align_t);
            params.maxSize      = 128;

            return rainbow::memory::allocators::makeOwning<
                rainbow::memory::allocators::Pool>(
                uniquePtrAllocator, parentAllocator, params);
        }};

    rainbow::containers::Vector<Class> vector{fixedSize};

    vector.resize(2);

    for(const auto& object : vector.view())
    {
        std::cout << "[" << object._hello << object._world << "]";
    }

    // play(malloc);

    for(int i = 0; i < 10000; ++i)
    {
        rainbow::makeUnique<int>(hierarchical, 42);

        if(not vector.push_back())
        {
            /*
            std::cout << "push_back() error (allocator free space: "
                      << *vector.allocator().info().free
                      << " bytes, required: " << sizeof(Class)
                      << " bytes, alignment required: " << alignof(Class)
                      << ")\n";

            std::exit(EXIT_FAILURE);
            */
        }
    }
}
