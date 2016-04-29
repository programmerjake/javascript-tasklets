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
#include <vector>
#include <cmath>

namespace javascript_tasklets
{
namespace value
{
enum class ToPrimitivePreferredType
{
    Default,
    String,
    Number
};

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
    bool isSameValue(UndefinedHandle) const noexcept
    {
        return true;
    }
    bool isSameValueZero(UndefinedHandle) const noexcept
    {
        return true;
    }
};
struct NullHandle final
{
    Handle<std::nullptr_t> get() const noexcept
    {
        return Handle<std::nullptr_t>();
    }
    bool isSameValue(NullHandle) const noexcept
    {
        return true;
    }
    bool isSameValueZero(NullHandle) const noexcept
    {
        return true;
    }
};
struct NumberHandle;
struct PrimitiveHandle;
struct Int32Handle;
struct UInt32Handle;
struct DoubleHandle;
struct IntegerHandle;
struct ObjectOrNullHandle;

struct ValueHandle final
{
    typedef gc::Value ValueType;
    Handle<gc::Value> value;
    ValueHandle() noexcept : value()
    {
    }
    ValueHandle(Handle<gc::Value> value) noexcept : value(std::move(value))
    {
    }
    ValueHandle(ObjectHandle value) noexcept;
    ValueHandle(ObjectOrNullHandle value) noexcept;
    ValueHandle(SymbolHandle value) noexcept;
    ValueHandle(BooleanHandle value) noexcept;
    ValueHandle(StringHandle value) noexcept;
    ValueHandle(UndefinedHandle value) noexcept : value()
    {
    }
    ValueHandle(NullHandle value) noexcept : value()
    {
        this->value.get() = ValueType::make<std::nullptr_t>(nullptr);
    }
    ValueHandle(NumberHandle value) noexcept;
    ValueHandle(PrimitiveHandle value) noexcept;
    ValueHandle(NameHandle value) noexcept;
    ValueHandle(Int32Handle value) noexcept;
    ValueHandle(UInt32Handle value) noexcept;
    ValueHandle(DoubleHandle value) noexcept;
    ValueHandle(IntegerHandle value) noexcept;
    Handle<gc::Value> get() const noexcept
    {
        return value;
    }
    gc::Value getValue(GC &gc) const noexcept
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
    bool isObjectOrNull() const noexcept
    {
        return isObject() || isNull();
    }
    ObjectOrNullHandle getObjectOrNull() const noexcept;
    bool isSameValueZero(const ValueHandle &rt) const noexcept;
    bool isSameValue(const ValueHandle &rt) const noexcept;
    PrimitiveHandle toPrimitive(ToPrimitivePreferredType preferredType, GC &gc) const;
};

struct PropertyHandle;

struct ObjectHandle final
{
    typedef gc::ObjectReference ValueType;
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
    ObjectOrNullHandle ordinaryGetPrototype(GC &gc) const;
    ObjectOrNullHandle getPrototype(GC &gc) const;
    bool getPrototypeIsOrdinary(GC &gc) const;
    BooleanHandle setPrototype(ObjectOrNullHandle newPrototype, GC &gc) const;
    void setPrototypeUnchecked(ObjectOrNullHandle newPrototype, GC &gc) const;
    BooleanHandle ordinaryIsExtensible(GC &gc) const;
    BooleanHandle isExtensible(GC &gc) const;
    BooleanHandle preventExtensions(GC &gc) const;
    PropertyHandle getOwnProperty(NameHandle name, GC &gc) const;
    PropertyHandle getOwnProperty(gc::InternalName name, GC &gc) const;
    BooleanHandle hasOwnProperty(NameHandle name, GC &gc) const;
    BooleanHandle hasOwnProperty(gc::InternalName name, GC &gc) const;
    BooleanHandle hasProperty(NameHandle name, GC &gc) const;
    BooleanHandle ordinaryHasProperty(NameHandle name, GC &gc) const;
    ValueHandle getValue(NameHandle name, ValueHandle reciever, GC &gc) const;
    ObjectOrNullHandle getMethod(NameHandle name, GC &gc) const;
    BooleanHandle ordinarySetValue(NameHandle name,
                                   ValueHandle newValue,
                                   ValueHandle reciever,
                                   GC &gc) const;
    BooleanHandle setValue(NameHandle name,
                           ValueHandle newValue,
                           ValueHandle reciever,
                           GC &gc) const;
    BooleanHandle ordinaryDeleteProperty(NameHandle name, GC &gc) const;
    BooleanHandle deleteProperty(NameHandle name, GC &gc) const;
    BooleanHandle defineOwnProperty(NameHandle name, PropertyHandle property, GC &gc) const;
    BooleanHandle ordinaryDefineOwnProperty(NameHandle name, PropertyHandle property, GC &gc) const;
    static bool validateAndApplyPropertyDescriptor(ObjectOrNullHandle object,
                                                   NameHandle name,
                                                   bool extensible,
                                                   PropertyHandle newProperty,
                                                   PropertyHandle currentProperty,
                                                   GC &gc);
    static bool isCompatiblePropertyDescriptor(bool extensible,
                                               PropertyHandle newProperty,
                                               PropertyHandle currentProperty,
                                               GC &gc);
    ObjectHandle enumerate(GC &gc) const;
    std::vector<NameHandle> ownPropertyKeys(GC &gc) const;
    std::vector<NameHandle> ordinaryOwnPropertyKeys(GC &gc) const;
    ValueHandle call(ValueHandle thisValue, std::vector<ValueHandle> arguments, GC &gc) const;
    bool isCallable(GC &gc) const;
    ObjectHandle construct(std::vector<ValueHandle> arguments,
                           ObjectHandle newTarget,
                           GC &gc) const;
    bool isConstructable(GC &gc) const;
    bool isSameValueZero(const ObjectHandle &rt) const noexcept
    {
        return isSameValue(rt);
    }
    bool isSameValue(const ObjectHandle &rt) const noexcept
    {
        return value.get() == rt.value.get();
    }
    PrimitiveHandle ordinaryToPrimitive(ToPrimitivePreferredType preferredType, GC &gc) const;
    PrimitiveHandle toPrimitive(ToPrimitivePreferredType preferredType, GC &gc) const;
    static ObjectHandle create(ObjectOrNullHandle prototype, GC &gc);
    static ObjectHandle create(Handle<gc::ObjectDescriptorReference> objectDescriptor,
                               std::unique_ptr<gc::Object::ExtraData> extraData,
                               ObjectOrNullHandle prototype,
                               GC &gc);
    static void throwTypeError(StringHandle message, GC &gc);
    static void throwTypeError(const String &message, GC &gc);
    static void throwTypeError(String &&message, GC &gc);

private:
    PropertyHandle getOwnProperty(gc::Name name, GC &gc) const;
    BooleanHandle hasOwnProperty(gc::Name name, GC &gc) const;
    void setOwnProperty(NameHandle name, const PropertyHandle &property, GC &gc) const;
    void setOwnProperty(NameHandle name,
                        const PropertyHandle &property,
                        GC &gc,
                        bool putInObjectDescriptor) const;
    void setOwnProperty(gc::Name name, const PropertyHandle &property, GC &gc) const;
    void setOwnProperty(gc::Name name,
                        const PropertyHandle &property,
                        GC &gc,
                        bool putInObjectDescriptor) const;
};

inline ValueHandle::ValueHandle(ObjectHandle value) noexcept : value(value.get())
{
}

inline ObjectHandle ValueHandle::getObject() const noexcept
{
    return ObjectHandle(Handle<gc::ObjectReference>(value, value.get().get<gc::ObjectReference>()));
}

struct ObjectOrNullHandle final
{
    typedef variant<gc::ObjectReference, std::nullptr_t> ValueType;
    Handle<ValueType> value;
    ObjectOrNullHandle(std::nullptr_t = nullptr) noexcept : value()
    {
        value.get() = ValueType::make<std::nullptr_t>(nullptr);
    }
    ObjectOrNullHandle(NullHandle) noexcept : value()
    {
        value.get() = ValueType::make<std::nullptr_t>(nullptr);
    }
    ObjectOrNullHandle(ObjectHandle value) noexcept : value(value.get())
    {
    }
    ObjectOrNullHandle(Handle<ValueType> value) noexcept : value(value)
    {
    }
    ObjectOrNullHandle(Handle<gc::ObjectReference> value) noexcept : value()
    {
        if(value.get() == nullptr)
        {
            this->value.get() = ValueType::make<std::nullptr_t>(nullptr);
        }
        else
        {
            this->value.get() = ValueType::make<gc::ObjectReference>(value.get());
        }
    }
    bool empty() const noexcept
    {
        return value.get().empty();
    }
    Handle<ValueType> get() const noexcept
    {
        return value;
    }
    bool isObject() const noexcept
    {
        return value.get().is<gc::ObjectReference>();
    }
    ObjectHandle getObject() const noexcept
    {
        return ObjectHandle(
            Handle<gc::ObjectReference>(value, value.get().get<gc::ObjectReference>()));
    }
    Handle<gc::ObjectReference> toObjectReference() const noexcept
    {
        if(isObject())
            return Handle<gc::ObjectReference>(value, value.get().get<gc::ObjectReference>());
        return Handle<gc::ObjectReference>();
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
    bool isSameValueZero(const ObjectOrNullHandle &rt) const noexcept
    {
        return isSameValue(rt);
    }
    bool isSameValue(const ObjectOrNullHandle &rt) const noexcept
    {
        return value.get() == rt.value.get();
    }
    PrimitiveHandle toPrimitive(ToPrimitivePreferredType preferredType, GC &gc) const;
};

inline ValueHandle::ValueHandle(ObjectOrNullHandle value) noexcept : value(value.get())
{
}

inline ObjectOrNullHandle ValueHandle::getObjectOrNull() const noexcept
{
    constexpr_assert(isObjectOrNull());
    return ObjectOrNullHandle(Handle<ObjectOrNullHandle::ValueType>(value));
}

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
    ObjectOrNullHandle get;
    bool hasSet;
    ObjectOrNullHandle set;
    PropertyHandle() noexcept : hasValue(false),
                                value(),
                                hasWritable(false),
                                writable(false),
                                hasEnumerable(false),
                                enumerable(false),
                                hasConfigurable(false),
                                configurable(false),
                                hasGet(false),
                                get(nullptr),
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
                   ObjectOrNullHandle get,
                   bool hasSet,
                   ObjectOrNullHandle set) noexcept : hasValue(hasValue),
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
    PropertyHandle(bool hasValue,
                   ValueHandle value,
                   bool hasWritable,
                   bool writable,
                   bool hasEnumerable,
                   bool enumerable,
                   bool hasConfigurable,
                   bool configurable) noexcept : hasValue(hasValue),
                                                 value(std::move(value)),
                                                 hasWritable(hasWritable),
                                                 writable(writable),
                                                 hasEnumerable(hasEnumerable),
                                                 enumerable(enumerable),
                                                 hasConfigurable(hasConfigurable),
                                                 configurable(configurable),
                                                 hasGet(false),
                                                 get(nullptr),
                                                 hasSet(false),
                                                 set(nullptr)
    {
    }
    PropertyHandle(ValueHandle value, bool writable, bool enumerable, bool configurable) noexcept
        : hasValue(true),
          value(std::move(value)),
          hasWritable(true),
          writable(writable),
          hasEnumerable(true),
          enumerable(enumerable),
          hasConfigurable(true),
          configurable(configurable),
          hasGet(false),
          get(nullptr),
          hasSet(false),
          set(nullptr)
    {
    }
    PropertyHandle(bool hasEnumerable,
                   bool enumerable,
                   bool hasConfigurable,
                   bool configurable,
                   bool hasGet = false,
                   ObjectOrNullHandle get = ObjectOrNullHandle(nullptr),
                   bool hasSet = false,
                   ObjectOrNullHandle set = ObjectOrNullHandle(nullptr)) noexcept
        : hasValue(false),
          value(),
          hasWritable(false),
          writable(false),
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
    PropertyHandle(bool enumerable,
                   bool configurable,
                   ObjectOrNullHandle get,
                   ObjectOrNullHandle set) noexcept : hasValue(false),
                                                      value(),
                                                      hasWritable(false),
                                                      writable(false),
                                                      hasEnumerable(true),
                                                      enumerable(enumerable),
                                                      hasConfigurable(true),
                                                      configurable(configurable),
                                                      hasGet(true),
                                                      get(std::move(get)),
                                                      hasSet(true),
                                                      set(std::move(set))
    {
    }
    PropertyHandle(bool enumerable, bool configurable) noexcept : hasValue(false),
                                                                  value(),
                                                                  hasWritable(false),
                                                                  writable(false),
                                                                  hasEnumerable(true),
                                                                  enumerable(enumerable),
                                                                  hasConfigurable(true),
                                                                  configurable(configurable),
                                                                  hasGet(false),
                                                                  get(nullptr),
                                                                  hasSet(false),
                                                                  set(nullptr)
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
                              ObjectOrNullHandle(),
                              false,
                              ObjectOrNullHandle());
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
                              ObjectOrNullHandle(),
                              false,
                              ObjectOrNullHandle());
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
    bool isCompleteDescriptor() const noexcept
    {
        if(isDataDescriptor())
        {
            return hasConfigurable && hasEnumerable && hasValue && hasWritable;
        }
        return hasConfigurable && hasEnumerable && hasGet && hasSet;
    }
    PropertyHandle &completePropertyDescriptor() noexcept;
    bool isSameValue(const PropertyHandle &rt) const noexcept
    {
        if(hasValue != rt.hasValue)
            return false;
        if(hasWritable != rt.hasWritable)
            return false;
        if(hasGet != rt.hasGet)
            return false;
        if(hasSet != rt.hasSet)
            return false;
        if(hasConfigurable != rt.hasConfigurable)
            return false;
        if(hasEnumerable != rt.hasEnumerable)
            return false;
        if(hasValue && !value.isSameValue(rt.value))
            return false;
        if(hasWritable && writable != rt.writable)
            return false;
        if(hasGet && !get.isSameValue(rt.get))
            return false;
        if(hasSet && !set.isSameValue(rt.set))
            return false;
        if(hasConfigurable && configurable != rt.configurable)
            return false;
        if(hasEnumerable && enumerable != rt.enumerable)
            return false;
        return true;
    }
    bool isSameValueZero(const PropertyHandle &rt) const noexcept
    {
        if(hasValue != rt.hasValue)
            return false;
        if(hasWritable != rt.hasWritable)
            return false;
        if(hasGet != rt.hasGet)
            return false;
        if(hasSet != rt.hasSet)
            return false;
        if(hasConfigurable != rt.hasConfigurable)
            return false;
        if(hasEnumerable != rt.hasEnumerable)
            return false;
        if(hasValue && !value.isSameValueZero(rt.value))
            return false;
        if(hasWritable && writable != rt.writable)
            return false;
        if(hasGet && !get.isSameValueZero(rt.get))
            return false;
        if(hasSet && !set.isSameValueZero(rt.set))
            return false;
        if(hasConfigurable && configurable != rt.configurable)
            return false;
        if(hasEnumerable && enumerable != rt.enumerable)
            return false;
        return true;
    }
};

