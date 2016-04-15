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
#include <cmath>

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

ObjectOrNullHandle ObjectHandle::ordinaryGetPrototype(GC &gc) const
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

ObjectOrNullHandle ObjectHandle::getPrototype(GC &gc) const
{
    return ordinaryGetPrototype(gc);
}

bool ObjectHandle::getPrototypeIsOrdinary(GC &gc) const
{
    return true;
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
    return ordinaryIsExtensible(gc);
}

BooleanHandle ObjectHandle::ordinaryIsExtensible(GC &gc) const
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
    return ordinaryHasProperty(name, gc);
}

BooleanHandle ObjectHandle::ordinaryHasProperty(NameHandle name, GC &gc) const
{
    ObjectHandle parentObject;
    {
        HandleScope handleScope(gc);
        BooleanHandle hasOwn = hasOwnProperty(name, gc);
        if(hasOwn.get().get())
        {
            return handleScope.escapeHandle(hasOwn);
        }
        ObjectOrNullHandle parent = getPrototype(gc);
        if(parent.isNull())
            return handleScope.escapeHandle(BooleanHandle(false, gc));
        parentObject = handleScope.escapeHandle(parent.getObject());
    }
    return parentObject.hasProperty(name, gc);
}

PropertyHandle ObjectHandle::getOwnProperty(gc::InternalName name, GC &gc) const
{
    return getOwnProperty(gc::Name(name), gc);
}

ValueHandle ObjectHandle::getValue(NameHandle name, ValueHandle reciever, GC &gc) const
{
    HandleScope handleScope(gc);
    PropertyHandle property = getOwnProperty(name, gc);
    if(property.empty())
    {
        ObjectOrNullHandle parent = getPrototype(gc);
        if(parent.isNull())
            return ValueHandle();
        return handleScope.escapeHandle(parent.getObject().getValue(name, reciever, gc));
    }
    if(property.isDataDescriptor())
    {
        constexpr_assert(property.hasValue);
        return handleScope.escapeHandle(property.value);
    }
    constexpr_assert(property.isAccessorDescriptor());
    constexpr_assert(property.hasGet);
    if(property.get.isNull())
        return ValueHandle();
    return handleScope.escapeHandle(
        property.get.getObject().call(reciever, std::vector<ValueHandle>(), gc));
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
    HandleScope handleScope(gc);
    constexpr_assert(newPrototype.isObject() || newPrototype.isNull());
    BooleanHandle extensible = ordinaryIsExtensible(gc);
    ObjectOrNullHandle current = ordinaryGetPrototype(gc);
    if(current.isSameValue(newPrototype))
    {
        return handleScope.escapeHandle(BooleanHandle(true, gc));
    }
    if(!extensible.getValue(gc))
    {
        return handleScope.escapeHandle(BooleanHandle(false, gc));
    }
    ObjectOrNullHandle prototype = newPrototype;
    while(true)
    {
        if(!prototype.isObject())
            break;
        ObjectHandle prototypeObject = prototype.getObject();
        if(prototypeObject.isSameValue(*this))
            return handleScope.escapeHandle(BooleanHandle(false, gc));
        if(!prototypeObject.getPrototypeIsOrdinary(gc))
            break;
        prototype = prototypeObject.ordinaryGetPrototype(gc);
    }
    setPrototypeUnchecked(newPrototype, gc);
    return handleScope.escapeHandle(BooleanHandle(true, gc));
}

ObjectHandle ObjectHandle::create(ObjectOrNullHandle prototype, GC &gc)
{
    HandleScope handleScope(gc);
    struct DefaultObjectTag final
    {
    };
    Handle<gc::Value> defaultObject = gc.getGlobalValue<DefaultObjectTag>();
    Handle<gc::ObjectDescriptorReference> objectDescriptor;
    if(defaultObject.get().empty())
    {
        auto mutableObjectDescriptor = gc.createObjectDescriptor();
        mutableObjectDescriptor.get()->addMember(gc::ObjectDescriptor::Member(
            gc::Name(gc::InternalNameMaker::get<ObjectExtensibleInternalNameTag>()),
            gc::ObjectMemberDescriptor::DataInDescriptor(
                false, false, gc::Value::make<bool>(true), true)));
        objectDescriptor = mutableObjectDescriptor;
        defaultObject = Handle<gc::Value>(gc.create(objectDescriptor));
        gc.setGlobalValue<DefaultObjectTag>(defaultObject);
    }
    else
    {
        objectDescriptor = gc.getObjectDescriptor(Handle<gc::ObjectReference>(
            defaultObject, defaultObject.get().get<gc::ObjectReference>()));
    }
    ObjectHandle retval = gc.create(objectDescriptor);
    retval.setPrototypeUnchecked(prototype, gc);
    return handleScope.escapeHandle(retval);
}

