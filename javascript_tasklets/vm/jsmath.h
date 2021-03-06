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
inline double cbrtHelper(double v) noexcept
{
    using namespace std;
    return cbrt(v);
}
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
inline double asin(double v) noexcept
{
    return std::asin(v);
}
inline double asinh(double v) noexcept
{
    return std::asinh(v);
}
inline double atan(double v) noexcept
{
    return std::atan(v);
}
inline double atanh(double v) noexcept
{
    return std::atanh(v);
}
inline double cbrt(double v) noexcept
{
    return cbrtHelper(v);
}
inline double ceil(double v) noexcept
{
    if(v < 0 && v > -1)
        return -0.0;
    return std::ceil(v);
}
inline double floor(double v) noexcept
{
    if(v > 0 && v < 1)
        return 0.0;
    return std::floor(v);
}

constexpr std::uint32_t clz4(std::uint8_t v) noexcept
{
#ifdef __GNUC__
    return __builtin_clz(v) - __builtin_clz(0x8U);
#else
    typedef const std::uint_fast8_t LookupTableType[0x10];
    return LookupTableType{4, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}[v];
#endif
}

constexpr std::uint32_t clz8(std::uint8_t v) noexcept
{
#ifdef __GNUC__
    return __builtin_clz(v) - __builtin_clz(0x80U);
#else
    return ((v & 0xF0) == 0) ? 4 + clz4(v) : clz4(v >> 4);
#endif
}

constexpr std::uint32_t clz16(std::uint16_t v) noexcept
{
#ifdef __GNUC__
    return __builtin_clz(v) - (__builtin_clz(0) - 16);
#else
    return ((v & 0xFF00U) == 0) ? 8 + clz8(v) : clz8(v >> 8);
#endif
}

constexpr std::uint32_t clz32(std::uint32_t v) noexcept
{
#ifdef __GNUC__
    return __builtin_clzl(v) - (__builtin_clzl(0) - 32);
#else
    return ((v & 0xFFFF0000UL) == 0) ? 16 + clz16(v) : clz16(v >> 16);
#endif
}

constexpr std::uint32_t clz64(std::uint64_t v) noexcept
{
#ifdef __GNUC__
    return __builtin_clzll(v) - (__builtin_clzll(0) - 64);
#else
    return ((v & 0xFFFFFFFF00000000ULL) == 0) ? 32 + clz32(v) : clz32(v >> 32);
#endif
}
constexpr std::uint32_t ctz4(std::uint8_t v) noexcept
{
#ifdef __GNUC__
    return v == 0 ? 4 : __builtin_ctz(v);
#else
    typedef const std::uint_fast8_t LookupTableType[0x10];
    return LookupTableType{4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0}[v];
#endif
}

constexpr std::uint32_t ctz8(std::uint8_t v) noexcept
{
#ifdef __GNUC__
    return v == 0 ? 8 : __builtin_ctz(v);
#else
    return ((v & 0xF0) == 0) ? ctz4(v) : 4 + ctz4(v >> 4);
#endif
}

constexpr std::uint32_t ctz16(std::uint16_t v) noexcept
{
#ifdef __GNUC__
    return v == 0 ? 16 : __builtin_ctz(v);
#else
    return ((v & 0xFF00U) == 0) ? ctz8(v) : 8 + ctz8(v >> 8);
#endif
}

constexpr std::uint32_t ctz32(std::uint32_t v) noexcept
{
#ifdef __GNUC__
    return v == 0 ? 32 : __builtin_ctzl(v);
#else
    return ((v & 0xFFFF0000UL) == 0) ? ctz16(v) : 16 + ctz16(v >> 16);
#endif
}

constexpr std::uint32_t ctz64(std::uint64_t v) noexcept
{
#ifdef __GNUC__
    return v == 0 ? 64 : __builtin_ctzll(v);
#else
    return ((v & 0xFFFFFFFF00000000ULL) == 0) ? ctz32(v) : 32 + ctz32(v >> 32);
#endif
}
inline double cos(double v) noexcept
{
    return std::cos(v);
}
inline double cosh(double v) noexcept
{
    return std::cosh(v);
}
inline double exp(double v) noexcept
{
    return std::exp(v);
}
inline double expm1(double v) noexcept
{
    return std::expm1(v);
}
inline double log(double v) noexcept
{
    return std::log(v);
}
inline double log1p(double v) noexcept
{
    return std::log1p(v);
}
inline double log10(double v) noexcept
{
    return std::log10(v);
}
inline double log2(double v) noexcept
{
    return std::log2(v);
}
inline double max(double a, double b) noexcept
{
    if(std::isnan(a))
        return a;
    if(a > b) // false if b is NaN
        return a;
    return b;
}
inline double min(double a, double b) noexcept
{
    if(std::isnan(a))
        return a;
    if(a < b) // false if b is NaN
        return a;
    return b;
}
inline double pow(double a, double b) noexcept
{
    if(std::isnan(b))
        return b;
    if((a == 1 || a == -1) && std::isinf(b))
        return NaN();
    if(b == 0)
        return 1;
    if(a == 0)
    {
        if(std::signbit(a))
        {
            if(b > 0)
            {
                b *= 0.5;
                if(b - std::floor(b) == 0.5)
                {
                    return -0.0;
                }
                return 0;
            }
            b *= 0.5;
            if(b - std::floor(b) == 0.5)
            {
                return -Infinity();
            }
            return Infinity();
        }
        if(b > 0)
            return 0;
        return Infinity();
    }
    return std::pow(a, b);
}
inline double round(double v) noexcept
{
    double retval = std::round(v);
    if(v < 0 && std::fabs(v - retval) == 0.5)
        return std::ceil(v);
    return retval;
}
inline double sign(double v) noexcept
{
    if(v < 0) // fall through for NaN and zero
        return -1;
    if(v > 0) // fall through for NaN and zero
        return 1;
    return v;
}
inline std::int32_t sign(std::int32_t v) noexcept
{
    if(v < 0)
        return -1;
    if(v > 0)
        return 1;
    return 0;
}
inline double sin(double v) noexcept
{
    return std::sin(v);
}
inline double sinh(double v) noexcept
{
    return std::sinh(v);
}
inline double sqrt(double v) noexcept
{
    return std::sqrt(v);
}
inline double tan(double v) noexcept
{
    return std::tan(v);
}
inline double tanh(double v) noexcept
{
    return std::tanh(v);
}
inline double trunc(double v) noexcept
{
    if(v > -1 && v < 1)
    {
        if(v > 0)
            return 0;
        return -0.0;
    }
    return std::trunc(v);
}
}
}
}

#endif /* JAVASCRIPT_TASKLETS_VM_JSMATH_H_ */
