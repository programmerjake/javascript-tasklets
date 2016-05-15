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
#include <algorithm>

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
                            true,
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
        return handleScope.escapeHandle(
            Handle<Token>(gc,
                          Token(Token::Type::EndOfFile,
                                Location(source, currentState.currentLocation),
                                Handle<gc::StringReference>(),
                                Handle<gc::StringReference>(),
                                precededByLineTerminator)));
    }
    const auto startPosition = currentState.currentLocation;
    if(currentState.peekCodePoint == U'\\'
       || character_properties::javascriptIdStart(currentState.peekCodePoint))
    {
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
                    u"invalid identifier start character from unicode escape sequence",
                    LocationHandle(gc, Location(source, currentState.currentLocation)),
                    false,
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
        while(currentState.peekCodePoint == U'\\'
              || character_properties::javascriptIdContinue(currentState.peekCodePoint))
        {
            if(currentState.peekCodePoint == U'\\')
            {
                gotEscape = true;
                nextCodePoint();
                std::uint32_t codePoint = parseUnicodeEscapeSequence(gc);
                if(!character_properties::javascriptIdContinue(codePoint))
                {
                    value::ObjectHandle::throwSyntaxError(
                        u"invalid identifier continue character from unicode escape sequence",
                        LocationHandle(gc, Location(source, currentState.currentLocation)),
                        false,
                        gc);
                    constexpr_assert(false);
                    return Handle<Token>();
                }
                processedValue = appendCodePoint(std::move(processedValue), codePoint);
            }
            else
            {
                processedValue =
                    appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
                nextCodePoint();
            }
        }
        Location location(source, startPosition, currentState.currentLocation);
        Token::Type type = Token::Type::Identifier;
        if(!gotEscape)
        {
            struct Keyword final
            {
                const char16_t *name;
                Token::Type type;
                bool operator<(const Keyword &rt) const noexcept
                {
                    const char16_t *l = name;
                    const char16_t *r = rt.name;
                    do
                    {
                        if(*l < *r)
                            return true;
                        if(*l > *r)
                            return false;
                        l++;
                        r++;
                    } while(*l && *r);
                    return *l < *r;
                }
            };
            static const Keyword keywords[] = {
                // must be in sorted order
                {u"await", Token::Type::AwaitKW},
                {u"break", Token::Type::BreakKW},
                {u"case", Token::Type::CaseKW},
                {u"catch", Token::Type::CatchKW},
                {u"class", Token::Type::ClassKW},
                {u"const", Token::Type::ConstKW},
                {u"continue", Token::Type::ContinueKW},
                {u"debugger", Token::Type::DebuggerKW},
                {u"default", Token::Type::DefaultKW},
                {u"delete", Token::Type::DeleteKW},
                {u"do", Token::Type::DoKW},
                {u"else", Token::Type::ElseKW},
                {u"enum", Token::Type::EnumKW},
                {u"export", Token::Type::ExportKW},
                {u"extends", Token::Type::ExtendsKW},
                {u"false", Token::Type::FalseLiteral},
                {u"finally", Token::Type::FinallyKW},
                {u"for", Token::Type::ForKW},
                {u"function", Token::Type::FunctionKW},
                {u"if", Token::Type::IfKW},
                {u"import", Token::Type::ImportKW},
                {u"in", Token::Type::InKW},
                {u"instanceof", Token::Type::InstanceOfKW},
                {u"new", Token::Type::NewKW},
                {u"null", Token::Type::NullLiteral},
                {u"return", Token::Type::ReturnKW},
                {u"super", Token::Type::SuperKW},
                {u"switch", Token::Type::SwitchKW},
                {u"this", Token::Type::ThisKW},
                {u"throw", Token::Type::ThrowKW},
                {u"true", Token::Type::TrueLiteral},
                {u"try", Token::Type::TryKW},
                {u"typeof", Token::Type::TypeOfKW},
                {u"var", Token::Type::VarKW},
                {u"void", Token::Type::VoidKW},
                {u"while", Token::Type::WhileKW},
                {u"with", Token::Type::WithKW},
                {u"yield", Token::Type::YieldKW},
            };
            static const char checkIfInSortedOrder =
                (constexpr_assert(std::is_sorted(std::begin(keywords), std::end(keywords))), '\0');
            static_cast<void>(checkIfInSortedOrder);
            auto lowerBound =
                std::lower_bound(std::begin(keywords),
                                 std::end(keywords),
                                 Keyword{processedValue.c_str(), Token::Type::Identifier});
            if(lowerBound != std::end(keywords) && lowerBound->name == processedValue)
            {
                type = lowerBound->type;
            }
        }
        return handleScope.escapeHandle(
            Handle<Token>(gc,
                          Token(type,
                                location,
                                gc.internString(std::move(processedValue)),
                                Handle<gc::StringReference>(),
                                precededByLineTerminator)));
    }
    if(character_properties::javascriptDecimalDigit(currentState.peekCodePoint)
       || currentState.peekCodePoint == U'.')
    {
        bool isPeriod = true;
        if(currentState.peekCodePoint == U'0')
        {
            isPeriod = false;
            nextCodePoint();
            if(currentState.peekCodePoint == U'b'
               || currentState.peekCodePoint == U'B') // binary literal
            {
                nextCodePoint();
                if(!character_properties::javascriptBinaryDigit(currentState.peekCodePoint))
                {
                    value::ObjectHandle::throwSyntaxError(
                        u"missing binary digit in binary literal",
                        LocationHandle(gc, Location(source, currentState.currentLocation)),
                        currentState.peekCodePoint == eofCodePoint,
                        gc);
                    constexpr_assert(false);
                    return Handle<Token>();
                }
                do
                {
                    nextCodePoint();
                } while(character_properties::javascriptBinaryDigit(currentState.peekCodePoint));
                return handleScope.escapeHandle(Handle<Token>(
                    gc,
                    Token(Token::Type::NumericLiteral,
                          Location(source, startPosition, currentState.currentLocation),
                          Handle<gc::StringReference>(),
                          Handle<gc::StringReference>(),
                          precededByLineTerminator)));
            }
            if(currentState.peekCodePoint == U'o'
               || currentState.peekCodePoint == U'O') // octal literal
            {
                nextCodePoint();
                if(!character_properties::javascriptOctalDigit(currentState.peekCodePoint))
                {
                    value::ObjectHandle::throwSyntaxError(
                        u"missing binary digit in octal literal",
                        LocationHandle(gc, Location(source, currentState.currentLocation)),
                        currentState.peekCodePoint == eofCodePoint,
                        gc);
                    constexpr_assert(false);
                    return Handle<Token>();
                }
                do
                {
                    nextCodePoint();
                } while(character_properties::javascriptOctalDigit(currentState.peekCodePoint));
                return handleScope.escapeHandle(Handle<Token>(
                    gc,
                    Token(Token::Type::NumericLiteral,
                          Location(source, startPosition, currentState.currentLocation),
                          Handle<gc::StringReference>(),
                          Handle<gc::StringReference>(),
                          precededByLineTerminator)));
            }
            if(currentState.peekCodePoint == U'x'
               || currentState.peekCodePoint == U'X') // hex literal
            {
                nextCodePoint();
                if(!character_properties::javascriptHexDigit(currentState.peekCodePoint))
                {
                    value::ObjectHandle::throwSyntaxError(
                        u"missing binary digit in hex literal",
                        LocationHandle(gc, Location(source, currentState.currentLocation)),
                        currentState.peekCodePoint == eofCodePoint,
                        gc);
                    constexpr_assert(false);
                    return Handle<Token>();
                }
                do
                {
                    nextCodePoint();
                } while(character_properties::javascriptHexDigit(currentState.peekCodePoint));
                return handleScope.escapeHandle(Handle<Token>(
                    gc,
                    Token(Token::Type::NumericLiteral,
                          Location(source, startPosition, currentState.currentLocation),
                          Handle<gc::StringReference>(),
                          Handle<gc::StringReference>(),
                          precededByLineTerminator)));
            }
        }
        else
        {
            while(character_properties::javascriptDecimalDigit(currentState.peekCodePoint))
            {
                isPeriod = false;
                nextCodePoint();
            }
        }
        if(currentState.peekCodePoint == U'.')
        {
            nextCodePoint();
            while(character_properties::javascriptDecimalDigit(currentState.peekCodePoint))
            {
                isPeriod = false;
                nextCodePoint();
            }
        }
        if(isPeriod)
        {
            if(currentState.peekCodePoint == U'.')
            {
                State savedState = currentState;
                nextCodePoint();
                if(currentState.peekCodePoint == U'.')
                {
                    nextCodePoint();
                    return handleScope.escapeHandle(Handle<Token>(
                        gc,
                        Token(Token::Type::Ellipses,
                              Location(source, startPosition, currentState.currentLocation),
                              Handle<gc::StringReference>(),
                              Handle<gc::StringReference>(),
                              precededByLineTerminator)));
                }
                currentState = savedState;
            }
            return handleScope.escapeHandle(
                Handle<Token>(gc,
                              Token(Token::Type::Period,
                                    Location(source, startPosition, currentState.currentLocation),
                                    Handle<gc::StringReference>(),
                                    Handle<gc::StringReference>(),
                                    precededByLineTerminator)));
        }
        if(character_properties::javascriptExponentIndicator(currentState.peekCodePoint))
        {
            parseExponentPart(gc);
        }
        return handleScope.escapeHandle(
            Handle<Token>(gc,
                          Token(Token::Type::NumericLiteral,
                                Location(source, startPosition, currentState.currentLocation),
                                Handle<gc::StringReference>(),
                                Handle<gc::StringReference>(),
                                precededByLineTerminator)));
    }
    if(currentState.peekCodePoint == U'\"' || currentState.peekCodePoint == U'\'')
    {
        String processedValue;
        std::uint32_t quoteCodePoint = currentState.peekCodePoint;
        nextCodePoint();
        for(;;)
        {
            if(currentState.peekCodePoint == eofCodePoint
               || character_properties::javascriptLineTerminator(currentState.peekCodePoint))
            {
                value::ObjectHandle::throwSyntaxError(
                    String(u"missing closing ") + static_cast<char16_t>(quoteCodePoint)
                        + u" in string literal",
                    LocationHandle(gc,
                                   Location(source, startPosition, currentState.currentLocation)),
                    currentState.peekCodePoint == eofCodePoint,
                    gc);
                constexpr_assert(false);
                return Handle<Token>();
            }
            else if(currentState.peekCodePoint == quoteCodePoint)
            {
                nextCodePoint();
                return handleScope.escapeHandle(Handle<Token>(
                    gc,
                    Token(Token::Type::StringLiteral,
                          Location(source, startPosition, currentState.currentLocation),
                          gc.internString(std::move(processedValue)),
                          Handle<gc::StringReference>(),
                          precededByLineTerminator)));
            }
            else if(currentState.peekCodePoint == U'\\')
            {
                nextCodePoint();
                if(currentState.peekCodePoint == U'\r')
                {
                    nextCodePoint();
                    if(currentState.peekCodePoint == U'\n')
                        nextCodePoint();
                }
                else if(character_properties::javascriptLineTerminator(currentState.peekCodePoint))
                {
                    nextCodePoint();
                }
                else
                {
                    parseEscapeSequence(processedValue, gc);
                }
            }
            else
            {
                processedValue =
                    appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
                nextCodePoint();
            }
        }
    }
    if(currentState.peekCodePoint == U'`')
    {
        String processedValue;
        nextCodePoint();
        for(;;)
        {
            if(currentState.peekCodePoint == eofCodePoint)
            {
                value::ObjectHandle::throwSyntaxError(
                    u"missing closing ` in template",
                    LocationHandle(gc,
                                   Location(source, startPosition, currentState.currentLocation)),
                    true,
                    gc);
                constexpr_assert(false);
                return Handle<Token>();
            }
            else if(currentState.peekCodePoint == U'\r')
            {
                nextCodePoint();
                if(currentState.peekCodePoint == U'\n')
                    nextCodePoint();
                processedValue = appendCodePoint(std::move(processedValue), U'\n');
            }
            else if(character_properties::javascriptLineTerminator(currentState.peekCodePoint))
            {
                processedValue =
                    appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
                nextCodePoint();
            }
            else if(currentState.peekCodePoint == U'$')
            {
                nextCodePoint();
                if(currentState.peekCodePoint == U'{')
                {
                    nextCodePoint();
                    String rawValue;
                    rawValue.reserve((currentState.currentLocation - 2) - (startPosition + 1));
                    for(std::size_t i = startPosition + 1; i < currentState.currentLocation - 2;
                        i++)
                    {
                        if(source.get()->contents[i] == u'\r')
                        {
                            if(i + 1 < currentState.currentLocation - 2
                               && source.get()->contents[i + 1] == u'\n')
                            {
                                i++;
                            }
                            rawValue += u'\n';
                        }
                        else
                            rawValue += source.get()->contents[i];
                    }
                    return handleScope.escapeHandle(Handle<Token>(
                        gc,
                        Token(Token::Type::TemplateHead,
                              Location(source, startPosition, currentState.currentLocation),
                              gc.internString(std::move(processedValue)),
                              gc.internString(std::move(rawValue)),
                              precededByLineTerminator)));
                }
                processedValue = appendCodePoint(std::move(processedValue), U'$');
            }
            else if(currentState.peekCodePoint == U'`')
            {
                nextCodePoint();
                String rawValue;
                rawValue.reserve((currentState.currentLocation - 1) - (startPosition + 1));
                for(std::size_t i = startPosition + 1; i < currentState.currentLocation - 1; i++)
                {
                    if(source.get()->contents[i] == u'\r')
                    {
                        if(i + 1 < currentState.currentLocation - 1
                           && source.get()->contents[i + 1] == u'\n')
                        {
                            i++;
                        }
                        rawValue += u'\n';
                    }
                    else
                        rawValue += source.get()->contents[i];
                }
                return handleScope.escapeHandle(Handle<Token>(
                    gc,
                    Token(Token::Type::NoSubstitutionTemplate,
                          Location(source, startPosition, currentState.currentLocation),
                          gc.internString(std::move(processedValue)),
                          gc.internString(std::move(rawValue)),
                          precededByLineTerminator)));
            }
            else if(currentState.peekCodePoint == U'\\')
            {
                nextCodePoint();
                if(currentState.peekCodePoint == U'\r')
                {
                    nextCodePoint();
                    if(currentState.peekCodePoint == U'\n')
                        nextCodePoint();
                }
                else if(character_properties::javascriptLineTerminator(currentState.peekCodePoint))
                {
                    nextCodePoint();
                }
                else
                {
                    parseEscapeSequence(processedValue, gc);
                }
            }
            else
            {
                processedValue =
                    appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
                nextCodePoint();
            }
        }
    }
    Token::Type type;
    switch(currentState.peekCodePoint)
    {
    case U'{':
        nextCodePoint();
        type = Token::Type::LBrace;
        break;
    case U'}':
        nextCodePoint();
        type = Token::Type::RBrace;
        break;
    case U'(':
        nextCodePoint();
        type = Token::Type::LParen;
        break;
    case U')':
        nextCodePoint();
        type = Token::Type::RParen;
        break;
    case U'[':
        nextCodePoint();
        type = Token::Type::LBracket;
        break;
    case U']':
        nextCodePoint();
        type = Token::Type::RBracket;
        break;
    case U';':
        nextCodePoint();
        type = Token::Type::Semicolon;
        break;
    case U',':
        nextCodePoint();
        type = Token::Type::Comma;
        break;
    case U'<':
        nextCodePoint();
        type = Token::Type::LAngle;
        if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::LAngleEqual;
        }
        else if(currentState.peekCodePoint == U'<')
        {
            nextCodePoint();
            type = Token::Type::LShift;
            if(currentState.peekCodePoint == U'=')
            {
                nextCodePoint();
                type = Token::Type::LShiftEqual;
            }
        }
        break;
    case U'>':
        nextCodePoint();
        type = Token::Type::RAngle;
        if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::RAngleEqual;
        }
        else if(currentState.peekCodePoint == U'>')
        {
            nextCodePoint();
            type = Token::Type::ARShift;
            if(currentState.peekCodePoint == U'=')
            {
                nextCodePoint();
                type = Token::Type::ARShiftEqual;
            }
            else if(currentState.peekCodePoint == U'>')
            {
                nextCodePoint();
                type = Token::Type::URShift;
                if(currentState.peekCodePoint == U'=')
                {
                    nextCodePoint();
                    type = Token::Type::URShiftEqual;
                }
            }
        }
        break;
    case U'=':
        nextCodePoint();
        type = Token::Type::SingleEqual;
        if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::DoubleEqual;
            if(currentState.peekCodePoint == U'=')
            {
                nextCodePoint();
                type = Token::Type::TripleEqual;
            }
        }
        else if(currentState.peekCodePoint == U'>')
        {
            nextCodePoint();
            type = Token::Type::Arrow;
        }
        break;
    case U'!':
        nextCodePoint();
        type = Token::Type::EMark;
        if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::NotEqual;
            if(currentState.peekCodePoint == U'=')
            {
                nextCodePoint();
                type = Token::Type::NotDoubleEqual;
            }
        }
        break;
    case U'+':
        nextCodePoint();
        type = Token::Type::Plus;
        if(currentState.peekCodePoint == U'+')
        {
            nextCodePoint();
            type = Token::Type::Inc;
        }
        else if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::PlusEqual;
        }
        break;
    case U'-':
        nextCodePoint();
        type = Token::Type::Minus;
        if(currentState.peekCodePoint == U'-')
        {
            nextCodePoint();
            type = Token::Type::Dec;
        }
        else if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::MinusEqual;
        }
        break;
    case U'*':
        nextCodePoint();
        type = Token::Type::Star;
        if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::StarEqual;
        }
        break;
    case U'%':
        nextCodePoint();
        type = Token::Type::Percent;
        if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::PercentEqual;
        }
        break;
    case U'&':
        nextCodePoint();
        type = Token::Type::Amp;
        if(currentState.peekCodePoint == U'&')
        {
            nextCodePoint();
            type = Token::Type::LAnd;
        }
        else if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::AmpEqual;
        }
        break;
    case U'|':
        nextCodePoint();
        type = Token::Type::Pipe;
        if(currentState.peekCodePoint == U'|')
        {
            nextCodePoint();
            type = Token::Type::LOr;
        }
        else if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::PipeEqual;
        }
        break;
    case U'^':
        nextCodePoint();
        type = Token::Type::Caret;
        if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::CaretEqual;
        }
        break;
    case U'~':
        nextCodePoint();
        type = Token::Type::Tilde;
        break;
    case U'?':
        nextCodePoint();
        type = Token::Type::QMark;
        break;
    case U':':
        nextCodePoint();
        type = Token::Type::Colon;
        break;
    case U'/':
        nextCodePoint();
        type = Token::Type::Div;
        if(currentState.peekCodePoint == U'=')
        {
            nextCodePoint();
            type = Token::Type::DivEqual;
        }
        break;
    default:
        value::ObjectHandle::throwSyntaxError(
            u"invalid token",
            LocationHandle(gc, Location(source, currentState.currentLocation)),
            false,
            gc);
        constexpr_assert(false);
        return Handle<Token>();
    }
    return handleScope.escapeHandle(
        Handle<Token>(gc,
                      Token(type,
                            Location(source, startPosition, currentState.currentLocation),
                            Handle<gc::StringReference>(),
                            Handle<gc::StringReference>(),
                            precededByLineTerminator)));
}