ObjectHandle ObjectHandle::create(Handle<gc::ObjectDescriptorReference> objectDescriptor,
                                  std::unique_ptr<gc::Object::ExtraData> extraData,
                                  ObjectOrNullHandle prototype,
                                  GC &gc)
{
    HandleScope handleScope(gc);
    ObjectHandle retval = gc.create(objectDescriptor, std::move(extraData));
    retval.setOwnProperty(gc::Name(gc::InternalNameMaker::get<ObjectExtensibleInternalNameTag>()),
                          PropertyHandle(BooleanHandle(true, gc), true, false, false),
                          gc,
                          true);
    retval.setPrototypeUnchecked(prototype, gc);
    return handleScope.escapeHandle(retval);
}

BooleanHandle ObjectHandle::hasOwnProperty(NameHandle name, GC &gc) const
{
    return hasOwnProperty(gc::Name(name.get().get()), gc);
}

BooleanHandle ObjectHandle::hasOwnProperty(gc::InternalName name, GC &gc) const
{
    return hasOwnProperty(gc::Name(name), gc);
}

BooleanHandle ObjectHandle::hasOwnProperty(gc::Name name, GC &gc) const
{
    HandleScope handleScope(gc);
    auto objectDescriptor = gc.getObjectDescriptor(value);
    std::size_t memberIndex = gc.readObjectDescriptor(objectDescriptor).findMember(name);
    if(memberIndex == gc::ObjectDescriptor::npos)
        return handleScope.escapeHandle(BooleanHandle(false, gc));
    return handleScope.escapeHandle(BooleanHandle(true, gc));
}

bool ObjectHandle::validateAndApplyPropertyDescriptor(ObjectOrNullHandle object,
                                                      NameHandle name,
                                                      bool extensible,
                                                      PropertyHandle newProperty,
                                                      PropertyHandle currentProperty,
                                                      GC &gc)
{
    if(currentProperty.empty())
    {
        if(!extensible)
            return false;
        if(object.isNull())
            return true;
        newProperty.completePropertyDescriptor();
        object.getObject().setOwnProperty(name, newProperty, gc);
        return true;
    }
    constexpr_assert(currentProperty.isCompleteDescriptor());
    if(newProperty.empty())
        return true;
    if(newProperty.isSameValue(currentProperty))
        return true;
    if(!currentProperty.configurable)
    {
        if(newProperty.hasConfigurable && newProperty.configurable)
            return false;
        if(newProperty.hasEnumerable && newProperty.enumerable != currentProperty.enumerable)
            return false;
    }
    if(newProperty.isGenericDescriptor())
    {
        // no more verification needed
    }
    else if(currentProperty.isDataDescriptor() != newProperty.isDataDescriptor())
    {
        if(!currentProperty.configurable)
            return false;
    }
    else if(currentProperty.isDataDescriptor() && newProperty.isDataDescriptor())
    {
        if(!currentProperty.configurable)
        {
            if(!currentProperty.writable && newProperty.hasWritable && newProperty.writable)
                return false;
            if(!currentProperty.writable && newProperty.hasValue
               && !currentProperty.value.isSameValue(newProperty.value))
                return false;
        }
    }
    else
    {
        constexpr_assert(currentProperty.isAccessorDescriptor());
        constexpr_assert(newProperty.isAccessorDescriptor());
        if(!currentProperty.configurable)
        {
            if(newProperty.hasGet && !currentProperty.get.isSameValue(newProperty.get))
                return false;
            if(newProperty.hasSet && !currentProperty.set.isSameValue(newProperty.set))
                return false;
        }
    }
    if(object.isNull())
        return true;
    if(newProperty.isGenericDescriptor())
    {
        if(newProperty.hasConfigurable)
            currentProperty.configurable = newProperty.configurable;
        if(newProperty.hasEnumerable)
            currentProperty.enumerable = newProperty.enumerable;
        object.getObject().setOwnProperty(name, currentProperty, gc);
        return true;
    }
    if(!newProperty.hasConfigurable)
        newProperty.configurable = currentProperty.configurable;
    newProperty.hasConfigurable = true;
    if(!newProperty.hasEnumerable)
        newProperty.enumerable = currentProperty.enumerable;
    newProperty.hasEnumerable = true;
    if(newProperty.isDataDescriptor())
    {
        if(!newProperty.hasWritable)
            newProperty.writable = currentProperty.writable;
        newProperty.hasWritable = true;
        if(!newProperty.hasValue)
            newProperty.value = currentProperty.value;
        newProperty.hasValue = true;
    }
    else
    {
        constexpr_assert(newProperty.isAccessorDescriptor());
        if(!newProperty.hasGet)
            newProperty.get = currentProperty.get;
        newProperty.hasGet = true;
        if(!newProperty.hasSet)
            newProperty.set = currentProperty.set;
        newProperty.hasSet = true;
    }
    object.getObject().setOwnProperty(name, newProperty, gc);
    return true;
}

bool ObjectHandle::isCompatiblePropertyDescriptor(bool extensible,
                                                  PropertyHandle newProperty,
                                                  PropertyHandle currentProperty,
                                                  GC &gc)
{
    return validateAndApplyPropertyDescriptor(
        ObjectOrNullHandle(nullptr), NameHandle(), extensible, newProperty, currentProperty, gc);
}