struct SymbolHandle final
{
    typedef gc::SymbolReference ValueType;
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
            gc.setGlobalValue<GetWellKnownHelper<SymbolTag>>(Handle<gc::Value>(retval));
            return std::move(retval);
        }
        return SymbolHandle(
            Handle<gc::SymbolReference>(value, value.get().get<gc::SymbolReference>()));
    }
    struct HasInstanceTag final
    {
        static constexpr auto description = u"Symbol.hasInstance";
    };
    static SymbolHandle getHasInstance(GC &gc)
    {
        return getWellKnown<HasInstanceTag>(gc);
    }
    struct IsConcatSpreadableTag final
    {
        static constexpr auto description = u"Symbol.isConcatSpreadable";
    };
    static SymbolHandle getIsConcatSpreadable(GC &gc)
    {
        return getWellKnown<IsConcatSpreadableTag>(gc);
    }
    struct IteratorTag final
    {
        static constexpr auto description = u"Symbol.iterator";
    };
    static SymbolHandle getIterator(GC &gc)
    {
        return getWellKnown<IteratorTag>(gc);
    }
    struct MatchTag final
    {
        static constexpr auto description = u"Symbol.match";
    };
    static SymbolHandle getMatch(GC &gc)
    {
        return getWellKnown<MatchTag>(gc);
    }
    struct ReplaceTag final
    {
        static constexpr auto description = u"Symbol.replace";
    };
    static SymbolHandle getReplace(GC &gc)
    {
        return getWellKnown<ReplaceTag>(gc);
    }
    struct SearchTag final
    {
        static constexpr auto description = u"Symbol.search";
    };
    static SymbolHandle getSearch(GC &gc)
    {
        return getWellKnown<SearchTag>(gc);
    }
    struct SpeciesTag final
    {
        static constexpr auto description = u"Symbol.species";
    };
    static SymbolHandle getSpecies(GC &gc)
    {
        return getWellKnown<SpeciesTag>(gc);
    }
    struct SplitTag final
    {
        static constexpr auto description = u"Symbol.split";
    };
    static SymbolHandle getSplit(GC &gc)
    {
        return getWellKnown<SplitTag>(gc);
    }
    struct ToPrimitiveTag final
    {
        static constexpr auto description = u"Symbol.toPrimitive";
    };
    static SymbolHandle getToPrimitive(GC &gc)
    {
        return getWellKnown<ToPrimitiveTag>(gc);
    }
    struct ToStringTagTag final
    {
        static constexpr auto description = u"Symbol.toStringTag";
    };
    static SymbolHandle getToStringTag(GC &gc)
    {
        return getWellKnown<ToStringTagTag>(gc);
    }
    struct UnscopablesTag final
    {
        static constexpr auto description = u"Symbol.unscopables";
    };
    static SymbolHandle getUnscopables(GC &gc)
    {
        return getWellKnown<UnscopablesTag>(gc);
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
    bool isSameValueZero(const SymbolHandle &rt) const noexcept
    {
        return isSameValue(rt);
    }
    bool isSameValue(const SymbolHandle &rt) const noexcept
    {
        return value.get() == rt.value.get();
    }
};

