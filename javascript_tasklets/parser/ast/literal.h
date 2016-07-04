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

#ifndef JAVASCRIPT_TASKLETS_PARSER_AST_LITERAL_H_
#define JAVASCRIPT_TASKLETS_PARSER_AST_LITERAL_H_

#include "expression.h"
#include "../../string.h"

namespace javascript_tasklets
{
namespace parser
{
namespace ast
{
struct ExpressionNullLiteral : public Expression
{
    using Expression::Expression;
};

struct ExpressionBooleanLiteral : public Expression
{
    bool value;
    ExpressionBooleanLiteral(std::size_t location, bool value) : Expression(location), value(value)
    {
    }
};

struct ExpressionNumericLiteral : public Expression
{
    double value;
    ExpressionNumericLiteral(std::size_t location, double value)
        : Expression(location), value(value)
    {
    }
};

struct ExpressionStringLiteral : public Expression
{
    String value;
    ExpressionStringLiteral(std::size_t location, String value)
        : Expression(location), value(std::move(value))
    {
    }
};

struct ExpressionRegExpLiteral : public Expression
{
    String body;
    String flags;
    ExpressionRegExpLiteral(std::size_t location, String body, String flags)
        : Expression(location), body(std::move(body)), flags(std::move(flags))
    {
    }
};
}
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_AST_LITERAL_H_ */
