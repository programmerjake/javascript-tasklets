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
#include "argument.h"
#include "../../string.h"
#include "literal.h"

namespace javascript_tasklets
{
namespace parser
{
namespace ast
{
struct ExpressionComma : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionComma(std::size_t location,
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

struct ExpressionLShift : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionLShift(std::size_t location,
                     Expression *firstExpression,
                     Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionARShift : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionARShift(std::size_t location,
                      Expression *firstExpression,
                      Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionURShift : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionURShift(std::size_t location,
                      Expression *firstExpression,
                      Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionPlus : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionPlus(std::size_t location, Expression *firstExpression, Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionMinus : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionMinus(std::size_t location, Expression *firstExpression, Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionMultiply : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionMultiply(std::size_t location,
                       Expression *firstExpression,
                       Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionDivide : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionDivide(std::size_t location,
                     Expression *firstExpression,
                     Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionModulus : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionModulus(std::size_t location,
                      Expression *firstExpression,
                      Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
};

struct ExpressionDelete : public Expression
{
    Expression *expression;
    ExpressionDelete(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionVoid : public Expression
{
    Expression *expression;
    ExpressionVoid(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionTypeOf : public Expression
{
    Expression *expression;
    ExpressionTypeOf(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionPrefixIncrement : public Expression
{
    Expression *expression;
    ExpressionPrefixIncrement(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionPrefixDecrement : public Expression
{
    Expression *expression;
    ExpressionPrefixDecrement(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionPostfixIncrement : public Expression
{
    Expression *expression;
    ExpressionPostfixIncrement(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionPostfixDecrement : public Expression
{
    Expression *expression;
    ExpressionPostfixDecrement(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionUnaryPlus : public Expression
{
    Expression *expression;
    ExpressionUnaryPlus(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionUnaryMinus : public Expression
{
    Expression *expression;
    ExpressionUnaryMinus(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionBitwiseNot : public Expression
{
    Expression *expression;
    ExpressionBitwiseNot(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionLogicalNot : public Expression
{
    Expression *expression;
    ExpressionLogicalNot(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
};

struct ExpressionNew : public Expression
{
    Expression *constructor;
    ArgumentList argumentList;
    ExpressionNew(std::size_t location,
                  Expression *constructor,
                  ArgumentList argumentList = ArgumentList())
        : Expression(location), constructor(constructor), argumentList(std::move(argumentList))
    {
    }
};

struct ExpressionCall : public Expression
{
    Expression *function;
    ArgumentList argumentList;
    ExpressionCall(std::size_t location, Expression *function, ArgumentList argumentList)
        : Expression(location), function(function), argumentList(std::move(argumentList))
    {
    }
};

struct ExpressionTemplateCall : public Expression
{
    Expression *function;
    ExpressionTemplateLiteral *templateLiteral;
    ExpressionTemplateCall(std::size_t location,
                           Expression *function,
                           ExpressionTemplateLiteral *templateLiteral)
        : Expression(location), function(function), templateLiteral(templateLiteral)
    {
    }
};

struct ExpressionSuperCall : public Expression
{
    ArgumentList argumentList;
    ExpressionSuperCall(std::size_t location, ArgumentList argumentList)
        : Expression(location), argumentList(std::move(argumentList))
    {
    }
};

struct ExpressionMember : public Expression
{
    Expression *object;
    Expression *name;
    ExpressionMember(std::size_t location, Expression *object, Expression *name)
        : Expression(location), object(object), name(name)
    {
    }
};

struct ExpressionSuperMember : public Expression
{
    Expression *name;
    ExpressionSuperMember(std::size_t location, Expression *name) : Expression(location), name(name)
    {
    }
};

struct ExpressionThis : public Expression
{
    using Expression::Expression;
};

struct ExpressionNewTarget : public Expression
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