inline ValueHandle::ValueHandle(SymbolHandle value) noexcept : value(value.get())
{
}

inline SymbolHandle ValueHandle::getSymbol() const noexcept
{
    return SymbolHandle(Handle<gc::SymbolReference>(value, value.get().get<gc::SymbolReference>()));
}

struct StringHandle final
{
    typedef gc::StringReference ValueType;
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
    bool isSameValueZero(const StringHandle &rt) const noexcept
    {
        return isSameValue(rt);
    }
    bool isSameValue(const StringHandle &rt) const noexcept
    {
        return value.get() == rt.value.get();
    }
    static constexpr char16_t getDigitCharacter(unsigned value) noexcept
    {
        return constexpr_assert(value < 36), value < 10 ? static_cast<char16_t>(value + '0') :
                                                          static_cast<char16_t>(value - 10 + 'a');
    }
    static double toNumberValue(const String &str) noexcept;
};

inline ValueHandle::ValueHandle(StringHandle value) noexcept : value(value.get())
{
}

inline StringHandle ValueHandle::getString() const noexcept
{
    return StringHandle(Handle<gc::StringReference>(value, value.get().get<gc::StringReference>()));
}

struct BooleanHandle final
{
    typedef bool ValueType;
    Handle<bool> value;
    BooleanHandle(std::nullptr_t = nullptr) noexcept : value()
    {
    }
    BooleanHandle(Handle<bool> value) noexcept : value(std::move(value))
    {
    }
    BooleanHandle(bool value, GC &gc) : value(gc, value)
    {
    }
    Handle<bool> get() const noexcept
    {
        return value;
    }
    bool getValue(GC &gc) const noexcept
    {
        return value.get();
    }
    bool isSameValueZero(const BooleanHandle &rt) const noexcept
    {
        return isSameValue(rt);
    }
    bool isSameValue(const BooleanHandle &rt) const noexcept
    {
        return value.get() == rt.value.get();
    }
};

