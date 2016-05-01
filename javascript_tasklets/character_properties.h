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

#ifndef JAVASCRIPT_TASKLETS_CHARACTER_PROPERTIES_H_
#define JAVASCRIPT_TASKLETS_CHARACTER_PROPERTIES_H_

#include <cstdint>

namespace javascript_tasklets
{
namespace character_properties
{
constexpr bool categoryLu(std::uint32_t codepoint) noexcept
{
#error finish
    return (codepoint >= 0x41 && codepoint <= 0x5A) || (codepoint >= 0xC0 && codepoint <= 0xD6)
           || (codepoint >= 0xD8 && codepoint <= 0xDE);
}
constexpr bool otherIdStart(std::uint32_t codepoint) noexcept
{
    return codepoint == 0x2118 || codepoint == 0x212E || codepoint == 0x309B || codepoint == 0x309C;
}
constexpr bool otherIdContinue(std::uint32_t codepoint) noexcept
{
    return codepoint == 0xB7 || codepoint == 0x387 || (codepoint >= 0x1369 && codepoint <= 0x1371)
           || codepoint == 0x19DA;
}
bool idStart(std::uint32_t codepoint) noexcept;
}
}

#endif /* JAVASCRIPT_TASKLETS_CHARACTER_PROPERTIES_H_ */
