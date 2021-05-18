add_rainbow_library(rainbow)

target_sources(rainbow PRIVATE
    memory/allocator.cpp
    memory/allocation.cpp
    memory/allocation_requirements.cpp
    memory/aligned.cpp
    memory/block.cpp
    memory/virtual.cpp
    memory/allocators/fixed_size.cpp
    memory/allocators/stack.cpp
    memory/allocators/malloc.cpp
    memory/allocators/pool.cpp
    memory/allocators/hierarchical.cpp
    memory/allocators/virtual_memory.cpp
    memory/allocators/reserved_virtual_memory.cpp
    views/vector.cpp
    containers/vector.cpp
    type.cpp
    object.cpp
    unique_ptr.cpp)

find_package(Threads REQUIRED)

target_link_libraries(rainbow PRIVATE Threads::Threads)
