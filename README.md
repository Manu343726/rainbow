## Rainbow: Explicit memory allocation library

This is a simple toy project. My idea was to experience myself the
development of a containers library designed around the following ideas:

 - Reduce template code to the minimum, type-erase anything related to
     memory management.
 - Design all library features around a polymorphic allocator API. 
 - Memory allocation must be as explicit as possible. There are no
     implicit memory allocators (except a default malloc() allocator
     for convenience), and all container and smart pointer types
     require an allocator parameter.
 - stdlib-like types such as unique pointer, vector, list, hash map, etc.
