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

#ifndef JAVASCRIPT_TASKLETS_GC_H_
#define JAVASCRIPT_TASKLETS_GC_H_

#include "string.h"
#include "constexpr_assert.h"

#include <iterator>
#include <cstdint>
#include <vector>
#include <new>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>

namespace javascript_tasklets
{
namespace gc
{
struct BaseReference
{
    std::size_t index;
    static constexpr std::size_t invalidIndex = ~static_cast<std::size_t>(0);

protected:
    constexpr explicit BaseReference(std::size_t index) noexcept : index(index)
    {
    }
    constexpr BaseReference(std::nullptr_t = nullptr) noexcept : index(invalidIndex)
    {
    }

public:
    constexpr bool operator==(const BaseReference &rt) const noexcept
    {
        return index == rt.index;
    }
    constexpr bool operator!=(const BaseReference &rt) const noexcept
    {
        return index != rt.index;
    }
    constexpr bool operator==(std::nullptr_t) const noexcept
    {
        return index == invalidIndex;
    }
    constexpr bool operator!=(std::nullptr_t) const noexcept
    {
        return index != invalidIndex;
    }
    friend bool operator==(std::nullptr_t, const BaseReference &v) noexcept
    {
        return v.index == invalidIndex;
    }
    friend bool operator!=(std::nullptr_t, const BaseReference &v) noexcept
    {
        return v.index != invalidIndex;
    }
};

struct ObjectReference final : public BaseReference
{
    constexpr explicit ObjectReference(std::size_t index) noexcept : BaseReference(index)
    {
    }
    constexpr ObjectReference(std::nullptr_t = nullptr) noexcept : BaseReference(nullptr)
    {
    }
};

struct ObjectMemberIndex final : public BaseReference
{
    constexpr explicit ObjectMemberIndex(std::size_t index) noexcept : BaseReference(index)
    {
    }
    constexpr ObjectMemberIndex(std::nullptr_t = nullptr) noexcept : BaseReference(nullptr)
    {
    }
};

struct StringOrSymbolReference final : public BaseReference
{
    constexpr explicit StringOrSymbolReference(std::size_t index) noexcept : BaseReference(index)
    {
    }
    constexpr StringOrSymbolReference(std::nullptr_t = nullptr) noexcept : BaseReference(nullptr)
    {
    }
};
}
}

namespace std
{
template <>
struct hash<javascript_tasklets::gc::BaseReference>
{
    constexpr std::size_t operator()(const javascript_tasklets::gc::BaseReference &v) const
    {
        return v.index;
    }
};

template <>
struct hash<javascript_tasklets::gc::ObjectReference> final
    : public hash<javascript_tasklets::gc::BaseReference>
{
};

template <>
struct hash<javascript_tasklets::gc::ObjectMemberIndex> final
    : public hash<javascript_tasklets::gc::BaseReference>
{
};

template <>
struct hash<javascript_tasklets::gc::StringOrSymbolReference> final
    : public hash<javascript_tasklets::gc::BaseReference>
{
};
}

