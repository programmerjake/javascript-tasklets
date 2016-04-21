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

#ifndef JAVASCRIPT_TASKLETS_SOFT_FLOAT_H_
#define JAVASCRIPT_TASKLETS_SOFT_FLOAT_H_

#include <cstdint>
#include <cmath>
#include "vm/jsmath.h"
#include "constexpr_assert.h"

namespace javascript_tasklets
{
namespace soft_float
{
struct UInt128 final
{
    std::uint64_t low;
    std::uint64_t high;
    constexpr UInt128(std::uint64_t high, std::uint64_t low) noexcept : low(low), high(high)
    {
    }
    constexpr explicit UInt128(std::uint64_t low = 0) noexcept : low(low), high(0)
    {
    }
    static constexpr bool addCarries(std::uint64_t a, std::uint64_t b) noexcept
    {
        return static_cast<std::uint64_t>(a + b) < a;
    }
    static constexpr bool subBorrows(std::uint64_t a, std::uint64_t b) noexcept
    {
        return static_cast<std::uint64_t>(a - b) > a;
    }
    friend constexpr UInt128 operator+(UInt128 a, UInt128 b) noexcept
    {
        return UInt128(a.high + b.high + addCarries(a.low, b.low), a.low + b.low);
    }
    UInt128 &operator+=(UInt128 v) noexcept
    {
        return *this = *this + v;
    }
    friend constexpr UInt128 operator-(UInt128 a, UInt128 b) noexcept
    {
        return UInt128(a.high - b.high - subBorrows(a.low, b.low), a.low - b.low);
    }
    UInt128 &operator-=(UInt128 v) noexcept
    {
        return *this = *this - v;
    }

private:
    static constexpr std::uint64_t multiplyHighHelper2(std::uint64_t h,
                                                       std::uint64_t m1,
                                                       std::uint64_t m2,
                                                       std::uint64_t l) noexcept
    {
        return (UInt128(h, l) + UInt128(m1 >> 32, m1 << 32) + UInt128(m2 >> 32, m2 << 32)).high;
    }
    static constexpr std::uint64_t multiplyHighHelper1(std::uint32_t ah,
                                                       std::uint32_t al,
                                                       std::uint32_t bh,
                                                       std::uint32_t bl) noexcept
    {
        return multiplyHighHelper2(static_cast<std::uint64_t>(ah) * bh,
                                   static_cast<std::uint64_t>(ah) * bl,
                                   static_cast<std::uint64_t>(al) * bh,
                                   static_cast<std::uint64_t>(al) * bl);
    }

public:
    static constexpr std::uint64_t multiplyHigh(std::uint64_t a, std::uint64_t b) noexcept
    {
        return multiplyHighHelper1(a >> 32, a, b >> 32, b);
    }
    friend constexpr UInt128 operator*(UInt128 a, UInt128 b) noexcept
    {
        return UInt128(a.high * b.low + a.low * b.high + multiplyHigh(a.low, b.low), a.low * b.low);
    }
    UInt128 &operator*=(UInt128 v) noexcept
    {
        return *this = *this * v;
    }
    struct DivModResult;
    static constexpr DivModResult divmod(UInt128 a, UInt128 b) noexcept;
    static constexpr UInt128 div(UInt128 a, UInt128 b) noexcept;
    static constexpr UInt128 mod(UInt128 a, UInt128 b) noexcept;
    friend constexpr UInt128 operator/(UInt128 a, UInt128 b) noexcept
    {
        return div(a, b);
    }
    friend constexpr UInt128 operator%(UInt128 a, UInt128 b) noexcept
    {
        return mod(a, b);
    }
    UInt128 &operator/=(UInt128 v) noexcept
    {
        return *this = *this / v;
    }
    UInt128 &operator%=(UInt128 v) noexcept
    {
        return *this = *this % v;
    }
    friend constexpr UInt128 operator&(UInt128 a, UInt128 b) noexcept
    {
        return UInt128(a.high & b.high, a.low & b.low);
    }
    UInt128 &operator&=(UInt128 v) noexcept
    {
        return *this = *this & v;
    }
    friend constexpr UInt128 operator|(UInt128 a, UInt128 b) noexcept
    {
        return UInt128(a.high | b.high, a.low | b.low);
    }
    UInt128 &operator|=(UInt128 v) noexcept
    {
        return *this = *this | v;
    }
    friend constexpr UInt128 operator^(UInt128 a, UInt128 b) noexcept
    {
        return UInt128(a.high ^ b.high, a.low ^ b.low);
    }
    UInt128 &operator^=(UInt128 v) noexcept
    {
        return *this = *this ^ v;
    }
    friend constexpr UInt128 operator<<(UInt128 v, unsigned shiftAmount) noexcept
    {
        return constexpr_assert(shiftAmount < 128),
               shiftAmount == 0 ? v : shiftAmount < 64 ?
                                  UInt128((v.high << shiftAmount) | (v.low >> (64 - shiftAmount)),
                                          v.low << shiftAmount) :
                                  shiftAmount == 64 ? UInt128(v.high, 0) :
                                                      UInt128(v.high << (shiftAmount - 64), 0);
    }
    UInt128 &operator<<=(unsigned shiftAmount) noexcept
    {
        return *this = *this << shiftAmount;
    }
    friend constexpr UInt128 operator>>(UInt128 v, unsigned shiftAmount) noexcept
    {
        return constexpr_assert(shiftAmount < 128),
               shiftAmount == 0 ? v : shiftAmount < 64 ?
                                  UInt128(v.high >> shiftAmount,
                                          (v.low >> shiftAmount) | (v.high << (64 - shiftAmount))) :
                                  shiftAmount == 64 ? UInt128(0, v.low) :
                                                      UInt128(0, v.low >> (shiftAmount - 64));
    }
    UInt128 &operator>>=(unsigned shiftAmount) noexcept
    {
        return *this = *this >> shiftAmount;
    }
    constexpr UInt128 operator+() noexcept
    {
        return *this;
    }
    constexpr UInt128 operator~() noexcept
    {
        return UInt128(~high, ~low);
    }
    constexpr UInt128 operator-() noexcept
    {
        return low != 0 ? UInt128(~high, -low) : UInt128(-high, 0);
    }
};

struct UInt128::DivModResult final
{
    UInt128 divResult;
    UInt128 modResult;
    constexpr DivModResult(UInt128 divResult, UInt128 modResult) noexcept : divResult(divResult),
                                                                            modResult(modResult)
    {
    }
};

constexpr UInt128::DivModResult UInt128::divmod(UInt128 a, UInt128 b) noexcept
{
#error finish
}

constexpr UInt128 UInt128::div(UInt128 a, UInt128 b) noexcept
{
    return divmod(a, b).divResult;
}

constexpr UInt128 UInt128::mod(UInt128 a, UInt128 b) noexcept
{
    return divmod(a, b).modResult;
}

struct ExtendedFloat final // modeled after IEEE754 standard
{
    std::uint64_t mantissa;
    std::uint16_t exponent;
    static constexpr std::uint16_t infinityNaNExponent() noexcept
    {
        return 0xFFFFU;
    }
    static constexpr std::uint16_t exponentBias() noexcept
    {
        return 0x7FFFU;
    }
    static constexpr std::uint64_t normalizedMantissaMax() noexcept
    {
        return 0xFFFFFFFFFFFFFFFFULL;
    }
    static constexpr std::uint64_t normalizedMantissaMin() noexcept
    {
        return 0x8000000000000000ULL;
    }
    void normalize() noexcept
    {
        if(exponent == infinityNaNExponent())
            return;
        if(mantissa == 0)
        {
            exponent = 0;
            return;
        }
#if defined(__GNUC__)
        bool isNeg = (mantissa < 0);
        if(isNeg)
        {
            mantissa = -mantissa;
        }
        int shiftAmount = clzMantissa(mantissa) - clzMantissa(normalizedMantissaMin());
        if(shiftAmount > 0)
        {
            if(exponent < static_cast<ExponentType>(-shiftAmount) + 0)
            {
                exponent = 0;
                mantissa = 0;
            }
            else
            {
                mantissa <<= shiftAmount;
                exponent -= shiftAmount;
            }
        }
        if(isNeg)
        {
            mantissa = -mantissa;
        }
#else
        while(mantissa > -normalizedMantissaMin() && mantissa < normalizedMantissaMin())
        {
            if(exponent == zeroExponent())
            {
                exponent = 0;
                mantissa = 0;
                return;
            }
            exponent--;
            mantissa <<= 1;
        }
#endif
    }
    constexpr ExtendedFloat() noexcept : mantissa(0), exponent(0)
    {
    }
    explicit ExtendedFloat(MantissaType mantissa, ExponentType exponent = exponentBias()) noexcept
        : mantissa(mantissa),
          exponent(exponent)
    {
        normalize();
    }
    explicit ExtendedFloat(double value) noexcept
    {
        if(std::isnan(value))
        {
            mantissa = 0;
            exponent = infinityNaNExponent();
            return;
        }
        if(std::isinf(value))
        {
            exponent = infinityNaNExponent();
            if(value < 0)
                mantissa = -1;
            else
                mantissa = 1;
            return;
        }
    }
};
}
}

#endif /* JAVASCRIPT_TASKLETS_SOFT_FLOAT_H_ */
