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

#if 1
#include "soft_float.h"
#if 1
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstdio>
namespace
{
using namespace javascript_tasklets::soft_float;
std::string hexValue(const ExtendedFloat &v)
{
    if(v.isNaN())
    {
        return "NaN";
    }
    if(v.isInfinite())
    {
        if(v.sign)
            return "-Infinity";
        return "+Infinity";
    }
    std::ostringstream ss;
    ss << std::hex << std::uppercase;
    ss.fill('0');
    if(v.sign)
        ss << "-";
    else
        ss << "+";
    ss << "0x";
    std::int32_t exponent = v.exponent;
    exponent -= ExtendedFloat::exponentBias();
    if(v.isZero())
        exponent = 0;
    std::uint64_t mantissa = v.mantissa;
    ss << (mantissa >> 63);
    mantissa <<= 1;
    ss << ".";
    ss.width(16);
    ss << mantissa;
    ss << "p";
    ss << std::dec << std::showpos;
    ss << exponent;
    return ss.str();
}
std::string hexValue(double v)
{
    if(std::isnan(v))
    {
        return "NaN";
    }
    if(std::isinf(v))
    {
        if(v < 0)
            return "-Infinity";
        return "+Infinity";
    }
    const std::size_t strSize = 64;
    char str[strSize];
    std::snprintf(str, sizeof(str), "%+1.16A", v);
    for(char &ch : str)
    {
        if(ch == '\0')
            break;
        if(ch == 'X')
            ch = 'x';
        else if(ch == 'P')
            ch = 'p';
    }
    return str;
}
void mainFn()
{
    auto a = ExtendedFloat(0x1.FFFFFFFFFFFFFp63);
    a += ExtendedFloat::One();
    std::cout << hexValue(a) << " == " << hexValue(static_cast<double>(a)) << std::endl;
}
struct Init
{
    Init()
    {
        mainFn();
        std::exit(0);
    }
};
Init init;
}
#endif
#else
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <list>
#include <sstream>
#include "constexpr_assert.h"