inline ValueHandle::ValueHandle(BooleanHandle value) noexcept : value(value.get())
{
}

inline BooleanHandle ValueHandle::getBoolean() const noexcept
{
    return BooleanHandle(Handle<bool>(value, value.get().get<bool>()));
}

struct Int32Handle final
{
    typedef std::int32_t ValueType;
    Handle<std::int32_t> value;
    Int32Handle(std::nullptr_t = nullptr) noexcept : value()
    {
    }
    Int32Handle(Handle<std::int32_t> value) noexcept : value(std::move(value))
    {
    }
    Int32Handle(std::int32_t value, GC &gc) : value(gc, value)
    {
    }
    Handle<std::int32_t> get() const noexcept
    {
        return value;
    }
    std::int32_t getValue(GC &gc) const noexcept
    {
        return value.get();
    }
    bool isSameValueZero(const Int32Handle &rt) const noexcept
    {
        return isSameValue(rt);
    }
    bool isSameValue(const Int32Handle &rt) const noexcept
    {
        return value.get() == rt.value.get();
    }
    static String toStringValue(std::int32_t value, unsigned base = 10);
};

inline ValueHandle::ValueHandle(Int32Handle value) noexcept : value(value.get())
{
}

inline Int32Handle ValueHandle::getInt32() const noexcept
{
    return Int32Handle(Handle<std::int32_t>(value, value.get().get<std::int32_t>()));
}

