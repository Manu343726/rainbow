#include <array>
#include <csignal>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <rainbow/containers/vector.hpp>
#include <rainbow/memory/allocators/committed_virtual_memory.hpp>
#include <rainbow/memory/allocators/fixed_size.hpp>
#include <rainbow/memory/allocators/hierarchical.hpp>
#include <rainbow/memory/allocators/malloc.hpp>
#include <rainbow/memory/allocators/owning.hpp>
#include <rainbow/memory/allocators/pool.hpp>
#include <rainbow/memory/allocators/virtual_memory.hpp>
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
    const auto object = rainbow::object::new_<Class>(allocator);
    object->world();
    rainbow::object::delete_(allocator, object);
}

int main()
{
    std::signal(SIGINT, std::exit);

    char                                   storage[256];
    rainbow::memory::allocators::FixedSize fixedSize{
        rainbow::memory::Block{storage}};
    rainbow::memory::allocators::CommittedVirtualMemory<
        rainbow::memory::allocators::FixedSize>
        lazyFixedSize{rainbow::memory::reserveVirtualMemory(
            rainbow::memory::pageSize() * 100000000)};

    const auto poolFactory = [](rainbow::memory::Allocator& uniquePtrAllocator,
                                rainbow::memory::Allocator& parentAllocator,
                                rainbow::memory::Allocator& managerAllocator)
        -> rainbow::UniquePtrAllocation<rainbow::memory::Allocator> {
        rainbow::memory::allocators::Pool::Parameters params;
        params.count        = 1024;
        params.maxAlignment = 1;
        params.maxSize      = 1024;

        return rainbow::memory::allocators::makeOwningThroughManager<
            rainbow::memory::allocators::Pool>(
            uniquePtrAllocator,
            parentAllocator,
            managerAllocator,
            rainbow::memory::allocators::Pool::adaptParametersToRequirements(
                params, &parentAllocator, &managerAllocator));
    };

    rainbow::memory::allocators::Hierarchical hierarchical{
        rainbow::memory::allocators::virtualMemory(), poolFactory};

    for(int i = 0; i < 1000; ++i)
    {
        using Type  = std::array<char, 1024>;
        auto result = rainbow::makeUnique<Type>(hierarchical);

        if(not result)
        {
            std::cout << rainbow::memory::toString(result.result()) << "\n";
            std::exit(EXIT_FAILURE);
        }

        result.release();

        struct alignas(4096) Page
        {
            char data[4096 + 128];
        };

        static_assert(alignof(Page) == 4096);
        static_assert(sizeof(Page) == 4096 * 2);

        std::cout << rainbow::memory::toString(
                         rainbow::makeUnique<Page>(lazyFixedSize).result())
                  << "\n";
    }
}
