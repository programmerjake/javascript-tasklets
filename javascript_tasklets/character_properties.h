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
constexpr bool categoryLu(std::uint32_t codePoint) noexcept
{
    return (codePoint >= 0x41U && codePoint <= 0x5AU) || (codePoint >= 0xC0U && codePoint <= 0xD6U)
           || (codePoint >= 0xD8U && codePoint <= 0xDEU)
           || (codePoint >= 0x100U && codePoint <= 0x136U && codePoint % 2 == 0)
           || (codePoint >= 0x139U && codePoint <= 0x147U && codePoint % 2 != 0)
           || (codePoint >= 0x14AU && codePoint <= 0x176U && codePoint % 2 == 0)
           || codePoint == 0x178U || codePoint == 0x179U
           || (codePoint >= 0x17BU && codePoint <= 0x17DU && codePoint % 2 != 0)
           || codePoint == 0x181U || codePoint == 0x182U || codePoint == 0x184U
           || codePoint == 0x186U || codePoint == 0x187U
           || (codePoint >= 0x189U && codePoint <= 0x18BU)
           || (codePoint >= 0x18EU && codePoint <= 0x191U) || codePoint == 0x193U
           || codePoint == 0x194U || (codePoint >= 0x196U && codePoint <= 0x198U)
           || codePoint == 0x19CU || codePoint == 0x19DU || codePoint == 0x19FU
           || codePoint == 0x1A0U
           || (codePoint >= 0x1A2U && codePoint <= 0x1A4U && codePoint % 2 == 0)
           || codePoint == 0x1A6U || codePoint == 0x1A7U || codePoint == 0x1A9U
           || codePoint == 0x1ACU || codePoint == 0x1AEU || codePoint == 0x1AFU
           || (codePoint >= 0x1B1U && codePoint <= 0x1B3U) || codePoint == 0x1B5U
           || codePoint == 0x1B7U || codePoint == 0x1B8U || codePoint == 0x1BCU
           || codePoint == 0x1C4U || codePoint == 0x1C7U || codePoint == 0x1CAU
           || (codePoint >= 0x1CDU && codePoint <= 0x1DBU && codePoint % 2 != 0)
           || (codePoint >= 0x1DEU && codePoint <= 0x1EEU && codePoint % 2 == 0)
           || codePoint == 0x1F1U || codePoint == 0x1F4U
           || (codePoint >= 0x1F6U && codePoint <= 0x1F8U)
           || (codePoint >= 0x1FAU && codePoint <= 0x232U && codePoint % 2 == 0)
           || codePoint == 0x23AU || codePoint == 0x23BU || codePoint == 0x23DU
           || codePoint == 0x23EU || codePoint == 0x241U
           || (codePoint >= 0x243U && codePoint <= 0x246U)
           || (codePoint >= 0x248U && codePoint <= 0x24EU && codePoint % 2 == 0)
           || (codePoint >= 0x370U && codePoint <= 0x372U && codePoint % 2 == 0)
           || codePoint == 0x376U || codePoint == 0x37FU || codePoint == 0x386U
           || (codePoint >= 0x388U && codePoint <= 0x38AU) || codePoint == 0x38CU
           || codePoint == 0x38EU || codePoint == 0x38FU
           || (codePoint >= 0x391U && codePoint <= 0x3A1U)
           || (codePoint >= 0x3A3U && codePoint <= 0x3ABU) || codePoint == 0x3CFU
           || (codePoint >= 0x3D2U && codePoint <= 0x3D4U)
           || (codePoint >= 0x3D8U && codePoint <= 0x3EEU && codePoint % 2 == 0)
           || codePoint == 0x3F4U || codePoint == 0x3F7U || codePoint == 0x3F9U
           || codePoint == 0x3FAU || (codePoint >= 0x3FDU && codePoint <= 0x42FU)
           || (codePoint >= 0x460U && codePoint <= 0x480U && codePoint % 2 == 0)
           || (codePoint >= 0x48AU && codePoint <= 0x4BEU && codePoint % 2 == 0)
           || codePoint == 0x4C0U || codePoint == 0x4C1U
           || (codePoint >= 0x4C3U && codePoint <= 0x4CDU && codePoint % 2 != 0)
           || (codePoint >= 0x4D0U && codePoint <= 0x52EU && codePoint % 2 == 0)
           || (codePoint >= 0x531U && codePoint <= 0x556U)
           || (codePoint >= 0x10A0U && codePoint <= 0x10C5U) || codePoint == 0x10C7U
           || codePoint == 0x10CDU || (codePoint >= 0x13A0U && codePoint <= 0x13F5U)
           || (codePoint >= 0x1E00U && codePoint <= 0x1E94U && codePoint % 2 == 0)
           || (codePoint >= 0x1E9EU && codePoint <= 0x1EFEU && codePoint % 2 == 0)
           || (codePoint >= 0x1F08U && codePoint <= 0x1F0FU)
           || (codePoint >= 0x1F18U && codePoint <= 0x1F1DU)
           || (codePoint >= 0x1F28U && codePoint <= 0x1F2FU)
           || (codePoint >= 0x1F38U && codePoint <= 0x1F3FU)
           || (codePoint >= 0x1F48U && codePoint <= 0x1F4DU)
           || (codePoint >= 0x1F59U && codePoint <= 0x1F5FU && codePoint % 2 != 0)
           || (codePoint >= 0x1F68U && codePoint <= 0x1F6FU)
           || (codePoint >= 0x1FB8U && codePoint <= 0x1FBBU)
           || (codePoint >= 0x1FC8U && codePoint <= 0x1FCBU)
           || (codePoint >= 0x1FD8U && codePoint <= 0x1FDBU)
           || (codePoint >= 0x1FE8U && codePoint <= 0x1FECU)
           || (codePoint >= 0x1FF8U && codePoint <= 0x1FFBU) || codePoint == 0x2102U
           || codePoint == 0x2107U || (codePoint >= 0x210BU && codePoint <= 0x210DU)
           || (codePoint >= 0x2110U && codePoint <= 0x2112U) || codePoint == 0x2115U
           || (codePoint >= 0x2119U && codePoint <= 0x211DU)
           || (codePoint >= 0x2124U && codePoint <= 0x2128U && codePoint % 2 == 0)
           || (codePoint >= 0x212AU && codePoint <= 0x212DU)
           || (codePoint >= 0x2130U && codePoint <= 0x2133U) || codePoint == 0x213EU
           || codePoint == 0x213FU || codePoint == 0x2145U || codePoint == 0x2183U
           || (codePoint >= 0x2C00U && codePoint <= 0x2C2EU) || codePoint == 0x2C60U
           || (codePoint >= 0x2C62U && codePoint <= 0x2C64U)
           || (codePoint >= 0x2C67U && codePoint <= 0x2C6BU && codePoint % 2 != 0)
           || (codePoint >= 0x2C6DU && codePoint <= 0x2C70U) || codePoint == 0x2C72U
           || codePoint == 0x2C75U || (codePoint >= 0x2C7EU && codePoint <= 0x2C80U)
           || (codePoint >= 0x2C82U && codePoint <= 0x2CE2U && codePoint % 2 == 0)
           || (codePoint >= 0x2CEBU && codePoint <= 0x2CEDU && codePoint % 2 != 0)
           || codePoint == 0x2CF2U
           || (codePoint >= 0xA640U && codePoint <= 0xA66CU && codePoint % 2 == 0)
           || (codePoint >= 0xA680U && codePoint <= 0xA69AU && codePoint % 2 == 0)
           || (codePoint >= 0xA722U && codePoint <= 0xA72EU && codePoint % 2 == 0)
           || (codePoint >= 0xA732U && codePoint <= 0xA76EU && codePoint % 2 == 0)
           || (codePoint >= 0xA779U && codePoint <= 0xA77BU && codePoint % 2 != 0)
           || codePoint == 0xA77DU || codePoint == 0xA77EU
           || (codePoint >= 0xA780U && codePoint <= 0xA786U && codePoint % 2 == 0)
           || (codePoint >= 0xA78BU && codePoint <= 0xA78DU && codePoint % 2 != 0)
           || (codePoint >= 0xA790U && codePoint <= 0xA792U && codePoint % 2 == 0)
           || (codePoint >= 0xA796U && codePoint <= 0xA7A8U && codePoint % 2 == 0)
           || (codePoint >= 0xA7AAU && codePoint <= 0xA7ADU)
           || (codePoint >= 0xA7B0U && codePoint <= 0xA7B4U) || codePoint == 0xA7B6U
           || (codePoint >= 0xFF21U && codePoint <= 0xFF3AU)
           || (codePoint >= 0x10400UL && codePoint <= 0x10427UL)
           || (codePoint >= 0x10C80UL && codePoint <= 0x10CB2UL)
           || (codePoint >= 0x118A0UL && codePoint <= 0x118BFUL)
           || (codePoint >= 0x1D400UL && codePoint <= 0x1D419UL)
           || (codePoint >= 0x1D434UL && codePoint <= 0x1D44DUL)
           || (codePoint >= 0x1D468UL && codePoint <= 0x1D481UL) || codePoint == 0x1D49CUL
           || codePoint == 0x1D49EUL || codePoint == 0x1D49FUL || codePoint == 0x1D4A2UL
           || codePoint == 0x1D4A5UL || codePoint == 0x1D4A6UL
           || (codePoint >= 0x1D4A9UL && codePoint <= 0x1D4ACUL)
           || (codePoint >= 0x1D4AEUL && codePoint <= 0x1D4B5UL)
           || (codePoint >= 0x1D4D0UL && codePoint <= 0x1D4E9UL) || codePoint == 0x1D504UL
           || codePoint == 0x1D505UL || (codePoint >= 0x1D507UL && codePoint <= 0x1D50AUL)
           || (codePoint >= 0x1D50DUL && codePoint <= 0x1D514UL)
           || (codePoint >= 0x1D516UL && codePoint <= 0x1D51CUL) || codePoint == 0x1D538UL
           || codePoint == 0x1D539UL || (codePoint >= 0x1D53BUL && codePoint <= 0x1D53EUL)
           || (codePoint >= 0x1D540UL && codePoint <= 0x1D544UL) || codePoint == 0x1D546UL
           || (codePoint >= 0x1D54AUL && codePoint <= 0x1D550UL)
           || (codePoint >= 0x1D56CUL && codePoint <= 0x1D585UL)
           || (codePoint >= 0x1D5A0UL && codePoint <= 0x1D5B9UL)
           || (codePoint >= 0x1D5D4UL && codePoint <= 0x1D5EDUL)
           || (codePoint >= 0x1D608UL && codePoint <= 0x1D621UL)
           || (codePoint >= 0x1D63CUL && codePoint <= 0x1D655UL)
           || (codePoint >= 0x1D670UL && codePoint <= 0x1D689UL)
           || (codePoint >= 0x1D6A8UL && codePoint <= 0x1D6C0UL)
           || (codePoint >= 0x1D6E2UL && codePoint <= 0x1D6FAUL)
           || (codePoint >= 0x1D71CUL && codePoint <= 0x1D734UL)
           || (codePoint >= 0x1D756UL && codePoint <= 0x1D76EUL)
           || (codePoint >= 0x1D790UL && codePoint <= 0x1D7A8UL) || codePoint == 0x1D7CAUL;
}
constexpr bool categoryLl(std::uint32_t codePoint) noexcept
{
    return (codePoint >= 0x61U && codePoint <= 0x7AU) || codePoint == 0xB5U
           || (codePoint >= 0xDFU && codePoint <= 0xF6U)
           || (codePoint >= 0xF8U && codePoint <= 0xFFU)
           || (codePoint >= 0x101U && codePoint <= 0x135U && codePoint % 2 != 0)
           || codePoint == 0x137U || codePoint == 0x138U
           || (codePoint >= 0x13AU && codePoint <= 0x146U && codePoint % 2 == 0)
           || codePoint == 0x148U || codePoint == 0x149U
           || (codePoint >= 0x14BU && codePoint <= 0x177U && codePoint % 2 != 0)
           || (codePoint >= 0x17AU && codePoint <= 0x17CU && codePoint % 2 == 0)
           || (codePoint >= 0x17EU && codePoint <= 0x180U)
           || (codePoint >= 0x183U && codePoint <= 0x185U && codePoint % 2 != 0)
           || codePoint == 0x188U || codePoint == 0x18CU || codePoint == 0x18DU
           || codePoint == 0x192U || codePoint == 0x195U
           || (codePoint >= 0x199U && codePoint <= 0x19BU) || codePoint == 0x19EU
           || (codePoint >= 0x1A1U && codePoint <= 0x1A5U && codePoint % 2 != 0)
           || codePoint == 0x1A8U || codePoint == 0x1AAU || codePoint == 0x1ABU
           || codePoint == 0x1ADU || codePoint == 0x1B0U
           || (codePoint >= 0x1B4U && codePoint <= 0x1B6U && codePoint % 2 == 0)
           || codePoint == 0x1B9U || codePoint == 0x1BAU
           || (codePoint >= 0x1BDU && codePoint <= 0x1BFU) || codePoint == 0x1C6U
           || codePoint == 0x1C9U
           || (codePoint >= 0x1CCU && codePoint <= 0x1DAU && codePoint % 2 == 0)
           || codePoint == 0x1DCU || codePoint == 0x1DDU
           || (codePoint >= 0x1DFU && codePoint <= 0x1EDU && codePoint % 2 != 0)
           || codePoint == 0x1EFU || codePoint == 0x1F0U
           || (codePoint >= 0x1F3U && codePoint <= 0x1F5U && codePoint % 2 != 0)
           || (codePoint >= 0x1F9U && codePoint <= 0x231U && codePoint % 2 != 0)
           || (codePoint >= 0x233U && codePoint <= 0x239U) || codePoint == 0x23CU
           || codePoint == 0x23FU || codePoint == 0x240U || codePoint == 0x242U
           || (codePoint >= 0x247U && codePoint <= 0x24DU && codePoint % 2 != 0)
           || (codePoint >= 0x24FU && codePoint <= 0x293U)
           || (codePoint >= 0x295U && codePoint <= 0x2AFU)
           || (codePoint >= 0x371U && codePoint <= 0x373U && codePoint % 2 != 0)
           || codePoint == 0x377U || (codePoint >= 0x37BU && codePoint <= 0x37DU)
           || codePoint == 0x390U || (codePoint >= 0x3ACU && codePoint <= 0x3CEU)
           || codePoint == 0x3D0U || codePoint == 0x3D1U
           || (codePoint >= 0x3D5U && codePoint <= 0x3D7U)
           || (codePoint >= 0x3D9U && codePoint <= 0x3EDU && codePoint % 2 != 0)
           || (codePoint >= 0x3EFU && codePoint <= 0x3F3U) || codePoint == 0x3F5U
           || codePoint == 0x3F8U || codePoint == 0x3FBU || codePoint == 0x3FCU
           || (codePoint >= 0x430U && codePoint <= 0x45FU)
           || (codePoint >= 0x461U && codePoint <= 0x481U && codePoint % 2 != 0)
           || (codePoint >= 0x48BU && codePoint <= 0x4BFU && codePoint % 2 != 0)
           || (codePoint >= 0x4C2U && codePoint <= 0x4CCU && codePoint % 2 == 0)
           || codePoint == 0x4CEU || codePoint == 0x4CFU
           || (codePoint >= 0x4D1U && codePoint <= 0x52FU && codePoint % 2 != 0)
           || (codePoint >= 0x561U && codePoint <= 0x587U)
           || (codePoint >= 0x13F8U && codePoint <= 0x13FDU)
           || (codePoint >= 0x1D00U && codePoint <= 0x1D2BU)
           || (codePoint >= 0x1D6BU && codePoint <= 0x1D77U)
           || (codePoint >= 0x1D79U && codePoint <= 0x1D9AU)
           || (codePoint >= 0x1E01U && codePoint <= 0x1E93U && codePoint % 2 != 0)
           || (codePoint >= 0x1E95U && codePoint <= 0x1E9DU)
           || (codePoint >= 0x1E9FU && codePoint <= 0x1EFDU && codePoint % 2 != 0)
           || (codePoint >= 0x1EFFU && codePoint <= 0x1F07U)
           || (codePoint >= 0x1F10U && codePoint <= 0x1F15U)
           || (codePoint >= 0x1F20U && codePoint <= 0x1F27U)
           || (codePoint >= 0x1F30U && codePoint <= 0x1F37U)
           || (codePoint >= 0x1F40U && codePoint <= 0x1F45U)
           || (codePoint >= 0x1F50U && codePoint <= 0x1F57U)
           || (codePoint >= 0x1F60U && codePoint <= 0x1F67U)
           || (codePoint >= 0x1F70U && codePoint <= 0x1F7DU)
           || (codePoint >= 0x1F80U && codePoint <= 0x1F87U)
           || (codePoint >= 0x1F90U && codePoint <= 0x1F97U)
           || (codePoint >= 0x1FA0U && codePoint <= 0x1FA7U)
           || (codePoint >= 0x1FB0U && codePoint <= 0x1FB4U) || codePoint == 0x1FB6U
           || codePoint == 0x1FB7U || codePoint == 0x1FBEU
           || (codePoint >= 0x1FC2U && codePoint <= 0x1FC4U) || codePoint == 0x1FC6U
           || codePoint == 0x1FC7U || (codePoint >= 0x1FD0U && codePoint <= 0x1FD3U)
           || codePoint == 0x1FD6U || codePoint == 0x1FD7U
           || (codePoint >= 0x1FE0U && codePoint <= 0x1FE7U)
           || (codePoint >= 0x1FF2U && codePoint <= 0x1FF4U) || codePoint == 0x1FF6U
           || codePoint == 0x1FF7U || codePoint == 0x210AU || codePoint == 0x210EU
           || codePoint == 0x210FU || codePoint == 0x2113U || codePoint == 0x212FU
           || codePoint == 0x2134U || codePoint == 0x2139U || codePoint == 0x213CU
           || codePoint == 0x213DU || (codePoint >= 0x2146U && codePoint <= 0x2149U)
           || codePoint == 0x214EU || codePoint == 0x2184U
           || (codePoint >= 0x2C30U && codePoint <= 0x2C5EU) || codePoint == 0x2C61U
           || codePoint == 0x2C65U || codePoint == 0x2C66U
           || (codePoint >= 0x2C68U && codePoint <= 0x2C6CU && codePoint % 2 == 0)
           || codePoint == 0x2C71U || codePoint == 0x2C73U || codePoint == 0x2C74U
           || (codePoint >= 0x2C76U && codePoint <= 0x2C7BU)
           || (codePoint >= 0x2C81U && codePoint <= 0x2CE1U && codePoint % 2 != 0)
           || codePoint == 0x2CE3U || codePoint == 0x2CE4U
           || (codePoint >= 0x2CECU && codePoint <= 0x2CEEU && codePoint % 2 == 0)
           || codePoint == 0x2CF3U || (codePoint >= 0x2D00U && codePoint <= 0x2D25U)
           || codePoint == 0x2D27U || codePoint == 0x2D2DU
           || (codePoint >= 0xA641U && codePoint <= 0xA66DU && codePoint % 2 != 0)
           || (codePoint >= 0xA681U && codePoint <= 0xA69BU && codePoint % 2 != 0)
           || (codePoint >= 0xA723U && codePoint <= 0xA72DU && codePoint % 2 != 0)
           || (codePoint >= 0xA72FU && codePoint <= 0xA731U)
           || (codePoint >= 0xA733U && codePoint <= 0xA76FU && codePoint % 2 != 0)
           || (codePoint >= 0xA771U && codePoint <= 0xA778U)
           || (codePoint >= 0xA77AU && codePoint <= 0xA77CU && codePoint % 2 == 0)
           || (codePoint >= 0xA77FU && codePoint <= 0xA787U && codePoint % 2 != 0)
           || (codePoint >= 0xA78CU && codePoint <= 0xA78EU && codePoint % 2 == 0)
           || codePoint == 0xA791U || (codePoint >= 0xA793U && codePoint <= 0xA795U)
           || (codePoint >= 0xA797U && codePoint <= 0xA7A9U && codePoint % 2 != 0)
           || (codePoint >= 0xA7B5U && codePoint <= 0xA7B7U && codePoint % 2 != 0)
           || codePoint == 0xA7FAU || (codePoint >= 0xAB30U && codePoint <= 0xAB5AU)
           || (codePoint >= 0xAB60U && codePoint <= 0xAB65U)
           || (codePoint >= 0xAB70U && codePoint <= 0xABBFU)
           || (codePoint >= 0xFB00U && codePoint <= 0xFB06U)
           || (codePoint >= 0xFB13U && codePoint <= 0xFB17U)
           || (codePoint >= 0xFF41U && codePoint <= 0xFF5AU)
           || (codePoint >= 0x10428UL && codePoint <= 0x1044FUL)
           || (codePoint >= 0x10CC0UL && codePoint <= 0x10CF2UL)
           || (codePoint >= 0x118C0UL && codePoint <= 0x118DFUL)
           || (codePoint >= 0x1D41AUL && codePoint <= 0x1D433UL)
           || (codePoint >= 0x1D44EUL && codePoint <= 0x1D454UL)
           || (codePoint >= 0x1D456UL && codePoint <= 0x1D467UL)
           || (codePoint >= 0x1D482UL && codePoint <= 0x1D49BUL)
           || (codePoint >= 0x1D4B6UL && codePoint <= 0x1D4B9UL) || codePoint == 0x1D4BBUL
           || (codePoint >= 0x1D4BDUL && codePoint <= 0x1D4C3UL)
           || (codePoint >= 0x1D4C5UL && codePoint <= 0x1D4CFUL)
           || (codePoint >= 0x1D4EAUL && codePoint <= 0x1D503UL)
           || (codePoint >= 0x1D51EUL && codePoint <= 0x1D537UL)
           || (codePoint >= 0x1D552UL && codePoint <= 0x1D56BUL)
           || (codePoint >= 0x1D586UL && codePoint <= 0x1D59FUL)
           || (codePoint >= 0x1D5BAUL && codePoint <= 0x1D5D3UL)
           || (codePoint >= 0x1D5EEUL && codePoint <= 0x1D607UL)
           || (codePoint >= 0x1D622UL && codePoint <= 0x1D63BUL)
           || (codePoint >= 0x1D656UL && codePoint <= 0x1D66FUL)
           || (codePoint >= 0x1D68AUL && codePoint <= 0x1D6A5UL)
           || (codePoint >= 0x1D6C2UL && codePoint <= 0x1D6DAUL)
           || (codePoint >= 0x1D6DCUL && codePoint <= 0x1D6E1UL)
           || (codePoint >= 0x1D6FCUL && codePoint <= 0x1D714UL)
           || (codePoint >= 0x1D716UL && codePoint <= 0x1D71BUL)
           || (codePoint >= 0x1D736UL && codePoint <= 0x1D74EUL)
           || (codePoint >= 0x1D750UL && codePoint <= 0x1D755UL)
           || (codePoint >= 0x1D770UL && codePoint <= 0x1D788UL)
           || (codePoint >= 0x1D78AUL && codePoint <= 0x1D78FUL)
           || (codePoint >= 0x1D7AAUL && codePoint <= 0x1D7C2UL)
           || (codePoint >= 0x1D7C4UL && codePoint <= 0x1D7C9UL) || codePoint == 0x1D7CBUL;
}
constexpr bool categoryLt(std::uint32_t codePoint) noexcept
{
    return codePoint == 0x1C5U || codePoint == 0x1C8U || codePoint == 0x1CBU || codePoint == 0x1F2U
           || (codePoint >= 0x1F88U && codePoint <= 0x1F8FU)
           || (codePoint >= 0x1F98U && codePoint <= 0x1F9FU)
           || (codePoint >= 0x1FA8U && codePoint <= 0x1FAFU) || codePoint == 0x1FBCU
           || codePoint == 0x1FCCU || codePoint == 0x1FFCU;
}
constexpr bool categoryLm(std::uint32_t codePoint) noexcept
{
    return (codePoint >= 0x2B0U && codePoint <= 0x2C1U)
           || (codePoint >= 0x2C6U && codePoint <= 0x2D1U)
           || (codePoint >= 0x2E0U && codePoint <= 0x2E4U)
           || (codePoint >= 0x2ECU && codePoint <= 0x2EEU && codePoint % 2 == 0)
           || codePoint == 0x374U || codePoint == 0x37AU || codePoint == 0x559U
           || codePoint == 0x640U || codePoint == 0x6E5U || codePoint == 0x6E6U
           || codePoint == 0x7F4U || codePoint == 0x7F5U || codePoint == 0x7FAU
           || codePoint == 0x81AU || codePoint == 0x824U || codePoint == 0x828U
           || codePoint == 0x971U || codePoint == 0xE46U || codePoint == 0xEC6U
           || codePoint == 0x10FCU || codePoint == 0x17D7U || codePoint == 0x1843U
           || codePoint == 0x1AA7U || (codePoint >= 0x1C78U && codePoint <= 0x1C7DU)
           || (codePoint >= 0x1D2CU && codePoint <= 0x1D6AU) || codePoint == 0x1D78U
           || (codePoint >= 0x1D9BU && codePoint <= 0x1DBFU) || codePoint == 0x2071U
           || codePoint == 0x207FU || (codePoint >= 0x2090U && codePoint <= 0x209CU)
           || codePoint == 0x2C7CU || codePoint == 0x2C7DU || codePoint == 0x2D6FU
           || codePoint == 0x2E2FU || codePoint == 0x3005U
           || (codePoint >= 0x3031U && codePoint <= 0x3035U) || codePoint == 0x303BU
           || codePoint == 0x309DU || codePoint == 0x309EU
           || (codePoint >= 0x30FCU && codePoint <= 0x30FEU) || codePoint == 0xA015U
           || (codePoint >= 0xA4F8U && codePoint <= 0xA4FDU) || codePoint == 0xA60CU
           || codePoint == 0xA67FU || codePoint == 0xA69CU || codePoint == 0xA69DU
           || (codePoint >= 0xA717U && codePoint <= 0xA71FU) || codePoint == 0xA770U
           || codePoint == 0xA788U || codePoint == 0xA7F8U || codePoint == 0xA7F9U
           || codePoint == 0xA9CFU || codePoint == 0xA9E6U || codePoint == 0xAA70U
           || codePoint == 0xAADDU || codePoint == 0xAAF3U || codePoint == 0xAAF4U
           || (codePoint >= 0xAB5CU && codePoint <= 0xAB5FU) || codePoint == 0xFF70U
           || codePoint == 0xFF9EU || codePoint == 0xFF9FU
           || (codePoint >= 0x16B40UL && codePoint <= 0x16B43UL)
           || (codePoint >= 0x16F93UL && codePoint <= 0x16F9FUL);
}
constexpr bool categoryLo(std::uint32_t codePoint) noexcept
{
    return codePoint == 0xAAU || codePoint == 0xBAU || codePoint == 0x1BBU
           || (codePoint >= 0x1C0U && codePoint <= 0x1C3U) || codePoint == 0x294U
           || (codePoint >= 0x5D0U && codePoint <= 0x5EAU)
           || (codePoint >= 0x5F0U && codePoint <= 0x5F2U)
           || (codePoint >= 0x620U && codePoint <= 0x63FU)
           || (codePoint >= 0x641U && codePoint <= 0x64AU) || codePoint == 0x66EU
           || codePoint == 0x66FU || (codePoint >= 0x671U && codePoint <= 0x6D3U)
           || codePoint == 0x6D5U || codePoint == 0x6EEU || codePoint == 0x6EFU
           || (codePoint >= 0x6FAU && codePoint <= 0x6FCU) || codePoint == 0x6FFU
           || codePoint == 0x710U || (codePoint >= 0x712U && codePoint <= 0x72FU)
           || (codePoint >= 0x74DU && codePoint <= 0x7A5U) || codePoint == 0x7B1U
           || (codePoint >= 0x7CAU && codePoint <= 0x7EAU)
           || (codePoint >= 0x800U && codePoint <= 0x815U)
           || (codePoint >= 0x840U && codePoint <= 0x858U)
           || (codePoint >= 0x8A0U && codePoint <= 0x8B4U)
           || (codePoint >= 0x904U && codePoint <= 0x939U) || codePoint == 0x93DU
           || codePoint == 0x950U || (codePoint >= 0x958U && codePoint <= 0x961U)
           || (codePoint >= 0x972U && codePoint <= 0x980U)
           || (codePoint >= 0x985U && codePoint <= 0x98CU) || codePoint == 0x98FU
           || codePoint == 0x990U || (codePoint >= 0x993U && codePoint <= 0x9A8U)
           || (codePoint >= 0x9AAU && codePoint <= 0x9B0U) || codePoint == 0x9B2U
           || (codePoint >= 0x9B6U && codePoint <= 0x9B9U) || codePoint == 0x9BDU
           || codePoint == 0x9CEU || codePoint == 0x9DCU || codePoint == 0x9DDU
           || (codePoint >= 0x9DFU && codePoint <= 0x9E1U) || codePoint == 0x9F0U
           || codePoint == 0x9F1U || (codePoint >= 0xA05U && codePoint <= 0xA0AU)
           || codePoint == 0xA0FU || codePoint == 0xA10U
           || (codePoint >= 0xA13U && codePoint <= 0xA28U)
           || (codePoint >= 0xA2AU && codePoint <= 0xA30U) || codePoint == 0xA32U
           || codePoint == 0xA33U || codePoint == 0xA35U || codePoint == 0xA36U
           || codePoint == 0xA38U || codePoint == 0xA39U
           || (codePoint >= 0xA59U && codePoint <= 0xA5CU) || codePoint == 0xA5EU
           || (codePoint >= 0xA72U && codePoint <= 0xA74U)
           || (codePoint >= 0xA85U && codePoint <= 0xA8DU)
           || (codePoint >= 0xA8FU && codePoint <= 0xA91U)
           || (codePoint >= 0xA93U && codePoint <= 0xAA8U)
           || (codePoint >= 0xAAAU && codePoint <= 0xAB0U) || codePoint == 0xAB2U
           || codePoint == 0xAB3U || (codePoint >= 0xAB5U && codePoint <= 0xAB9U)
           || codePoint == 0xABDU || codePoint == 0xAD0U || codePoint == 0xAE0U
           || codePoint == 0xAE1U || codePoint == 0xAF9U
           || (codePoint >= 0xB05U && codePoint <= 0xB0CU) || codePoint == 0xB0FU
           || codePoint == 0xB10U || (codePoint >= 0xB13U && codePoint <= 0xB28U)
           || (codePoint >= 0xB2AU && codePoint <= 0xB30U) || codePoint == 0xB32U
           || codePoint == 0xB33U || (codePoint >= 0xB35U && codePoint <= 0xB39U)
           || codePoint == 0xB3DU || codePoint == 0xB5CU || codePoint == 0xB5DU
           || (codePoint >= 0xB5FU && codePoint <= 0xB61U) || codePoint == 0xB71U
           || codePoint == 0xB83U || (codePoint >= 0xB85U && codePoint <= 0xB8AU)
           || (codePoint >= 0xB8EU && codePoint <= 0xB90U)
           || (codePoint >= 0xB92U && codePoint <= 0xB95U) || codePoint == 0xB99U
           || codePoint == 0xB9AU || codePoint == 0xB9CU || codePoint == 0xB9EU
           || codePoint == 0xB9FU || codePoint == 0xBA3U || codePoint == 0xBA4U
           || (codePoint >= 0xBA8U && codePoint <= 0xBAAU)
           || (codePoint >= 0xBAEU && codePoint <= 0xBB9U) || codePoint == 0xBD0U
           || (codePoint >= 0xC05U && codePoint <= 0xC0CU)
           || (codePoint >= 0xC0EU && codePoint <= 0xC10U)
           || (codePoint >= 0xC12U && codePoint <= 0xC28U)
           || (codePoint >= 0xC2AU && codePoint <= 0xC39U) || codePoint == 0xC3DU
           || (codePoint >= 0xC58U && codePoint <= 0xC5AU) || codePoint == 0xC60U
           || codePoint == 0xC61U || (codePoint >= 0xC85U && codePoint <= 0xC8CU)
           || (codePoint >= 0xC8EU && codePoint <= 0xC90U)
           || (codePoint >= 0xC92U && codePoint <= 0xCA8U)
           || (codePoint >= 0xCAAU && codePoint <= 0xCB3U)
           || (codePoint >= 0xCB5U && codePoint <= 0xCB9U) || codePoint == 0xCBDU
           || codePoint == 0xCDEU || codePoint == 0xCE0U || codePoint == 0xCE1U
           || codePoint == 0xCF1U || codePoint == 0xCF2U
           || (codePoint >= 0xD05U && codePoint <= 0xD0CU)
           || (codePoint >= 0xD0EU && codePoint <= 0xD10U)
           || (codePoint >= 0xD12U && codePoint <= 0xD3AU) || codePoint == 0xD3DU
           || codePoint == 0xD4EU || (codePoint >= 0xD5FU && codePoint <= 0xD61U)
           || (codePoint >= 0xD7AU && codePoint <= 0xD7FU)
           || (codePoint >= 0xD85U && codePoint <= 0xD96U)
           || (codePoint >= 0xD9AU && codePoint <= 0xDB1U)
           || (codePoint >= 0xDB3U && codePoint <= 0xDBBU) || codePoint == 0xDBDU
           || (codePoint >= 0xDC0U && codePoint <= 0xDC6U)
           || (codePoint >= 0xE01U && codePoint <= 0xE30U) || codePoint == 0xE32U
           || codePoint == 0xE33U || (codePoint >= 0xE40U && codePoint <= 0xE45U)
           || codePoint == 0xE81U || codePoint == 0xE82U || codePoint == 0xE84U
           || codePoint == 0xE87U || codePoint == 0xE88U || codePoint == 0xE8AU
           || codePoint == 0xE8DU || (codePoint >= 0xE94U && codePoint <= 0xE97U)
           || (codePoint >= 0xE99U && codePoint <= 0xE9FU)
           || (codePoint >= 0xEA1U && codePoint <= 0xEA3U)
           || (codePoint >= 0xEA5U && codePoint <= 0xEA7U && codePoint % 2 != 0)
           || codePoint == 0xEAAU || codePoint == 0xEABU
           || (codePoint >= 0xEADU && codePoint <= 0xEB0U) || codePoint == 0xEB2U
           || codePoint == 0xEB3U || codePoint == 0xEBDU
           || (codePoint >= 0xEC0U && codePoint <= 0xEC4U)
           || (codePoint >= 0xEDCU && codePoint <= 0xEDFU) || codePoint == 0xF00U
           || (codePoint >= 0xF40U && codePoint <= 0xF47U)
           || (codePoint >= 0xF49U && codePoint <= 0xF6CU)
           || (codePoint >= 0xF88U && codePoint <= 0xF8CU)
           || (codePoint >= 0x1000U && codePoint <= 0x102AU) || codePoint == 0x103FU
           || (codePoint >= 0x1050U && codePoint <= 0x1055U)
           || (codePoint >= 0x105AU && codePoint <= 0x105DU) || codePoint == 0x1061U
           || codePoint == 0x1065U || codePoint == 0x1066U
           || (codePoint >= 0x106EU && codePoint <= 0x1070U)
           || (codePoint >= 0x1075U && codePoint <= 0x1081U) || codePoint == 0x108EU
           || (codePoint >= 0x10D0U && codePoint <= 0x10FAU)
           || (codePoint >= 0x10FDU && codePoint <= 0x1248U)
           || (codePoint >= 0x124AU && codePoint <= 0x124DU)
           || (codePoint >= 0x1250U && codePoint <= 0x1256U) || codePoint == 0x1258U
           || (codePoint >= 0x125AU && codePoint <= 0x125DU)
           || (codePoint >= 0x1260U && codePoint <= 0x1288U)
           || (codePoint >= 0x128AU && codePoint <= 0x128DU)
           || (codePoint >= 0x1290U && codePoint <= 0x12B0U)
           || (codePoint >= 0x12B2U && codePoint <= 0x12B5U)
           || (codePoint >= 0x12B8U && codePoint <= 0x12BEU) || codePoint == 0x12C0U
           || (codePoint >= 0x12C2U && codePoint <= 0x12C5U)
           || (codePoint >= 0x12C8U && codePoint <= 0x12D6U)
           || (codePoint >= 0x12D8U && codePoint <= 0x1310U)
           || (codePoint >= 0x1312U && codePoint <= 0x1315U)
           || (codePoint >= 0x1318U && codePoint <= 0x135AU)
           || (codePoint >= 0x1380U && codePoint <= 0x138FU)
           || (codePoint >= 0x1401U && codePoint <= 0x166CU)
           || (codePoint >= 0x166FU && codePoint <= 0x167FU)
           || (codePoint >= 0x1681U && codePoint <= 0x169AU)
           || (codePoint >= 0x16A0U && codePoint <= 0x16EAU)
           || (codePoint >= 0x16F1U && codePoint <= 0x16F8U)
           || (codePoint >= 0x1700U && codePoint <= 0x170CU)
           || (codePoint >= 0x170EU && codePoint <= 0x1711U)
           || (codePoint >= 0x1720U && codePoint <= 0x1731U)
           || (codePoint >= 0x1740U && codePoint <= 0x1751U)
           || (codePoint >= 0x1760U && codePoint <= 0x176CU)
           || (codePoint >= 0x176EU && codePoint <= 0x1770U)
           || (codePoint >= 0x1780U && codePoint <= 0x17B3U) || codePoint == 0x17DCU
           || (codePoint >= 0x1820U && codePoint <= 0x1842U)
           || (codePoint >= 0x1844U && codePoint <= 0x1877U)
           || (codePoint >= 0x1880U && codePoint <= 0x18A8U) || codePoint == 0x18AAU
           || (codePoint >= 0x18B0U && codePoint <= 0x18F5U)
           || (codePoint >= 0x1900U && codePoint <= 0x191EU)
           || (codePoint >= 0x1950U && codePoint <= 0x196DU)
           || (codePoint >= 0x1970U && codePoint <= 0x1974U)
           || (codePoint >= 0x1980U && codePoint <= 0x19ABU)
           || (codePoint >= 0x19B0U && codePoint <= 0x19C9U)
           || (codePoint >= 0x1A00U && codePoint <= 0x1A16U)
           || (codePoint >= 0x1A20U && codePoint <= 0x1A54U)
           || (codePoint >= 0x1B05U && codePoint <= 0x1B33U)
           || (codePoint >= 0x1B45U && codePoint <= 0x1B4BU)
           || (codePoint >= 0x1B83U && codePoint <= 0x1BA0U) || codePoint == 0x1BAEU
           || codePoint == 0x1BAFU || (codePoint >= 0x1BBAU && codePoint <= 0x1BE5U)
           || (codePoint >= 0x1C00U && codePoint <= 0x1C23U)
           || (codePoint >= 0x1C4DU && codePoint <= 0x1C4FU)
           || (codePoint >= 0x1C5AU && codePoint <= 0x1C77U)
           || (codePoint >= 0x1CE9U && codePoint <= 0x1CECU)
           || (codePoint >= 0x1CEEU && codePoint <= 0x1CF1U) || codePoint == 0x1CF5U
           || codePoint == 0x1CF6U || (codePoint >= 0x2135U && codePoint <= 0x2138U)
           || (codePoint >= 0x2D30U && codePoint <= 0x2D67U)
           || (codePoint >= 0x2D80U && codePoint <= 0x2D96U)
           || (codePoint >= 0x2DA0U && codePoint <= 0x2DA6U)
           || (codePoint >= 0x2DA8U && codePoint <= 0x2DAEU)
           || (codePoint >= 0x2DB0U && codePoint <= 0x2DB6U)
           || (codePoint >= 0x2DB8U && codePoint <= 0x2DBEU)
           || (codePoint >= 0x2DC0U && codePoint <= 0x2DC6U)
           || (codePoint >= 0x2DC8U && codePoint <= 0x2DCEU)
           || (codePoint >= 0x2DD0U && codePoint <= 0x2DD6U)
           || (codePoint >= 0x2DD8U && codePoint <= 0x2DDEU) || codePoint == 0x3006U
           || codePoint == 0x303CU || (codePoint >= 0x3041U && codePoint <= 0x3096U)
           || codePoint == 0x309FU || (codePoint >= 0x30A1U && codePoint <= 0x30FAU)
           || codePoint == 0x30FFU || (codePoint >= 0x3105U && codePoint <= 0x312DU)
           || (codePoint >= 0x3131U && codePoint <= 0x318EU)
           || (codePoint >= 0x31A0U && codePoint <= 0x31BAU)
           || (codePoint >= 0x31F0U && codePoint <= 0x31FFU)
           || (codePoint >= 0x3400U && codePoint <= 0x4DB5U)
           || (codePoint >= 0x4E00U && codePoint <= 0x9FD5U)
           || (codePoint >= 0xA000U && codePoint <= 0xA014U)
           || (codePoint >= 0xA016U && codePoint <= 0xA48CU)
           || (codePoint >= 0xA4D0U && codePoint <= 0xA4F7U)
           || (codePoint >= 0xA500U && codePoint <= 0xA60BU)
           || (codePoint >= 0xA610U && codePoint <= 0xA61FU) || codePoint == 0xA62AU
           || codePoint == 0xA62BU || codePoint == 0xA66EU
           || (codePoint >= 0xA6A0U && codePoint <= 0xA6E5U) || codePoint == 0xA78FU
           || codePoint == 0xA7F7U || (codePoint >= 0xA7FBU && codePoint <= 0xA801U)
           || (codePoint >= 0xA803U && codePoint <= 0xA805U)
           || (codePoint >= 0xA807U && codePoint <= 0xA80AU)
           || (codePoint >= 0xA80CU && codePoint <= 0xA822U)
           || (codePoint >= 0xA840U && codePoint <= 0xA873U)
           || (codePoint >= 0xA882U && codePoint <= 0xA8B3U)
           || (codePoint >= 0xA8F2U && codePoint <= 0xA8F7U)
           || (codePoint >= 0xA8FBU && codePoint <= 0xA8FDU && codePoint % 2 != 0)
           || (codePoint >= 0xA90AU && codePoint <= 0xA925U)
           || (codePoint >= 0xA930U && codePoint <= 0xA946U)
           || (codePoint >= 0xA960U && codePoint <= 0xA97CU)
           || (codePoint >= 0xA984U && codePoint <= 0xA9B2U)
           || (codePoint >= 0xA9E0U && codePoint <= 0xA9E4U)
           || (codePoint >= 0xA9E7U && codePoint <= 0xA9EFU)
           || (codePoint >= 0xA9FAU && codePoint <= 0xA9FEU)
           || (codePoint >= 0xAA00U && codePoint <= 0xAA28U)
           || (codePoint >= 0xAA40U && codePoint <= 0xAA42U)
           || (codePoint >= 0xAA44U && codePoint <= 0xAA4BU)
           || (codePoint >= 0xAA60U && codePoint <= 0xAA6FU)
           || (codePoint >= 0xAA71U && codePoint <= 0xAA76U) || codePoint == 0xAA7AU
           || (codePoint >= 0xAA7EU && codePoint <= 0xAAAFU) || codePoint == 0xAAB1U
           || codePoint == 0xAAB5U || codePoint == 0xAAB6U
           || (codePoint >= 0xAAB9U && codePoint <= 0xAABDU)
           || (codePoint >= 0xAAC0U && codePoint <= 0xAAC2U && codePoint % 2 == 0)
           || codePoint == 0xAADBU || codePoint == 0xAADCU
           || (codePoint >= 0xAAE0U && codePoint <= 0xAAEAU) || codePoint == 0xAAF2U
           || (codePoint >= 0xAB01U && codePoint <= 0xAB06U)
           || (codePoint >= 0xAB09U && codePoint <= 0xAB0EU)
           || (codePoint >= 0xAB11U && codePoint <= 0xAB16U)
           || (codePoint >= 0xAB20U && codePoint <= 0xAB26U)
           || (codePoint >= 0xAB28U && codePoint <= 0xAB2EU)
           || (codePoint >= 0xABC0U && codePoint <= 0xABE2U)
           || (codePoint >= 0xAC00U && codePoint <= 0xD7A3U)
           || (codePoint >= 0xD7B0U && codePoint <= 0xD7C6U)
           || (codePoint >= 0xD7CBU && codePoint <= 0xD7FBU)
           || (codePoint >= 0xF900U && codePoint <= 0xFA6DU)
           || (codePoint >= 0xFA70U && codePoint <= 0xFAD9U) || codePoint == 0xFB1DU
           || (codePoint >= 0xFB1FU && codePoint <= 0xFB28U)
           || (codePoint >= 0xFB2AU && codePoint <= 0xFB36U)
           || (codePoint >= 0xFB38U && codePoint <= 0xFB3CU) || codePoint == 0xFB3EU
           || codePoint == 0xFB40U || codePoint == 0xFB41U || codePoint == 0xFB43U
           || codePoint == 0xFB44U || (codePoint >= 0xFB46U && codePoint <= 0xFBB1U)
           || (codePoint >= 0xFBD3U && codePoint <= 0xFD3DU)
           || (codePoint >= 0xFD50U && codePoint <= 0xFD8FU)
           || (codePoint >= 0xFD92U && codePoint <= 0xFDC7U)
           || (codePoint >= 0xFDF0U && codePoint <= 0xFDFBU)
           || (codePoint >= 0xFE70U && codePoint <= 0xFE74U)
           || (codePoint >= 0xFE76U && codePoint <= 0xFEFCU)
           || (codePoint >= 0xFF66U && codePoint <= 0xFF6FU)
           || (codePoint >= 0xFF71U && codePoint <= 0xFF9DU)
           || (codePoint >= 0xFFA0U && codePoint <= 0xFFBEU)
           || (codePoint >= 0xFFC2U && codePoint <= 0xFFC7U)
           || (codePoint >= 0xFFCAU && codePoint <= 0xFFCFU)
           || (codePoint >= 0xFFD2U && codePoint <= 0xFFD7U)
           || (codePoint >= 0xFFDAU && codePoint <= 0xFFDCU)
           || (codePoint >= 0x10000UL && codePoint <= 0x1000BUL)
           || (codePoint >= 0x1000DUL && codePoint <= 0x10026UL)
           || (codePoint >= 0x10028UL && codePoint <= 0x1003AUL) || codePoint == 0x1003CUL
           || codePoint == 0x1003DUL || (codePoint >= 0x1003FUL && codePoint <= 0x1004DUL)
           || (codePoint >= 0x10050UL && codePoint <= 0x1005DUL)
           || (codePoint >= 0x10080UL && codePoint <= 0x100FAUL)
           || (codePoint >= 0x10280UL && codePoint <= 0x1029CUL)
           || (codePoint >= 0x102A0UL && codePoint <= 0x102D0UL)
           || (codePoint >= 0x10300UL && codePoint <= 0x1031FUL)
           || (codePoint >= 0x10330UL && codePoint <= 0x10340UL)
           || (codePoint >= 0x10342UL && codePoint <= 0x10349UL)
           || (codePoint >= 0x10350UL && codePoint <= 0x10375UL)
           || (codePoint >= 0x10380UL && codePoint <= 0x1039DUL)
           || (codePoint >= 0x103A0UL && codePoint <= 0x103C3UL)
           || (codePoint >= 0x103C8UL && codePoint <= 0x103CFUL)
           || (codePoint >= 0x10450UL && codePoint <= 0x1049DUL)
           || (codePoint >= 0x10500UL && codePoint <= 0x10527UL)
           || (codePoint >= 0x10530UL && codePoint <= 0x10563UL)
           || (codePoint >= 0x10600UL && codePoint <= 0x10736UL)
           || (codePoint >= 0x10740UL && codePoint <= 0x10755UL)
           || (codePoint >= 0x10760UL && codePoint <= 0x10767UL)
           || (codePoint >= 0x10800UL && codePoint <= 0x10805UL) || codePoint == 0x10808UL
           || (codePoint >= 0x1080AUL && codePoint <= 0x10835UL) || codePoint == 0x10837UL
           || codePoint == 0x10838UL || codePoint == 0x1083CUL
           || (codePoint >= 0x1083FUL && codePoint <= 0x10855UL)
           || (codePoint >= 0x10860UL && codePoint <= 0x10876UL)
           || (codePoint >= 0x10880UL && codePoint <= 0x1089EUL)
           || (codePoint >= 0x108E0UL && codePoint <= 0x108F2UL) || codePoint == 0x108F4UL
           || codePoint == 0x108F5UL || (codePoint >= 0x10900UL && codePoint <= 0x10915UL)
           || (codePoint >= 0x10920UL && codePoint <= 0x10939UL)
           || (codePoint >= 0x10980UL && codePoint <= 0x109B7UL) || codePoint == 0x109BEUL
           || codePoint == 0x109BFUL || codePoint == 0x10A00UL
           || (codePoint >= 0x10A10UL && codePoint <= 0x10A13UL)
           || (codePoint >= 0x10A15UL && codePoint <= 0x10A17UL)
           || (codePoint >= 0x10A19UL && codePoint <= 0x10A33UL)
           || (codePoint >= 0x10A60UL && codePoint <= 0x10A7CUL)
           || (codePoint >= 0x10A80UL && codePoint <= 0x10A9CUL)
           || (codePoint >= 0x10AC0UL && codePoint <= 0x10AC7UL)
           || (codePoint >= 0x10AC9UL && codePoint <= 0x10AE4UL)
           || (codePoint >= 0x10B00UL && codePoint <= 0x10B35UL)
           || (codePoint >= 0x10B40UL && codePoint <= 0x10B55UL)
           || (codePoint >= 0x10B60UL && codePoint <= 0x10B72UL)
           || (codePoint >= 0x10B80UL && codePoint <= 0x10B91UL)
           || (codePoint >= 0x10C00UL && codePoint <= 0x10C48UL)
           || (codePoint >= 0x11003UL && codePoint <= 0x11037UL)
           || (codePoint >= 0x11083UL && codePoint <= 0x110AFUL)
           || (codePoint >= 0x110D0UL && codePoint <= 0x110E8UL)
           || (codePoint >= 0x11103UL && codePoint <= 0x11126UL)
           || (codePoint >= 0x11150UL && codePoint <= 0x11172UL) || codePoint == 0x11176UL
           || (codePoint >= 0x11183UL && codePoint <= 0x111B2UL)
           || (codePoint >= 0x111C1UL && codePoint <= 0x111C4UL)
           || (codePoint >= 0x111DAUL && codePoint <= 0x111DCUL && codePoint % 2 == 0)
           || (codePoint >= 0x11200UL && codePoint <= 0x11211UL)
           || (codePoint >= 0x11213UL && codePoint <= 0x1122BUL)
           || (codePoint >= 0x11280UL && codePoint <= 0x11286UL) || codePoint == 0x11288UL
           || (codePoint >= 0x1128AUL && codePoint <= 0x1128DUL)
           || (codePoint >= 0x1128FUL && codePoint <= 0x1129DUL)
           || (codePoint >= 0x1129FUL && codePoint <= 0x112A8UL)
           || (codePoint >= 0x112B0UL && codePoint <= 0x112DEUL)
           || (codePoint >= 0x11305UL && codePoint <= 0x1130CUL) || codePoint == 0x1130FUL
           || codePoint == 0x11310UL || (codePoint >= 0x11313UL && codePoint <= 0x11328UL)
           || (codePoint >= 0x1132AUL && codePoint <= 0x11330UL) || codePoint == 0x11332UL
           || codePoint == 0x11333UL || (codePoint >= 0x11335UL && codePoint <= 0x11339UL)
           || codePoint == 0x1133DUL || codePoint == 0x11350UL
           || (codePoint >= 0x1135DUL && codePoint <= 0x11361UL)
           || (codePoint >= 0x11480UL && codePoint <= 0x114AFUL) || codePoint == 0x114C4UL
           || codePoint == 0x114C5UL || codePoint == 0x114C7UL
           || (codePoint >= 0x11580UL && codePoint <= 0x115AEUL)
           || (codePoint >= 0x115D8UL && codePoint <= 0x115DBUL)
           || (codePoint >= 0x11600UL && codePoint <= 0x1162FUL) || codePoint == 0x11644UL
           || (codePoint >= 0x11680UL && codePoint <= 0x116AAUL)
           || (codePoint >= 0x11700UL && codePoint <= 0x11719UL) || codePoint == 0x118FFUL
           || (codePoint >= 0x11AC0UL && codePoint <= 0x11AF8UL)
           || (codePoint >= 0x12000UL && codePoint <= 0x12399UL)
           || (codePoint >= 0x12480UL && codePoint <= 0x12543UL)
           || (codePoint >= 0x13000UL && codePoint <= 0x1342EUL)
           || (codePoint >= 0x14400UL && codePoint <= 0x14646UL)
           || (codePoint >= 0x16800UL && codePoint <= 0x16A38UL)
           || (codePoint >= 0x16A40UL && codePoint <= 0x16A5EUL)
           || (codePoint >= 0x16AD0UL && codePoint <= 0x16AEDUL)
           || (codePoint >= 0x16B00UL && codePoint <= 0x16B2FUL)
           || (codePoint >= 0x16B63UL && codePoint <= 0x16B77UL)
           || (codePoint >= 0x16B7DUL && codePoint <= 0x16B8FUL)
           || (codePoint >= 0x16F00UL && codePoint <= 0x16F44UL) || codePoint == 0x16F50UL
           || codePoint == 0x1B000UL || codePoint == 0x1B001UL
           || (codePoint >= 0x1BC00UL && codePoint <= 0x1BC6AUL)
           || (codePoint >= 0x1BC70UL && codePoint <= 0x1BC7CUL)
           || (codePoint >= 0x1BC80UL && codePoint <= 0x1BC88UL)
           || (codePoint >= 0x1BC90UL && codePoint <= 0x1BC99UL)
           || (codePoint >= 0x1E800UL && codePoint <= 0x1E8C4UL)
           || (codePoint >= 0x1EE00UL && codePoint <= 0x1EE03UL)
           || (codePoint >= 0x1EE05UL && codePoint <= 0x1EE1FUL) || codePoint == 0x1EE21UL
           || codePoint == 0x1EE22UL || codePoint == 0x1EE24UL || codePoint == 0x1EE27UL
           || (codePoint >= 0x1EE29UL && codePoint <= 0x1EE32UL)
           || (codePoint >= 0x1EE34UL && codePoint <= 0x1EE37UL)
           || (codePoint >= 0x1EE39UL && codePoint <= 0x1EE3BUL && codePoint % 2 != 0)
           || codePoint == 0x1EE42UL
           || (codePoint >= 0x1EE47UL && codePoint <= 0x1EE4BUL && codePoint % 2 != 0)
           || (codePoint >= 0x1EE4DUL && codePoint <= 0x1EE4FUL) || codePoint == 0x1EE51UL
           || codePoint == 0x1EE52UL || codePoint == 0x1EE54UL
           || (codePoint >= 0x1EE57UL && codePoint <= 0x1EE5FUL && codePoint % 2 != 0)
           || codePoint == 0x1EE61UL || codePoint == 0x1EE62UL || codePoint == 0x1EE64UL
           || (codePoint >= 0x1EE67UL && codePoint <= 0x1EE6AUL)
           || (codePoint >= 0x1EE6CUL && codePoint <= 0x1EE72UL)
           || (codePoint >= 0x1EE74UL && codePoint <= 0x1EE77UL)
           || (codePoint >= 0x1EE79UL && codePoint <= 0x1EE7CUL) || codePoint == 0x1EE7EUL
           || (codePoint >= 0x1EE80UL && codePoint <= 0x1EE89UL)
           || (codePoint >= 0x1EE8BUL && codePoint <= 0x1EE9BUL)
           || (codePoint >= 0x1EEA1UL && codePoint <= 0x1EEA3UL)
           || (codePoint >= 0x1EEA5UL && codePoint <= 0x1EEA9UL)
           || (codePoint >= 0x1EEABUL && codePoint <= 0x1EEBBUL)
           || (codePoint >= 0x20000UL && codePoint <= 0x2A6D6UL)
           || (codePoint >= 0x2A700UL && codePoint <= 0x2B734UL)
           || (codePoint >= 0x2B740UL && codePoint <= 0x2B81DUL)
           || (codePoint >= 0x2B820UL && codePoint <= 0x2CEA1UL)
           || (codePoint >= 0x2F800UL && codePoint <= 0x2FA1DUL);
}
constexpr bool categoryNl(std::uint32_t codePoint) noexcept
{
    return (codePoint >= 0x16EEU && codePoint <= 0x16F0U)
           || (codePoint >= 0x2160U && codePoint <= 0x2182U)
           || (codePoint >= 0x2185U && codePoint <= 0x2188U) || codePoint == 0x3007U
           || (codePoint >= 0x3021U && codePoint <= 0x3029U)
           || (codePoint >= 0x3038U && codePoint <= 0x303AU)
           || (codePoint >= 0xA6E6U && codePoint <= 0xA6EFU)
           || (codePoint >= 0x10140UL && codePoint <= 0x10174UL) || codePoint == 0x10341UL
           || codePoint == 0x1034AUL || (codePoint >= 0x103D1UL && codePoint <= 0x103D5UL)
           || (codePoint >= 0x12400UL && codePoint <= 0x1246EUL);
}
constexpr bool categoryMn(std::uint32_t codePoint) noexcept
{
    return (codePoint >= 0x300U && codePoint <= 0x36FU)
           || (codePoint >= 0x483U && codePoint <= 0x487U)
           || (codePoint >= 0x591U && codePoint <= 0x5BDU) || codePoint == 0x5BFU
           || codePoint == 0x5C1U || codePoint == 0x5C2U || codePoint == 0x5C4U
           || codePoint == 0x5C5U || codePoint == 0x5C7U
           || (codePoint >= 0x610U && codePoint <= 0x61AU)
           || (codePoint >= 0x64BU && codePoint <= 0x65FU) || codePoint == 0x670U
           || (codePoint >= 0x6D6U && codePoint <= 0x6DCU)
           || (codePoint >= 0x6DFU && codePoint <= 0x6E4U) || codePoint == 0x6E7U
           || codePoint == 0x6E8U || (codePoint >= 0x6EAU && codePoint <= 0x6EDU)
           || codePoint == 0x711U || (codePoint >= 0x730U && codePoint <= 0x74AU)
           || (codePoint >= 0x7A6U && codePoint <= 0x7B0U)
           || (codePoint >= 0x7EBU && codePoint <= 0x7F3U)
           || (codePoint >= 0x816U && codePoint <= 0x819U)
           || (codePoint >= 0x81BU && codePoint <= 0x823U)
           || (codePoint >= 0x825U && codePoint <= 0x827U)
           || (codePoint >= 0x829U && codePoint <= 0x82DU)
           || (codePoint >= 0x859U && codePoint <= 0x85BU)
           || (codePoint >= 0x8E3U && codePoint <= 0x902U)
           || (codePoint >= 0x93AU && codePoint <= 0x93CU && codePoint % 2 == 0)
           || (codePoint >= 0x941U && codePoint <= 0x948U) || codePoint == 0x94DU
           || (codePoint >= 0x951U && codePoint <= 0x957U) || codePoint == 0x962U
           || codePoint == 0x963U || codePoint == 0x981U || codePoint == 0x9BCU
           || (codePoint >= 0x9C1U && codePoint <= 0x9C4U) || codePoint == 0x9CDU
           || codePoint == 0x9E2U || codePoint == 0x9E3U || codePoint == 0xA01U
           || codePoint == 0xA02U || codePoint == 0xA3CU || codePoint == 0xA41U
           || codePoint == 0xA42U || codePoint == 0xA47U || codePoint == 0xA48U
           || (codePoint >= 0xA4BU && codePoint <= 0xA4DU) || codePoint == 0xA51U
           || codePoint == 0xA70U || codePoint == 0xA71U || codePoint == 0xA75U
           || codePoint == 0xA81U || codePoint == 0xA82U || codePoint == 0xABCU
           || (codePoint >= 0xAC1U && codePoint <= 0xAC5U) || codePoint == 0xAC7U
           || codePoint == 0xAC8U || codePoint == 0xACDU || codePoint == 0xAE2U
           || codePoint == 0xAE3U || codePoint == 0xB01U || codePoint == 0xB3CU
           || codePoint == 0xB3FU || (codePoint >= 0xB41U && codePoint <= 0xB44U)
           || codePoint == 0xB4DU || codePoint == 0xB56U || codePoint == 0xB62U
           || codePoint == 0xB63U || codePoint == 0xB82U || codePoint == 0xBC0U
           || codePoint == 0xBCDU || codePoint == 0xC00U
           || (codePoint >= 0xC3EU && codePoint <= 0xC40U)
           || (codePoint >= 0xC46U && codePoint <= 0xC48U)
           || (codePoint >= 0xC4AU && codePoint <= 0xC4DU) || codePoint == 0xC55U
           || codePoint == 0xC56U || codePoint == 0xC62U || codePoint == 0xC63U
           || codePoint == 0xC81U || codePoint == 0xCBCU || codePoint == 0xCBFU
           || codePoint == 0xCC6U || codePoint == 0xCCCU || codePoint == 0xCCDU
           || codePoint == 0xCE2U || codePoint == 0xCE3U || codePoint == 0xD01U
           || (codePoint >= 0xD41U && codePoint <= 0xD44U) || codePoint == 0xD4DU
           || codePoint == 0xD62U || codePoint == 0xD63U || codePoint == 0xDCAU
           || (codePoint >= 0xDD2U && codePoint <= 0xDD4U) || codePoint == 0xDD6U
           || codePoint == 0xE31U || (codePoint >= 0xE34U && codePoint <= 0xE3AU)
           || (codePoint >= 0xE47U && codePoint <= 0xE4EU) || codePoint == 0xEB1U
           || (codePoint >= 0xEB4U && codePoint <= 0xEB9U) || codePoint == 0xEBBU
           || codePoint == 0xEBCU || (codePoint >= 0xEC8U && codePoint <= 0xECDU)
           || codePoint == 0xF18U || codePoint == 0xF19U
           || (codePoint >= 0xF35U && codePoint <= 0xF39U && codePoint % 2 != 0)
           || (codePoint >= 0xF71U && codePoint <= 0xF7EU)
           || (codePoint >= 0xF80U && codePoint <= 0xF84U) || codePoint == 0xF86U
           || codePoint == 0xF87U || (codePoint >= 0xF8DU && codePoint <= 0xF97U)
           || (codePoint >= 0xF99U && codePoint <= 0xFBCU) || codePoint == 0xFC6U
           || (codePoint >= 0x102DU && codePoint <= 0x1030U)
           || (codePoint >= 0x1032U && codePoint <= 0x1037U) || codePoint == 0x1039U
           || codePoint == 0x103AU || codePoint == 0x103DU || codePoint == 0x103EU
           || codePoint == 0x1058U || codePoint == 0x1059U
           || (codePoint >= 0x105EU && codePoint <= 0x1060U)
           || (codePoint >= 0x1071U && codePoint <= 0x1074U) || codePoint == 0x1082U
           || codePoint == 0x1085U || codePoint == 0x1086U || codePoint == 0x108DU
           || codePoint == 0x109DU || (codePoint >= 0x135DU && codePoint <= 0x135FU)
           || (codePoint >= 0x1712U && codePoint <= 0x1714U)
           || (codePoint >= 0x1732U && codePoint <= 0x1734U) || codePoint == 0x1752U
           || codePoint == 0x1753U || codePoint == 0x1772U || codePoint == 0x1773U
           || codePoint == 0x17B4U || codePoint == 0x17B5U
           || (codePoint >= 0x17B7U && codePoint <= 0x17BDU) || codePoint == 0x17C6U
           || (codePoint >= 0x17C9U && codePoint <= 0x17D3U) || codePoint == 0x17DDU
           || (codePoint >= 0x180BU && codePoint <= 0x180DU) || codePoint == 0x18A9U
           || (codePoint >= 0x1920U && codePoint <= 0x1922U) || codePoint == 0x1927U
           || codePoint == 0x1928U || codePoint == 0x1932U
           || (codePoint >= 0x1939U && codePoint <= 0x193BU) || codePoint == 0x1A17U
           || codePoint == 0x1A18U || codePoint == 0x1A1BU || codePoint == 0x1A56U
           || (codePoint >= 0x1A58U && codePoint <= 0x1A5EU)
           || (codePoint >= 0x1A60U && codePoint <= 0x1A62U && codePoint % 2 == 0)
           || (codePoint >= 0x1A65U && codePoint <= 0x1A6CU)
           || (codePoint >= 0x1A73U && codePoint <= 0x1A7CU) || codePoint == 0x1A7FU
           || (codePoint >= 0x1AB0U && codePoint <= 0x1ABDU)
           || (codePoint >= 0x1B00U && codePoint <= 0x1B03U) || codePoint == 0x1B34U
           || (codePoint >= 0x1B36U && codePoint <= 0x1B3AU) || codePoint == 0x1B3CU
           || codePoint == 0x1B42U || (codePoint >= 0x1B6BU && codePoint <= 0x1B73U)
           || codePoint == 0x1B80U || codePoint == 0x1B81U
           || (codePoint >= 0x1BA2U && codePoint <= 0x1BA5U) || codePoint == 0x1BA8U
           || codePoint == 0x1BA9U || (codePoint >= 0x1BABU && codePoint <= 0x1BADU)
           || codePoint == 0x1BE6U || codePoint == 0x1BE8U || codePoint == 0x1BE9U
           || codePoint == 0x1BEDU || (codePoint >= 0x1BEFU && codePoint <= 0x1BF1U)
           || (codePoint >= 0x1C2CU && codePoint <= 0x1C33U) || codePoint == 0x1C36U
           || codePoint == 0x1C37U || (codePoint >= 0x1CD0U && codePoint <= 0x1CD2U)
           || (codePoint >= 0x1CD4U && codePoint <= 0x1CE0U)
           || (codePoint >= 0x1CE2U && codePoint <= 0x1CE8U) || codePoint == 0x1CEDU
           || codePoint == 0x1CF4U || codePoint == 0x1CF8U || codePoint == 0x1CF9U
           || (codePoint >= 0x1DC0U && codePoint <= 0x1DF5U)
           || (codePoint >= 0x1DFCU && codePoint <= 0x1DFFU)
           || (codePoint >= 0x20D0U && codePoint <= 0x20DCU) || codePoint == 0x20E1U
           || (codePoint >= 0x20E5U && codePoint <= 0x20F0U)
           || (codePoint >= 0x2CEFU && codePoint <= 0x2CF1U) || codePoint == 0x2D7FU
           || (codePoint >= 0x2DE0U && codePoint <= 0x2DFFU)
           || (codePoint >= 0x302AU && codePoint <= 0x302DU) || codePoint == 0x3099U
           || codePoint == 0x309AU || codePoint == 0xA66FU
           || (codePoint >= 0xA674U && codePoint <= 0xA67DU) || codePoint == 0xA69EU
           || codePoint == 0xA69FU || codePoint == 0xA6F0U || codePoint == 0xA6F1U
           || codePoint == 0xA802U || codePoint == 0xA806U || codePoint == 0xA80BU
           || codePoint == 0xA825U || codePoint == 0xA826U || codePoint == 0xA8C4U
           || (codePoint >= 0xA8E0U && codePoint <= 0xA8F1U)
           || (codePoint >= 0xA926U && codePoint <= 0xA92DU)
           || (codePoint >= 0xA947U && codePoint <= 0xA951U)
           || (codePoint >= 0xA980U && codePoint <= 0xA982U) || codePoint == 0xA9B3U
           || (codePoint >= 0xA9B6U && codePoint <= 0xA9B9U) || codePoint == 0xA9BCU
           || codePoint == 0xA9E5U || (codePoint >= 0xAA29U && codePoint <= 0xAA2EU)
           || codePoint == 0xAA31U || codePoint == 0xAA32U || codePoint == 0xAA35U
           || codePoint == 0xAA36U || codePoint == 0xAA43U || codePoint == 0xAA4CU
           || codePoint == 0xAA7CU || codePoint == 0xAAB0U
           || (codePoint >= 0xAAB2U && codePoint <= 0xAAB4U) || codePoint == 0xAAB7U
           || codePoint == 0xAAB8U || codePoint == 0xAABEU || codePoint == 0xAABFU
           || codePoint == 0xAAC1U || codePoint == 0xAAECU || codePoint == 0xAAEDU
           || codePoint == 0xAAF6U || codePoint == 0xABE5U || codePoint == 0xABE8U
           || codePoint == 0xABEDU || codePoint == 0xFB1EU
           || (codePoint >= 0xFE00U && codePoint <= 0xFE0FU)
           || (codePoint >= 0xFE20U && codePoint <= 0xFE2FU) || codePoint == 0x101FDUL
           || codePoint == 0x102E0UL || (codePoint >= 0x10376UL && codePoint <= 0x1037AUL)
           || (codePoint >= 0x10A01UL && codePoint <= 0x10A03UL) || codePoint == 0x10A05UL
           || codePoint == 0x10A06UL || (codePoint >= 0x10A0CUL && codePoint <= 0x10A0FUL)
           || (codePoint >= 0x10A38UL && codePoint <= 0x10A3AUL) || codePoint == 0x10A3FUL
           || codePoint == 0x10AE5UL || codePoint == 0x10AE6UL || codePoint == 0x11001UL
           || (codePoint >= 0x11038UL && codePoint <= 0x11046UL)
           || (codePoint >= 0x1107FUL && codePoint <= 0x11081UL)
           || (codePoint >= 0x110B3UL && codePoint <= 0x110B6UL) || codePoint == 0x110B9UL
           || codePoint == 0x110BAUL || (codePoint >= 0x11100UL && codePoint <= 0x11102UL)
           || (codePoint >= 0x11127UL && codePoint <= 0x1112BUL)
           || (codePoint >= 0x1112DUL && codePoint <= 0x11134UL) || codePoint == 0x11173UL
           || codePoint == 0x11180UL || codePoint == 0x11181UL
           || (codePoint >= 0x111B6UL && codePoint <= 0x111BEUL)
           || (codePoint >= 0x111CAUL && codePoint <= 0x111CCUL)
           || (codePoint >= 0x1122FUL && codePoint <= 0x11231UL) || codePoint == 0x11234UL
           || codePoint == 0x11236UL || codePoint == 0x11237UL || codePoint == 0x112DFUL
           || (codePoint >= 0x112E3UL && codePoint <= 0x112EAUL) || codePoint == 0x11300UL
           || codePoint == 0x11301UL || codePoint == 0x1133CUL || codePoint == 0x11340UL
           || (codePoint >= 0x11366UL && codePoint <= 0x1136CUL)
           || (codePoint >= 0x11370UL && codePoint <= 0x11374UL)
           || (codePoint >= 0x114B3UL && codePoint <= 0x114B8UL) || codePoint == 0x114BAUL
           || codePoint == 0x114BFUL || codePoint == 0x114C0UL || codePoint == 0x114C2UL
           || codePoint == 0x114C3UL || (codePoint >= 0x115B2UL && codePoint <= 0x115B5UL)
           || codePoint == 0x115BCUL || codePoint == 0x115BDUL || codePoint == 0x115BFUL
           || codePoint == 0x115C0UL || codePoint == 0x115DCUL || codePoint == 0x115DDUL
           || (codePoint >= 0x11633UL && codePoint <= 0x1163AUL) || codePoint == 0x1163DUL
           || codePoint == 0x1163FUL || codePoint == 0x11640UL
           || (codePoint >= 0x116ABUL && codePoint <= 0x116ADUL && codePoint % 2 != 0)
           || (codePoint >= 0x116B0UL && codePoint <= 0x116B5UL) || codePoint == 0x116B7UL
           || (codePoint >= 0x1171DUL && codePoint <= 0x1171FUL)
           || (codePoint >= 0x11722UL && codePoint <= 0x11725UL)
           || (codePoint >= 0x11727UL && codePoint <= 0x1172BUL)
           || (codePoint >= 0x16AF0UL && codePoint <= 0x16AF4UL)
           || (codePoint >= 0x16B30UL && codePoint <= 0x16B36UL)
           || (codePoint >= 0x16F8FUL && codePoint <= 0x16F92UL) || codePoint == 0x1BC9DUL
           || codePoint == 0x1BC9EUL || (codePoint >= 0x1D167UL && codePoint <= 0x1D169UL)
           || (codePoint >= 0x1D17BUL && codePoint <= 0x1D182UL)
           || (codePoint >= 0x1D185UL && codePoint <= 0x1D18BUL)
           || (codePoint >= 0x1D1AAUL && codePoint <= 0x1D1ADUL)
           || (codePoint >= 0x1D242UL && codePoint <= 0x1D244UL)
           || (codePoint >= 0x1DA00UL && codePoint <= 0x1DA36UL)
           || (codePoint >= 0x1DA3BUL && codePoint <= 0x1DA6CUL) || codePoint == 0x1DA75UL
           || codePoint == 0x1DA84UL || (codePoint >= 0x1DA9BUL && codePoint <= 0x1DA9FUL)
           || (codePoint >= 0x1DAA1UL && codePoint <= 0x1DAAFUL)
           || (codePoint >= 0x1E8D0UL && codePoint <= 0x1E8D6UL)
           || (codePoint >= 0xE0100UL && codePoint <= 0xE01EFUL);
}
constexpr bool patternSyntax(std::uint32_t codePoint) noexcept
{
    return (codePoint >= 0x21U && codePoint <= 0x2FU) || (codePoint >= 0x3AU && codePoint <= 0x40U)
           || (codePoint >= 0x5BU && codePoint <= 0x5EU) || codePoint == 0x60U
           || (codePoint >= 0x7BU && codePoint <= 0x7EU)
           || (codePoint >= 0xA1U && codePoint <= 0xA7U) || codePoint == 0xA9U || codePoint == 0xABU
           || codePoint == 0xACU || codePoint == 0xAEU || codePoint == 0xB0U || codePoint == 0xB1U
           || codePoint == 0xB6U || codePoint == 0xBBU || codePoint == 0xBFU || codePoint == 0xD7U
           || codePoint == 0xF7U || (codePoint >= 0x2010U && codePoint <= 0x2027U)
           || (codePoint >= 0x2030U && codePoint <= 0x203EU)
           || (codePoint >= 0x2041U && codePoint <= 0x2053U)
           || (codePoint >= 0x2055U && codePoint <= 0x205EU)
           || (codePoint >= 0x2190U && codePoint <= 0x245FU)
           || (codePoint >= 0x2500U && codePoint <= 0x2775U)
           || (codePoint >= 0x2794U && codePoint <= 0x2BFFU)
           || (codePoint >= 0x2E00U && codePoint <= 0x2E7FU)
           || (codePoint >= 0x3001U && codePoint <= 0x3003U)
           || (codePoint >= 0x3008U && codePoint <= 0x3020U) || codePoint == 0x3030U
           || codePoint == 0xFD3EU || codePoint == 0xFD3FU || codePoint == 0xFE45U
           || codePoint == 0xFE46U;
}
constexpr bool categoryMc(std::uint32_t codePoint) noexcept
{
    return codePoint == 0x903U || codePoint == 0x93BU
           || (codePoint >= 0x93EU && codePoint <= 0x940U)
           || (codePoint >= 0x949U && codePoint <= 0x94CU) || codePoint == 0x94EU
           || codePoint == 0x94FU || codePoint == 0x982U || codePoint == 0x983U
           || (codePoint >= 0x9BEU && codePoint <= 0x9C0U) || codePoint == 0x9C7U
           || codePoint == 0x9C8U || codePoint == 0x9CBU || codePoint == 0x9CCU
           || codePoint == 0x9D7U || codePoint == 0xA03U
           || (codePoint >= 0xA3EU && codePoint <= 0xA40U) || codePoint == 0xA83U
           || (codePoint >= 0xABEU && codePoint <= 0xAC0U) || codePoint == 0xAC9U
           || codePoint == 0xACBU || codePoint == 0xACCU || codePoint == 0xB02U
           || codePoint == 0xB03U
           || (codePoint >= 0xB3EU && codePoint <= 0xB40U && codePoint % 2 == 0)
           || codePoint == 0xB47U || codePoint == 0xB48U || codePoint == 0xB4BU
           || codePoint == 0xB4CU || codePoint == 0xB57U || codePoint == 0xBBEU
           || codePoint == 0xBBFU || codePoint == 0xBC1U || codePoint == 0xBC2U
           || (codePoint >= 0xBC6U && codePoint <= 0xBC8U)
           || (codePoint >= 0xBCAU && codePoint <= 0xBCCU) || codePoint == 0xBD7U
           || (codePoint >= 0xC01U && codePoint <= 0xC03U)
           || (codePoint >= 0xC41U && codePoint <= 0xC44U) || codePoint == 0xC82U
           || codePoint == 0xC83U || codePoint == 0xCBEU
           || (codePoint >= 0xCC0U && codePoint <= 0xCC4U) || codePoint == 0xCC7U
           || codePoint == 0xCC8U || codePoint == 0xCCAU || codePoint == 0xCCBU
           || codePoint == 0xCD5U || codePoint == 0xCD6U || codePoint == 0xD02U
           || codePoint == 0xD03U || (codePoint >= 0xD3EU && codePoint <= 0xD40U)
           || (codePoint >= 0xD46U && codePoint <= 0xD48U)
           || (codePoint >= 0xD4AU && codePoint <= 0xD4CU) || codePoint == 0xD57U
           || codePoint == 0xD82U || codePoint == 0xD83U
           || (codePoint >= 0xDCFU && codePoint <= 0xDD1U)
           || (codePoint >= 0xDD8U && codePoint <= 0xDDFU) || codePoint == 0xDF2U
           || codePoint == 0xDF3U || codePoint == 0xF3EU || codePoint == 0xF3FU
           || codePoint == 0xF7FU || codePoint == 0x102BU || codePoint == 0x102CU
           || codePoint == 0x1031U || codePoint == 0x1038U || codePoint == 0x103BU
           || codePoint == 0x103CU || codePoint == 0x1056U || codePoint == 0x1057U
           || (codePoint >= 0x1062U && codePoint <= 0x1064U)
           || (codePoint >= 0x1067U && codePoint <= 0x106DU) || codePoint == 0x1083U
           || codePoint == 0x1084U || (codePoint >= 0x1087U && codePoint <= 0x108CU)
           || codePoint == 0x108FU || (codePoint >= 0x109AU && codePoint <= 0x109CU)
           || codePoint == 0x17B6U || (codePoint >= 0x17BEU && codePoint <= 0x17C5U)
           || codePoint == 0x17C7U || codePoint == 0x17C8U
           || (codePoint >= 0x1923U && codePoint <= 0x1926U)
           || (codePoint >= 0x1929U && codePoint <= 0x192BU) || codePoint == 0x1930U
           || codePoint == 0x1931U || (codePoint >= 0x1933U && codePoint <= 0x1938U)
           || codePoint == 0x1A19U || codePoint == 0x1A1AU
           || (codePoint >= 0x1A55U && codePoint <= 0x1A57U && codePoint % 2 != 0)
           || codePoint == 0x1A61U || codePoint == 0x1A63U || codePoint == 0x1A64U
           || (codePoint >= 0x1A6DU && codePoint <= 0x1A72U) || codePoint == 0x1B04U
           || codePoint == 0x1B35U || codePoint == 0x1B3BU
           || (codePoint >= 0x1B3DU && codePoint <= 0x1B41U) || codePoint == 0x1B43U
           || codePoint == 0x1B44U || codePoint == 0x1B82U || codePoint == 0x1BA1U
           || codePoint == 0x1BA6U || codePoint == 0x1BA7U || codePoint == 0x1BAAU
           || codePoint == 0x1BE7U || (codePoint >= 0x1BEAU && codePoint <= 0x1BECU)
           || codePoint == 0x1BEEU || codePoint == 0x1BF2U || codePoint == 0x1BF3U
           || (codePoint >= 0x1C24U && codePoint <= 0x1C2BU) || codePoint == 0x1C34U
           || codePoint == 0x1C35U || codePoint == 0x1CE1U || codePoint == 0x1CF2U
           || codePoint == 0x1CF3U || codePoint == 0x302EU || codePoint == 0x302FU
           || codePoint == 0xA823U || codePoint == 0xA824U || codePoint == 0xA827U
           || codePoint == 0xA880U || codePoint == 0xA881U
           || (codePoint >= 0xA8B4U && codePoint <= 0xA8C3U) || codePoint == 0xA952U
           || codePoint == 0xA953U || codePoint == 0xA983U || codePoint == 0xA9B4U
           || codePoint == 0xA9B5U || codePoint == 0xA9BAU || codePoint == 0xA9BBU
           || (codePoint >= 0xA9BDU && codePoint <= 0xA9C0U) || codePoint == 0xAA2FU
           || codePoint == 0xAA30U || codePoint == 0xAA33U || codePoint == 0xAA34U
           || codePoint == 0xAA4DU
           || (codePoint >= 0xAA7BU && codePoint <= 0xAA7DU && codePoint % 2 != 0)
           || codePoint == 0xAAEBU || codePoint == 0xAAEEU || codePoint == 0xAAEFU
           || codePoint == 0xAAF5U || codePoint == 0xABE3U || codePoint == 0xABE4U
           || codePoint == 0xABE6U || codePoint == 0xABE7U || codePoint == 0xABE9U
           || codePoint == 0xABEAU || codePoint == 0xABECU
           || (codePoint >= 0x11000UL && codePoint <= 0x11002UL && codePoint % 2 == 0)
           || codePoint == 0x11082UL || (codePoint >= 0x110B0UL && codePoint <= 0x110B2UL)
           || codePoint == 0x110B7UL || codePoint == 0x110B8UL || codePoint == 0x1112CUL
           || codePoint == 0x11182UL || (codePoint >= 0x111B3UL && codePoint <= 0x111B5UL)
           || codePoint == 0x111BFUL || codePoint == 0x111C0UL
           || (codePoint >= 0x1122CUL && codePoint <= 0x1122EUL) || codePoint == 0x11232UL
           || codePoint == 0x11233UL || codePoint == 0x11235UL
           || (codePoint >= 0x112E0UL && codePoint <= 0x112E2UL) || codePoint == 0x11302UL
           || codePoint == 0x11303UL || codePoint == 0x1133EUL || codePoint == 0x1133FUL
           || (codePoint >= 0x11341UL && codePoint <= 0x11344UL) || codePoint == 0x11347UL
           || codePoint == 0x11348UL || (codePoint >= 0x1134BUL && codePoint <= 0x1134DUL)
           || codePoint == 0x11357UL || codePoint == 0x11362UL || codePoint == 0x11363UL
           || (codePoint >= 0x114B0UL && codePoint <= 0x114B2UL) || codePoint == 0x114B9UL
           || (codePoint >= 0x114BBUL && codePoint <= 0x114BEUL) || codePoint == 0x114C1UL
           || (codePoint >= 0x115AFUL && codePoint <= 0x115B1UL)
           || (codePoint >= 0x115B8UL && codePoint <= 0x115BBUL) || codePoint == 0x115BEUL
           || (codePoint >= 0x11630UL && codePoint <= 0x11632UL) || codePoint == 0x1163BUL
           || codePoint == 0x1163CUL || codePoint == 0x1163EUL || codePoint == 0x116ACUL
           || codePoint == 0x116AEUL || codePoint == 0x116AFUL || codePoint == 0x116B6UL
           || codePoint == 0x11720UL || codePoint == 0x11721UL || codePoint == 0x11726UL
           || (codePoint >= 0x16F51UL && codePoint <= 0x16F7EUL) || codePoint == 0x1D165UL
           || codePoint == 0x1D166UL || (codePoint >= 0x1D16DUL && codePoint <= 0x1D172UL);
}
constexpr bool categoryNd(std::uint32_t codePoint) noexcept
{
    return (codePoint >= 0x30U && codePoint <= 0x39U)
           || (codePoint >= 0x660U && codePoint <= 0x669U)
           || (codePoint >= 0x6F0U && codePoint <= 0x6F9U)
           || (codePoint >= 0x7C0U && codePoint <= 0x7C9U)
           || (codePoint >= 0x966U && codePoint <= 0x96FU)
           || (codePoint >= 0x9E6U && codePoint <= 0x9EFU)
           || (codePoint >= 0xA66U && codePoint <= 0xA6FU)
           || (codePoint >= 0xAE6U && codePoint <= 0xAEFU)
           || (codePoint >= 0xB66U && codePoint <= 0xB6FU)
           || (codePoint >= 0xBE6U && codePoint <= 0xBEFU)
           || (codePoint >= 0xC66U && codePoint <= 0xC6FU)
           || (codePoint >= 0xCE6U && codePoint <= 0xCEFU)
           || (codePoint >= 0xD66U && codePoint <= 0xD6FU)
           || (codePoint >= 0xDE6U && codePoint <= 0xDEFU)
           || (codePoint >= 0xE50U && codePoint <= 0xE59U)
           || (codePoint >= 0xED0U && codePoint <= 0xED9U)
           || (codePoint >= 0xF20U && codePoint <= 0xF29U)
           || (codePoint >= 0x1040U && codePoint <= 0x1049U)
           || (codePoint >= 0x1090U && codePoint <= 0x1099U)
           || (codePoint >= 0x17E0U && codePoint <= 0x17E9U)
           || (codePoint >= 0x1810U && codePoint <= 0x1819U)
           || (codePoint >= 0x1946U && codePoint <= 0x194FU)
           || (codePoint >= 0x19D0U && codePoint <= 0x19D9U)
           || (codePoint >= 0x1A80U && codePoint <= 0x1A89U)
           || (codePoint >= 0x1A90U && codePoint <= 0x1A99U)
           || (codePoint >= 0x1B50U && codePoint <= 0x1B59U)
           || (codePoint >= 0x1BB0U && codePoint <= 0x1BB9U)
           || (codePoint >= 0x1C40U && codePoint <= 0x1C49U)
           || (codePoint >= 0x1C50U && codePoint <= 0x1C59U)
           || (codePoint >= 0xA620U && codePoint <= 0xA629U)
           || (codePoint >= 0xA8D0U && codePoint <= 0xA8D9U)
           || (codePoint >= 0xA900U && codePoint <= 0xA909U)
           || (codePoint >= 0xA9D0U && codePoint <= 0xA9D9U)
           || (codePoint >= 0xA9F0U && codePoint <= 0xA9F9U)
           || (codePoint >= 0xAA50U && codePoint <= 0xAA59U)
           || (codePoint >= 0xABF0U && codePoint <= 0xABF9U)
           || (codePoint >= 0xFF10U && codePoint <= 0xFF19U)
           || (codePoint >= 0x104A0UL && codePoint <= 0x104A9UL)
           || (codePoint >= 0x11066UL && codePoint <= 0x1106FUL)
           || (codePoint >= 0x110F0UL && codePoint <= 0x110F9UL)
           || (codePoint >= 0x11136UL && codePoint <= 0x1113FUL)
           || (codePoint >= 0x111D0UL && codePoint <= 0x111D9UL)
           || (codePoint >= 0x112F0UL && codePoint <= 0x112F9UL)
           || (codePoint >= 0x114D0UL && codePoint <= 0x114D9UL)
           || (codePoint >= 0x11650UL && codePoint <= 0x11659UL)
           || (codePoint >= 0x116C0UL && codePoint <= 0x116C9UL)
           || (codePoint >= 0x11730UL && codePoint <= 0x11739UL)
           || (codePoint >= 0x118E0UL && codePoint <= 0x118E9UL)
           || (codePoint >= 0x16A60UL && codePoint <= 0x16A69UL)
           || (codePoint >= 0x16B50UL && codePoint <= 0x16B59UL)
           || (codePoint >= 0x1D7CEUL && codePoint <= 0x1D7FFUL);
}
constexpr bool categoryPc(std::uint32_t codePoint) noexcept
{
    return codePoint == 0x5FU || codePoint == 0x203FU || codePoint == 0x2040U
           || codePoint == 0x2054U || codePoint == 0xFE33U || codePoint == 0xFE34U
           || (codePoint >= 0xFE4DU && codePoint <= 0xFE4FU) || codePoint == 0xFF3FU;
}
constexpr bool categoryZs(std::uint32_t codePoint) noexcept
{
    return codePoint == 0x20U || codePoint == 0xA0U || codePoint == 0x1680U
           || (codePoint >= 0x2000U && codePoint <= 0x200AU) || codePoint == 0x202FU
           || codePoint == 0x205FU || codePoint == 0x3000U;
}
constexpr bool patternWhiteSpace(std::uint32_t codePoint) noexcept
{
    return (codePoint >= 0x9U && codePoint <= 0xDU) || codePoint == 0x20U || codePoint == 0x85U
           || codePoint == 0x200EU || codePoint == 0x200FU || codePoint == 0x2028U
           || codePoint == 0x2029U;
}
constexpr bool otherIdStart(std::uint32_t codePoint) noexcept
{
    return codePoint == 0x2118 || codePoint == 0x212E || codePoint == 0x309B || codePoint == 0x309C;
}
constexpr bool otherIdContinue(std::uint32_t codePoint) noexcept
{
    return codePoint == 0xB7 || codePoint == 0x387 || (codePoint >= 0x1369 && codePoint <= 0x1371)
           || codePoint == 0x19DA;
}
constexpr bool idStart(std::uint32_t codePoint) noexcept
{
    return patternSyntax(codePoint) || patternWhiteSpace(codePoint) ?
               false :
               otherIdStart(codePoint) || categoryLu(codePoint) || categoryLl(codePoint)
                   || categoryLt(codePoint) || categoryLm(codePoint) || categoryLo(codePoint)
                   || categoryNl(codePoint);
}
constexpr bool idContinue(std::uint32_t codePoint) noexcept
{
    return patternSyntax(codePoint) || patternWhiteSpace(codePoint) ?
               false :
               idStart(codePoint) || otherIdContinue(codePoint) || categoryMn(codePoint)
                   || categoryMc(codePoint) || categoryNd(codePoint) || categoryPc(codePoint);
}
constexpr bool javascriptIdStart(std::uint32_t codePoint) noexcept
{
    return idStart(codePoint) || otherIdStart(codePoint) || codePoint == 0x24U
           || codePoint == 0x5FU;
}
constexpr bool javascriptIdContinue(std::uint32_t codePoint) noexcept
{
    return idContinue(codePoint) || otherIdContinue(codePoint) || otherIdStart(codePoint)
           || codePoint == 0x24U || codePoint == 0x5FU || codePoint == 0x200CU
           || codePoint == 0x200DU;
}
constexpr bool javascriptWhiteSpace(std::uint32_t codePoint) noexcept
{
    return categoryZs(codePoint) || codePoint == 0x9U || codePoint == 0xBU || codePoint == 0xCU
           || codePoint == 0xFEFFU;
}
constexpr bool javascriptLineTerminator(std::uint32_t codePoint) noexcept
{
    return codePoint == 0xAU || codePoint == 0xDU || codePoint == 0x2028U || codePoint == 0x2029U;
}
}
}

#endif /* JAVASCRIPT_TASKLETS_CHARACTER_PROPERTIES_H_ */
