#ifndef RAINBOW_RESULT_HPP_INCLUDED
#define RAINBOW_RESULT_HPP_INCLUDED

#include <type_traits>
#include <utility>

namespace rainbow
{

template<typename T, typename ResultCode>
struct Result : public T, public Result<void, ResultCode>
{
public:
    using Result<void, ResultCode>::Result;
    using Result<void, ResultCode>::operator bool;

    template<typename... Args>
    constexpr Result(Args&&... args)
        : T{std::forward<Args>(args)...},
          Result<void, ResultCode>{ResultCode::Ok}
    {
    }
};

template<typename ResultCode>
struct Result<void, ResultCode>
{
    constexpr Result(const ResultCode resultCode) : _result{resultCode} {}

    constexpr ResultCode result() const
    {
        return _result;
    }

    constexpr explicit operator bool() const
    {
        return _result == ResultCode::Ok;
    }

private:
    ResultCode _result;
};

namespace result
{

template<typename Result>
constexpr std::enable_if_t<std::is_enum<Result>{}, Result>
    code(const Result result)
{
    return result;
}

template<typename T, typename ResultCode>
constexpr ResultCode code(const rainbow::Result<T, ResultCode>& result)
{
    return result.result();
}

template<typename Result>
using Code = decltype(code(std::declval<Result>()));

template<typename Result>
constexpr Code<Result> Ok = Code<Result>::Ok;

template<typename Result>
constexpr bool isOk(const Result& result)
{
    return code(result) == Ok<Result>;
}

} // namespace result

#ifndef RAINBOW_PP_CAT
#define RAINBOW_PP_CAT_IMPL2(x, y) x##y
#define RAINBOW_PP_CAT_IMPL1(x, y) RAINBOW_PP_CAT_IMPL2(x, y)
#define RAINBOW_PP_CAT(x, y) RAINBOW_PP_CAT_IMPL1(x, y)
#endif // RAINBOW_PP_CAT


#define RAINBOW_RESULT_TRY_IMPL


#define CAT(A, B) A##B
#define SELECT(NAME, NUM) CAT(NAME##_, NUM)

#define GET_COUNT(_1, _2, _3, _4, _5, _6 /* ad nauseam */, COUNT, ...) COUNT
#define VA_SIZE(...) GET_COUNT(__VA_ARGS__, 6, 5, 4, 3, 2, 1)

#define VA_SELECT(NAME, ...) SELECT(NAME, VA_SIZE(__VA_ARGS__))(__VA_ARGS__)

#define RAINBOW_RESULT_TRY(...) VA_SELECT(RAINBOW_RESULT_TRY, __VA_ARGS__)

#define RAINBOW_RESULT_TRY_2(var, expression)                                  \
    auto var = expression;                                                     \
    if(not rainbow::result::isOk(var))                                         \
    {                                                                          \
        return rainbow::result::code(var);                                     \
    }

#define RAINBOW_RESULT_TRY_1(expression)                                       \
    RAINBOW_RESULT_TRY_2(                                                      \
        RAINBOW_PP_CAT(__rainbow_result_try__, __LINE__), expression)


} // namespace rainbow

#endif // RAINBOW_RESULT_HPP_INCLUDED
