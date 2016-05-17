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

namespace javascript_tasklets
{
namespace parser
{
using namespace vm::interpreter;
struct CodeEmitter : public gc::Object::ExtraData
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
};

struct Parser : public gc::Object::ExtraData
{
    CodeEmitter &codeEmitter;
    Token peek;
    std::vector<Token> putBackStack;
    explicit Parser(CodeEmitter &codeEmitter) : codeEmitter(codeEmitter), peek(), putBackStack()
    {
    }
    void throwSyntaxError(const String &message,
                          const Location &location,
                          bool isPrefixOfValidSource,
                          GC &gc)
    {
        value::ObjectHandle::throwSyntaxError(
            message, LocationHandle(gc, location), isPrefixOfValidSource, gc);
    }
    void throwSyntaxError(const String &message, const Location &location, GC &gc)
    {
        throwSyntaxError(message, location, false, gc);
    }
    void throwSyntaxError(String &&message,
                          const Location &location,
                          bool isPrefixOfValidSource,
                          GC &gc)
    {
        value::ObjectHandle::throwSyntaxError(
            std::move(message), LocationHandle(gc, location), isPrefixOfValidSource, gc);
    }
    void throwSyntaxError(String &&message, const Location &location, GC &gc)
    {
        throwSyntaxError(std::move(message), location, false, gc);
    }
    virtual std::unique_ptr<ExtraData> clone() const override final
    {
        constexpr_assert(!"Parser::clone called");
        return std::unique_ptr<ExtraData>();
    }
    virtual void getGCReferences(gc::GCReferencesCallback &callback) const override
    {
        callback(peek);
        callback(putBackStack);
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
    void parseStatementList(GC &gc)
    {
#warning finish
    }
    void parseScriptBody(GC &gc)
    {
        parseStatementList(gc);
#warning check if StatementList Contains super unless the source code containing super is eval code that is being processed by a direct eval that is contained in function code that is not the function code of an ArrowFunction
#warning check if StatementList Contains NewTarget unless the source code containing NewTarget is eval code that is being processed by a direct eval that is contained in function code that is not the function code of an ArrowFunction
#warning check if ContainsDuplicateLabels({}) of StatementList is true
#warning check if ContainsUndefinedBreakTarget({}) of StatementList is true
#warning check if ContainsUndefinedContinueTarget({}, {}) of StatementList is true
    }
    void parseScript(GC &gc)
    {
        if(peek.type != Token::Type::EndOfFile)
        {
            parseScriptBody(gc);
#warning check if the LexicallyDeclaredNames of ScriptBody contains any duplicate entries
#warning check if any element of the LexicallyDeclaredNames of ScriptBody also occurs in the VarDeclaredNames of ScriptBody
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
    auto parserUniquePtr = std::unique_ptr<Parser>(new SourceParser(codeEmitter, source, gc));
    auto &parser = *parserUniquePtr;
    auto parserObject =
        value::ObjectHandle::create(std::move(parserUniquePtr), value::NullHandle(), gc);
    parser.parseScript(gc);
    return codeEmitter.finishGlobalCode(parser.peek.location, gc);
}
}
}
