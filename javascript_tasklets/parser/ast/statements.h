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
}
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_AST_STATEMENTS_H_ */