namespace javascript_tasklets
{
namespace gc
{
struct TypelessValue
{
    union
    {
        ObjectReference valueObject;
        double valueDouble;
        std::uint32_t valueIntegerOrBoolean;
        StringOrSymbolReference valueStringOrSymbol;
    };
    constexpr TypelessValue() noexcept : valueIntegerOrBoolean()
    {
    }
    constexpr explicit TypelessValue(StringOrSymbolReference valueStringOrSymbol) noexcept
        : valueStringOrSymbol(valueStringOrSymbol)
    {
    }
    constexpr explicit TypelessValue(std::uint32_t valueIntegerOrBoolean) noexcept
        : valueIntegerOrBoolean(valueIntegerOrBoolean)
    {
    }
    constexpr explicit TypelessValue(std::int32_t valueIntegerOrBoolean) noexcept
        : valueIntegerOrBoolean(valueIntegerOrBoolean)
    {
    }
    constexpr explicit TypelessValue(bool valueIntegerOrBoolean) noexcept
        : valueIntegerOrBoolean(valueIntegerOrBoolean)
    {
    }
    constexpr explicit TypelessValue(double valueDouble) noexcept : valueDouble(valueDouble)
    {
    }
    constexpr explicit TypelessValue(ObjectReference valueObject) noexcept
        : valueObject(valueObject)
    {
    }
    template <typename T>
    explicit TypelessValue(T &&) = delete;
    constexpr ObjectReference getObject() const noexcept
    {
        return valueObject;
    }
    constexpr double getDouble() const noexcept
    {
        return valueDouble;
    }
    constexpr std::int32_t getInt32() const noexcept
    {
        return valueIntegerOrBoolean;
    }
    constexpr std::uint32_t getUInt32() const noexcept
    {
        return valueIntegerOrBoolean;
    }
    constexpr bool getBoolean() const noexcept
    {
        return valueIntegerOrBoolean;
    }
    constexpr StringOrSymbolReference getString() const noexcept
    {
        return valueStringOrSymbol;
    }
    constexpr StringOrSymbolReference getSymbol() const noexcept
    {
        return valueStringOrSymbol;
    }
};

struct Value : private TypelessValue
{
    enum class Type : std::uint8_t
    {
        Undefined = 0, // undefined must be zero because that is the default value
        Null,
        String,
        Symbol,
        Boolean,
        Double,
        Int32,
        UInt32,
        Object
    };

private:
    Type type;

public:
    constexpr Value(Type type, const TypelessValue &value) noexcept : TypelessValue(value),
                                                                      type(type)
    {
    }
    constexpr Value() noexcept : TypelessValue(), type(Type::Undefined)
    {
    }
    constexpr TypelessValue toTypelessValue() const noexcept
    {
        return *this;
    }
    static constexpr Value Undefined() noexcept
    {
        return Value();
    }
    static constexpr Value Null() noexcept
    {
        return Value(Type::Null, TypelessValue());
    }
    static constexpr Value String(StringOrSymbolReference value) noexcept
    {
        return constexpr_assert(value != nullptr), Value(Type::String, TypelessValue(value));
    }
    static constexpr Value Symbol(StringOrSymbolReference value) noexcept
    {
        return constexpr_assert(value != nullptr), Value(Type::Symbol, TypelessValue(value));
    }
    static constexpr Value Boolean(bool value) noexcept
    {
        return Value(Type::Boolean, TypelessValue(value));
    }
    static constexpr Value Double(double value) noexcept
    {
        return Value(Type::Double, TypelessValue(value));
    }
    static constexpr Value Int32(std::int32_t value) noexcept
    {
        return Value(Type::Int32, TypelessValue(value));
    }
    static constexpr Value UInt32(std::uint32_t value) noexcept
    {
        return Value(Type::UInt32, TypelessValue(value));
    }
    static constexpr Value Object(ObjectReference value) noexcept
    {
        return constexpr_assert(value != nullptr), Value(Type::Object, TypelessValue(value));
    }
    constexpr ObjectReference getObject() const noexcept
    {
        return constexpr_assert(type == Type::Object), constexpr_assert(valueObject != nullptr),
               valueObject;
    }
    constexpr double getDouble() const noexcept
    {
        return constexpr_assert(type == Type::Double), valueDouble;
    }
    constexpr std::int32_t getInt32() const noexcept
    {
        return constexpr_assert(type == Type::Int32), valueIntegerOrBoolean;
    }
    constexpr std::uint32_t getUInt32() const noexcept
    {
        return constexpr_assert(type == Type::UInt32), valueIntegerOrBoolean;
    }
    constexpr bool getBoolean() const noexcept
    {
        return constexpr_assert(type == Type::Boolean), valueIntegerOrBoolean;
    }
    constexpr StringOrSymbolReference getString() const noexcept
    {
        return constexpr_assert(type == Type::String), valueStringOrSymbol;
    }
    constexpr StringOrSymbolReference getSymbol() const noexcept
    {
        return constexpr_assert(type == Type::Symbol), valueStringOrSymbol;
    }
    constexpr Type getType() const noexcept
    {
        return type;
    }
    constexpr bool isUndefined() const noexcept
    {
        return type == Type::Undefined;
    }
    constexpr bool isNull() const noexcept
    {
        return type == Type::Null;
    }
    constexpr bool isString() const noexcept
    {
        return type == Type::String;
    }
    constexpr bool isSymbol() const noexcept
    {
        return type == Type::Symbol;
    }
    constexpr bool isBoolean() const noexcept
    {
        return type == Type::Boolean;
    }
    constexpr bool isDouble() const noexcept
    {
        return type == Type::Double;
    }
    constexpr bool isInt32() const noexcept
    {
        return type == Type::Int32;
    }
    constexpr bool isUInt32() const noexcept
    {
        return type == Type::UInt32;
    }
    constexpr bool isObject() const noexcept
    {
        return type == Type::Object;
    }
};

class InternalNameMaker final
{
private:
    template <typename Tag>
    struct Implementation final
    {
        static const char variable = 0;
    };

public:
    typedef const void *InternalName;
    template <typename Tag>
    static constexpr InternalName get() noexcept
    {
        return &Implementation<Tag>::variable;
    }
};

template <typename Tag>
const char InternalNameMaker::Implementation<Tag>::variable;

typedef InternalNameMaker::InternalName InternalName;

struct Name final
{
    enum class Type : std::uint8_t
    {
        Empty,
        String,
        Symbol,
        InternalName
    };

private:
    union
    {
        StringOrSymbolReference valueStringOrSymbol;
        InternalName valueInternalName;
    };
    Type type;

public:
    constexpr Name() noexcept : valueInternalName(), type(Type::Empty)
    {
    }

private:
    constexpr Name(Type type, StringOrSymbolReference valueStringOrSymbol) noexcept
        : valueStringOrSymbol((constexpr_assert(valueStringOrSymbol != nullptr),
                               valueStringOrSymbol)),
          type(type)
    {
    }
    constexpr Name(Type type, gc::InternalName valueInternalName) noexcept
        : valueInternalName(valueInternalName),
          type(type)
    {
    }

public:
    constexpr explicit Name(const Value &value) noexcept : Name(fromValue(value))
    {
    }
    static constexpr Name fromValue(const Value &value) noexcept
    {
        return value.isString() ? Name(Type::String, value.getString()) :
                                  Name(Type::Symbol, value.getSymbol());
    }
    static constexpr Name InternalName(gc::InternalName name) noexcept
    {
        return constexpr_assert(name), Name(Type::InternalName, name);
    }
    static constexpr Name Empty() noexcept
    {
        return Name();
    }
    static constexpr Name String(StringOrSymbolReference name) noexcept
    {
        return constexpr_assert(name != nullptr), Name(Type::String, name);
    }
    static constexpr Name Symbol(StringOrSymbolReference name) noexcept
    {
        return constexpr_assert(name != nullptr), Name(Type::Symbol, name);
    }
    constexpr bool isPublicName() const noexcept
    {
        return type == Type::String || type == Type::Symbol;
    }
    constexpr explicit operator Value() const noexcept
    {
        return toValue();
    }
    constexpr Type getType() const noexcept
    {
        return type;
    }
    constexpr bool empty() const noexcept
    {
        return type == Type::Empty;
    }
    constexpr bool isString() const noexcept
    {
        return type == Type::String;
    }
    constexpr bool isSymbol() const noexcept
    {
        return type == Type::Symbol;
    }
    constexpr bool isInternalName() const noexcept
    {
        return type == Type::InternalName;
    }
    constexpr Value toValue() const noexcept
    {
        return constexpr_assert(isPublicName()), type == Type::String ?
                                                     Value::String(valueStringOrSymbol) :
                                                     Value::Symbol(valueStringOrSymbol);
    }
    constexpr StringOrSymbolReference getString() const noexcept
    {
        return constexpr_assert(type == Type::String), valueStringOrSymbol;
    }
    constexpr StringOrSymbolReference getSymbol() const noexcept
    {
        return constexpr_assert(type == Type::Symbol), valueStringOrSymbol;
    }
    constexpr gc::InternalName getInternalName() const noexcept
    {
        return constexpr_assert(type == Type::InternalName), valueInternalName;
    }
    bool operator==(const Name &rt) const noexcept
    {
        if(type != rt.type)
            return false;
        switch(type)
        {
        case Type::Empty:
            return true;
        case Type::String:
        case Type::Symbol:
            return valueStringOrSymbol == rt.valueStringOrSymbol;
        case Type::InternalName:
            return valueInternalName == rt.valueInternalName;
        }
        constexpr_assert(false);
        return true;
    }
    bool operator!=(const Name &rt) const noexcept
    {
        return !operator==(rt);
    }
};
}
}

namespace std
{
template <>
struct hash<javascript_tasklets::gc::Name> final
{
    std::size_t operator()(javascript_tasklets::gc::Name value) const noexcept
    {
        using namespace javascript_tasklets::gc;
        switch(value.getType())
        {
        case Name::Type::Empty:
            return static_cast<std::size_t>(Name::Type::Empty);
        case Name::Type::String:
            return hash<StringOrSymbolReference>()(value.getString())
                   + static_cast<std::size_t>(Name::Type::String);
        case Name::Type::Symbol:
            return hash<StringOrSymbolReference>()(value.getSymbol())
                   + static_cast<std::size_t>(Name::Type::Symbol);
        case Name::Type::InternalName:
            return hash<InternalName>()(value.getInternalName())
                   + static_cast<std::size_t>(Name::Type::InternalName);
        }
        constexpr_assert(false);
        return 0;
    }
};
}

