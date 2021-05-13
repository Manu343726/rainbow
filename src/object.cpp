#include <rainbow/object.hpp>

using namespace rainbow::raw;
using namespace rainbow::memory;


Object::Object() : Object{nullptr} {}
Object::Object(std::nullptr_t) : Object{nullptr, rainbow::trivialType()} {}
Object::Object(const Block& block, const rainbow::Type* type)
    : Block{block}, _type{type}
{
}

const rainbow::Type* Object::type() const
{
    return _type;
}

Block Object::get() const
{
    return aligned(_type->alignment()).left(_type->size());
}

namespace rainbow
{

void destroy(const rainbow::raw::Object& object)
{
    object.type()->destroy(object.get());
}

} // namespace rainbow
