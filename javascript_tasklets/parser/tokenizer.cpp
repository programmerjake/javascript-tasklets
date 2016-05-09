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
#include "tokenizer.h"
#include "../value.h"
#include "../character_properties.h"

namespace javascript_tasklets
{
namespace parser
{
void Tokenizer::getCurrentCodePoint() noexcept
{
    if(currentState.currentLocation < source.get()->contents.size())
    {
        currentState.peekCodePoint =
            source.get()->contents[currentState.currentLocation] & 0xFFFFUL;
        if(currentState.peekCodePoint >= 0xD800U && currentState.peekCodePoint <= 0xDBFFU
           && currentState.currentLocation + 1 < source.get()->contents.size())
        {
            std::uint32_t nextValue =
                source.get()->contents[currentState.currentLocation + 1] & 0xFFFFUL;
            if(nextValue >= 0xDC00U && nextValue <= 0xDFFFU)
            {
                currentState.peekCodePoint <<= 10;
                currentState.peekCodePoint &= 0xFFC00UL;
                currentState.peekCodePoint |= nextValue & 0x3FFU;
                currentState.peekCodePoint += 0x10000UL;
            }
        }
    }
    else
    {
        currentState.peekCodePoint = eofCodePoint;
    }
}

void Tokenizer::nextCodePoint() noexcept
{
    if(currentState.peekCodePoint == eofCodePoint)
        return;
    if(currentState.peekCodePoint >= 0x10000UL)
        currentState.currentLocation += 2; // surrogate pair
    else
        currentState.currentLocation += 1;
    getCurrentCodePoint();
}

Tokenizer::Tokenizer(const SourceHandle &source) : source(source), currentState(0, 0)
{
    getCurrentCodePoint();
}

Handle<Token> Tokenizer::next(GC &gc)
{
    HandleScope handleScope(gc);
    bool precededByLineTerminator = false;
    while(character_properties::javascriptWhiteSpace(currentState.peekCodePoint)
          || character_properties::javascriptLineTerminator(currentState.peekCodePoint)
          || currentState.peekCodePoint == '/')
    {
        if(currentState.peekCodePoint == '\r')
        {
            precededByLineTerminator = true;
            nextCodePoint();
            if(currentState.peekCodePoint == '\n')
                nextCodePoint();
        }
        else if(character_properties::javascriptLineTerminator(currentState.peekCodePoint))
        {
            precededByLineTerminator = true;
            nextCodePoint();
        }
        else if(currentState.peekCodePoint == '/')
        {
            auto stateBeforeFirstSlash = currentState;
            nextCodePoint();
            if(currentState.peekCodePoint == '/')
            {
                while(
                    currentState.peekCodePoint != eofCodePoint
                    && !character_properties::javascriptLineTerminator(currentState.peekCodePoint))
                {
                    nextCodePoint();
                }
            }
            else if(currentState.peekCodePoint == '*')
            {
                nextCodePoint();
                while(true)
                {
                    if(currentState.peekCodePoint == eofCodePoint)
                    {
                        value::ObjectHandle::throwSyntaxError(
                            u"missing closing */",
                            LocationHandle(gc, Location(source, currentState.currentLocation)),
                            gc);
                        constexpr_assert(false);
                        return Handle<Token>();
                    }
                    if(currentState.peekCodePoint == '*')
                    {
                        while(currentState.peekCodePoint == '*')
                        {
                            nextCodePoint();
                        }
                        if(currentState.peekCodePoint == '/')
                        {
                            nextCodePoint();
                            break;
                        }
                    }
                    else
                    {
                        if(character_properties::javascriptLineTerminator(currentState.peekCodePoint))
                        {
                            precededByLineTerminator = true;
                        }
                        nextCodePoint();
                    }
                }
            }
            else
            {
                currentState = stateBeforeFirstSlash;
                break;
            }
        }
        else
        {
            nextCodePoint();
        }
    }
    Handle<Token> retval;
#error finish
}

Handle<Token> Tokenizer::reparseAsTemplateContinuation(Handle<Token> token, GC &gc)
{
#error finish
}

Handle<Token> Tokenizer::reparseAsRegExp(Handle<Token> token, GC &gc)
{
#error finish
}
}
}
