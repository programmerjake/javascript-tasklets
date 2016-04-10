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
#include "variant.h"

#if 0
using namespace javascript_tasklets;
#include <iostream>
#include <typeinfo>
#include <cstdlib>
struct variant_base::test final
{
    template <typename Child>
    struct TestTypeBase
    {
        static const char *getName()
        {
            return Child::name;
        }
        TestTypeBase()
        {
            std::cout << getName() << "(" << this << "): default construct" << std::endl;
        }
        TestTypeBase(const TestTypeBase &rt)
        {
            std::cout << getName() << "(" << this << "): copy construct (" << &rt << ")"
                      << std::endl;
        }
        TestTypeBase(TestTypeBase &&rt)
        {
            std::cout << getName() << "(" << this << "): move construct (" << &rt << ")"
                      << std::endl;
        }
        TestTypeBase &operator=(const TestTypeBase &rt)
        {
            std::cout << getName() << "(" << this << "): copy assign (" << &rt << ")" << std::endl;
            return *this;
        }
        TestTypeBase &operator=(TestTypeBase &&rt)
        {
            std::cout << getName() << "(" << this << "): move assign (" << &rt << ")" << std::endl;
            return *this;
        }
        ~TestTypeBase()
        {
            std::cout << getName() << "(" << this << "): destruct" << std::endl;
        }
    };
    struct TypeA final : public TestTypeBase<TypeA>
    {
        static constexpr auto name = "TypeA";
    };
    struct TypeB final : public TestTypeBase<TypeB>
    {
        static constexpr auto name = "TypeB";
    };
    struct TypeC final : public TestTypeBase<TypeC>
    {
        static constexpr auto name = "TypeC";
    };
    typedef variant<TypeA, TypeB, TypeC> VariantABC;
    typedef variant<TypeB, TypeC> VariantBC;
    void testIdFor()
    {
        std::cout << "\ntestIdFor():" << std::endl;
        std::cout << "VariantABC::idFor<TypeA>() = " << VariantABC::idFor<TypeA>() << std::endl;
        std::cout << "VariantABC::idFor<TypeB>() = " << VariantABC::idFor<TypeB>() << std::endl;
        std::cout << "VariantABC::idFor<TypeC>() = " << VariantABC::idFor<TypeC>() << std::endl;
        std::cout << "VariantABC::idFor<void>() = " << VariantABC::idFor<void>() << std::endl;
    }
    void testGet()
    {
        std::cout << "\ntestGet():" << std::endl;
        VariantABC value = VariantABC::make<TypeA>();
        std::cout << "&VariantABC::get<TypeA>() = " << &value.get<TypeA>() << std::endl;
        value = VariantABC::make<TypeB>();
        std::cout << "&VariantABC::get<TypeB>() = " << &value.get<TypeB>() << std::endl;
        value = VariantABC::make<TypeC>();
        std::cout << "&VariantABC::get<TypeC>() = " << &value.get<TypeC>() << std::endl;
    }
    void testConvertCast()
    {
        std::cout << "\ntestConvertCast():" << std::endl;
        VariantABC value = VariantABC::make<TypeC>();
        VariantBC value2(std::move(value));
    }
    test()
    {
        testIdFor();
        testGet();
        testConvertCast();
    }
};
namespace
{
struct Tester
{
    Tester()
    {
        variant_base::test();
        std::exit(0);
    }
};
Tester tester;
}
#endif
