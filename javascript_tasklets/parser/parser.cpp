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
        constexpr Value() noexcept : type(Type::Empty), baseOrValue(), referencedName()
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
    struct RuleStatus
    {
        static constexpr std::size_t npos = static_cast<std::size_t>(-1);
        std::size_t endPosition = npos;
        const char16_t *errorMessage;
        constexpr RuleStatus() noexcept : endPosition(npos), errorMessage(nullptr)
        {
        }
        constexpr RuleStatus(std::size_t endPosition, const char16_t *errorMessage) noexcept
            : endPosition(endPosition),
              errorMessage(errorMessage)
        {
        }
        static constexpr RuleStatus makeEmpty() noexcept
        {
            return RuleStatus();
        }
        static constexpr RuleStatus makeFailed(std::size_t endPosition,
                                               const char16_t *errorMessage) noexcept
        {
            return constexpr_assert(errorMessage), RuleStatus(endPosition, errorMessage);
        }
        static constexpr RuleStatus makeSuccess(std::size_t endPosition) noexcept
        {
            return constexpr_assert(endPosition != npos), RuleStatus(endPosition, nullptr);
        }
        constexpr bool empty() const noexcept
        {
            return errorMessage == nullptr && endPosition == npos;
        }
        constexpr bool fail() const noexcept
        {
            return errorMessage != nullptr;
        }
        constexpr bool success() const noexcept
        {
            return !empty() && !fail();
        }
    };
    class RuleResult final
    {
        std::size_t startPosition;
        std::size_t endPosition;
        const char16_t *errorMessage;
        constexpr bool success() const noexcept
        {
            return errorMessage == nullptr;
        }
        constexpr bool fail() const noexcept
        {
            return !success();
        }
        constexpr RuleResult(std::size_t startPosition, RuleStatus ruleStatus) noexcept
            : startPosition(startPosition),
              endPosition(ruleStatus.endPosition),
              errorMessage(ruleStatus.errorMessage)
        {
        }
        constexpr RuleResult(std::size_t startPosition,
                             std::size_t endPosition,
                             const char16_t *errorMessage) noexcept : startPosition(startPosition),
                                                                      endPosition(endPosition),
                                                                      errorMessage(errorMessage)
        {
        }
        constexpr RuleResult() noexcept : startPosition(1), endPosition(0), errorMessage(nullptr)
        {
        }
        static constexpr RuleResult makeSuccess(std::size_t startPosition,
                                                std::size_t endPosition) noexcept
        {
            return constexpr_assert(startPosition <= endPosition),
                   RuleResult(startPosition, endPosition, nullptr);
        }
        static constexpr RuleResult makeFailure(std::size_t position,
                                                const char16_t *errorMessage) noexcept
        {
            return constexpr_assert(errorMessage), RuleResult(position, position, errorMessage);
        }
        static constexpr RuleResult makeFailure(std::size_t startPosition,
                                                std::size_t endPosition,
                                                const char16_t *errorMessage) noexcept
        {
            return constexpr_assert(startPosition <= endPosition), constexpr_assert(errorMessage),
                   RuleResult(startPosition, endPosition, errorMessage);
        }
        constexpr explicit operator RuleStatus() noexcept
        {
            return success() ? RuleStatus::makeSuccess(endPosition) :
                               RuleStatus::makeFailed(endPosition, errorMessage);
        }
        constexpr RuleResult merge(RuleResult rt) noexcept
        {
            return constexpr_assert(fail() && rt.fail()),
                   startPosition < rt.startPosition ? rt : *this;
        }
    };
    struct RuleStatuses final
    {
        gc::StringReference identifierNameValue;
        gc::StringReference numericLiteralValue;
        gc::StringReference stringLiteralValue;
        gc::StringReference regExpLiteralValue;
        gc::StringReference regExpLiteralFlagsValue;
        gc::StringReference templateRawValue;
        gc::StringReference templateValue;
        RuleStatus identifierNameStatus;
        RuleStatus identifierStatus;
        RuleStatus escapelessIdentifierNameStatus;
        RuleStatus stringLiteralStatus;
        RuleStatus tokenSeperatorStatus;
        RuleStatus noNewlineTokenSeperatorStatus;
        void getGCReferences(gc::GCReferencesCallback &callback) const
        {
            callback(stringLiteralValue);
            callback(identifierNameValue);
            callback(numericLiteralValue);
            callback(regExpLiteralValue);
            callback(regExpLiteralFlagsValue);
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
    explicit Parser(ASTTranslator &astTranslator, gc::SourceReference source)
        : astTranslator(astTranslator),
          ruleStatusesArray(source->contents.size() + 1), // one extra for EndOfFile
          source(source),
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
    RuleResult parseTokenSeperator(GC &gc)
    {
        RuleStatuses &statuses = getRuleStatuses(currentPosition);
        RuleStatus &status = statuses.tokenSeperatorStatus;
        if(!status.empty())
        {
            constexpr_assert(!statuses.noNewlineTokenSeperatorStatus.empty());
            return RuleResult(currentPosition, status);
        }
        constexpr_assert(statuses.noNewlineTokenSeperatorStatus.empty());
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
                            auto retval = RuleResult::makeFailure(
                                startPosition, currentPosition, u"comment missing closing */");
                            status = retval;
                            statuses.noNewlineTokenSeperatorStatus = retval;
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
        auto retval = RuleResult::makeSuccess(startPosition, currentPosition);
        status = retval;
        if(gotNewLine)
            statuses.noNewlineTokenSeperatorStatus = retval;
        else
            statuses.noNewlineTokenSeperatorStatus =
                RuleStatus::makeFailed(currentPosition, u"line split not allowed here");
        return retval;
    }
    RuleResult parseNoNewlineTokenSeperator(GC &gc)
    {
        std::size_t startPosition = currentPosition;
        parseTokenSeperator(gc);
        auto retval =
            RuleResult(startPosition, getRuleStatuses(startPosition).noNewlineTokenSeperatorStatus);
        if(retval.fail())
            currentPosition = startPosition;
        return retval;
    }
    RuleResult parseUnicodeEscapeSequence(GC &gc, std::uint32_t *valuePtr = nullptr)
    {
        std::size_t startPosition = currentPosition;
        if(getCodePoint(currentPosition, currentPosition) != U'u')
        {
            auto errorPosition = startPosition;
            currentPosition = startPosition;
            return RuleResult::makeFailure(errorPosition, u"unicode escape sequence is missing u");
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
                return RuleResult::makeFailure(errorPosition,
                                               u"unicode escape sequence missing hex digit");
            }
            std::uint32_t value = 0;
            while(character_properties::javascriptHexDigit(codePoint))
            {
                value *= 0x10;
                value += character_properties::javascriptDigitValue(codePoint);
                if(value > 0x10FFFFUL)
                {
                    auto errorPosition = currentPosition;
                    currentPosition = startPosition;
                    return RuleResult::makeFailure(errorPosition,
                                                   u"unicode escape sequence value out of range");
                }
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
            }
            if(codePoint != U'}')
            {
                auto errorPosition = currentPosition;
                currentPosition = startPosition;
                return RuleResult::makeFailure(errorPosition,
                                               u"unicode escape sequence missing closing }");
            }
            currentPosition = nextPosition;
            if(valuePtr)
                *valuePtr = value;
            return RuleResult::makeSuccess(startPosition, currentPosition);
        }
        else if(character_properties::javascriptHexDigit(codePoint))
        {
            std::uint32_t value = 0;
            for(std::size_t i = 0; i < 4; i++)
            {
                if(!character_properties::javascriptHexDigit(codePoint))
                {
                    auto errorPosition = currentPosition;
                    currentPosition = startPosition;
                    return RuleResult::makeFailure(errorPosition,
                                                   u"unicode escape sequence missing hex digit");
                }
                value *= 0x10;
                value += character_properties::javascriptDigitValue(codePoint);
                currentPosition = nextPosition;
                codePoint = getCodePoint(currentPosition, nextPosition);
            }
            if(valuePtr)
                *valuePtr = value;
            return RuleResult::makeSuccess(startPosition, currentPosition);
        }
        else
        {
            auto errorPosition = currentPosition;
            currentPosition = startPosition;
            return RuleResult::makeFailure(errorPosition,
                                           u"unicode escape sequence is missing { or a hex digit");
        }
    }
    RuleResult parseIdentifierName(GC &gc)
    {
        HandleScope handleScope(gc);
        auto &statuses = getRuleStatuses(currentPosition);
        RuleStatus &status = statuses.identifierNameStatus;
        if(!status.empty())
        {
            constexpr_assert(!statuses.escapelessIdentifierNameStatus.empty());
            return RuleResult(currentPosition, status);
        }
        constexpr_assert(statuses.escapelessIdentifierNameStatus.empty());
        constexpr_assert(statuses.identifierNameValue == nullptr);
        std::size_t startPosition = currentPosition;
        std::size_t nextPosition;
        auto codePoint = getCodePoint(currentPosition, nextPosition);
        if(codePoint == U'\\')
        {
            statuses.escapelessIdentifierNameStatus = RuleStatus(
                currentPosition, u"unicode escape sequence in identifier not allowed here");
            currentPosition = nextPosition;
            auto retval = parseUnicodeEscapeSequence(gc, &codePoint);
            if(retval.fail())
            {
                status = retval;
                return retval;
            }
            if(!character_properties::javascriptIdStart(codePoint))
            {
                auto retval = RuleResult::makeFailure(
                    startPosition,
                    currentPosition,
                    u"invalid identifier start character from unicode escape sequence");
                status = retval;
                return retval;
            }
        }
        else if(!character_properties::javascriptIdStart(codePoint))
        {
            auto retval =
                RuleResult::makeFailure(startPosition, u"invalid identifier start character");
            status = retval;
            statuses.escapelessIdentifierNameStatus;
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
                if(statuses.escapelessIdentifierNameStatus.empty())
                    statuses.escapelessIdentifierNameStatus = RuleStatus(
                        currentPosition, u"unicode escape sequence in identifier not allowed here");
                currentPosition = nextPosition;
                auto retval = parseUnicodeEscapeSequence(gc, &codePoint);
                if(retval.fail())
                {
                    status = retval;
                    return retval;
                }
                if(!character_properties::javascriptIdContinue(codePoint))
                {
                    auto retval = RuleResult::makeFailure(
                        startPosition,
                        currentPosition,
                        u"invalid identifier character from unicode escape sequence");
                    status = retval;
                    return retval;
                }
            }
            currentPosition = nextPosition;
            identifierValue = appendCodePoint(std::move(identifierValue), codePoint);
            codePoint = getCodePoint(currentPosition, nextPosition);
        }
        statuses.identifierNameValue = gc.internString(std::move(identifierValue));
        auto retval = RuleResult::makeSuccess(startPosition, currentPosition);
        status = retval;
        if(statuses.escapelessIdentifierNameStatus.empty())
            statuses.escapelessIdentifierNameStatus = retval;
        return retval;
    }
    RuleResult parseEscapelessIdentifierName(GC &gc)
    {
        std::size_t startPosition = currentPosition;
        parseIdentifierName(gc);
        auto retval = RuleResult(startPosition,
                                 getRuleStatuses(startPosition).escapelessIdentifierNameStatus);
        if(retval.fail())
            currentPosition = startPosition;
        return retval;
    }
    RuleResult parseKeyword(GC &gc, const char16_t *text, const char16_t *errorMessage)
    {
        HandleScope handleScope(gc);
        std::size_t startPosition = currentPosition;
        RuleResult retval = parseEscapelessIdentifierName(gc);
        if(retval.fail()
           || gc.readString(Handle<gc::StringReference>(
                  gc, getRuleStatuses(startPosition).identifierNameValue)) != text)
        {
            retval =
                RuleResult::makeFailure(retval.startPosition, retval.endPosition, errorMessage);
            currentPosition = startPosition;
        }
        return retval;
    }
    RuleResult parseBreak(GC &gc)
    {
        return parseKeyword(gc, u"break", u"missing break keyword");
    }
    RuleResult parseScript(GC &gc)
    {
        constexpr_assert(false);
#warning finish
        return RuleResult();
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
