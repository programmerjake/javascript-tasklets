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
          || currentState.peekCodePoint == U'/')
    {
        if(currentState.peekCodePoint == U'\r')
        {
            precededByLineTerminator = true;
            nextCodePoint();
            if(currentState.peekCodePoint == U'\n')
                nextCodePoint();
        }
        else if(character_properties::javascriptLineTerminator(currentState.peekCodePoint))
        {
            precededByLineTerminator = true;
            nextCodePoint();
        }
        else if(currentState.peekCodePoint == U'/')
        {
            auto stateBeforeFirstSlash = currentState;
            nextCodePoint();
            if(currentState.peekCodePoint == U'/')
            {
                while(
                    currentState.peekCodePoint != eofCodePoint
                    && !character_properties::javascriptLineTerminator(currentState.peekCodePoint))
                {
                    nextCodePoint();
                }
            }
            else if(currentState.peekCodePoint == U'*')
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
                    if(currentState.peekCodePoint == U'*')
                    {
                        while(currentState.peekCodePoint == U'*')
                        {
                            nextCodePoint();
                        }
                        if(currentState.peekCodePoint == U'/')
                        {
                            nextCodePoint();
                            break;
                        }
                    }
                    else
                    {
                        if(character_properties::javascriptLineTerminator(
                               currentState.peekCodePoint))
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
    if(currentState.peekCodePoint == eofCodePoint)
    {
        return Handle<Token>(gc,
                             Token(Token::Type::EndOfFile,
                                   Location(source, currentState.currentLocation),
                                   nullptr,
                                   precededByLineTerminator));
    }
    if(currentState.peekCodePoint == U'\\' || character_properties::javascriptIdStart(currentState.peekCodePoint))
    {
        auto startPosition = currentState.currentLocation;
        String processedValue = u"";
        bool gotEscape = false;
        if(currentState.peekCodePoint == U'\\')
        {
            gotEscape = true;
            nextCodePoint();
            std::uint32_t codePoint = parseUnicodeEscapeSequence(gc);
            if(!character_properties::javascriptIdStart(codePoint))
            {
                value::ObjectHandle::throwSyntaxError(
                    u"invalid identifier start character from unicode escape",
                    LocationHandle(gc, Location(source, currentState.currentLocation)),
                    gc);
                constexpr_assert(false);
                return Handle<Token>();
            }
            processedValue = appendCodePoint(std::move(processedValue), codePoint);
        }
        else
        {
            processedValue = appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
            nextCodePoint();
        }
        while(currentState.peekCodePoint == U'\\' || character_properties::javascriptIdContinue(currentState.peekCodePoint))
        {
            if(currentState.peekCodePoint == U'\\')
            {
                gotEscape = true;
                nextCodePoint();
                std::uint32_t codePoint = parseUnicodeEscapeSequence(gc);
                if(!character_properties::javascriptIdContinue(codePoint))
                {
                    value::ObjectHandle::throwSyntaxError(
                        u"invalid identifier continue character from unicode escape",
                        LocationHandle(gc, Location(source, currentState.currentLocation)),
                        gc);
                    constexpr_assert(false);
                    return Handle<Token>();
                }
                processedValue = appendCodePoint(std::move(processedValue), codePoint);
            }
            else
            {
                processedValue = appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
                nextCodePoint();
            }
        }
        Location location(source, startPosition, currentState.currentLocation);
    }
#error finish
}

Handle<Token> Tokenizer::reparseAsTemplateContinuation(Handle<Token> token, GC &gc)
{
#error finish
}

std::uint32_t Tokenizer::parseUnicodeEscapeSequence(GC &gc)
{
    if(currentState.peekCodePoint != U'u')
    {
        value::ObjectHandle::throwSyntaxError(
            u"invalid identifier continue character from unicode escape",
            LocationHandle(gc, Location(source, currentState.currentLocation)),
            gc);
        constexpr_assert(false);
        return eofCodePoint;
    }
    nextCodePoint();
    if(currentState.peekCodePoint == U'{')
    {
        nextCodePoint();
    }
}

Handle<Token> Tokenizer::reparseAsRegExp(Handle<Token> token, GC &gc)
{
#error finish
}
}
}