Handle<Token> Tokenizer::reparseAsTemplateContinuation(Handle<Token> token, GC &gc)
{
    HandleScope handleScope(gc);
    constexpr_assert(token.get().type == Token::Type::RBrace);
    constexpr_assert(token.get().location.source == source.get());
    constexpr_assert(token.get().location.endPosition == currentState.currentLocation);
    constexpr_assert(token.get().location.beginPosition + 1 == currentState.currentLocation);
    constexpr_assert(token.get().processedValue == nullptr);
    constexpr_assert(token.get().supplementaryValue == nullptr);
    constexpr_assert(source.get()->contents[token.get().location.beginPosition] == u'{');
    const auto startPosition = token.get().location.beginPosition;
    String processedValue;
    for(;;)
    {
        if(currentState.peekCodePoint == eofCodePoint)
        {
            value::ObjectHandle::throwSyntaxError(
                u"missing closing ` in template",
                LocationHandle(gc, Location(source, startPosition, currentState.currentLocation)),
                true,
                gc);
            constexpr_assert(false);
            return Handle<Token>();
        }
        else if(currentState.peekCodePoint == U'\r')
        {
            nextCodePoint();
            if(currentState.peekCodePoint == U'\n')
                nextCodePoint();
            processedValue = appendCodePoint(std::move(processedValue), U'\n');
        }
        else if(character_properties::javascriptLineTerminator(currentState.peekCodePoint))
        {
            processedValue = appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
            nextCodePoint();
        }
        else if(currentState.peekCodePoint == U'$')
        {
            nextCodePoint();
            if(currentState.peekCodePoint == U'{')
            {
                nextCodePoint();
                String rawValue;
                rawValue.reserve((currentState.currentLocation - 2) - (startPosition + 1));
                for(std::size_t i = startPosition + 1; i < currentState.currentLocation - 2; i++)
                {
                    if(source.get()->contents[i] == u'\r')
                    {
                        if(i + 1 < currentState.currentLocation - 2
                           && source.get()->contents[i + 1] == u'\n')
                        {
                            i++;
                        }
                        rawValue += u'\n';
                    }
                    else
                        rawValue += source.get()->contents[i];
                }
                return handleScope.escapeHandle(Handle<Token>(
                    gc,
                    Token(Token::Type::TemplateMiddle,
                          Location(source, startPosition, currentState.currentLocation),
                          gc.internString(std::move(processedValue)),
                          gc.internString(std::move(rawValue)),
                          token.get().precededByLineTerminator)));
            }
            processedValue = appendCodePoint(std::move(processedValue), U'$');
        }
        else if(currentState.peekCodePoint == U'`')
        {
            nextCodePoint();
            String rawValue;
            rawValue.reserve((currentState.currentLocation - 1) - (startPosition + 1));
            for(std::size_t i = startPosition + 1; i < currentState.currentLocation - 1; i++)
            {
                if(source.get()->contents[i] == u'\r')
                {
                    if(i + 1 < currentState.currentLocation - 1
                       && source.get()->contents[i + 1] == u'\n')
                    {
                        i++;
                    }
                    rawValue += u'\n';
                }
                else
                    rawValue += source.get()->contents[i];
            }
            return handleScope.escapeHandle(
                Handle<Token>(gc,
                              Token(Token::Type::TemplateTail,
                                    Location(source, startPosition, currentState.currentLocation),
                                    gc.internString(std::move(processedValue)),
                                    gc.internString(std::move(rawValue)),
                                    token.get().precededByLineTerminator)));
        }
        else if(currentState.peekCodePoint == U'\\')
        {
            nextCodePoint();
            if(currentState.peekCodePoint == U'\r')
            {
                nextCodePoint();
                if(currentState.peekCodePoint == U'\n')
                    nextCodePoint();
            }
            else if(character_properties::javascriptLineTerminator(currentState.peekCodePoint))
            {
                nextCodePoint();
            }
            else
            {
                parseEscapeSequence(processedValue, gc);
            }
        }
        else
        {
            processedValue = appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
            nextCodePoint();
        }
    }
    constexpr_assert(false);
    return Handle<Token>();
}

