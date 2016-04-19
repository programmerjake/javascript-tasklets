/*
 * Copyright (C) 2012-2016 Jacob R. Lifshay
 * This file is part of Voxels.
 *
 * Voxels is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Voxels is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Voxels; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#ifndef JAVASCRIPT_TASKLETS_VM_JSMATH_H_
#define JAVASCRIPT_TASKLETS_VM_JSMATH_H_

#include <cmath>
#include <limits>
#include <cstdint>

namespace javascript_tasklets
{
namespace vm
{
namespace math
{
constexpr double NaN() noexcept
{
    return std::numeric_limits<double>::quiet_NaN();
}
constexpr double Infinity() noexcept
{
    return std::numeric_limits<double>::infinity();
}
constexpr double Pi() noexcept
{
    return 3.141592653589793238462643383279502884L;
}
constexpr double E() noexcept
{
    return 2.718281828459045235360287471352662498L;
}
constexpr double Ln10() noexcept // natural log of 10
{
    return 2.302585092994045684017991454684364208L;
}
constexpr double Ln2() noexcept // natural log of 2
{
    return 0.693147180559945309417232121458176568L;
}
constexpr double Log10E() noexcept // base 10 log of e
{
    return 0.434294481903251827651128918916605082L;
}
constexpr double Log2E() noexcept // base 2 log of e
{
    return 1.442695040888963407359924681001892137L;
}
constexpr double Sqrt1_2() noexcept // square root of 1/2
{
    return 0.707106781186547524400844362104849039L;
}
constexpr double Sqrt2() noexcept // square root of 2
{
    return 1.414213562373095048801688724209698079L;
}
inline double mod(double a, double b) noexcept
{
    return std::fmod(a, b);
}
inline std::int32_t mod(std::int32_t a, std::int32_t b) noexcept
{
    std::uint32_t ua = a, ub = b;
    if(b < 0)
        ub = -b;
    else if(b == 0)
        return 0;
    if(a < 0)
    {
        ua = -a;
        return -(ua % ub);
    }
    return ua % ub;
}
inline std::uint32_t mod(std::uint32_t a, std::uint32_t b) noexcept
{
    if(b == 0)
        return 0;
    return a % b;
}
inline std::int32_t div(std::int32_t a, std::int32_t b) noexcept
{
    if(b == 0)
        return 0;
    if(b == -1 && a == static_cast<std::int32_t>(0x80000000UL))
        return static_cast<std::int32_t>(0x80000000UL);
    return a / b;
}
inline std::uint32_t div(std::uint32_t a, std::uint32_t b) noexcept
{
    if(b == 0)
        return 0;
    return a / b;
}
inline double atan2(double y, double x) noexcept
{
    return std::atan2(y, x);
}
inline double abs(double v) noexcept
{
    return std::fabs(v);
}
inline std::int32_t abs(std::int32_t v) noexcept
{
    if(v < 0)
        return -static_cast<std::uint32_t>(v);
    return v;
}
inline double acos(double v) noexcept
{
    return std::acos(v);
}
inline double acosh(double v) noexcept
{
    return std::acosh(v);
}
}
}
}

#endif /* JAVASCRIPT_TASKLETS_VM_JSMATH_H_ */
