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
#include <iterator>

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
                                                                   endPosition(position)
    {
    }
    Location(SourceHandle source, std::size_t beginPosition, std::size_t endPosition) noexcept
        : source(source.get()),
          beginPosition(beginPosition),
          endPosition(endPosition)
    {
        constexpr_assert(endPosition >= beginPosition);
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

class LocationGetter
{
    friend class GC;
    friend class GC::LocationIterator;
    LocationGetter(const LocationGetter &rt) = delete;
    LocationGetter &operator=(const LocationGetter &rt) = delete;
    void *operator new(std::size_t) = delete;

private:
    const LocationGetter *next;
    GC &gc;

public:
    explicit LocationGetter(GC &gc) noexcept : next(gc.locationGetterStack), gc(gc)
    {
        gc.locationGetterStack = this;
    }
    virtual ~LocationGetter()
    {
        gc.locationGetterStack = next;
    }
    virtual parser::LocationHandle get(GC &gc) const = 0;
};

class GC::LocationIterator final : public std::iterator<std::input_iterator_tag,
                                                        parser::LocationHandle,
                                                        std::ptrdiff_t,
                                                        parser::LocationHandle *,
                                                        parser::LocationHandle>
{
    friend class GC;

private:
    GC *gc;
    const LocationGetter *currentLocationGetter;
    constexpr explicit LocationIterator(GC *gc,
                                        const LocationGetter *currentLocationGetter) noexcept
        : gc(gc),
          currentLocationGetter(currentLocationGetter)
    {
    }

public:
    constexpr LocationIterator() noexcept : gc(nullptr), currentLocationGetter(nullptr)
    {
    }
    LocationIterator &operator++()
    {
        constexpr_assert(gc != nullptr);
        constexpr_assert(currentLocationGetter != nullptr);
        constexpr_assert(&currentLocationGetter->gc == gc);
        currentLocationGetter = currentLocationGetter->next;
        return *this;
    }
    LocationIterator operator++(int)
    {
        LocationIterator retval(*this);
        operator++();
        return retval;
    }
    constexpr bool operator==(const LocationIterator &rt) const noexcept
    {
        return currentLocationGetter == rt.currentLocationGetter;
    }
    constexpr bool operator!=(const LocationIterator &rt) const noexcept
    {
        return currentLocationGetter != rt.currentLocationGetter;
    }
    parser::LocationHandle operator*() const
    {
        constexpr_assert(gc != nullptr);
        constexpr_assert(currentLocationGetter != nullptr);
        constexpr_assert(&currentLocationGetter->gc == gc);
        return currentLocationGetter->get(*gc);
    }
    class LocationHandleHolder final
    {
        friend class LocationIterator;

    private:
        parser::LocationHandle value;
        explicit LocationHandleHolder(parser::LocationHandle value) : value(value)
        {
        }

    public:
        const parser::LocationHandle *operator->() const noexcept
        {
            return &value;
        }
    };
    LocationHandleHolder operator->() const
    {
        return LocationHandleHolder(operator*());
    }
};

inline GC::LocationIterator GC::locationsBegin() noexcept
{
    return LocationIterator(this, locationGetterStack);
}

inline GC::LocationIterator GC::locationsEnd() noexcept
{
    return LocationIterator(this, nullptr);
}

inline GC::LocationIterator GC::Locations::begin() noexcept
{
    return gc->locationsBegin();
}

inline GC::LocationIterator GC::Locations::end() noexcept
{
    return gc->locationsEnd();
}

struct LocalLocationGetter final : public LocationGetter
{
    parser::LocationHandle location;
    LocalLocationGetter(GC &gc, parser::LocationHandle location)
        : LocationGetter(gc), location(location)
    {
    }
    LocalLocationGetter(GC &gc, const parser::Location &location)
        : LocalLocationGetter(gc, parser::LocationHandle(gc, location))
    {
    }
    LocalLocationGetter(GC &gc, String builtinFunctionName)
        : LocalLocationGetter(gc, parser::Location(gc.internBuiltinSource(std::move(builtinFunctionName)), 0))
    {
    }
    virtual parser::LocationHandle get(GC &gc) const override
    {
        return location;
    }
};
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_LOCATION_H_ */
