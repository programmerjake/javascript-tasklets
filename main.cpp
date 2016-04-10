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
#include "javascript_tasklets/gc.h"
#include "javascript_tasklets/string.h"
#include <iostream>
#include <sstream>

using namespace javascript_tasklets;

namespace test
{
#if 0
void main()
{

}
#else
gc::Handle<gc::ObjectReference> testFn(gc::GC &gc)
{
    struct MyExtraData final : public gc::Object::ExtraData
    {
        ~MyExtraData()
        {
            std::cout << "destroyed MyExtraData" << std::endl;
        }
        virtual std::unique_ptr<ExtraData> clone() const override
        {
            std::cout << "MyExtraData::clone" << std::endl;
            return std::unique_ptr<MyExtraData>(new MyExtraData(*this));
        }
    };
    gc::HandleScope scope(gc);
    auto objectDescriptor = gc.createObjectDescriptor();
    return scope.escapeHandle(
        gc.create(objectDescriptor, std::unique_ptr<MyExtraData>(new MyExtraData)));
}

void main()
{
    gc::GC gc;
    {
        gc::HandleScope scope(gc);
        std::cout << "before test" << std::endl;
        auto object = testFn(gc);
        gc.addOrChangeObjectMemberDataInObject(
            gc::Handle<gc::Name>(gc.internString("member"_js)), object, true, true, true);
        std::cout << "after test" << std::endl;
        gc.collect();
        std::cout << "after first collect" << std::endl;
    }
    gc.collect();
    std::cout << "after second collect" << std::endl;
}
#endif
}

int main()
{
    test::main();
    return 0;
}
