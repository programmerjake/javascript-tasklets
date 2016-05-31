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

#include "parser_imp.h"
#include "../vm/interpreter.h"
#include "../constexpr_assert.h"
#include "../character_properties.h"
#include <deque>
#include <iostream>
#include <sstream>

namespace javascript_tasklets
{
namespace parser
{
using namespace vm::interpreter;
struct CodeEmitter final : public gc::Object::ExtraData
{
    std::vector<Instruction> instructions;
    std::size_t registerCount;
    CodeEmitter() : instructions(), registerCount(0)
    {
    }
    virtual std::unique_ptr<ExtraData> clone() const override final
    {
        constexpr_assert(!"CodeEmitter::clone called");
        return std::unique_ptr<ExtraData>();
    }
    virtual void getGCReferences(gc::GCReferencesCallback &callback) const override
    {
        callback(instructions);
    }
    value::ObjectHandle finishGlobalCode(const Location &location, GC &gc)
    {
        HandleScope handleScope(gc);
        instructions.push_back(Instruction::makeReturnUndefined(location));
        Handle<std::vector<Instruction>> newInstructions(gc, instructions);
        return handleScope.escapeHandle(value::ObjectHandle::createBuiltinFunction(
            std::unique_ptr<vm::Code>(
                new FunctionCode(std::move(newInstructions.get()), registerCount, 0, gc)),
            0,
            u"file: '" + location.source->fileName + u"'",
            value::ObjectHandle::FunctionKind::NonConstructor,
            value::ObjectHandle::ConstructorKind::Base,
            gc));
    }
    struct Value final
    {
        enum class Type
        {
            Empty,
            NonstrictObjectReference,
            StrictObjectReference,
            NonstrictEnvironmentReference,
            StrictEnvironmentReference,
            NonstrictSuperReference,
            StrictSuperReference,
            Value,
        };
        Type type;
        RegisterIndex baseOrValue;
        RegisterIndex referencedName;
        RegisterIndex thisValue;
        constexpr Value() noexcept : type(Type::Empty), baseOrValue(), referencedName(), thisValue()
        {
        }
        constexpr Value(Type type,
                        RegisterIndex baseOrValue,
                        RegisterIndex referencedName,
                        RegisterIndex thisValue) noexcept : type(type),
                                                            baseOrValue(baseOrValue),
                                                            referencedName(referencedName),
                                                            thisValue(thisValue)
        {
        }
        static constexpr Value makeValue(RegisterIndex value)
        {
            return Value(Type::Value, value, RegisterIndex(), RegisterIndex());
        }
        static constexpr Value makeNonstrictObjectReference(RegisterIndex object,
                                                            RegisterIndex referencedName)
        {
            return Value(Type::NonstrictObjectReference, object, referencedName, RegisterIndex());
        }
        static constexpr Value makeStrictObjectReference(RegisterIndex object,
                                                         RegisterIndex referencedName)
        {
            return Value(Type::StrictObjectReference, object, referencedName, RegisterIndex());
        }
        static constexpr Value makeObjectReference(RegisterIndex object,
                                                   RegisterIndex referencedName,
                                                   bool strict)
        {
            return Value(strict ? Type::StrictObjectReference : Type::NonstrictObjectReference,
                         object,
                         referencedName,
                         RegisterIndex());
        }
        static constexpr Value makeNonstrictEnvironmentReference(RegisterIndex environment,
                                                                 RegisterIndex referencedName)
        {
            return Value(
                Type::NonstrictEnvironmentReference, environment, referencedName, RegisterIndex());
        }
        static constexpr Value makeStrictEnvironmentReference(RegisterIndex environment,
                                                              RegisterIndex referencedName)
        {
            return Value(
                Type::StrictEnvironmentReference, environment, referencedName, RegisterIndex());
        }
        static constexpr Value makeEnvironmentReference(RegisterIndex environment,
                                                        RegisterIndex referencedName,
                                                        bool strict)
        {
            return Value(
                strict ? Type::StrictEnvironmentReference : Type::NonstrictEnvironmentReference,
                environment,
                referencedName,
                RegisterIndex());
        }
        static constexpr Value makeNonstrictSuperReference(RegisterIndex base,
                                                           RegisterIndex referencedName,
                                                           RegisterIndex thisValue)
        {
            return Value(Type::NonstrictSuperReference, base, referencedName, thisValue);
        }
        static constexpr Value makeStrictSuperReference(RegisterIndex base,
                                                        RegisterIndex referencedName,
                                                        RegisterIndex thisValue)
        {
            return Value(Type::StrictSuperReference, base, referencedName, thisValue);
        }
        static constexpr Value makeSuperReference(RegisterIndex base,
                                                  RegisterIndex referencedName,
                                                  RegisterIndex thisValue,
                                                  bool strict)
        {
            return Value(strict ? Type::StrictSuperReference : Type::NonstrictSuperReference,
                         base,
                         referencedName,
                         thisValue);
        }
        constexpr bool isEmpty() const noexcept
        {
            return type == Type::Empty;
        }
        constexpr bool isReference() const noexcept
        {
            return type == Type::NonstrictObjectReference || type == Type::StrictObjectReference
                   || type == Type::NonstrictEnvironmentReference
                   || type == Type::StrictEnvironmentReference
                   || type == Type::NonstrictSuperReference || type == Type::StrictSuperReference;
        }
        constexpr bool isObjectReference() const noexcept
        {
            return type == Type::NonstrictObjectReference || type == Type::StrictObjectReference;
        }
        constexpr bool isEnvironmentReference() const noexcept
        {
            return type == Type::NonstrictEnvironmentReference
                   || type == Type::StrictEnvironmentReference;
        }
        constexpr bool isSuperReference() const noexcept
        {
            return type == Type::NonstrictSuperReference || type == Type::StrictSuperReference;
        }
        constexpr bool isStrict() const noexcept
        {
            return type == Type::StrictObjectReference || type == Type::StrictEnvironmentReference
                   || type == Type::StrictSuperReference;
        }
        constexpr bool isValue() const noexcept
        {
            return type == Type::Value;
        }
    };
    std::vector<Value> valueStack;
    void valueStackMerge()
    {
        constexpr_assert(valueStack.size() >= 2);
        if(valueStack.back().isEmpty())
            valueStack.pop_back();
        else
        {
            Value value = std::move(valueStack.back());
            valueStack.pop_back();
            valueStack.back() = std::move(value);
        }
    }
    void valueStackPushEmpty()
    {
        valueStack.emplace_back();
    }
    void valueStackPop()
    {
        constexpr_assert(valueStack.size() >= 1);
        valueStack.pop_back();
    }
    void valueStackGetValue()
    {
        constexpr_assert(valueStack.size() >= 1);
        if(!valueStack.back().isReference())
            return;
        constexpr_assert(false);
#warning finish
    }
};

struct ASTTranslator final : public gc::Object::ExtraData
{
    CodeEmitter &codeEmitter;
    explicit ASTTranslator(CodeEmitter &codeEmitter) : codeEmitter(codeEmitter)
    {
    }
    virtual std::unique_ptr<ExtraData> clone() const override final
    {
        constexpr_assert(!"ASTTranslator::clone called");
        return std::unique_ptr<ExtraData>();
    }
    virtual void getGCReferences(gc::GCReferencesCallback &callback) const override
    {
    }
};

Parser::Parser(ASTTranslator &astTranslator, SourceHandle source)
    : astTranslator(astTranslator),
      ruleStatusesArray(source.get()->contents.size() + 1), // one extra for EndOfFile
      source(source.get()),
      currentPosition(0)
{
}

std::unique_ptr<gc::Object::ExtraData> Parser::clone() const
{
    constexpr_assert(!"Parser::clone called");
    return std::unique_ptr<ExtraData>();
}

void Parser::getGCReferences(gc::GCReferencesCallback &callback) const
{
    for(const auto &ruleStatuses : ruleStatusesArray)
    {
        if(ruleStatuses)
        {
            ruleStatuses->getGCReferences(callback);
        }
    }
    callback(source);
}

Parser::RuleStatuses &Parser::getRuleStatuses(std::size_t position)
{
    constexpr_assert(position < ruleStatusesArray.size());
    auto &retval = ruleStatusesArray[position];
    if(!retval)
        retval = std::unique_ptr<RuleStatuses>(new RuleStatuses);
    return *retval;
}

std::uint32_t Parser::getCodePoint(std::size_t position)
{
    if(position >= source->contents.size())
    {
        return eofCodePoint;
    }
    std::uint32_t retval = 0xFFFFU & source->contents[position];
    if(retval >= 0xD800U && retval <= 0xDBFFU && position + 1 < source->contents.size())
    {
        std::uint32_t nextValue = source->contents[position + 1] & 0xFFFFUL;
        if(nextValue >= 0xDC00U && nextValue <= 0xDFFFU)
        {
            retval <<= 10;
            retval &= 0xFFC00UL;
            retval |= nextValue & 0x3FFU;
            retval += 0x10000UL;
            return retval;
        }
    }
    return retval;
}

std::uint32_t Parser::getCodePoint(std::size_t position, std::size_t &nextPosition)
{
    if(position >= source->contents.size())
    {
        nextPosition = position;
        return eofCodePoint;
    }
    std::uint32_t retval = 0xFFFFU & source->contents[position];
    if(retval >= 0xD800U && retval <= 0xDBFFU && position + 1 < source->contents.size())
    {
        std::uint32_t nextValue = source->contents[position + 1] & 0xFFFFUL;
        if(nextValue >= 0xDC00U && nextValue <= 0xDFFFU)
        {
            retval <<= 10;
            retval &= 0xFFC00UL;
            retval |= nextValue & 0x3FFU;
            retval += 0x10000UL;
            nextPosition = position + 2;
            return retval;
        }
    }
    nextPosition = position + 1;
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationLineTerminatorSequence(GC &gc)
{
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint == U'\r')
    {
        currentPosition = nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'\n')
            currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    }
    if(character_properties::javascriptLineTerminator(codePoint))
    {
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    }
    return RuleStatus::makeFailure(
        startPosition, startPosition, u"missing line terminator sequence");
}

Parser::RuleStatus Parser::parseTokenizationSeperator(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationSeperatorStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    bool gotNewLine = false;
    while(codePoint == U'/' || character_properties::javascriptWhiteSpace(codePoint)
          || character_properties::javascriptLineTerminator(codePoint))
    {
        if(codePoint == U'/')
        {
            auto slashStartPosition = currentPosition;
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(codePoint == U'/')
            {
                while(codePoint != eofCodePoint
                      && !character_properties::javascriptLineTerminator(codePoint))
                {
                    currentPosition = nextPosition;
                    codePoint = getCodePoint(currentPosition, nextPosition);
                }
            }
            else if(codePoint == U'*')
            {
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
                while(true)
                {
                    if(codePoint == U'*')
                    {
                        while(codePoint == U'*')
                        {
                            currentPosition = nextPosition;
                            codePoint = getCodePoint(currentPosition, nextPosition);
                        }
                        if(codePoint == U'/')
                        {
                            currentPosition = nextPosition;
                            codePoint = getCodePoint(currentPosition, nextPosition);
                            break;
                        }
                    }
                    else if(codePoint == eofCodePoint)
                    {
                        retval = RuleStatus::makeFailure(
                            slashStartPosition, currentPosition, u"comment missing closing */");
                        currentPosition = startPosition;
                        return retval;
                    }
                    else if(character_properties::javascriptLineTerminator(codePoint))
                    {
                        gotNewLine = true;
                        currentPosition = nextPosition;
                        codePoint = getCodePoint(currentPosition, nextPosition);
                    }
                    else
                    {
                        currentPosition = nextPosition;
                        codePoint = getCodePoint(currentPosition, nextPosition);
                    }
                }
            }
            else
            {
                currentPosition = slashStartPosition;
                break;
            }
        }
        else if(character_properties::javascriptLineTerminator(codePoint))
        {
            gotNewLine = true;
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
        else
        {
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
    }
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    statuses.lineSplit = gotNewLine;
    getRuleStatuses(currentPosition).lineSplit = gotNewLine;
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationUnicodeEscapeSequence(GC &gc, std::uint32_t &value)
{
    std::size_t startPosition = currentPosition;
    if(getCodePoint(currentPosition, currentPosition) != U'u')
    {
        currentPosition = startPosition;
        return RuleStatus::makeFailure(
            startPosition, startPosition, u"unicode escape sequence is missing u");
    }
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint == U'{')
    {
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(!character_properties::javascriptHexDigit(codePoint))
        {
            auto errorPosition = currentPosition;
            currentPosition = startPosition;
            return RuleStatus::makeFailure(
                errorPosition, errorPosition, u"unicode escape sequence missing hex digit");
        }
        value = 0;
        while(character_properties::javascriptHexDigit(codePoint))
        {
            value *= 0x10;
            value += character_properties::javascriptDigitValue(codePoint);
            if(value > 0x10FFFFUL)
            {
                auto errorPosition = currentPosition;
                currentPosition = startPosition;
                return RuleStatus::makeFailure(
                    errorPosition, errorPosition, u"unicode escape sequence value out of range");
            }
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
        if(codePoint != U'}')
        {
            auto errorPosition = currentPosition;
            currentPosition = startPosition;
            return RuleStatus::makeFailure(
                errorPosition, errorPosition, u"unicode escape sequence missing closing }");
        }
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    }
    else if(character_properties::javascriptHexDigit(codePoint))
    {
        value = 0;
        for(std::size_t i = 0; i < 4; i++)
        {
            if(!character_properties::javascriptHexDigit(codePoint))
            {
                auto errorPosition = currentPosition;
                currentPosition = startPosition;
                return RuleStatus::makeFailure(
                    errorPosition, errorPosition, u"unicode escape sequence missing hex digit");
            }
            value *= 0x10;
            value += character_properties::javascriptDigitValue(codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    }
    else
    {
        auto errorPosition = currentPosition;
        currentPosition = startPosition;
        return RuleStatus::makeFailure(
            errorPosition, errorPosition, u"unicode escape sequence is missing { or a hex digit");
    }
}

Parser::RuleStatus Parser::parseTokenizationHexEscapeSequence(GC &gc, std::uint32_t &value)
{
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'x')
    {
        auto errorPosition = startPosition;
        currentPosition = startPosition;
        return RuleStatus::makeFailure(
            errorPosition, errorPosition, u"hex escape sequence is missing x");
    }
    currentPosition = nextPosition;
    codePoint = getCodePoint(currentPosition, nextPosition);
    value = 0;
    for(std::size_t i = 0; i < 2; i++)
    {
        if(!character_properties::javascriptHexDigit(codePoint))
        {
            auto errorPosition = currentPosition;
            currentPosition = startPosition;
            return RuleStatus::makeFailure(
                errorPosition, errorPosition, u"hex escape sequence missing hex digit");
        }
        value *= 0x10;
        value += character_properties::javascriptDigitValue(codePoint);
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
    }
    return RuleStatus::makeSuccess(startPosition, currentPosition);
}

Parser::RuleStatus Parser::parseTokenizationEscapeSequence(GC &gc, std::uint32_t &value)
{
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    switch(codePoint)
    {
    case U'u':
        return parseTokenizationUnicodeEscapeSequence(gc, value);
    case U'x':
        return parseTokenizationHexEscapeSequence(gc, value);
    case U'b':
        value = U'\b';
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'f':
        value = U'\f';
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'n':
        value = U'\n';
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'r':
        value = U'\r';
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U't':
        value = U'\t';
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'v':
        value = U'\v';
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'0':
        value = U'\0';
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(character_properties::javascriptDecimalDigit(codePoint))
        {
            auto errorPriorityPosition = currentPosition;
            currentPosition = startPosition;
            return RuleStatus::makeFailure(
                startPosition, errorPriorityPosition, u"invalid escape sequence");
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    default:
        if(character_properties::javascriptLineTerminator(codePoint)
           || character_properties::javascriptDecimalDigit(codePoint))
        {
            auto errorPriorityPosition = currentPosition;
            currentPosition = startPosition;
            return RuleStatus::makeFailure(
                startPosition, errorPriorityPosition, u"invalid escape sequence");
        }
        value = codePoint;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    }
}

Parser::RuleStatus Parser::parseTokenizationIdentifierName(GC &gc)
{
    HandleScope handleScope(gc);
    auto &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationIdentifierNameStatus;
    if(!retval.empty())
    {
        constexpr_assert(!statuses.tokenizationEscapelessIdentifierNameStatus.empty());
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    constexpr_assert(statuses.tokenizationEscapelessIdentifierNameStatus.empty());
    constexpr_assert(statuses.identifierNameValue == nullptr);
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint == U'\\')
    {
        statuses.tokenizationEscapelessIdentifierNameStatus =
            RuleStatus::makeFailure(currentPosition,
                                    currentPosition,
                                    u"unicode escape sequence in identifier not allowed here");
        currentPosition = nextPosition;
        retval = parseTokenizationUnicodeEscapeSequence(gc, codePoint);
        if(retval.fail())
        {
            return retval;
        }
        if(!character_properties::javascriptIdStart(codePoint))
        {
            retval = RuleStatus::makeFailure(
                startPosition,
                retval.endPositionOrErrorPriorityPosition,
                u"invalid identifier start character from unicode escape sequence");
            return retval;
        }
    }
    else if(!character_properties::javascriptIdStart(codePoint))
    {
        retval = RuleStatus::makeFailure(
            startPosition, startPosition, u"invalid identifier start character");
        statuses.tokenizationEscapelessIdentifierNameStatus = static_cast<RuleStatus>(retval);
        return retval;
    }
    currentPosition = nextPosition;
    String identifierValue;
    identifierValue = appendCodePoint(std::move(identifierValue), codePoint);
    codePoint = getCodePoint(currentPosition, nextPosition);
    while(character_properties::javascriptIdContinue(codePoint) || codePoint == U'\\')
    {
        if(codePoint == U'\\')
        {
            if(statuses.tokenizationEscapelessIdentifierNameStatus.empty())
                statuses.tokenizationEscapelessIdentifierNameStatus = RuleStatus::makeFailure(
                    currentPosition,
                    currentPosition,
                    u"unicode escape sequence in identifier not allowed here");
            currentPosition = nextPosition;
            retval = parseTokenizationUnicodeEscapeSequence(gc, codePoint);
            if(retval.fail())
            {
                return retval;
            }
            if(!character_properties::javascriptIdContinue(codePoint))
            {
                retval = RuleStatus::makeFailure(
                    currentPosition,
                    retval.endPositionOrErrorPriorityPosition,
                    u"invalid identifier character from unicode escape sequence");
                return retval;
            }
        }
        currentPosition = nextPosition;
        identifierValue = appendCodePoint(std::move(identifierValue), codePoint);
        codePoint = getCodePoint(currentPosition, nextPosition);
    }
    statuses.identifierNameValue = gc.internString(std::move(identifierValue)).get();
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    if(statuses.tokenizationEscapelessIdentifierNameStatus.empty())
        statuses.tokenizationEscapelessIdentifierNameStatus = static_cast<RuleStatus>(retval);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationEscapelessIdentifierName(GC &gc)
{
    std::size_t startPosition = currentPosition;
    parseTokenizationIdentifierName(gc);
    auto retval = getRuleStatuses(startPosition).tokenizationEscapelessIdentifierNameStatus;
    if(retval.fail())
        currentPosition = startPosition;
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationKeyword(GC &gc,
                                                    const char16_t *text,
                                                    const char16_t *errorMessage)
{
    HandleScope handleScope(gc);
    std::size_t startPosition = currentPosition;
    RuleStatus retval = parseTokenizationEscapelessIdentifierName(gc);
    if(retval.fail()
       || gc.readString(Handle<gc::StringReference>(
              gc, getRuleStatuses(startPosition).identifierNameValue)) != text)
    {
        retval = RuleStatus::makeFailure(retval.startPositionOrErrorPosition,
                                         retval.endPositionOrErrorPriorityPosition,
                                         errorMessage);
        currentPosition = startPosition;
    }
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationAwait(GC &gc)
{
    return parseTokenizationKeyword(gc, u"await", u"missing await keyword");
}

Parser::RuleStatus Parser::parseTokenizationBreak(GC &gc)
{
    return parseTokenizationKeyword(gc, u"break", u"missing break keyword");
}

Parser::RuleStatus Parser::parseTokenizationCase(GC &gc)
{
    return parseTokenizationKeyword(gc, u"case", u"missing case keyword");
}

Parser::RuleStatus Parser::parseTokenizationCatch(GC &gc)
{
    return parseTokenizationKeyword(gc, u"catch", u"missing catch keyword");
}

Parser::RuleStatus Parser::parseTokenizationClass(GC &gc)
{
    return parseTokenizationKeyword(gc, u"class", u"missing class keyword");
}

Parser::RuleStatus Parser::parseTokenizationConst(GC &gc)
{
    return parseTokenizationKeyword(gc, u"const", u"missing const keyword");
}

Parser::RuleStatus Parser::parseTokenizationContinue(GC &gc)
{
    return parseTokenizationKeyword(gc, u"continue", u"missing continue keyword");
}

Parser::RuleStatus Parser::parseTokenizationDebugger(GC &gc)
{
    return parseTokenizationKeyword(gc, u"debugger", u"missing debugger keyword");
}

Parser::RuleStatus Parser::parseTokenizationDefault(GC &gc)
{
    return parseTokenizationKeyword(gc, u"default", u"missing default keyword");
}

Parser::RuleStatus Parser::parseTokenizationDelete(GC &gc)
{
    return parseTokenizationKeyword(gc, u"delete", u"missing delete keyword");
}

Parser::RuleStatus Parser::parseTokenizationDo(GC &gc)
{
    return parseTokenizationKeyword(gc, u"do", u"missing do keyword");
}

Parser::RuleStatus Parser::parseTokenizationElse(GC &gc)
{
    return parseTokenizationKeyword(gc, u"else", u"missing else keyword");
}

Parser::RuleStatus Parser::parseTokenizationEnum(GC &gc)
{
    return parseTokenizationKeyword(gc, u"enum", u"missing enum keyword");
}

Parser::RuleStatus Parser::parseTokenizationExport(GC &gc)
{
    return parseTokenizationKeyword(gc, u"export", u"missing export keyword");
}

Parser::RuleStatus Parser::parseTokenizationExtends(GC &gc)
{
    return parseTokenizationKeyword(gc, u"extends", u"missing extends keyword");
}

Parser::RuleStatus Parser::parseTokenizationFalse(GC &gc)
{
    return parseTokenizationKeyword(gc, u"false", u"missing false keyword");
}

Parser::RuleStatus Parser::parseTokenizationFinally(GC &gc)
{
    return parseTokenizationKeyword(gc, u"finally", u"missing finally keyword");
}

Parser::RuleStatus Parser::parseTokenizationFor(GC &gc)
{
    return parseTokenizationKeyword(gc, u"for", u"missing for keyword");
}

Parser::RuleStatus Parser::parseTokenizationFunction(GC &gc)
{
    return parseTokenizationKeyword(gc, u"function", u"missing function keyword");
}

Parser::RuleStatus Parser::parseTokenizationIf(GC &gc)
{
    return parseTokenizationKeyword(gc, u"if", u"missing if keyword");
}

Parser::RuleStatus Parser::parseTokenizationImport(GC &gc)
{
    return parseTokenizationKeyword(gc, u"import", u"missing import keyword");
}

Parser::RuleStatus Parser::parseTokenizationIn(GC &gc)
{
    return parseTokenizationKeyword(gc, u"in", u"missing in keyword");
}

Parser::RuleStatus Parser::parseTokenizationInstanceOf(GC &gc)
{
    return parseTokenizationKeyword(gc, u"instanceof", u"missing instanceof keyword");
}

Parser::RuleStatus Parser::parseTokenizationLet(GC &gc)
{
    return parseTokenizationKeyword(gc, u"let", u"missing let");
}

Parser::RuleStatus Parser::parseTokenizationNew(GC &gc)
{
    return parseTokenizationKeyword(gc, u"new", u"missing new keyword");
}

Parser::RuleStatus Parser::parseTokenizationNull(GC &gc)
{
    return parseTokenizationKeyword(gc, u"null", u"missing null keyword");
}

Parser::RuleStatus Parser::parseTokenizationReturn(GC &gc)
{
    return parseTokenizationKeyword(gc, u"return", u"missing return keyword");
}

Parser::RuleStatus Parser::parseTokenizationSuper(GC &gc)
{
    return parseTokenizationKeyword(gc, u"super", u"missing super keyword");
}

Parser::RuleStatus Parser::parseTokenizationSwitch(GC &gc)
{
    return parseTokenizationKeyword(gc, u"switch", u"missing switch keyword");
}

Parser::RuleStatus Parser::parseTokenizationThis(GC &gc)
{
    return parseTokenizationKeyword(gc, u"this", u"missing this keyword");
}

Parser::RuleStatus Parser::parseTokenizationThrow(GC &gc)
{
    return parseTokenizationKeyword(gc, u"throw", u"missing throw keyword");
}

Parser::RuleStatus Parser::parseTokenizationTrue(GC &gc)
{
    return parseTokenizationKeyword(gc, u"true", u"missing true keyword");
}

Parser::RuleStatus Parser::parseTokenizationTry(GC &gc)
{
    return parseTokenizationKeyword(gc, u"try", u"missing try keyword");
}

Parser::RuleStatus Parser::parseTokenizationTypeOf(GC &gc)
{
    return parseTokenizationKeyword(gc, u"typeof", u"missing typeof keyword");
}

Parser::RuleStatus Parser::parseTokenizationVar(GC &gc)
{
    return parseTokenizationKeyword(gc, u"var", u"missing var keyword");
}

Parser::RuleStatus Parser::parseTokenizationVoid(GC &gc)
{
    return parseTokenizationKeyword(gc, u"void", u"missing void keyword");
}

Parser::RuleStatus Parser::parseTokenizationWhile(GC &gc)
{
    return parseTokenizationKeyword(gc, u"while", u"missing while keyword");
}

Parser::RuleStatus Parser::parseTokenizationWith(GC &gc)
{
    return parseTokenizationKeyword(gc, u"with", u"missing with keyword");
}

Parser::RuleStatus Parser::parseTokenizationYield(GC &gc)
{
    return parseTokenizationKeyword(gc, u"yield", u"missing yield keyword");
}

Parser::RuleStatus Parser::parseTokenizationKeyword(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationKeywordStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    retval = parseTokenizationBreak(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationCase(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationCatch(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationClass(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationConst(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationContinue(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationDebugger(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationDefault(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationDelete(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationDo(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationElse(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationExport(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationExtends(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationFinally(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationFor(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationFunction(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationIf(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationImport(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationIn(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationInstanceOf(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationNew(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationReturn(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationSuper(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationSwitch(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationThis(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationThrow(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationTry(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationTypeOf(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationVar(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationVoid(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationWhile(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationWith(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationYield(gc);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationFutureReservedWord(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationFutureReservedWordStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    retval = parseTokenizationAwait(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationEnum(gc);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationBooleanLiteral(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationBooleanLiteralStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    retval = parseTokenizationFalse(gc);
    if(retval.success())
    {
        statuses.booleanLiteralValue = false;
        return retval;
    }
    retval /= parseTokenizationTrue(gc);
    if(retval.success())
    {
        statuses.booleanLiteralValue = true;
    }
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationReservedWord(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationReservedWordStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    retval = parseTokenizationKeyword(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationFutureReservedWord(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationNull(gc);
    if(retval.success())
    {
        return retval;
    }
    retval /= parseTokenizationBooleanLiteral(gc);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationIdentifier(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationIdentifierStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    auto reservedWordResult = parseTokenizationReservedWord(gc);
    if(reservedWordResult.success())
    {
        currentPosition = startPosition;
        retval = RuleStatus::makeFailure(reservedWordResult.startPositionOrErrorPosition,
                                         reservedWordResult.endPositionOrErrorPriorityPosition,
                                         u"expected a valid identifier");
        return retval;
    }
    retval = parseTokenizationIdentifierName(gc);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationNumericLiteral(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationNumericLiteralStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'.' && !character_properties::javascriptDecimalDigit(codePoint))
    {
        retval = RuleStatus::makeFailure(startPosition, startPosition, u"missing numeric literal");
        currentPosition = startPosition;
        return retval;
    }
    String stringValue;
    bool gotDigit = false;
    if(codePoint == U'0')
    {
        stringValue = appendCodePoint(std::move(stringValue), codePoint);
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(character_properties::javascriptDecimalDigit(codePoint))
        {
            retval = RuleStatus::makeFailure(
                startPosition,
                currentPosition,
                u"legacy octal numeric literals not supported (use 0o1234 syntax instead)");
            currentPosition = startPosition;
            return retval;
        }
        if(codePoint == U'b' || codePoint == U'B')
        {
            stringValue = appendCodePoint(std::move(stringValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(!character_properties::javascriptBinaryDigit(codePoint))
            {
                retval = RuleStatus::makeFailure(currentPosition,
                                                 currentPosition,
                                                 u"binary numeric literal missing binary digit");
                currentPosition = startPosition;
                return retval;
            }
            while(character_properties::javascriptBinaryDigit(codePoint))
            {
                stringValue = appendCodePoint(std::move(stringValue), codePoint);
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
            }
            retval = RuleStatus::makeSuccess(startPosition, currentPosition);
            statuses.numericLiteralValue = value::StringHandle::toNumberValue(stringValue);
            return retval;
        }
        if(codePoint == U'o' || codePoint == U'O')
        {
            stringValue = appendCodePoint(std::move(stringValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(!character_properties::javascriptOctalDigit(codePoint))
            {
                retval = RuleStatus::makeFailure(
                    currentPosition, currentPosition, u"octal numeric literal missing octal digit");
                currentPosition = startPosition;
                return retval;
            }
            while(character_properties::javascriptOctalDigit(codePoint))
            {
                stringValue = appendCodePoint(std::move(stringValue), codePoint);
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
            }
            retval = RuleStatus::makeSuccess(startPosition, currentPosition);
            statuses.numericLiteralValue = value::StringHandle::toNumberValue(stringValue);
            return retval;
        }
        if(codePoint == U'x' || codePoint == U'X')
        {
            stringValue = appendCodePoint(std::move(stringValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(!character_properties::javascriptHexDigit(codePoint))
            {
                retval = RuleStatus::makeFailure(
                    currentPosition, currentPosition, u"hex numeric literal missing hex digit");
                currentPosition = startPosition;
                return retval;
            }
            while(character_properties::javascriptHexDigit(codePoint))
            {
                stringValue = appendCodePoint(std::move(stringValue), codePoint);
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
            }
            retval = RuleStatus::makeSuccess(startPosition, currentPosition);
            statuses.numericLiteralValue = value::StringHandle::toNumberValue(stringValue);
            return retval;
        }
        stringValue = appendCodePoint(std::move(stringValue), codePoint);
        currentPosition = nextPosition;
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        statuses.numericLiteralValue = 0;
        return retval;
    }
    while(character_properties::javascriptDecimalDigit(codePoint))
    {
        gotDigit = true;
        stringValue = appendCodePoint(std::move(stringValue), codePoint);
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
    }
    if(codePoint == U'.')
    {
        stringValue = appendCodePoint(std::move(stringValue), codePoint);
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        while(character_properties::javascriptDecimalDigit(codePoint))
        {
            gotDigit = true;
            stringValue = appendCodePoint(std::move(stringValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
    }
    if(!gotDigit)
    {
        retval =
            RuleStatus::makeFailure(startPosition, currentPosition, u"missing numeric literal");
        currentPosition = startPosition;
        return retval;
    }
    if(codePoint == U'e' || codePoint == U'E')
    {
        stringValue = appendCodePoint(std::move(stringValue), codePoint);
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'-' || codePoint == U'+')
        {
            stringValue = appendCodePoint(std::move(stringValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
        if(!character_properties::javascriptDecimalDigit(codePoint))
        {
            retval = RuleStatus::makeFailure(
                currentPosition, currentPosition, u"numeric literal missing digit in exponent");
            currentPosition = startPosition;
            return retval;
        }
        while(character_properties::javascriptDecimalDigit(codePoint))
        {
            stringValue = appendCodePoint(std::move(stringValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
    }
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    statuses.numericLiteralValue = value::StringHandle::toNumberValue(stringValue);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationStringLiteral(GC &gc)
{
    HandleScope handleScope(gc);
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationStringLiteralStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    String stringValue;
    if(codePoint != U'\"' && codePoint != U'\'')
    {
        retval = RuleStatus::makeFailure(startPosition, currentPosition, u"missing string literal");
        currentPosition = startPosition;
        return retval;
    }
    bool escapeInString = false;
    const auto enclosingQuoteCodePoint = codePoint;
    currentPosition = nextPosition;
    codePoint = getCodePoint(currentPosition, nextPosition);
    while(codePoint != enclosingQuoteCodePoint)
    {
        if(codePoint == eofCodePoint || character_properties::javascriptLineTerminator(codePoint))
        {
            retval = RuleStatus::makeFailure(
                startPosition, currentPosition, u"string literal missing closing quote");
            currentPosition = startPosition;
            return retval;
        }
        else if(codePoint == U'\\')
        {
            escapeInString = true;
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(character_properties::javascriptLineTerminator(codePoint))
            {
                parseTokenizationLineTerminatorSequence(gc); // never fails
                codePoint = getCodePoint(currentPosition, nextPosition);
                continue;
            }
            auto escapeSequenceResult = parseTokenizationEscapeSequence(gc, codePoint);
            if(escapeSequenceResult.fail())
            {
                retval = escapeSequenceResult;
                currentPosition = startPosition;
                return retval;
            }
            stringValue = appendCodePoint(std::move(stringValue), codePoint);
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
        else
        {
            stringValue = appendCodePoint(std::move(stringValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
    }
    currentPosition = nextPosition;
    statuses.stringLiteralValue = gc.internString(std::move(stringValue)).get();
    statuses.escapeInString = escapeInString;
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationRegularExpressionLiteral(GC &gc)
{
    HandleScope handleScope(gc);
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationRegularExpressionLiteralStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    String bodyValue;
    if(codePoint != U'/')
    {
        retval = RuleStatus::makeFailure(
            startPosition, currentPosition, u"missing regular expression literal");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint == U'/' || codePoint == U'*')
    {
        retval = RuleStatus::makeFailure(
            startPosition, currentPosition, u"missing regular expression literal");
        currentPosition = startPosition;
        return retval;
    }
    while(codePoint != U'/')
    {
        if(codePoint == eofCodePoint || character_properties::javascriptLineTerminator(codePoint))
        {
            retval = RuleStatus::makeFailure(currentPosition,
                                             currentPosition,
                                             u"regular expression literal missing closing slash");
            currentPosition = startPosition;
            return retval;
        }
        else if(codePoint == U'\\')
        {
            bodyValue = appendCodePoint(std::move(bodyValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(character_properties::javascriptLineTerminator(codePoint))
            {
                retval = RuleStatus::makeFailure(
                    currentPosition,
                    currentPosition,
                    u"incomplete escape sequence in regular expression literal");
                currentPosition = startPosition;
                return retval;
            }
            bodyValue = appendCodePoint(std::move(bodyValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
        else if(codePoint == U'[')
        {
            bodyValue = appendCodePoint(std::move(bodyValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            while(codePoint != U']')
            {
                if(codePoint == eofCodePoint
                   || character_properties::javascriptLineTerminator(codePoint))
                {
                    retval = RuleStatus::makeFailure(
                        currentPosition,
                        currentPosition,
                        u"regular expression literal class missing closing ]");
                    currentPosition = startPosition;
                    return retval;
                }
                else if(codePoint == U'\\')
                {
                    bodyValue = appendCodePoint(std::move(bodyValue), codePoint);
                    currentPosition = nextPosition;
                    codePoint = getCodePoint(currentPosition, nextPosition);
                    if(character_properties::javascriptLineTerminator(codePoint))
                    {
                        retval = RuleStatus::makeFailure(
                            currentPosition,
                            currentPosition,
                            u"incomplete escape sequence in regular expression literal");
                        currentPosition = startPosition;
                        return retval;
                    }
                    bodyValue = appendCodePoint(std::move(bodyValue), codePoint);
                    currentPosition = nextPosition;
                    codePoint = getCodePoint(currentPosition, nextPosition);
                }
                else
                {
                    bodyValue = appendCodePoint(std::move(bodyValue), codePoint);
                    currentPosition = nextPosition;
                    codePoint = getCodePoint(currentPosition, nextPosition);
                }
            }
        }
        else
        {
            bodyValue = appendCodePoint(std::move(bodyValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
    }
    currentPosition = nextPosition;
    codePoint = getCodePoint(currentPosition, nextPosition);
    statuses.regularExpressionLiteralValue = gc.internString(std::move(bodyValue)).get();
    String flagsValue;
    while(character_properties::javascriptIdContinue(codePoint))
    {
        flagsValue = appendCodePoint(std::move(flagsValue), codePoint);
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
    }
    if(codePoint == U'\\')
    {
        retval =
            RuleStatus::makeFailure(currentPosition,
                                    currentPosition,
                                    u"unicode escape not permitted in regular expression flags");
        currentPosition = startPosition;
        return retval;
    }
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationTemplateCharacters(GC &gc)
{
    HandleScope handleScope(gc);
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationTemplateCharactersStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    String templateValue;
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    while(codePoint != U'`')
    {
        if(codePoint == eofCodePoint)
        {
            retval = RuleStatus::makeFailure(
                startPosition, currentPosition, u"template literal missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        else if(codePoint == U'\\')
        {
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(character_properties::javascriptLineTerminator(codePoint))
            {
                parseTokenizationLineTerminatorSequence(gc); // never fails
                codePoint = getCodePoint(currentPosition, nextPosition);
            }
            else
            {
                retval = parseTokenizationEscapeSequence(gc, codePoint);
                if(retval.fail())
                {
                    currentPosition = startPosition;
                    return retval;
                }
                templateValue = appendCodePoint(std::move(templateValue), codePoint);
                codePoint = getCodePoint(currentPosition, nextPosition);
            }
        }
        else if(codePoint == U'\r')
        {
            templateValue = appendCodePoint(std::move(templateValue), U'\n');
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(codePoint == U'\n')
            {
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
            }
        }
        else if(codePoint == U'$')
        {
            auto dollarPosition = currentPosition;
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(codePoint == U'{')
            {
                currentPosition = dollarPosition;
                break;
            }
            templateValue = appendCodePoint(std::move(templateValue), U'$');
        }
        else
        {
            templateValue = appendCodePoint(std::move(templateValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
    }
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    String templateRawValue;
    std::size_t pos = startPosition;
    while(pos < currentPosition)
    {
        codePoint = getCodePoint(pos, pos);
        if(codePoint == U'\r')
        {
            templateRawValue = appendCodePoint(std::move(templateRawValue), U'\n');
            std::size_t nextPos;
            codePoint = getCodePoint(pos, nextPos);
            if(codePoint == U'\n')
                pos = nextPos;
        }
        else
        {
            templateRawValue = appendCodePoint(std::move(templateRawValue), codePoint);
        }
    }
    statuses.templateValue = gc.internString(std::move(templateValue)).get();
    statuses.templateRawValue = gc.internString(std::move(templateRawValue)).get();
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationNoSubstitutionTemplate(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationNoSubstitutionTemplateStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'`')
    {
        retval =
            RuleStatus::makeFailure(currentPosition, currentPosition, u"missing template literal");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    auto &templateCharactersStatuses = getRuleStatuses(currentPosition);
    retval = parseTokenizationTemplateCharacters(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'`')
    {
        retval = RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    statuses.templateValue = templateCharactersStatuses.templateValue;
    statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationTemplateHead(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationTemplateHeadStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'`')
    {
        retval =
            RuleStatus::makeFailure(currentPosition, currentPosition, u"missing template literal");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    auto &templateCharactersStatuses = getRuleStatuses(currentPosition);
    retval = parseTokenizationTemplateCharacters(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'$')
    {
        retval = RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'{')
    {
        retval = RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    statuses.templateValue = templateCharactersStatuses.templateValue;
    statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationTemplateTail(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationTemplateTailStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'}')
    {
        retval = RuleStatus::makeFailure(
            currentPosition, currentPosition, u"missing template substitution tail");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    auto &templateCharactersStatuses = getRuleStatuses(currentPosition);
    retval = parseTokenizationTemplateCharacters(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'`')
    {
        retval = RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    statuses.templateValue = templateCharactersStatuses.templateValue;
    statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationTemplateMiddle(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.tokenizationTemplateMiddleStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'}')
    {
        retval = RuleStatus::makeFailure(
            currentPosition, currentPosition, u"missing template substitution tail");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    auto &templateCharactersStatuses = getRuleStatuses(currentPosition);
    retval = parseTokenizationTemplateCharacters(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'$')
    {
        retval = RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    codePoint = getCodePoint(currentPosition, nextPosition);
    if(codePoint != U'{')
    {
        retval = RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
        currentPosition = startPosition;
        return retval;
    }
    currentPosition = nextPosition;
    statuses.templateValue = templateCharactersStatuses.templateValue;
    statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    return retval;
}

Parser::RuleStatus Parser::parseTokenizationPunctuator(GC &gc, Punctuator &punctuator)
{
    std::size_t startPosition = currentPosition;
    if(parseTokenizationNumericLiteral(gc).success())
    {
        auto errorPriorityPosition = currentPosition;
        currentPosition = startPosition;
        return RuleStatus::makeFailure(startPosition, errorPriorityPosition, u"missing punctuator");
    }
    std::size_t nextPosition;
    auto codePoint = getCodePoint(currentPosition, nextPosition);
    switch(codePoint)
    {
    case U'{':
        punctuator = Punctuator::LBrace;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'}':
        punctuator = Punctuator::RBrace;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'(':
        punctuator = Punctuator::LParen;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U')':
        punctuator = Punctuator::RParen;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'[':
        punctuator = Punctuator::LBracket;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U']':
        punctuator = Punctuator::RBracket;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'.':
    {
        punctuator = Punctuator::Period;
        currentPosition = nextPosition;
        auto singlePeriodPosition = currentPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'.')
        {
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(codePoint == U'.')
            {
                punctuator = Punctuator::Ellipsis;
                currentPosition = nextPosition;
            }
            else
            {
                currentPosition = singlePeriodPosition;
            }
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    }
    case U';':
        punctuator = Punctuator::Semicolon;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U',':
        punctuator = Punctuator::Comma;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'<':
        punctuator = Punctuator::LAngle;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'<')
        {
            punctuator = Punctuator::LAngleLAngle;
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(codePoint == U'=')
            {
                punctuator = Punctuator::LAngleLAngleEqual;
                currentPosition = nextPosition;
            }
        }
        else if(codePoint == U'=')
        {
            punctuator = Punctuator::LAngleEqual;
            currentPosition = nextPosition;
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'>':
        punctuator = Punctuator::RAngle;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'>')
        {
            punctuator = Punctuator::RAngleRAngle;
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(codePoint == U'>')
            {
                punctuator = Punctuator::RAngleRAngleRAngle;
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
                if(codePoint == U'=')
                {
                    punctuator = Punctuator::RAngleRAngleRAngleEqual;
                    currentPosition = nextPosition;
                }
            }
            else if(codePoint == U'=')
            {
                punctuator = Punctuator::RAngleRAngleEqual;
                currentPosition = nextPosition;
            }
        }
        else if(codePoint == U'=')
        {
            punctuator = Punctuator::RAngleEqual;
            currentPosition = nextPosition;
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'=':
        punctuator = Punctuator::Equal;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'>')
        {
            punctuator = Punctuator::EqualRAngle;
            currentPosition = nextPosition;
        }
        else if(codePoint == U'=')
        {
            punctuator = Punctuator::EqualEqual;
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(codePoint == U'=')
            {
                punctuator = Punctuator::EqualEqualEqual;
                currentPosition = nextPosition;
            }
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'!':
        punctuator = Punctuator::EMark;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'=')
        {
            punctuator = Punctuator::EMarkEqual;
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(codePoint == U'=')
            {
                punctuator = Punctuator::EMarkEqualEqual;
                currentPosition = nextPosition;
            }
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'+':
        punctuator = Punctuator::Plus;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'=')
        {
            punctuator = Punctuator::PlusEqual;
            currentPosition = nextPosition;
        }
        else if(codePoint == U'+')
        {
            punctuator = Punctuator::PlusPlus;
            currentPosition = nextPosition;
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'-':
        punctuator = Punctuator::Minus;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'=')
        {
            punctuator = Punctuator::MinusEqual;
            currentPosition = nextPosition;
        }
        else if(codePoint == U'-')
        {
            punctuator = Punctuator::MinusMinus;
            currentPosition = nextPosition;
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'*':
        punctuator = Punctuator::Star;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'=')
        {
            punctuator = Punctuator::StarEqual;
            currentPosition = nextPosition;
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'%':
        punctuator = Punctuator::Percent;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'=')
        {
            punctuator = Punctuator::PercentEqual;
            currentPosition = nextPosition;
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'&':
        punctuator = Punctuator::Amp;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'=')
        {
            punctuator = Punctuator::AmpEqual;
            currentPosition = nextPosition;
        }
        else if(codePoint == U'&')
        {
            punctuator = Punctuator::AmpAmp;
            currentPosition = nextPosition;
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'|':
        punctuator = Punctuator::Pipe;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'=')
        {
            punctuator = Punctuator::PipeEqual;
            currentPosition = nextPosition;
        }
        else if(codePoint == U'|')
        {
            punctuator = Punctuator::PipePipe;
            currentPosition = nextPosition;
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'^':
        punctuator = Punctuator::Caret;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'=')
        {
            punctuator = Punctuator::CaretEqual;
            currentPosition = nextPosition;
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'~':
        punctuator = Punctuator::Tilde;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'?':
        punctuator = Punctuator::QMark;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U':':
        punctuator = Punctuator::Colon;
        currentPosition = nextPosition;
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    case U'/':
        punctuator = Punctuator::FSlash;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'=')
        {
            punctuator = Punctuator::FSlashEqual;
            currentPosition = nextPosition;
        }
        else if(codePoint == U'*' || codePoint == U'/')
        {
            auto errorPriorityPosition = currentPosition;
            currentPosition = startPosition;
            return RuleStatus::makeFailure(
                startPosition, errorPriorityPosition, u"missing punctuator");
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    default:
        currentPosition = startPosition;
        return RuleStatus::makeFailure(startPosition, startPosition, u"missing punctuator");
    }
}

Parser::RuleStatus Parser::parseTokenizationPunctuator(GC &gc,
                                                       Punctuator correctPunctuator,
                                                       const char16_t *failMessage)
{
    Punctuator punctuator;
    std::size_t startPosition = currentPosition;
    if(parseTokenizationPunctuator(gc, punctuator).success())
    {
        if(punctuator == correctPunctuator)
        {
            return RuleStatus::makeSuccess(startPosition, currentPosition);
        }
    }
    auto errorPriorityPosition = currentPosition;
    currentPosition = startPosition;
    return RuleStatus::makeFailure(startPosition, errorPriorityPosition, failMessage);
}

Parser::RuleStatus Parser::parseTokenizationLBrace(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::LBrace, u"missing {");
}

Parser::RuleStatus Parser::parseTokenizationRBrace(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::RBrace, u"missing }");
}

Parser::RuleStatus Parser::parseTokenizationLParen(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::LParen, u"missing (");
}

Parser::RuleStatus Parser::parseTokenizationRParen(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::RParen, u"missing )");
}

Parser::RuleStatus Parser::parseTokenizationLBracket(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::LBracket, u"missing [");
}

Parser::RuleStatus Parser::parseTokenizationRBracket(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::RBracket, u"missing ]");
}

Parser::RuleStatus Parser::parseTokenizationPeriod(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Period, u"missing .");
}

Parser::RuleStatus Parser::parseTokenizationEllipsis(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Ellipsis, u"missing ...");
}

Parser::RuleStatus Parser::parseTokenizationSemicolon(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Semicolon, u"missing ;");
}

Parser::RuleStatus Parser::parseTokenizationComma(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Comma, u"missing ,");
}

Parser::RuleStatus Parser::parseTokenizationLAngle(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::LAngle, u"missing <");
}

Parser::RuleStatus Parser::parseTokenizationRAngle(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::RAngle, u"missing >");
}

Parser::RuleStatus Parser::parseTokenizationLAngleEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::LAngleEqual, u"missing <=");
}

Parser::RuleStatus Parser::parseTokenizationRAngleEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::RAngleEqual, u"missing >=");
}

Parser::RuleStatus Parser::parseTokenizationEqualEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::EqualEqual, u"missing ==");
}

Parser::RuleStatus Parser::parseTokenizationEMarkEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::EMarkEqual, u"missing !=");
}

Parser::RuleStatus Parser::parseTokenizationEqualEqualEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::EqualEqualEqual, u"missing ===");
}

Parser::RuleStatus Parser::parseTokenizationEMarkEqualEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::EMarkEqualEqual, u"missing !==");
}

Parser::RuleStatus Parser::parseTokenizationPlus(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Plus, u"missing +");
}

Parser::RuleStatus Parser::parseTokenizationMinus(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Minus, u"missing -");
}

Parser::RuleStatus Parser::parseTokenizationStar(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Star, u"missing *");
}

Parser::RuleStatus Parser::parseTokenizationPercent(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Percent, u"missing %");
}

Parser::RuleStatus Parser::parseTokenizationPlusPlus(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::PlusPlus, u"missing ++");
}

Parser::RuleStatus Parser::parseTokenizationMinusMinus(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::MinusMinus, u"missing --");
}

Parser::RuleStatus Parser::parseTokenizationLAngleLAngle(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::LAngleLAngle, u"missing <<");
}

Parser::RuleStatus Parser::parseTokenizationRAngleRAngle(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::RAngleRAngle, u"missing >>");
}

Parser::RuleStatus Parser::parseTokenizationRAngleRAngleRAngle(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::RAngleRAngleRAngle, u"missing >>>");
}

Parser::RuleStatus Parser::parseTokenizationAmp(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Amp, u"missing &");
}

Parser::RuleStatus Parser::parseTokenizationPipe(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Pipe, u"missing |");
}

Parser::RuleStatus Parser::parseTokenizationCaret(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Caret, u"missing ^");
}

Parser::RuleStatus Parser::parseTokenizationEMark(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::EMark, u"missing !");
}

Parser::RuleStatus Parser::parseTokenizationTilde(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Tilde, u"missing ~");
}

Parser::RuleStatus Parser::parseTokenizationAmpAmp(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::AmpAmp, u"missing &&");
}

Parser::RuleStatus Parser::parseTokenizationPipePipe(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::PipePipe, u"missing ||");
}

Parser::RuleStatus Parser::parseTokenizationQMark(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::QMark, u"missing ?");
}

Parser::RuleStatus Parser::parseTokenizationColon(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Colon, u"missing :");
}

Parser::RuleStatus Parser::parseTokenizationEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::Equal, u"missing =");
}

Parser::RuleStatus Parser::parseTokenizationPlusEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::PlusEqual, u"missing +=");
}

Parser::RuleStatus Parser::parseTokenizationMinusEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::MinusEqual, u"missing -=");
}

Parser::RuleStatus Parser::parseTokenizationStarEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::StarEqual, u"missing *=");
}

Parser::RuleStatus Parser::parseTokenizationPercentEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::PercentEqual, u"missing %=");
}

Parser::RuleStatus Parser::parseTokenizationLAngleLAngleEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::LAngleLAngleEqual, u"missing <<=");
}

Parser::RuleStatus Parser::parseTokenizationRAngleRAngleEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::RAngleRAngleEqual, u"missing >>=");
}

Parser::RuleStatus Parser::parseTokenizationRAngleRAngleRAngleEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::RAngleRAngleRAngleEqual, u"missing >>>=");
}

Parser::RuleStatus Parser::parseTokenizationAmpEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::AmpEqual, u"missing &=");
}

Parser::RuleStatus Parser::parseTokenizationPipeEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::PipeEqual, u"missing |=");
}

Parser::RuleStatus Parser::parseTokenizationCaretEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::CaretEqual, u"missing ^=");
}

Parser::RuleStatus Parser::parseTokenizationEqualRAngle(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::EqualRAngle, u"missing =>");
}

Parser::RuleStatus Parser::parseTokenizationFSlash(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::FSlash, u"missing /");
}

Parser::RuleStatus Parser::parseTokenizationFSlashEqual(GC &gc)
{
    return parseTokenizationPunctuator(gc, Punctuator::FSlashEqual, u"missing /=");
}

Parser::RuleStatus Parser::parseTokenizationEndOfFile(GC &gc)
{
    if(getCodePoint(currentPosition) == eofCodePoint)
        return RuleStatus::makeSuccess(currentPosition, currentPosition);
    return RuleStatus::makeFailure(currentPosition, currentPosition, u"unexpected token");
}

bool Parser::testParseToken(bool parseTemplateContinuation, bool parseRegularExpression, GC &gc)
{
    HandleScope handleScope(gc);
    Location location(SourceHandle(gc, source), currentPosition);
    std::ostringstream ss;
    writeString(ss, location.toString());
    ss << ": ";
    ss << std::boolalpha;
    if(getCodePoint(currentPosition) == eofCodePoint)
    {
        ss << "EndOfFile";
        std::cout << ss.str() << std::endl;
        return false;
    }
    do
    {
        std::size_t startPosition = currentPosition;
        auto result = parseTokenizationSeperator(gc);
        if(result.success()
           && result.endPositionOrErrorPriorityPosition != result.startPositionOrErrorPosition)
        {
            ss << "Seperator: lineSplit = " << getRuleStatuses(startPosition).lineSplit;
            break;
        }
        else if(result.success())
        {
            result = parseTokenizationAwait(gc);
        }
        else
        {
            result /= parseTokenizationAwait(gc);
        }
        if(result.success())
        {
            ss << "Await";
            break;
        }
        result /= parseTokenizationBreak(gc);
        if(result.success())
        {
            ss << "Break";
            break;
        }
        result /= parseTokenizationCase(gc);
        if(result.success())
        {
            ss << "Case";
            break;
        }
        result /= parseTokenizationCatch(gc);
        if(result.success())
        {
            ss << "Catch";
            break;
        }
        result /= parseTokenizationClass(gc);
        if(result.success())
        {
            ss << "Class";
            break;
        }
        result /= parseTokenizationConst(gc);
        if(result.success())
        {
            ss << "Const";
            break;
        }
        result /= parseTokenizationContinue(gc);
        if(result.success())
        {
            ss << "Continue";
            break;
        }
        result /= parseTokenizationDebugger(gc);
        if(result.success())
        {
            ss << "Debugger";
            break;
        }
        result /= parseTokenizationDefault(gc);
        if(result.success())
        {
            ss << "Default";
            break;
        }
        result /= parseTokenizationDelete(gc);
        if(result.success())
        {
            ss << "Delete";
            break;
        }
        result /= parseTokenizationDo(gc);
        if(result.success())
        {
            ss << "Do";
            break;
        }
        result /= parseTokenizationElse(gc);
        if(result.success())
        {
            ss << "Else";
            break;
        }
        result /= parseTokenizationEnum(gc);
        if(result.success())
        {
            ss << "Enum";
            break;
        }
        result /= parseTokenizationExport(gc);
        if(result.success())
        {
            ss << "Export";
            break;
        }
        result /= parseTokenizationExtends(gc);
        if(result.success())
        {
            ss << "Extends";
            break;
        }
        result /= parseTokenizationFinally(gc);
        if(result.success())
        {
            ss << "Finally";
            break;
        }
        result /= parseTokenizationFor(gc);
        if(result.success())
        {
            ss << "For";
            break;
        }
        result /= parseTokenizationFunction(gc);
        if(result.success())
        {
            ss << "Function";
            break;
        }
        result /= parseTokenizationIf(gc);
        if(result.success())
        {
            ss << "If";
            break;
        }
        result /= parseTokenizationImport(gc);
        if(result.success())
        {
            ss << "Import";
            break;
        }
        result /= parseTokenizationIn(gc);
        if(result.success())
        {
            ss << "In";
            break;
        }
        result /= parseTokenizationInstanceOf(gc);
        if(result.success())
        {
            ss << "InstanceOf";
            break;
        }
        result /= parseTokenizationNew(gc);
        if(result.success())
        {
            ss << "New";
            break;
        }
        result /= parseTokenizationNull(gc);
        if(result.success())
        {
            ss << "Null";
            break;
        }
        result /= parseTokenizationReturn(gc);
        if(result.success())
        {
            ss << "Return";
            break;
        }
        result /= parseTokenizationSuper(gc);
        if(result.success())
        {
            ss << "Super";
            break;
        }
        result /= parseTokenizationSwitch(gc);
        if(result.success())
        {
            ss << "Switch";
            break;
        }
        result /= parseTokenizationThis(gc);
        if(result.success())
        {
            ss << "This";
            break;
        }
        result /= parseTokenizationThrow(gc);
        if(result.success())
        {
            ss << "Throw";
            break;
        }
        result /= parseTokenizationTry(gc);
        if(result.success())
        {
            ss << "Try";
            break;
        }
        result /= parseTokenizationTypeOf(gc);
        if(result.success())
        {
            ss << "TypeOf";
            break;
        }
        result /= parseTokenizationVar(gc);
        if(result.success())
        {
            ss << "Var";
            break;
        }
        result /= parseTokenizationVoid(gc);
        if(result.success())
        {
            ss << "Void";
            break;
        }
        result /= parseTokenizationWhile(gc);
        if(result.success())
        {
            ss << "While";
            break;
        }
        result /= parseTokenizationWith(gc);
        if(result.success())
        {
            ss << "With";
            break;
        }
        result /= parseTokenizationYield(gc);
        if(result.success())
        {
            ss << "Yield";
            break;
        }
        result /= parseTokenizationBooleanLiteral(gc);
        if(result.success())
        {
            ss << "BooleanLiteral: booleanLiteralValue = "
               << getRuleStatuses(startPosition).booleanLiteralValue;
            break;
        }
        result /= parseTokenizationIdentifier(gc);
        if(result.success())
        {
            ss << "Identifier: identifierNameValue = ";
            writeString(ss,
                        gc.readString(Handle<gc::StringReference>(
                            gc, getRuleStatuses(startPosition).identifierNameValue)));
            break;
        }
        result /= parseTokenizationNumericLiteral(gc);
        if(result.success())
        {
            ss << "NumericLiteral: numericLiteralValue = ";
            writeString(ss,
                        value::DoubleHandle::toStringValue(
                            getRuleStatuses(startPosition).numericLiteralValue));
            break;
        }
        result /= parseTokenizationStringLiteral(gc);
        if(result.success())
        {
            ss << "StringLiteral: escapeInString = "
               << getRuleStatuses(startPosition).escapeInString << " stringLiteralValue = ";
            writeString(ss,
                        gc.readString(Handle<gc::StringReference>(
                            gc, getRuleStatuses(startPosition).stringLiteralValue)));
            break;
        }
        if(parseRegularExpression)
        {
            result /= parseTokenizationRegularExpressionLiteral(gc);
            if(result.success())
            {
                ss << "RegularExpressionLiteral: regularExpressionLiteralValue = ";
                writeString(ss,
                            gc.readString(Handle<gc::StringReference>(
                                gc, getRuleStatuses(startPosition).regularExpressionLiteralValue)));
                ss << " regularExpressionLiteralFlagsValue = ";
                writeString(
                    ss,
                    gc.readString(Handle<gc::StringReference>(
                        gc, getRuleStatuses(startPosition).regularExpressionLiteralFlagsValue)));
                break;
            }
        }
        result /= parseTokenizationTemplateHead(gc);
        if(result.success())
        {
            ss << "TemplateHead: templateValue = ";
            writeString(ss,
                        gc.readString(Handle<gc::StringReference>(
                            gc, getRuleStatuses(startPosition).templateValue)));
            ss << " templateRawValue = ";
            writeString(ss,
                        gc.readString(Handle<gc::StringReference>(
                            gc, getRuleStatuses(startPosition).templateRawValue)));
            break;
        }
        result /= parseTokenizationNoSubstitutionTemplate(gc);
        if(result.success())
        {
            ss << "NoSubstitutionTemplate: templateValue = ";
            writeString(ss,
                        gc.readString(Handle<gc::StringReference>(
                            gc, getRuleStatuses(startPosition).templateValue)));
            ss << " templateRawValue = ";
            writeString(ss,
                        gc.readString(Handle<gc::StringReference>(
                            gc, getRuleStatuses(startPosition).templateRawValue)));
            break;
        }
        if(parseTemplateContinuation)
        {
            result /= parseTokenizationTemplateMiddle(gc);
            if(result.success())
            {
                ss << "TemplateMiddle: templateValue = ";
                writeString(ss,
                            gc.readString(Handle<gc::StringReference>(
                                gc, getRuleStatuses(startPosition).templateValue)));
                ss << " templateRawValue = ";
                writeString(ss,
                            gc.readString(Handle<gc::StringReference>(
                                gc, getRuleStatuses(startPosition).templateRawValue)));
                break;
            }
            result /= parseTokenizationTemplateTail(gc);
            if(result.success())
            {
                ss << "TemplateMiddle: templateValue = ";
                writeString(ss,
                            gc.readString(Handle<gc::StringReference>(
                                gc, getRuleStatuses(startPosition).templateValue)));
                ss << " templateRawValue = ";
                writeString(ss,
                            gc.readString(Handle<gc::StringReference>(
                                gc, getRuleStatuses(startPosition).templateRawValue)));
                break;
            }
        }
        result /= parseTokenizationLBrace(gc);
        if(result.success())
        {
            ss << "LBrace";
            break;
        }
        if(!parseTemplateContinuation)
        {
            result /= parseTokenizationRBrace(gc);
            if(result.success())
            {
                ss << "RBrace";
                break;
            }
        }
        result /= parseTokenizationLParen(gc);
        if(result.success())
        {
            ss << "LParen";
            break;
        }
        result /= parseTokenizationRParen(gc);
        if(result.success())
        {
            ss << "RParen";
            break;
        }
        result /= parseTokenizationLBracket(gc);
        if(result.success())
        {
            ss << "LBracket";
            break;
        }
        result /= parseTokenizationRBracket(gc);
        if(result.success())
        {
            ss << "RBracket";
            break;
        }
        result /= parseTokenizationPeriod(gc);
        if(result.success())
        {
            ss << "Period";
            break;
        }
        result /= parseTokenizationEllipsis(gc);
        if(result.success())
        {
            ss << "Ellipsis";
            break;
        }
        result /= parseTokenizationSemicolon(gc);
        if(result.success())
        {
            ss << "Semicolon";
            break;
        }
        result /= parseTokenizationComma(gc);
        if(result.success())
        {
            ss << "Comma";
            break;
        }
        result /= parseTokenizationLAngle(gc);
        if(result.success())
        {
            ss << "LAngle";
            break;
        }
        result /= parseTokenizationRAngle(gc);
        if(result.success())
        {
            ss << "RAngle";
            break;
        }
        result /= parseTokenizationLAngleEqual(gc);
        if(result.success())
        {
            ss << "LAngleEqual";
            break;
        }
        result /= parseTokenizationRAngleEqual(gc);
        if(result.success())
        {
            ss << "RAngleEqual";
            break;
        }
        result /= parseTokenizationEqualEqual(gc);
        if(result.success())
        {
            ss << "EqualEqual";
            break;
        }
        result /= parseTokenizationEMarkEqual(gc);
        if(result.success())
        {
            ss << "EMarkEqual";
            break;
        }
        result /= parseTokenizationEqualEqualEqual(gc);
        if(result.success())
        {
            ss << "EqualEqualEqual";
            break;
        }
        result /= parseTokenizationEMarkEqualEqual(gc);
        if(result.success())
        {
            ss << "EMarkEqualEqual";
            break;
        }
        result /= parseTokenizationPlus(gc);
        if(result.success())
        {
            ss << "Plus";
            break;
        }
        result /= parseTokenizationMinus(gc);
        if(result.success())
        {
            ss << "Minus";
            break;
        }
        result /= parseTokenizationStar(gc);
        if(result.success())
        {
            ss << "Star";
            break;
        }
        result /= parseTokenizationPercent(gc);
        if(result.success())
        {
            ss << "Percent";
            break;
        }
        result /= parseTokenizationPlusPlus(gc);
        if(result.success())
        {
            ss << "PlusPlus";
            break;
        }
        result /= parseTokenizationMinusMinus(gc);
        if(result.success())
        {
            ss << "MinusMinus";
            break;
        }
        result /= parseTokenizationLAngleLAngle(gc);
        if(result.success())
        {
            ss << "LAngleLAngle";
            break;
        }
        result /= parseTokenizationRAngleRAngle(gc);
        if(result.success())
        {
            ss << "RAngleRAngle";
            break;
        }
        result /= parseTokenizationRAngleRAngleRAngle(gc);
        if(result.success())
        {
            ss << "RAngleRAngleRAngle";
            break;
        }
        result /= parseTokenizationAmp(gc);
        if(result.success())
        {
            ss << "Amp";
            break;
        }
        result /= parseTokenizationPipe(gc);
        if(result.success())
        {
            ss << "Pipe";
            break;
        }
        result /= parseTokenizationCaret(gc);
        if(result.success())
        {
            ss << "Caret";
            break;
        }
        result /= parseTokenizationEMark(gc);
        if(result.success())
        {
            ss << "EMark";
            break;
        }
        result /= parseTokenizationTilde(gc);
        if(result.success())
        {
            ss << "Tilde";
            break;
        }
        result /= parseTokenizationAmpAmp(gc);
        if(result.success())
        {
            ss << "AmpAmp";
            break;
        }
        result /= parseTokenizationPipePipe(gc);
        if(result.success())
        {
            ss << "PipePipe";
            break;
        }
        result /= parseTokenizationQMark(gc);
        if(result.success())
        {
            ss << "QMark";
            break;
        }
        result /= parseTokenizationColon(gc);
        if(result.success())
        {
            ss << "Colon";
            break;
        }
        result /= parseTokenizationEqual(gc);
        if(result.success())
        {
            ss << "Equal";
            break;
        }
        result /= parseTokenizationPlusEqual(gc);
        if(result.success())
        {
            ss << "PlusEqual";
            break;
        }
        result /= parseTokenizationMinusEqual(gc);
        if(result.success())
        {
            ss << "MinusEqual";
            break;
        }
        result /= parseTokenizationStarEqual(gc);
        if(result.success())
        {
            ss << "StarEqual";
            break;
        }
        result /= parseTokenizationPercentEqual(gc);
        if(result.success())
        {
            ss << "PercentEqual";
            break;
        }
        result /= parseTokenizationLAngleLAngleEqual(gc);
        if(result.success())
        {
            ss << "LAngleLAngleEqual";
            break;
        }
        result /= parseTokenizationRAngleRAngleEqual(gc);
        if(result.success())
        {
            ss << "RAngleRAngleEqual";
            break;
        }
        result /= parseTokenizationRAngleRAngleRAngleEqual(gc);
        if(result.success())
        {
            ss << "RAngleRAngleRAngleEqual";
            break;
        }
        result /= parseTokenizationAmpEqual(gc);
        if(result.success())
        {
            ss << "AmpEqual";
            break;
        }
        result /= parseTokenizationPipeEqual(gc);
        if(result.success())
        {
            ss << "PipeEqual";
            break;
        }
        result /= parseTokenizationCaretEqual(gc);
        if(result.success())
        {
            ss << "CaretEqual";
            break;
        }
        result /= parseTokenizationEqualRAngle(gc);
        if(result.success())
        {
            ss << "EqualRAngle";
            break;
        }
        if(!parseRegularExpression)
        {
            result /= parseTokenizationFSlash(gc);
            if(result.success())
            {
                ss << "FSlash";
                break;
            }
            result /= parseTokenizationFSlashEqual(gc);
            if(result.success())
            {
                ss << "FSlashEqual";
                break;
            }
        }
        if(!result.success())
        {
            writeString(
                ss,
                Location(SourceHandle(gc, source), result.startPositionOrErrorPosition).toString());
            ss << ": ";
            ss << "Error: ";
            writeString(ss, result.errorMessage);
            std::cout << ss.str() << std::endl;
            return false;
        }
    } while(false);
    std::cout << ss.str() << std::endl;
    return true;
}

template <Parser::RuleStatus (Parser::*parseTokenization)(GC &gc)>
Parser::RuleStatus Parser::parseSeperatorAndToken(GC &gc,
                                                  void (*finalCopy)(RuleStatuses &dest,
                                                                    RuleStatuses &src))
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    std::size_t startPosition = currentPosition;
    RuleStatus retval = parseTokenizationSeperator(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    std::size_t afterSeperatorPosition = currentPosition;
    retval = (this->*parseTokenization)(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    if(finalCopy)
        finalCopy(statuses, getRuleStatuses(afterSeperatorPosition));
    return retval;
}

Parser::RuleStatus Parser::parseTokenIdentifierName(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationIdentifierName>(
        gc,
        [](RuleStatuses &dest, RuleStatuses &src)
        {
            dest.identifierNameValue = src.identifierNameValue;
        });
}

Parser::RuleStatus Parser::parseTokenIdentifier(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationIdentifier>(
        gc,
        [](RuleStatuses &dest, RuleStatuses &src)
        {
            dest.identifierNameValue = src.identifierNameValue;
        });
}

Parser::RuleStatus Parser::parseTokenLBrace(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationLBrace>(gc);
}

Parser::RuleStatus Parser::parseTokenRBrace(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationRBrace>(gc);
}

Parser::RuleStatus Parser::parseTokenLParen(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationLParen>(gc);
}

Parser::RuleStatus Parser::parseTokenRParen(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationRParen>(gc);
}

Parser::RuleStatus Parser::parseTokenLBracket(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationLBracket>(gc);
}

Parser::RuleStatus Parser::parseTokenRBracket(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationRBracket>(gc);
}

Parser::RuleStatus Parser::parseTokenPeriod(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationPeriod>(gc);
}

Parser::RuleStatus Parser::parseTokenEllipsis(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationEllipsis>(gc);
}

Parser::RuleStatus Parser::parseTokenSemicolon(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationSemicolon>(gc);
}

Parser::RuleStatus Parser::parseTokenComma(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationComma>(gc);
}

Parser::RuleStatus Parser::parseTokenLAngle(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationLAngle>(gc);
}

Parser::RuleStatus Parser::parseTokenRAngle(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationRAngle>(gc);
}

Parser::RuleStatus Parser::parseTokenLAngleEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationLAngleEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenRAngleEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationRAngleEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenEqualEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationEqualEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenEMarkEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationEMarkEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenEqualEqualEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationEqualEqualEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenEMarkEqualEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationEMarkEqualEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenPlus(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationPlus>(gc);
}

Parser::RuleStatus Parser::parseTokenMinus(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationMinus>(gc);
}

Parser::RuleStatus Parser::parseTokenStar(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationStar>(gc);
}

Parser::RuleStatus Parser::parseTokenPercent(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationPercent>(gc);
}

Parser::RuleStatus Parser::parseTokenPlusPlus(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationPlusPlus>(gc);
}

Parser::RuleStatus Parser::parseTokenMinusMinus(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationMinusMinus>(gc);
}

Parser::RuleStatus Parser::parseTokenLAngleLAngle(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationLAngleLAngle>(gc);
}

Parser::RuleStatus Parser::parseTokenRAngleRAngle(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationRAngleRAngle>(gc);
}

Parser::RuleStatus Parser::parseTokenRAngleRAngleRAngle(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationRAngleRAngleRAngle>(gc);
}

Parser::RuleStatus Parser::parseTokenAmp(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationAmp>(gc);
}

Parser::RuleStatus Parser::parseTokenPipe(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationPipe>(gc);
}

Parser::RuleStatus Parser::parseTokenCaret(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationCaret>(gc);
}

Parser::RuleStatus Parser::parseTokenEMark(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationEMark>(gc);
}

Parser::RuleStatus Parser::parseTokenTilde(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationTilde>(gc);
}

Parser::RuleStatus Parser::parseTokenAmpAmp(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationAmpAmp>(gc);
}

Parser::RuleStatus Parser::parseTokenPipePipe(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationPipePipe>(gc);
}

Parser::RuleStatus Parser::parseTokenQMark(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationQMark>(gc);
}

Parser::RuleStatus Parser::parseTokenColon(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationColon>(gc);
}

Parser::RuleStatus Parser::parseTokenEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenPlusEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationPlusEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenMinusEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationMinusEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenStarEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationStarEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenPercentEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationPercentEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenLAngleLAngleEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationLAngleLAngleEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenRAngleRAngleEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationRAngleRAngleEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenRAngleRAngleRAngleEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationRAngleRAngleRAngleEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenAmpEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationAmpEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenPipeEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationPipeEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenCaretEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationCaretEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenEqualRAngle(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationEqualRAngle>(gc);
}

Parser::RuleStatus Parser::parseTokenFSlash(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationFSlash>(gc);
}

Parser::RuleStatus Parser::parseTokenFSlashEqual(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationFSlashEqual>(gc);
}

Parser::RuleStatus Parser::parseTokenAwait(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationAwait>(gc);
}

Parser::RuleStatus Parser::parseTokenBreak(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationBreak>(gc);
}

Parser::RuleStatus Parser::parseTokenCase(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationCase>(gc);
}

Parser::RuleStatus Parser::parseTokenCatch(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationCatch>(gc);
}

Parser::RuleStatus Parser::parseTokenClass(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationClass>(gc);
}

Parser::RuleStatus Parser::parseTokenConst(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationConst>(gc);
}

Parser::RuleStatus Parser::parseTokenContinue(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationContinue>(gc);
}

Parser::RuleStatus Parser::parseTokenDebugger(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationDebugger>(gc);
}

Parser::RuleStatus Parser::parseTokenDefault(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationDefault>(gc);
}

Parser::RuleStatus Parser::parseTokenDelete(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationDelete>(gc);
}

Parser::RuleStatus Parser::parseTokenDo(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationDo>(gc);
}

Parser::RuleStatus Parser::parseTokenElse(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationElse>(gc);
}

Parser::RuleStatus Parser::parseTokenEnum(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationEnum>(gc);
}

Parser::RuleStatus Parser::parseTokenExport(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationExport>(gc);
}

Parser::RuleStatus Parser::parseTokenExtends(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationExtends>(gc);
}

Parser::RuleStatus Parser::parseTokenFalse(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationFalse>(gc);
}

Parser::RuleStatus Parser::parseTokenFinally(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationFinally>(gc);
}

Parser::RuleStatus Parser::parseTokenFor(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationFor>(gc);
}

Parser::RuleStatus Parser::parseTokenFunction(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationFunction>(gc);
}

Parser::RuleStatus Parser::parseTokenIf(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationIf>(gc);
}

Parser::RuleStatus Parser::parseTokenImport(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationImport>(gc);
}

Parser::RuleStatus Parser::parseTokenIn(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationIn>(gc);
}

Parser::RuleStatus Parser::parseTokenInstanceOf(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationInstanceOf>(gc);
}

Parser::RuleStatus Parser::parseTokenLet(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationLet>(gc);
}

Parser::RuleStatus Parser::parseTokenNew(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationNew>(gc);
}

Parser::RuleStatus Parser::parseTokenNull(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationNull>(gc);
}

Parser::RuleStatus Parser::parseTokenReturn(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationReturn>(gc);
}

Parser::RuleStatus Parser::parseTokenSuper(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationSuper>(gc);
}

Parser::RuleStatus Parser::parseTokenSwitch(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationSwitch>(gc);
}

Parser::RuleStatus Parser::parseTokenThis(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationThis>(gc);
}

Parser::RuleStatus Parser::parseTokenThrow(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationThrow>(gc);
}

Parser::RuleStatus Parser::parseTokenTrue(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationTrue>(gc);
}

Parser::RuleStatus Parser::parseTokenTry(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationTry>(gc);
}

Parser::RuleStatus Parser::parseTokenTypeOf(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationTypeOf>(gc);
}

Parser::RuleStatus Parser::parseTokenVar(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationVar>(gc);
}

Parser::RuleStatus Parser::parseTokenVoid(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationVoid>(gc);
}

Parser::RuleStatus Parser::parseTokenWhile(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationWhile>(gc);
}

Parser::RuleStatus Parser::parseTokenWith(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationWith>(gc);
}

Parser::RuleStatus Parser::parseTokenYield(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationYield>(gc);
}

Parser::RuleStatus Parser::parseTokenBooleanLiteral(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationBooleanLiteral>(
        gc,
        [](RuleStatuses &dest, RuleStatuses &src)
        {
            dest.booleanLiteralValue = src.booleanLiteralValue;
        });
}

Parser::RuleStatus Parser::parseTokenNumericLiteral(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationNumericLiteral>(
        gc,
        [](RuleStatuses &dest, RuleStatuses &src)
        {
            dest.numericLiteralValue = src.numericLiteralValue;
        });
}

Parser::RuleStatus Parser::parseTokenStringLiteral(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationStringLiteral>(
        gc,
        [](RuleStatuses &dest, RuleStatuses &src)
        {
            dest.stringLiteralValue = src.stringLiteralValue;
            dest.escapeInString = src.escapeInString;
        });
}

Parser::RuleStatus Parser::parseTokenRegularExpressionLiteral(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationRegularExpressionLiteral>(
        gc,
        [](RuleStatuses &dest, RuleStatuses &src)
        {
            dest.regularExpressionLiteralValue = src.regularExpressionLiteralValue;
            dest.regularExpressionLiteralFlagsValue = src.regularExpressionLiteralFlagsValue;
        });
}

Parser::RuleStatus Parser::parseTokenNoSubstitutionTemplate(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationNoSubstitutionTemplate>(
        gc,
        [](RuleStatuses &dest, RuleStatuses &src)
        {
            dest.templateValue = src.templateValue;
            dest.templateRawValue = src.templateRawValue;
        });
}

Parser::RuleStatus Parser::parseTokenTemplateHead(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationTemplateHead>(
        gc,
        [](RuleStatuses &dest, RuleStatuses &src)
        {
            dest.templateValue = src.templateValue;
            dest.templateRawValue = src.templateRawValue;
        });
}

Parser::RuleStatus Parser::parseTokenTemplateMiddle(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationTemplateMiddle>(
        gc,
        [](RuleStatuses &dest, RuleStatuses &src)
        {
            dest.templateValue = src.templateValue;
            dest.templateRawValue = src.templateRawValue;
        });
}

Parser::RuleStatus Parser::parseTokenTemplateTail(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationTemplateTail>(
        gc,
        [](RuleStatuses &dest, RuleStatuses &src)
        {
            dest.templateValue = src.templateValue;
            dest.templateRawValue = src.templateRawValue;
        });
}

Parser::RuleStatus Parser::parseTokenEndOfFile(GC &gc)
{
    return parseSeperatorAndToken<&Parser::parseTokenizationEndOfFile>(gc);
}

Parser::RuleStatus Parser::parseOrInsertSemicolon(GC &gc,
                                                  bool lineTerminatorAllowed,
                                                  bool isTerminatingSemicolonInDoWhile)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    auto startPosition = currentPosition;
    auto retval = parseTokenizationSeperator(gc);
    if(retval.fail())
        return retval;
    if(statuses.lineSplit && !lineTerminatorAllowed)
    {
        currentPosition = startPosition;
        return RuleStatus::makeSuccess(startPosition, startPosition);
    }
    retval = parseTokenSemicolon(gc);
    if(retval.success())
        return retval;
    if(statuses.lineSplit)
    {
        currentPosition = startPosition;
        return RuleStatus::makeSuccess(startPosition, startPosition);
    }
    if(getCodePoint(currentPosition) == eofCodePoint)
    {
        currentPosition = startPosition;
        return RuleStatus::makeSuccess(startPosition, startPosition);
    }
    if(getCodePoint(currentPosition) == U'}')
    {
        currentPosition = startPosition;
        return RuleStatus::makeSuccess(startPosition, startPosition);
    }
    if(isTerminatingSemicolonInDoWhile)
    {
        currentPosition = startPosition;
        return RuleStatus::makeSuccess(startPosition, startPosition);
    }
    currentPosition = startPosition;
    return retval;
}

Parser::RuleStatus Parser::parseDirectivePrologue(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.directivePrologueStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    std::size_t endPosition = startPosition;
    bool hasUseStrict = false;
    for(;;)
    {
        std::size_t stringLiteralPosition = currentPosition;
        retval = parseTokenStringLiteral(gc);
        if(retval.fail())
            break;
        auto &stringLiteralStatuses = getRuleStatuses(stringLiteralPosition);
        if(stringLiteralStatuses.escapeInString)
            break;
        retval = parseOrInsertSemicolon(gc, true, false);
        if(retval.fail())
            break;
        endPosition = currentPosition;
        HandleScope handleScope(gc);
        Handle<gc::StringReference> stringHandle(gc, stringLiteralStatuses.stringLiteralValue);
        if(gc.readString(stringHandle) == u"use strict")
        {
            if(hasUseStrict)
            {
                gc.writeWarning(Location(SourceHandle(gc, source), stringLiteralPosition),
                                u"multiple 'use strict' directives");
            }
            hasUseStrict = true;
        }
        else
        {
            gc.writeWarning(Location(SourceHandle(gc, source), stringLiteralPosition),
                            u"unrecognized directive");
        }
    }
    currentPosition = endPosition;
    retval = RuleStatus::makeSuccess(startPosition, endPosition);
    statuses.directivePrologueHasUseStrict = hasUseStrict;
    return retval;
}

template <bool hasYield>
Parser::RuleStatus Parser::parseLeftHandSideExpression(GC &gc)
{
    std::size_t startPosition = currentPosition;
    parseTokenIdentifierName(gc);
    std::size_t errorPriorityPosition = currentPosition;
    currentPosition = startPosition;
    return RuleStatus::makeFailure(
        startPosition, errorPriorityPosition, u"implement parseLeftHandSideExpression");
#warning finish
}

template <bool hasIn, bool hasYield>
Parser::RuleStatus Parser::parseConditionalExpression(GC &gc)
{
    std::size_t startPosition = currentPosition;
    parseTokenIdentifierName(gc);
    std::size_t errorPriorityPosition = currentPosition;
    currentPosition = startPosition;
    return RuleStatus::makeFailure(
        startPosition, errorPriorityPosition, u"implement parseConditionalExpression");
#warning finish
}

template <bool hasIn, bool hasYield>
Parser::RuleStatus Parser::parseAssignmentExpression(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.assignmentExpressionStatus[hasIn][hasYield];
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    if(hasYield)
    {
        retval = parseYieldExpression<hasIn>(gc);
        if(retval.success())
            return retval;
        retval /= parseArrowFunction<hasIn, hasYield>(gc);
        if(retval.success())
            return retval;
    }
    else
    {
        retval = parseArrowFunction<hasIn, hasYield>(gc);
        if(retval.success())
            return retval;
    }
    retval /= parseLeftHandSideExpression<hasYield>(gc);
    if(retval.success())
    {
        do
        {
            retval = parseTokenEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenStarEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenFSlashEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenPercentEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenPlusEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenMinusEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenLAngleLAngleEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenRAngleRAngleEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenRAngleRAngleRAngleEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenAmpEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenCaretEqual(gc);
            if(retval.success())
                break;
            retval /= parseTokenPipeEqual(gc);
        } while(false);
        if(retval.success())
        {
            retval = parseAssignmentExpression<hasIn, hasYield>(gc);
            if(retval.success())
                return retval;
        }
    }
    currentPosition = startPosition;
    retval /= parseConditionalExpression<hasIn, hasYield>(gc);
    return retval;
}

template <bool hasIn, bool hasYield>
Parser::RuleStatus Parser::parseExpression(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.expressionStatus[hasIn][hasYield];
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    retval = parseAssignmentExpression<hasIn, hasYield>(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    while(true)
    {
        retval = parseTokenComma(gc);
        if(retval.fail())
            break;
        retval = parseAssignmentExpression<hasIn, hasYield>(gc);
        if(retval.fail())
        {
            currentPosition = startPosition;
            return retval;
        }
    }
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    return retval;
}

template <bool hasYield, bool hasReturn>
Parser::RuleStatus Parser::parseStatement(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.statementStatus[hasYield][hasReturn];
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    retval = parseBlockStatement<hasYield, hasReturn>(gc);
    if(retval.success())
        return retval;
    retval /= parseVariableStatement<hasYield>(gc);
    if(retval.success())
        return retval;
    retval /= parseEmptyStatement(gc);
    if(retval.success())
        return retval;
    retval /= parseExpressionStatement<hasYield>(gc);
    if(retval.success())
        return retval;
#warning finish
    retval /= parseDebuggerStatement(gc);
    if(retval.success())
        return retval;
    return retval;
}

template <bool hasYield>
Parser::RuleStatus Parser::parseDeclaration(GC &gc)
{
    std::size_t startPosition = currentPosition;
    parseTokenIdentifierName(gc);
    std::size_t errorPriorityPosition = currentPosition;
    currentPosition = startPosition;
    return RuleStatus::makeFailure(
        startPosition, errorPriorityPosition, u"implement parseDeclaration");
#warning finish
}

template <bool hasYield, bool hasReturn>
Parser::RuleStatus Parser::parseBlockStatement(GC &gc)
{
    return parseBlock<hasYield, hasReturn>(gc);
}

template <bool hasYield, bool hasReturn>
Parser::RuleStatus Parser::parseBlock(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.blockStatus[hasYield][hasReturn];
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    retval = parseTokenLBrace(gc);
    if(retval.fail())
        return retval;
    retval = parseTokenRBrace(gc);
    if(retval.success())
        return retval;
    retval /= parseStatementList<hasYield, hasReturn>(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    retval = parseTokenRBrace(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    return retval;
}

template <bool hasYield, bool hasReturn>
Parser::RuleStatus Parser::parseStatementList(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.statementListStatus[hasYield][hasReturn];
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    retval = parseStatementListItem<hasYield, hasReturn>(gc);
    if(retval.fail())
        return retval;
    std::size_t endPosition = currentPosition;
    while(true)
    {
        retval = parseStatementListItem<hasYield, hasReturn>(gc);
        if(retval.fail())
        {
            if(parseTokenEndOfFile(gc).success() || parseTokenRBrace(gc).success()
               || parseTokenCase(gc).success() || parseTokenDefault(gc).success())
            {
                break;
            }
            currentPosition = startPosition;
            return retval;
        }
        endPosition = currentPosition;
    }
    currentPosition = endPosition;
    retval = RuleStatus::makeSuccess(startPosition, endPosition);
    return retval;
}

template <bool hasYield, bool hasReturn>
Parser::RuleStatus Parser::parseStatementListItem(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.statementListItemStatus[hasYield][hasReturn];
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    retval = parseDeclaration<hasYield>(gc);
    if(retval.success())
        return retval;
    retval /= parseStatement<hasYield, hasReturn>(gc);
    return retval;
}

template <bool hasYield>
Parser::RuleStatus Parser::parseVariableStatement(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.variableStatementStatus[hasYield];
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    retval = parseTokenVar(gc);
    if(retval.fail())
        return retval;
    retval = parseVariableDeclarationList<true, hasYield>(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    retval = parseOrInsertSemicolon(gc, true, false);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    return retval;
}

template <bool hasIn, bool hasYield>
Parser::RuleStatus Parser::parseVariableDeclarationList(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.variableDeclarationListStatus[hasIn][hasYield];
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    retval = parseVariableDeclaration<hasIn, hasYield>(gc);
    if(retval.fail())
        return retval;
    while(true)
    {
        retval = parseTokenComma(gc);
        if(retval.fail())
            break;
        retval = parseVariableDeclaration<hasIn, hasYield>(gc);
        if(retval.fail())
        {
            currentPosition = startPosition;
            return retval;
        }
    }
    retval = RuleStatus::makeSuccess(startPosition, currentPosition);
    return retval;
}

template <bool hasIn, bool hasYield>
Parser::RuleStatus Parser::parseVariableDeclaration(GC &gc)
{
    std::size_t startPosition = currentPosition;
    parseTokenIdentifierName(gc);
    std::size_t errorPriorityPosition = currentPosition;
    currentPosition = startPosition;
    parseTokenLBrace(gc);
    if(currentPosition > errorPriorityPosition)
        errorPriorityPosition = currentPosition;
    currentPosition = startPosition;
    parseTokenLBracket(gc);
    if(currentPosition > errorPriorityPosition)
        errorPriorityPosition = currentPosition;
    currentPosition = startPosition;
    return RuleStatus::makeFailure(
        startPosition, errorPriorityPosition, u"implement parseVariableDeclaration");
#warning finish
}

Parser::RuleStatus Parser::parseEmptyStatement(GC &gc)
{
    return parseTokenSemicolon(gc);
}

template <bool hasYield>
Parser::RuleStatus Parser::parseExpressionStatement(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.expressionStatementStatus[hasYield];
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    if(parseTokenLBrace(gc).success() || parseTokenFunction(gc).success()
       || parseTokenClass(gc).success())
    {
        std::size_t errorPriorityPosition = currentPosition;
        currentPosition = startPosition;
        retval = RuleStatus::makeFailure(
            startPosition, errorPriorityPosition, u"missing expression statement");
        return retval;
    }
    if(parseTokenLet(gc).success())
    {
        if(parseTokenLBracket(gc).success())
        {
            std::size_t errorPriorityPosition = currentPosition;
            currentPosition = startPosition;
            retval = RuleStatus::makeFailure(
                startPosition, errorPriorityPosition, u"missing expression statement");
            return retval;
        }
        currentPosition = startPosition;
    }
    retval = parseExpression<true, hasYield>(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    retval = parseOrInsertSemicolon(gc, true, false);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    return retval;
}

Parser::RuleStatus Parser::parseDebuggerStatement(GC &gc)
{
    std::size_t startPosition = currentPosition;
    RuleStatus retval = parseTokenDebugger(gc);
    if(retval.fail())
        return retval;
    retval = parseOrInsertSemicolon(gc, true, false);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    return RuleStatus::makeSuccess(startPosition, currentPosition);
}

Parser::RuleStatus Parser::parseScript(GC &gc)
{
    RuleStatuses &statuses = getRuleStatuses(currentPosition);
    RuleStatus &retval = statuses.scriptStatus;
    if(!retval.empty())
    {
        if(retval.success())
            currentPosition = retval.endPositionOrErrorPriorityPosition;
        return retval;
    }
    std::size_t startPosition = currentPosition;
    retval = parseTokenEndOfFile(gc);
    if(retval.success())
        return retval;
    retval = parseScriptBody(gc);
    if(retval.fail())
        return retval;
    retval = parseTokenEndOfFile(gc);
    if(retval.fail())
    {
        currentPosition = startPosition;
        return retval;
    }
    return retval;
#warning finish
}

template <bool hasIn, bool hasYield>
Parser::RuleStatus Parser::parseArrowFunction(GC &gc)
{
    std::size_t startPosition = currentPosition;
    parseTokenIdentifierName(gc);
    std::size_t errorPriorityPosition = currentPosition;
    currentPosition = startPosition;
    return RuleStatus::makeFailure(
        startPosition, errorPriorityPosition, u"implement parseArrowFunction");
#warning finish
}

template <bool hasIn>
Parser::RuleStatus Parser::parseYieldExpression(GC &gc)
{
    std::size_t startPosition = currentPosition;
    auto retval = parseTokenYield(gc);
    if(retval.fail())
    {
        return retval;
    }
    std::size_t errorPriorityPosition = currentPosition;
    currentPosition = startPosition;
    return RuleStatus::makeFailure(
        startPosition, errorPriorityPosition, u"implement parseYieldExpression");
#warning finish
}

void Parser::translateScript(GC &gc)
{
    constexpr_assert(false);
#warning finish
}

Parser::RuleStatus Parser::parseScriptBody(GC &gc)
{
    return parseStatementList<false, false>(gc);
}

void Parser::test(GC &gc)
{
    while(testParseToken(false, false, gc))
    {
    }
}

value::ObjectHandle Parser::mainParseScript(SourceHandle source, GC &gc)
{
    HandleScope handleScope(gc);
    auto codeEmitterUniquePtr = std::unique_ptr<CodeEmitter>(new CodeEmitter());
    auto &codeEmitter = *codeEmitterUniquePtr;
    auto codeEmitterObject =
        value::ObjectHandle::create(std::move(codeEmitterUniquePtr), value::NullHandle(), gc);
    static_cast<void>(codeEmitterObject);
    auto astTranslatorUniquePtr = std::unique_ptr<ASTTranslator>(new ASTTranslator(codeEmitter));
    auto &astTranslator = *astTranslatorUniquePtr;
    auto astTranslatorObject =
        value::ObjectHandle::create(std::move(astTranslatorUniquePtr), value::NullHandle(), gc);
    static_cast<void>(astTranslatorObject);
    auto parserUniquePtr = std::unique_ptr<Parser>(new Parser(astTranslator, source));
    auto &parser = *parserUniquePtr;
    auto parserObject =
        value::ObjectHandle::create(std::move(parserUniquePtr), value::NullHandle(), gc);
    static_cast<void>(parserObject);
    auto status = parser.parseScript(gc);
    if(status.fail())
        value::ObjectHandle::throwSyntaxError(
            status.errorMessage,
            LocationHandle(gc, Location(source, status.startPositionOrErrorPosition)),
            gc);

    return handleScope.escapeHandle(
        codeEmitter.finishGlobalCode(Location(source, source.get()->contents.size()), gc));
}

void Parser::mainTestParse(SourceHandle source, GC &gc)
{
    HandleScope handleScope(gc);
    auto codeEmitterUniquePtr = std::unique_ptr<CodeEmitter>(new CodeEmitter());
    auto &codeEmitter = *codeEmitterUniquePtr;
    auto codeEmitterObject =
        value::ObjectHandle::create(std::move(codeEmitterUniquePtr), value::NullHandle(), gc);
    static_cast<void>(codeEmitterObject);
    auto astTranslatorUniquePtr = std::unique_ptr<ASTTranslator>(new ASTTranslator(codeEmitter));
    auto &astTranslator = *astTranslatorUniquePtr;
    auto astTranslatorObject =
        value::ObjectHandle::create(std::move(astTranslatorUniquePtr), value::NullHandle(), gc);
    static_cast<void>(astTranslatorObject);
    auto parserUniquePtr = std::unique_ptr<Parser>(new Parser(astTranslator, source));
    auto &parser = *parserUniquePtr;
    auto parserObject =
        value::ObjectHandle::create(std::move(parserUniquePtr), value::NullHandle(), gc);
    static_cast<void>(parserObject);
    parser.test(gc);
}
}
}
