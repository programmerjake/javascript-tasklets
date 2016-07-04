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

#ifndef JAVASCRIPT_TASKLETS_PARSER_AST_ARENA_H_
#define JAVASCRIPT_TASKLETS_PARSER_AST_ARENA_H_

#include "node.h"
#include <utility>
#include <type_traits>

namespace javascript_tasklets
{
namespace parser
{
namespace ast
{
class Arena final
{
    Arena(const Arena &) = delete;

private:
    Node *head;
    Node *tail;

public:
    Arena() noexcept : head(nullptr), tail(nullptr)
    {
    }
    Arena(Arena &&rt) noexcept : Arena()
    {
        using std::swap;
        swap(head, rt.head);
        swap(tail, rt.tail);
    }
    ~Arena()
    {
        while(head)
        {
            Node *deleteMe = head;
            head = head->arenaNext;
            delete deleteMe;
        }
    }
    Arena &operator=(Arena rt) noexcept
    {
        using std::swap;
        swap(head, rt.head);
        swap(tail, rt.tail);
        return *this;
    }
    template <typename T, typename... Args>
    typename std::enable_if<std::is_base_of<Node, T>::value, T *>::type make(Args &&... args)
    {
        T *retval = new T(std::forward<Args>(args)...);
        Node *node = retval;
        node->arenaNext = tail;
        if(!tail)
            head = node;
        else
            tail->arenaNext = node;
        tail = node;
        return retval;
    }
};
}
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_AST_ARENA_H_ */
