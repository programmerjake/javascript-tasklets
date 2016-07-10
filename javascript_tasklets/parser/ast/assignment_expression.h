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

#ifndef JAVASCRIPT_TASKLETS_PARSER_AST_ASSIGNMENT_EXPRESSION_H_
#define JAVASCRIPT_TASKLETS_PARSER_AST_ASSIGNMENT_EXPRESSION_H_

#include "expression.h"

namespace javascript_tasklets
{
namespace parser
{
namespace ast
{
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
}
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_AST_ASSIGNMENT_EXPRESSION_H_ */
