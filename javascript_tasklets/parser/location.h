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

#ifndef JAVASCRIPT_TASKLETS_PARSER_LOCATION_H_
#define JAVASCRIPT_TASKLETS_PARSER_LOCATION_H_

#include "source.h"
#include "../gc.h"

namespace javascript_tasklets
{
namespace parser
{
struct Location final
{
    gc::SourceReference source;
    std::size_t beginPosition;
    std::size_t endPosition; // one past the end
    constexpr Location() noexcept : source(), beginPosition(0), endPosition(0)
    {
    }
    Location(SourceHandle source, std::size_t position) noexcept : source(source.get()),
                                                                   beginPosition(position),
                                                                   endPosition(position + 1)
    {
    }
    Location(SourceHandle source, std::size_t beginPosition, std::size_t endPosition) noexcept
        : source(source.get()),
          beginPosition(beginPosition),
          endPosition(endPosition)
    {
        constexpr_assert(endPosition > beginPosition);
    }
    Location &operator+=(const Location &rt) noexcept
    {
        constexpr_assert(source == rt.source);
        if(beginPosition > rt.beginPosition)
            beginPosition = rt.beginPosition;
        if(endPosition > rt.endPosition)
            endPosition = rt.endPosition;
        return *this;
    }
    Location operator+(const Location &rt) const noexcept
    {
        return Location(*this) += rt;
    }
    String toString() const
    {
        if(source == nullptr)
            return u"<no-location>";
        return source->getLocationString(beginPosition);
    }
};
typedef Handle<Location> LocationHandle;
}
namespace gc
{
template <>
struct AddHandleToHandleScope<parser::Location> final
{
    void operator()(HandleScope &handleScope, const parser::Location &value) const
    {
        AddHandleToHandleScope<SourceReference>()(handleScope, value.source);
    }
};
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_LOCATION_H_ */
