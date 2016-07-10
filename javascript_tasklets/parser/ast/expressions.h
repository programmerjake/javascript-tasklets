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

#ifndef JAVASCRIPT_TASKLETS_PARSER_AST_EXPRESSIONS_H_
#define JAVASCRIPT_TASKLETS_PARSER_AST_EXPRESSIONS_H_

#include "expression.h"
#include "../../string.h"

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

struct ExpressionConditional : public Expression
{
    Expression *condition;
    Expression *trueExression;
    Expression *falseExression;
    ExpressionConditional(std::size_t location,
                          Expression *condition,
                          Expression *trueExression,
                          Expression *falseExression)
        : Expression(location),
          condition(condition),
          trueExression(trueExression),
          falseExression(falseExression)
    {
    }
};

struct ExpressionLogicalOr : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionLogicalOr(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionLogicalAnd : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionLogicalAnd(std::size_t location,
                         Expression *firstExpression,
                         Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionBitwiseOr : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionBitwiseOr(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionBitwiseXor : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionBitwiseXor(std::size_t location,
                         Expression *firstExpression,
                         Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionBitwiseAnd : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionBitwiseAnd(std::size_t location,
                         Expression *firstExpression,
                         Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionCompareEq : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareEq(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionCompareNE : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareNE(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionCompareStrictEq : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareStrictEq(std::size_t location,
                              Expression *firstExpression,
                              Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionCompareStrictNE : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareStrictNE(std::size_t location,
                              Expression *firstExpression,
                              Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionCompareLT : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareLT(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionCompareLE : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareLE(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionCompareGT : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareGT(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionCompareGE : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareGE(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionIn : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionIn(std::size_t location, Expression *firstExpression, Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionInstanceOf : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionInstanceOf(std::size_t location,
                         Expression *firstExpression,
                         Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionThis : public Expression
{
    using Expression::Expression;
};

struct GenericAssignmentExpression : public Expression
{
    Expression *destination;
    Expression *source;
    GenericAssignmentExpression(std::size_t location, Expression *destination, Expression *source)
        : Expression(location), destination(destination), source(source)
    {
    }
};

struct AssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct MultiplyAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct DivideAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct ModulusAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct AddAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct SubtractAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct LShiftAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct ARShiftAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct URShiftAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct AndAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct XorAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct OrAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
};

struct ExpressionIdentifierReference : public Expression
{
    String name;
    ExpressionIdentifierReference(std::size_t location, String name)
        : Expression(location), name(std::move(name))
    {
    }
};
}
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_AST_EXPRESSIONS_H_ */
