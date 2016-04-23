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
    static DivModResult divmod(UInt128 a, UInt128 b) noexcept;
    static UInt128 div(UInt128 a, UInt128 b) noexcept;
    static UInt128 mod(UInt128 a, UInt128 b) noexcept;
    friend UInt128 operator/(UInt128 a, UInt128 b) noexcept
    {
        return div(a, b);
    }
    friend UInt128 operator%(UInt128 a, UInt128 b) noexcept
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
    friend constexpr bool operator==(UInt128 a, UInt128 b) noexcept
    {
        return a.high == b.high && a.low == b.low;
    }
    friend constexpr bool operator!=(UInt128 a, UInt128 b) noexcept
    {
        return a.high != b.high || a.low != b.low;
    }
    friend constexpr bool operator<(UInt128 a, UInt128 b) noexcept
    {
        return a.high < b.high || (a.high == b.high && a.low < b.low);
    }
    friend constexpr bool operator<=(UInt128 a, UInt128 b) noexcept
    {
        return a.high < b.high || (a.high == b.high && a.low <= b.low);
    }
    friend constexpr bool operator>(UInt128 a, UInt128 b) noexcept
    {
        return a.high > b.high || (a.high == b.high && a.low > b.low);
    }
    friend constexpr bool operator>=(UInt128 a, UInt128 b) noexcept
    {
        return a.high > b.high || (a.high == b.high && a.low >= b.low);
    }
    friend constexpr unsigned clz128(UInt128 v) noexcept
    {
        return v.high == 0 ? 64 + vm::math::clz64(v.low) : vm::math::clz64(v.high);
    }
    friend constexpr unsigned ctz128(UInt128 v) noexcept
    {
        return v.low == 0 ? 64 + vm::math::ctz64(v.high) : vm::math::ctz64(v.low);
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

inline UInt128::DivModResult UInt128::divmod(UInt128 a, UInt128 b) noexcept
{
    constexpr std::size_t NumberSizes = 4;
    typedef std::uint32_t Digit;
    typedef std::uint64_t DoubleDigit;
    constexpr unsigned DigitBitCount = 32;
    struct DigitCLZFn final
    {
        constexpr unsigned operator()(Digit v) const noexcept
        {
            return vm::math::clz32(v);
        }
    };
    constexpr Digit DigitMax = (static_cast<DoubleDigit>(1) << DigitBitCount) - 1;
    const Digit numerator[NumberSizes] = {
        static_cast<Digit>(a.high >> DigitBitCount),
        static_cast<Digit>(a.high & DigitMax),
        static_cast<Digit>(a.low >> DigitBitCount),
        static_cast<Digit>(a.low & DigitMax),
    };
    const Digit denominator[NumberSizes] = {
        static_cast<Digit>(b.high >> DigitBitCount),
        static_cast<Digit>(b.high & DigitMax),
        static_cast<Digit>(b.low >> DigitBitCount),
        static_cast<Digit>(b.low & DigitMax),
    };
    Digit quotient[NumberSizes];
    Digit remainder[NumberSizes];
    std::size_t m = NumberSizes;
    for(std::size_t i = 0; i < NumberSizes; i++)
    {
        if(denominator[i] != 0)
        {
            m = i;
            break;
        }
    }
    const std::size_t n = NumberSizes - m;
    if(n <= 1)
    {
        constexpr_assert(denominator[NumberSizes - 1] != 0);
        for(std::size_t i = 0; i < NumberSizes - 1; i++)
        {
            remainder[i] = 0;
        }
        Digit currentRemainder = 0;
        for(std::size_t i = 0; i < NumberSizes; i++)
        {
            DoubleDigit n = currentRemainder;
            n <<= DigitBitCount;
            n |= numerator[i];
            quotient[i] = n / denominator[NumberSizes - 1];
            currentRemainder = n % denominator[NumberSizes - 1];
        }
        remainder[NumberSizes - 1] = currentRemainder;
    }
    else
    {
        // from algorithm D, section 4.3.1 in Art of Computer Programming volume 2 by Knuth.
        unsigned log2D = DigitCLZFn()(denominator[m]);
        Digit u[NumberSizes + 1];
        u[NumberSizes] = (numerator[NumberSizes - 1] << log2D) & DigitMax;
        u[0] = ((static_cast<DoubleDigit>(numerator[0]) << log2D) >> DigitBitCount) & DigitMax;
        for(std::size_t i = 1; i < NumberSizes; i++)
        {
            DoubleDigit value = numerator[i - 1];
            value <<= DigitBitCount;
            value |= numerator[i];
            value <<= log2D;
            u[i] = (value >> DigitBitCount) & DigitMax;
        }
        Digit v[NumberSizes + 1] = {};
        v[n] = (denominator[NumberSizes - 1] << log2D) & DigitMax;
        for(std::size_t i = 1; i < n; i++)
        {
            DoubleDigit value = denominator[m + i - 1];
            value <<= DigitBitCount;
            value |= denominator[m + i];
            value <<= log2D;
            v[i] = (value >> DigitBitCount) & DigitMax;
            quotient[i - 1] = 0;
        }
        for(std::size_t j = 0; j <= m; j++)
        {
            DoubleDigit qHat;
            if(u[j] == v[1])
            {
                qHat = DigitMax;
            }
            else
            {
                qHat = ((static_cast<DoubleDigit>(u[j]) << DigitBitCount) | u[j + 1]) / v[1];
            }
            {
                DoubleDigit lhs = v[2] * qHat;
                DoubleDigit rhsHigh =
                    ((static_cast<DoubleDigit>(u[j]) << DigitBitCount) | u[j + 1]) - qHat * v[1];
                Digit rhsLow = u[j + 2];
                if(rhsHigh < static_cast<DoubleDigit>(1) << DigitBitCount
                   && lhs > ((rhsHigh << DigitBitCount) | rhsLow))
                {
                    qHat--;
                    lhs -= v[2];
                    rhsHigh += v[1];
                    if(rhsHigh < static_cast<DoubleDigit>(1) << DigitBitCount
                       && lhs > ((rhsHigh << DigitBitCount) | rhsLow))
                    {
                        qHat--;
                    }
                }
            }
            bool borrow = false;
            {
                Digit mulCarry = 0;
                for(std::size_t i = n; i > 0; i--)
                {
                    constexpr_assert(i <= NumberSizes);
                    DoubleDigit product = qHat * v[i] + mulCarry;
                    mulCarry = product >> DigitBitCount;
                    product &= DigitMax;
                    bool prevBorrow = borrow;
                    DoubleDigit digit = u[j + i] - product - prevBorrow;
                    borrow = digit != (digit & DigitMax);
                    digit &= DigitMax;
                    u[j + i] = digit;
                }
                bool prevBorrow = borrow;
                DoubleDigit digit = u[j] - mulCarry - prevBorrow;
                borrow = digit != (digit & DigitMax);
                digit &= DigitMax;
                u[j] = digit;
            }
            Digit qj = qHat;
            if(borrow)
            {
                qj--;
                bool carry = false;
                for(std::size_t i = n; i > 0; i--)
                {
                    bool prevCarry = carry;
                    constexpr_assert(i + j <= NumberSizes);
                    DoubleDigit digit = u[j + i] + v[i] + prevCarry;
                    carry = digit != (digit & DigitMax);
                    digit &= DigitMax;
                    u[j + i] = digit;
                }
                u[j] = (u[j] + carry) & DigitMax;
            }
            quotient[j + n - 1] = qj;
        }
        for(std::size_t i = 0; i < NumberSizes; i++)
        {
            DoubleDigit value = u[i];
            value <<= DigitBitCount;
            value |= u[i + 1];
            remainder[i] = value >> log2D;
        }
    }
    return DivModResult(
        UInt128((static_cast<DoubleDigit>(quotient[0]) << DigitBitCount) | quotient[1],
                (static_cast<DoubleDigit>(quotient[2]) << DigitBitCount) | quotient[3]),
        UInt128((static_cast<DoubleDigit>(remainder[0]) << DigitBitCount) | remainder[1],
                (static_cast<DoubleDigit>(remainder[2]) << DigitBitCount) | remainder[3]));
}

inline UInt128 UInt128::div(UInt128 a, UInt128 b) noexcept
{
    return divmod(a, b).divResult;
}

inline UInt128 UInt128::mod(UInt128 a, UInt128 b) noexcept
{
    return divmod(a, b).modResult;
}

#if 0
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
#endif
}
}

#endif /* JAVASCRIPT_TASKLETS_SOFT_FLOAT_H_ */
