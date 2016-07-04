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
#include "parser.h"
#include "parser_imp.h"
#include "ast/arena.h"
#include <memory>
#include <iostream>

namespace javascript_tasklets
{
namespace parser
{
value::ObjectHandle parseScript(SourceHandle source, GC &gc)
{
    HandleScope handleScope(gc);
    std::u32string u32source;
    const String &u16source = source.get()->contents;
    u32source.reserve(u16source.size());
    for(std::size_t i = 0; i < u16source.size(); i++)
    {
        std::uint16_t ch = u16source[i];
        std::uint_fast32_t value = ch;
        if(ch >= 0xD800U && ch < 0xDC00U && i + 1 < u16source.size() && u16source[i + 1] >= 0xDC00U
           && u16source[i + 1] < 0xC000U)
        {
            value = ch & 0x3FFU;
            value <<= 10;
            value += u16source[++i] & 0x3FFU;
            value += 0x10000UL;
        }
        u32source += static_cast<char32_t>(value);
    }
    ast::Arena arena;
    ast::Node *result;
    try
    {
        Parser parser(std::move(u32source));
#warning finish
        result = parser.parsePrimaryExpression<false, false, true>();
        arena = std::move(parser.arena);
    }
    catch(Parser::ParseError &e)
    {
        value::ObjectHandle::throwSyntaxError(string_cast<String>(std::string(e.message)),
                                              LocationHandle(gc, Location(source, e.location)),
                                              gc);
        return value::ObjectHandle();
    }
    value::ObjectHandle::throwSyntaxError(
        u"parseScript is not implemented", LocationHandle(gc, Location(source, 0)), gc);
    return value::ObjectHandle();
}
}
}
