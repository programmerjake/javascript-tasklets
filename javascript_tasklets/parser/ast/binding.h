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

#ifndef JAVASCRIPT_TASKLETS_PARSER_AST_BINDING_H_
#define JAVASCRIPT_TASKLETS_PARSER_AST_BINDING_H_

#include "node.h"
#include "../../string.h"
#include "expression.h"
#include <vector>

namespace javascript_tasklets
{
namespace parser
{
namespace ast
{
struct Binding : public Node
{
    using Node::Node;
};

typedef std::vector<Binding *> BindingList;

struct BindingIdentifier final : public Binding
{
    String name;
    BindingIdentifier(std::size_t location, String name) : Binding(location), name(std::move(name))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "BindingIdentifier name=";
        writeString(os, name);
        os << "\n";
    }
};

struct LexicalBinding final : public Binding
{
    Binding *binding;
    Expression *initializer;
    LexicalBinding(std::size_t location, Binding *binding, Expression *initializer)
        : Binding(location), binding(binding), initializer(initializer)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "LexicalBinding\n";
        binding->dump(os, indentDepth + 1);
        if(initializer)
            initializer->dump(os, indentDepth + 1);
    }
};

struct VariableDeclaration final : public Binding
{
    Binding *binding;
    Expression *initializer;
    VariableDeclaration(std::size_t location, Binding *binding, Expression *initializer)
        : Binding(location), binding(binding), initializer(initializer)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "VariableDeclaration\n";
        binding->dump(os, indentDepth + 1);
        if(initializer)
            initializer->dump(os, indentDepth + 1);
    }
};

struct BindingElement final : public Binding
{
    Binding *binding;
    Expression *initializer;
    BindingElement(std::size_t location, Binding *binding, Expression *initializer)
        : Binding(location), binding(binding), initializer(initializer)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "BindingElement\n";
        binding->dump(os, indentDepth + 1);
        if(initializer)
            initializer->dump(os, indentDepth + 1);
    }
};

struct BindingRestElement final : public Binding
{
    String name;
    BindingRestElement(std::size_t location, String name) : Binding(location), name(std::move(name))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "BindingRestElement name=";
        writeString(os, name);
        os << "\n";
    }
};

struct ObjectBindingPattern final : public Binding
{
    BindingList properties;
    ObjectBindingPattern(std::size_t location, BindingList properties)
        : Binding(location), properties(std::move(properties))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ObjectBindingPattern\n";
        for(auto property : properties)
            property->dump(os, indentDepth + 1);
    }
};

struct ArrayBindingPattern final : public Binding
{
    BindingList elements;
    ArrayBindingPattern(std::size_t location, BindingList elements)
        : Binding(location), elements(std::move(elements))
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "ArrayBindingPattern\n";
        for(auto element : elements)
        {
            if(element)
            {
                element->dump(os, indentDepth + 1);
            }
            else
            {
                dumpWriteIndent(os, indentDepth);
                os << "<empty>\n";
            }
        }
    }
};

struct BindingProperty final : public Binding
{
    PropertyName *propertyName;
    Binding *bindingElement;
    BindingProperty(std::size_t location, PropertyName *propertyName, Binding *bindingElement)
        : Binding(location), propertyName(propertyName), bindingElement(bindingElement)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "BindingProperty\n";
        propertyName->dump(os, indentDepth + 1);
        bindingElement->dump(os, indentDepth + 1);
    }
};

struct SingleNameBinding final : public Binding
{
    String name;
    Expression *initializer;
    SingleNameBinding(std::size_t location, String name, Expression *initializer)
        : Binding(location), name(std::move(name)), initializer(initializer)
    {
    }
    virtual void dump(std::ostream &os, std::size_t indentDepth) const override
    {
        dumpWriteIndent(os, indentDepth);
        os << "SingleNameBinding name=";
        writeString(os, name);
        os << "\n";
        if(initializer)
            initializer->dump(os, indentDepth + 1);
    }
};
}
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_AST_BINDING_H_ */
