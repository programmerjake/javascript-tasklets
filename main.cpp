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
#include "javascript_tasklets/value.h"
#include "javascript_tasklets/parser/parser.h"
#include <iostream>
#include <sstream>

using namespace javascript_tasklets;

namespace test
{
const auto testSource =
    uR"(// test
a.true+1 ;
String.raw`abc\u{11000}d$a`;
 /*/
)";
void main()
{
    using namespace value;
    using namespace parser;
    const std::shared_ptr<GC> gcPointer = std::make_shared<GC>();
    GC &gc = *gcPointer;
    HandleScope handleScope(gc);
    gc::LocalLocationGetter locationGetter(gc, u"main");
    try
    {
        try
        {
            auto source = gc.createSource(u"builtin:testSource.js", testSource);
#if 0
            auto value =
                parser::parseScript(source, gc)
                    .call(ObjectHandle::getGlobalObject(gc), {}, gc)
                    .toString(gc);
            writeString(std::cout, value.getValue(gc));
            std::cout << std::endl;
#else
#warning testing parser
            parser::testParse(source, gc);
#endif
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
