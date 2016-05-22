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
        constexpr Value() noexcept : type(Type::Empty),
                                     baseOrValue(nullptr),
                                     referencedName(nullptr),
                                     thisValue(nullptr)
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
            return Value(Type::Value, value, nullptr, nullptr);
        }
        static constexpr Value makeNonstrictObjectReference(RegisterIndex object,
                                                            RegisterIndex referencedName)
        {
            return Value(Type::NonstrictObjectReference, object, referencedName, nullptr);
        }
        static constexpr Value makeStrictObjectReference(RegisterIndex object,
                                                         RegisterIndex referencedName)
        {
            return Value(Type::StrictObjectReference, object, referencedName, nullptr);
        }
        static constexpr Value makeObjectReference(RegisterIndex object,
                                                   RegisterIndex referencedName,
                                                   bool strict)
        {
            return Value(strict ? Type::StrictObjectReference : Type::NonstrictObjectReference,
                         object,
                         referencedName,
                         nullptr);
        }
        static constexpr Value makeNonstrictEnvironmentReference(RegisterIndex environment,
                                                                 RegisterIndex referencedName)
        {
            return Value(Type::NonstrictEnvironmentReference, environment, referencedName, nullptr);
        }
        static constexpr Value makeStrictEnvironmentReference(RegisterIndex environment,
                                                              RegisterIndex referencedName)
        {
            return Value(Type::StrictEnvironmentReference, environment, referencedName, nullptr);
        }
        static constexpr Value makeEnvironmentReference(RegisterIndex environment,
                                                        RegisterIndex referencedName,
                                                        bool strict)
        {
            return Value(
                strict ? Type::StrictEnvironmentReference : Type::NonstrictEnvironmentReference,
                environment,
                referencedName,
                nullptr);
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
        std::size_t startPosition;
        std::size_t endPosition;
        const char16_t *errorMessage;
        constexpr bool success() const noexcept
        {
            return errorMessage == nullptr && startPosition != npos;
        }
        constexpr bool empty() const noexcept
        {
            return errorMessage == nullptr && startPosition == npos;
        }
        constexpr bool fail() const noexcept
        {
            return errorMessage != nullptr;
        }

    private:
        constexpr RuleStatus(std::size_t startPosition,
                             std::size_t endPosition,
                             const char16_t *errorMessage) noexcept : startPosition(startPosition),
                                                                      endPosition(endPosition),
                                                                      errorMessage(errorMessage)
        {
        }

    public:
        constexpr RuleStatus() noexcept : startPosition(npos),
                                          endPosition(npos),
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
        static constexpr RuleStatus makeFailure(std::size_t position,
                                                const char16_t *errorMessage) noexcept
        {
            return constexpr_assert(errorMessage), RuleStatus(position, position, errorMessage);
        }
        constexpr RuleStatus operator/(RuleStatus rt) noexcept
        {
            return constexpr_assert(fail() || rt.fail()), constexpr_assert(!empty() && !rt.empty()),
                   !fail() ? *this : !rt.fail() ? rt : startPosition < rt.startPosition ? rt :
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
        RuleStatus tokenIdentifierNameStatus;
        RuleStatus tokenIdentifierStatus;
        RuleStatus tokenEscapelessIdentifierNameStatus;
        RuleStatus tokenStringLiteralStatus;
        RuleStatus tokenSeperatorStatus;
        RuleStatus tokenNoNewlineSeperatorStatus;
        RuleStatus tokenKeywordStatus;
        RuleStatus tokenFutureReservedWordStatus;
        RuleStatus tokenBooleanLiteralStatus;
        RuleStatus tokenReservedWordStatus;
        RuleStatus tokenNumericLiteralStatus;
        RuleStatus tokenRegularExpressionLiteralStatus;
        RuleStatus tokenTemplateCharactersStatus;
        RuleStatus tokenNoSubstitutionTemplateStatus;
        RuleStatus tokenTemplateHeadStatus;
        RuleStatus tokenTemplateTailStatus;
        RuleStatus tokenTemplateMiddleStatus;
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
    RuleStatus parseTokenLineTerminatorSequence(GC &gc)
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
        return RuleStatus::makeFailure(startPosition, u"missing line terminator sequence");
    }
    RuleStatus parseTokenSeperator(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenSeperatorStatus;
        if(!retval.empty())
        {
            constexpr_assert(!statuses.tokenNoNewlineSeperatorStatus.empty());
            return retval;
        }
        constexpr_assert(statuses.tokenNoNewlineSeperatorStatus.empty());
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
                            retval = RuleStatus::makeFailure(startPosition,
                                                             u"comment missing closing */");
                            statuses.tokenNoNewlineSeperatorStatus = retval;
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
        if(gotNewLine)
            statuses.tokenNoNewlineSeperatorStatus = retval;
        else
            statuses.tokenNoNewlineSeperatorStatus =
                RuleStatus::makeFailure(startPosition, u"line split not allowed here");
        return retval;
    }
    RuleStatus parseTokenNoNewlineSeperator(GC &gc)
    {
        std::size_t startPosition = currentPosition;
        parseTokenSeperator(gc);
        auto retval = getRuleStatuses(startPosition).tokenNoNewlineSeperatorStatus;
        if(retval.fail())
            currentPosition = startPosition;
        return retval;
    }
    RuleStatus parseTokenUnicodeEscapeSequence(GC &gc, std::uint32_t &value)
    {
        std::size_t startPosition = currentPosition;
        if(getCodePoint(currentPosition, currentPosition) != U'u')
        {
            auto errorPosition = startPosition;
            currentPosition = startPosition;
            return RuleStatus::makeFailure(errorPosition, u"unicode escape sequence is missing u");
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
                return RuleStatus::makeFailure(errorPosition,
                                               u"unicode escape sequence missing hex digit");
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
                                                   u"unicode escape sequence value out of range");
                }
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
            }
            if(codePoint != U'}')
            {
                auto errorPosition = currentPosition;
                currentPosition = startPosition;
                return RuleStatus::makeFailure(errorPosition,
                                               u"unicode escape sequence missing closing }");
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
                    return RuleStatus::makeFailure(errorPosition,
                                                   u"unicode escape sequence missing hex digit");
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
                                           u"unicode escape sequence is missing { or a hex digit");
        }
    }
    RuleStatus parseTokenHexEscapeSequence(GC &gc, std::uint32_t &value)
    {
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'x')
        {
            auto errorPosition = startPosition;
            currentPosition = startPosition;
            return RuleStatus::makeFailure(errorPosition, u"hex escape sequence is missing x");
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
                return RuleStatus::makeFailure(errorPosition,
                                               u"hex escape sequence missing hex digit");
            }
            value *= 0x10;
            value += character_properties::javascriptDigitValue(codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
        return RuleStatus::makeSuccess(startPosition, currentPosition);
    }
    RuleStatus parseTokenEscapeSequence(GC &gc, std::uint32_t &value)
    {
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        switch(codePoint)
        {
        case U'u':
            return parseTokenUnicodeEscapeSequence(gc, value);
        case U'x':
            return parseTokenHexEscapeSequence(gc, value);
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
                auto errorPosition = startPosition;
                currentPosition = startPosition;
                return RuleStatus::makeFailure(errorPosition, u"invalid escape sequence");
            }
            return RuleStatus::makeSuccess(startPosition, currentPosition);
        default:
            if(character_properties::javascriptLineTerminator(codePoint)
               || character_properties::javascriptDecimalDigit(codePoint))
            {
                auto errorPosition = startPosition;
                currentPosition = startPosition;
                return RuleStatus::makeFailure(errorPosition, u"invalid escape sequence");
            }
            value = codePoint;
            currentPosition = nextPosition;
            return RuleStatus::makeSuccess(startPosition, currentPosition);
        }
    }
    RuleStatus parseTokenIdentifierName(GC &gc)
    {
        HandleScope handleScope(gc);
        auto &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenIdentifierNameStatus;
        if(!retval.empty())
        {
            constexpr_assert(!statuses.tokenEscapelessIdentifierNameStatus.empty());
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        constexpr_assert(statuses.tokenEscapelessIdentifierNameStatus.empty());
        constexpr_assert(statuses.identifierNameValue == nullptr);
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'\\')
        {
            statuses.tokenEscapelessIdentifierNameStatus = RuleStatus::makeFailure(
                currentPosition, u"unicode escape sequence in identifier not allowed here");
            currentPosition = nextPosition;
            retval = parseTokenUnicodeEscapeSequence(gc, codePoint);
            if(retval.fail())
            {
                return retval;
            }
            if(!character_properties::javascriptIdStart(codePoint))
            {
                retval = RuleStatus::makeFailure(
                    startPosition,
                    u"invalid identifier start character from unicode escape sequence");
                return retval;
            }
        }
        else if(!character_properties::javascriptIdStart(codePoint))
        {
            retval = RuleStatus::makeFailure(startPosition, u"invalid identifier start character");
            statuses.tokenEscapelessIdentifierNameStatus = static_cast<RuleStatus>(retval);
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
                if(statuses.tokenEscapelessIdentifierNameStatus.empty())
                    statuses.tokenEscapelessIdentifierNameStatus = RuleStatus::makeFailure(
                        currentPosition, u"unicode escape sequence in identifier not allowed here");
                currentPosition = nextPosition;
                retval = parseTokenUnicodeEscapeSequence(gc, codePoint);
                if(retval.fail())
                {
                    return retval;
                }
                if(!character_properties::javascriptIdContinue(codePoint))
                {
                    retval = RuleStatus::makeFailure(
                        currentPosition,
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
        if(statuses.tokenEscapelessIdentifierNameStatus.empty())
            statuses.tokenEscapelessIdentifierNameStatus = static_cast<RuleStatus>(retval);
        return retval;
    }
    RuleStatus parseTokenEscapelessIdentifierName(GC &gc)
    {
        std::size_t startPosition = currentPosition;
        parseTokenIdentifierName(gc);
        auto retval = getRuleStatuses(startPosition).tokenEscapelessIdentifierNameStatus;
        if(retval.fail())
            currentPosition = startPosition;
        return retval;
    }
    RuleStatus parseTokenKeyword(GC &gc, const char16_t *text, const char16_t *errorMessage)
    {
        HandleScope handleScope(gc);
        std::size_t startPosition = currentPosition;
        RuleStatus retval = parseTokenEscapelessIdentifierName(gc);
        if(retval.fail()
           || gc.readString(Handle<gc::StringReference>(
                  gc, getRuleStatuses(startPosition).identifierNameValue)) != text)
        {
            retval = RuleStatus::makeFailure(retval.startPosition, errorMessage);
            currentPosition = startPosition;
        }
        return retval;
    }
    RuleStatus parseTokenAwait(GC &gc)
    {
        return parseTokenKeyword(gc, u"await", u"missing await keyword");
    }
    RuleStatus parseTokenBreak(GC &gc)
    {
        return parseTokenKeyword(gc, u"break", u"missing break keyword");
    }
    RuleStatus parseTokenCase(GC &gc)
    {
        return parseTokenKeyword(gc, u"case", u"missing case keyword");
    }
    RuleStatus parseTokenCatch(GC &gc)
    {
        return parseTokenKeyword(gc, u"catch", u"missing catch keyword");
    }
    RuleStatus parseTokenClass(GC &gc)
    {
        return parseTokenKeyword(gc, u"class", u"missing class keyword");
    }
    RuleStatus parseTokenConst(GC &gc)
    {
        return parseTokenKeyword(gc, u"const", u"missing const keyword");
    }
    RuleStatus parseTokenContinue(GC &gc)
    {
        return parseTokenKeyword(gc, u"continue", u"missing continue keyword");
    }
    RuleStatus parseTokenDebugger(GC &gc)
    {
        return parseTokenKeyword(gc, u"debugger", u"missing debugger keyword");
    }
    RuleStatus parseTokenDefault(GC &gc)
    {
        return parseTokenKeyword(gc, u"default", u"missing default keyword");
    }
    RuleStatus parseTokenDelete(GC &gc)
    {
        return parseTokenKeyword(gc, u"delete", u"missing delete keyword");
    }
    RuleStatus parseTokenDo(GC &gc)
    {
        return parseTokenKeyword(gc, u"do", u"missing do keyword");
    }
    RuleStatus parseTokenElse(GC &gc)
    {
        return parseTokenKeyword(gc, u"else", u"missing else keyword");
    }
    RuleStatus parseTokenEnum(GC &gc)
    {
        return parseTokenKeyword(gc, u"enum", u"missing enum keyword");
    }
    RuleStatus parseTokenExport(GC &gc)
    {
        return parseTokenKeyword(gc, u"export", u"missing export keyword");
    }
    RuleStatus parseTokenExtends(GC &gc)
    {
        return parseTokenKeyword(gc, u"extends", u"missing extends keyword");
    }
    RuleStatus parseTokenFalse(GC &gc)
    {
        return parseTokenKeyword(gc, u"false", u"missing false keyword");
    }
    RuleStatus parseTokenFinally(GC &gc)
    {
        return parseTokenKeyword(gc, u"finally", u"missing finally keyword");
    }
    RuleStatus parseTokenFor(GC &gc)
    {
        return parseTokenKeyword(gc, u"for", u"missing for keyword");
    }
    RuleStatus parseTokenFunction(GC &gc)
    {
        return parseTokenKeyword(gc, u"function", u"missing function keyword");
    }
    RuleStatus parseTokenIf(GC &gc)
    {
        return parseTokenKeyword(gc, u"if", u"missing if keyword");
    }
    RuleStatus parseTokenImport(GC &gc)
    {
        return parseTokenKeyword(gc, u"import", u"missing import keyword");
    }
    RuleStatus parseTokenIn(GC &gc)
    {
        return parseTokenKeyword(gc, u"in", u"missing in keyword");
    }
    RuleStatus parseTokenInstanceOf(GC &gc)
    {
        return parseTokenKeyword(gc, u"instanceof", u"missing instanceof keyword");
    }
    RuleStatus parseTokenNew(GC &gc)
    {
        return parseTokenKeyword(gc, u"new", u"missing new keyword");
    }
    RuleStatus parseTokenNull(GC &gc)
    {
        return parseTokenKeyword(gc, u"null", u"missing null keyword");
    }
    RuleStatus parseTokenReturn(GC &gc)
    {
        return parseTokenKeyword(gc, u"return", u"missing return keyword");
    }
    RuleStatus parseTokenSuper(GC &gc)
    {
        return parseTokenKeyword(gc, u"super", u"missing super keyword");
    }
    RuleStatus parseTokenSwitch(GC &gc)
    {
        return parseTokenKeyword(gc, u"switch", u"missing switch keyword");
    }
    RuleStatus parseTokenThis(GC &gc)
    {
        return parseTokenKeyword(gc, u"this", u"missing this keyword");
    }
    RuleStatus parseTokenThrow(GC &gc)
    {
        return parseTokenKeyword(gc, u"throw", u"missing throw keyword");
    }
    RuleStatus parseTokenTrue(GC &gc)
    {
        return parseTokenKeyword(gc, u"true", u"missing true keyword");
    }
    RuleStatus parseTokenTry(GC &gc)
    {
        return parseTokenKeyword(gc, u"try", u"missing try keyword");
    }
    RuleStatus parseTokenTypeOf(GC &gc)
    {
        return parseTokenKeyword(gc, u"typeof", u"missing typeof keyword");
    }
    RuleStatus parseTokenVar(GC &gc)
    {
        return parseTokenKeyword(gc, u"var", u"missing var keyword");
    }
    RuleStatus parseTokenVoid(GC &gc)
    {
        return parseTokenKeyword(gc, u"void", u"missing void keyword");
    }
    RuleStatus parseTokenWhile(GC &gc)
    {
        return parseTokenKeyword(gc, u"while", u"missing while keyword");
    }
    RuleStatus parseTokenWith(GC &gc)
    {
        return parseTokenKeyword(gc, u"with", u"missing with keyword");
    }
    RuleStatus parseTokenYield(GC &gc)
    {
        return parseTokenKeyword(gc, u"yield", u"missing yield keyword");
    }
    RuleStatus parseTokenKeyword(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenKeywordStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        retval = parseTokenBreak(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenCase(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenCatch(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenClass(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenConst(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenContinue(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenDebugger(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenDefault(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenDelete(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenDo(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenElse(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenExport(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenExtends(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenFinally(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenFor(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenFunction(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenIf(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenImport(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenIn(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenInstanceOf(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenNew(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenReturn(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenSuper(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenSwitch(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenThis(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenThrow(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenTry(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenTypeOf(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenVar(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenVoid(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenWhile(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenWith(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenYield(gc);
        return retval;
    }
    RuleStatus parseTokenFutureReservedWord(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenFutureReservedWordStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        retval = parseTokenAwait(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenEnum(gc);
        return retval;
    }
    RuleStatus parseTokenBooleanLiteral(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenBooleanLiteralStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        retval = parseTokenFalse(gc);
        if(retval.success())
        {
            statuses.booleanLiteralValue = false;
            return retval;
        }
        retval /= parseTokenTrue(gc);
        if(retval.success())
        {
            statuses.booleanLiteralValue = true;
        }
        return retval;
    }
    RuleStatus parseTokenReservedWord(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenReservedWordStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        retval = parseTokenKeyword(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenFutureReservedWord(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenNull(gc);
        if(retval.success())
        {
            return retval;
        }
        retval /= parseTokenBooleanLiteral(gc);
        return retval;
    }
    RuleStatus parseTokenIdentifier(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenIdentifierStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        std::size_t startPosition = currentPosition;
        auto reservedWordResult = parseTokenReservedWord(gc);
        if(reservedWordResult.success())
        {
            currentPosition = startPosition;
            retval = RuleStatus::makeFailure(reservedWordResult.startPosition,
                                             u"expected a valid identifier");
            return retval;
        }
        retval = parseTokenIdentifierName(gc);
        return retval;
    }
    RuleStatus parseTokenNumericLiteral(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenNumericLiteralStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        String stringValue;
        if(codePoint == U'0')
        {
            stringValue = appendCodePoint(std::move(stringValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
            if(character_properties::javascriptDecimalDigit(codePoint))
            {
                retval = RuleStatus::makeFailure(
                    startPosition,
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
                    retval = RuleStatus::makeFailure(
                        currentPosition, u"binary numeric literal missing binary digit");
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
                    retval = RuleStatus::makeFailure(currentPosition,
                                                     u"hex numeric literal missing hex digit");
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
                stringValue = appendCodePoint(std::move(stringValue), codePoint);
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
            }
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
                retval = RuleStatus::makeFailure(currentPosition,
                                                 u"numeric literal missing digit in exponent");
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
        if(stringValue == u".")
        {
            retval = RuleStatus::makeFailure(startPosition, u"missing numeric literal");
            currentPosition = startPosition;
            return retval;
        }
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        statuses.numericLiteralValue = value::StringHandle::toNumberValue(stringValue);
        return retval;
    }
    RuleStatus parseTokenStringLiteral(GC &gc)
    {
        HandleScope handleScope(gc);
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenStringLiteralStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        String stringValue;
        if(codePoint != U'\"' && codePoint != U'\'')
        {
            retval = RuleStatus::makeFailure(startPosition, u"missing string literal");
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
                retval =
                    RuleStatus::makeFailure(startPosition, u"string literal missing closing quote");
                currentPosition = startPosition;
                return retval;
            }
            else if(codePoint == U'\\')
            {
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
                if(character_properties::javascriptLineTerminator(codePoint))
                {
                    parseTokenLineTerminatorSequence(gc); // never fails
                    codePoint = getCodePoint(currentPosition, nextPosition);
                    continue;
                }
                auto escapeSequenceResult = parseTokenEscapeSequence(gc, codePoint);
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
        statuses.stringLiteralValue = gc.internString(std::move(stringValue));
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    RuleStatus parseTokenRegularExpressionLiteral(GC &gc)
    {
        HandleScope handleScope(gc);
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenRegularExpressionLiteralStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        String bodyValue;
        if(codePoint != U'/')
        {
            retval = RuleStatus::makeFailure(startPosition, u"missing regular expression literal");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'/' || codePoint == U'*')
        {
            retval = RuleStatus::makeFailure(startPosition, u"missing regular expression literal");
            currentPosition = startPosition;
            return retval;
        }
        while(codePoint != U'/')
        {
            if(codePoint == eofCodePoint
               || character_properties::javascriptLineTerminator(codePoint))
            {
                retval = RuleStatus::makeFailure(currentPosition,
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
                        currentPosition, u"incomplete escape sequence in regular expression literal");
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
                        retval = RuleStatus::makeFailure(currentPosition,
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
                                currentPosition, u"incomplete escape sequence in regular expression literal");
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
        statuses.regularExpressionLiteralValue = gc.internString(std::move(bodyValue));
        String flagsValue;
        while(character_properties::javascriptIdContinue(codePoint))
        {
            flagsValue = appendCodePoint(std::move(flagsValue), codePoint);
            currentPosition = nextPosition;
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
        if(codePoint == U'\\')
        {
            retval = RuleStatus::makeFailure(currentPosition,
                                             u"unicode escape not permitted in regular expression flags");
            currentPosition = startPosition;
            return retval;
        }
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    RuleStatus parseTokenTemplateCharacters(GC &gc)
    {
        HandleScope handleScope(gc);
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenTemplateCharactersStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
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
                retval =
                    RuleStatus::makeFailure(currentPosition, u"template literal missing closing `");
                currentPosition = startPosition;
                return retval;
            }
            else if(codePoint == U'\\')
            {
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
                if(character_properties::javascriptLineTerminator(codePoint))
                {
                    parseTokenLineTerminatorSequence(gc); // never fails
                    codePoint = getCodePoint(currentPosition, nextPosition);
                }
                else
                {
                    retval = parseTokenEscapeSequence(gc, codePoint);
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
    RuleStatus parseTokenNoSubstitutionTemplate(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenNoSubstitutionTemplateStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'`')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, u"missing template literal");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        auto &templateCharactersStatuses = getRuleStatuses(currentPosition);
        retval = parseTokenTemplateCharacters(gc);
        if(retval.fail())
        {
            currentPosition = startPosition;
            return retval;
        }
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'`')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        statuses.templateValue = templateCharactersStatuses.templateValue;
        statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    RuleStatus parseTokenTemplateHead(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenTemplateHeadStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'`')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, u"missing template literal");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        auto &templateCharactersStatuses = getRuleStatuses(currentPosition);
        retval = parseTokenTemplateCharacters(gc);
        if(retval.fail())
        {
            currentPosition = startPosition;
            return retval;
        }
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'$')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'{')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        statuses.templateValue = templateCharactersStatuses.templateValue;
        statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    RuleStatus parseTokenTemplateTail(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenTemplateTailStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'}')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, u"missing template substitution tail");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        auto &templateCharactersStatuses = getRuleStatuses(currentPosition);
        retval = parseTokenTemplateCharacters(gc);
        if(retval.fail())
        {
            currentPosition = startPosition;
            return retval;
        }
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'`')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        statuses.templateValue = templateCharactersStatuses.templateValue;
        statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
    }
    RuleStatus parseTokenTemplateMiddle(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &retval = statuses.tokenTemplateMiddleStatus;
        if(!retval.empty())
        {
            if(retval.success())
                currentPosition = retval.endPosition;
            return retval;
        }
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'}')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, u"missing template substitution tail");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        auto &templateCharactersStatuses = getRuleStatuses(currentPosition);
        retval = parseTokenTemplateCharacters(gc);
        if(retval.fail())
        {
            currentPosition = startPosition;
            return retval;
        }
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'$')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint != U'{')
        {
            retval =
                RuleStatus::makeFailure(currentPosition, u"missing closing `");
            currentPosition = startPosition;
            return retval;
        }
        currentPosition = nextPosition;
        statuses.templateValue = templateCharactersStatuses.templateValue;
        statuses.templateRawValue = templateCharactersStatuses.templateRawValue;
        retval = RuleStatus::makeSuccess(startPosition, currentPosition);
        return retval;
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
}
}