namespace javascript_tasklets
{
namespace gc
{
struct ObjectMemberDescriptor final
{
    enum class Kind : unsigned
    {
        DataInObject,
        DataInDescriptor,
        AccessorInDescriptor,
    };

private:
    bool configurable : 1;
    bool enumerable : 1;
    Kind kind;
    struct AccessorInDescriptorT final
    {
        ObjectReference getter;
        ObjectReference setter;
        constexpr AccessorInDescriptorT(ObjectReference getter, ObjectReference setter) noexcept
            : getter(getter),
              setter(setter)
        {
        }
        constexpr AccessorInDescriptorT() noexcept : getter(), setter()
        {
        }
    };
    struct Data
    {
        bool writable : 1;
        constexpr explicit Data(bool writable = false) noexcept : writable(writable)
        {
        }
    };
    struct DataInDescriptorT final : public Data
    {
        Value value;
        constexpr explicit DataInDescriptorT(Value value = Value::Undefined(),
                                             bool writable = false) noexcept : Data(writable),
                                                                               value(value)
        {
        }
    };
    struct DataInObjectT final : public Data
    {
        ObjectMemberIndex memberIndex;
        constexpr explicit DataInObjectT(ObjectMemberIndex memberIndex = ObjectMemberIndex(),
                                         bool writable = false) noexcept : Data(writable),
                                                                           memberIndex(memberIndex)
        {
        }
    };
    union
    {
        AccessorInDescriptorT accessorInDescriptor;
        DataInObjectT dataInObject;
        DataInDescriptorT dataInDescriptor;
    };
    constexpr ObjectMemberDescriptor(bool configurable,
                                     bool enumerable,
                                     AccessorInDescriptorT accessorInDescriptor) noexcept
        : configurable(configurable),
          enumerable(enumerable),
          kind(Kind::AccessorInDescriptor),
          accessorInDescriptor(accessorInDescriptor)
    {
    }
    constexpr ObjectMemberDescriptor(bool configurable,
                                     bool enumerable,
                                     DataInObjectT dataInObject) noexcept
        : configurable(configurable),
          enumerable(enumerable),
          kind(Kind::DataInObject),
          dataInObject(dataInObject)
    {
    }
    constexpr ObjectMemberDescriptor(bool configurable,
                                     bool enumerable,
                                     DataInDescriptorT dataInDescriptor) noexcept
        : configurable(configurable),
          enumerable(enumerable),
          kind(Kind::DataInDescriptor),
          dataInDescriptor(dataInDescriptor)
    {
    }

public:
    constexpr ObjectMemberDescriptor() noexcept : configurable(false),
                                                  enumerable(false),
                                                  kind(Kind::DataInObject),
                                                  dataInObject()
    {
    }
    constexpr Kind getKind() const noexcept
    {
        return kind;
    }
    static constexpr ObjectMemberDescriptor AccessorInDescriptor(bool configurable,
                                                                 bool enumerable,
                                                                 ObjectReference getter,
                                                                 ObjectReference setter) noexcept
    {
        return ObjectMemberDescriptor(
            configurable, enumerable, AccessorInDescriptorT(getter, setter));
    }
    static constexpr ObjectMemberDescriptor DataInObject(bool configurable,
                                                         bool enumerable,
                                                         ObjectMemberIndex memberIndex,
                                                         bool writable) noexcept
    {
        return ObjectMemberDescriptor(
            configurable, enumerable, DataInObjectT(memberIndex, writable));
    }
    static constexpr ObjectMemberDescriptor DataInDescriptor(bool configurable,
                                                             bool enumerable,
                                                             Value value,
                                                             bool writable) noexcept
    {
        return ObjectMemberDescriptor(configurable, enumerable, DataInDescriptorT(value, writable));
    }
    constexpr bool isAccessorInDescriptor() const noexcept
    {
        return kind == Kind::AccessorInDescriptor;
    }
    constexpr bool isAccessor() const noexcept
    {
        return kind == Kind::AccessorInDescriptor;
    }
    constexpr bool isDataInObject() const noexcept
    {
        return kind == Kind::DataInObject;
    }
    constexpr bool isEmbedded() const noexcept
    {
        return kind == Kind::DataInObject;
    }
    constexpr bool isDataInDescriptor() const noexcept
    {
        return kind == Kind::DataInDescriptor;
    }
    constexpr bool isData() const noexcept
    {
        return kind == Kind::DataInDescriptor || kind == Kind::DataInObject;
    }
    constexpr bool isConfigurable() const noexcept
    {
        return configurable;
    }
    void setConfigurable(bool value) noexcept
    {
        configurable = value;
    }
    constexpr bool isEnumerable() const noexcept
    {
        return enumerable;
    }
    void setEnumerable(bool value) noexcept
    {
        enumerable = value;
    }
    constexpr ObjectReference getGetter() const noexcept
    {
        return constexpr_assert(kind == Kind::AccessorInDescriptor), accessorInDescriptor.getter;
    }
    void setGetter(ObjectReference value) noexcept
    {
        constexpr_assert(kind == Kind::AccessorInDescriptor);
        accessorInDescriptor.getter = value;
    }
    constexpr ObjectReference getSetter() const noexcept
    {
        return constexpr_assert(kind == Kind::AccessorInDescriptor), accessorInDescriptor.setter;
    }
    void setSetter(ObjectReference value) noexcept
    {
        constexpr_assert(kind == Kind::AccessorInDescriptor);
        accessorInDescriptor.setter = value;
    }
    constexpr bool isWritable() const noexcept
    {
        return kind == Kind::DataInDescriptor ?
                   dataInDescriptor.writable :
                   (constexpr_assert(kind == Kind::DataInObject), dataInObject.writable);
    }
    void setWritable(bool value) noexcept
    {
        if(kind == Kind::DataInDescriptor)
        {
            dataInDescriptor.writable = value;
        }
        else
        {
            constexpr_assert(kind == Kind::DataInObject);
            dataInObject.writable = value;
        }
    }
    constexpr ObjectMemberIndex getMemberIndex() const noexcept
    {
        return constexpr_assert(kind == Kind::DataInObject), dataInObject.memberIndex;
    }
    void setMemberIndex(ObjectMemberIndex value) noexcept
    {
        constexpr_assert(kind == Kind::DataInObject);
        dataInObject.memberIndex = value;
    }
    constexpr Value getValueInDescriptor() const noexcept
    {
        return constexpr_assert(kind == Kind::DataInDescriptor), dataInDescriptor.value;
    }
    void setValueInDescriptor(Value value) noexcept
    {
        constexpr_assert(kind == Kind::DataInDescriptor);
        dataInDescriptor.value = value;
    }
};

struct ObjectMemberGroup final
{
    static constexpr std::size_t objectMembersPerGroup =
        sizeof(TypelessValue) / sizeof(Value::Type);
    Value::Type types[objectMembersPerGroup];
    TypelessValue values[objectMembersPerGroup];
    constexpr ObjectMemberGroup() noexcept : types(), values()
    {
        static_assert(Value::Type() == Value::Type::Undefined,
                      "Value::Type's default value is not Undefined");
    }
    constexpr Value get(std::size_t index) const noexcept
    {
        return constexpr_assert(index < objectMembersPerGroup), Value(types[index], values[index]);
    }
    void set(std::size_t index, Value value) noexcept
    {
        constexpr_assert(index < objectMembersPerGroup);
        types[index] = value.getType();
        values[index] = value.toTypelessValue();
    }
};

class GC;
struct ObjectDescriptor;

class Object final
{
    friend class GC;
    Object(const Object &rt) = delete;
    Object &operator=(const Object &rt) = delete;

public:
    struct ExtraData
    {
        virtual ~ExtraData() = default;
        ExtraData() = default;
        virtual std::unique_ptr<ExtraData> clone() const = 0;
    };

private:
    Object(const ObjectDescriptor *objectDescriptor,
           const GC *gc,
           std::unique_ptr<ExtraData> extraData)
        : objectDescriptor(objectDescriptor), gc(gc), extraData(std::move(extraData))
    {
    }
    ~Object() = default;

private:
    const ObjectDescriptor *objectDescriptor;

public:
    const GC *const gc;
    std::unique_ptr<ExtraData> extraData;
    ObjectMemberGroup *getMembersArray() noexcept;
    const ObjectMemberGroup *getMembersArray() const noexcept;
    Value::Type &getMemberType(ObjectMemberIndex memberIndex) noexcept;
    TypelessValue &getMemberTypelessValue(ObjectMemberIndex memberIndex) noexcept;
    Value::Type getMemberType(ObjectMemberIndex memberIndex) const noexcept;
    const TypelessValue &getMemberTypelessValue(ObjectMemberIndex memberIndex) const noexcept;
    Value getMemberValue(ObjectMemberIndex memberIndex) const noexcept
    {
        return Value(getMemberType(memberIndex), getMemberTypelessValue(memberIndex));
    }
    void setMemberValue(ObjectMemberIndex memberIndex, Value value) noexcept
    {
        getMemberType(memberIndex) = value.getType();
        getMemberTypelessValue(memberIndex) = value.toTypelessValue();
    }
    static std::size_t getSize(const ObjectDescriptor *objectDescriptor);
    static std::size_t getMemberGroupCount(const ObjectDescriptor *objectDescriptor);
};

inline ObjectMemberGroup *Object::getMembersArray() noexcept
{
    return reinterpret_cast<ObjectMemberGroup *>(this + 1);
}

inline const ObjectMemberGroup *Object::getMembersArray() const noexcept
{
    return reinterpret_cast<const ObjectMemberGroup *>(this + 1);
}

template <typename T = ObjectDescriptor>
struct ObjectDescriptorHandle;

template <typename Dest, typename Src>
constexpr ObjectDescriptorHandle<Dest> static_handle_cast(ObjectDescriptorHandle<Src>) noexcept;

template <typename Dest, typename Src>
ObjectDescriptorHandle<Dest> dynamic_handle_cast(ObjectDescriptorHandle<Src>) noexcept;

template <>
struct ObjectDescriptorHandle<ObjectDescriptor>
{
    template <typename Dest, typename Src>
    friend constexpr ObjectDescriptorHandle<Dest> static_handle_cast(
        ObjectDescriptorHandle<Src>) noexcept;
    template <typename Dest, typename Src>
    friend ObjectDescriptorHandle<Dest> dynamic_handle_cast(ObjectDescriptorHandle<Src>) noexcept;
    friend class GC;

private:
    const ObjectDescriptor *value;

protected:
    constexpr explicit ObjectDescriptorHandle(const ObjectDescriptor *objectDescriptor) noexcept
        : value(objectDescriptor)
    {
    }

public:
    ObjectDescriptorHandle(GC &gc, const ObjectDescriptor *objectDescriptor);
    constexpr ObjectDescriptorHandle(std::nullptr_t = nullptr) noexcept : value(nullptr)
    {
    }
    constexpr const ObjectDescriptor *get() const noexcept
    {
        return value;
    }
    constexpr const ObjectDescriptor *getBase() const noexcept
    {
        return value;
    }
    constexpr bool empty() const noexcept
    {
        return value == nullptr;
    }
};

template <typename Dest, typename Src>
constexpr ObjectDescriptorHandle<Dest> static_handle_cast(ObjectDescriptorHandle<Src>) noexcept;

template <typename Dest, typename Src>
ObjectDescriptorHandle<Dest> dynamic_handle_cast(ObjectDescriptorHandle<Src>) noexcept;

#error finish

struct ObjectDescriptor
{
    friend class GC;
    class ObjectDescriptorInitializer final
    {
        friend class GC;
        friend struct ObjectDescriptor;
        ObjectDescriptorInitializer(const ObjectDescriptorInitializer &) = delete;
        ObjectDescriptorInitializer &operator=(const ObjectDescriptor &) = delete;

