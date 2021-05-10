#include <rainbow/memory/allocators/fixed_size.hpp>
#include <utility>

using namespace rainbow::memory;
using namespace rainbow::memory::allocators;

FixedSize::FixedSize(Block storage) : _storage{storage}, _used{0} {}

Allocator::Features FixedSize::features() const
{
    return FEATURES;
}

Allocator::Info FixedSize::info() const
{
    Info result;

    result.storage = _storage;
    result.used    = _used;
    result.free    = freeSpace();
    result.parent  = _parent;

    return result;
}

Block FixedSize::allocate(const std::size_t bytes)
{
    if(bytes > freeSpace())
    {
        return nullptr;
    }

    _used += bytes;

    return _storage(_used - bytes, _used);
}

Block FixedSize::allocateAligned(
    const std::size_t bytes, const std::size_t boundary)
{
    if(freeStorage().isAligned(boundary))
    {
        return allocate(bytes);
    }

    return allocate(
        bytes + rainbow::memory::distanceToNextAligned(
                    freeStorage().begin(), boundary));
}

Block FixedSize::freeStorage() const
{
    return _storage(_used);
}

std::size_t FixedSize::freeSpace() const
{
    return _storage.size() - _used;
}

bool FixedSize::free([[maybe_unused]] const Block& block)
{
    return true;
}
