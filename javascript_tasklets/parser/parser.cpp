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

struct Parser final : public gc::Object::ExtraData
{
    Parser(const Parser &) = delete;
    Parser &operator=(const Parser &) = delete;

public:
    struct RuleStatus final
    {
        static constexpr std::size_t npos = static_cast<std::size_t>(-1);
        std::size_t startPositionOrErrorPosition;
        std::size_t endPositionOrErrorPriorityPosition;
        const char16_t *errorMessage;
        constexpr bool success() const noexcept
        {
            return errorMessage == nullptr && startPositionOrErrorPosition != npos;
        }
        constexpr bool empty() const noexcept
        {
            return errorMessage == nullptr && startPositionOrErrorPosition == npos;
        }
        constexpr bool fail() const noexcept
        {
            return errorMessage != nullptr;
        }

    private:
        constexpr RuleStatus(std::size_t startPositionOrErrorPosition,
                             std::size_t endPositionOrErrorPriorityPosition,
                             const char16_t *errorMessage) noexcept
            : startPositionOrErrorPosition(startPositionOrErrorPosition),
              endPositionOrErrorPriorityPosition(endPositionOrErrorPriorityPosition),
              errorMessage(errorMessage)
        {
        }

    public:
        constexpr RuleStatus() noexcept : startPositionOrErrorPosition(npos),
                                          endPositionOrErrorPriorityPosition(npos),
                                          errorMessage(nullptr)
        {
        }
        static constexpr RuleStatus makeSuccess(std::size_t startPosition,
                                                std::size_t endPosition) noexcept
        {
            return constexpr_assert(startPosition <= endPosition && startPosition != npos
                                    && endPosition != npos),
                   RuleStatus(startPosition, endPosition, nullptr);
        }
        static constexpr RuleStatus makeFailure(std::size_t errorPosition,
                                                std::size_t errorPriorityPosition,
                                                const char16_t *errorMessage) noexcept
        {
            return constexpr_assert(errorMessage),
                   RuleStatus(errorPosition, errorPriorityPosition, errorMessage);
        }
        constexpr RuleStatus operator/(RuleStatus rt) noexcept
        {
            return constexpr_assert(fail() || rt.fail()), constexpr_assert(!empty() && !rt.empty()),
                   !fail() ? *this : !rt.fail() ? rt :
                                                  endPositionOrErrorPriorityPosition
                                                          < rt.endPositionOrErrorPriorityPosition ?
                                                  rt :
                                                  *this;
        }
        RuleStatus &operator/=(RuleStatus rt) noexcept
        {
            return operator=(operator/(rt));
        }
    };
    struct RuleStatuses final
    {
        gc::StringReference identifierNameValue;
        double numericLiteralValue = 0;
        bool booleanLiteralValue = false;
        gc::StringReference stringLiteralValue;
        gc::StringReference regularExpressionLiteralValue;
        gc::StringReference regularExpressionLiteralFlagsValue;
        gc::StringReference templateRawValue;
        gc::StringReference templateValue;
        bool lineSplit = false;
        RuleStatus tokenizationIdentifierNameStatus;
        RuleStatus tokenizationIdentifierStatus;
        RuleStatus tokenizationEscapelessIdentifierNameStatus;
        RuleStatus tokenizationStringLiteralStatus;
        RuleStatus tokenizationSeperatorStatus;
        RuleStatus tokenizationKeywordStatus;
        RuleStatus tokenizationFutureReservedWordStatus;
        RuleStatus tokenizationBooleanLiteralStatus;
        RuleStatus tokenizationReservedWordStatus;
        RuleStatus tokenizationNumericLiteralStatus;
        RuleStatus tokenizationRegularExpressionLiteralStatus;
        RuleStatus tokenizationTemplateCharactersStatus;
        RuleStatus tokenizationNoSubstitutionTemplateStatus;
        RuleStatus tokenizationTemplateHeadStatus;
        RuleStatus tokenizationTemplateTailStatus;
        RuleStatus tokenizationTemplateMiddleStatus;
        void getGCReferences(gc::GCReferencesCallback &callback) const
        {
            callback(identifierNameValue);
            callback(stringLiteralValue);
            callback(regularExpressionLiteralValue);
            callback(regularExpressionLiteralFlagsValue);
            callback(templateRawValue);
            callback(templateValue);
        }
    };

public:
    static constexpr std::uint32_t eofCodePoint = 0xFFFFFFFFUL;

public:
    ASTTranslator &astTranslator;
    std::vector<std::unique_ptr<RuleStatuses>> ruleStatusesArray;
    gc::SourceReference source;
    std::size_t currentPosition;

public:
    explicit Parser(ASTTranslator &astTranslator, SourceHandle source)
        : astTranslator(astTranslator),
          ruleStatusesArray(source.get()->contents.size() + 1), // one extra for EndOfFile
          source(source.get()),
          currentPosition(0)
    {
    }
    virtual std::unique_ptr<ExtraData> clone() const override
    {
        constexpr_assert(!"Parser::clone called");
        return std::unique_ptr<ExtraData>();
    }
    virtual void getGCReferences(gc::GCReferencesCallback &callback) const override
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
    RuleStatuses &getRuleStatuses(std::size_t position)
    {
        constexpr_assert(position < ruleStatusesArray.size());
        auto &retval = ruleStatusesArray[position];
        if(!retval)
            retval = std::unique_ptr<RuleStatuses>(new RuleStatuses);
        return *retval;
    }
    std::uint32_t getCodePoint(std::size_t position)
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
    std::uint32_t getCodePoint(std::size_t position, std::size_t &nextPosition)
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
    RuleStatus parseTokenizationLineTerminatorSequence(GC &gc)
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
    RuleStatus parseTokenizationSeperator(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenizationSeperatorStatus;
        if(!retval.empty())
        {
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
        return retval;
    }
    RuleStatus parseTokenizationUnicodeEscapeSequence(GC &gc, std::uint32_t &value)
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
                    return RuleStatus::makeFailure(errorPosition,
                                                   errorPosition,
                                                   u"unicode escape sequence value out of range");
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
            return RuleStatus::makeFailure(errorPosition,
                                           errorPosition,
                                           u"unicode escape sequence is missing { or a hex digit");
        }
    }
    RuleStatus parseTokenizationHexEscapeSequence(GC &gc, std::uint32_t &value)
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
    RuleStatus parseTokenizationEscapeSequence(GC &gc, std::uint32_t &value)
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
    RuleStatus parseTokenizationIdentifierName(GC &gc)
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
    RuleStatus parseTokenizationEscapelessIdentifierName(GC &gc)
    {
        std::size_t startPosition = currentPosition;
        parseTokenizationIdentifierName(gc);
        auto retval = getRuleStatuses(startPosition).tokenizationEscapelessIdentifierNameStatus;
        if(retval.fail())
            currentPosition = startPosition;
        return retval;
    }
    RuleStatus parseTokenizationKeyword(GC &gc, const char16_t *text, const char16_t *errorMessage)
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
    RuleStatus parseTokenizationAwait(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"await", u"missing await keyword");
    }
    RuleStatus parseTokenizationBreak(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"break", u"missing break keyword");
    }
    RuleStatus parseTokenizationCase(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"case", u"missing case keyword");
    }
    RuleStatus parseTokenizationCatch(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"catch", u"missing catch keyword");
    }
    RuleStatus parseTokenizationClass(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"class", u"missing class keyword");
    }
    RuleStatus parseTokenizationConst(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"const", u"missing const keyword");
    }
    RuleStatus parseTokenizationContinue(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"continue", u"missing continue keyword");
    }
    RuleStatus parseTokenizationDebugger(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"debugger", u"missing debugger keyword");
    }
    RuleStatus parseTokenizationDefault(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"default", u"missing default keyword");
    }
    RuleStatus parseTokenizationDelete(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"delete", u"missing delete keyword");
    }
    RuleStatus parseTokenizationDo(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"do", u"missing do keyword");
    }
    RuleStatus parseTokenizationElse(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"else", u"missing else keyword");
    }
    RuleStatus parseTokenizationEnum(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"enum", u"missing enum keyword");
    }
    RuleStatus parseTokenizationExport(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"export", u"missing export keyword");
    }
    RuleStatus parseTokenizationExtends(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"extends", u"missing extends keyword");
    }
    RuleStatus parseTokenizationFalse(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"false", u"missing false keyword");
    }
    RuleStatus parseTokenizationFinally(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"finally", u"missing finally keyword");
    }
    RuleStatus parseTokenizationFor(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"for", u"missing for keyword");
    }
    RuleStatus parseTokenizationFunction(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"function", u"missing function keyword");
    }
    RuleStatus parseTokenizationIf(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"if", u"missing if keyword");
    }
    RuleStatus parseTokenizationImport(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"import", u"missing import keyword");
    }
    RuleStatus parseTokenizationIn(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"in", u"missing in keyword");
    }
    RuleStatus parseTokenizationInstanceOf(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"instanceof", u"missing instanceof keyword");
    }
    RuleStatus parseTokenizationNew(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"new", u"missing new keyword");
    }
    RuleStatus parseTokenizationNull(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"null", u"missing null keyword");
    }
    RuleStatus parseTokenizationReturn(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"return", u"missing return keyword");
    }
    RuleStatus parseTokenizationSuper(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"super", u"missing super keyword");
    }
    RuleStatus parseTokenizationSwitch(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"switch", u"missing switch keyword");
    }
    RuleStatus parseTokenizationThis(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"this", u"missing this keyword");
    }
    RuleStatus parseTokenizationThrow(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"throw", u"missing throw keyword");
    }
    RuleStatus parseTokenizationTrue(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"true", u"missing true keyword");
    }
    RuleStatus parseTokenizationTry(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"try", u"missing try keyword");
    }
    RuleStatus parseTokenizationTypeOf(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"typeof", u"missing typeof keyword");
    }
    RuleStatus parseTokenizationVar(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"var", u"missing var keyword");
    }
    RuleStatus parseTokenizationVoid(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"void", u"missing void keyword");
    }
    RuleStatus parseTokenizationWhile(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"while", u"missing while keyword");
    }
    RuleStatus parseTokenizationWith(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"with", u"missing with keyword");
    }
    RuleStatus parseTokenizationYield(GC &gc)
    {
        return parseTokenizationKeyword(gc, u"yield", u"missing yield keyword");
    }
    RuleStatus parseTokenizationKeyword(GC &gc)
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
    RuleStatus parseTokenizationFutureReservedWord(GC &gc)
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
    RuleStatus parseTokenizationBooleanLiteral(GC &gc)
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
    RuleStatus parseTokenizationReservedWord(GC &gc)
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
    RuleStatus parseTokenizationIdentifier(GC &gc)
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
    RuleStatus parseTokenizationNumericLiteral(GC &gc)
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
            retval =
                RuleStatus::makeFailure(startPosition, startPosition, u"missing numeric literal");
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
                    retval =
                        RuleStatus::makeFailure(currentPosition,
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
                    retval = RuleStatus::makeFailure(currentPosition,
                                                     currentPosition,
                                                     u"octal numeric literal missing octal digit");
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
    RuleStatus parseTokenizationStringLiteral(GC &gc)
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
            retval =
                RuleStatus::makeFailure(startPosition, currentPosition, u"missing string literal");
            currentPosition = startPosition;
            return retval;
        }
        const auto enclosingQuoteCodePoint = codePoint;
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        while(codePoint != enclosingQuoteCodePoint)
        {
            if(codePoint == eofCodePoint
               || character_properties::javascriptLineTerminator(codePoint))
            {
                retval = RuleStatus::makeFailure(
                    startPosition, currentPosition, u"string literal missing closing quote");
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
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    RuleStatus parseTokenizationRegularExpressionLiteral(GC &gc)
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
            if(codePoint == eofCodePoint
               || character_properties::javascriptLineTerminator(codePoint))
            {
                retval =
                    RuleStatus::makeFailure(currentPosition,
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
            retval = RuleStatus::makeFailure(
                currentPosition,
                currentPosition,
                u"unicode escape not permitted in regular expression flags");
            currentPosition = startPosition;
            return retval;
        }
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    RuleStatus parseTokenizationTemplateCharacters(GC &gc)
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
    RuleStatus parseTokenizationNoSubstitutionTemplate(GC &gc)
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
            retval = RuleStatus::makeFailure(
                currentPosition, currentPosition, u"missing template literal");
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
            retval =
                RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        statuses.templateValue = templateCharactersStatuses.templateValue;
        statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    RuleStatus parseTokenizationTemplateHead(GC &gc)
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
            retval = RuleStatus::makeFailure(
                currentPosition, currentPosition, u"missing template literal");
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
            retval =
                RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'{')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        statuses.templateValue = templateCharactersStatuses.templateValue;
        statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    RuleStatus parseTokenizationTemplateTail(GC &gc)
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
            retval =
                RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        statuses.templateValue = templateCharactersStatuses.templateValue;
        statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    RuleStatus parseTokenizationTemplateMiddle(GC &gc)
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
            retval =
                RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'{')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        statuses.templateValue = templateCharactersStatuses.templateValue;
        statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    enum class Punctuator
    {
        LBrace,
        RBrace,
        LParen,
        RParen,
        LBracket,
        RBracket,
        Period,
        Ellipsis,
        Semicolon,
        Comma,
        LAngle,
        RAngle,
        LAngleEqual,
        RAngleEqual,
        EqualEqual,
        EMarkEqual,
        EqualEqualEqual,
        EMarkEqualEqual,
        Plus,
        Minus,
        Star,
        Percent,
        PlusPlus,
        MinusMinus,
        LAngleLAngle,
        RAngleRAngle,
        RAngleRAngleRAngle,
        Amp,
        Pipe,
        Caret,
        EMark,
        Tilde,
        AmpAmp,
        PipePipe,
        QMark,
        Colon,
        Equal,
        PlusEqual,
        MinusEqual,
        StarEqual,
        PercentEqual,
        LAngleLAngleEqual,
        RAngleRAngleEqual,
        RAngleRAngleRAngleEqual,
        AmpEqual,
        PipeEqual,
        CaretEqual,
        EqualRAngle,
        FSlash,
        FSlashEqual,
    };
    RuleStatus parseTokenizationPunctuator(GC &gc, Punctuator &punctuator)
    {
        std::size_t startPosition = currentPosition;
        if(parseTokenizationNumericLiteral(gc).success())
        {
            auto errorPriorityPosition = currentPosition;
            currentPosition = startPosition;
            return RuleStatus::makeFailure(
                startPosition, errorPriorityPosition, u"missing punctuator");
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
    RuleStatus parseTokenizationPunctuator(GC &gc,
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
    RuleStatus parseTokenizationLBrace(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::LBrace, u"missing {");
    }
    RuleStatus parseTokenizationRBrace(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::RBrace, u"missing }");
    }
    RuleStatus parseTokenizationLParen(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::LParen, u"missing (");
    }
    RuleStatus parseTokenizationRParen(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::RParen, u"missing )");
    }
    RuleStatus parseTokenizationLBracket(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::LBracket, u"missing [");
    }
    RuleStatus parseTokenizationRBracket(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::RBracket, u"missing ]");
    }
    RuleStatus parseTokenizationPeriod(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Period, u"missing .");
    }
    RuleStatus parseTokenizationEllipsis(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Ellipsis, u"missing ...");
    }
    RuleStatus parseTokenizationSemicolon(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Semicolon, u"missing ;");
    }
    RuleStatus parseTokenizationComma(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Comma, u"missing ,");
    }
    RuleStatus parseTokenizationLAngle(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::LAngle, u"missing <");
    }
    RuleStatus parseTokenizationRAngle(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::RAngle, u"missing >");
    }
    RuleStatus parseTokenizationLAngleEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::LAngleEqual, u"missing <=");
    }
    RuleStatus parseTokenizationRAngleEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::RAngleEqual, u"missing >=");
    }
    RuleStatus parseTokenizationEqualEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::EqualEqual, u"missing ==");
    }
    RuleStatus parseTokenizationEMarkEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::EMarkEqual, u"missing !=");
    }
    RuleStatus parseTokenizationEqualEqualEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::EqualEqualEqual, u"missing ===");
    }
    RuleStatus parseTokenizationEMarkEqualEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::EMarkEqualEqual, u"missing !==");
    }
    RuleStatus parseTokenizationPlus(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Plus, u"missing +");
    }
    RuleStatus parseTokenizationMinus(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Minus, u"missing -");
    }
    RuleStatus parseTokenizationStar(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Star, u"missing *");
    }
    RuleStatus parseTokenizationPercent(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Percent, u"missing %");
    }
    RuleStatus parseTokenizationPlusPlus(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::PlusPlus, u"missing ++");
    }
    RuleStatus parseTokenizationMinusMinus(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::MinusMinus, u"missing --");
    }
    RuleStatus parseTokenizationLAngleLAngle(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::LAngleLAngle, u"missing <<");
    }
    RuleStatus parseTokenizationRAngleRAngle(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::RAngleRAngle, u"missing >>");
    }
    RuleStatus parseTokenizationRAngleRAngleRAngle(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::RAngleRAngleRAngle, u"missing >>>");
    }
    RuleStatus parseTokenizationAmp(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Amp, u"missing &");
    }
    RuleStatus parseTokenizationPipe(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Pipe, u"missing |");
    }
    RuleStatus parseTokenizationCaret(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Caret, u"missing ^");
    }
    RuleStatus parseTokenizationEMark(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::EMark, u"missing !");
    }
    RuleStatus parseTokenizationTilde(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Tilde, u"missing ~");
    }
    RuleStatus parseTokenizationAmpAmp(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::AmpAmp, u"missing &&");
    }
    RuleStatus parseTokenizationPipePipe(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::PipePipe, u"missing ||");
    }
    RuleStatus parseTokenizationQMark(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::QMark, u"missing ?");
    }
    RuleStatus parseTokenizationColon(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Colon, u"missing :");
    }
    RuleStatus parseTokenizationEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::Equal, u"missing =");
    }
    RuleStatus parseTokenizationPlusEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::PlusEqual, u"missing +=");
    }
    RuleStatus parseTokenizationMinusEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::MinusEqual, u"missing -=");
    }
    RuleStatus parseTokenizationStarEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::StarEqual, u"missing *=");
    }
    RuleStatus parseTokenizationPercentEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::PercentEqual, u"missing %=");
    }
    RuleStatus parseTokenizationLAngleLAngleEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::LAngleLAngleEqual, u"missing <<=");
    }
    RuleStatus parseTokenizationRAngleRAngleEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::RAngleRAngleEqual, u"missing >>=");
    }
    RuleStatus parseTokenizationRAngleRAngleRAngleEqual(GC &gc)
    {
        return parseTokenizationPunctuator(
            gc, Punctuator::RAngleRAngleRAngleEqual, u"missing >>>=");
    }
    RuleStatus parseTokenizationAmpEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::AmpEqual, u"missing &=");
    }
    RuleStatus parseTokenizationPipeEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::PipeEqual, u"missing |=");
    }
    RuleStatus parseTokenizationCaretEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::CaretEqual, u"missing ^=");
    }
    RuleStatus parseTokenizationEqualRAngle(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::EqualRAngle, u"missing =>");
    }
    RuleStatus parseTokenizationFSlash(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::FSlash, u"missing /");
    }
    RuleStatus parseTokenizationFSlashEqual(GC &gc)
    {
        return parseTokenizationPunctuator(gc, Punctuator::FSlashEqual, u"missing /=");
    }
    bool testParseToken(bool parseTemplateContinuation, bool parseRegularExpression, GC &gc)
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
                ss << "StringLiteral: stringLiteralValue = ";
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
                    writeString(
                        ss,
                        gc.readString(Handle<gc::StringReference>(
                            gc, getRuleStatuses(startPosition).regularExpressionLiteralValue)));
                    ss << " regularExpressionLiteralFlagsValue = ";
                    writeString(
                        ss,
                        gc.readString(Handle<gc::StringReference>(
                            gc,
                            getRuleStatuses(startPosition).regularExpressionLiteralFlagsValue)));
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
                writeString(ss,
                            Location(SourceHandle(gc, source), result.startPositionOrErrorPosition)
                                .toString());
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
    template <RuleStatus (Parser::*parseTokenization)(GC &gc)>
    RuleStatus parseSeperatorAndToken(GC &gc,
                                      void (*finalCopy)(RuleStatuses &dest,
                                                        RuleStatuses &src) = nullptr)
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
        retval = parseTokenizationSeperator(gc);
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
    RuleStatus parseIdentifierName(GC &gc)
    {
        return parseSeperatorAndToken<&Parser::parseTokenizationIdentifierName>(
            gc,
            [](RuleStatuses &dest, RuleStatuses &src)
            {
                dest.identifierNameValue = src.identifierNameValue;
            });
    }
    RuleStatus parseIdentifier(GC &gc)
    {
        return parseSeperatorAndToken<&Parser::parseTokenizationIdentifier>(
            gc,
            [](RuleStatuses &dest, RuleStatuses &src)
            {
                dest.identifierNameValue = src.identifierNameValue;
            });
    }
    RuleStatus parseScript(GC &gc)
    {
        constexpr_assert(false);
#warning finish
        return RuleStatus();
    }
    void translateScript(GC &gc)
    {
        constexpr_assert(false);
#warning finish
    }
    void test(GC &gc)
    {
        while(testParseToken(false, false, gc))
        {
        }
    }
};

value::ObjectHandle parseScript(SourceHandle source, GC &gc)
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
    parser.parseScript(gc);
    return handleScope.escapeHandle(
        codeEmitter.finishGlobalCode(Location(source, source.get()->contents.size()), gc));
}

void testParse(SourceHandle source, GC &gc)
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
