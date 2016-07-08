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

#ifndef JAVASCRIPT_TASKLETS_PARSER_AST_COMMA_OPERATOR_H_
#define JAVASCRIPT_TASKLETS_PARSER_AST_COMMA_OPERATOR_H_

#include "expression.h"

namespace javascript_tasklets
{
namespace parser
{
namespace ast
{
struct ExpressionCommaOperator : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCommaOperator(std::size_t location,
                            Expression *firstExpression,
                            Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};
}
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_AST_COMMA_OPERATOR_H_ */