struct UInt32Handle final
{
    typedef std::uint32_t ValueType;
    Handle<std::uint32_t> value;
    UInt32Handle(std::nullptr_t = nullptr) noexcept : value()
    {
    }
    UInt32Handle(Handle<std::uint32_t> value) noexcept : value(std::move(value))
    {
    }
    UInt32Handle(std::uint32_t value, GC &gc) : value(gc, value)
    {
    }
    Handle<std::uint32_t> get() const noexcept
    {
        return value;
    }
    std::uint32_t getValue(GC &gc) const noexcept
    {
        return value.get();
    }
    bool isSameValueZero(const UInt32Handle &rt) const noexcept
    {
        return isSameValue(rt);
    }
    bool isSameValue(const UInt32Handle &rt) const noexcept
    {
        return value.get() == rt.value.get();
    }
    static String toStringValue(std::uint32_t value, unsigned base = 10);
};

inline ValueHandle::ValueHandle(UInt32Handle value) noexcept : value(value.get())
{
}

inline UInt32Handle ValueHandle::getUInt32() const noexcept
{
    return UInt32Handle(Handle<std::uint32_t>(value, value.get().get<std::uint32_t>()));
}

struct DoubleHandle final
{
    typedef double ValueType;
    Handle<double> value;
    DoubleHandle(std::nullptr_t = nullptr) noexcept : value()
    {
    }
    DoubleHandle(Handle<double> value) noexcept : value(std::move(value))
    {
    }
    DoubleHandle(double value, GC &gc) : value(gc, value)
    {
    }
    Handle<double> get() const noexcept
    {
        return value;
    }
    double getValue(GC &gc) const noexcept
    {
        return value.get();
    }
    bool isSameValueZero(const DoubleHandle &rt) const noexcept
    {
        if(std::isnan(value.get()))
            return std::isnan(rt.value.get());
        return value.get() == rt.value.get();
    }
    bool isSameValue(const DoubleHandle &rt) const noexcept
    {
        if(std::isnan(value.get()))
            return std::isnan(rt.value.get());
        if(value.get() == 0)
        {
            if(rt.value.get() != 0)
                return false;
            return std::signbit(value.get()) == std::signbit(rt.value.get());
        }
        return value.get() == rt.value.get();
    }
    static String toStringValue(double value, unsigned base = 10);
};

inline ValueHandle::ValueHandle(DoubleHandle value) noexcept : value(value.get())
{
}

inline DoubleHandle ValueHandle::getDouble() const noexcept
{
    return DoubleHandle(Handle<double>(value, value.get().get<double>()));
}

struct NameHandle final
{
    typedef variant<gc::StringReference, gc::SymbolReference> ValueType;
    Handle<ValueType> value;
    NameHandle(std::nullptr_t = nullptr) noexcept : value()
    {
    }
    NameHandle(StringHandle value) noexcept : value(value.get())
    {
    }
    NameHandle(SymbolHandle value) noexcept : value(value.get())
    {
    }
    NameHandle(Handle<ValueType> value) noexcept : value(value)
    {
    }
    bool empty() const noexcept
    {
        return value.get().empty();
    }
    Handle<ValueType> get() const noexcept
    {
        return value;
    }
    bool isString() const noexcept
    {
        return value.get().is<gc::StringReference>();
    }
    StringHandle getString() const noexcept
    {
        return StringHandle(
            Handle<gc::StringReference>(value, value.get().get<gc::StringReference>()));
    }
    bool isSymbol() const noexcept
    {
        return value.get().is<gc::SymbolReference>();
    }
    SymbolHandle getSymbol() const noexcept
    {
        return SymbolHandle(
            Handle<gc::SymbolReference>(value, value.get().get<gc::SymbolReference>()));
    }
    bool isSameValueZero(const NameHandle &rt) const noexcept
    {
        return isSameValue(rt);
    }
    bool isSameValue(const NameHandle &rt) const noexcept
    {
        return value.get() == rt.value.get();
    }
};

inline ValueHandle::ValueHandle(NameHandle value) noexcept : value(value.get())
{
}

inline NameHandle ValueHandle::getName() const noexcept
{
    constexpr_assert(isName());
    return NameHandle(Handle<NameHandle::ValueType>(value));
}

