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

#include <cassert>
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
struct ObjectDescriptor;

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
struct hash<javascript_tasklets::gc::StringOrSymbolReference> final
    : public hash<javascript_tasklets::gc::BaseReference>
{
};
}

namespace javascript_tasklets
{
namespace gc
{
union ObjectMember final
{
    ObjectReference objectValue;
    double doubleValue;
    std::uint32_t uint32Value;
    StringOrSymbolReference stringOrSymbolValue;
    constexpr ObjectMember() noexcept
        : stringOrSymbolValue(nullptr) // initialize stringOrSymbolValue because it is used by
    // Undefined, which is the default type
    {
    }
    typedef std::uint8_t TypeBaseType;
    enum class Type : TypeBaseType
    {
        Undefined = 0, // default value; stringOrSymbolValue is empty
        Object, // uses objectValue
        Double, // uses doubleValue
        Int32, // uses uint32Value
        UInt32, // uses uint32Value
        Boolean, // uses uint32Value
        Symbol, // uses stringOrSymbolValue
        String = Undefined, // uses stringOrSymbolValue
        Null = Symbol, // stringOrSymbolValue is empty
    };
    constexpr bool isObject(Type type) const noexcept
    {
        return type == Type::Object;
    }
    constexpr bool isDouble(Type type) const noexcept
    {
        return type == Type::Double;
    }
    constexpr bool isInt32(Type type) const noexcept
    {
        return type == Type::Int32;
    }
    constexpr bool isUInt32(Type type) const noexcept
    {
        return type == Type::UInt32;
    }
    constexpr bool isBoolean(Type type) const noexcept
    {
        return type == Type::Boolean;
    }
    constexpr bool isSymbol(Type type) const noexcept
    {
        return type == Type::Symbol && stringOrSymbolValue != nullptr;
    }
    constexpr bool isString(Type type) const noexcept
    {
        return type == Type::String && stringOrSymbolValue != nullptr;
    }
    constexpr bool isUndefined(Type type) const noexcept
    {
        return type == Type::Undefined && stringOrSymbolValue == nullptr;
    }
    constexpr bool isNull(Type type) const noexcept
    {
        return type == Type::Null && stringOrSymbolValue == nullptr;
    }
};

struct Value final
{
    ObjectMember value;
    ObjectMember::Type type;
    constexpr Value(ObjectMember::Type type, ObjectMember value) noexcept : value(value), type(type)
    {
    }
    Value() noexcept : value(), type(ObjectMember::Type::Undefined)
    {
        value.stringOrSymbolValue = nullptr;
    }
};

struct ObjectMemberGroup final
{
    static constexpr std::size_t objectMembersPerGroup =
        sizeof(ObjectMember) / sizeof(ObjectMember::Type);
    ObjectMember::Type types[objectMembersPerGroup];
    ObjectMember values[objectMembersPerGroup];
    ObjectMemberGroup() noexcept // initialize everything to Undefined
    {
        for(ObjectMember::Type &type : types)
        {
            type = ObjectMember::Type::Undefined;
        }
        for(ObjectMember &value : values)
        {
            value.stringOrSymbolValue = nullptr;
        }
    }
};

class GC;

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
    ObjectMemberGroup *getMembersArray();
    const ObjectMemberGroup *getMembersArray() const;
    ObjectMember::Type &getMemberType(std::size_t memberIndex);
    ObjectMember &getMemberValue(std::size_t memberIndex);
    ObjectMember::Type getMemberType(std::size_t memberIndex) const;
    const ObjectMember &getMemberValue(std::size_t memberIndex) const;
    static std::size_t getSize(const ObjectDescriptor *objectDescriptor);
    static std::size_t getMemberGroupCount(const ObjectDescriptor *objectDescriptor);
};

inline ObjectMemberGroup *Object::getMembersArray()
{
    return reinterpret_cast<ObjectMemberGroup *>(this + 1);
}

inline const ObjectMemberGroup *Object::getMembersArray() const
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
    constexpr ObjectDescriptorHandle() noexcept : value(nullptr)
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
        enum class Kind
        {
            Empty,
            Embedded,
            Constant,
        };
        union MemberValue final
        {
            std::size_t embeddedMember;
            Value constantMember;
            constexpr MemberValue() : embeddedMember()
            {
            }
            constexpr MemberValue(std::size_t embeddedMember) : embeddedMember(embeddedMember)
            {
            }
            constexpr MemberValue(Value constantMember) : constantMember(constantMember)
            {
            }
        };
        Kind kind;
        MemberValue value;
        constexpr Member() : kind(Kind::Empty), value()
        {
        }

