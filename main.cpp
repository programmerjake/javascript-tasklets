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
#include "javascript_tasklets/parser/tokenizer.h"
#include <iostream>
#include <sstream>

using namespace javascript_tasklets;

namespace test
{
const auto testSource =
    uR"(// test
console.log(`this is a test\u{1D7DF}`);
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
            Tokenizer tokenizer(gc.createSource(u"test", testSource));
            for(Handle<Token> token = tokenizer.next(gc);
                token.get().type != Token::Type::EndOfFile;
                token = tokenizer.next(gc))
            {
                writeString(std::cout, token.get().location.toString());
                std::cout << ": '";
                writeString(std::cout, static_cast<String>(token.get().sourceValue()));
                std::cout << "': ";
                writeString(std::cout, Token::getTypeString(token.get().type));
                if(token.get().processedValue != nullptr)
                    writeString(std::cout << ": processedValue = '",
                                gc.readString(Handle<gc::StringReference>(
                                    token, token.get().processedValue))) << "'";
                if(token.get().supplementaryValue != nullptr)
                    writeString(std::cout << ": supplementaryValue = '",
                                gc.readString(Handle<gc::StringReference>(
                                    token, token.get().supplementaryValue))) << "'";
                std::cout << std::endl;
            }
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
