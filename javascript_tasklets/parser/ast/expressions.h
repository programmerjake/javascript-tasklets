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
#include "../../value.h"
#include "literal.h"

namespace javascript_tasklets
{
namespace parser
{
namespace ast
{
struct ExpressionComma final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionComma(std::size_t location, Expression *firstExpression, Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionComma\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
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
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionConditional\n";
        condition->dump(os, indentDepth + 1);
        trueExression->dump(os, indentDepth + 1);
        falseExression->dump(os, indentDepth + 1);
    }
};

struct ExpressionLogicalOr final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionLogicalOr(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionLogicalOr\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionLogicalAnd final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionLogicalAnd(std::size_t location,
                         Expression *firstExpression,
                         Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionLogicalAnd\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionBitwiseOr final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionBitwiseOr(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionBitwiseOr\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionBitwiseXor final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionBitwiseXor(std::size_t location,
                         Expression *firstExpression,
                         Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionBitwiseXor\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionBitwiseAnd final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionBitwiseAnd(std::size_t location,
                         Expression *firstExpression,
                         Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionBitwiseAnd\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionCompareEq final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareEq(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionCompareEq\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionCompareNE final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareNE(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionCompareNE\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionCompareStrictEq final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareStrictEq(std::size_t location,
                              Expression *firstExpression,
                              Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionCompareStrictEq\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionCompareStrictNE final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareStrictNE(std::size_t location,
                              Expression *firstExpression,
                              Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionCompareStrictNE\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionCompareLT final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareLT(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionCompareLT\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionCompareLE final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareLE(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionCompareLE\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionCompareGT final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareGT(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionCompareGT\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionCompareGE final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionCompareGE(std::size_t location,
                        Expression *firstExpression,
                        Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionCompareGE\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionIn final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionIn(std::size_t location, Expression *firstExpression, Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionIn\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionInstanceOf final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionInstanceOf(std::size_t location,
                         Expression *firstExpression,
                         Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionInstanceOf\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionLShift final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionLShift(std::size_t location,
                     Expression *firstExpression,
                     Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionLShift\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionARShift final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionARShift(std::size_t location,
                      Expression *firstExpression,
                      Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionARShift\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionURShift final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionURShift(std::size_t location,
                      Expression *firstExpression,
                      Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionURShift\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionPlus final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionPlus(std::size_t location, Expression *firstExpression, Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionPlus\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionMinus final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionMinus(std::size_t location, Expression *firstExpression, Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionMinus\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionMultiply final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionMultiply(std::size_t location,
                       Expression *firstExpression,
                       Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionMultiply\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionDivide final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionDivide(std::size_t location,
                     Expression *firstExpression,
                     Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionDivide\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionModulus final : public Expression
{
    Expression *firstExpression;
    Expression *secondExpression;
    ExpressionModulus(std::size_t location,
                      Expression *firstExpression,
                      Expression *secondExpression)
        : Expression(location), firstExpression(firstExpression), secondExpression(secondExpression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionModulus\n";
        firstExpression->dump(os, indentDepth + 1);
        secondExpression->dump(os, indentDepth + 1);
    }
};

struct ExpressionDelete final : public Expression
{
    Expression *expression;
    ExpressionDelete(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionDelete\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionVoid final : public Expression
{
    Expression *expression;
    ExpressionVoid(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionVoid\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionTypeOf final : public Expression
{
    Expression *expression;
    ExpressionTypeOf(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionTypeOf\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionPrefixIncrement final : public Expression
{
    Expression *expression;
    ExpressionPrefixIncrement(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionPrefixIncrement\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionPrefixDecrement final : public Expression
{
    Expression *expression;
    ExpressionPrefixDecrement(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionPrefixDecrement\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionPostfixIncrement final : public Expression
{
    Expression *expression;
    ExpressionPostfixIncrement(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionPostfixIncrement\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionPostfixDecrement final : public Expression
{
    Expression *expression;
    ExpressionPostfixDecrement(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionPostfixDecrement\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionUnaryPlus final : public Expression
{
    Expression *expression;
    ExpressionUnaryPlus(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionUnaryPlus\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionUnaryMinus final : public Expression
{
    Expression *expression;
    ExpressionUnaryMinus(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionUnaryMinus\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionBitwiseNot final : public Expression
{
    Expression *expression;
    ExpressionBitwiseNot(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionBitwiseNot\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionLogicalNot final : public Expression
{
    Expression *expression;
    ExpressionLogicalNot(std::size_t location, Expression *expression)
        : Expression(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionLogicalNot\n";
        expression->dump(os, indentDepth + 1);
    }
};

struct ExpressionNew final : public Expression
{
    Expression *constructor;
    ArgumentList argumentList;
    ExpressionNew(std::size_t location,
                  Expression *constructor,
                  ArgumentList argumentList = ArgumentList())
        : Expression(location), constructor(constructor), argumentList(std::move(argumentList))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionNew\n";
        constructor->dump(os, indentDepth + 1);
        for(auto argument : argumentList)
        {
            argument->dump(os, indentDepth + 1);
        }
    }
};

struct ExpressionCall final : public Expression
{
    Expression *function;
    ArgumentList argumentList;
    ExpressionCall(std::size_t location, Expression *function, ArgumentList argumentList)
        : Expression(location), function(function), argumentList(std::move(argumentList))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionCall\n";
        function->dump(os, indentDepth + 1);
        for(auto argument : argumentList)
        {
            argument->dump(os, indentDepth + 1);
        }
    }
};

struct ExpressionTemplateCall final : public Expression
{
    Expression *function;
    ExpressionTemplateLiteral *templateLiteral;
    ExpressionTemplateCall(std::size_t location,
                           Expression *function,
                           ExpressionTemplateLiteral *templateLiteral)
        : Expression(location), function(function), templateLiteral(templateLiteral)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionTemplateCall\n";
        function->dump(os, indentDepth + 1);
        templateLiteral->dump(os, indentDepth + 1);
    }
};

struct ExpressionSuperCall final : public Expression
{
    ArgumentList argumentList;
    ExpressionSuperCall(std::size_t location, ArgumentList argumentList)
        : Expression(location), argumentList(std::move(argumentList))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionSuperCall\n";
        for(auto argument : argumentList)
        {
            argument->dump(os, indentDepth + 1);
        }
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
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionMember\n";
        object->dump(os, indentDepth + 1);
        name->dump(os, indentDepth + 1);
    }
};

struct ExpressionSuperMember final : public Expression
{
    Expression *name;
    ExpressionSuperMember(std::size_t location, Expression *name) : Expression(location), name(name)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionSuperMember\n";
        name->dump(os, indentDepth + 1);
    }
};

struct ExpressionThis final : public Expression
{
    using Expression::Expression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionThis\n";
    }
};

struct ExpressionNewTarget final : public Expression
{
    using Expression::Expression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionNewTarget\n";
    }
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
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "AssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct MultiplyAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "MultiplyAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct DivideAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "DivideAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct ModulusAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ModulusAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct AddAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "AddAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct SubtractAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "SubtractAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct LShiftAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "LShiftAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct ARShiftAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ARShiftAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct URShiftAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "URShiftAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct AndAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "AndAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct XorAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "XorAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct OrAssignmentExpression final : public GenericAssignmentExpression
{
    using GenericAssignmentExpression::GenericAssignmentExpression;
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "OrAssignmentExpression\n";
        destination->dump(os, indentDepth + 1);
        source->dump(os, indentDepth + 1);
    }
};

struct ExpressionIdentifierReference final : public Expression
{
    String name;
    ExpressionIdentifierReference(std::size_t location, String name)
        : Expression(location), name(std::move(name))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ExpressionIdentifierReference name=";
        writeString(os, name);
        os << "\n";
    }
};

struct IdentifierPropertyName final : public PropertyName
{
    String name;
    IdentifierPropertyName(std::size_t location, String name)
        : PropertyName(location), name(std::move(name))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "IdentifierPropertyName name=";
        writeString(os, name);
        os << "\n";
    }
};

struct StringPropertyName final : public PropertyName
{
    String value;
    StringPropertyName(std::size_t location, String value)
        : PropertyName(location), value(std::move(value))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "StringPropertyName value=\'";
        writeString(os, value);
        os << "\'\n";
    }
};

struct NumericPropertyName final : public PropertyName
{
    double value;
    NumericPropertyName(std::size_t location, double value) : PropertyName(location), value(value)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "NumericPropertyName value=";
        writeString(os, value::DoubleHandle::toStringValue(value));
        os << "\n";
    }
};

struct ComputedPropertyName final : public PropertyName
{
    Expression *expression;
    ComputedPropertyName(std::size_t location, Expression *expression)
        : PropertyName(location), expression(expression)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ComputedPropertyName\n";
        expression->dump(os, indentDepth + 1);
    }
};
}
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_AST_EXPRESSIONS_H_ */