struct IntegerHandle final
{
    typedef variant<std::uint32_t, std::int32_t> ValueType;
    Handle<ValueType> value;
    IntegerHandle(std::nullptr_t = nullptr) noexcept : value()
    {
    }
    IntegerHandle(Int32Handle value) noexcept : value(value.get())
    {
    }
    IntegerHandle(UInt32Handle value) noexcept : value(value.get())
    {
    }
    IntegerHandle(Handle<ValueType> value) noexcept : value(value)
    {
    }
    bool empty() const noexcept
    {
        return value.get().empty();
    }
    Handle<ValueType> get() const noexcept
    {
        return value;
    }
    bool isInt32() const noexcept
    {
        return value.get().is<std::int32_t>();
    }
    Int32Handle getInt32() const noexcept
    {
        return Int32Handle(Handle<std::int32_t>(value, value.get().get<std::int32_t>()));
    }
    bool isUInt32() const noexcept
    {
        return value.get().is<std::uint32_t>();
    }
    UInt32Handle getUInt32() const noexcept
    {
        return UInt32Handle(Handle<std::uint32_t>(value, value.get().get<std::uint32_t>()));
    }
    bool isSameValueZero(const IntegerHandle &rt) const noexcept
    {
        return isSameValue(rt);
    }
    bool isSameValue(const IntegerHandle &rt) const noexcept
    {
        if(isUInt32())
        {
            std::uint32_t lValue = value.get().get<std::uint32_t>();
            if(rt.isUInt32())
                return lValue == rt.value.get().get<std::uint32_t>();
            std::int32_t rValue = rt.value.get().get<std::int32_t>();
            if(rValue < 0)
                return false;
            return lValue == static_cast<std::uint32_t>(rValue);
        }
        constexpr_assert(isInt32());
        std::int32_t lValue = value.get().get<std::int32_t>();
        if(rt.isInt32())
            return lValue == rt.value.get().get<std::int32_t>();
        std::uint32_t rValue = rt.value.get().get<std::uint32_t>();
        if(lValue < 0)
            return false;
        return static_cast<std::uint32_t>(lValue) == rValue;
    }
};

inline ValueHandle::ValueHandle(IntegerHandle value) noexcept : value(value.get())
{
}

inline IntegerHandle ValueHandle::getInteger() const noexcept
{
    constexpr_assert(isInteger());
    return IntegerHandle(Handle<IntegerHandle::ValueType>(value));
}

struct NumberHandle final
{
    typedef variant<double, std::uint32_t, std::int32_t> ValueType;
    Handle<ValueType> value;
    NumberHandle(std::nullptr_t = nullptr) noexcept : value()
    {
    }
    NumberHandle(DoubleHandle value) noexcept : value(value.get())
    {
    }
    NumberHandle(Int32Handle value) noexcept : value(value.get())
    {
    }
    NumberHandle(UInt32Handle value) noexcept : value(value.get())
    {
    }
    NumberHandle(IntegerHandle value) noexcept : value(value.get())
    {
    }
    NumberHandle(Handle<ValueType> value) noexcept : value(value)
    {
    }
    bool empty() const noexcept
    {
        return value.get().empty();
    }
    Handle<ValueType> get() const noexcept
    {
        return value;
    }
    bool isInt32() const noexcept
    {
        return value.get().is<std::int32_t>();
    }
    Int32Handle getInt32() const noexcept
    {
        return Int32Handle(Handle<std::int32_t>(value, value.get().get<std::int32_t>()));
    }
    bool isUInt32() const noexcept
    {
        return value.get().is<std::uint32_t>();
    }
    UInt32Handle getUInt32() const noexcept
    {
        return UInt32Handle(Handle<std::uint32_t>(value, value.get().get<std::uint32_t>()));
    }
    bool isDouble() const noexcept
    {
        return value.get().is<double>();
    }
    DoubleHandle getDouble() const noexcept
    {
        return DoubleHandle(Handle<double>(value, value.get().get<double>()));
    }
    bool isInteger() const noexcept
    {
        return isInt32() || isUInt32();
    }
    IntegerHandle getInteger() const noexcept
    {
        constexpr_assert(isInteger());
        return IntegerHandle(Handle<IntegerHandle::ValueType>(value));
    }
    bool isSameValueZero(const NumberHandle &rt) const noexcept
    {
        if(isInteger() && rt.isInteger())
            return getInteger().isSameValueZero(rt.getInteger());
        if(isDouble() && rt.isDouble())
            return getDouble().isSameValueZero(rt.getDouble());
        if(isUInt32())
        {
            return value.get().get<std::uint32_t>() == rt.value.get().get<double>();
        }
        if(isInt32())
        {
            return value.get().get<std::int32_t>() == rt.value.get().get<double>();
        }
        if(rt.isInt32())
        {
            return value.get().get<double>() == rt.value.get().get<std::int32_t>();
        }
        return value.get().get<double>() == rt.value.get().get<std::uint32_t>();
    }
    bool isSameValue(const NumberHandle &rt) const noexcept
    {
        if(isInteger() && rt.isInteger())
            return getInteger().isSameValueZero(rt.getInteger());
        if(isDouble() && rt.isDouble())
            return getDouble().isSameValueZero(rt.getDouble());
        if(isUInt32())
        {
            std::uint32_t lValue = value.get().get<std::uint32_t>();
            double rValue = rt.value.get().get<double>();
            if(lValue != 0)
                return lValue == rValue;
            if(rValue != 0)
                return false;
            if(std::signbit(rValue))
                return false;
            return true;
        }
        if(isInt32())
        {
            std::int32_t lValue = value.get().get<std::int32_t>();
            double rValue = rt.value.get().get<double>();
            if(lValue != 0)
                return lValue == rValue;
            if(rValue != 0)
                return false;
            if(std::signbit(rValue))
                return false;
            return true;
        }
        double lValue = value.get().get<double>();
        if(rt.isInt32())
        {
            std::int32_t rValue = rt.value.get().get<std::int32_t>();
            if(rValue != 0)
                return lValue == rValue;
            if(lValue != 0)
                return false;
            if(std::signbit(lValue))
                return false;
            return true;
        }
        std::uint32_t rValue = rt.value.get().get<std::uint32_t>();
        if(rValue != 0)
            return lValue == rValue;
        if(lValue != 0)
            return false;
        if(std::signbit(lValue))
            return false;
        return true;
    }
};