namespace
{
unsigned clz8(std::uint8_t v)
{
    return __builtin_clz(v) - __builtin_clz(0x80U);
}
unsigned ctz8(std::uint8_t v)
{
    return v == 0 ? 8 : __builtin_ctz(v);
}
struct UInt16 final
{
    std::uint8_t high;
    std::uint8_t low;
    explicit UInt16(std::uint8_t low = 0) : high(0), low(low)
    {
    }
    UInt16(std::uint8_t high, std::uint8_t low) : high(high), low(low)
    {
    }
    friend unsigned clz16(UInt16 v)
    {
        return v.high == 0 ? 8 + clz8(v.low) : clz8(v.high);
    }
    friend unsigned ctz16(UInt16 v)
    {
        return v.low == 0 ? 8 + ctz8(v.high) : ctz8(v.low);
    }
    static UInt16 mul8x8(std::uint8_t a, std::uint8_t b)
    {
        unsigned v = a;
        v *= b;
        return UInt16(v >> 8, v & 0xFFU);
    }
    static bool addCarry(std::uint8_t a, std::uint8_t b)
    {
        return static_cast<std::uint16_t>(a) + b > 0xFFU;
    }
    static bool addCarry(std::uint8_t a, std::uint8_t b, bool carry)
    {
        return static_cast<unsigned>(a) + b + carry > 0xFFU;
    }
    static bool subBorrow(std::uint8_t a, std::uint8_t b)
    {
        return a < b;
    }
    static bool subBorrow(std::uint8_t a, std::uint8_t b, bool borrow)
    {
        return a < b || (a == b && borrow);
    }
    friend UInt16 operator+(UInt16 a, UInt16 b)
    {
        return UInt16(a.high + b.high + addCarry(a.low, b.low), a.low + b.low);
    }
    friend UInt16 operator-(UInt16 a, UInt16 b)
    {
        return UInt16(a.high - b.high - subBorrow(a.low, b.low), a.low - b.low);
    }
    friend UInt16 operator<<(UInt16 v, unsigned shiftAmount)
    {
        return shiftAmount == 0 ? v : shiftAmount < 8 ?
                                  UInt16((v.high << shiftAmount) | (v.low >> (8 - shiftAmount)),
                                         v.low << shiftAmount) :
                                  UInt16(v.low << (shiftAmount - 8), 0);
    }
    friend UInt16 operator>>(UInt16 v, unsigned shiftAmount)
    {
        return shiftAmount == 0 ? v : shiftAmount < 8 ?
                                  UInt16(v.high >> shiftAmount,
                                         (v.low >> shiftAmount) | (v.high << (8 - shiftAmount))) :
                                  UInt16(v.high >> (shiftAmount - 8));
    }
    struct DivModResult8 final
    {
        std::uint8_t divResult;
        std::uint8_t modResult;
        DivModResult8(std::uint8_t divResult, std::uint8_t modResult)
            : divResult(divResult), modResult(modResult)
        {
        }
    };
    static DivModResult8 divMod16x8(UInt16 n, std::uint8_t d)
    {
        constexpr_assert(d != 0);
        std::uint16_t v = n.high;
        v <<= 8;
        v |= n.low;
        std::uint16_t divResult = v / d;
        std::uint16_t modResult = v % d;
        constexpr_assert(divResult <= 0xFFU);
        constexpr_assert(modResult <= 0xFFU);
        return DivModResult8(divResult, modResult);
    }
    struct DivModResult;
    static DivModResult divMod(UInt16 uIn, UInt16 vIn);
    static DivModResult divMod2(UInt16 n, UInt16 d);
    friend bool operator==(UInt16 a, UInt16 b) noexcept
    {
        return a.high == b.high && a.low == b.low;
    }
    friend bool operator!=(UInt16 a, UInt16 b) noexcept
    {
        return a.high != b.high || a.low != b.low;
    }
    friend bool operator<(UInt16 a, UInt16 b) noexcept
    {
        return a.high < b.high || (a.high == b.high && a.low < b.low);
    }
    friend bool operator<=(UInt16 a, UInt16 b) noexcept
    {
        return a.high < b.high || (a.high == b.high && a.low <= b.low);
    }
    friend bool operator>(UInt16 a, UInt16 b) noexcept
    {
        return a.high > b.high || (a.high == b.high && a.low > b.low);
    }
    friend bool operator>=(UInt16 a, UInt16 b) noexcept
    {
        return a.high > b.high || (a.high == b.high && a.low >= b.low);
    }
};
struct UInt16::DivModResult final
{
    UInt16 divResult;
    UInt16 modResult;
    DivModResult(UInt16 divResult, UInt16 modResult) : divResult(divResult), modResult(modResult)
    {
    }
};
UInt16::DivModResult UInt16::divMod2(UInt16 n, UInt16 d)
{
    std::uint16_t nv = n.high;
    nv <<= 8;
    nv |= n.low;
    std::uint16_t dv = d.high;
    dv <<= 8;
    dv |= d.low;
    std::uint16_t qv = nv / dv;
    std::uint16_t rv = nv % dv;
    return DivModResult(UInt16(qv >> 8, qv & 0xFF), UInt16(rv >> 8, rv & 0xFF));
}
template <std::size_t NumberSizes,
          typename Digit,
          typename DoubleDigit,
          unsigned DigitBitCount,
          typename DigitCLZFn>
void divMod(const Digit(&numerator)[NumberSizes],
            const Digit(&denominator)[NumberSizes],
            Digit(&quotient)[NumberSizes],
            Digit(&remainder)[NumberSizes])
{
    constexpr Digit DigitMax = (static_cast<DoubleDigit>(1) << DigitBitCount) - 1;
    static_assert(NumberSizes != 0, "bad size");
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
        return;
    }
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
struct OpClz4 final
{
    constexpr unsigned operator()(std::uint16_t value) const noexcept
    {
        return __builtin_clz(value) - (__builtin_clz(0) - 4);
    }
};
UInt16::DivModResult UInt16::divMod(UInt16 uIn, UInt16 vIn)
{
    constexpr std::size_t NumberSizes = 4;
    typedef std::uint16_t Digit;
    typedef unsigned DoubleDigit;
    constexpr unsigned DigitBitCount = 4;
    Digit numerator[NumberSizes], denominator[NumberSizes], quotient[NumberSizes],
        remainder[NumberSizes];
    numerator[0] = uIn.high >> 4;
    numerator[1] = uIn.high & 0xF;
    numerator[2] = uIn.low >> 4;
    numerator[3] = uIn.low & 0xF;
    denominator[0] = vIn.high >> 4;
    denominator[1] = vIn.high & 0xF;
    denominator[2] = vIn.low >> 4;
    denominator[3] = vIn.low & 0xF;
    ::divMod<NumberSizes, Digit, DoubleDigit, DigitBitCount, OpClz4>(
        numerator, denominator, quotient, remainder);
    return DivModResult(
        UInt16((quotient[0] << 4) | quotient[1], (quotient[2] << 4) | quotient[3]),
        UInt16((remainder[0] << 4) | remainder[1], (remainder[2] << 4) | remainder[3]));
}
void mainFn(std::uint8_t start, std::uint8_t end)
{
    for(unsigned dHigh = start; dHigh <= end; dHigh++)
    {
        if(start == 0)
        {
            std::ostringstream ss;
            ss << dHigh * 100 / (end + 1) << "%\n";
            std::cout << ss.str() << std::flush;
        }
        for(unsigned dLow = 0; dLow < 0x100U; dLow++)
        {
            UInt16 d(dHigh, dLow);
            if(d == UInt16(0))
                continue;
#if 0
            if(d < UInt16(2, 0))
                continue;
#endif
            for(unsigned nHigh = 0; nHigh < 0x100U; nHigh++)
            {
                for(unsigned nLow = 0; nLow < 0x100U; nLow++)
                {
                    UInt16 n(nHigh, nLow);
                    auto result = UInt16::divMod(n, d);
                    auto result2 = UInt16::divMod2(n, d);
                    if(result.divResult != result2.divResult
                       || result.modResult != result2.modResult)
                    {
                        std::ostringstream ss;
                        ss << std::hex << std::uppercase;
                        ss.fill('0');
                        ss.width(2);
                        ss << static_cast<unsigned>(n.high);
                        ss.width(2);
                        ss << static_cast<unsigned>(n.low);
                        ss << " / ";
                        ss.width(2);
                        ss << static_cast<unsigned>(d.high);
                        ss.width(2);
                        ss << static_cast<unsigned>(d.low);
                        ss << " == ";
                        ss.width(2);
                        ss << static_cast<unsigned>(result.divResult.high);
                        ss.width(2);
                        ss << static_cast<unsigned>(result.divResult.low);
                        ss << ", ";
                        ss.width(2);
                        ss << static_cast<unsigned>(result2.divResult.high);
                        ss.width(2);
                        ss << static_cast<unsigned>(result2.divResult.low);
                        ss << std::endl;
                        ss.width(2);
                        ss << static_cast<unsigned>(n.high);
                        ss.width(2);
                        ss << static_cast<unsigned>(n.low);
                        ss << " % ";
                        ss.width(2);
                        ss << static_cast<unsigned>(d.high);
                        ss.width(2);
                        ss << static_cast<unsigned>(d.low);
                        ss << " == ";
                        ss.width(2);
                        ss << static_cast<unsigned>(result.modResult.high);
                        ss.width(2);
                        ss << static_cast<unsigned>(result.modResult.low);
                        ss << ", ";
                        ss.width(2);
                        ss << static_cast<unsigned>(result2.modResult.high);
                        ss.width(2);
                        ss << static_cast<unsigned>(result2.modResult.low);
                        std::cout << ss.str() << std::endl;
                        return;
                    }
                }
            }
        }
    }
}
struct Init
{
    Init()
    {
        const std::size_t splitCount = 6;
        std::list<std::thread> threads;
        for(std::size_t i = 0; i < splitCount; i++)
        {
            auto start = i * 0x100 / splitCount;
            auto end = (i + 1) * 0x100 / splitCount - 1;
            threads.push_back(std::thread([=]()
                                          {
                                              mainFn(start, end);
                                          }));
        }
        for(std::thread &thread : threads)
            thread.join();
        std::exit(0);
    }
};
Init init;
}
#endif