BooleanHandle ObjectHandle::ordinaryDefineOwnProperty(NameHandle name,
                                                      PropertyHandle property,
                                                      GC &gc) const
{
    PropertyHandle current = getOwnProperty(name, gc);
    bool extensible = ordinaryIsExtensible(gc).get().get();
    return validateAndApplyPropertyDescriptor(
        ObjectOrNullHandle(*this), name, extensible, property, current, gc);
}

BooleanHandle ObjectHandle::defineOwnProperty(NameHandle name,
                                              PropertyHandle property,
                                              GC &gc) const
{
    return ordinaryDefineOwnProperty(name, property, gc);
}

BooleanHandle ObjectHandle::deleteProperty(NameHandle name, GC &gc) const
{
    return ordinaryDeleteProperty(name, gc);
}

BooleanHandle ObjectHandle::ordinaryDeleteProperty(NameHandle name, GC &gc) const
{
    HandleScope handleScope(gc);
    PropertyHandle property = getOwnProperty(name, gc);
    if(property.empty())
        return handleScope.escapeHandle(BooleanHandle(true, gc));
    if(property.configurable)
    {
        gc.removeObjectMember(Handle<gc::Name>(name.get()), value);
        return handleScope.escapeHandle(BooleanHandle(true, gc));
    }
    return handleScope.escapeHandle(BooleanHandle(false, gc));
}

BooleanHandle ObjectHandle::setValue(NameHandle name,
                                     ValueHandle newValue,
                                     ValueHandle reciever,
                                     GC &gc) const
{
    return ordinarySetValue(name, newValue, reciever, gc);
}

BooleanHandle ObjectHandle::ordinarySetValue(NameHandle name,
                                             ValueHandle newValue,
                                             ValueHandle reciever,
                                             GC &gc) const
{
    HandleScope handleScope(gc);
    PropertyHandle ownProperty = getOwnProperty(name, gc);
    if(ownProperty.empty())
    {
        ObjectOrNullHandle parent = getPrototype(gc);
        if(!parent.isNull())
            return handleScope.escapeHandle(
                parent.getObject().setValue(name, newValue, reciever, gc));
        ownProperty = PropertyHandle(true, ValueHandle(), true, true, true, true, true, true);
    }
    constexpr_assert(ownProperty.isCompleteDescriptor());
    if(ownProperty.isDataDescriptor())
    {
        if(!ownProperty.writable)
            return handleScope.escapeHandle(BooleanHandle(false, gc));
        if(!reciever.isObject())
            return handleScope.escapeHandle(BooleanHandle(false, gc));
        PropertyHandle existingProperty = reciever.getObject().getOwnProperty(name, gc);
        if(!existingProperty.empty())
        {
            if(existingProperty.isAccessorDescriptor())
                return handleScope.escapeHandle(BooleanHandle(false, gc));
            if(existingProperty.writable)
                return handleScope.escapeHandle(BooleanHandle(false, gc));
            return handleScope.escapeHandle(reciever.getObject().defineOwnProperty(
                name,
                PropertyHandle(true, newValue, false, false, false, false, false, false),
                gc));
        }
        return handleScope.escapeHandle(reciever.getObject().defineOwnProperty(
            name, PropertyHandle(true, newValue, true, true, true, true, true, true), gc));
    }
    constexpr_assert(ownProperty.isAccessorDescriptor());
    if(ownProperty.set.isNull())
        return handleScope.escapeHandle(BooleanHandle(false, gc));
    ownProperty.set.getObject().call(reciever, std::vector<ValueHandle>(1, newValue), gc);
    return handleScope.escapeHandle(BooleanHandle(true, gc));
}

void ObjectHandle::setOwnProperty(gc::Name name,
                                  const PropertyHandle &property,
                                  GC &gc,
                                  bool putInObjectDescriptor) const
{
    HandleScope handleScope(gc);
    if(property.isAccessorDescriptor())
    {
        constexpr_assert(property.isCompleteDescriptor());
        // putInObjectDescriptor = true;
        gc.addOrChangeObjectMemberAccessorInDescriptor(Handle<gc::Name>(gc, name),
                                                       value,
                                                       property.configurable,
                                                       property.enumerable,
                                                       property.get.toObjectReference(),
                                                       property.set.toObjectReference());
    }
    else
    {
        constexpr_assert(property.isDataDescriptor());
        constexpr_assert(property.isCompleteDescriptor());
        if(putInObjectDescriptor)
        {
            gc.addOrChangeObjectMemberDataInDescriptor(Handle<gc::Name>(gc, name),
                                                       value,
                                                       property.configurable,
                                                       property.enumerable,
                                                       property.value.get(),
                                                       property.writable);
        }
        else
        {
            auto member = gc.addOrChangeObjectMemberDataInObject(Handle<gc::Name>(gc, name),
                                                                 value,
                                                                 property.configurable,
                                                                 property.enumerable,
                                                                 property.writable);
            gc::ObjectMemberIndex memberIndex = member.descriptor.getMemberIndex();
            gc.writeObject(value).getMember(memberIndex) = property.value.value.get();
        }
    }
}
}
}
