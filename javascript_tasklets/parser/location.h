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
    std::size_t position;
    Location() noexcept : source(), position(0)
    {
    }
    Location(SourceHandle source, std::size_t position) noexcept : source(source.get()),
                                                                   position(position)
    {
    }
    String toString() const
    {
        if(source == nullptr)
            return u"<no-location>";
        return source->getLocationString(position);
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
        AddHandleToHandleScope<gc::SourceReference>()(handleScope, value.source);
    }
};
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_LOCATION_H_ */
