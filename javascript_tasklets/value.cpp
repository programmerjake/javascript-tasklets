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

#include "value.h"

namespace javascript_tasklets
{
namespace value
{
PropertyHandle &PropertyHandle::completePropertyDescriptor() noexcept
{
    if(isGenericDescriptor() || isDataDescriptor())
    {
        if(!hasValue)
            value = ValueHandle();

        hasValue = true;
        if(!hasWritable)
            writable = false;

        hasWritable = true;
    }
    else
    {
        if(!hasGet)
            get = ObjectOrNullHandle(nullptr);

        hasGet = true;
        if(!hasSet)
            set = ObjectOrNullHandle(nullptr);

        hasSet = true;
    }
    if(!hasConfigurable)
        configurable = false;

    hasConfigurable = true;
    if(!hasEnumerable)
        enumerable = false;

    hasEnumerable = true;
    return *this;
}

namespace
{
struct ObjectPrototypeInternalNameTag final
{
};
struct ObjectExtensibleInternalNameTag final
{
};
}

ObjectOrNullHandle ObjectHandle::getPrototype(GC &gc) const
{
    HandleScope handleScope(gc);
    PropertyHandle property =
        getOwnProperty(gc::InternalNameMaker::get<ObjectPrototypeInternalNameTag>(), gc);
    if(property.empty())
        return ObjectOrNullHandle(nullptr);
    constexpr_assert(property.isDataDescriptor());
    constexpr_assert(property.hasValue);
    return handleScope.escapeHandle(property.value.getObjectOrNull());
}

void ObjectHandle::setPrototypeUnchecked(ObjectOrNullHandle newPrototype, GC &gc) const
{
    HandleScope handleScope(gc);
    PropertyHandle property(newPrototype, true, false, false);
    setOwnProperty(
        gc::Name(gc::InternalNameMaker::get<ObjectPrototypeInternalNameTag>()), property, gc, true);
}

BooleanHandle ObjectHandle::isExtensible(GC &gc) const
{
    HandleScope handleScope(gc);
    PropertyHandle property =
        getOwnProperty(gc::InternalNameMaker::get<ObjectExtensibleInternalNameTag>(), gc);
    if(property.empty())
        return false;
    constexpr_assert(property.isDataDescriptor());
    constexpr_assert(property.hasValue);
    return handleScope.escapeHandle(property.value.getBoolean());
}

BooleanHandle ObjectHandle::preventExtensions(GC &gc) const
{
    HandleScope handleScope(gc);
    PropertyHandle property(BooleanHandle(false, gc), false, false, false);
    setOwnProperty(gc::Name(gc::InternalNameMaker::get<ObjectExtensibleInternalNameTag>()),
                   property,
                   gc,
                   true);
    return handleScope.escapeHandle(BooleanHandle(true, gc));
}

PropertyHandle ObjectHandle::getOwnProperty(NameHandle name, GC &gc) const
{
    return getOwnProperty(gc::Name(name.get().get()), gc);
}

BooleanHandle ObjectHandle::hasProperty(NameHandle name, GC &gc) const
{
    constexpr_assert(false);
#warning finish
}

PropertyHandle ObjectHandle::getOwnProperty(gc::InternalName name, GC &gc) const
{
    return getOwnProperty(gc::Name(name), gc);
}

ValueHandle ObjectHandle::getValue(NameHandle name, GC &gc) const
{
    constexpr_assert(false);
#warning finish
}

namespace
{
struct ObjectHandleGetOwnPropertyFillInRetval final
{
    PropertyHandle &retval;
    GC &gc;
    const Handle<gc::ObjectReference> &value;
    const gc::ObjectMemberDescriptor &member;
    ObjectHandleGetOwnPropertyFillInRetval(PropertyHandle &retval,
                                           GC &gc,
                                           const Handle<gc::ObjectReference> &value,
                                           const gc::ObjectMemberDescriptor &member)
        : retval(retval), gc(gc), value(value), member(member)
    {
    }
    void operator()()
    {
        constexpr_assert(false);
    }
    template <typename T>
    void operator()(T) = delete;
    void operator()(const gc::ObjectMemberDescriptor::AccessorInDescriptorT &v)
    {
        retval = PropertyHandle(member.enumerable,
                                member.configurable,
                                ObjectOrNullHandle(Handle<gc::ObjectReference>(value, v.getter)),
                                ObjectOrNullHandle(Handle<gc::ObjectReference>(value, v.setter)));
    }
    void operator()(const gc::ObjectMemberDescriptor::DataInDescriptorT &v)
    {
        retval = PropertyHandle(
            Handle<gc::Value>(value, v.value), v.writable, member.enumerable, member.configurable);
    }
    void operator()(const gc::ObjectMemberDescriptor::DataInObjectT &v)
    {
        retval =
            PropertyHandle(Handle<gc::Value>(value, gc.readObject(value).getMember(v.memberIndex)),
                           v.writable,
                           member.enumerable,
                           member.configurable);
    }
};
}

PropertyHandle ObjectHandle::getOwnProperty(gc::Name name, GC &gc) const
{
    HandleScope handleScope(gc);
    auto objectDescriptor = gc.getObjectDescriptor(value);
    std::size_t memberIndex = gc.readObjectDescriptor(objectDescriptor).findMember(name);
    if(memberIndex == gc::ObjectDescriptor::npos)
        return PropertyHandle();
    PropertyHandle retval;
    gc::ObjectDescriptor::Member member =
        gc.readObjectDescriptor(objectDescriptor).getMember(memberIndex);
    member.descriptor.value.apply(
        ObjectHandleGetOwnPropertyFillInRetval(retval, gc, value, member.descriptor));
    return handleScope.escapeHandle(retval);
}

BooleanHandle ObjectHandle::setPrototype(ObjectOrNullHandle newPrototype, GC &gc) const
{
    constexpr_assert(false);
#warning finish
}

void ObjectHandle::setOwnProperty(gc::Name name,
                                  const PropertyHandle &value,
                                  GC &gc,
                                  bool putInObjectDescriptor) const
{
    constexpr_assert(false);
#warning finish
}
}
}
