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

#include <cassert>
#include <iterator>
#include <cstdint>
#include <vector>
#include <new>
#include <memory>
#include <unordered_map>
#include "string.h"

namespace javascript_tasklets
{
namespace gc
{
struct ObjectDescriptor;

struct ObjectReference final
{
    std::size_t index;
    static constexpr std::size_t invalidIndex = ~static_cast<std::size_t>(0);
    constexpr explicit ObjectReference(std::size_t index) : index(index)
    {
    }
    constexpr explicit ObjectReference(std::nullptr_t = nullptr) : index(invalidIndex)
    {
    }
    constexpr bool operator==(const ObjectReference &rt) const
    {
        return index == rt.index;
    }
    constexpr bool operator!=(const ObjectReference &rt) const
    {
        return index != rt.index;
    }
    constexpr bool operator==(std::nullptr_t) const
    {
        return index == invalidIndex;
    }
    constexpr bool operator!=(std::nullptr_t) const
    {
        return index != invalidIndex;
    }
    friend bool operator==(std::nullptr_t, const ObjectReference &v)
    {
        return v.index == invalidIndex;
    }
    friend bool operator!=(std::nullptr_t, const ObjectReference &v)
    {
        return v.index != invalidIndex;
    }
};
}
}

namespace std
{
template <>
struct hash<javascript_tasklets::gc::ObjectReference> final
{
    constexpr std::size_t operator()(const javascript_tasklets::gc::ObjectReference &v) const
    {
        return v.index;
    }
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
    const String *stringValue;
    constexpr ObjectMember();
    typedef std::uint8_t TypeBaseType;
    enum class Type : TypeBaseType
    {
        Object, // uses objectValue
        Double, // uses doubleValue
        Int32, // uses uint32Value
        UInt32, // uses uint32Value
        Boolean, // uses uint32Value
        Symbol, // uses stringValue
        String, // uses stringValue
        Undefined = String, // stringValue is null
        Null = Symbol, // stringValue is null
    };
    constexpr bool isObject(Type type) const
    {
        return type == Type::Object;
    }
    constexpr bool isDouble(Type type) const
    {
        return type == Type::Double;
    }
    constexpr bool isInt32(Type type) const
    {
        return type == Type::Int32;
    }
    constexpr bool isUInt32(Type type) const
    {
        return type == Type::UInt32;
    }
    constexpr bool isBoolean(Type type) const
    {
        return type == Type::Boolean;
    }
    constexpr bool isSymbol(Type type) const
    {
        return type == Type::Symbol && stringValue != nullptr;
    }
    constexpr bool isString(Type type) const
    {
        return type == Type::String && stringValue != nullptr;
    }
    constexpr bool isUndefined(Type type) const
    {
        return type == Type::Undefined && stringValue == nullptr;
    }
    constexpr bool isNull(Type type) const
    {
        return type == Type::Null && stringValue == nullptr;
    }
};

constexpr ObjectMember::ObjectMember() : uint32Value()
{
}

struct Value final
{
    ObjectMember value;
    ObjectMember::Type type;
};

struct ObjectMemberGroup final
{
    static constexpr std::size_t objectMembersPerGroup =
        sizeof(ObjectMember) / sizeof(ObjectMember::Type);
    ObjectMember::Type types[objectMembersPerGroup];
    ObjectMember values[objectMembersPerGroup];
};

class GC;

class Object final
{
    friend class GC;
    Object(const Object &rt) = delete;
    Object &operator=(const Object &rt) = delete;

public:
    struct ExtraData : public std::enable_shared_from_this<ExtraData>
    {
        virtual ~ExtraData() = default;
        ExtraData() = default;
        virtual std::shared_ptr<ExtraData> clone() const = 0;
    };

private:
    Object(const ObjectDescriptor *objectDescriptor,
           const GC *gc,
           std::shared_ptr<ExtraData> extraData)
        : objectDescriptor(objectDescriptor), gc(gc), extraData(std::move(extraData))
    {
    }
    ~Object() = default;

public:
    const ObjectDescriptor *objectDescriptor;
    const GC *gc;
    std::shared_ptr<ExtraData> extraData;
    ObjectMemberGroup *getMembersArray();
    const ObjectMemberGroup *getMembersArray() const;
    ObjectMember::Type &getMemberType(std::size_t memberIndex);
    ObjectMember &getMemberValue(std::size_t memberIndex);
    ObjectMember::Type getMemberType(std::size_t memberIndex) const;
    const ObjectMember &getMemberValue(std::size_t memberIndex) const;
};

inline ObjectMemberGroup *Object::getMembersArray()
{
    return reinterpret_cast<ObjectMemberGroup *>(this + 1);
}

inline const ObjectMemberGroup *Object::getMembersArray() const
{
    return reinterpret_cast<const ObjectMemberGroup *>(this + 1);
}

struct ObjectDescriptor
{
    struct Member final
    {
        enum class Kind
        {
            Empty,
            Transition,
            Embedded,
            Constant,
        };
        union MemberValue final
        {
            std::size_t embeddedMember;
            const ObjectDescriptor *newObjectDescriptor;
            Value constantMember;
            constexpr MemberValue() : embeddedMember()
            {
            }
            constexpr MemberValue(std::size_t embeddedMember) : embeddedMember(embeddedMember)
            {
            }
            constexpr MemberValue(const ObjectDescriptor *newObjectDescriptor)
                : newObjectDescriptor(newObjectDescriptor)
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
        static constexpr Member makeTransition(const ObjectDescriptor *newObjectDescriptor)
        {
            return Member(Kind::Transition, MemberValue(newObjectDescriptor));
        }
        static constexpr Member makeEmbedded(std::size_t embeddedMember)
        {
            return Member(Kind::Embedded, MemberValue(embeddedMember));
        }
        static constexpr Member makeConstant(Value constantMember)
        {
            return Member(Kind::Embedded, Value(constantMember));
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

private:
    std::size_t memberCount;
    std::unordered_map<String, Member> namedMembers;
    std::unordered_map<const String *, Member> symbolMembers;
    std::unordered_map<const InternalSlot *, Member> internalSlots;

public:
    explicit ObjectDescriptor(const GC *gc);
    virtual ~ObjectDescriptor() = default;
    std::size_t getMemberCount() const
    {
        return memberCount;
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
};

inline ObjectMember::Type &Object::getMemberType(std::size_t memberIndex)
{
    assert(memberIndex < objectDescriptor->getMemberCount());
    return getMembersArray()[memberIndex / ObjectMemberGroup::objectMembersPerGroup]
        .types[memberIndex % ObjectMemberGroup::objectMembersPerGroup];
}

inline ObjectMember &Object::getMemberValue(std::size_t memberIndex)
{
    assert(memberIndex < objectDescriptor->getMemberCount());
    return getMembersArray()[memberIndex / ObjectMemberGroup::objectMembersPerGroup]
        .values[memberIndex % ObjectMemberGroup::objectMembersPerGroup];
}

inline ObjectMember::Type Object::getMemberType(std::size_t memberIndex) const
{
    assert(memberIndex < objectDescriptor->getMemberCount());
    return getMembersArray()[memberIndex / ObjectMemberGroup::objectMembersPerGroup]
        .types[memberIndex % ObjectMemberGroup::objectMembersPerGroup];
}

inline const ObjectMember &Object::getMemberValue(std::size_t memberIndex) const
{
    assert(memberIndex < objectDescriptor->getMemberCount());
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
    const String *string;
    inline StringHandle(GC &gc, const String *string);
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
    const String *symbol;
    inline SymbolHandle(GC &gc, const String *symbol);
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
        value.stringValue = nullptr;
    }
    inline ValueHandle(GC &gc, ObjectMember::Type type, ObjectMember value);

private:
    ValueHandle(ObjectMember::Type type, ObjectMember value) : value(value), type(type)
    {
    }

public:
    static ValueHandle makeUndefined()
    {
        ObjectMember value;
        value.stringValue = nullptr;
        return ValueHandle(ObjectMember::Type::Undefined, value);
    }
    static ValueHandle makeNull()
    {
        ObjectMember value;
        value.stringValue = nullptr;
        return ValueHandle(ObjectMember::Type::Null, value);
    }
    static ValueHandle makeObject(ObjectHandle object)
    {
        assert(!object.empty());
        ObjectMember value;
        value.objectValue = object.object;
        return ValueHandle(ObjectMember::Type::Object, value);
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
    static ValueHandle makeInt32(std::int32_t intValue)
    {
        ObjectMember value;
        value.uint32Value = static_cast<std::uint32_t>(intValue);
        return ValueHandle(ObjectMember::Type::Int32, value);
    }
    static ValueHandle makeUInt32(std::uint32_t intValue)
    {
        ObjectMember value;
        value.uint32Value = intValue;
        return ValueHandle(ObjectMember::Type::UInt32, value);
    }
    static ValueHandle makeBoolean(bool boolValue)
    {
        ObjectMember value;
        value.uint32Value = boolValue ? 1 : 0;
        return ValueHandle(ObjectMember::Type::Boolean, value);
    }
    static ValueHandle makeSymbol(SymbolHandle symbol)
    {
        assert(!symbol.empty());
        ObjectMember value;
        value.stringValue = symbol.symbol;
        return ValueHandle(ObjectMember::Type::Symbol, value);
    }
    static ValueHandle makeSymbol(GC &gc, const String *symbol)
    {
        return makeSymbol(SymbolHandle(gc, symbol));
    }
    static ValueHandle makeString(StringHandle string)
    {
        assert(!string.empty());
        ObjectMember value;
        value.stringValue = string.string;
        return ValueHandle(ObjectMember::Type::String, value);
    }
    static ValueHandle makeString(GC &gc, const String *string)
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
    HandleScope(const HandleScope &rt) = delete;
    HandleScope &operator=(const HandleScope &rt) = delete;
    void *operator new(std::size_t) = delete;

private:
    HandleScope *parent;
    GC &gc;
    std::vector<ObjectReference> objectReferences;
    std::vector<const String *> stringReferences;
    inline void addToGC();
    inline void removeFromGC();
    void init(std::size_t objectReferenceCount, std::size_t stringReferenceCount)
    {
        objectReferences.reserve(objectReferenceCount);
        stringReferences.reserve(stringReferenceCount);
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
            stringReferences.push_back(string.string);
    }
    void addHandle(const SymbolHandle &symbol)
    {
        if(!symbol.empty())
            stringReferences.push_back(symbol.symbol);
    }
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
            if(value.value.stringValue)
                stringReferences.push_back(value.value.stringValue);
            return;
        }
    }

public:
    explicit HandleScope(GC &gc) : parent(nullptr), gc(gc), objectReferences(), stringReferences()
    {
        init();
    }
    HandleScope(GC &gc, std::size_t referenceCount)
        : parent(nullptr), gc(gc), objectReferences(), stringReferences()
    {
        init(referenceCount);
    }
    HandleScope(GC &gc, std::size_t objectReferenceCount, std::size_t stringReferenceCount)
        : parent(nullptr), gc(gc), objectReferences(), stringReferences()
    {
        init(objectReferenceCount, stringReferenceCount);
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
};

class GC final : public std::enable_shared_from_this<GC>
{
    friend class HandleScope;
    friend struct ObjectHandle;
    friend struct StringHandle;
    friend struct SymbolHandle;
    friend struct ValueHandle;
    GC &operator=(const GC &) = delete;
    GC(const GC &) = delete;

private:
    std::vector<Object *> objects;
    std::vector<String *> strings;
    bool immutable;
    const std::shared_ptr<const GC> parent;
    HandleScope *handleScopesStack;
    Object *objectCopyOnWrite(ObjectReference objectReference);

public:
    GC();
    GC(std::shared_ptr<const GC> parent);
    ~GC();
    void collect();
    bool isImmutable() const
    {
        return immutable;
    }
    void makeImmutable()
    {
        immutable = true;
    }
    const Object *getObjectForRead(ObjectReference objectReference) const
    {
        assert(objectReference.index < objects.size());
        return objects[objectReference.index];
    }
    Object *getObjectForWrite(ObjectReference objectReference)
    {
        assert(!immutable);
        assert(objectReference.index < objects.size());
        Object *&retval = objects[objectReference.index];
        if(retval->gc == this)
            return retval;
        return objectCopyOnWrite(objectReference);
    }
    ObjectHandle create(ObjectDescriptor *objectDescriptor);
};

inline ObjectHandle::ObjectHandle(GC &gc, ObjectReference object) : object(object)
{
    assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}

inline StringHandle::StringHandle(GC &gc, const String *string) : string(string)
{
    assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}

inline SymbolHandle::SymbolHandle(GC &gc, const String *symbol) : symbol(symbol)
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
}
}

#endif /* JAVASCRIPT_TASKLETS_GC_H_ */
