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
#include "tokenizer.h"
#include "../vm/interpreter.h"
#include "../constexpr_assert.h"

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

struct Parser : public gc::Object::ExtraData
{
    CodeEmitter &codeEmitter;
    enum class RuleStatus
    {
        Unknown,
        Succeded,
        Failed
    };
    struct TokenAndRuleStatuses final
    {
        Token token;
        explicit TokenAndRuleStatuses(Token token = Token()) : token(std::move(token))
        {
        }
        RuleStatus scriptStatus = RuleStatus::Unknown;
        RuleStatus scriptBodyStatus = RuleStatus::Unknown;
    };
    std::vector<TokenAndRuleStatuses> tokens;
    std::size_t currentPosition;
    String errorMessage;
    Location errorLocation;
    bool doSetError(const Location &location)
    {
        return errorLocation.source == nullptr
               || location.beginPosition > errorLocation.beginPosition
               || (location.beginPosition == errorLocation.beginPosition
                   && location.endPosition > errorLocation.endPosition);
    }
    template <typename Message>
    void setError(const Location &location, Message &&message)
    {
        if(doSetError(location))
        {
            errorLocation = location;
            errorMessage = std::forward<Message>(message);
        }
    }
    TokenAndRuleStatuses &getCurrent(GC &gc)
    {
        while(currentPosition >= tokens.size())
        {
            HandleScope handleScope(gc);
            if(!tokens.empty() && tokens.back().token.type == Token::Type::EndOfFile)
            {
                auto eofToken = tokens.back();
                tokens.push_back(std::move(eofToken));
            }
            else
            {
                tokens.emplace_back(readNextTokenFromInput(gc));
            }
        }
        return tokens[currentPosition];
    }
    TokenAndRuleStatuses &getNext(GC &gc)
    {
        currentPosition++;
        return getCurrent(gc);
    }
    template <typename Message>
    bool matchTokenType(Token::Type type,
                        bool errorOnMatchFail,
                        bool errorOnMatchSuccess,
                        bool extractOnMatchSuccess,
                        Message &&message,
                        GC &gc)
    {
        if(getCurrent(gc).token.type == type)
        {
            if(errorOnMatchSuccess)
            {
                setError(getCurrent(gc).token.location, std::forward<Message>(message));
            }
            if(extractOnMatchSuccess)
            {
                getNext(gc);
            }
            return true;
        }
        if(errorOnMatchFail)
        {
            setError(getCurrent(gc).token.location, std::forward<Message>(message));
        }
        return false;
    }
    template <typename Name, typename Message>
    bool matchIdentifierName(Name &&name,
                             bool errorOnMatchFail,
                             bool errorOnMatchSuccess,
                             bool extractOnMatchSuccess,
                             Message &&message,
                             GC &gc)
    {
        if(getCurrent(gc).token.type == Token::Type::Identifier)
        {
            HandleScope handleScope(gc);
            if(gc.readString(Handle<gc::StringReference>(gc, getCurrent(gc).token.processedValue))
               == std::forward<Name>(name))
            {
                if(errorOnMatchSuccess)
                {
                    setError(getCurrent(gc).token.location, std::forward<Message>(message));
                }
                if(extractOnMatchSuccess)
                {
                    getNext(gc);
                }
                return true;
            }
        }
        if(errorOnMatchFail)
        {
            setError(getCurrent(gc).token.location, std::forward<Message>(message));
        }
        return false;
    }
    explicit Parser(CodeEmitter &codeEmitter)
        : codeEmitter(codeEmitter), tokens(), currentPosition(0)
    {
    }
    virtual std::unique_ptr<ExtraData> clone() const override final
    {
        constexpr_assert(!"Parser::clone called");
        return std::unique_ptr<ExtraData>();
    }
    virtual void getGCReferences(gc::GCReferencesCallback &callback) const override
    {
        for(const auto &v : tokens)
        {
            callback(v.token);
        }
        callback(errorLocation);
    }
    virtual Handle<Token> readNextTokenFromInput(GC &gc) = 0;
    virtual Handle<Token> reparseAsTemplateContinuation(Handle<Token> token, GC &gc) = 0;
    virtual Handle<Token> reparseAsRegExp(Handle<Token> token, GC &gc) = 0;
    void next(GC &gc)
    {
        HandleScope handleScope(gc);
        if(putBackStack.empty())
            peek = readNextTokenFromInput(gc).get();
        else
        {
            peek = std::move(putBackStack.back());
            putBackStack.pop_back();
        }
    }
    void init(GC &gc)
    {
        HandleScope handleScope(gc);
        peek = readNextTokenFromInput(gc).get();
    }
    void putBack(Token token)
    {
        putBackStack.push_back(std::move(peek));
        peek = token;
    }
    bool isLet(GC &gc)
    {
        HandleScope handleScope(gc);
        if(peek.type == Token::Type::Identifier
           && gc.readString(Handle<gc::StringReference>(gc, peek.processedValue)) == u"let")
            return true;
        return false;
    }
    bool isLetLBracket(GC &gc)
    {
        if(isLet(gc))
        {
            HandleScope handleScope(gc);
            Token letToken = peek;
            next(gc);
            bool isLBracket = peek.type == Token::Type::LBracket;
            putBack(letToken);
            return isLBracket;
        }
        return false;
    }
    bool isLineSplit(GC &gc)
    {
        if(peek.precededByLineTerminator)
            return true;
        return false;
    }
    void getOrInsertSemicolon(GC &gc, bool isAfterWhileInDoWhile)
    {
        if(peek.type == Token::Type::Semicolon)
        {
            next(gc);
        }
        else if(!isAfterWhileInDoWhile && peek.type != Token::Type::RBrace
                && peek != Token::Type::EndOfFile
                && !peek.precededByLineTerminator)
        {
            throwSyntaxError(u"expected ;", peek.location, gc);
        }
    }
    void parseConditionalExpression(bool inFlag, bool yieldFlag, GC &gc)
    {
#warning finish
    }
    bool isStartConditionalExpressionToken(bool inFlag, bool yieldFlag, GC &gc)
    {
#warning finish
    }
    void parseYieldExpression(bool inFlag, GC &gc)
    {
#warning finish
    }
    bool isStartYieldExpressionToken(bool inFlag, GC &gc)
    {
#warning finish
    }
    void parseArrowFunction(bool inFlag, bool yieldFlag, GC &gc)
    {
#warning finish
    }
    bool isStartArrowFunctionToken(bool inFlag, bool yieldFlag, GC &gc)
    {
#warning finish
    }
    void parseLeftHandSideExpression(bool yieldFlag, GC &gc)
    {
#warning finish
    }
    bool isStartLeftHandSideExpressionToken(bool yieldFlag, GC &gc)
    {
#warning finish
    }
    void parseAssignmentExpression(bool inFlag, bool yieldFlag, GC &gc)
    {
#warning finish
    }
    bool isStartAssignmentExpressionToken(bool inFlag, bool yieldFlag, GC &gc)
    {
        if(isStartConditionalExpressionToken(inFlag, yieldFlag, gc))
            return true;
        if(yieldFlag && isStartYieldExpressionToken(inFlag, gc))
            return true;
        if(isStartArrowFunctionToken(inFlag, yieldFlag, gc))
            return true;
        if(isStartLeftHandSideExpressionToken(yieldFlag, gc))
            return true;
        return false;
    }
    void parseExpression(bool inFlag, bool yieldFlag, GC &gc)
    {
        parseAssignmentExpression(inFlag, yieldFlag, gc);
        while(peek.type == Token::Type::Comma)
        {
            codeEmitter.valueStackGetValue();
            codeEmitter.valueStackPop();
            next(gc);
            parseAssignmentExpression(inFlag, yieldFlag, gc);
            codeEmitter.valueStackGetValue();
        }
    }
    bool isStartExpressionToken(bool inFlag, bool yieldFlag, GC &gc)
    {
        return isStartAssignmentExpressionToken(inFlag, yieldFlag, gc);
    }
    void parseDeclaration(bool yieldFlag, GC &gc)
    {
#warning finish
    }
    bool isStartDeclarationToken(bool yieldFlag, GC &gc)
    {
        switch(peek.type)
        {
        case Token::Type::EndOfFile:
        case Token::Type::Identifier:
        case Token::Type::LBrace:
        case Token::Type::RBrace:
        case Token::Type::Ellipses:
        case Token::Type::LParen:
        case Token::Type::RParen:
        case Token::Type::LBracket:
        case Token::Type::RBracket:
        case Token::Type::Period:
        case Token::Type::Semicolon:
        case Token::Type::Comma:
        case Token::Type::LAngle:
        case Token::Type::RAngle:
        case Token::Type::LAngleEqual:
        case Token::Type::RAngleEqual:
        case Token::Type::DoubleEqual:
        case Token::Type::NotEqual:
        case Token::Type::TripleEqual:
        case Token::Type::NotDoubleEqual:
        case Token::Type::Plus:
        case Token::Type::Minus:
        case Token::Type::Star:
        case Token::Type::Percent:
        case Token::Type::Inc:
        case Token::Type::Dec:
        case Token::Type::LShift:
        case Token::Type::ARShift:
        case Token::Type::URShift:
        case Token::Type::Amp:
        case Token::Type::Pipe:
        case Token::Type::Caret:
        case Token::Type::EMark:
        case Token::Type::Tilde:
        case Token::Type::LAnd:
        case Token::Type::LOr:
        case Token::Type::QMark:
        case Token::Type::Colon:
        case Token::Type::SingleEqual:
        case Token::Type::PlusEqual:
        case Token::Type::MinusEqual:
        case Token::Type::StarEqual:
        case Token::Type::PercentEqual:
        case Token::Type::LShiftEqual:
        case Token::Type::ARShiftEqual:
        case Token::Type::URShiftEqual:
        case Token::Type::AmpEqual:
        case Token::Type::PipeEqual:
        case Token::Type::CaretEqual:
        case Token::Type::Arrow:
        case Token::Type::Div:
        case Token::Type::DivEqual:
        case Token::Type::NumericLiteral:
        case Token::Type::StringLiteral:
        case Token::Type::RegExpLiteral:
        case Token::Type::NoSubstitutionTemplate:
        case Token::Type::TemplateHead:
        case Token::Type::TemplateMiddle:
        case Token::Type::TemplateTail:
        case Token::Type::BreakKW:
        case Token::Type::CaseKW:
        case Token::Type::CatchKW:
        case Token::Type::ClassKW:
        case Token::Type::ConstKW:
        case Token::Type::ContinueKW:
        case Token::Type::DebuggerKW:
        case Token::Type::DefaultKW:
        case Token::Type::DeleteKW:
        case Token::Type::DoKW:
        case Token::Type::ElseKW:
        case Token::Type::ExportKW:
        case Token::Type::ExtendsKW:
        case Token::Type::FinallyKW:
        case Token::Type::ForKW:
        case Token::Type::FunctionKW:
        case Token::Type::IfKW:
        case Token::Type::ImportKW:
        case Token::Type::InKW:
        case Token::Type::InstanceOfKW:
        case Token::Type::NewKW:
        case Token::Type::ReturnKW:
        case Token::Type::SuperKW:
        case Token::Type::SwitchKW:
        case Token::Type::ThisKW:
        case Token::Type::ThrowKW:
        case Token::Type::TryKW:
        case Token::Type::TypeOfKW:
        case Token::Type::VarKW:
        case Token::Type::VoidKW:
        case Token::Type::WhileKW:
        case Token::Type::WithKW:
        case Token::Type::YieldKW:
        case Token::Type::EnumKW:
        case Token::Type::AwaitKW:
        case Token::Type::NullLiteral:
        case Token::Type::TrueLiteral:
        case Token::Type::FalseLiteral:
#warning finish
            return false;
        }
        constexpr_assert(false);
        return false;
    }
    void parseBlockStatement(bool yieldFlag, bool returnFlag, GC &gc)
    {
        if(peek.type != Token::Type::LBrace)
            throwSyntaxError(u"missing opening {", peek.location, gc);
        next(gc);
        if(isStartStatementListToken(yieldFlag, returnFlag, gc))
        {
            parseStatementList(yieldFlag, returnFlag, gc);
        }
        else
        {
            codeEmitter.valueStackPushEmpty();
        }
        if(peek.type != Token::Type::RBrace)
            throwSyntaxError(u"missing closing }", peek.location, gc);
        next(gc);
    }
    bool isStartBlockStatementToken(bool yieldFlag, bool returnFlag, GC &gc)
    {
        if(peek.type == Token::Type::LBrace)
            return true;
        return false;
    }
    void parseVariableStatement(bool yieldFlag, GC &gc)
    {
        if(peek.type != Token::Type::VarKW)
            throwSyntaxError(u"missing var", peek.location, gc);
        next(gc);
        constexpr_assert(false);
#warning finish
    }
    bool isStartVariableStatementToken(bool yieldFlag, GC &gc)
    {
        if(peek.type == Token::Type::VarKW)
            return true;
        return false;
    }
    void parseEmptyStatement(GC &gc)
    {
        if(peek.type != Token::Type::Semicolon)
            throwSyntaxError(u"missing ;", peek.location, gc);
        next(gc);
        codeEmitter.valueStackPushEmpty();
    }
    bool isStartEmptyStatementToken(GC &gc)
    {
        if(peek.type == Token::Type::Semicolon)
            return true;
        return false;
    }
    void parseExpressionStatement(bool yieldFlag, GC &gc)
    {
        parseExpression(true, yieldFlag, gc);
        codeEmitter.valueStackGetValue();
        getOrInsertSemicolon(gc, false);
    }
    bool isStartExpressionStatementToken(bool yieldFlag, GC &gc)
    {
        if(peek.type == Token::Type::LBrace || peek.type == Token::Type::FunctionKW
           || peek.type == Token::Type::ClassKW
           || isLetLBracket(gc))
            return false;
        return isStartExpressionToken(true, yieldFlag, gc);
    }
    void parseStatement(bool yieldFlag, bool returnFlag, GC &gc)
    {
        if(isStartBlockStatementToken(yieldFlag, returnFlag, gc))
        {
            parseBlockStatement(yieldFlag, returnFlag, gc);
        }
        else if(isStartVariableStatementToken(yieldFlag, gc))
        {
            parseVariableStatement(yieldFlag, gc);
        }
        else if(isStartEmptyStatementToken(gc))
        {
            parseEmptyStatement(gc);
        }
        else if(isStartExpressionStatementToken(yieldFlag, gc))
        {
            parseExpressionStatement(yieldFlag, gc);
        }
        else
        {
            constexpr_assert(false);
#warning finish
        }
    }
    bool isStartStatementToken(bool yieldFlag, bool returnFlag, GC &gc)
    {
        return isStartBlockStatementToken(yieldFlag, returnFlag, gc)
               || isStartVariableStatementToken(yieldFlag, gc) || isStartEmptyStatementToken(gc)
               || isStartExpressionStatementToken(yieldFlag, gc);
    }
    void parseStatementListItem(bool yieldFlag, bool returnFlag, GC &gc)
    {
        if(isStartDeclarationToken(yieldFlag, gc))
            parseDeclaration(yieldFlag, gc);
        else
            parseStatement(yieldFlag, returnFlag, gc);
    }
    bool isStartStatementListItemToken(bool yieldFlag, bool returnFlag, GC &gc)
    {
        return isStartDeclarationToken(yieldFlag, gc)
               || isStartStatementToken(yieldFlag, returnFlag, gc);
    }
    bool isStartStatementListToken(bool yieldFlag, bool returnFlag, GC &gc)
    {
        return isStartStatementListItemToken(yieldFlag, returnFlag, gc);
    }
    void parseStatementList(bool yieldFlag, bool returnFlag, GC &gc)
    {
        parseStatementListItem(yieldFlag, returnFlag, gc);
        while(isStartStatementListItemToken(yieldFlag, returnFlag, gc))
        {
            parseStatementListItem(yieldFlag, returnFlag, gc);
            codeEmitter.valueStackMerge();
        }
#warning finish
    }
    bool isStartScriptBodyToken(GC &gc)
    {
        return isStartStatementListToken(false, false, gc);
    }
    void parseScriptBody(GC &gc)
    {
        parseStatementList(false, false, gc);
#warning check if StatementList Contains super unless the source code containing super is eval code that is being processed by a direct eval that is contained in function code that is not the function code of an ArrowFunction
#warning check if StatementList Contains NewTarget unless the source code containing NewTarget is eval code that is being processed by a direct eval that is contained in function code that is not the function code of an ArrowFunction
#warning check if ContainsDuplicateLabels({}) of StatementList is true
#warning check if ContainsUndefinedBreakTarget({}) of StatementList is true
#warning check if ContainsUndefinedContinueTarget({}, {}) of StatementList is true
    }
    bool parseScript(GC &gc)
    {
        if(isStartScriptBodyToken(gc))
        {
            parseScriptBody(gc);
#warning check if the LexicallyDeclaredNames of ScriptBody contains any duplicate entries
#warning check if any element of the LexicallyDeclaredNames of ScriptBody also occurs in the VarDeclaredNames of ScriptBody
        }
        else
        {
            codeEmitter.valueStackPushEmpty();
        }
        if(peek.type != Token::Type::EndOfFile)
        {
            throwSyntaxError(u"unexpected token", peek.location, gc);
        }
    }
};

struct SourceParser final : public Parser
{
    Tokenizer tokenizer;
    SourceParser(CodeEmitter &codeEmitter, SourceHandle source, GC &gc)
        : Parser(codeEmitter), tokenizer(source)
    {
        init(gc);
    }
    virtual Handle<Token> readNextTokenFromInput(GC &gc) override
    {
        return tokenizer.next(gc);
    }
    virtual Handle<Token> reparseAsTemplateContinuation(Handle<Token> token, GC &gc) override
    {
        return tokenizer.reparseAsTemplateContinuation(token, gc);
    }
    virtual Handle<Token> reparseAsRegExp(Handle<Token> token, GC &gc) override
    {
        return tokenizer.reparseAsRegExp(token, gc);
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
    auto parserUniquePtr = std::unique_ptr<Parser>(new SourceParser(astTranslator, source, gc));
    auto &parser = *parserUniquePtr;
    auto parserObject =
        value::ObjectHandle::create(std::move(parserUniquePtr), value::NullHandle(), gc);
    static_cast<void>(parserObject);
    parser.parseScript(gc);
    return handleScope.escapeHandle(codeEmitter.finishGlobalCode(parser.peek.location, gc));
}
}
}