    private:
        constexpr Member(Kind kind, MemberValue value) : kind(kind), value(value)
        {
        }

    public:
        static constexpr Member makeEmbedded(std::size_t embeddedMember)
        {
            return Member(Kind::Embedded, MemberValue(embeddedMember));
        }
        static constexpr Member makeConstant(Value constantMember)
        {
            return Member(Kind::Constant, Value(constantMember));
        }
    };
    struct InternalSlot final
    {
        const char *name;
        template <typename TagType>
        const InternalSlot *get()
        {
            static InternalSlot retval(TagType::name);
            return &retval;
        }
    };
    const GC *const gc;
    const ObjectDescriptor *const parent;
    const std::size_t index;

private:
    std::size_t embeddedMemberCount;
    std::unordered_map<String, Member> namedMembers;
    std::unordered_map<StringOrSymbolReference, Member> symbolMembers;
    std::unordered_map<const InternalSlot *, Member> internalSlots;

public:
    explicit ObjectDescriptor(const ObjectDescriptorInitializer &initializer);
    virtual ~ObjectDescriptor() = default;
    std::size_t getEmbeddedMemberCount() const
    {
        return embeddedMemberCount;
    }
    Member getInternalSlot(const InternalSlot *slot) const
    {
        assert(slot);
        auto iter = internalSlots.find(slot);
        if(iter == internalSlots.end())
            return Member();
        return std::get<1>(*iter);
    }
    void setInternalSlot(const InternalSlot *slot, Member member)
    {
        assert(slot);
        internalSlots[slot] = member;
    }
    virtual Member getNamedMember(const String &name) const
    {
        auto iter = namedMembers.find(name);
        if(iter == namedMembers.end())
            return Member();
        return std::get<1>(*iter);
    }
    virtual Member getSymbolMember(StringOrSymbolReference symbol) const
    {
        auto iter = symbolMembers.find(symbol);
        if(iter == symbolMembers.end())
            return Member();
        return std::get<1>(*iter);
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
        assert(dynamic_cast<T *>(getBase()) == getBase());
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
    assert(objectDescriptor);
    return (objectDescriptor->getEmbeddedMemberCount() + ObjectMemberGroup::objectMembersPerGroup
            - 1) / ObjectMemberGroup::objectMembersPerGroup;
}

inline ObjectMember::Type &Object::getMemberType(std::size_t memberIndex)
{
    assert(memberIndex < objectDescriptor->getEmbeddedMemberCount());
    return getMembersArray()[memberIndex / ObjectMemberGroup::objectMembersPerGroup]
        .types[memberIndex % ObjectMemberGroup::objectMembersPerGroup];
}

inline ObjectMember &Object::getMemberValue(std::size_t memberIndex)
{
    assert(memberIndex < objectDescriptor->getEmbeddedMemberCount());
    return getMembersArray()[memberIndex / ObjectMemberGroup::objectMembersPerGroup]
        .values[memberIndex % ObjectMemberGroup::objectMembersPerGroup];
}

inline ObjectMember::Type Object::getMemberType(std::size_t memberIndex) const
{
    assert(memberIndex < objectDescriptor->getEmbeddedMemberCount());
    return getMembersArray()[memberIndex / ObjectMemberGroup::objectMembersPerGroup]
        .types[memberIndex % ObjectMemberGroup::objectMembersPerGroup];
}

inline const ObjectMember &Object::getMemberValue(std::size_t memberIndex) const
{
    assert(memberIndex < objectDescriptor->getEmbeddedMemberCount());
    return getMembersArray()[memberIndex / ObjectMemberGroup::objectMembersPerGroup]
        .values[memberIndex % ObjectMemberGroup::objectMembersPerGroup];
}

struct ObjectHandle final
{
    ObjectReference object;
    inline ObjectHandle(GC &gc, ObjectReference object);
    constexpr ObjectHandle() : object()
    {
    }
    constexpr bool empty() const
    {
        return object == ObjectReference();
    }
};

struct StringHandle final
{
    StringOrSymbolReference string;
    inline StringHandle(GC &gc, StringOrSymbolReference string);
    constexpr StringHandle() : string(nullptr)
    {
    }
    constexpr bool empty() const
    {
        return string == nullptr;
    }
};