inline ValueHandle::ValueHandle(NumberHandle value) noexcept : value(value.get())
{
}

inline NumberHandle ValueHandle::getNumber() const noexcept
{
    constexpr_assert(isNumber());
    return NumberHandle(Handle<NumberHandle::ValueType>(value));
}

inline bool ValueHandle::isSameValue(const ValueHandle &rt) const noexcept
{
    if(!isNumber())
        return value.get() == rt.value.get();
    if(!rt.isNumber())
        return false;
    return getNumber().isSameValue(rt.getNumber());
}

inline bool ValueHandle::isSameValueZero(const ValueHandle &rt) const noexcept
{
    if(!isNumber())
        return value.get() == rt.value.get();
    if(!rt.isNumber())
        return false;
    return getNumber().isSameValueZero(rt.getNumber());
}

struct PrimitiveHandle final
{
    typedef variant<double,
                    std::int32_t,
                    std::uint32_t,
                    gc::StringReference,
                    gc::SymbolReference,
                    bool,
                    std::nullptr_t> ValueType;
    Handle<ValueType> value;
    PrimitiveHandle() noexcept : value()
    {
    }
    PrimitiveHandle(Handle<ValueType> value) noexcept : value(std::move(value))
    {
    }
    PrimitiveHandle(SymbolHandle value) noexcept : value(value.get())
    {
    }
    PrimitiveHandle(BooleanHandle value) noexcept : value(value.get())
    {
    }
    PrimitiveHandle(StringHandle value) noexcept : value(value.get())
    {
    }
    PrimitiveHandle(UndefinedHandle value) noexcept : value()
    {
    }
    PrimitiveHandle(NullHandle) noexcept : value()
    {
        value.get() = ValueType::make<std::nullptr_t>(nullptr);
    }
    PrimitiveHandle(NumberHandle value) noexcept : value(value.get())
    {
    }
    PrimitiveHandle(Int32Handle value) noexcept : value(value.get())
    {
    }
    PrimitiveHandle(UInt32Handle value) noexcept : value(value.get())
    {
    }
    PrimitiveHandle(DoubleHandle value) noexcept : value(value.get())
    {
    }
    PrimitiveHandle(IntegerHandle value) noexcept : value(value.get())
    {
    }
    Handle<ValueType> get() const noexcept
    {
        return value;
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
    BooleanHandle getBoolean() const noexcept
    {
        return BooleanHandle(Handle<bool>(value, value.get().get<bool>()));
    }
    bool isString() const noexcept
    {
        return value.get().is<gc::StringReference>();
    }
    StringHandle getString() const noexcept
    {
        return StringHandle(
            Handle<gc::StringReference>(value, value.get().get<gc::StringReference>()));
    }
    bool isSymbol() const noexcept
    {
        return value.get().is<gc::SymbolReference>();
    }
    SymbolHandle getSymbol() const noexcept
    {
        return SymbolHandle(
            Handle<gc::SymbolReference>(value, value.get().get<gc::SymbolReference>()));
    }
    bool isName() const noexcept
    {
        return isString() || isSymbol();
    }
    NameHandle getName() const noexcept
    {
        constexpr_assert(isName());
        return NameHandle(Handle<NameHandle::ValueType>(value));
    }
    bool isDouble() const noexcept
    {
        return value.get().is<double>();
    }
    DoubleHandle getDouble() const noexcept
    {
        return DoubleHandle(Handle<double>(value, value.get().get<double>()));
    }
    bool isInt32() const noexcept
    {
        return value.get().is<std::int32_t>();
    }
    Int32Handle getInt32() const noexcept
    {
        return Int32Handle(Handle<std::int32_t>(value, value.get().get<std::int32_t>()));
    }
    bool isUInt32() const noexcept
    {
        return value.get().is<std::uint32_t>();
    }
    UInt32Handle getUInt32() const noexcept
    {
        return UInt32Handle(Handle<std::uint32_t>(value, value.get().get<std::uint32_t>()));
    }
    bool isInteger() const noexcept
    {
        return isInt32() || isUInt32();
    }
    IntegerHandle getInteger() const noexcept
    {
        constexpr_assert(isInteger());
        return IntegerHandle(Handle<IntegerHandle::ValueType>(value));
    }
    bool isNumber() const noexcept
    {
        return isDouble() || isInt32() || isUInt32();
    }
    NumberHandle getNumber() const noexcept
    {
        constexpr_assert(isNumber());
        return NumberHandle(Handle<NumberHandle::ValueType>(value));
    }
    bool isObjectOrNull() const noexcept
    {
        return isNull();
    }
    ObjectOrNullHandle getObjectOrNull() const noexcept
    {
        constexpr_assert(isObjectOrNull());
        return ObjectOrNullHandle(getNull());
    }
    bool isSameValue(const PrimitiveHandle &rt) const noexcept
    {
        if(!isNumber())
            return value.get() == rt.value.get();
        if(!rt.isNumber())
            return false;
        return getNumber().isSameValue(rt.getNumber());
    }
    bool isSameValueZero(const PrimitiveHandle &rt) const noexcept
    {
        if(!isNumber())
            return value.get() == rt.value.get();
        if(!rt.isNumber())
            return false;
        return getNumber().isSameValueZero(rt.getNumber());
    }
};

inline ValueHandle::ValueHandle(PrimitiveHandle value) noexcept : value(value.get())
{
}

inline PrimitiveHandle ValueHandle::getPrimitive() const noexcept
{
    constexpr_assert(isPrimitive());
    return PrimitiveHandle(Handle<PrimitiveHandle::ValueType>(value));
}

inline PrimitiveHandle ValueHandle::toPrimitive(ToPrimitivePreferredType preferredType,
                                                GC &gc) const
{
    if(isObject())
        return getObject().toPrimitive(preferredType, gc);
    return getPrimitive();
}

inline PrimitiveHandle ObjectOrNullHandle::toPrimitive(ToPrimitivePreferredType preferredType,
                                                       GC &gc) const
{
    if(isObject())
        return getObject().toPrimitive(preferredType, gc);
    return getNull();
}

inline void ObjectHandle::throwTypeError(const String &message, GC &gc)
{
    throwTypeError(gc.internString(message), gc);
}

inline void ObjectHandle::throwTypeError(String &&message, GC &gc)
{
    throwTypeError(gc.internString(std::move(message)), gc);
}
}

