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
#include "javascript_tasklets/vm/interpreter.h"
#include <iostream>
#include <sstream>

using namespace javascript_tasklets;

namespace test
{
void main()
{
    using namespace value;
    const std::shared_ptr<GC> gcPointer = std::make_shared<GC>();
    GC &gc = *gcPointer;
    HandleScope handleScope(gc);
    try
    {
        try
        {
            gc::LocalLocationGetter locationGetter(gc, u"main");
            ValueHandle value = ObjectHandle::getFunctionPrototype(gc);
            writeString(std::cout,
                        value.invoke(StringHandle(u"toString", gc), {}, gc)
                            .toString(gc)
                            .getValue(gc));
            std::cout << std::endl;
            ObjectHandle::throwTypeError(u"test exception", gc);
        }
        catch(gc::ScriptException &e)
        {
            HandleScope handleScope(gc);
            std::cerr << "exception: " << e.what() << std::endl;
            ValueHandle value = e.value;
            if(value.isObject())
            {
                if(value.getObject().isErrorObject(gc))
                {
                    auto locations = value.getObject().getLocationsIfError(gc);
                    std::cerr << "backtrace:" << std::endl;
                    for(const auto &location : locations.get())
                    {
                        writeString(std::cerr, location.toString());
                        std::cerr << std::endl;
                    }
                }
            }
        }
    }
    catch(gc::ExceptionBase &e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}
}

int main()
{
    test::main();
    return 0;
}