struct SymbolHandle final
{
    StringOrSymbolReference symbol;
    inline SymbolHandle(GC &gc, StringOrSymbolReference symbol);
    constexpr SymbolHandle() : symbol(nullptr)
    {
    }
    constexpr bool empty() const
    {
        return symbol == nullptr;
    }
};

struct ValueHandle final
{
    ObjectMember value;
    ObjectMember::Type type;
    ValueHandle() : value(), type(ObjectMember::Type::Undefined)
    {
        value.stringOrSymbolValue = nullptr;
    }
    inline ValueHandle(GC &gc, ObjectMember::Type type, ObjectMember value);
    ValueHandle(GC &gc, const Value &value) : ValueHandle(gc, value.type, value.value)
    {
    }
    constexpr operator Value() const noexcept
    {
        return Value(type, value);
    }

private:
    constexpr ValueHandle(ObjectMember::Type type, ObjectMember value) : value(value), type(type)
    {
    }
    constexpr ValueHandle(const Value &value) : value(value.value), type(value.type)
    {
    }

public:
    static ValueHandle makeUndefined()
    {
        ObjectMember value;
        value.stringOrSymbolValue = nullptr;
        return ValueHandle(ObjectMember::Type::Undefined, value);
    }
    bool isUndefined() const
    {
        return value.isUndefined(type);
    }
    static ValueHandle makeNull()
    {
        ObjectMember value;
        value.stringOrSymbolValue = nullptr;
        return ValueHandle(ObjectMember::Type::Null, value);
    }
    bool isNull() const
    {
        return value.isNull(type);
    }
    static ValueHandle makeObject(ObjectHandle object)
    {
        assert(!object.empty());
        ObjectMember value;
        value.objectValue = object.object;
        return ValueHandle(ObjectMember::Type::Object, value);
    }
    static ValueHandle makeObjectOrNull(ObjectHandle object)
    {
        if(object.empty())
            return makeNull();
        return makeObject(object);
    }
    bool isObject() const
    {
        return value.isObject(type);
    }
    ObjectHandle getObject() const
    {
        assert(isObject());
        ObjectHandle retval;
        retval.object = value.objectValue;
        return retval;
    }
    ObjectHandle getIfObject() const
    {
        if(isObject())
            return getObject();
        return ObjectHandle();
    }
    static ValueHandle makeObject(GC &gc, ObjectReference object)
    {
        return makeObject(ObjectHandle(gc, object));
    }
    static ValueHandle makeDouble(double doubleValue)
    {
        ObjectMember value;
        value.doubleValue = doubleValue;
        return ValueHandle(ObjectMember::Type::Double, value);
    }
    bool isDouble() const
    {
        return value.isDouble(type);
    }
    double getDouble() const
    {
        assert(isDouble());
        return value.doubleValue;
    }
    static ValueHandle makeInt32(std::int32_t intValue)
    {
        ObjectMember value;
        value.uint32Value = static_cast<std::uint32_t>(intValue);
        return ValueHandle(ObjectMember::Type::Int32, value);
    }
    bool isInt32() const
    {
        return value.isInt32(type);
    }
    std::int32_t getInt32() const
    {
        assert(isInt32());
        return static_cast<std::int32_t>(value.uint32Value);
    }
    static ValueHandle makeUInt32(std::uint32_t intValue)
    {
        ObjectMember value;
        value.uint32Value = intValue;
        return ValueHandle(ObjectMember::Type::UInt32, value);
    }
    bool isUInt32() const
    {
        return value.isUInt32(type);
    }
    std::uint32_t getUInt32() const
    {
        assert(isUInt32());
        return value.uint32Value;
    }
    static ValueHandle makeBoolean(bool boolValue)
    {
        ObjectMember value;
        value.uint32Value = boolValue ? 1 : 0;
        return ValueHandle(ObjectMember::Type::Boolean, value);
    }
    bool isBoolean() const
    {
        return value.isBoolean(type);
    }
    bool getBoolean() const
    {
        assert(isBoolean());
        return value.uint32Value ? true : false;
    }
    static ValueHandle makeSymbol(SymbolHandle symbol)
    {
        assert(!symbol.empty());
        ObjectMember value;
        value.stringOrSymbolValue = symbol.symbol;
        return ValueHandle(ObjectMember::Type::Symbol, value);
    }
    bool isSymbol() const
    {
        return value.isSymbol(type);
    }
    SymbolHandle getSymbol() const
    {
        assert(isSymbol());
        SymbolHandle retval;
        retval.symbol = value.stringOrSymbolValue;
        return retval;
    }
    SymbolHandle getIfSymbol() const
    {
        if(isSymbol())
            return getSymbol();
        return SymbolHandle();
    }
    static ValueHandle makeSymbol(GC &gc, StringOrSymbolReference symbol)
    {
        return makeSymbol(SymbolHandle(gc, symbol));
    }
    static ValueHandle makeString(StringHandle string)
    {
        assert(!string.empty());
        ObjectMember value;
        value.stringOrSymbolValue = string.string;
        return ValueHandle(ObjectMember::Type::String, value);
    }
    bool isString() const
    {
        return value.isString(type);
    }
    StringHandle getString() const
    {
        assert(isString());
        StringHandle retval;
        retval.string = value.stringOrSymbolValue;
        return retval;
    }
    StringHandle getIfString() const
    {
        if(isString())
            return getString();
        return StringHandle();
    }
    static ValueHandle makeString(GC &gc, StringOrSymbolReference string)
    {
        return makeString(StringHandle(gc, string));
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
    HandleScope *parent;
    GC &gc;
    std::vector<ObjectReference> objectReferences;
    std::vector<StringOrSymbolReference> stringOrSymbolReferences;
    std::vector<const ObjectDescriptor *> objectDescriptors;
    inline void addToGC();
    inline void removeFromGC();
    void init(std::size_t objectReferenceCount, std::size_t stringOrSymbolReferenceCount)
    {
        objectReferences.reserve(objectReferenceCount);
        stringOrSymbolReferences.reserve(stringOrSymbolReferenceCount);
        addToGC();
    }
    void init(std::size_t referenceCount = 20)
    {
        init(referenceCount, referenceCount);
    }
    void addHandle(const ObjectHandle &object)
    {
        if(!object.empty())
            objectReferences.push_back(object.object);
    }
    void addHandle(const StringHandle &string)
    {
        if(!string.empty())
            stringOrSymbolReferences.push_back(string.string);
    }
    void addHandle(const SymbolHandle &symbol)
    {
        if(!symbol.empty())
            stringOrSymbolReferences.push_back(symbol.symbol);
    }
    inline void addHandle(const ObjectDescriptorHandle<> &objectDescriptor);
    void addHandle(const ValueHandle &value)
    {
        switch(value.type)
        {
        case ObjectMember::Type::Object:
            assert(value.value.objectValue != nullptr);
            objectReferences.push_back(value.value.objectValue);
            return;
        case ObjectMember::Type::Double:
        case ObjectMember::Type::Int32:
        case ObjectMember::Type::UInt32:
        case ObjectMember::Type::Boolean:
            return;
        case ObjectMember::Type::Symbol:
        case ObjectMember::Type::String:
            if(value.value.stringOrSymbolValue != nullptr)
                stringOrSymbolReferences.push_back(value.value.stringOrSymbolValue);
            return;
        }
    }

public:
    explicit HandleScope(GC &gc)
        : parent(nullptr), gc(gc), objectReferences(), stringOrSymbolReferences()
    {
        init();
    }
    HandleScope(GC &gc, std::size_t referenceCount)
        : parent(nullptr), gc(gc), objectReferences(), stringOrSymbolReferences()
    {
        init(referenceCount);
    }
    HandleScope(GC &gc, std::size_t objectReferenceCount, std::size_t stringOrSymbolReferenceCount)
        : parent(nullptr), gc(gc), objectReferences(), stringOrSymbolReferences()
    {
        init(objectReferenceCount, stringOrSymbolReferenceCount);
    }
    ~HandleScope()
    {
        removeFromGC();
    }
    ObjectHandle escapeHandle(const ObjectHandle &handle)
    {
        assert(parent);
        parent->addHandle(handle);
        return handle;
    }
    SymbolHandle escapeHandle(const SymbolHandle &handle)
    {
        assert(parent);
        parent->addHandle(handle);
        return handle;
    }
    StringHandle escapeHandle(const StringHandle &handle)
    {
        assert(parent);
        parent->addHandle(handle);
        return handle;
    }
    ValueHandle escapeHandle(const ValueHandle &handle)
    {
        assert(parent);
        parent->addHandle(handle);
        return handle;
    }
    template <typename T>
    ObjectDescriptorHandle<T> escapeHandle(const ObjectDescriptorHandle<T> &handle)
    {
        assert(parent);
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
    std::vector<String *> strings, oldStrings;
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
    void collectorMarkObjectMember(ObjectMember::Type type, const ObjectMember &value) noexcept;
    void collectorMarkObjectDescriptorMember(const ObjectDescriptor::Member &value) noexcept;
    void collectorScanObject(ObjectReference objectReference) noexcept;
    void collectorScanObjectDescriptor(ObjectDescriptor *objectDescriptor) noexcept;
    StringHandle internStringHelper(const String &value, std::size_t valueHash) noexcept;
    ObjectDescriptorStringOrSymbolTransition &findOrAddObjectDescriptorStringOrSymbolTransition(
        std::vector<std::forward_list<ObjectDescriptorStringOrSymbolTransition>> &transitions,
        StringOrSymbolReference stringOrSymbol,
        const ObjectDescriptor *sourceDescriptor);

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
        assert(handle.object.index < objects.size());
        const Object *retval = objects[handle.object.index];
        assert(retval);
        return *retval;
    }
    Object &writeObject(ObjectHandle handle)
    {
        assert(!handle.empty());
        assert(!immutable);
        assert(handle.object.index < objects.size());
        Object *&retval = objects[handle.object.index];
        assert(retval);
        if(retval->gc == this)
            return *retval;
        return *objectCopyOnWrite(handle.object);
    }
    ObjectDescriptorHandle<> getObjectDescriptor(ObjectHandle handle) const noexcept
    {
        return ObjectDescriptorHandle<>(readObject(handle).objectDescriptor);
    }
    ObjectDescriptor::Member addObjectMember(StringHandle nameHandle,
                                             ObjectHandle object,
                                             ValueHandle value,
                                             bool isEmbedded = true);
    ObjectDescriptor::Member addObjectMember(SymbolHandle nameHandle,
                                             ObjectHandle object,
                                             ValueHandle value,
                                             bool isEmbedded = true);
    const String &readString(StringHandle handle) const noexcept
    {
        assert(handle.string.index < strings.size());
        const String *retval = strings[handle.string.index];
        assert(retval);
        return *retval;
    }
    const String &readSymbol(SymbolHandle handle) const noexcept
    {
        assert(handle.symbol.index < strings.size());
        const String *retval = strings[handle.symbol.index];
        assert(retval);
        return *retval;
    }
    StringHandle internString(const String &value);
    StringHandle internString(String &&value);
    SymbolHandle createSymbol(String description);
    ObjectHandle create(ObjectDescriptorHandle<> objectDescriptor);
    template <typename T, typename... Args>
    ObjectDescriptorHandle<T> createObjectDescriptor(
        ObjectDescriptorHandle<T> parentObjectDescriptor, Args &&... args)
    {
        static_assert(std::is_base_of<ObjectDescriptor, T>::value,
                      "T is not derived from ObjectDescriptor");
        assert(parentObjectDescriptor.empty() || typeid(parentObjectDescriptor.get()) == typeid(T));
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
};

inline ObjectHandle::ObjectHandle(GC &gc, ObjectReference object) : object(object)
{
    assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}

inline StringHandle::StringHandle(GC &gc, StringOrSymbolReference string) : string(string)
{
    assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}

inline SymbolHandle::SymbolHandle(GC &gc, StringOrSymbolReference symbol) : symbol(symbol)
{
    assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}

inline ValueHandle::ValueHandle(GC &gc, ObjectMember::Type type, ObjectMember value)
    : value(value), type(type)
{
    assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}

inline void HandleScope::addToGC()
{
    assert(!parent);
    parent = gc.handleScopesStack;
    gc.handleScopesStack = this;
}

inline void HandleScope::removeFromGC()
{
    assert(this == gc.handleScopesStack);
    gc.handleScopesStack = parent;
    parent = nullptr;
}

inline void HandleScope::addHandle(const ObjectDescriptorHandle<> &objectDescriptor)
{
    if(!objectDescriptor.empty() && objectDescriptor.get()->gc == &gc)
    {
        objectDescriptors.push_back(objectDescriptor.get());
    }
}

inline ObjectDescriptorHandle<ObjectDescriptor>::ObjectDescriptorHandle(
    GC &gc, const ObjectDescriptor *objectDescriptor)
    : value(objectDescriptor)
{
    assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}
}
}

#endif /* JAVASCRIPT_TASKLETS_GC_H_ */
