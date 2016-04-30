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

#ifndef JAVASCRIPT_TASKLETS_ARRAY_REF_H_
#define JAVASCRIPT_TASKLETS_ARRAY_REF_H_

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <initializer_list>
#include <stdexcept>
#include "constexpr_assert.h"

namespace javascript_tasklets
{
template <typename T>
class ArrayRef final
{
    template <typename>
    friend class ArrayRef;
public:
    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
    value_type *elements;
    std::size_t elementCount;

public:
    constexpr ArrayRef(value_type *elements, std::size_t elementCount) noexcept
        : elements(elementCount ? elements : nullptr),
          elementCount((constexpr_assert(elementCount == 0 || elements), elementCount))
    {
    }
    constexpr ArrayRef() noexcept : elements(nullptr), elementCount(0)
    {
    }
    template <typename T2, typename = typename std::enable_if<std::is_convertible<T2 *, T *>::value>::type>
    constexpr ArrayRef(ArrayRef<T2> rt) noexcept : elements(rt.elements), elementCount(rt.elementCount)
    {
    }
    template <typename = typename std::enable_if<std::is_const<value_type>::value>::type>
    constexpr ArrayRef(std::initializer_list<value_type> init_list) noexcept
        : elements(init_list.size() != 0 ? init_list.begin() : nullptr),
          elementCount(init_list.size())
    {
    }
    template <typename C, typename = typename std::enable_if<!std::is_array<C>::value>::type>
    constexpr explicit ArrayRef(C &&container) noexcept(noexcept(container.size())
                                                        && noexcept(*container.begin()))
        : elements(container.size() == 0 ? nullptr : std::addressof(*container.begin())),
          elementCount(container.size())
    {
    }
    template <std::size_t N>
    constexpr explicit ArrayRef(value_type(&array)[N]) noexcept : elements(array), elementCount(N)
    {
    }
    reference at(std::size_t pos)
    {
        return pos >= elementCount ? throw std::out_of_range("ArrayRef::at") : elements[pos];
    }
    constexpr const_reference at(std::size_t pos) const
    {
        return pos >= elementCount ? throw std::out_of_range("ArrayRef::at") : elements[pos];
    }
    reference operator[](std::size_t pos) noexcept
    {
        return constexpr_assert(pos < elementCount), elements[pos];
    }
    constexpr const_reference operator[](std::size_t pos) const noexcept
    {
        return constexpr_assert(pos < elementCount), elements[pos];
    }
    reference front() noexcept
    {
        return constexpr_assert(elementCount != 0), *elements;
    }
    constexpr const_reference front() const noexcept
    {
        return constexpr_assert(elementCount != 0), *elements;
    }
    reference back() noexcept
    {
        return constexpr_assert(elementCount != 0), elements[elementCount - 1];
    }
    constexpr const_reference back() const noexcept
    {
        return constexpr_assert(elementCount != 0), elements[elementCount - 1];
    }
    pointer data() noexcept
    {
        return elements;
    }
    constexpr const_pointer data() const noexcept
    {
        return elements;
    }
    pointer begin() noexcept
    {
        return elements;
    }
    constexpr const_pointer begin() const noexcept
    {
        return elements;
    }
    constexpr const_pointer cbegin() const noexcept
    {
        return elements;
    }
    pointer end() noexcept
    {
        return elements + elementCount;
    }
    constexpr const_pointer end() const noexcept
    {
        return elements + elementCount;
    }
    constexpr const_pointer cend() const noexcept
    {
        return elements + elementCount;
    }
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    constexpr bool empty() const noexcept
    {
        return elementCount == 0;
    }
    constexpr std::size_t size() const noexcept
    {
        return elementCount;
    }
};
}

#endif /* JAVASCRIPT_TASKLETS_ARRAY_REF_H_ */