void Tokenizer::parseEscapeSequence(String &processedValue, GC &gc)
{
    switch(currentState.peekCodePoint)
    {
    case U'0':
        nextCodePoint();
        processedValue = appendCodePoint(std::move(processedValue), 0);
        if(character_properties::javascriptDecimalDigit(currentState.peekCodePoint))
        {
            value::ObjectHandle::throwSyntaxError(
                u"invalid escape sequence in string literal",
                LocationHandle(gc, Location(source, currentState.currentLocation)),
                false,
                gc);
            constexpr_assert(false);
            return;
        }
        break;
    case U'u':
    {
        auto codePoint = parseUnicodeEscapeSequence(gc);
        processedValue = appendCodePoint(std::move(processedValue), codePoint);
        break;
    }
    case U'x':
    {
        nextCodePoint();
        if(!character_properties::javascriptHexDigit(currentState.peekCodePoint))
        {
            value::ObjectHandle::throwSyntaxError(
                u"invalid hex escape sequence: missing hex digit",
                LocationHandle(gc, Location(source, currentState.currentLocation)),
                currentState.peekCodePoint == eofCodePoint,
                gc);
            constexpr_assert(false);
            return;
        }
        std::uint32_t codePoint =
            character_properties::javascriptDigitValue(currentState.peekCodePoint);
        nextCodePoint();
        if(!character_properties::javascriptHexDigit(currentState.peekCodePoint))
        {
            value::ObjectHandle::throwSyntaxError(
                u"invalid hex escape sequence: missing hex digit",
                LocationHandle(gc, Location(source, currentState.currentLocation)),
                currentState.peekCodePoint == eofCodePoint,
                gc);
            constexpr_assert(false);
            return;
        }
        codePoint *= 0x10;
        codePoint += character_properties::javascriptDigitValue(currentState.peekCodePoint);
        nextCodePoint();
        processedValue = appendCodePoint(std::move(processedValue), codePoint);
        break;
    }
    case U'\\':
    case U'\"':
    case U'\'':
        processedValue = appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
        nextCodePoint();
        break;
    case U'b':
        processedValue = appendCodePoint(std::move(processedValue), U'\b');
        nextCodePoint();
        break;
    case U't':
        processedValue = appendCodePoint(std::move(processedValue), U'\t');
        nextCodePoint();
        break;
    case U'n':
        processedValue = appendCodePoint(std::move(processedValue), U'\n');
        nextCodePoint();
        break;
    case U'r':
        processedValue = appendCodePoint(std::move(processedValue), U'\r');
        nextCodePoint();
        break;
    case U'f':
        processedValue = appendCodePoint(std::move(processedValue), U'\f');
        nextCodePoint();
        break;
    case U'v':
        processedValue = appendCodePoint(std::move(processedValue), U'\v');
        nextCodePoint();
        break;
    default:
        if(character_properties::javascriptNonEscapeCharacter(currentState.peekCodePoint))
        {
            processedValue = appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
            nextCodePoint();
        }
        else
        {
            value::ObjectHandle::throwSyntaxError(
                u"invalid escape sequence in string literal",
                LocationHandle(gc, Location(source, currentState.currentLocation)),
                currentState.peekCodePoint == eofCodePoint,
                gc);
            constexpr_assert(false);
            return;
        }
    }
}

