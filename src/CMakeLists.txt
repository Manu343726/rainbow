add_rainbow_library(rainbow)

target_sources(rainbow PRIVATE
    memory/allocator.cpp
    memory/aligned.cpp
    memory/block.cpp
    memory/allocators/fixed_size.cpp
    memory/allocators/malloc.cpp
    memory/allocators/pool.cpp
    memory/allocators/hierarchical.cpp
    views/vector.cpp
    containers/vector.cpp
    type.cpp
    object.cpp
    unique_ptr.cpp)

find_package(Threads REQUIRED)

target_link_libraries(rainbow PRIVATE Threads::Threads)
