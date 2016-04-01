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

union ObjectMember final
{
    std::size_t referenceIndexValue;
    double doubleValue;
    std::uint32_t uint32Value;
    const String *stringValue;
    typedef std::uint8_t TypeBaseType;
    enum class Type : TypeBaseType
    {
        Object, // uses referenceIndexValue
        Double, // uses doubleValue
        Int32, // uses uint32Value
        UInt32, // uses uint32Value
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

struct Value final
{
    ObjectMember::Type type;
    ObjectMember value;
};

struct ObjectMemberGroup final
{
    static constexpr std::size_t objectMembersPerGroup =
        sizeof(ObjectMember) / sizeof(ObjectMember::Type);
    ObjectMember::Type types[objectMembersPerGroup];
    ObjectMember values[objectMembersPerGroup];
};

class GC;

struct Object final
{
    const ObjectDescriptor *objectDescriptor;
    const GC *gc;
    struct ExtraData : public std::enable_shared_from_this<ExtraData>
    {
        virtual ~ExtraData() = default;
        ExtraData() = default;
        virtual std::shared_ptr<ExtraData> clone() const = 0;
    };
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


class GC : public std::enable_shared_from_this<GC>
{
    GC &operator=(const GC &) = delete;
    GC(const GC &) = delete;

private:
    std::vector<Object *> objects;
    std::vector<String *> strings;
    bool immutable;
    const std::shared_ptr<const GC> parent;
    Object *objectCopyOnWrite(std::size_t referenceIndex);

public:
    GC();
    GC(std::shared_ptr<const GC> parent);
    ~GC();
    bool isImmutable() const
    {
        return immutable;
    }
    void makeImmutable()
    {
        immutable = true;
    }
    const Object *getObjectForRead(std::size_t referenceIndex) const
    {
        assert(referenceIndex > 0 && referenceIndex < objects.size());
        return objects[referenceIndex];
    }
    Object *getObjectForWrite(std::size_t referenceIndex)
    {
        assert(!immutable);
        assert(referenceIndex > 0 && referenceIndex < objects.size());
        Object *&retval = objects[referenceIndex];
        if(retval->gc == this)
            return retval;
        return objectCopyOnWrite(referenceIndex);
    }
    Object *create(ObjectDescriptor *objectDescriptor);
};
}
}

#endif /* JAVASCRIPT_TASKLETS_GC_H_ */
