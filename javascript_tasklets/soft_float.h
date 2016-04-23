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

#if 1
struct ExtendedFloat final // modeled after IEEE754 standard
{
    std::uint64_t mantissa;
    std::uint16_t exponent;
    bool sign;
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
    struct NormalizedTag final
    {
    };
    static constexpr ExtendedFloat normalizeHelper(const ExtendedFloat &v,
                                                   unsigned shiftAmount) noexcept
    {
        return shiftAmount > 0 && v.exponent >= -shiftAmount ?
                   ExtendedFloat(NormalizedTag{},
                                 v.mantissa << shiftAmount,
                                 v.exponent - shiftAmount,
                                 v.sign) :
                   v;
    }
    static constexpr ExtendedFloat normalize(const ExtendedFloat &v) noexcept
    {
        return v.exponent == infinityNaNExponent() ? v : v.mantissa == 0 ?
                                                     Zero(v.sign) :
                                                     normalizeHelper(v, vm::math::clz64(v.mantissa));
    }
    constexpr ExtendedFloat() noexcept : mantissa(0), exponent(0), sign(false)
    {
    }
    constexpr ExtendedFloat(NormalizedTag,
                            std::uint64_t mantissa,
                            std::uint16_t exponent,
                            bool sign = false) noexcept : mantissa(mantissa),
                                                          exponent(exponent),
                                                          sign(sign)
    {
    }
    explicit constexpr ExtendedFloat(std::uint64_t mantissa,
                                     std::uint16_t exponent = exponentBias(),
                                     bool sign = false) noexcept
        : ExtendedFloat(normalize(ExtendedFloat(NormalizedTag{}, mantissa, exponent, sign)))
    {
    }
    explicit ExtendedFloat(double value) noexcept : mantissa(0),
                                                    exponent(0),
                                                    sign(std::signbit(value))
    {
        value = std::fabs(value);
        if(std::isnan(value))
        {
            mantissa = 1;
            exponent = infinityNaNExponent();
            return;
        }
        if(std::isinf(value))
        {
            exponent = infinityNaNExponent();
            mantissa = 0;
            return;
        }
        if(value == 0)
        {
            exponent = 0;
            mantissa = 0;
            return;
        }
        int log2Value = std::ilogb(value);
        exponent = log2Value + exponentBias();
        value = std::scalbn(value, log2Value + 63);
        mantissa = value;
    }
    explicit operator double() const noexcept
    {
        if(exponent == infinityNaNExponent())
        {
            double retval = std::numeric_limits<double>::infinity();
            if(mantissa)
                retval = std::numeric_limits<double>::quiet_NaN();
            if(sign)
                return -retval;
            return retval;
        }
        if(exponent == 0)
        {
            if(sign)
                return -0.0;
            return 0;
        }
        double value = std::scalbln(mantissa, static_cast<long>(exponent) - exponentBias() - 63);
        if(sign)
            return -value;
        return value;
    }
    constexpr bool isNaN() const noexcept
    {
        return exponent == infinityNaNExponent() && mantissa != 0;
    }
    constexpr bool isInfinite() const noexcept
    {
        return exponent == infinityNaNExponent() && mantissa == 0;
    }
    constexpr bool isFinite() const noexcept
    {
        return exponent != infinityNaNExponent();
    }
    constexpr bool isNormal() const noexcept
    {
        return exponent != infinityNaNExponent() && exponent != 0;
    }
    constexpr bool isDenormal() const noexcept
    {
        return exponent == 0 && mantissa != 0;
    }
    constexpr bool isZero() const noexcept
    {
        return exponent == 0 && mantissa == 0;
    }
    constexpr bool signBit() const noexcept
    {
        return sign;
    }
    static constexpr ExtendedFloat NaN() noexcept
    {
        return ExtendedFloat(NormalizedTag{}, 1, infinityNaNExponent());
    }
    static constexpr ExtendedFloat Infinity(bool sign = false) noexcept
    {
        return ExtendedFloat(NormalizedTag{}, 0, infinityNaNExponent(), sign);
    }
    static constexpr ExtendedFloat Zero(bool sign = false) noexcept
    {
        return ExtendedFloat(NormalizedTag{}, 0, 0, sign);
    }
    constexpr ExtendedFloat operator+() const noexcept
    {
        return *this;
    }
    constexpr ExtendedFloat operator-() const noexcept
    {
        return ExtendedFloat(NormalizedTag{}, mantissa, exponent, !sign);
    }
    static constexpr UInt128 shiftHelper(std::uint64_t a, unsigned shift) noexcept
    {
        return shift >= 128 ? UInt128(0) : UInt128(a, 0) >> shift;
    }
    static constexpr std::uint64_t roundHelper(UInt128 v) noexcept
    {
        return v.low == 0x8000000000000000ULL && (v.high & 1) == 0 ?
                   v.high :
                   (v + UInt128(0x8000000000000000ULL)).high;
    }
    static constexpr ExtendedFloat subtractHelper6(UInt128 mantissa,
                                                   std::uint16_t exponent,
                                                   bool sign,
                                                   unsigned shift)
    {
        return ExtendedFloat(
            NormalizedTag{}, roundHelper(mantissa << shift), exponent - shift, sign);
    }
    static constexpr ExtendedFloat subtractHelper5(UInt128 mantissa,
                                                   std::uint16_t exponent,
                                                   bool sign,
                                                   unsigned shift)
    {
        return subtractHelper6(mantissa, exponent, sign, shift > exponent ? exponent : shift);
    }
    static constexpr ExtendedFloat subtractHelper4(UInt128 mantissa,
                                                   std::uint16_t exponent,
                                                   bool sign)
    {
        return subtractHelper5(mantissa, exponent, sign, clz128(mantissa));
    }
    static constexpr ExtendedFloat subtractHelper3(UInt128 aMantissa,
                                                   UInt128 bMantissa,
                                                   std::uint16_t exponent) noexcept
    {
        return aMantissa == bMantissa ? Zero() : aMantissa < bMantissa ?
                                        subtractHelper4(bMantissa - aMantissa, exponent, true) :
                                        subtractHelper4(aMantissa - bMantissa, exponent, false);
    }
    static constexpr ExtendedFloat subtractHelper2(std::uint64_t aMantissa,
                                                   std::uint16_t aExponent,
                                                   std::uint64_t bMantissa,
                                                   std::uint16_t bExponent,
                                                   std::uint16_t maxExponent) noexcept
    {
        return subtractHelper3(shiftHelper(aMantissa, maxExponent - aExponent),
                               shiftHelper(bMantissa, maxExponent - bExponent),
                               maxExponent);
    }
    static constexpr ExtendedFloat subtractHelper(std::uint64_t aMantissa,
                                                  std::uint16_t aExponent,
                                                  std::uint64_t bMantissa,
                                                  std::uint16_t bExponent) noexcept
    {
        return subtractHelper2(aMantissa,
                               aExponent,
                               bMantissa,
                               bExponent,
                               aExponent < bExponent ? bExponent : aExponent);
    }
    static constexpr ExtendedFloat addHelper3(UInt128 mantissa,
                                              std::uint16_t exponent,
                                              bool sign) noexcept
    {
        return mantissa >= UInt128(0x8000000000000000ULL, 0) ?
                   (exponent + 1 == infinityNaNExponent() ?
                        Infinity(sign) :
                        ExtendedFloat(NormalizedTag{}, roundHelper(mantissa), exponent + 1, sign)) :
                   ExtendedFloat(NormalizedTag{}, roundHelper(mantissa << 1), exponent + 1, sign);
    }
    static constexpr ExtendedFloat addHelper2(std::uint64_t aMantissa,
                                              std::uint16_t aExponent,
                                              std::uint64_t bMantissa,
                                              std::uint16_t bExponent,
                                              std::uint16_t maxExponent,
                                              bool sign) noexcept
    {
        return addHelper3(shiftHelper(aMantissa, maxExponent - aExponent + 1)
                              + shiftHelper(bMantissa, maxExponent - bExponent + 1),
                          maxExponent,
                          sign);
    }
    static constexpr ExtendedFloat addHelper(std::uint64_t aMantissa,
                                             std::uint16_t aExponent,
                                             std::uint64_t bMantissa,
                                             std::uint16_t bExponent,
                                             bool sign) noexcept
    {
        return addHelper2(aMantissa,
                          aExponent,
                          bMantissa,
                          bExponent,
                          aExponent < bExponent ? bExponent : aExponent,
                          sign);
    }
    constexpr friend ExtendedFloat operator+(const ExtendedFloat &a,
                                             const ExtendedFloat &b) noexcept
    {
        return a.isNaN() ? a : b.isNaN() ?
                           b :
                           a.isInfinite() ?
                           (b.isInfinite() ? (a.sign == b.sign ? a : NaN()) : a) :
                           b.isInfinite() ?
                           b :
                           a.isZero() ?
                           (b.isZero() ? Zero(a.sign && b.sign) : b) :
                           b.isZero() ?
                           a :
                           a.sign == b.sign ?
                           addHelper(a.mantissa, a.exponent, b.mantissa, b.exponent, a.sign) :
                           a.sign ? subtractHelper(b.mantissa, b.exponent, a.mantissa, a.exponent) :
                                    subtractHelper(a.mantissa, a.exponent, b.mantissa, b.exponent);
    }
    constexpr friend ExtendedFloat operator-(const ExtendedFloat &a,
                                             const ExtendedFloat &b) noexcept
    {
        return a + b.operator-();
    }
    ExtendedFloat &operator+=(const ExtendedFloat &v) noexcept
    {
        return *this = *this + v;
    }
    ExtendedFloat &operator-=(const ExtendedFloat &v) noexcept
    {
        return *this = *this - v;
    }
    friend constexpr bool operator==(const ExtendedFloat &a, const ExtendedFloat &b) noexcept
    {
        return a.isNaN() ? false : b.isNaN() ? false : a.isZero() ?
                                               b.isZero() :
                                               a.exponent == b.exponent && a.mantissa == b.mantissa;
    }
    friend constexpr bool operator!=(const ExtendedFloat &a, const ExtendedFloat &b) noexcept
    {
        return !(a == b);
    }
    static constexpr int compareHelper(const ExtendedFloat &a, const ExtendedFloat &b) noexcept
    {
        return a.isZero() ? (b.isZero() ? 0 : (b.sign ? 1 : -1)) : a.sign != b.sign ?
                            (a.sign ? -1 : 1) :
                            a.exponent != b.exponent ?
                            ((a.exponent < b.exponent) != a.sign ? -1 : 1) :
                            a.mantissa == b.mantissa ? 0 :
                                                       (a.mantissa < b.mantissa) != a.sign ? -1 : 1;
    }
    friend constexpr bool operator<(const ExtendedFloat &a, const ExtendedFloat &b) noexcept
    {
        return a.isNaN() ? false : b.isNaN() ? false : compareHelper(a, b) < 0;
    }
    friend constexpr bool operator<=(const ExtendedFloat &a, const ExtendedFloat &b) noexcept
    {
        return a.isNaN() ? false : b.isNaN() ? false : compareHelper(a, b) <= 0;
    }
    friend constexpr bool operator>(const ExtendedFloat &a, const ExtendedFloat &b) noexcept
    {
        return a.isNaN() ? false : b.isNaN() ? false : compareHelper(a, b) > 0;
    }
    friend constexpr bool operator>=(const ExtendedFloat &a, const ExtendedFloat &b) noexcept
    {
        return a.isNaN() ? false : b.isNaN() ? false : compareHelper(a, b) >= 0;
    }
};
#endif
}
}

#endif /* JAVASCRIPT_TASKLETS_SOFT_FLOAT_H_ */
