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

#ifndef JAVASCRIPT_TASKLETS_PARSER_AST_STATEMENTS_H_
#define JAVASCRIPT_TASKLETS_PARSER_AST_STATEMENTS_H_

#include "statement.h"
#include "expression.h"
#include "binding.h"

namespace javascript_tasklets
{
namespace parser
{
namespace ast
{
struct StatementEmpty final : public Statement
{
    using Statement::Statement;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "StatementEmpty\n";
    }
};

struct StatementBlock final : public Statement
{
    StatementList statementList;
    StatementBlock(std::size_t location, StatementList statementList)
        : Statement(location), statementList(std::move(statementList))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "StatementBlock\n";
        for(auto statement : statementList)
        {
            statement->dump(os, indentDepth + 1);
        }
    }
};

struct StatementExpression final : public Statement
{
    Expression *expression;
    StatementExpression(std::size_t location, Expression *expression)
        : Statement(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "StatementExpression\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct StatementIf final : public Statement
{
    Expression *condition;
    Statement *thenPart;
    Statement *elsePart;
    StatementIf(std::size_t location,
                Expression *condition,
                Statement *thenPart,
                Statement *elsePart)
        : Statement(location), condition(condition), thenPart(thenPart), elsePart(elsePart)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "StatementIf\n";
        condition->dump(os, indentDepth + 1);
        thenPart->dump(os, indentDepth + 1);
        if(elsePart)
        {
            elsePart->dump(os, indentDepth + 1);
        }
        else
        {
            dumpWriteIndent(os, indentDepth + 1);
            os << "<no else-part>\n";
        }
    }
};

struct StatementVariable final : public Statement
{
    BindingList bindingList;
    StatementVariable(std::size_t location, BindingList bindingList)
        : Statement(location), bindingList(std::move(bindingList))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "StatementVariable\n";
        for(auto binding : bindingList)
            binding->dump(os, indentDepth + 1);
    }
};

struct StatementDoWhile final : public Statement
{
    Statement *body;
    Expression *condition;
    StatementDoWhile(std::size_t location, Statement *body, Expression *condition)
        : Statement(location), body(body), condition(condition)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "StatementDoWhile\n";
        body->dump(os, indentDepth + 1);
        condition->dump(os, indentDepth + 1);
    }
};

struct StatementWhile final : public Statement
{
    Expression *condition;
    Statement *body;
    StatementWhile(std::size_t location, Expression *condition, Statement *body)
        : Statement(location), condition(condition), body(body)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "StatementWhile\n";
        condition->dump(os, indentDepth + 1);
        body->dump(os, indentDepth + 1);
    }
};

struct StatementFor final : public Statement
{
    Expression *initialize;
    Expression *condition;
    Expression *increment;
    Statement *body;
    StatementFor(std::size_t location,
                 Expression *initialize,
                 Expression *condition,
                 Expression *increment,
                 Statement *body)
        : Statement(location),
          initialize(initialize),
          condition(condition),
          increment(increment),
          body(body)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "StatementFor\n";
        if(initialize)
        {
            initialize->dump(os, indentDepth + 1);
        }
        else
        {
            dumpWriteIndent(os, indentDepth + 1);
            os << "<no initialize-part>\n";
        }
        if(condition)
        {
            condition->dump(os, indentDepth + 1);
        }
        else
        {
            dumpWriteIndent(os, indentDepth + 1);
            os << "<no condition>\n";
        }
        if(increment)
        {
            increment->dump(os, indentDepth + 1);
        }
        else
        {
            dumpWriteIndent(os, indentDepth + 1);
            os << "<no increment-part>\n";
        }
        body->dump(os, indentDepth + 1);
    }
};

struct StatementForVar final : public Statement
{
    BindingList initialize;
    Expression *condition;
    Expression *increment;
    Statement *body;
    StatementForVar(std::size_t location,
                    BindingList initialize,
                    Expression *condition,
                    Expression *increment,
                    Statement *body)
        : Statement(location),
          initialize(std::move(initialize)),
          condition(condition),
          increment(increment),
          body(body)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "StatementForVar\n";
        dumpWriteIndent(os, indentDepth + 1);
        os << "StatementForVarInitializer\n";
        for(auto binding : initialize)
        {
            binding->dump(os, indentDepth + 2);
        }
        if(condition)
        {
            condition->dump(os, indentDepth + 1);
        }
        else
        {
            dumpWriteIndent(os, indentDepth + 1);
            os << "<no condition>\n";
        }
        if(increment)
        {
            increment->dump(os, indentDepth + 1);
        }
        else
        {
            dumpWriteIndent(os, indentDepth + 1);
            os << "<no increment-part>\n";
        }
        body->dump(os, indentDepth + 1);
    }
};
}
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_AST_STATEMENTS_H_ */