std::uint32_t Tokenizer::parseUnicodeEscapeSequence(GC &gc)
{
    if(currentState.peekCodePoint != U'u')
    {
        value::ObjectHandle::throwSyntaxError(
            u"invalid unicode escape sequence: missing u",
            LocationHandle(gc, Location(source, currentState.currentLocation)),
            currentState.peekCodePoint == eofCodePoint,
            gc);
        constexpr_assert(false);
        return eofCodePoint;
    }
    nextCodePoint();
    if(currentState.peekCodePoint == U'{')
    {
        nextCodePoint();
        if(!character_properties::javascriptHexDigit(currentState.peekCodePoint))
        {
            value::ObjectHandle::throwSyntaxError(
                u"invalid unicode escape sequence: missing hex digit",
                LocationHandle(gc, Location(source, currentState.currentLocation)),
                currentState.peekCodePoint == eofCodePoint,
                gc);
            constexpr_assert(false);
            return eofCodePoint;
        }
        std::uint32_t value = 0;
        while(character_properties::javascriptHexDigit(currentState.peekCodePoint))
        {
            value *= 0x10;
            value += character_properties::javascriptDigitValue(currentState.peekCodePoint);
            if(value > 0x10FFFFUL)
            {
                value::ObjectHandle::throwSyntaxError(
                    u"invalid unicode escape sequence: value out of range",
                    LocationHandle(gc, Location(source, currentState.currentLocation)),
                    false,
                    gc);
                constexpr_assert(false);
                return eofCodePoint;
            }
            nextCodePoint();
        }
        if(currentState.peekCodePoint != U'}')
        {
            value::ObjectHandle::throwSyntaxError(
                u"invalid unicode escape sequence: missing }",
                LocationHandle(gc, Location(source, currentState.currentLocation)),
                currentState.peekCodePoint == eofCodePoint,
                gc);
            constexpr_assert(false);
            return eofCodePoint;
        }
        nextCodePoint();
        return value;
    }
    std::uint32_t value = 0;
    if(!character_properties::javascriptHexDigit(currentState.peekCodePoint))
    {
        value::ObjectHandle::throwSyntaxError(
            u"invalid unicode escape sequence: missing hex digit or {",
            LocationHandle(gc, Location(source, currentState.currentLocation)),
            currentState.peekCodePoint == eofCodePoint,
            gc);
        constexpr_assert(false);
        return eofCodePoint;
    }
    for(std::size_t i = 0; i < 4; i++)
    {
        if(!character_properties::javascriptHexDigit(currentState.peekCodePoint))
        {
            value::ObjectHandle::throwSyntaxError(
                u"invalid unicode escape sequence: missing hex digit",
                LocationHandle(gc, Location(source, currentState.currentLocation)),
                currentState.peekCodePoint == eofCodePoint,
                gc);
            constexpr_assert(false);
            return eofCodePoint;
        }
        value *= 0x10;
        value += character_properties::javascriptDigitValue(currentState.peekCodePoint);
        nextCodePoint();
    }
    return value;
}

