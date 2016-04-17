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

#ifndef JAVASCRIPT_TASKLETS_VM_INTERPRETER_H_
#define JAVASCRIPT_TASKLETS_VM_INTERPRETER_H_

#include "../value.h"

namespace javascript_tasklets
{
namespace vm
{
namespace interpreter
{
struct BaseIndex
{
    std::size_t index;
    static constexpr std::size_t noIndex = static_cast<std::size_t>(-1);
    explicit constexpr BaseIndex(std::size_t index) noexcept
        : index((constexpr_assert(index != noIndex), index))
    {
    }
    explicit constexpr BaseIndex(std::nullptr_t = nullptr) noexcept : index(noIndex)
    {
    }
    constexpr bool empty() const noexcept
    {
        return index == noIndex;
    }
    constexpr bool operator==(BaseIndex rt) const noexcept
    {
        return index == rt.index;
    }
    constexpr bool operator!=(BaseIndex rt) const noexcept
    {
        return index != rt.index;
    }
    constexpr bool operator==(std::nullptr_t) const noexcept
    {
        return empty();
    }
    constexpr bool operator!=(std::nullptr_t) const noexcept
    {
        return !empty();
    }
    friend constexpr bool operator==(std::nullptr_t, BaseIndex v) const noexcept
    {
        return v.empty();
    }
    friend constexpr bool operator!=(std::nullptr_t, BaseIndex v) const noexcept
    {
        return !v.empty();
    }
};

struct RegisterIndex final : public BaseIndex
{
    using BaseIndex::BaseIndex;
};

struct Instruction
{
    enum class Operation
    {
        AddU, // uint32 + uint32 -> uint32
        AddI, // int32 + int32 -> int32
        AddD, // double + double -> double
        SubU, // uint32 - uint32 -> uint32
        SubI, // int32 - int32 -> int32
        SubD, // double - double -> double
        MulU, // uint32 * uint32 -> uint32
        MulI, // int32 * int32 -> int32
        MulD, // double * double -> double
        DivU, // uint32 / uint32 -> uint32
        DivI, // int32 / int32 -> int32
        DivD, // double / double -> double
        ModU, // uint32 % uint32 -> uint32
        ModI, // int32 % int32 -> int32
        ModD, // double % double -> double
        ShlU, // uint32 << uint32 -> uint32
        ShlI, // int32 << uint32 -> int32
        ShrU, // uint32 >> uint32 -> uint32
        ShrI, // uint32 >> uint32 -> int32
        SarU, // int32 >> uint32 -> uint32
        SarI, // int32 >> uint32 -> int32
        AndU, // uint32 & uint32 -> uint32
        AndI, // int32 & int32 -> int32
        OrU, // uint32 | uint32 -> uint32
        OrI, // int32 | int32 -> int32
        XorU, // uint32 ^ uint32 -> uint32
        XorI, // int32 ^ int32 -> int32
        CmpEqI, // int32 == int32 -> bool
        CmpEqUI, // uint32 == int32 -> bool
        CmpEqD, // double == double -> bool
        CmpNEI, // int32 != int32 -> bool
        CmpNEUI, // uint32 != int32 -> bool
        CmpNED, // double != double -> bool
        CmpLTI, // int32 < int32 -> bool
        CmpLTUI, // uint32 < int32 -> bool
        CmpLTIU, // int32 < uint32 -> bool
        CmpLTU, // uint32 < uint32 -> bool
        CmpLTD, // double < double -> bool
        CmpNLTD, // !(double < double) -> bool
        CmpLEI, // int32 <= int32 -> bool
        CmpLEUI, // uint32 <= int32 -> bool
        CmpLEIU, // int32 <= uint32 -> bool
        CmpLEU, // uint32 <= uint32 -> bool
        CmpLED, // double <= double -> bool
        CmpNLED, // !(double <= double) -> bool
        CmpGTD, // double > double -> bool
        CmpNGTD, // !(double > double) -> bool
        CmpGED, // double >= double -> bool
        CmpNGED, // !(double >= double) -> bool
#error finish
    };
    Operation operation;
};
}
}
}

#endif /* JAVASCRIPT_TASKLETS_VM_INTERPRETER_H_ */
