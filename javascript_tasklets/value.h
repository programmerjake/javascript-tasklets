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

#ifndef JAVASCRIPT_TASKLETS_VALUE_H_
#define JAVASCRIPT_TASKLETS_VALUE_H_

#include "gc.h"
#include <utility>
#include <cstdint>

namespace javascript_tasklets
{
namespace value
{
struct ObjectHandle;
struct SymbolHandle;
struct BooleanHandle;
struct StringHandle;
struct NameHandle;
struct UndefinedHandle final
{
    void get() const noexcept
    {
    }
};
struct NullHandle final
{
    std::nullptr_t get() const noexcept
    {
        return nullptr;
    }
};
struct NumberHandle;
struct PrimitiveHandle;
struct Int32Handle;
struct UInt32Handle;
struct DoubleHandle;
struct IntegerHandle;

struct ValueHandle final
{
    Handle<gc::Value> value;
    ValueHandle() noexcept : value()
    {
    }
    ValueHandle(Handle<gc::Value> value) noexcept : value(std::move(value))
    {
    }
    ValueHandle(ObjectHandle value) noexcept;
    ValueHandle(SymbolHandle value) noexcept;
    ValueHandle(BooleanHandle value) noexcept;
    ValueHandle(StringHandle value) noexcept;
    ValueHandle(UndefinedHandle value) noexcept;
    ValueHandle(NullHandle value) noexcept;
    ValueHandle(NumberHandle value) noexcept;
    ValueHandle(PrimitiveHandle value) noexcept;
    ValueHandle(Int32Handle value) noexcept;
    ValueHandle(UInt32Handle value) noexcept;
    ValueHandle(DoubleHandle value) noexcept;
    ValueHandle(IntegerHandle value) noexcept;
    explicit operator Handle<gc::Value>() const noexcept
    {
        return value;
    }
    const gc::Value &get() const noexcept
    {
        return value.get();
    }
    bool isUndefined() const noexcept
    {
        return value.get().is<void>();
    }
    UndefinedHandle getUndefined() const noexcept
    {
        constexpr_assert(isUndefined());
        return UndefinedHandle();
    }
    bool isNull() const noexcept
    {
        return value.get().is<std::nullptr_t>();
    }
    NullHandle getNull() const noexcept
    {
        constexpr_assert(isNull());
        return NullHandle();
    }
    bool isBoolean() const noexcept
    {
        return value.get().is<bool>();
    }
    BooleanHandle getBoolean() const noexcept;
    bool isString() const noexcept
    {
        return value.get().is<gc::StringReference>();
    }
    StringHandle getString() const noexcept;
    bool isSymbol() const noexcept
    {
        return value.get().is<gc::SymbolReference>();
    }
    SymbolHandle getSymbol() const noexcept;
    bool isName() const noexcept
    {
        return isString() || isSymbol();
    }
    NameHandle getName() const noexcept;
    bool isDouble() const noexcept
    {
        return value.get().is<double>();
    }
    DoubleHandle getDouble() const noexcept;
    bool isInt32() const noexcept
    {
        return value.get().is<std::int32_t>();
    }
    Int32Handle getInt32() const noexcept;
    bool isUInt32() const noexcept
    {
        return value.get().is<std::uint32_t>();
    }
    UInt32Handle getUInt32() const noexcept;
    bool isInteger() const noexcept
    {
        return isInt32() || isUInt32();
    }
    IntegerHandle getInteger() const noexcept;
    bool isNumber() const noexcept
    {
        return isDouble() || isInt32() || isUInt32();
    }
    NumberHandle getNumber() const noexcept;
    bool isObject() const noexcept
    {
        return value.get().is<gc::ObjectReference>();
    }
    ObjectHandle getObject() const noexcept;
    bool isPrimitive() const noexcept
    {
        return !isObject();
    }
    PrimitiveHandle getPrimitive() const noexcept;
};

struct PropertyHandle;

struct ObjectHandle final
{
    Handle<gc::ObjectReference> value;
    ObjectHandle(std::nullptr_t = nullptr) noexcept : value()
    {
    }
    ObjectHandle(Handle<gc::ObjectReference> value) noexcept : value(std::move(value))
    {
    }
    Handle<gc::ObjectReference> get() const noexcept
    {
        return value;
    }
    bool empty() const noexcept
    {
        return value.get() == nullptr;
    }
    ObjectHandle getPrototype(GC &gc) const;
    bool setPrototype(ObjectHandle newPrototype, GC &gc) const;
    bool isExtensible(GC &gc) const;
    bool preventExtensions(GC &gc) const;
    PropertyHandle getOwnProperty(NameHandle name, GC &gc) const;
    bool hasProperty(NameHandle name, GC &gc) const;
    ValueHandle getValue(NameHandle name, GC &gc) const;
};

struct PropertyHandle final
{
    bool hasValue;
    ValueHandle value;
    bool hasWritable;
    bool writable;
    bool hasEnumerable;
    bool enumerable;
    bool hasConfigurable;
    bool configurable;
    bool hasGet;
    ObjectHandle get;
    bool hasSet;
    ObjectHandle set;
    PropertyHandle() noexcept : hasValue(false),
                                value(),
                                hasWritable(false),
                                writable(false),
                                hasEnumerable(false),
                                enumerable(false),
                                hasConfigurable(false),
                                configurable(false),
                                hasGet(false),
                                get(),
                                hasSet(false),
                                set()
    {
    }
    PropertyHandle(bool hasValue,
                   ValueHandle value,
                   bool hasWritable,
                   bool writable,
                   bool hasEnumerable,
                   bool enumerable,
                   bool hasConfigurable,
                   bool configurable,
                   bool hasGet,
                   ObjectHandle get,
                   bool hasSet,
                   ObjectHandle set) noexcept : hasValue(hasValue),
                                                value(std::move(value)),
                                                hasWritable(hasWritable),
                                                writable(writable),
                                                hasEnumerable(hasEnumerable),
                                                enumerable(enumerable),
                                                hasConfigurable(hasConfigurable),
                                                configurable(configurable),
                                                hasGet(hasGet),
                                                get(std::move(get)),
                                                hasSet(hasSet),
                                                set(std::move(set))
    {
    }
    bool empty() const noexcept
    {
        return !hasValue && !hasWritable && !hasEnumerable && !hasConfigurable && !hasGet
               && !hasSet;
    }
    static PropertyHandle DefaultData() noexcept
    {
        return PropertyHandle(true,
                              ValueHandle(),
                              true,
                              false,
                              true,
                              false,
                              true,
                              false,
                              false,
                              ObjectHandle(),
                              false,
                              ObjectHandle());
    }
    static PropertyHandle DefaultAccessor() noexcept
    {
        return PropertyHandle(false,
                              ValueHandle(),
                              false,
                              false,
                              true,
                              false,
                              true,
                              false,
                              false,
                              ObjectHandle(),
                              false,
                              ObjectHandle());
    }
    bool isAccessorDescriptor() const noexcept
    {
        if(hasGet || hasSet)
            return true;
        return false;
    }
    bool isDataDescriptor() const noexcept
    {
        if(hasValue || hasWritable)
            return true;
        return false;
    }
    bool isGenericDescriptor() const noexcept
    {
        if(!isAccessorDescriptor() && !isDataDescriptor())
            return true;
        return false;
    }
    PropertyHandle &completePropertyDescriptor() noexcept;
};

struct SymbolHandle final
{
    Handle<gc::SymbolReference> value;
    SymbolHandle(std::nullptr_t = nullptr) noexcept : value()
    {
    }
    SymbolHandle(Handle<gc::SymbolReference> value) noexcept : value(std::move(value))
    {
    }

private:
    template <typename SymbolTag>
    struct GetWellKnownHelper final
    {
    };

public:
    // SymbolTag must have a description static member variable that is a string or c-string
    template <typename SymbolTag>
    static SymbolHandle getWellKnown(GC &gc)
    {
        Handle<gc::Value> value = gc.getGlobalValue<GetWellKnownHelper<SymbolTag>>();
        if(value.get().empty())
        {
            String description = SymbolTag::description;
            Handle<gc::SymbolReference> retval = gc.createSymbol(std::move(description));
            value = retval;
            gc.setGlobalValue<GetWellKnownHelper<SymbolTag>>(std::move(value));
            return std::move(retval);
        }
        return SymbolHandle(
            Handle<gc::SymbolReference>(value, value.get().get<gc::SymbolReference>()));
    }
    Handle<gc::SymbolReference> get() const noexcept
    {
        return value;
    }
    bool empty() const noexcept
    {
        return value.get() == nullptr;
    }
    String getDescription(GC &gc) const noexcept
    {
        return gc.readSymbol(value);
    }
};

struct StringHandle final
{
    Handle<gc::StringReference> value;
    StringHandle(std::nullptr_t = nullptr) noexcept : value()
    {
    }
    StringHandle(Handle<gc::StringReference> value) noexcept : value(std::move(value))
    {
    }
    StringHandle(const String &string, GC &gc) : value(gc.internString(string))
    {
    }
    StringHandle(String &&string, GC &gc) : value(gc.internString(std::move(string)))
    {
    }
    Handle<gc::StringReference> get() const noexcept
    {
        return value;
    }
    bool empty() const noexcept
    {
        return value.get() == nullptr;
    }
    String getValue(GC &gc) const noexcept
    {
        return gc.readString(value);
    }
};
}
}

#endif /* JAVASCRIPT_TASKLETS_VALUE_H_ */
