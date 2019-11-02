#pragma once

#include <limits>

namespace mygal
{

template<typename T>
constexpr T EPSILON = std::numeric_limits<T>::epsilon();

// Almost predicates are easier to satisfy than the normal ones

template<typename T>
constexpr bool almostLower(T lhs, T rhs) noexcept
{
    return lhs <= rhs + EPSILON<T>;
}

template<typename T>
constexpr bool almostGreater(T lhs, T rhs) noexcept
{
    return lhs >= rhs - EPSILON<T>;
}

template<typename T>
constexpr bool almostEqual(T lhs, T rhs) noexcept
{
    return almostLower(lhs, rhs) && almostGreater(lhs, rhs);
}

template<typename T>
constexpr bool almostZero(T x) noexcept
{
    return almostEqual(x, static_cast<T>(0));
}

template<typename T>
constexpr bool almostBetween(T x, T a, T b) noexcept
{
    return almostGreater(x, a) && almostLower(x, b);
}

// Strictly predicates are harder to satisfy than the normal ones

template<typename T>
constexpr bool strictlyLower(T lhs, T rhs) noexcept
{
    return lhs < rhs - EPSILON<T>;
}

template<typename T>
constexpr bool strictlyGreater(T lhs, T rhs) noexcept
{
    return lhs > rhs + EPSILON<T>;
}

template<typename T>
constexpr bool strictlyBetween(T x, T a, T b) noexcept
{
    return strictlyGreater(x, a) && strictlyLower(x, b);
}

}