void Tokenizer::parseExponentPart(GC &gc)
{
    constexpr_assert(character_properties::javascriptExponentIndicator(currentState.peekCodePoint));
    nextCodePoint();
    if(currentState.peekCodePoint == U'-' || currentState.peekCodePoint == U'+')
        nextCodePoint();
    if(!character_properties::javascriptDecimalDigit(currentState.peekCodePoint))
    {
        value::ObjectHandle::throwSyntaxError(
            u"invalid numeric literal: missing digit in exponent",
            LocationHandle(gc, Location(source, currentState.currentLocation)),
            currentState.peekCodePoint == eofCodePoint,
            gc);
        constexpr_assert(false);
        return;
    }
    do
    {
        nextCodePoint();
    } while(character_properties::javascriptDecimalDigit(currentState.peekCodePoint));
}

Handle<Token> Tokenizer::reparseAsRegExp(Handle<Token> token, GC &gc)
{
    HandleScope handleScope(gc);
    constexpr_assert(token.get().type == Token::Type::Div
                     || token.get().type == Token::Type::DivEqual);
    constexpr_assert(token.get().location.source == source.get());
    constexpr_assert(token.get().location.endPosition == currentState.currentLocation);
    constexpr_assert(token.get().location.beginPosition
                         + (token.get().type == Token::Type::DivEqual ? 2 : 1)
                     == currentState.currentLocation);
    constexpr_assert(token.get().processedValue == nullptr);
    constexpr_assert(token.get().supplementaryValue == nullptr);
    constexpr_assert(source.get()->contents[token.get().location.beginPosition] == u'/');
    constexpr_assert(token.get().type != Token::Type::DivEqual
                     || source.get()->contents[token.get().location.beginPosition + 1] == u'=');
    const auto startPosition = token.get().location.beginPosition;
    String processedValue;
    if(token.get().type == Token::Type::DivEqual)
        processedValue = u"=";
    else
    {
        constexpr_assert(currentState.peekCodePoint != U'/');
        if(currentState.peekCodePoint == U'*')
        {
            value::ObjectHandle::throwSyntaxError(
                u"invalid regular expression literal: * is first character",
                LocationHandle(gc, Location(source, currentState.currentLocation)),
                false,
                gc);
            constexpr_assert(false);
            return Handle<Token>();
        }
    }
    while(currentState.peekCodePoint != U'/')
    {
        if(currentState.peekCodePoint == eofCodePoint
           || character_properties::javascriptLineTerminator(currentState.peekCodePoint))
        {
            value::ObjectHandle::throwSyntaxError(
                u"invalid regular expression literal: missing closing /",
                LocationHandle(gc, Location(source, currentState.currentLocation)),
                currentState.peekCodePoint == eofCodePoint,
                gc);
            constexpr_assert(false);
            return Handle<Token>();
        }
        if(currentState.peekCodePoint == U'\\')
        {
            processedValue += u'\\';
            nextCodePoint();
            if(currentState.peekCodePoint == eofCodePoint
               || character_properties::javascriptLineTerminator(currentState.peekCodePoint))
            {
                value::ObjectHandle::throwSyntaxError(
                    u"invalid regular expression literal: missing escaped character",
                    LocationHandle(gc, Location(source, currentState.currentLocation)),
                    currentState.peekCodePoint == eofCodePoint,
                    gc);
                constexpr_assert(false);
                return Handle<Token>();
            }
            processedValue = appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
            nextCodePoint();
        }
        else if(currentState.peekCodePoint == U'[')
        {
            processedValue += u'[';
            nextCodePoint();
            while(currentState.peekCodePoint != U']')
            {
                if(currentState.peekCodePoint == eofCodePoint
                   || character_properties::javascriptLineTerminator(currentState.peekCodePoint))
                {
                    value::ObjectHandle::throwSyntaxError(
                        u"invalid regular expression literal: missing closing ]",
                        LocationHandle(gc, Location(source, currentState.currentLocation)),
                        currentState.peekCodePoint == eofCodePoint,
                        gc);
                    constexpr_assert(false);
                    return Handle<Token>();
                }
                if(currentState.peekCodePoint == U'\\')
                {
                    processedValue += u'\\';
                    nextCodePoint();
                    if(currentState.peekCodePoint == eofCodePoint
                       || character_properties::javascriptLineTerminator(
                              currentState.peekCodePoint))
                    {
                        value::ObjectHandle::throwSyntaxError(
                            u"invalid regular expression literal: missing escaped character",
                            LocationHandle(gc, Location(source, currentState.currentLocation)),
                            currentState.peekCodePoint == eofCodePoint,
                            gc);
                        constexpr_assert(false);
                        return Handle<Token>();
                    }
                    processedValue =
                        appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
                    nextCodePoint();
                }
                else
                {
                    processedValue =
                        appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
                    nextCodePoint();
                }
            }
            constexpr_assert(currentState.peekCodePoint == U']');
            processedValue += u']';
            nextCodePoint();
        }
        else
        {
            processedValue = appendCodePoint(std::move(processedValue), currentState.peekCodePoint);
            nextCodePoint();
        }
    }
    constexpr_assert(currentState.peekCodePoint == U'/');
    nextCodePoint();
    String flagsValue = u"";
    while(character_properties::javascriptIdContinue(currentState.peekCodePoint))
    {
        flagsValue = appendCodePoint(std::move(flagsValue), currentState.peekCodePoint);
        nextCodePoint();
    }
    if(currentState.peekCodePoint == U'\\')
    {
        value::ObjectHandle::throwSyntaxError(
            u"escape sequence not allowed in regular expression flags",
            LocationHandle(gc, Location(source, currentState.currentLocation)),
            false,
            gc);
        constexpr_assert(false);
        return Handle<Token>();
    }
    return handleScope.escapeHandle(
        Handle<Token>(gc,
                      Token(Token::Type::RegExpLiteral,
                            Location(source, startPosition, currentState.currentLocation),
                            gc.internString(std::move(processedValue)),
                            gc.internString(std::move(flagsValue)),
                            token.get().precededByLineTerminator)));
}
}
}
