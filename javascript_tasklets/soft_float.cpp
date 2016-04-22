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
UInt16::DivModResult UInt16::divMod(UInt16 uIn, UInt16 vIn)
{
    if(vIn.high == 0)
    {
        if(uIn.high == 0)
        {
            return DivModResult(UInt16(uIn.low / vIn.low), UInt16(uIn.low % vIn.low));
        }
        auto qr = divMod16x8(UInt16(uIn.high % vIn.low, uIn.low), vIn.low);
        return DivModResult(UInt16(uIn.high / vIn.low, qr.divResult), UInt16(qr.modResult));
    }
    if(uIn < vIn)
    {
        return DivModResult(UInt16(0), uIn);
    }
    unsigned log2D = clz8(vIn.high);
    std::uint8_t u[3], v[3] = {};
    u[2] = (uIn << log2D).low;
    u[1] = (uIn << log2D).high;
    u[0] = (UInt16(uIn.high) << log2D).high;
    v[1] = (vIn << log2D).high;
    v[2] = (vIn << log2D).low;
    unsigned j = 0;
    std::uint8_t qHat;
    if(u[j] == v[1])
        qHat = 0xFF;
    else
        qHat = divMod16x8(UInt16(u[j], u[j + 1]), v[1]).divResult;
    {
        UInt16 rHigh = UInt16(u[j], u[j + 1]) - mul8x8(v[1], qHat);
        std::uint8_t rLow = u[j + 2];
        UInt16 l = mul8x8(v[2], qHat);
        if(rHigh.high == 0 && l > UInt16(rHigh.low, rLow))
        {
            qHat--;
            rHigh = rHigh + UInt16(v[1]);
            l = l - UInt16(v[2]);
        }
        if(rHigh.high == 0 && l > UInt16(rHigh.low, rLow))
        {
            qHat--;
        }
    }
    bool borrow;
    {
        UInt16 product = mul8x8(qHat, v[2]);
        std::uint8_t mulCarry = product.high;
        borrow = subBorrow(u[j + 2], product.low);
        u[j + 2] -= product.low;
        product = mul8x8(qHat, v[1]) + UInt16(mulCarry);
        mulCarry = product.high;
        bool prevBorrow = borrow;
        borrow = subBorrow(u[j + 1], product.low, prevBorrow);
        u[j + 1] -= product.low + prevBorrow;
        prevBorrow = borrow;
        borrow = subBorrow(u[j], mulCarry, prevBorrow);
        u[j] -= mulCarry + prevBorrow;
    }
    std::uint8_t qj = qHat;
    if(borrow)
    {
        qj--;
        bool carry = addCarry(u[j + 2], v[2]);
        u[j + 2] += v[2];
        bool prevCarry = carry;
        carry = addCarry(u[j + 1], v[1], prevCarry);
        u[j + 1] += v[1] + prevCarry;
        u[j] += carry;
    }
    return DivModResult(UInt16(qj), UInt16(u[1], u[2]) >> log2D);
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
