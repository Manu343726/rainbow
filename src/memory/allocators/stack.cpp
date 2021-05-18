#include <rainbow/memory/allocators/stack.hpp>
#include <utility>

using namespace rainbow::memory;
using namespace rainbow::memory::allocators;

Stack::Stack(Block storage) : _storage{storage}, _used{0} {}

Allocator::Features Stack::features() const
{
    return FEATURES;
}

Allocator::Info Stack::info() const
{
    Info result;

    result.storage = _storage;
    result.used    = _used;
    result.free    = freeSpace();

    return result;
}

Allocation Stack::allocate(const std::size_t bytes)
{
    if(bytes > freeSpace())
    {
        return AllocationResult::NoSpaceLeft;
    }

    _used += bytes;

    return _storage.right(bytes);
}
Allocation
    Stack::allocateAligned(const std::size_t bytes, const std::size_t boundary)
{
    if(freeStorage().isAligned(boundary))
    {
        return allocate(bytes);
    }

    return allocate(
        bytes + rainbow::memory::distanceToNextAligned(
                    freeStorage().begin(), boundary));
}

Block Stack::freeStorage() const
{
    return _storage.shrinkFromBegin(_used);
}

std::size_t Stack::freeSpace() const
{
    return _storage.size() - _used;
}

Block Stack::usedStorage() const
{
    return _storage.left(_used);
}

Deallocation Stack::free([[maybe_unused]] const Block& block)
{
    if(_used < block.size() or usedStorage().right(block.size()) != block)
    {
        return DeallocationResult::UnknownNode;
    }

    _used -= block.size();

    return DeallocationResult::Ok;
}