    public:
        const GC *const gc;
        const ObjectDescriptor *const parent;
        const std::size_t index;

    private:
        ObjectDescriptorInitializer(const GC *gc, const ObjectDescriptor *parent, std::size_t index)
            : gc(gc), parent(parent), index(index)
        {
        }
    };
    struct Member final
    {
        Name name;
        ObjectMemberDescriptor descriptor;
        constexpr Member() noexcept : name(), descriptor()
        {
        }
        constexpr Member(Name name, ObjectMemberDescriptor descriptor) noexcept
            : name(name),
              descriptor(descriptor)
        {
        }
        constexpr bool empty() const noexcept
        {
            return name.empty();
        }
    };
    const GC *const gc;
    const ObjectDescriptor *const parent;
    const std::size_t index;

private:
    std::size_t embeddedMemberCount;
    std::vector<Member> members;

public:
    explicit ObjectDescriptor(const ObjectDescriptorInitializer &initializer);
    virtual ~ObjectDescriptor() = default;
    std::size_t getEmbeddedMemberCount() const
    {
        return embeddedMemberCount;
    }
    const std::vector<Member> &getMembers() const
    {
        return members;
    }
    const Member &getMember(std::size_t index) const noexcept
    {
        constexpr_assert(index < members.size());
        return members[index];
    }
    void setMember(std::size_t index, const Member &member) noexcept
    {
        constexpr_assert(index < members.size());
        constexpr_assert(!member.empty());
        if(members[index].descriptor.isEmbedded())
            embeddedMemberCount--;
        if(member.descriptor.isEmbedded())
            embeddedMemberCount++;
        members[index] = member;
    }
    std::size_t addMember(const Member &member)
    {
        constexpr_assert(!member.empty());
        std::size_t retval = members.size();
        members.push_back(member);
        if(members.back().descriptor.isEmbedded())
        {
            members.back().descriptor.setMemberIndex(ObjectMemberIndex(embeddedMemberCount));
            embeddedMemberCount++;
        }
        return retval;
    }
    static constexpr std::size_t npos = ~static_cast<std::size_t>(0);
    std::size_t findMember(const Name &name) const noexcept
    {
        constexpr_assert(!name.empty());
        for(std::size_t index = 0; index < members.size(); index++)
        {
            if(members[index].name == name)
                return index;
        }
        return npos;
    }
    virtual ObjectDescriptorHandle<> duplicate(ObjectDescriptorHandle<> self, GC &gc) const;
};

template <typename T>
struct ObjectDescriptorHandle final : public ObjectDescriptorHandle<ObjectDescriptor>
{
    template <typename Dest, typename Src>
    friend constexpr ObjectDescriptorHandle<Dest> static_handle_cast(
        ObjectDescriptorHandle<Src>) noexcept;
    template <typename Dest, typename Src>
    friend ObjectDescriptorHandle<Dest> dynamic_handle_cast(ObjectDescriptorHandle<Src>) noexcept;

private:
    constexpr explicit ObjectDescriptorHandle(const T *objectDescriptor)
        : ObjectDescriptorHandle<ObjectDescriptor>(objectDescriptor)
    {
    }

public:
    static_assert(std::is_base_of<ObjectDescriptor, T>::value,
                  "ObjectDescriptor must be a base class of T");
    constexpr ObjectDescriptorHandle() noexcept = default;
    ObjectDescriptorHandle(GC &gc, const T *objectDescriptor)
        : ObjectDescriptorHandle<ObjectDescriptor>(gc, objectDescriptor)
    {
    }
    template <typename U>
    constexpr ObjectDescriptorHandle(const ObjectDescriptorHandle<U> &rt) noexcept
        : ObjectDescriptorHandle<ObjectDescriptor>(rt)
    {
        static_assert(std::is_base_of<T, U>::value, "can't down-convert from U to T");
    }
    const T *get() const noexcept
    {
        constexpr_assert(dynamic_cast<T *>(getBase()) == getBase());
        return static_cast<T *>(getBase());
    }
};

template <typename Dest, typename Src>
constexpr ObjectDescriptorHandle<Dest> static_handle_cast(
    ObjectDescriptorHandle<Src> source) noexcept
{
    return ObjectDescriptorHandle<Dest>(static_cast<const Dest *>(source.get()));
}

template <typename Dest, typename Src>
ObjectDescriptorHandle<Dest> dynamic_handle_cast(ObjectDescriptorHandle<Src> source) noexcept
{
    return ObjectDescriptorHandle<Dest>(dynamic_cast<const Dest *>(source.get()));
}

inline std::size_t Object::getSize(const ObjectDescriptor *objectDescriptor)
{
    return sizeof(Object) + sizeof(ObjectMemberGroup) * getMemberGroupCount(objectDescriptor);
}

inline std::size_t Object::getMemberGroupCount(const ObjectDescriptor *objectDescriptor)
{
    constexpr_assert(objectDescriptor);
    return (objectDescriptor->getEmbeddedMemberCount() + ObjectMemberGroup::objectMembersPerGroup
            - 1) / ObjectMemberGroup::objectMembersPerGroup;
}

inline Value::Type &Object::getMemberType(ObjectMemberIndex memberIndex) noexcept
{
    constexpr_assert(memberIndex.index < objectDescriptor->getEmbeddedMemberCount());
    return getMembersArray()[memberIndex.index / ObjectMemberGroup::objectMembersPerGroup]
        .types[memberIndex.index % ObjectMemberGroup::objectMembersPerGroup];
}

inline TypelessValue &Object::getMemberTypelessValue(ObjectMemberIndex memberIndex) noexcept
{
    constexpr_assert(memberIndex.index < objectDescriptor->getEmbeddedMemberCount());
    return getMembersArray()[memberIndex.index / ObjectMemberGroup::objectMembersPerGroup]
        .values[memberIndex.index % ObjectMemberGroup::objectMembersPerGroup];
}

inline Value::Type Object::getMemberType(ObjectMemberIndex memberIndex) const noexcept
{
    constexpr_assert(memberIndex.index < objectDescriptor->getEmbeddedMemberCount());
    return getMembersArray()[memberIndex.index / ObjectMemberGroup::objectMembersPerGroup]
        .types[memberIndex.index % ObjectMemberGroup::objectMembersPerGroup];
}

inline const TypelessValue &Object::getMemberTypelessValue(ObjectMemberIndex memberIndex) const
    noexcept
{
    constexpr_assert(memberIndex.index < objectDescriptor->getEmbeddedMemberCount());
    return getMembersArray()[memberIndex.index / ObjectMemberGroup::objectMembersPerGroup]
        .values[memberIndex.index % ObjectMemberGroup::objectMembersPerGroup];
}

struct ValueHandle;

struct ObjectHandle final
{
    friend struct ValueHandle;
    ObjectReference object;
    inline ObjectHandle(GC &gc, ObjectReference object);
    constexpr ObjectHandle() noexcept : object()
    {
    }

private:
    constexpr ObjectHandle(ObjectReference object) noexcept : object(object)
    {
    }

public:
    constexpr bool empty() const noexcept
    {
        return object == ObjectReference();
    }
};

struct StringHandle final
{
    friend struct ValueHandle;
    StringOrSymbolReference string;
    inline StringHandle(GC &gc, StringOrSymbolReference string);
    constexpr StringHandle() noexcept : string(nullptr)
    {
    }

private:
    constexpr StringHandle(StringOrSymbolReference string) noexcept : string(string)
    {
    }

public:
    constexpr bool empty() const noexcept
    {
        return string == nullptr;
    }
};

struct SymbolHandle final
{
    friend struct ValueHandle;
    friend struct NameHandle;
    StringOrSymbolReference symbol;
    inline SymbolHandle(GC &gc, StringOrSymbolReference symbol);
    constexpr SymbolHandle() noexcept : symbol(nullptr)
    {
    }

private:
    constexpr SymbolHandle(StringOrSymbolReference symbol) noexcept : symbol(symbol)
    {
    }

public:
    constexpr bool empty() const noexcept
    {
        return symbol == nullptr;
    }
};

struct NameHandle final
{
    friend struct ValueHandle;
    Name value;
    inline NameHandle(GC &gc, Name value);
    constexpr NameHandle() noexcept : value()
    {
    }
    constexpr NameHandle(StringHandle value) noexcept : NameHandle(String(value))
    {
    }
    constexpr NameHandle(SymbolHandle value) noexcept : NameHandle(Symbol(value))
    {
    }

private:
    constexpr NameHandle(Name value) noexcept : value(Name::Empty())
    {
    }

public:
    constexpr bool empty() const noexcept
    {
        return value.empty();
    }
    constexpr Name::Type getType() const noexcept
    {
        return value.getType();
    }
    static constexpr NameHandle Symbol(SymbolHandle symbol) noexcept
    {
        return NameHandle(Value::Symbol(symbol.symbol));
    }
    constexpr bool isSymbol() const noexcept
    {
        return value.isSymbol();
    }
    constexpr NameHandle getSymbol() const noexcept
    {
        return SymbolHandle(value.getSymbol());
    }
    constexpr SymbolHandle getIfSymbol() const noexcept
    {
        return isSymbol() ? getSymbol() : SymbolHandle();
    }
    static NameHandle Symbol(GC &gc, StringOrSymbolReference symbol)
    {
        return Symbol(SymbolHandle(gc, symbol));
    }
    static constexpr NameHandle String(StringHandle string) noexcept
    {
        return NameHandle(Value::String(string.string));
    }
    constexpr bool isString() const noexcept
    {
        return value.isString();
    }
    constexpr StringHandle getString() const noexcept
    {
        return StringHandle(value.getString());
    }
    constexpr StringHandle getIfString() const noexcept
    {
        return isString() ? getString() : StringHandle();
    }
    static NameHandle String(GC &gc, StringOrSymbolReference string)
    {
        return String(StringHandle(gc, string));
    }
    static constexpr NameHandle InternalName(gc::InternalName value) noexcept
    {
        return NameHandle(Name::InternalName(value));
    }
    constexpr bool isInternalName() const noexcept
    {
        return value.isInternalName();
    }
    constexpr gc::InternalName getInternalName() const noexcept
    {
        return value.getInternalName();
    }
};

struct ValueHandle final
{
    Value value;
    constexpr ValueHandle() : value()
    {
    }
    inline ValueHandle(GC &gc, const Value &value);
    constexpr ValueHandle(StringHandle value) noexcept : ValueHandle(String(value))
    {
    }
    constexpr ValueHandle(ObjectHandle value) noexcept : ValueHandle(Object(value))
    {
    }
    constexpr ValueHandle(SymbolHandle value) noexcept : ValueHandle(Symbol(value))
    {
    }
    constexpr explicit ValueHandle(NameHandle value) noexcept : value(value.value.toValue())
    {
    }
    constexpr operator Value() const noexcept
    {
        return value;
    }

private:
    constexpr ValueHandle(const Value &value) noexcept : value(value)
    {
    }

public:
    constexpr Value::Type getType() const noexcept
    {
        return value.getType();
    }
    static constexpr ValueHandle Undefined() noexcept
    {
        return ValueHandle(Value::Undefined());
    }
    constexpr bool isUndefined() const noexcept
    {
        return value.isUndefined();
    }
    static constexpr ValueHandle Null() noexcept
    {
        return ValueHandle(Value::Null());
    }
    constexpr bool isNull() const noexcept
    {
        return value.isNull();
    }
    static constexpr ValueHandle Object(ObjectHandle object) noexcept
    {
        return ValueHandle(Value::Object(object.object));
    }
    static constexpr ValueHandle ObjectOrNull(ObjectHandle object) noexcept
    {
        return object.empty() ? Null() : Object(object);
    }
    constexpr bool isObject() const noexcept
    {
        return value.isObject();
    }
    constexpr ObjectHandle getObject() const noexcept
    {
        return ObjectHandle(value.getObject());
    }
    constexpr ObjectHandle getIfObject() const noexcept
    {
        return isObject() ? getObject() : ObjectHandle();
    }
    static ValueHandle Object(GC &gc, ObjectReference object)
    {
        return Object(ObjectHandle(gc, object));
    }
    static constexpr ValueHandle Double(double doubleValue) noexcept
    {
        return ValueHandle(Value::Double(doubleValue));
    }
    constexpr bool isDouble() const noexcept
    {
        return value.isDouble();
    }
    constexpr double getDouble() const noexcept
    {
        return value.getDouble();
    }
    static constexpr ValueHandle Int32(std::int32_t intValue) noexcept
    {
        return ValueHandle(Value::Int32(intValue));
    }
    constexpr bool isInt32() const noexcept
    {
        return value.isInt32();
    }
    constexpr std::int32_t getInt32() const noexcept
    {
        return value.getInt32();
    }
    static constexpr ValueHandle UInt32(std::uint32_t intValue) noexcept
    {
        return ValueHandle(Value::UInt32(intValue));
    }
    constexpr bool isUInt32() const noexcept
    {
        return value.isUInt32();
    }
    constexpr std::uint32_t getUInt32() const noexcept
    {
        return value.getUInt32();
    }
    static constexpr ValueHandle Boolean(bool boolValue) noexcept
    {
        return ValueHandle(Value::Boolean(boolValue));
    }
    constexpr bool isBoolean() const noexcept
    {
        return value.isBoolean();
    }
    constexpr bool getBoolean() const noexcept
    {
        return value.getBoolean();
    }
    static constexpr ValueHandle Symbol(SymbolHandle symbol) noexcept
    {
        return ValueHandle(Value::Symbol(symbol.symbol));
    }
    constexpr bool isSymbol() const noexcept
    {
        return value.isSymbol();
    }
    constexpr SymbolHandle getSymbol() const noexcept
    {
        return SymbolHandle(value.getSymbol());
    }
    constexpr SymbolHandle getIfSymbol() const noexcept
    {
        return isSymbol() ? getSymbol() : SymbolHandle();
    }
    static ValueHandle Symbol(GC &gc, StringOrSymbolReference symbol)
    {
        return Symbol(SymbolHandle(gc, symbol));
    }
    static constexpr ValueHandle String(StringHandle string) noexcept
    {
        return ValueHandle(Value::String(string.string));
    }
    constexpr bool isString() const noexcept
    {
        return value.isString();
    }
    constexpr StringHandle getString() const noexcept
    {
        return StringHandle(value.getString());
    }
    constexpr StringHandle getIfString() const noexcept
    {
        return isString() ? getString() : StringHandle();
    }
    static ValueHandle String(GC &gc, StringOrSymbolReference string)
    {
        return String(StringHandle(gc, string));
    }
};

class HandleScope final
{
    friend class GC;
    friend struct ObjectHandle;
    friend struct StringHandle;
    friend struct SymbolHandle;
    friend struct ValueHandle;
    friend struct ObjectDescriptorHandle<>;
    HandleScope(const HandleScope &rt) = delete;
    HandleScope &operator=(const HandleScope &rt) = delete;
    void *operator new(std::size_t) = delete;

private:
    HandleScope *parent = nullptr;
    GC &gc;
    ObjectReference *objectReferences = nullptr;
    StringOrSymbolReference *stringOrSymbolReferences = nullptr;
    const ObjectDescriptor **objectDescriptors = nullptr;
    static constexpr std::size_t embeddedHandleCount = 7, initialDynamicAllocationSize = 32;
    std::size_t objectReferenceCount = 0;
    std::size_t stringOrSymbolReferenceCount = 0;
    std::size_t objectDescriptorCount = 0;
    std::size_t objectReferencesAllocated = embeddedHandleCount;
    std::size_t stringOrSymbolReferencesAllocated = embeddedHandleCount;
    std::size_t objectDescriptorsAllocated = embeddedHandleCount;
    ObjectReference embeddedObjectReferences[embeddedHandleCount];
    StringOrSymbolReference embeddedStringOrSymbolReferences[embeddedHandleCount];
    const ObjectDescriptor *embeddedObjectDescriptors[embeddedHandleCount];
    inline void addToGC();
    inline void removeFromGC();
    void init()
    {
        addToGC();
    }
    template <typename T>
    static void expandArrayImp(std::size_t &allocated, T *&array, std::size_t count);
    void expandObjectReferences();
    void expandStringOrSymbolReferences();
    void expandObjectDescriptors();
    void addReference(ObjectReference reference)
    {
        constexpr_assert(reference != nullptr);
        if(objectReferenceCount < embeddedHandleCount)
        {
            embeddedObjectReferences[objectReferenceCount++] = reference;
        }
        else
        {
            if(objectReferenceCount >= objectReferencesAllocated)
                expandObjectReferences();
            objectReferences[objectReferenceCount++ - embeddedHandleCount] = reference;
        }
    }
    void addReference(StringOrSymbolReference reference)
    {
        constexpr_assert(reference != nullptr);
        if(stringOrSymbolReferenceCount < embeddedHandleCount)
        {
            embeddedStringOrSymbolReferences[stringOrSymbolReferenceCount++] = reference;
        }
        else
        {
            if(stringOrSymbolReferenceCount >= stringOrSymbolReferencesAllocated)
                expandStringOrSymbolReferences();
            stringOrSymbolReferences[stringOrSymbolReferenceCount++ - embeddedHandleCount] =
                reference;
        }
    }
    void addReference(const ObjectDescriptor *reference)
    {
        constexpr_assert(reference != nullptr);
        if(objectDescriptorCount < embeddedHandleCount)
        {
            embeddedObjectDescriptors[objectDescriptorCount++] = reference;
        }
        else
        {
            if(objectDescriptorCount >= objectDescriptorsAllocated)
                expandObjectDescriptors();
            objectDescriptors[objectDescriptorCount++ - embeddedHandleCount] = reference;
        }
    }
    void addHandle(const ObjectHandle &object)
    {
        if(object.empty())
            return;
        addReference(object.object);
    }
    void addHandle(const StringHandle &string)
    {
        if(string.empty())
            return;
        addReference(string.string);
    }
    void addHandle(const SymbolHandle &symbol)
    {
        if(symbol.empty())
            return;
        addReference(symbol.symbol);
    }
    inline void addHandle(const ObjectDescriptorHandle<> &objectDescriptor);
    void addHandle(const ValueHandle &value)
    {
        switch(value.getType())
        {
        case Value::Type::Object:
            addReference(value.getObject().object);
            return;
        case Value::Type::Double:
        case Value::Type::Int32:
        case Value::Type::UInt32:
        case Value::Type::Boolean:
        case Value::Type::Undefined:
        case Value::Type::Null:
            return;
        case Value::Type::Symbol:
            addReference(value.getSymbol().symbol);
            return;
        case Value::Type::String:
            addReference(value.getString().string);
            return;
        }
        constexpr_assert(false);
    }

public:
    explicit HandleScope(GC &gc)
        : gc(gc),
          embeddedObjectReferences(),
          embeddedStringOrSymbolReferences(),
          embeddedObjectDescriptors()
    {
        init();
    }
    ~HandleScope()
    {
        removeFromGC();
        delete[] objectReferences;
        delete[] stringOrSymbolReferences;
        delete[] objectDescriptors;
    }
    ObjectHandle escapeHandle(const ObjectHandle &handle)
    {
        constexpr_assert(parent);
        parent->addHandle(handle);
        return handle;
    }
    SymbolHandle escapeHandle(const SymbolHandle &handle)
    {
        constexpr_assert(parent);
        parent->addHandle(handle);
        return handle;
    }
    StringHandle escapeHandle(const StringHandle &handle)
    {
        constexpr_assert(parent);
        parent->addHandle(handle);
        return handle;
    }
    ValueHandle escapeHandle(const ValueHandle &handle)
    {
        constexpr_assert(parent);
        parent->addHandle(handle);
        return handle;
    }
    template <typename T>
    ObjectDescriptorHandle<T> escapeHandle(const ObjectDescriptorHandle<T> &handle)
    {
        constexpr_assert(parent);
        parent->addHandle(handle);
        return handle;
    }
};

class GC final : public std::enable_shared_from_this<GC>
{
    friend class HandleScope;
    friend struct ObjectHandle;
    friend struct StringHandle;
    friend struct SymbolHandle;
    friend struct ValueHandle;
    friend struct ObjectDescriptorHandle<>;
    GC &operator=(const GC &) = delete;
    GC(const GC &) = delete;

private:
    std::vector<Object *> objects, oldObjects;
    std::vector<std::size_t> freeObjectIndexesList;
    std::vector<ObjectReference> objectsWorklist;
    std::vector<String *> strings;
    std::vector<String *> oldStrings;
    std::vector<std::size_t> freeStringsIndexesList;
    std::vector<ObjectDescriptor *> objectDescriptors, oldObjectDescriptors;
    std::vector<std::size_t> freeObjectDescriptorIndexesList;
    std::vector<std::size_t> objectDescriptorsWorklist;
    bool immutable;
    const std::shared_ptr<const GC> parent;
    HandleScope *handleScopesStack;
    const std::size_t startingAllocationsLeftTillNextCollect;
    const std::size_t startingMemoryLeftTillNextCollect;
    std::size_t allocationsLeftTillNextCollect;
    std::size_t memoryLeftTillNextCollect;
    struct ObjectDescriptorStringOrSymbolTransition final
    {
        StringOrSymbolReference stringOrSymbol;
        const ObjectDescriptor *sourceDescriptor;
        const ObjectDescriptor *
            destDescriptor; // destDescriptor is ignored for hash and equality comparison
        constexpr ObjectDescriptorStringOrSymbolTransition(
            StringOrSymbolReference stringOrSymbol,
            const ObjectDescriptor *sourceDescriptor,
            const ObjectDescriptor *destDescriptor = nullptr) noexcept
            : stringOrSymbol(stringOrSymbol),
              sourceDescriptor(sourceDescriptor),
              destDescriptor(destDescriptor)
        {
        }
        std::size_t hash() const noexcept
        {
            return std::hash<StringOrSymbolReference>()(stringOrSymbol)
                   + 3923 * std::hash<const ObjectDescriptor *>()(sourceDescriptor);
        }
        bool operator==(const ObjectDescriptorStringOrSymbolTransition &rt) const noexcept
        {
            return stringOrSymbol == rt.stringOrSymbol && sourceDescriptor == rt.sourceDescriptor;
        }
        bool operator!=(const ObjectDescriptorStringOrSymbolTransition &rt) const noexcept
        {
            return !operator==(rt);
        }
    };
    std::vector<std::forward_list<ObjectDescriptorStringOrSymbolTransition>>
        objectDescriptorSymbolTransitions;
    std::vector<std::forward_list<ObjectDescriptorStringOrSymbolTransition>>
        objectDescriptorStringTransitions;
    std::unordered_multimap<std::size_t, StringOrSymbolReference> stringHashToStringReferenceMap;
    std::unordered_map<const void *, Value> globalValuesMap;

private:
    ObjectReference allocateObjectIndex();
    void freeObjectIndex(ObjectReference object) noexcept;
    std::size_t allocateStringIndex();
    void freeStringIndex(std::size_t stringIndex) noexcept;
    std::size_t allocateObjectDescriptorIndex();
    void freeObjectDescriptorIndex(std::size_t objectDescriptorIndex) noexcept;
    Object *objectCopyOnWrite(ObjectReference objectReference);
    Object *createInternal(const ObjectDescriptor *objectDescriptor,
                           std::unique_ptr<Object::ExtraData> extraData);
    void freeInternal(Object *object) noexcept;
    void collectorMarkObject(ObjectReference object) noexcept;
    void collectorMarkStringOrSymbol(StringOrSymbolReference stringOrSymbol) noexcept;
    void collectorMarkObjectDescriptor(const ObjectDescriptor *objectDescriptor) noexcept;
    void collectorMarkObjectMember(Value::Type type, const TypelessValue &value) noexcept;
    void collectorMarkName(const Name &value) noexcept;
    void collectorMarkValue(const Value &value) noexcept;
    void collectorMarkObjectDescriptorMember(const ObjectDescriptor::Member &value) noexcept;
    void collectorMarkObjectMemberDescriptor(const ObjectMemberDescriptor &descriptor) noexcept;
    void collectorScanObject(ObjectReference objectReference) noexcept;
    void collectorScanObjectDescriptor(ObjectDescriptor *objectDescriptor) noexcept;
    StringHandle internStringHelper(const String &value, std::size_t valueHash) noexcept;
    ObjectDescriptorStringOrSymbolTransition &findOrAddObjectDescriptorStringOrSymbolTransition(
        std::vector<std::forward_list<ObjectDescriptorStringOrSymbolTransition>> &transitions,
        StringOrSymbolReference stringOrSymbol,
        const ObjectDescriptor *sourceDescriptor);
    template <typename Tag>
    static const void *getGlobalValueMapKey() noexcept
    {
        static char key;
        return &key;
    }
    ValueHandle getGlobalValue(const void *key);
    void setGlobalValue(const void *key, ValueHandle);

public:
    explicit GC(std::shared_ptr<const GC> parent = nullptr);
    ~GC();
    void collect() noexcept;
    bool isImmutable() const
    {
        return immutable;
    }
    void makeImmutable() noexcept
    {
        if(!immutable)
        {
            collect();
            immutable = true;
        }
    }
    const Object &readObject(ObjectHandle handle) const noexcept
    {
        constexpr_assert(handle.object.index < objects.size());
        const Object *retval = objects[handle.object.index];
        constexpr_assert(retval);
        return *retval;
    }
    Object &writeObject(ObjectHandle handle)
    {
        constexpr_assert(!handle.empty());
        constexpr_assert(!immutable);
        constexpr_assert(handle.object.index < objects.size());
        Object *&retval = objects[handle.object.index];
        constexpr_assert(retval);
        if(retval->gc == this)
            return *retval;
        return *objectCopyOnWrite(handle.object);
    }
    ObjectDescriptorHandle<> getObjectDescriptor(ObjectHandle handle) const noexcept
    {
        return ObjectDescriptorHandle<>(readObject(handle).objectDescriptor);
    }
    ObjectDescriptor::Member addObjectMember(NameHandle nameHandle,
                                             ObjectHandle object,
                                             ValueHandle value,
                                             bool isEmbedded = true);
    ObjectDescriptor::Member addObjectMember(SymbolHandle nameHandle,
                                             ObjectHandle object,
                                             ValueHandle value,
                                             bool isEmbedded = true);
    const String &readString(StringHandle handle) const noexcept
    {
        constexpr_assert(handle.string.index < strings.size());
        const String *retval = strings[handle.string.index];
        constexpr_assert(retval);
        return *retval;
    }
    const String &readSymbol(SymbolHandle handle) const noexcept
    {
        constexpr_assert(handle.symbol.index < strings.size());
        const String *retval = strings[handle.symbol.index];
        constexpr_assert(retval);
        return *retval;
    }
    StringHandle internString(const String &value);
    StringHandle internString(String &&value);
    SymbolHandle createSymbol(String description);
    ObjectHandle create(ObjectDescriptorHandle<> objectDescriptor,
                        std::unique_ptr<Object::ExtraData> extraData = nullptr);
    template <typename T = ObjectDescriptor, typename... Args>
    ObjectDescriptorHandle<T> createObjectDescriptor(
        ObjectDescriptorHandle<T> parentObjectDescriptor, Args &&... args)
    {
        static_assert(std::is_base_of<ObjectDescriptor, T>::value,
                      "T is not derived from ObjectDescriptor");
        constexpr_assert(parentObjectDescriptor.empty()
                         || typeid(*parentObjectDescriptor.get()) == typeid(T));
        const ObjectDescriptor::ObjectDescriptorInitializer initializer(
            this, parentObjectDescriptor.get(), allocateObjectDescriptorIndex());
        T *retval;
        try
        {
            retval = new T(
                static_cast<const ObjectDescriptor::ObjectDescriptorInitializer &>(initializer),
                std::forward<Args>(args)...);
        }
        catch(...)
        {
            freeObjectDescriptorIndex(initializer.index);
            throw;
        }
        objectDescriptors[initializer.index] = retval;
        return ObjectDescriptorHandle<T>(*this, retval);
    }
    ObjectDescriptorHandle<> createObjectDescriptor()
    {
        return createObjectDescriptor(ObjectDescriptorHandle<>());
    }
    template <typename Tag>
    ValueHandle getGlobalValue()
    {
        return getGlobalValue(getGlobalValueMapKey<Tag>());
    }
    template <typename Tag>
    void setGlobalValue(ValueHandle value)
    {
        setGlobalValue(getGlobalValueMapKey<Tag>(), value);
    }
};

inline ObjectHandle::ObjectHandle(GC &gc, ObjectReference object) : object(object)
{
    constexpr_assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}

inline StringHandle::StringHandle(GC &gc, StringOrSymbolReference string) : string(string)
{
    constexpr_assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}

inline SymbolHandle::SymbolHandle(GC &gc, StringOrSymbolReference symbol) : symbol(symbol)
{
    constexpr_assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}

inline ValueHandle::ValueHandle(GC &gc, const Value &value) : value(value)
{
    constexpr_assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}

inline void HandleScope::addToGC()
{
    constexpr_assert(!parent);
    parent = gc.handleScopesStack;
    gc.handleScopesStack = this;
}

inline void HandleScope::removeFromGC()
{
    constexpr_assert(this == gc.handleScopesStack);
    gc.handleScopesStack = parent;
    parent = nullptr;
}

inline void HandleScope::addHandle(const ObjectDescriptorHandle<> &objectDescriptor)
{
    if(!objectDescriptor.empty() && objectDescriptor.get()->gc == &gc)
    {
        addReference(objectDescriptor.get());
    }
}

inline ObjectDescriptorHandle<ObjectDescriptor>::ObjectDescriptorHandle(
    GC &gc, const ObjectDescriptor *objectDescriptor)
    : value(objectDescriptor)
{
    constexpr_assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}
}
}

#endif /* JAVASCRIPT_TASKLETS_GC_H_ */