namespace gc
{
template <>
struct AddHandleToHandleScope<value::ObjectHandle> final
{
    void operator()(HandleScope &handleScope, const value::ObjectHandle &v) const
    {
        AddHandleToHandleScope<value::ObjectHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::SymbolHandle> final
{
    void operator()(HandleScope &handleScope, const value::SymbolHandle &v) const
    {
        AddHandleToHandleScope<value::SymbolHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::BooleanHandle> final
{
    void operator()(HandleScope &handleScope, const value::BooleanHandle &v) const
    {
        AddHandleToHandleScope<value::BooleanHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::StringHandle> final
{
    void operator()(HandleScope &handleScope, const value::StringHandle &v) const
    {
        AddHandleToHandleScope<value::StringHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::NameHandle> final
{
    void operator()(HandleScope &handleScope, const value::NameHandle &v) const
    {
        AddHandleToHandleScope<value::NameHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::UndefinedHandle> final
{
    void operator()(HandleScope &handleScope, const value::UndefinedHandle &) const
    {
    }
};

template <>
struct AddHandleToHandleScope<value::NullHandle> final
{
    void operator()(HandleScope &handleScope, const value::NullHandle &) const
    {
    }
};

template <>
struct AddHandleToHandleScope<value::NumberHandle> final
{
    void operator()(HandleScope &handleScope, const value::NumberHandle &v) const
    {
        AddHandleToHandleScope<value::NumberHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::PrimitiveHandle> final
{
    void operator()(HandleScope &handleScope, const value::PrimitiveHandle &v) const
    {
        AddHandleToHandleScope<value::PrimitiveHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::Int32Handle> final
{
    void operator()(HandleScope &handleScope, const value::Int32Handle &v) const
    {
        AddHandleToHandleScope<value::Int32Handle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::UInt32Handle> final
{
    void operator()(HandleScope &handleScope, const value::UInt32Handle &v) const
    {
        AddHandleToHandleScope<value::UInt32Handle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::DoubleHandle> final
{
    void operator()(HandleScope &handleScope, const value::DoubleHandle &v) const
    {
        AddHandleToHandleScope<value::DoubleHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::IntegerHandle> final
{
    void operator()(HandleScope &handleScope, const value::IntegerHandle &v) const
    {
        AddHandleToHandleScope<value::IntegerHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::ObjectOrNullHandle> final
{
    void operator()(HandleScope &handleScope, const value::ObjectOrNullHandle &v) const
    {
        AddHandleToHandleScope<value::ObjectOrNullHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::ValueHandle> final
{
    void operator()(HandleScope &handleScope, const value::ValueHandle &v) const
    {
        AddHandleToHandleScope<value::ValueHandle::ValueType>()(handleScope, v.get().get());
    }
};

template <>
struct AddHandleToHandleScope<value::PropertyHandle> final
{
    void operator()(HandleScope &handleScope, const value::PropertyHandle &v) const
    {
        AddHandleToHandleScope<value::ValueHandle>()(handleScope, v.value);
        AddHandleToHandleScope<value::ObjectOrNullHandle>()(handleScope, v.get);
        AddHandleToHandleScope<value::ObjectOrNullHandle>()(handleScope, v.set);
    }
};
}
}

#endif /* JAVASCRIPT_TASKLETS_VALUE_H_ */
