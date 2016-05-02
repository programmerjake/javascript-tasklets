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
    return (codepoint >= 0x41U && codepoint <= 0x5AU) || (codepoint >= 0xC0U && codepoint <= 0xD6U)
           || (codepoint >= 0xD8U && codepoint <= 0xDEU)
           || (codepoint >= 0x100U && codepoint <= 0x136U && codepoint % 2 == 0)
           || (codepoint >= 0x139U && codepoint <= 0x147U && codepoint % 2 != 0)
           || (codepoint >= 0x14AU && codepoint <= 0x176U && codepoint % 2 == 0)
           || codepoint == 0x178U || codepoint == 0x179U
           || (codepoint >= 0x17BU && codepoint <= 0x17DU && codepoint % 2 != 0)
           || codepoint == 0x181U || codepoint == 0x182U || codepoint == 0x184U
           || codepoint == 0x186U || codepoint == 0x187U
           || (codepoint >= 0x189U && codepoint <= 0x18BU)
           || (codepoint >= 0x18EU && codepoint <= 0x191U) || codepoint == 0x193U
           || codepoint == 0x194U || (codepoint >= 0x196U && codepoint <= 0x198U)
           || codepoint == 0x19CU || codepoint == 0x19DU || codepoint == 0x19FU
           || codepoint == 0x1A0U
           || (codepoint >= 0x1A2U && codepoint <= 0x1A4U && codepoint % 2 == 0)
           || codepoint == 0x1A6U || codepoint == 0x1A7U || codepoint == 0x1A9U
           || codepoint == 0x1ACU || codepoint == 0x1AEU || codepoint == 0x1AFU
           || (codepoint >= 0x1B1U && codepoint <= 0x1B3U) || codepoint == 0x1B5U
           || codepoint == 0x1B7U || codepoint == 0x1B8U || codepoint == 0x1BCU
           || codepoint == 0x1C4U || codepoint == 0x1C7U || codepoint == 0x1CAU
           || (codepoint >= 0x1CDU && codepoint <= 0x1DBU && codepoint % 2 != 0)
           || (codepoint >= 0x1DEU && codepoint <= 0x1EEU && codepoint % 2 == 0)
           || codepoint == 0x1F1U || codepoint == 0x1F4U
           || (codepoint >= 0x1F6U && codepoint <= 0x1F8U)
           || (codepoint >= 0x1FAU && codepoint <= 0x232U && codepoint % 2 == 0)
           || codepoint == 0x23AU || codepoint == 0x23BU || codepoint == 0x23DU
           || codepoint == 0x23EU || codepoint == 0x241U
           || (codepoint >= 0x243U && codepoint <= 0x246U)
           || (codepoint >= 0x248U && codepoint <= 0x24EU && codepoint % 2 == 0)
           || (codepoint >= 0x370U && codepoint <= 0x372U && codepoint % 2 == 0)
           || codepoint == 0x376U || codepoint == 0x37FU || codepoint == 0x386U
           || (codepoint >= 0x388U && codepoint <= 0x38AU) || codepoint == 0x38CU
           || codepoint == 0x38EU || codepoint == 0x38FU
           || (codepoint >= 0x391U && codepoint <= 0x3A1U)
           || (codepoint >= 0x3A3U && codepoint <= 0x3ABU) || codepoint == 0x3CFU
           || (codepoint >= 0x3D2U && codepoint <= 0x3D4U)
           || (codepoint >= 0x3D8U && codepoint <= 0x3EEU && codepoint % 2 == 0)
           || codepoint == 0x3F4U || codepoint == 0x3F7U || codepoint == 0x3F9U
           || codepoint == 0x3FAU || (codepoint >= 0x3FDU && codepoint <= 0x42FU)
           || (codepoint >= 0x460U && codepoint <= 0x480U && codepoint % 2 == 0)
           || (codepoint >= 0x48AU && codepoint <= 0x4BEU && codepoint % 2 == 0)
           || codepoint == 0x4C0U || codepoint == 0x4C1U
           || (codepoint >= 0x4C3U && codepoint <= 0x4CDU && codepoint % 2 != 0)
           || (codepoint >= 0x4D0U && codepoint <= 0x52EU && codepoint % 2 == 0)
           || (codepoint >= 0x531U && codepoint <= 0x556U)
           || (codepoint >= 0x10A0U && codepoint <= 0x10C5U) || codepoint == 0x10C7U
           || codepoint == 0x10CDU || (codepoint >= 0x13A0U && codepoint <= 0x13F5U)
           || (codepoint >= 0x1E00U && codepoint <= 0x1E94U && codepoint % 2 == 0)
           || (codepoint >= 0x1E9EU && codepoint <= 0x1EFEU && codepoint % 2 == 0)
           || (codepoint >= 0x1F08U && codepoint <= 0x1F0FU)
           || (codepoint >= 0x1F18U && codepoint <= 0x1F1DU)
           || (codepoint >= 0x1F28U && codepoint <= 0x1F2FU)
           || (codepoint >= 0x1F38U && codepoint <= 0x1F3FU)
           || (codepoint >= 0x1F48U && codepoint <= 0x1F4DU)
           || (codepoint >= 0x1F59U && codepoint <= 0x1F5FU && codepoint % 2 != 0)
           || (codepoint >= 0x1F68U && codepoint <= 0x1F6FU)
           || (codepoint >= 0x1FB8U && codepoint <= 0x1FBBU)
           || (codepoint >= 0x1FC8U && codepoint <= 0x1FCBU)
           || (codepoint >= 0x1FD8U && codepoint <= 0x1FDBU)
           || (codepoint >= 0x1FE8U && codepoint <= 0x1FECU)
           || (codepoint >= 0x1FF8U && codepoint <= 0x1FFBU) || codepoint == 0x2102U
           || codepoint == 0x2107U || (codepoint >= 0x210BU && codepoint <= 0x210DU)
           || (codepoint >= 0x2110U && codepoint <= 0x2112U) || codepoint == 0x2115U
           || (codepoint >= 0x2119U && codepoint <= 0x211DU)
           || (codepoint >= 0x2124U && codepoint <= 0x2128U && codepoint % 2 == 0)
           || (codepoint >= 0x212AU && codepoint <= 0x212DU)
           || (codepoint >= 0x2130U && codepoint <= 0x2133U) || codepoint == 0x213EU
           || codepoint == 0x213FU || codepoint == 0x2145U || codepoint == 0x2183U
           || (codepoint >= 0x2C00U && codepoint <= 0x2C2EU) || codepoint == 0x2C60U
           || (codepoint >= 0x2C62U && codepoint <= 0x2C64U)
           || (codepoint >= 0x2C67U && codepoint <= 0x2C6BU && codepoint % 2 != 0)
           || (codepoint >= 0x2C6DU && codepoint <= 0x2C70U) || codepoint == 0x2C72U
           || codepoint == 0x2C75U || (codepoint >= 0x2C7EU && codepoint <= 0x2C80U)
           || (codepoint >= 0x2C82U && codepoint <= 0x2CE2U && codepoint % 2 == 0)
           || (codepoint >= 0x2CEBU && codepoint <= 0x2CEDU && codepoint % 2 != 0)
           || codepoint == 0x2CF2U
           || (codepoint >= 0xA640U && codepoint <= 0xA66CU && codepoint % 2 == 0)
           || (codepoint >= 0xA680U && codepoint <= 0xA69AU && codepoint % 2 == 0)
           || (codepoint >= 0xA722U && codepoint <= 0xA72EU && codepoint % 2 == 0)
           || (codepoint >= 0xA732U && codepoint <= 0xA76EU && codepoint % 2 == 0)
           || (codepoint >= 0xA779U && codepoint <= 0xA77BU && codepoint % 2 != 0)
           || codepoint == 0xA77DU || codepoint == 0xA77EU
           || (codepoint >= 0xA780U && codepoint <= 0xA786U && codepoint % 2 == 0)
           || (codepoint >= 0xA78BU && codepoint <= 0xA78DU && codepoint % 2 != 0)
           || (codepoint >= 0xA790U && codepoint <= 0xA792U && codepoint % 2 == 0)
           || (codepoint >= 0xA796U && codepoint <= 0xA7A8U && codepoint % 2 == 0)
           || (codepoint >= 0xA7AAU && codepoint <= 0xA7ADU)
           || (codepoint >= 0xA7B0U && codepoint <= 0xA7B4U) || codepoint == 0xA7B6U
           || (codepoint >= 0xFF21U && codepoint <= 0xFF3AU)
           || (codepoint >= 0x10400UL && codepoint <= 0x10427UL)
           || (codepoint >= 0x10C80UL && codepoint <= 0x10CB2UL)
           || (codepoint >= 0x118A0UL && codepoint <= 0x118BFUL)
           || (codepoint >= 0x1D400UL && codepoint <= 0x1D419UL)
           || (codepoint >= 0x1D434UL && codepoint <= 0x1D44DUL)
           || (codepoint >= 0x1D468UL && codepoint <= 0x1D481UL) || codepoint == 0x1D49CUL
           || codepoint == 0x1D49EUL || codepoint == 0x1D49FUL || codepoint == 0x1D4A2UL
           || codepoint == 0x1D4A5UL || codepoint == 0x1D4A6UL
           || (codepoint >= 0x1D4A9UL && codepoint <= 0x1D4ACUL)
           || (codepoint >= 0x1D4AEUL && codepoint <= 0x1D4B5UL)
           || (codepoint >= 0x1D4D0UL && codepoint <= 0x1D4E9UL) || codepoint == 0x1D504UL
           || codepoint == 0x1D505UL || (codepoint >= 0x1D507UL && codepoint <= 0x1D50AUL)
           || (codepoint >= 0x1D50DUL && codepoint <= 0x1D514UL)
           || (codepoint >= 0x1D516UL && codepoint <= 0x1D51CUL) || codepoint == 0x1D538UL
           || codepoint == 0x1D539UL || (codepoint >= 0x1D53BUL && codepoint <= 0x1D53EUL)
           || (codepoint >= 0x1D540UL && codepoint <= 0x1D544UL) || codepoint == 0x1D546UL
           || (codepoint >= 0x1D54AUL && codepoint <= 0x1D550UL)
           || (codepoint >= 0x1D56CUL && codepoint <= 0x1D585UL)
           || (codepoint >= 0x1D5A0UL && codepoint <= 0x1D5B9UL)
           || (codepoint >= 0x1D5D4UL && codepoint <= 0x1D5EDUL)
           || (codepoint >= 0x1D608UL && codepoint <= 0x1D621UL)
           || (codepoint >= 0x1D63CUL && codepoint <= 0x1D655UL)
           || (codepoint >= 0x1D670UL && codepoint <= 0x1D689UL)
           || (codepoint >= 0x1D6A8UL && codepoint <= 0x1D6C0UL)
           || (codepoint >= 0x1D6E2UL && codepoint <= 0x1D6FAUL)
           || (codepoint >= 0x1D71CUL && codepoint <= 0x1D734UL)
           || (codepoint >= 0x1D756UL && codepoint <= 0x1D76EUL)
           || (codepoint >= 0x1D790UL && codepoint <= 0x1D7A8UL) || codepoint == 0x1D7CAUL;
}
constexpr bool categoryLl(std::uint32_t codepoint) noexcept
{
    return (codepoint >= 0x61U && codepoint <= 0x7AU) || codepoint == 0xB5U
           || (codepoint >= 0xDFU && codepoint <= 0xF6U)
           || (codepoint >= 0xF8U && codepoint <= 0xFFU)
           || (codepoint >= 0x101U && codepoint <= 0x135U && codepoint % 2 != 0)
           || codepoint == 0x137U || codepoint == 0x138U
           || (codepoint >= 0x13AU && codepoint <= 0x146U && codepoint % 2 == 0)
           || codepoint == 0x148U || codepoint == 0x149U
           || (codepoint >= 0x14BU && codepoint <= 0x177U && codepoint % 2 != 0)
           || (codepoint >= 0x17AU && codepoint <= 0x17CU && codepoint % 2 == 0)
           || (codepoint >= 0x17EU && codepoint <= 0x180U)
           || (codepoint >= 0x183U && codepoint <= 0x185U && codepoint % 2 != 0)
           || codepoint == 0x188U || codepoint == 0x18CU || codepoint == 0x18DU
           || codepoint == 0x192U || codepoint == 0x195U
           || (codepoint >= 0x199U && codepoint <= 0x19BU) || codepoint == 0x19EU
           || (codepoint >= 0x1A1U && codepoint <= 0x1A5U && codepoint % 2 != 0)
           || codepoint == 0x1A8U || codepoint == 0x1AAU || codepoint == 0x1ABU
           || codepoint == 0x1ADU || codepoint == 0x1B0U
           || (codepoint >= 0x1B4U && codepoint <= 0x1B6U && codepoint % 2 == 0)
           || codepoint == 0x1B9U || codepoint == 0x1BAU
           || (codepoint >= 0x1BDU && codepoint <= 0x1BFU) || codepoint == 0x1C6U
           || codepoint == 0x1C9U
           || (codepoint >= 0x1CCU && codepoint <= 0x1DAU && codepoint % 2 == 0)
           || codepoint == 0x1DCU || codepoint == 0x1DDU
           || (codepoint >= 0x1DFU && codepoint <= 0x1EDU && codepoint % 2 != 0)
           || codepoint == 0x1EFU || codepoint == 0x1F0U
           || (codepoint >= 0x1F3U && codepoint <= 0x1F5U && codepoint % 2 != 0)
           || (codepoint >= 0x1F9U && codepoint <= 0x231U && codepoint % 2 != 0)
           || (codepoint >= 0x233U && codepoint <= 0x239U) || codepoint == 0x23CU
           || codepoint == 0x23FU || codepoint == 0x240U || codepoint == 0x242U
           || (codepoint >= 0x247U && codepoint <= 0x24DU && codepoint % 2 != 0)
           || (codepoint >= 0x24FU && codepoint <= 0x293U)
           || (codepoint >= 0x295U && codepoint <= 0x2AFU)
           || (codepoint >= 0x371U && codepoint <= 0x373U && codepoint % 2 != 0)
           || codepoint == 0x377U || (codepoint >= 0x37BU && codepoint <= 0x37DU)
           || codepoint == 0x390U || (codepoint >= 0x3ACU && codepoint <= 0x3CEU)
           || codepoint == 0x3D0U || codepoint == 0x3D1U
           || (codepoint >= 0x3D5U && codepoint <= 0x3D7U)
           || (codepoint >= 0x3D9U && codepoint <= 0x3EDU && codepoint % 2 != 0)
           || (codepoint >= 0x3EFU && codepoint <= 0x3F3U) || codepoint == 0x3F5U
           || codepoint == 0x3F8U || codepoint == 0x3FBU || codepoint == 0x3FCU
           || (codepoint >= 0x430U && codepoint <= 0x45FU)
           || (codepoint >= 0x461U && codepoint <= 0x481U && codepoint % 2 != 0)
           || (codepoint >= 0x48BU && codepoint <= 0x4BFU && codepoint % 2 != 0)
           || (codepoint >= 0x4C2U && codepoint <= 0x4CCU && codepoint % 2 == 0)
           || codepoint == 0x4CEU || codepoint == 0x4CFU
           || (codepoint >= 0x4D1U && codepoint <= 0x52FU && codepoint % 2 != 0)
           || (codepoint >= 0x561U && codepoint <= 0x587U)
           || (codepoint >= 0x13F8U && codepoint <= 0x13FDU)
           || (codepoint >= 0x1D00U && codepoint <= 0x1D2BU)
           || (codepoint >= 0x1D6BU && codepoint <= 0x1D77U)
           || (codepoint >= 0x1D79U && codepoint <= 0x1D9AU)
           || (codepoint >= 0x1E01U && codepoint <= 0x1E93U && codepoint % 2 != 0)
           || (codepoint >= 0x1E95U && codepoint <= 0x1E9DU)
           || (codepoint >= 0x1E9FU && codepoint <= 0x1EFDU && codepoint % 2 != 0)
           || (codepoint >= 0x1EFFU && codepoint <= 0x1F07U)
           || (codepoint >= 0x1F10U && codepoint <= 0x1F15U)
           || (codepoint >= 0x1F20U && codepoint <= 0x1F27U)
           || (codepoint >= 0x1F30U && codepoint <= 0x1F37U)
           || (codepoint >= 0x1F40U && codepoint <= 0x1F45U)
           || (codepoint >= 0x1F50U && codepoint <= 0x1F57U)
           || (codepoint >= 0x1F60U && codepoint <= 0x1F67U)
           || (codepoint >= 0x1F70U && codepoint <= 0x1F7DU)
           || (codepoint >= 0x1F80U && codepoint <= 0x1F87U)
           || (codepoint >= 0x1F90U && codepoint <= 0x1F97U)
           || (codepoint >= 0x1FA0U && codepoint <= 0x1FA7U)
           || (codepoint >= 0x1FB0U && codepoint <= 0x1FB4U) || codepoint == 0x1FB6U
           || codepoint == 0x1FB7U || codepoint == 0x1FBEU
           || (codepoint >= 0x1FC2U && codepoint <= 0x1FC4U) || codepoint == 0x1FC6U
           || codepoint == 0x1FC7U || (codepoint >= 0x1FD0U && codepoint <= 0x1FD3U)
           || codepoint == 0x1FD6U || codepoint == 0x1FD7U
           || (codepoint >= 0x1FE0U && codepoint <= 0x1FE7U)
           || (codepoint >= 0x1FF2U && codepoint <= 0x1FF4U) || codepoint == 0x1FF6U
           || codepoint == 0x1FF7U || codepoint == 0x210AU || codepoint == 0x210EU
           || codepoint == 0x210FU || codepoint == 0x2113U || codepoint == 0x212FU
           || codepoint == 0x2134U || codepoint == 0x2139U || codepoint == 0x213CU
           || codepoint == 0x213DU || (codepoint >= 0x2146U && codepoint <= 0x2149U)
           || codepoint == 0x214EU || codepoint == 0x2184U
           || (codepoint >= 0x2C30U && codepoint <= 0x2C5EU) || codepoint == 0x2C61U
           || codepoint == 0x2C65U || codepoint == 0x2C66U
           || (codepoint >= 0x2C68U && codepoint <= 0x2C6CU && codepoint % 2 == 0)
           || codepoint == 0x2C71U || codepoint == 0x2C73U || codepoint == 0x2C74U
           || (codepoint >= 0x2C76U && codepoint <= 0x2C7BU)
           || (codepoint >= 0x2C81U && codepoint <= 0x2CE1U && codepoint % 2 != 0)
           || codepoint == 0x2CE3U || codepoint == 0x2CE4U
           || (codepoint >= 0x2CECU && codepoint <= 0x2CEEU && codepoint % 2 == 0)
           || codepoint == 0x2CF3U || (codepoint >= 0x2D00U && codepoint <= 0x2D25U)
           || codepoint == 0x2D27U || codepoint == 0x2D2DU
           || (codepoint >= 0xA641U && codepoint <= 0xA66DU && codepoint % 2 != 0)
           || (codepoint >= 0xA681U && codepoint <= 0xA69BU && codepoint % 2 != 0)
           || (codepoint >= 0xA723U && codepoint <= 0xA72DU && codepoint % 2 != 0)
           || (codepoint >= 0xA72FU && codepoint <= 0xA731U)
           || (codepoint >= 0xA733U && codepoint <= 0xA76FU && codepoint % 2 != 0)
           || (codepoint >= 0xA771U && codepoint <= 0xA778U)
           || (codepoint >= 0xA77AU && codepoint <= 0xA77CU && codepoint % 2 == 0)
           || (codepoint >= 0xA77FU && codepoint <= 0xA787U && codepoint % 2 != 0)
           || (codepoint >= 0xA78CU && codepoint <= 0xA78EU && codepoint % 2 == 0)
           || codepoint == 0xA791U || (codepoint >= 0xA793U && codepoint <= 0xA795U)
           || (codepoint >= 0xA797U && codepoint <= 0xA7A9U && codepoint % 2 != 0)
           || (codepoint >= 0xA7B5U && codepoint <= 0xA7B7U && codepoint % 2 != 0)
           || codepoint == 0xA7FAU || (codepoint >= 0xAB30U && codepoint <= 0xAB5AU)
           || (codepoint >= 0xAB60U && codepoint <= 0xAB65U)
           || (codepoint >= 0xAB70U && codepoint <= 0xABBFU)
           || (codepoint >= 0xFB00U && codepoint <= 0xFB06U)
           || (codepoint >= 0xFB13U && codepoint <= 0xFB17U)
           || (codepoint >= 0xFF41U && codepoint <= 0xFF5AU)
           || (codepoint >= 0x10428UL && codepoint <= 0x1044FUL)
           || (codepoint >= 0x10CC0UL && codepoint <= 0x10CF2UL)
           || (codepoint >= 0x118C0UL && codepoint <= 0x118DFUL)
           || (codepoint >= 0x1D41AUL && codepoint <= 0x1D433UL)
           || (codepoint >= 0x1D44EUL && codepoint <= 0x1D454UL)
           || (codepoint >= 0x1D456UL && codepoint <= 0x1D467UL)
           || (codepoint >= 0x1D482UL && codepoint <= 0x1D49BUL)
           || (codepoint >= 0x1D4B6UL && codepoint <= 0x1D4B9UL) || codepoint == 0x1D4BBUL
           || (codepoint >= 0x1D4BDUL && codepoint <= 0x1D4C3UL)
           || (codepoint >= 0x1D4C5UL && codepoint <= 0x1D4CFUL)
           || (codepoint >= 0x1D4EAUL && codepoint <= 0x1D503UL)
           || (codepoint >= 0x1D51EUL && codepoint <= 0x1D537UL)
           || (codepoint >= 0x1D552UL && codepoint <= 0x1D56BUL)
           || (codepoint >= 0x1D586UL && codepoint <= 0x1D59FUL)
           || (codepoint >= 0x1D5BAUL && codepoint <= 0x1D5D3UL)
           || (codepoint >= 0x1D5EEUL && codepoint <= 0x1D607UL)
           || (codepoint >= 0x1D622UL && codepoint <= 0x1D63BUL)
           || (codepoint >= 0x1D656UL && codepoint <= 0x1D66FUL)
           || (codepoint >= 0x1D68AUL && codepoint <= 0x1D6A5UL)
           || (codepoint >= 0x1D6C2UL && codepoint <= 0x1D6DAUL)
           || (codepoint >= 0x1D6DCUL && codepoint <= 0x1D6E1UL)
           || (codepoint >= 0x1D6FCUL && codepoint <= 0x1D714UL)
           || (codepoint >= 0x1D716UL && codepoint <= 0x1D71BUL)
           || (codepoint >= 0x1D736UL && codepoint <= 0x1D74EUL)
           || (codepoint >= 0x1D750UL && codepoint <= 0x1D755UL)
           || (codepoint >= 0x1D770UL && codepoint <= 0x1D788UL)
           || (codepoint >= 0x1D78AUL && codepoint <= 0x1D78FUL)
           || (codepoint >= 0x1D7AAUL && codepoint <= 0x1D7C2UL)
           || (codepoint >= 0x1D7C4UL && codepoint <= 0x1D7C9UL) || codepoint == 0x1D7CBUL;
}
constexpr bool categoryLt(std::uint32_t codepoint) noexcept
{
    return codepoint == 0x1C5U || codepoint == 0x1C8U || codepoint == 0x1CBU || codepoint == 0x1F2U
           || (codepoint >= 0x1F88U && codepoint <= 0x1F8FU)
           || (codepoint >= 0x1F98U && codepoint <= 0x1F9FU)
           || (codepoint >= 0x1FA8U && codepoint <= 0x1FAFU) || codepoint == 0x1FBCU
           || codepoint == 0x1FCCU || codepoint == 0x1FFCU;
}
constexpr bool categoryLm(std::uint32_t codepoint) noexcept
{
    return (codepoint >= 0x2B0U && codepoint <= 0x2C1U)
           || (codepoint >= 0x2C6U && codepoint <= 0x2D1U)
           || (codepoint >= 0x2E0U && codepoint <= 0x2E4U)
           || (codepoint >= 0x2ECU && codepoint <= 0x2EEU && codepoint % 2 == 0)
           || codepoint == 0x374U || codepoint == 0x37AU || codepoint == 0x559U
           || codepoint == 0x640U || codepoint == 0x6E5U || codepoint == 0x6E6U
           || codepoint == 0x7F4U || codepoint == 0x7F5U || codepoint == 0x7FAU
           || codepoint == 0x81AU || codepoint == 0x824U || codepoint == 0x828U
           || codepoint == 0x971U || codepoint == 0xE46U || codepoint == 0xEC6U
           || codepoint == 0x10FCU || codepoint == 0x17D7U || codepoint == 0x1843U
           || codepoint == 0x1AA7U || (codepoint >= 0x1C78U && codepoint <= 0x1C7DU)
           || (codepoint >= 0x1D2CU && codepoint <= 0x1D6AU) || codepoint == 0x1D78U
           || (codepoint >= 0x1D9BU && codepoint <= 0x1DBFU) || codepoint == 0x2071U
           || codepoint == 0x207FU || (codepoint >= 0x2090U && codepoint <= 0x209CU)
           || codepoint == 0x2C7CU || codepoint == 0x2C7DU || codepoint == 0x2D6FU
           || codepoint == 0x2E2FU || codepoint == 0x3005U
           || (codepoint >= 0x3031U && codepoint <= 0x3035U) || codepoint == 0x303BU
           || codepoint == 0x309DU || codepoint == 0x309EU
           || (codepoint >= 0x30FCU && codepoint <= 0x30FEU) || codepoint == 0xA015U
           || (codepoint >= 0xA4F8U && codepoint <= 0xA4FDU) || codepoint == 0xA60CU
           || codepoint == 0xA67FU || codepoint == 0xA69CU || codepoint == 0xA69DU
           || (codepoint >= 0xA717U && codepoint <= 0xA71FU) || codepoint == 0xA770U
           || codepoint == 0xA788U || codepoint == 0xA7F8U || codepoint == 0xA7F9U
           || codepoint == 0xA9CFU || codepoint == 0xA9E6U || codepoint == 0xAA70U
           || codepoint == 0xAADDU || codepoint == 0xAAF3U || codepoint == 0xAAF4U
           || (codepoint >= 0xAB5CU && codepoint <= 0xAB5FU) || codepoint == 0xFF70U
           || codepoint == 0xFF9EU || codepoint == 0xFF9FU
           || (codepoint >= 0x16B40UL && codepoint <= 0x16B43UL)
           || (codepoint >= 0x16F93UL && codepoint <= 0x16F9FUL);
}
constexpr bool categoryLo(std::uint32_t codepoint) noexcept
{
    return codepoint == 0xAAU || codepoint == 0xBAU || codepoint == 0x1BBU
           || (codepoint >= 0x1C0U && codepoint <= 0x1C3U) || codepoint == 0x294U
           || (codepoint >= 0x5D0U && codepoint <= 0x5EAU)
           || (codepoint >= 0x5F0U && codepoint <= 0x5F2U)
           || (codepoint >= 0x620U && codepoint <= 0x63FU)
           || (codepoint >= 0x641U && codepoint <= 0x64AU) || codepoint == 0x66EU
           || codepoint == 0x66FU || (codepoint >= 0x671U && codepoint <= 0x6D3U)
           || codepoint == 0x6D5U || codepoint == 0x6EEU || codepoint == 0x6EFU
           || (codepoint >= 0x6FAU && codepoint <= 0x6FCU) || codepoint == 0x6FFU
           || codepoint == 0x710U || (codepoint >= 0x712U && codepoint <= 0x72FU)
           || (codepoint >= 0x74DU && codepoint <= 0x7A5U) || codepoint == 0x7B1U
           || (codepoint >= 0x7CAU && codepoint <= 0x7EAU)
           || (codepoint >= 0x800U && codepoint <= 0x815U)
           || (codepoint >= 0x840U && codepoint <= 0x858U)
           || (codepoint >= 0x8A0U && codepoint <= 0x8B4U)
           || (codepoint >= 0x904U && codepoint <= 0x939U) || codepoint == 0x93DU
           || codepoint == 0x950U || (codepoint >= 0x958U && codepoint <= 0x961U)
           || (codepoint >= 0x972U && codepoint <= 0x980U)
           || (codepoint >= 0x985U && codepoint <= 0x98CU) || codepoint == 0x98FU
           || codepoint == 0x990U || (codepoint >= 0x993U && codepoint <= 0x9A8U)
           || (codepoint >= 0x9AAU && codepoint <= 0x9B0U) || codepoint == 0x9B2U
           || (codepoint >= 0x9B6U && codepoint <= 0x9B9U) || codepoint == 0x9BDU
           || codepoint == 0x9CEU || codepoint == 0x9DCU || codepoint == 0x9DDU
           || (codepoint >= 0x9DFU && codepoint <= 0x9E1U) || codepoint == 0x9F0U
           || codepoint == 0x9F1U || (codepoint >= 0xA05U && codepoint <= 0xA0AU)
           || codepoint == 0xA0FU || codepoint == 0xA10U
           || (codepoint >= 0xA13U && codepoint <= 0xA28U)
           || (codepoint >= 0xA2AU && codepoint <= 0xA30U) || codepoint == 0xA32U
           || codepoint == 0xA33U || codepoint == 0xA35U || codepoint == 0xA36U
           || codepoint == 0xA38U || codepoint == 0xA39U
           || (codepoint >= 0xA59U && codepoint <= 0xA5CU) || codepoint == 0xA5EU
           || (codepoint >= 0xA72U && codepoint <= 0xA74U)
           || (codepoint >= 0xA85U && codepoint <= 0xA8DU)
           || (codepoint >= 0xA8FU && codepoint <= 0xA91U)
           || (codepoint >= 0xA93U && codepoint <= 0xAA8U)
           || (codepoint >= 0xAAAU && codepoint <= 0xAB0U) || codepoint == 0xAB2U
           || codepoint == 0xAB3U || (codepoint >= 0xAB5U && codepoint <= 0xAB9U)
           || codepoint == 0xABDU || codepoint == 0xAD0U || codepoint == 0xAE0U
           || codepoint == 0xAE1U || codepoint == 0xAF9U
           || (codepoint >= 0xB05U && codepoint <= 0xB0CU) || codepoint == 0xB0FU
           || codepoint == 0xB10U || (codepoint >= 0xB13U && codepoint <= 0xB28U)
           || (codepoint >= 0xB2AU && codepoint <= 0xB30U) || codepoint == 0xB32U
           || codepoint == 0xB33U || (codepoint >= 0xB35U && codepoint <= 0xB39U)
           || codepoint == 0xB3DU || codepoint == 0xB5CU || codepoint == 0xB5DU
           || (codepoint >= 0xB5FU && codepoint <= 0xB61U) || codepoint == 0xB71U
           || codepoint == 0xB83U || (codepoint >= 0xB85U && codepoint <= 0xB8AU)
           || (codepoint >= 0xB8EU && codepoint <= 0xB90U)
           || (codepoint >= 0xB92U && codepoint <= 0xB95U) || codepoint == 0xB99U
           || codepoint == 0xB9AU || codepoint == 0xB9CU || codepoint == 0xB9EU
           || codepoint == 0xB9FU || codepoint == 0xBA3U || codepoint == 0xBA4U
           || (codepoint >= 0xBA8U && codepoint <= 0xBAAU)
           || (codepoint >= 0xBAEU && codepoint <= 0xBB9U) || codepoint == 0xBD0U
           || (codepoint >= 0xC05U && codepoint <= 0xC0CU)
           || (codepoint >= 0xC0EU && codepoint <= 0xC10U)
           || (codepoint >= 0xC12U && codepoint <= 0xC28U)
           || (codepoint >= 0xC2AU && codepoint <= 0xC39U) || codepoint == 0xC3DU
           || (codepoint >= 0xC58U && codepoint <= 0xC5AU) || codepoint == 0xC60U
           || codepoint == 0xC61U || (codepoint >= 0xC85U && codepoint <= 0xC8CU)
           || (codepoint >= 0xC8EU && codepoint <= 0xC90U)
           || (codepoint >= 0xC92U && codepoint <= 0xCA8U)
           || (codepoint >= 0xCAAU && codepoint <= 0xCB3U)
           || (codepoint >= 0xCB5U && codepoint <= 0xCB9U) || codepoint == 0xCBDU
           || codepoint == 0xCDEU || codepoint == 0xCE0U || codepoint == 0xCE1U
           || codepoint == 0xCF1U || codepoint == 0xCF2U
           || (codepoint >= 0xD05U && codepoint <= 0xD0CU)
           || (codepoint >= 0xD0EU && codepoint <= 0xD10U)
           || (codepoint >= 0xD12U && codepoint <= 0xD3AU) || codepoint == 0xD3DU
           || codepoint == 0xD4EU || (codepoint >= 0xD5FU && codepoint <= 0xD61U)
           || (codepoint >= 0xD7AU && codepoint <= 0xD7FU)
           || (codepoint >= 0xD85U && codepoint <= 0xD96U)
           || (codepoint >= 0xD9AU && codepoint <= 0xDB1U)
           || (codepoint >= 0xDB3U && codepoint <= 0xDBBU) || codepoint == 0xDBDU
           || (codepoint >= 0xDC0U && codepoint <= 0xDC6U)
           || (codepoint >= 0xE01U && codepoint <= 0xE30U) || codepoint == 0xE32U
           || codepoint == 0xE33U || (codepoint >= 0xE40U && codepoint <= 0xE45U)
           || codepoint == 0xE81U || codepoint == 0xE82U || codepoint == 0xE84U
           || codepoint == 0xE87U || codepoint == 0xE88U || codepoint == 0xE8AU
           || codepoint == 0xE8DU || (codepoint >= 0xE94U && codepoint <= 0xE97U)
           || (codepoint >= 0xE99U && codepoint <= 0xE9FU)
           || (codepoint >= 0xEA1U && codepoint <= 0xEA3U)
           || (codepoint >= 0xEA5U && codepoint <= 0xEA7U && codepoint % 2 != 0)
           || codepoint == 0xEAAU || codepoint == 0xEABU
           || (codepoint >= 0xEADU && codepoint <= 0xEB0U) || codepoint == 0xEB2U
           || codepoint == 0xEB3U || codepoint == 0xEBDU
           || (codepoint >= 0xEC0U && codepoint <= 0xEC4U)
           || (codepoint >= 0xEDCU && codepoint <= 0xEDFU) || codepoint == 0xF00U
           || (codepoint >= 0xF40U && codepoint <= 0xF47U)
           || (codepoint >= 0xF49U && codepoint <= 0xF6CU)
           || (codepoint >= 0xF88U && codepoint <= 0xF8CU)
           || (codepoint >= 0x1000U && codepoint <= 0x102AU) || codepoint == 0x103FU
           || (codepoint >= 0x1050U && codepoint <= 0x1055U)
           || (codepoint >= 0x105AU && codepoint <= 0x105DU) || codepoint == 0x1061U
           || codepoint == 0x1065U || codepoint == 0x1066U
           || (codepoint >= 0x106EU && codepoint <= 0x1070U)
           || (codepoint >= 0x1075U && codepoint <= 0x1081U) || codepoint == 0x108EU
           || (codepoint >= 0x10D0U && codepoint <= 0x10FAU)
           || (codepoint >= 0x10FDU && codepoint <= 0x1248U)
           || (codepoint >= 0x124AU && codepoint <= 0x124DU)
           || (codepoint >= 0x1250U && codepoint <= 0x1256U) || codepoint == 0x1258U
           || (codepoint >= 0x125AU && codepoint <= 0x125DU)
           || (codepoint >= 0x1260U && codepoint <= 0x1288U)
           || (codepoint >= 0x128AU && codepoint <= 0x128DU)
           || (codepoint >= 0x1290U && codepoint <= 0x12B0U)
           || (codepoint >= 0x12B2U && codepoint <= 0x12B5U)
           || (codepoint >= 0x12B8U && codepoint <= 0x12BEU) || codepoint == 0x12C0U
           || (codepoint >= 0x12C2U && codepoint <= 0x12C5U)
           || (codepoint >= 0x12C8U && codepoint <= 0x12D6U)
           || (codepoint >= 0x12D8U && codepoint <= 0x1310U)
           || (codepoint >= 0x1312U && codepoint <= 0x1315U)
           || (codepoint >= 0x1318U && codepoint <= 0x135AU)
           || (codepoint >= 0x1380U && codepoint <= 0x138FU)
           || (codepoint >= 0x1401U && codepoint <= 0x166CU)
           || (codepoint >= 0x166FU && codepoint <= 0x167FU)
           || (codepoint >= 0x1681U && codepoint <= 0x169AU)
           || (codepoint >= 0x16A0U && codepoint <= 0x16EAU)
           || (codepoint >= 0x16F1U && codepoint <= 0x16F8U)
           || (codepoint >= 0x1700U && codepoint <= 0x170CU)
           || (codepoint >= 0x170EU && codepoint <= 0x1711U)
           || (codepoint >= 0x1720U && codepoint <= 0x1731U)
           || (codepoint >= 0x1740U && codepoint <= 0x1751U)
           || (codepoint >= 0x1760U && codepoint <= 0x176CU)
           || (codepoint >= 0x176EU && codepoint <= 0x1770U)
           || (codepoint >= 0x1780U && codepoint <= 0x17B3U) || codepoint == 0x17DCU
           || (codepoint >= 0x1820U && codepoint <= 0x1842U)
           || (codepoint >= 0x1844U && codepoint <= 0x1877U)
           || (codepoint >= 0x1880U && codepoint <= 0x18A8U) || codepoint == 0x18AAU
           || (codepoint >= 0x18B0U && codepoint <= 0x18F5U)
           || (codepoint >= 0x1900U && codepoint <= 0x191EU)
           || (codepoint >= 0x1950U && codepoint <= 0x196DU)
           || (codepoint >= 0x1970U && codepoint <= 0x1974U)
           || (codepoint >= 0x1980U && codepoint <= 0x19ABU)
           || (codepoint >= 0x19B0U && codepoint <= 0x19C9U)
           || (codepoint >= 0x1A00U && codepoint <= 0x1A16U)
           || (codepoint >= 0x1A20U && codepoint <= 0x1A54U)
           || (codepoint >= 0x1B05U && codepoint <= 0x1B33U)
           || (codepoint >= 0x1B45U && codepoint <= 0x1B4BU)
           || (codepoint >= 0x1B83U && codepoint <= 0x1BA0U) || codepoint == 0x1BAEU
           || codepoint == 0x1BAFU || (codepoint >= 0x1BBAU && codepoint <= 0x1BE5U)
           || (codepoint >= 0x1C00U && codepoint <= 0x1C23U)
           || (codepoint >= 0x1C4DU && codepoint <= 0x1C4FU)
           || (codepoint >= 0x1C5AU && codepoint <= 0x1C77U)
           || (codepoint >= 0x1CE9U && codepoint <= 0x1CECU)
           || (codepoint >= 0x1CEEU && codepoint <= 0x1CF1U) || codepoint == 0x1CF5U
           || codepoint == 0x1CF6U || (codepoint >= 0x2135U && codepoint <= 0x2138U)
           || (codepoint >= 0x2D30U && codepoint <= 0x2D67U)
           || (codepoint >= 0x2D80U && codepoint <= 0x2D96U)
           || (codepoint >= 0x2DA0U && codepoint <= 0x2DA6U)
           || (codepoint >= 0x2DA8U && codepoint <= 0x2DAEU)
           || (codepoint >= 0x2DB0U && codepoint <= 0x2DB6U)
           || (codepoint >= 0x2DB8U && codepoint <= 0x2DBEU)
           || (codepoint >= 0x2DC0U && codepoint <= 0x2DC6U)
           || (codepoint >= 0x2DC8U && codepoint <= 0x2DCEU)
           || (codepoint >= 0x2DD0U && codepoint <= 0x2DD6U)
           || (codepoint >= 0x2DD8U && codepoint <= 0x2DDEU) || codepoint == 0x3006U
           || codepoint == 0x303CU || (codepoint >= 0x3041U && codepoint <= 0x3096U)
           || codepoint == 0x309FU || (codepoint >= 0x30A1U && codepoint <= 0x30FAU)
           || codepoint == 0x30FFU || (codepoint >= 0x3105U && codepoint <= 0x312DU)
           || (codepoint >= 0x3131U && codepoint <= 0x318EU)
           || (codepoint >= 0x31A0U && codepoint <= 0x31BAU)
           || (codepoint >= 0x31F0U && codepoint <= 0x31FFU)
           || (codepoint >= 0x3400U && codepoint <= 0x4DB5U)
           || (codepoint >= 0x4E00U && codepoint <= 0x9FD5U)
           || (codepoint >= 0xA000U && codepoint <= 0xA014U)
           || (codepoint >= 0xA016U && codepoint <= 0xA48CU)
           || (codepoint >= 0xA4D0U && codepoint <= 0xA4F7U)
           || (codepoint >= 0xA500U && codepoint <= 0xA60BU)
           || (codepoint >= 0xA610U && codepoint <= 0xA61FU) || codepoint == 0xA62AU
           || codepoint == 0xA62BU || codepoint == 0xA66EU
           || (codepoint >= 0xA6A0U && codepoint <= 0xA6E5U) || codepoint == 0xA78FU
           || codepoint == 0xA7F7U || (codepoint >= 0xA7FBU && codepoint <= 0xA801U)
           || (codepoint >= 0xA803U && codepoint <= 0xA805U)
           || (codepoint >= 0xA807U && codepoint <= 0xA80AU)
           || (codepoint >= 0xA80CU && codepoint <= 0xA822U)
           || (codepoint >= 0xA840U && codepoint <= 0xA873U)
           || (codepoint >= 0xA882U && codepoint <= 0xA8B3U)
           || (codepoint >= 0xA8F2U && codepoint <= 0xA8F7U)
           || (codepoint >= 0xA8FBU && codepoint <= 0xA8FDU && codepoint % 2 != 0)
           || (codepoint >= 0xA90AU && codepoint <= 0xA925U)
           || (codepoint >= 0xA930U && codepoint <= 0xA946U)
           || (codepoint >= 0xA960U && codepoint <= 0xA97CU)
           || (codepoint >= 0xA984U && codepoint <= 0xA9B2U)
           || (codepoint >= 0xA9E0U && codepoint <= 0xA9E4U)
           || (codepoint >= 0xA9E7U && codepoint <= 0xA9EFU)
           || (codepoint >= 0xA9FAU && codepoint <= 0xA9FEU)
           || (codepoint >= 0xAA00U && codepoint <= 0xAA28U)
           || (codepoint >= 0xAA40U && codepoint <= 0xAA42U)
           || (codepoint >= 0xAA44U && codepoint <= 0xAA4BU)
           || (codepoint >= 0xAA60U && codepoint <= 0xAA6FU)
           || (codepoint >= 0xAA71U && codepoint <= 0xAA76U) || codepoint == 0xAA7AU
           || (codepoint >= 0xAA7EU && codepoint <= 0xAAAFU) || codepoint == 0xAAB1U
           || codepoint == 0xAAB5U || codepoint == 0xAAB6U
           || (codepoint >= 0xAAB9U && codepoint <= 0xAABDU)
           || (codepoint >= 0xAAC0U && codepoint <= 0xAAC2U && codepoint % 2 == 0)
           || codepoint == 0xAADBU || codepoint == 0xAADCU
           || (codepoint >= 0xAAE0U && codepoint <= 0xAAEAU) || codepoint == 0xAAF2U
           || (codepoint >= 0xAB01U && codepoint <= 0xAB06U)
           || (codepoint >= 0xAB09U && codepoint <= 0xAB0EU)
           || (codepoint >= 0xAB11U && codepoint <= 0xAB16U)
           || (codepoint >= 0xAB20U && codepoint <= 0xAB26U)
           || (codepoint >= 0xAB28U && codepoint <= 0xAB2EU)
           || (codepoint >= 0xABC0U && codepoint <= 0xABE2U)
           || (codepoint >= 0xAC00U && codepoint <= 0xD7A3U)
           || (codepoint >= 0xD7B0U && codepoint <= 0xD7C6U)
           || (codepoint >= 0xD7CBU && codepoint <= 0xD7FBU)
           || (codepoint >= 0xF900U && codepoint <= 0xFA6DU)
           || (codepoint >= 0xFA70U && codepoint <= 0xFAD9U) || codepoint == 0xFB1DU
           || (codepoint >= 0xFB1FU && codepoint <= 0xFB28U)
           || (codepoint >= 0xFB2AU && codepoint <= 0xFB36U)
           || (codepoint >= 0xFB38U && codepoint <= 0xFB3CU) || codepoint == 0xFB3EU
           || codepoint == 0xFB40U || codepoint == 0xFB41U || codepoint == 0xFB43U
           || codepoint == 0xFB44U || (codepoint >= 0xFB46U && codepoint <= 0xFBB1U)
           || (codepoint >= 0xFBD3U && codepoint <= 0xFD3DU)
           || (codepoint >= 0xFD50U && codepoint <= 0xFD8FU)
           || (codepoint >= 0xFD92U && codepoint <= 0xFDC7U)
           || (codepoint >= 0xFDF0U && codepoint <= 0xFDFBU)
           || (codepoint >= 0xFE70U && codepoint <= 0xFE74U)
           || (codepoint >= 0xFE76U && codepoint <= 0xFEFCU)
           || (codepoint >= 0xFF66U && codepoint <= 0xFF6FU)
           || (codepoint >= 0xFF71U && codepoint <= 0xFF9DU)
           || (codepoint >= 0xFFA0U && codepoint <= 0xFFBEU)
           || (codepoint >= 0xFFC2U && codepoint <= 0xFFC7U)
           || (codepoint >= 0xFFCAU && codepoint <= 0xFFCFU)
           || (codepoint >= 0xFFD2U && codepoint <= 0xFFD7U)
           || (codepoint >= 0xFFDAU && codepoint <= 0xFFDCU)
           || (codepoint >= 0x10000UL && codepoint <= 0x1000BUL)
           || (codepoint >= 0x1000DUL && codepoint <= 0x10026UL)
           || (codepoint >= 0x10028UL && codepoint <= 0x1003AUL) || codepoint == 0x1003CUL
           || codepoint == 0x1003DUL || (codepoint >= 0x1003FUL && codepoint <= 0x1004DUL)
           || (codepoint >= 0x10050UL && codepoint <= 0x1005DUL)
           || (codepoint >= 0x10080UL && codepoint <= 0x100FAUL)
           || (codepoint >= 0x10280UL && codepoint <= 0x1029CUL)
           || (codepoint >= 0x102A0UL && codepoint <= 0x102D0UL)
           || (codepoint >= 0x10300UL && codepoint <= 0x1031FUL)
           || (codepoint >= 0x10330UL && codepoint <= 0x10340UL)
           || (codepoint >= 0x10342UL && codepoint <= 0x10349UL)
           || (codepoint >= 0x10350UL && codepoint <= 0x10375UL)
           || (codepoint >= 0x10380UL && codepoint <= 0x1039DUL)
           || (codepoint >= 0x103A0UL && codepoint <= 0x103C3UL)
           || (codepoint >= 0x103C8UL && codepoint <= 0x103CFUL)
           || (codepoint >= 0x10450UL && codepoint <= 0x1049DUL)
           || (codepoint >= 0x10500UL && codepoint <= 0x10527UL)
           || (codepoint >= 0x10530UL && codepoint <= 0x10563UL)
           || (codepoint >= 0x10600UL && codepoint <= 0x10736UL)
           || (codepoint >= 0x10740UL && codepoint <= 0x10755UL)
           || (codepoint >= 0x10760UL && codepoint <= 0x10767UL)
           || (codepoint >= 0x10800UL && codepoint <= 0x10805UL) || codepoint == 0x10808UL
           || (codepoint >= 0x1080AUL && codepoint <= 0x10835UL) || codepoint == 0x10837UL
           || codepoint == 0x10838UL || codepoint == 0x1083CUL
           || (codepoint >= 0x1083FUL && codepoint <= 0x10855UL)
           || (codepoint >= 0x10860UL && codepoint <= 0x10876UL)
           || (codepoint >= 0x10880UL && codepoint <= 0x1089EUL)
           || (codepoint >= 0x108E0UL && codepoint <= 0x108F2UL) || codepoint == 0x108F4UL
           || codepoint == 0x108F5UL || (codepoint >= 0x10900UL && codepoint <= 0x10915UL)
           || (codepoint >= 0x10920UL && codepoint <= 0x10939UL)
           || (codepoint >= 0x10980UL && codepoint <= 0x109B7UL) || codepoint == 0x109BEUL
           || codepoint == 0x109BFUL || codepoint == 0x10A00UL
           || (codepoint >= 0x10A10UL && codepoint <= 0x10A13UL)
           || (codepoint >= 0x10A15UL && codepoint <= 0x10A17UL)
           || (codepoint >= 0x10A19UL && codepoint <= 0x10A33UL)
           || (codepoint >= 0x10A60UL && codepoint <= 0x10A7CUL)
           || (codepoint >= 0x10A80UL && codepoint <= 0x10A9CUL)
           || (codepoint >= 0x10AC0UL && codepoint <= 0x10AC7UL)
           || (codepoint >= 0x10AC9UL && codepoint <= 0x10AE4UL)
           || (codepoint >= 0x10B00UL && codepoint <= 0x10B35UL)
           || (codepoint >= 0x10B40UL && codepoint <= 0x10B55UL)
           || (codepoint >= 0x10B60UL && codepoint <= 0x10B72UL)
           || (codepoint >= 0x10B80UL && codepoint <= 0x10B91UL)
           || (codepoint >= 0x10C00UL && codepoint <= 0x10C48UL)
           || (codepoint >= 0x11003UL && codepoint <= 0x11037UL)
           || (codepoint >= 0x11083UL && codepoint <= 0x110AFUL)
           || (codepoint >= 0x110D0UL && codepoint <= 0x110E8UL)
           || (codepoint >= 0x11103UL && codepoint <= 0x11126UL)
           || (codepoint >= 0x11150UL && codepoint <= 0x11172UL) || codepoint == 0x11176UL
           || (codepoint >= 0x11183UL && codepoint <= 0x111B2UL)
           || (codepoint >= 0x111C1UL && codepoint <= 0x111C4UL)
           || (codepoint >= 0x111DAUL && codepoint <= 0x111DCUL && codepoint % 2 == 0)
           || (codepoint >= 0x11200UL && codepoint <= 0x11211UL)
           || (codepoint >= 0x11213UL && codepoint <= 0x1122BUL)
           || (codepoint >= 0x11280UL && codepoint <= 0x11286UL) || codepoint == 0x11288UL
           || (codepoint >= 0x1128AUL && codepoint <= 0x1128DUL)
           || (codepoint >= 0x1128FUL && codepoint <= 0x1129DUL)
           || (codepoint >= 0x1129FUL && codepoint <= 0x112A8UL)
           || (codepoint >= 0x112B0UL && codepoint <= 0x112DEUL)
           || (codepoint >= 0x11305UL && codepoint <= 0x1130CUL) || codepoint == 0x1130FUL
           || codepoint == 0x11310UL || (codepoint >= 0x11313UL && codepoint <= 0x11328UL)
           || (codepoint >= 0x1132AUL && codepoint <= 0x11330UL) || codepoint == 0x11332UL
           || codepoint == 0x11333UL || (codepoint >= 0x11335UL && codepoint <= 0x11339UL)
           || codepoint == 0x1133DUL || codepoint == 0x11350UL
           || (codepoint >= 0x1135DUL && codepoint <= 0x11361UL)
           || (codepoint >= 0x11480UL && codepoint <= 0x114AFUL) || codepoint == 0x114C4UL
           || codepoint == 0x114C5UL || codepoint == 0x114C7UL
           || (codepoint >= 0x11580UL && codepoint <= 0x115AEUL)
           || (codepoint >= 0x115D8UL && codepoint <= 0x115DBUL)
           || (codepoint >= 0x11600UL && codepoint <= 0x1162FUL) || codepoint == 0x11644UL
           || (codepoint >= 0x11680UL && codepoint <= 0x116AAUL)
           || (codepoint >= 0x11700UL && codepoint <= 0x11719UL) || codepoint == 0x118FFUL
           || (codepoint >= 0x11AC0UL && codepoint <= 0x11AF8UL)
           || (codepoint >= 0x12000UL && codepoint <= 0x12399UL)
           || (codepoint >= 0x12480UL && codepoint <= 0x12543UL)
           || (codepoint >= 0x13000UL && codepoint <= 0x1342EUL)
           || (codepoint >= 0x14400UL && codepoint <= 0x14646UL)
           || (codepoint >= 0x16800UL && codepoint <= 0x16A38UL)
           || (codepoint >= 0x16A40UL && codepoint <= 0x16A5EUL)
           || (codepoint >= 0x16AD0UL && codepoint <= 0x16AEDUL)
           || (codepoint >= 0x16B00UL && codepoint <= 0x16B2FUL)
           || (codepoint >= 0x16B63UL && codepoint <= 0x16B77UL)
           || (codepoint >= 0x16B7DUL && codepoint <= 0x16B8FUL)
           || (codepoint >= 0x16F00UL && codepoint <= 0x16F44UL) || codepoint == 0x16F50UL
           || codepoint == 0x1B000UL || codepoint == 0x1B001UL
           || (codepoint >= 0x1BC00UL && codepoint <= 0x1BC6AUL)
           || (codepoint >= 0x1BC70UL && codepoint <= 0x1BC7CUL)
           || (codepoint >= 0x1BC80UL && codepoint <= 0x1BC88UL)
           || (codepoint >= 0x1BC90UL && codepoint <= 0x1BC99UL)
           || (codepoint >= 0x1E800UL && codepoint <= 0x1E8C4UL)
           || (codepoint >= 0x1EE00UL && codepoint <= 0x1EE03UL)
           || (codepoint >= 0x1EE05UL && codepoint <= 0x1EE1FUL) || codepoint == 0x1EE21UL
           || codepoint == 0x1EE22UL || codepoint == 0x1EE24UL || codepoint == 0x1EE27UL
           || (codepoint >= 0x1EE29UL && codepoint <= 0x1EE32UL)
           || (codepoint >= 0x1EE34UL && codepoint <= 0x1EE37UL)
           || (codepoint >= 0x1EE39UL && codepoint <= 0x1EE3BUL && codepoint % 2 != 0)
           || codepoint == 0x1EE42UL
           || (codepoint >= 0x1EE47UL && codepoint <= 0x1EE4BUL && codepoint % 2 != 0)
           || (codepoint >= 0x1EE4DUL && codepoint <= 0x1EE4FUL) || codepoint == 0x1EE51UL
           || codepoint == 0x1EE52UL || codepoint == 0x1EE54UL
           || (codepoint >= 0x1EE57UL && codepoint <= 0x1EE5FUL && codepoint % 2 != 0)
           || codepoint == 0x1EE61UL || codepoint == 0x1EE62UL || codepoint == 0x1EE64UL
           || (codepoint >= 0x1EE67UL && codepoint <= 0x1EE6AUL)
           || (codepoint >= 0x1EE6CUL && codepoint <= 0x1EE72UL)
           || (codepoint >= 0x1EE74UL && codepoint <= 0x1EE77UL)
           || (codepoint >= 0x1EE79UL && codepoint <= 0x1EE7CUL) || codepoint == 0x1EE7EUL
           || (codepoint >= 0x1EE80UL && codepoint <= 0x1EE89UL)
           || (codepoint >= 0x1EE8BUL && codepoint <= 0x1EE9BUL)
           || (codepoint >= 0x1EEA1UL && codepoint <= 0x1EEA3UL)
           || (codepoint >= 0x1EEA5UL && codepoint <= 0x1EEA9UL)
           || (codepoint >= 0x1EEABUL && codepoint <= 0x1EEBBUL)
           || (codepoint >= 0x20000UL && codepoint <= 0x2A6D6UL)
           || (codepoint >= 0x2A700UL && codepoint <= 0x2B734UL)
           || (codepoint >= 0x2B740UL && codepoint <= 0x2B81DUL)
           || (codepoint >= 0x2B820UL && codepoint <= 0x2CEA1UL)
           || (codepoint >= 0x2F800UL && codepoint <= 0x2FA1DUL);
}
constexpr bool categoryNl(std::uint32_t codepoint) noexcept
{
    return (codepoint >= 0x16EEU && codepoint <= 0x16F0U)
           || (codepoint >= 0x2160U && codepoint <= 0x2182U)
           || (codepoint >= 0x2185U && codepoint <= 0x2188U) || codepoint == 0x3007U
           || (codepoint >= 0x3021U && codepoint <= 0x3029U)
           || (codepoint >= 0x3038U && codepoint <= 0x303AU)
           || (codepoint >= 0xA6E6U && codepoint <= 0xA6EFU)
           || (codepoint >= 0x10140UL && codepoint <= 0x10174UL) || codepoint == 0x10341UL
           || codepoint == 0x1034AUL || (codepoint >= 0x103D1UL && codepoint <= 0x103D5UL)
           || (codepoint >= 0x12400UL && codepoint <= 0x1246EUL);
}
constexpr bool categoryMn(std::uint32_t codepoint) noexcept
{
    return (codepoint >= 0x300U && codepoint <= 0x36FU)
           || (codepoint >= 0x483U && codepoint <= 0x487U)
           || (codepoint >= 0x591U && codepoint <= 0x5BDU) || codepoint == 0x5BFU
           || codepoint == 0x5C1U || codepoint == 0x5C2U || codepoint == 0x5C4U
           || codepoint == 0x5C5U || codepoint == 0x5C7U
           || (codepoint >= 0x610U && codepoint <= 0x61AU)
           || (codepoint >= 0x64BU && codepoint <= 0x65FU) || codepoint == 0x670U
           || (codepoint >= 0x6D6U && codepoint <= 0x6DCU)
           || (codepoint >= 0x6DFU && codepoint <= 0x6E4U) || codepoint == 0x6E7U
           || codepoint == 0x6E8U || (codepoint >= 0x6EAU && codepoint <= 0x6EDU)
           || codepoint == 0x711U || (codepoint >= 0x730U && codepoint <= 0x74AU)
           || (codepoint >= 0x7A6U && codepoint <= 0x7B0U)
           || (codepoint >= 0x7EBU && codepoint <= 0x7F3U)
           || (codepoint >= 0x816U && codepoint <= 0x819U)
           || (codepoint >= 0x81BU && codepoint <= 0x823U)
           || (codepoint >= 0x825U && codepoint <= 0x827U)
           || (codepoint >= 0x829U && codepoint <= 0x82DU)
           || (codepoint >= 0x859U && codepoint <= 0x85BU)
           || (codepoint >= 0x8E3U && codepoint <= 0x902U)
           || (codepoint >= 0x93AU && codepoint <= 0x93CU && codepoint % 2 == 0)
           || (codepoint >= 0x941U && codepoint <= 0x948U) || codepoint == 0x94DU
           || (codepoint >= 0x951U && codepoint <= 0x957U) || codepoint == 0x962U
           || codepoint == 0x963U || codepoint == 0x981U || codepoint == 0x9BCU
           || (codepoint >= 0x9C1U && codepoint <= 0x9C4U) || codepoint == 0x9CDU
           || codepoint == 0x9E2U || codepoint == 0x9E3U || codepoint == 0xA01U
           || codepoint == 0xA02U || codepoint == 0xA3CU || codepoint == 0xA41U
           || codepoint == 0xA42U || codepoint == 0xA47U || codepoint == 0xA48U
           || (codepoint >= 0xA4BU && codepoint <= 0xA4DU) || codepoint == 0xA51U
           || codepoint == 0xA70U || codepoint == 0xA71U || codepoint == 0xA75U
           || codepoint == 0xA81U || codepoint == 0xA82U || codepoint == 0xABCU
           || (codepoint >= 0xAC1U && codepoint <= 0xAC5U) || codepoint == 0xAC7U
           || codepoint == 0xAC8U || codepoint == 0xACDU || codepoint == 0xAE2U
           || codepoint == 0xAE3U || codepoint == 0xB01U || codepoint == 0xB3CU
           || codepoint == 0xB3FU || (codepoint >= 0xB41U && codepoint <= 0xB44U)
           || codepoint == 0xB4DU || codepoint == 0xB56U || codepoint == 0xB62U
           || codepoint == 0xB63U || codepoint == 0xB82U || codepoint == 0xBC0U
           || codepoint == 0xBCDU || codepoint == 0xC00U
           || (codepoint >= 0xC3EU && codepoint <= 0xC40U)
           || (codepoint >= 0xC46U && codepoint <= 0xC48U)
           || (codepoint >= 0xC4AU && codepoint <= 0xC4DU) || codepoint == 0xC55U
           || codepoint == 0xC56U || codepoint == 0xC62U || codepoint == 0xC63U
           || codepoint == 0xC81U || codepoint == 0xCBCU || codepoint == 0xCBFU
           || codepoint == 0xCC6U || codepoint == 0xCCCU || codepoint == 0xCCDU
           || codepoint == 0xCE2U || codepoint == 0xCE3U || codepoint == 0xD01U
           || (codepoint >= 0xD41U && codepoint <= 0xD44U) || codepoint == 0xD4DU
           || codepoint == 0xD62U || codepoint == 0xD63U || codepoint == 0xDCAU
           || (codepoint >= 0xDD2U && codepoint <= 0xDD4U) || codepoint == 0xDD6U
           || codepoint == 0xE31U || (codepoint >= 0xE34U && codepoint <= 0xE3AU)
           || (codepoint >= 0xE47U && codepoint <= 0xE4EU) || codepoint == 0xEB1U
           || (codepoint >= 0xEB4U && codepoint <= 0xEB9U) || codepoint == 0xEBBU
           || codepoint == 0xEBCU || (codepoint >= 0xEC8U && codepoint <= 0xECDU)
           || codepoint == 0xF18U || codepoint == 0xF19U
           || (codepoint >= 0xF35U && codepoint <= 0xF39U && codepoint % 2 != 0)
           || (codepoint >= 0xF71U && codepoint <= 0xF7EU)
           || (codepoint >= 0xF80U && codepoint <= 0xF84U) || codepoint == 0xF86U
           || codepoint == 0xF87U || (codepoint >= 0xF8DU && codepoint <= 0xF97U)
           || (codepoint >= 0xF99U && codepoint <= 0xFBCU) || codepoint == 0xFC6U
           || (codepoint >= 0x102DU && codepoint <= 0x1030U)
           || (codepoint >= 0x1032U && codepoint <= 0x1037U) || codepoint == 0x1039U
           || codepoint == 0x103AU || codepoint == 0x103DU || codepoint == 0x103EU
           || codepoint == 0x1058U || codepoint == 0x1059U
           || (codepoint >= 0x105EU && codepoint <= 0x1060U)
           || (codepoint >= 0x1071U && codepoint <= 0x1074U) || codepoint == 0x1082U
           || codepoint == 0x1085U || codepoint == 0x1086U || codepoint == 0x108DU
           || codepoint == 0x109DU || (codepoint >= 0x135DU && codepoint <= 0x135FU)
           || (codepoint >= 0x1712U && codepoint <= 0x1714U)
           || (codepoint >= 0x1732U && codepoint <= 0x1734U) || codepoint == 0x1752U
           || codepoint == 0x1753U || codepoint == 0x1772U || codepoint == 0x1773U
           || codepoint == 0x17B4U || codepoint == 0x17B5U
           || (codepoint >= 0x17B7U && codepoint <= 0x17BDU) || codepoint == 0x17C6U
           || (codepoint >= 0x17C9U && codepoint <= 0x17D3U) || codepoint == 0x17DDU
           || (codepoint >= 0x180BU && codepoint <= 0x180DU) || codepoint == 0x18A9U
           || (codepoint >= 0x1920U && codepoint <= 0x1922U) || codepoint == 0x1927U
           || codepoint == 0x1928U || codepoint == 0x1932U
           || (codepoint >= 0x1939U && codepoint <= 0x193BU) || codepoint == 0x1A17U
           || codepoint == 0x1A18U || codepoint == 0x1A1BU || codepoint == 0x1A56U
           || (codepoint >= 0x1A58U && codepoint <= 0x1A5EU)
           || (codepoint >= 0x1A60U && codepoint <= 0x1A62U && codepoint % 2 == 0)
           || (codepoint >= 0x1A65U && codepoint <= 0x1A6CU)
           || (codepoint >= 0x1A73U && codepoint <= 0x1A7CU) || codepoint == 0x1A7FU
           || (codepoint >= 0x1AB0U && codepoint <= 0x1ABDU)
           || (codepoint >= 0x1B00U && codepoint <= 0x1B03U) || codepoint == 0x1B34U
           || (codepoint >= 0x1B36U && codepoint <= 0x1B3AU) || codepoint == 0x1B3CU
           || codepoint == 0x1B42U || (codepoint >= 0x1B6BU && codepoint <= 0x1B73U)
           || codepoint == 0x1B80U || codepoint == 0x1B81U
           || (codepoint >= 0x1BA2U && codepoint <= 0x1BA5U) || codepoint == 0x1BA8U
           || codepoint == 0x1BA9U || (codepoint >= 0x1BABU && codepoint <= 0x1BADU)
           || codepoint == 0x1BE6U || codepoint == 0x1BE8U || codepoint == 0x1BE9U
           || codepoint == 0x1BEDU || (codepoint >= 0x1BEFU && codepoint <= 0x1BF1U)
           || (codepoint >= 0x1C2CU && codepoint <= 0x1C33U) || codepoint == 0x1C36U
           || codepoint == 0x1C37U || (codepoint >= 0x1CD0U && codepoint <= 0x1CD2U)
           || (codepoint >= 0x1CD4U && codepoint <= 0x1CE0U)
           || (codepoint >= 0x1CE2U && codepoint <= 0x1CE8U) || codepoint == 0x1CEDU
           || codepoint == 0x1CF4U || codepoint == 0x1CF8U || codepoint == 0x1CF9U
           || (codepoint >= 0x1DC0U && codepoint <= 0x1DF5U)
           || (codepoint >= 0x1DFCU && codepoint <= 0x1DFFU)
           || (codepoint >= 0x20D0U && codepoint <= 0x20DCU) || codepoint == 0x20E1U
           || (codepoint >= 0x20E5U && codepoint <= 0x20F0U)
           || (codepoint >= 0x2CEFU && codepoint <= 0x2CF1U) || codepoint == 0x2D7FU
           || (codepoint >= 0x2DE0U && codepoint <= 0x2DFFU)
           || (codepoint >= 0x302AU && codepoint <= 0x302DU) || codepoint == 0x3099U
           || codepoint == 0x309AU || codepoint == 0xA66FU
           || (codepoint >= 0xA674U && codepoint <= 0xA67DU) || codepoint == 0xA69EU
           || codepoint == 0xA69FU || codepoint == 0xA6F0U || codepoint == 0xA6F1U
           || codepoint == 0xA802U || codepoint == 0xA806U || codepoint == 0xA80BU
           || codepoint == 0xA825U || codepoint == 0xA826U || codepoint == 0xA8C4U
           || (codepoint >= 0xA8E0U && codepoint <= 0xA8F1U)
           || (codepoint >= 0xA926U && codepoint <= 0xA92DU)
           || (codepoint >= 0xA947U && codepoint <= 0xA951U)
           || (codepoint >= 0xA980U && codepoint <= 0xA982U) || codepoint == 0xA9B3U
           || (codepoint >= 0xA9B6U && codepoint <= 0xA9B9U) || codepoint == 0xA9BCU
           || codepoint == 0xA9E5U || (codepoint >= 0xAA29U && codepoint <= 0xAA2EU)
           || codepoint == 0xAA31U || codepoint == 0xAA32U || codepoint == 0xAA35U
           || codepoint == 0xAA36U || codepoint == 0xAA43U || codepoint == 0xAA4CU
           || codepoint == 0xAA7CU || codepoint == 0xAAB0U
           || (codepoint >= 0xAAB2U && codepoint <= 0xAAB4U) || codepoint == 0xAAB7U
           || codepoint == 0xAAB8U || codepoint == 0xAABEU || codepoint == 0xAABFU
           || codepoint == 0xAAC1U || codepoint == 0xAAECU || codepoint == 0xAAEDU
           || codepoint == 0xAAF6U || codepoint == 0xABE5U || codepoint == 0xABE8U
           || codepoint == 0xABEDU || codepoint == 0xFB1EU
           || (codepoint >= 0xFE00U && codepoint <= 0xFE0FU)
           || (codepoint >= 0xFE20U && codepoint <= 0xFE2FU) || codepoint == 0x101FDUL
           || codepoint == 0x102E0UL || (codepoint >= 0x10376UL && codepoint <= 0x1037AUL)
           || (codepoint >= 0x10A01UL && codepoint <= 0x10A03UL) || codepoint == 0x10A05UL
           || codepoint == 0x10A06UL || (codepoint >= 0x10A0CUL && codepoint <= 0x10A0FUL)
           || (codepoint >= 0x10A38UL && codepoint <= 0x10A3AUL) || codepoint == 0x10A3FUL
           || codepoint == 0x10AE5UL || codepoint == 0x10AE6UL || codepoint == 0x11001UL
           || (codepoint >= 0x11038UL && codepoint <= 0x11046UL)
           || (codepoint >= 0x1107FUL && codepoint <= 0x11081UL)
           || (codepoint >= 0x110B3UL && codepoint <= 0x110B6UL) || codepoint == 0x110B9UL
           || codepoint == 0x110BAUL || (codepoint >= 0x11100UL && codepoint <= 0x11102UL)
           || (codepoint >= 0x11127UL && codepoint <= 0x1112BUL)
           || (codepoint >= 0x1112DUL && codepoint <= 0x11134UL) || codepoint == 0x11173UL
           || codepoint == 0x11180UL || codepoint == 0x11181UL
           || (codepoint >= 0x111B6UL && codepoint <= 0x111BEUL)
           || (codepoint >= 0x111CAUL && codepoint <= 0x111CCUL)
           || (codepoint >= 0x1122FUL && codepoint <= 0x11231UL) || codepoint == 0x11234UL
           || codepoint == 0x11236UL || codepoint == 0x11237UL || codepoint == 0x112DFUL
           || (codepoint >= 0x112E3UL && codepoint <= 0x112EAUL) || codepoint == 0x11300UL
           || codepoint == 0x11301UL || codepoint == 0x1133CUL || codepoint == 0x11340UL
           || (codepoint >= 0x11366UL && codepoint <= 0x1136CUL)
           || (codepoint >= 0x11370UL && codepoint <= 0x11374UL)
           || (codepoint >= 0x114B3UL && codepoint <= 0x114B8UL) || codepoint == 0x114BAUL
           || codepoint == 0x114BFUL || codepoint == 0x114C0UL || codepoint == 0x114C2UL
           || codepoint == 0x114C3UL || (codepoint >= 0x115B2UL && codepoint <= 0x115B5UL)
           || codepoint == 0x115BCUL || codepoint == 0x115BDUL || codepoint == 0x115BFUL
           || codepoint == 0x115C0UL || codepoint == 0x115DCUL || codepoint == 0x115DDUL
           || (codepoint >= 0x11633UL && codepoint <= 0x1163AUL) || codepoint == 0x1163DUL
           || codepoint == 0x1163FUL || codepoint == 0x11640UL
           || (codepoint >= 0x116ABUL && codepoint <= 0x116ADUL && codepoint % 2 != 0)
           || (codepoint >= 0x116B0UL && codepoint <= 0x116B5UL) || codepoint == 0x116B7UL
           || (codepoint >= 0x1171DUL && codepoint <= 0x1171FUL)
           || (codepoint >= 0x11722UL && codepoint <= 0x11725UL)
           || (codepoint >= 0x11727UL && codepoint <= 0x1172BUL)
           || (codepoint >= 0x16AF0UL && codepoint <= 0x16AF4UL)
           || (codepoint >= 0x16B30UL && codepoint <= 0x16B36UL)
           || (codepoint >= 0x16F8FUL && codepoint <= 0x16F92UL) || codepoint == 0x1BC9DUL
           || codepoint == 0x1BC9EUL || (codepoint >= 0x1D167UL && codepoint <= 0x1D169UL)
           || (codepoint >= 0x1D17BUL && codepoint <= 0x1D182UL)
           || (codepoint >= 0x1D185UL && codepoint <= 0x1D18BUL)
           || (codepoint >= 0x1D1AAUL && codepoint <= 0x1D1ADUL)
           || (codepoint >= 0x1D242UL && codepoint <= 0x1D244UL)
           || (codepoint >= 0x1DA00UL && codepoint <= 0x1DA36UL)
           || (codepoint >= 0x1DA3BUL && codepoint <= 0x1DA6CUL) || codepoint == 0x1DA75UL
           || codepoint == 0x1DA84UL || (codepoint >= 0x1DA9BUL && codepoint <= 0x1DA9FUL)
           || (codepoint >= 0x1DAA1UL && codepoint <= 0x1DAAFUL)
           || (codepoint >= 0x1E8D0UL && codepoint <= 0x1E8D6UL)
           || (codepoint >= 0xE0100UL && codepoint <= 0xE01EFUL);
}
constexpr bool patternSyntax(std::uint32_t codepoint) noexcept
{
    return (codepoint >= 0x21U && codepoint <= 0x2FU) || (codepoint >= 0x3AU && codepoint <= 0x40U)
           || (codepoint >= 0x5BU && codepoint <= 0x5EU) || codepoint == 0x60U
           || (codepoint >= 0x7BU && codepoint <= 0x7EU)
           || (codepoint >= 0xA1U && codepoint <= 0xA7U) || codepoint == 0xA9U || codepoint == 0xABU
           || codepoint == 0xACU || codepoint == 0xAEU || codepoint == 0xB0U || codepoint == 0xB1U
           || codepoint == 0xB6U || codepoint == 0xBBU || codepoint == 0xBFU || codepoint == 0xD7U
           || codepoint == 0xF7U || (codepoint >= 0x2010U && codepoint <= 0x2027U)
           || (codepoint >= 0x2030U && codepoint <= 0x203EU)
           || (codepoint >= 0x2041U && codepoint <= 0x2053U)
           || (codepoint >= 0x2055U && codepoint <= 0x205EU)
           || (codepoint >= 0x2190U && codepoint <= 0x245FU)
           || (codepoint >= 0x2500U && codepoint <= 0x2775U)
           || (codepoint >= 0x2794U && codepoint <= 0x2BFFU)
           || (codepoint >= 0x2E00U && codepoint <= 0x2E7FU)
           || (codepoint >= 0x3001U && codepoint <= 0x3003U)
           || (codepoint >= 0x3008U && codepoint <= 0x3020U) || codepoint == 0x3030U
           || codepoint == 0xFD3EU || codepoint == 0xFD3FU || codepoint == 0xFE45U
           || codepoint == 0xFE46U;
}
constexpr bool categoryMc(std::uint32_t codepoint) noexcept
{
    return codepoint == 0x903U || codepoint == 0x93BU
           || (codepoint >= 0x93EU && codepoint <= 0x940U)
           || (codepoint >= 0x949U && codepoint <= 0x94CU) || codepoint == 0x94EU
           || codepoint == 0x94FU || codepoint == 0x982U || codepoint == 0x983U
           || (codepoint >= 0x9BEU && codepoint <= 0x9C0U) || codepoint == 0x9C7U
           || codepoint == 0x9C8U || codepoint == 0x9CBU || codepoint == 0x9CCU
           || codepoint == 0x9D7U || codepoint == 0xA03U
           || (codepoint >= 0xA3EU && codepoint <= 0xA40U) || codepoint == 0xA83U
           || (codepoint >= 0xABEU && codepoint <= 0xAC0U) || codepoint == 0xAC9U
           || codepoint == 0xACBU || codepoint == 0xACCU || codepoint == 0xB02U
           || codepoint == 0xB03U
           || (codepoint >= 0xB3EU && codepoint <= 0xB40U && codepoint % 2 == 0)
           || codepoint == 0xB47U || codepoint == 0xB48U || codepoint == 0xB4BU
           || codepoint == 0xB4CU || codepoint == 0xB57U || codepoint == 0xBBEU
           || codepoint == 0xBBFU || codepoint == 0xBC1U || codepoint == 0xBC2U
           || (codepoint >= 0xBC6U && codepoint <= 0xBC8U)
           || (codepoint >= 0xBCAU && codepoint <= 0xBCCU) || codepoint == 0xBD7U
           || (codepoint >= 0xC01U && codepoint <= 0xC03U)
           || (codepoint >= 0xC41U && codepoint <= 0xC44U) || codepoint == 0xC82U
           || codepoint == 0xC83U || codepoint == 0xCBEU
           || (codepoint >= 0xCC0U && codepoint <= 0xCC4U) || codepoint == 0xCC7U
           || codepoint == 0xCC8U || codepoint == 0xCCAU || codepoint == 0xCCBU
           || codepoint == 0xCD5U || codepoint == 0xCD6U || codepoint == 0xD02U
           || codepoint == 0xD03U || (codepoint >= 0xD3EU && codepoint <= 0xD40U)
           || (codepoint >= 0xD46U && codepoint <= 0xD48U)
           || (codepoint >= 0xD4AU && codepoint <= 0xD4CU) || codepoint == 0xD57U
           || codepoint == 0xD82U || codepoint == 0xD83U
           || (codepoint >= 0xDCFU && codepoint <= 0xDD1U)
           || (codepoint >= 0xDD8U && codepoint <= 0xDDFU) || codepoint == 0xDF2U
           || codepoint == 0xDF3U || codepoint == 0xF3EU || codepoint == 0xF3FU
           || codepoint == 0xF7FU || codepoint == 0x102BU || codepoint == 0x102CU
           || codepoint == 0x1031U || codepoint == 0x1038U || codepoint == 0x103BU
           || codepoint == 0x103CU || codepoint == 0x1056U || codepoint == 0x1057U
           || (codepoint >= 0x1062U && codepoint <= 0x1064U)
           || (codepoint >= 0x1067U && codepoint <= 0x106DU) || codepoint == 0x1083U
           || codepoint == 0x1084U || (codepoint >= 0x1087U && codepoint <= 0x108CU)
           || codepoint == 0x108FU || (codepoint >= 0x109AU && codepoint <= 0x109CU)
           || codepoint == 0x17B6U || (codepoint >= 0x17BEU && codepoint <= 0x17C5U)
           || codepoint == 0x17C7U || codepoint == 0x17C8U
           || (codepoint >= 0x1923U && codepoint <= 0x1926U)
           || (codepoint >= 0x1929U && codepoint <= 0x192BU) || codepoint == 0x1930U
           || codepoint == 0x1931U || (codepoint >= 0x1933U && codepoint <= 0x1938U)
           || codepoint == 0x1A19U || codepoint == 0x1A1AU
           || (codepoint >= 0x1A55U && codepoint <= 0x1A57U && codepoint % 2 != 0)
           || codepoint == 0x1A61U || codepoint == 0x1A63U || codepoint == 0x1A64U
           || (codepoint >= 0x1A6DU && codepoint <= 0x1A72U) || codepoint == 0x1B04U
           || codepoint == 0x1B35U || codepoint == 0x1B3BU
           || (codepoint >= 0x1B3DU && codepoint <= 0x1B41U) || codepoint == 0x1B43U
           || codepoint == 0x1B44U || codepoint == 0x1B82U || codepoint == 0x1BA1U
           || codepoint == 0x1BA6U || codepoint == 0x1BA7U || codepoint == 0x1BAAU
           || codepoint == 0x1BE7U || (codepoint >= 0x1BEAU && codepoint <= 0x1BECU)
           || codepoint == 0x1BEEU || codepoint == 0x1BF2U || codepoint == 0x1BF3U
           || (codepoint >= 0x1C24U && codepoint <= 0x1C2BU) || codepoint == 0x1C34U
           || codepoint == 0x1C35U || codepoint == 0x1CE1U || codepoint == 0x1CF2U
           || codepoint == 0x1CF3U || codepoint == 0x302EU || codepoint == 0x302FU
           || codepoint == 0xA823U || codepoint == 0xA824U || codepoint == 0xA827U
           || codepoint == 0xA880U || codepoint == 0xA881U
           || (codepoint >= 0xA8B4U && codepoint <= 0xA8C3U) || codepoint == 0xA952U
           || codepoint == 0xA953U || codepoint == 0xA983U || codepoint == 0xA9B4U
           || codepoint == 0xA9B5U || codepoint == 0xA9BAU || codepoint == 0xA9BBU
           || (codepoint >= 0xA9BDU && codepoint <= 0xA9C0U) || codepoint == 0xAA2FU
           || codepoint == 0xAA30U || codepoint == 0xAA33U || codepoint == 0xAA34U
           || codepoint == 0xAA4DU
           || (codepoint >= 0xAA7BU && codepoint <= 0xAA7DU && codepoint % 2 != 0)
           || codepoint == 0xAAEBU || codepoint == 0xAAEEU || codepoint == 0xAAEFU
           || codepoint == 0xAAF5U || codepoint == 0xABE3U || codepoint == 0xABE4U
           || codepoint == 0xABE6U || codepoint == 0xABE7U || codepoint == 0xABE9U
           || codepoint == 0xABEAU || codepoint == 0xABECU
           || (codepoint >= 0x11000UL && codepoint <= 0x11002UL && codepoint % 2 == 0)
           || codepoint == 0x11082UL || (codepoint >= 0x110B0UL && codepoint <= 0x110B2UL)
           || codepoint == 0x110B7UL || codepoint == 0x110B8UL || codepoint == 0x1112CUL
           || codepoint == 0x11182UL || (codepoint >= 0x111B3UL && codepoint <= 0x111B5UL)
           || codepoint == 0x111BFUL || codepoint == 0x111C0UL
           || (codepoint >= 0x1122CUL && codepoint <= 0x1122EUL) || codepoint == 0x11232UL
           || codepoint == 0x11233UL || codepoint == 0x11235UL
           || (codepoint >= 0x112E0UL && codepoint <= 0x112E2UL) || codepoint == 0x11302UL
           || codepoint == 0x11303UL || codepoint == 0x1133EUL || codepoint == 0x1133FUL
           || (codepoint >= 0x11341UL && codepoint <= 0x11344UL) || codepoint == 0x11347UL
           || codepoint == 0x11348UL || (codepoint >= 0x1134BUL && codepoint <= 0x1134DUL)
           || codepoint == 0x11357UL || codepoint == 0x11362UL || codepoint == 0x11363UL
           || (codepoint >= 0x114B0UL && codepoint <= 0x114B2UL) || codepoint == 0x114B9UL
           || (codepoint >= 0x114BBUL && codepoint <= 0x114BEUL) || codepoint == 0x114C1UL
           || (codepoint >= 0x115AFUL && codepoint <= 0x115B1UL)
           || (codepoint >= 0x115B8UL && codepoint <= 0x115BBUL) || codepoint == 0x115BEUL
           || (codepoint >= 0x11630UL && codepoint <= 0x11632UL) || codepoint == 0x1163BUL
           || codepoint == 0x1163CUL || codepoint == 0x1163EUL || codepoint == 0x116ACUL
           || codepoint == 0x116AEUL || codepoint == 0x116AFUL || codepoint == 0x116B6UL
           || codepoint == 0x11720UL || codepoint == 0x11721UL || codepoint == 0x11726UL
           || (codepoint >= 0x16F51UL && codepoint <= 0x16F7EUL) || codepoint == 0x1D165UL
           || codepoint == 0x1D166UL || (codepoint >= 0x1D16DUL && codepoint <= 0x1D172UL);
}
constexpr bool categoryNd(std::uint32_t codepoint) noexcept
{
    return (codepoint >= 0x30U && codepoint <= 0x39U)
           || (codepoint >= 0x660U && codepoint <= 0x669U)
           || (codepoint >= 0x6F0U && codepoint <= 0x6F9U)
           || (codepoint >= 0x7C0U && codepoint <= 0x7C9U)
           || (codepoint >= 0x966U && codepoint <= 0x96FU)
           || (codepoint >= 0x9E6U && codepoint <= 0x9EFU)
           || (codepoint >= 0xA66U && codepoint <= 0xA6FU)
           || (codepoint >= 0xAE6U && codepoint <= 0xAEFU)
           || (codepoint >= 0xB66U && codepoint <= 0xB6FU)
           || (codepoint >= 0xBE6U && codepoint <= 0xBEFU)
           || (codepoint >= 0xC66U && codepoint <= 0xC6FU)
           || (codepoint >= 0xCE6U && codepoint <= 0xCEFU)
           || (codepoint >= 0xD66U && codepoint <= 0xD6FU)
           || (codepoint >= 0xDE6U && codepoint <= 0xDEFU)
           || (codepoint >= 0xE50U && codepoint <= 0xE59U)
           || (codepoint >= 0xED0U && codepoint <= 0xED9U)
           || (codepoint >= 0xF20U && codepoint <= 0xF29U)
           || (codepoint >= 0x1040U && codepoint <= 0x1049U)
           || (codepoint >= 0x1090U && codepoint <= 0x1099U)
           || (codepoint >= 0x17E0U && codepoint <= 0x17E9U)
           || (codepoint >= 0x1810U && codepoint <= 0x1819U)
           || (codepoint >= 0x1946U && codepoint <= 0x194FU)
           || (codepoint >= 0x19D0U && codepoint <= 0x19D9U)
           || (codepoint >= 0x1A80U && codepoint <= 0x1A89U)
           || (codepoint >= 0x1A90U && codepoint <= 0x1A99U)
           || (codepoint >= 0x1B50U && codepoint <= 0x1B59U)
           || (codepoint >= 0x1BB0U && codepoint <= 0x1BB9U)
           || (codepoint >= 0x1C40U && codepoint <= 0x1C49U)
           || (codepoint >= 0x1C50U && codepoint <= 0x1C59U)
           || (codepoint >= 0xA620U && codepoint <= 0xA629U)
           || (codepoint >= 0xA8D0U && codepoint <= 0xA8D9U)
           || (codepoint >= 0xA900U && codepoint <= 0xA909U)
           || (codepoint >= 0xA9D0U && codepoint <= 0xA9D9U)
           || (codepoint >= 0xA9F0U && codepoint <= 0xA9F9U)
           || (codepoint >= 0xAA50U && codepoint <= 0xAA59U)
           || (codepoint >= 0xABF0U && codepoint <= 0xABF9U)
           || (codepoint >= 0xFF10U && codepoint <= 0xFF19U)
           || (codepoint >= 0x104A0UL && codepoint <= 0x104A9UL)
           || (codepoint >= 0x11066UL && codepoint <= 0x1106FUL)
           || (codepoint >= 0x110F0UL && codepoint <= 0x110F9UL)
           || (codepoint >= 0x11136UL && codepoint <= 0x1113FUL)
           || (codepoint >= 0x111D0UL && codepoint <= 0x111D9UL)
           || (codepoint >= 0x112F0UL && codepoint <= 0x112F9UL)
           || (codepoint >= 0x114D0UL && codepoint <= 0x114D9UL)
           || (codepoint >= 0x11650UL && codepoint <= 0x11659UL)
           || (codepoint >= 0x116C0UL && codepoint <= 0x116C9UL)
           || (codepoint >= 0x11730UL && codepoint <= 0x11739UL)
           || (codepoint >= 0x118E0UL && codepoint <= 0x118E9UL)
           || (codepoint >= 0x16A60UL && codepoint <= 0x16A69UL)
           || (codepoint >= 0x16B50UL && codepoint <= 0x16B59UL)
           || (codepoint >= 0x1D7CEUL && codepoint <= 0x1D7FFUL);
}
constexpr bool categoryPc(std::uint32_t codepoint) noexcept
{
    return codepoint == 0x5FU || codepoint == 0x203FU || codepoint == 0x2040U
           || codepoint == 0x2054U || codepoint == 0xFE33U || codepoint == 0xFE34U
           || (codepoint >= 0xFE4DU && codepoint <= 0xFE4FU) || codepoint == 0xFF3FU;
}
constexpr bool patternWhiteSpace(std::uint32_t codepoint) noexcept
{
    return (codepoint >= 0x9U && codepoint <= 0xDU) || codepoint == 0x20U || codepoint == 0x85U
           || codepoint == 0x200EU || codepoint == 0x200FU || codepoint == 0x2028U
           || codepoint == 0x2029U;
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
constexpr bool idStart(std::uint32_t codepoint) noexcept
{
    return patternSyntax(codepoint) || patternWhiteSpace(codepoint) ?
               false :
               otherIdStart(codepoint) || categoryLu(codepoint) || categoryLl(codepoint)
                   || categoryLt(codepoint) || categoryLm(codepoint) || categoryLo(codepoint)
                   || categoryNl(codepoint);
}
constexpr bool idContinue(std::uint32_t codepoint) noexcept
{
    return patternSyntax(codepoint) || patternWhiteSpace(codepoint) ?
               false :
               idStart(codepoint) || otherIdContinue(codepoint) || categoryMn(codepoint)
                   || categoryMc(codepoint) || categoryNd(codepoint) || categoryPc(codepoint);
}
}
}

#endif /* JAVASCRIPT_TASKLETS_CHARACTER_PROPERTIES_H_ */
