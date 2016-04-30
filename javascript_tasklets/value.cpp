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
#include <algorithm>
#include "soft_float.h"
#include <sstream>
#include <array>
#include "string.h"

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
    HandleScope handleScope(gc);
    PropertyHandle current = getOwnProperty(name, gc);
    bool extensible = ordinaryIsExtensible(gc).get().get();
    return handleScope.escapeHandle(
        BooleanHandle(validateAndApplyPropertyDescriptor(
                          ObjectOrNullHandle(*this), name, extensible, property, current, gc),
                      gc));
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

ValueHandle ObjectHandle::call(ValueHandle thisValue,
                               std::vector<ValueHandle> arguments,
                               GC &gc) const
{
    constexpr_assert(false);
#warning finish
    return ValueHandle();
}

void ObjectHandle::setOwnProperty(NameHandle name, const PropertyHandle &property, GC &gc) const
{
    setOwnProperty(gc::Name(name.get().get()), property, gc);
}

void ObjectHandle::setOwnProperty(NameHandle name,
                                  const PropertyHandle &property,
                                  GC &gc,
                                  bool putInObjectDescriptor) const
{
    setOwnProperty(gc::Name(name.get().get()), property, gc, putInObjectDescriptor);
}

void ObjectHandle::setOwnProperty(gc::Name name, const PropertyHandle &property, GC &gc) const
{
    HandleScope handleScope(gc);
    if(property.isAccessorDescriptor())
    {
        constexpr_assert(property.isCompleteDescriptor());
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
        if(!hasOwnProperty(name, gc).get().get())
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

ObjectOrNullHandle ObjectHandle::getMethod(NameHandle name, GC &gc) const
{
    ValueHandle retval = this->getValue(name, *this, gc);
    if(retval.isNull() || retval.isUndefined())
        return ObjectOrNullHandle(nullptr);
    if(!retval.isObject() || !retval.getObject().isCallable(gc))
        throwTypeError(u"Property is not a function", gc);
    return retval.getObject();
}

ObjectHandle ObjectHandle::enumerate(GC &gc) const
{
#warning finish
    constexpr_assert(false);
    return ObjectHandle();
}

std::vector<NameHandle> ObjectHandle::ownPropertyKeys(GC &gc) const
{
#warning finish
    constexpr_assert(false);
    return std::vector<NameHandle>();
}

std::vector<NameHandle> ObjectHandle::ordinaryOwnPropertyKeys(GC &gc) const
{
#warning finish
    constexpr_assert(false);
    return std::vector<NameHandle>();
}

bool ObjectHandle::isCallable(GC &gc) const
{
#warning finish
    constexpr_assert(false);
    return false;
}

ObjectHandle ObjectHandle::construct(std::vector<ValueHandle> arguments,
                                     ObjectHandle newTarget,
                                     GC &gc) const
{
#warning finish
    constexpr_assert(false);
    return ObjectHandle();
}

bool ObjectHandle::isConstructable(GC &gc) const
{
#warning finish
    constexpr_assert(false);
    return false;
}

void ObjectHandle::throwTypeError(StringHandle message, GC &gc)
{
#warning finish
    constexpr_assert(false);
}

ObjectHandle ObjectHandle::getObjectPrototype(GC &gc)
{
    HandleScope handleScope(gc);
    struct ObjectPrototypeTag final
    {
    };
    ValueHandle objectPrototype = gc.getGlobalValue<ObjectPrototypeTag>();
    if(objectPrototype.isUndefined())
    {
        objectPrototype = create(NullHandle(), gc);
        gc.setGlobalValue<ObjectPrototypeTag>(objectPrototype.get());
#warning add Object.prototype members
    }
    return handleScope.escapeHandle(objectPrototype.getObject());
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

struct ObjectHandle::FunctionPrototypeCode final : public vm::Code
{
    virtual ValueHandle run(ArrayRef<const ValueHandle> arguments, GC &gc) const override
    {
        return UndefinedHandle();
    }
};

ObjectHandle ObjectHandle::getFunctionPrototype(GC &gc)
{
#if 0
    HandleScope handleScope(gc);
    struct FunctionPrototypeTag final
    {
    };
    ValueHandle functionPrototype = gc.getGlobalValue<FunctionPrototypeTag>();
    if(functionPrototype.isUndefined())
    {
        functionPrototype = createFunction(std::unique_ptr<vm::Code>(new FunctionPrototypeCode),
                                           0,
                                           gc.internString(u""),
                                           getObjectPrototype(gc),
                                           NullHandle(),
                                           LexicalEnvironmentHandle(nullptr),
                                           FunctionKind::NonConstructor,
                                           ConstructorKind::Base,
                                           ThisMode::Strict,
                                           true,
                                           NullHandle(),
                                           gc);
        gc.setGlobalValue<FunctionPrototypeTag>(functionPrototype.get());
#warning add Function.prototype members
    }
    return handleScope.escapeHandle(functionPrototype.getObject());
#else
#warning finish
    constexpr_assert(false);
    return ObjectHandle();
#endif
}

ObjectHandle ObjectHandle::createFunction(std::unique_ptr<vm::Code> code,
                                          std::uint32_t length,
                                          const StringHandle &name,
                                          const ObjectOrNullHandle &prototype,
                                          const ObjectOrNullHandle &constructorPrototype,
                                          const LexicalEnvironmentHandle &environment,
                                          FunctionKind functionKind,
                                          ConstructorKind constructorKind,
                                          ThisMode thisMode,
                                          bool strict,
                                          const ObjectOrNullHandle &homeObject,
                                          GC &gc)
{
#warning finish
    constexpr_assert(false);
    return ObjectHandle();
}

String Int32Handle::toStringValue(std::int32_t value, unsigned base)
{
    if(value < 0)
        return u"-" + UInt32Handle::toStringValue(-value, base);
    return UInt32Handle::toStringValue(value, base);
}

String UInt32Handle::toStringValue(std::uint32_t value, unsigned base)
{
    constexpr_assert(base >= 2 && base <= 36);
    String retval;
    retval.reserve(32); // max number of digits is base 2 with 32 digits
    do
    {
        retval += StringHandle::getDigitCharacter(value % base);
        value /= base;
    } while(value != 0);
    std::reverse(retval.begin(), retval.end());
    return retval;
}

namespace
{
constexpr std::array<soft_float::ExtendedFloat, 37> makeBase2Logs() noexcept
{
    return std::array<soft_float::ExtendedFloat, 37>{
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(0))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(1))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(2))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(3))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(4))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(5))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(6))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(7))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(8))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(9))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(10))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(11))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(12))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(13))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(14))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(15))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(16))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(17))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(18))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(19))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(20))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(21))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(22))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(23))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(24))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(25))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(26))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(27))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(28))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(29))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(30))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(31))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(32))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(33))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(34))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(35))),
        log2(soft_float::ExtendedFloat(static_cast<std::uint64_t>(36))),
    };
}
std::string toStringValueHelper(std::uint64_t value, unsigned base)
{
    constexpr_assert(base >= 2 && base <= 36);
    std::string retval;
    retval.reserve(32); // max number of digits is base 2 with 32 digits
    do
    {
        retval += static_cast<char>(StringHandle::getDigitCharacter(value % base));
        value /= base;
    } while(value != 0);
    std::reverse(retval.begin(), retval.end());
    return retval;
}
}

String DoubleHandle::toStringValue(double valueIn, unsigned base)
{
    constexpr_assert(base >= 2 && base <= 36);
    const char exponentChar = base == 10 ? 'e' : base == 16 ? 'h' : base == 8 ? 'o' : 'E';
    soft_float::ExtendedFloat value(valueIn), baseF(static_cast<std::uint64_t>(base));
    auto invBaseF = soft_float::ExtendedFloat::One() / baseF;
    static constexpr auto base2Logs = makeBase2Logs();
    auto limit21 =
        static_cast<std::int64_t>(round(soft_float::ExtendedFloat(static_cast<std::uint64_t>(21))
                                        * (base2Logs[10] / base2Logs[base])));
    constexpr_assert(limit21 > 0);
    auto limit6 =
        static_cast<std::int64_t>(round(soft_float::ExtendedFloat(static_cast<std::uint64_t>(6))
                                        * (base2Logs[10] / base2Logs[base])));
    constexpr_assert(limit6 > 0);
    if(value.isNaN())
        return u"NaN";
    if(value.isZero())
        return u"0";
    std::ostringstream ss;
    if(value.isInfinite())
    {
        if(value.signBit())
            return u"-Infinity";
        return u"Infinity";
    }
    if(value.signBit())
    {
        ss << "-";
        value = -value;
        valueIn = -valueIn;
    }
    auto nF = log2(value) / base2Logs[base] + soft_float::ExtendedFloat::One();
    auto n = static_cast<std::int64_t>(floor(nF));
    soft_float::ExtendedFloat baseToThePowerOfN = pow(baseF, n);
    soft_float::ExtendedFloat baseToThePowerOfMinusN =
        soft_float::ExtendedFloat::One() / baseToThePowerOfN;
    auto scaledValue = value * baseToThePowerOfMinusN;
    if(scaledValue + scalbn(soft_float::ExtendedFloat::One(), -62)
       < invBaseF) // extra is to handle round-off error
    {
        n--;
        baseToThePowerOfN *= invBaseF;
        baseToThePowerOfMinusN *= baseF;
        scaledValue = value * baseToThePowerOfMinusN;
    }
    else if(scaledValue >= soft_float::ExtendedFloat::One())
    {
        n++;
        baseToThePowerOfN *= baseF;
        baseToThePowerOfMinusN *= invBaseF;
        scaledValue = value * baseToThePowerOfMinusN;
    }
    std::int64_t k = 0;
    soft_float::ExtendedFloat sF = soft_float::ExtendedFloat::One();
    auto baseToThePowerOfK = soft_float::ExtendedFloat::One();
    auto baseToThePowerOfMinusK = soft_float::ExtendedFloat::One();
    while(sF < soft_float::ExtendedFloat::TwoToThe64())
    {
        k++;
        baseToThePowerOfK *= baseF;
        baseToThePowerOfMinusK *= invBaseF;
        sF = round(scaledValue * baseToThePowerOfK);
        if(valueIn == static_cast<double>(sF * baseToThePowerOfMinusK * baseToThePowerOfN))
            break;
    }
    std::uint64_t s = static_cast<std::uint64_t>(sF);
    std::string sDigits = toStringValueHelper(s, base);
    constexpr_assert(sDigits.size() == static_cast<std::uint64_t>(k));
    if(k <= n && n <= limit21)
    {
        ss << sDigits;
        for(std::size_t i = n - k; i > 0; i--)
            ss << '0';
    }
    else if(0 < n && n <= limit21)
    {
        for(std::int64_t i = 0; i < n; i++)
            ss << sDigits[i];
        ss << '.';
        for(std::int64_t i = n; i < k; i++)
            ss << sDigits[i];
    }
    else if(-limit6 < n && n <= 0)
    {
        ss << "0.";
        for(std::size_t i = -n; i > 0; i--)
            ss << '0';
        ss << sDigits;
    }
    else if(k == 1)
    {
        ss << sDigits << exponentChar;
        if(n - 1 >= 0)
            ss << '+' << (n - 1);
        else
            ss << (n - 1);
    }
    else
    {
        ss << sDigits[0] << '.';
        for(std::int64_t i = 1; i < k; i++)
            ss << sDigits[i];
        ss << exponentChar;
        if(n - 1 >= 0)
            ss << '+' << (n - 1);
        else
            ss << (n - 1);
    }
    return string_cast<String>(ss.str());
}

double StringHandle::toNumberValue(const String &str) noexcept
{
    const auto whiteSpace =
        u"\t"
        "\v"
        "\f"
        " "
        "\u2028"
        "\u2029"
        "\u00A0"
        "\u1680"
        "\u2000"
        "\u2001"
        "\u2002"
        "\u2003"
        "\u2004"
        "\u2005"
        "\u2006"
        "\u2007"
        "\u2008"
        "\u2009"
        "\u200A"
        "\u202F"
        "\u205F"
        "\u3000"
        "\uFEFF";
    std::size_t firstNonSpace = str.find_first_not_of(whiteSpace);
    if(firstNonSpace == String::npos)
        return 0;
    std::size_t lastNonSpace = str.find_last_not_of(whiteSpace);
    constexpr_assert(lastNonSpace != String::npos && lastNonSpace >= firstNonSpace);
    if(str.compare(firstNonSpace, lastNonSpace - firstNonSpace + 1, u"Infinity") == 0
       || str.compare(firstNonSpace, lastNonSpace - firstNonSpace + 1, u"+Infinity") == 0)
    {
        return std::numeric_limits<double>::infinity();
    }
    if(str.compare(firstNonSpace, lastNonSpace - firstNonSpace + 1, u"-Infinity") == 0)
    {
        return -std::numeric_limits<double>::infinity();
    }
    std::size_t currentPosition = firstNonSpace;
    if(str[currentPosition] == u'0')
    {
        if(currentPosition == lastNonSpace)
            return 0;
        currentPosition++;
        if(str[currentPosition] == u'b' || str[currentPosition] == u'B')
        {
            if(currentPosition == lastNonSpace)
                return std::numeric_limits<double>::quiet_NaN();
            currentPosition++;
            if(str[currentPosition] != u'0' && str[currentPosition] != u'1')
                return std::numeric_limits<double>::quiet_NaN();
            auto retval = str[currentPosition] == u'1' ? soft_float::ExtendedFloat::One() :
                                                         soft_float::ExtendedFloat::Zero();
            while(currentPosition != lastNonSpace)
            {
                currentPosition++;
                retval = scalbn(retval, 1);
                if(str[currentPosition] != u'0' && str[currentPosition] != u'1')
                    return std::numeric_limits<double>::quiet_NaN();
                if(str[currentPosition] == u'1')
                    retval += soft_float::ExtendedFloat::One();
            }
            return static_cast<double>(retval);
        }
        if(str[currentPosition] == u'o' || str[currentPosition] == u'O')
        {
            if(currentPosition == lastNonSpace)
                return std::numeric_limits<double>::quiet_NaN();
            currentPosition++;
            if(str[currentPosition] < u'0' || str[currentPosition] > u'7')
                return std::numeric_limits<double>::quiet_NaN();
            auto retval =
                soft_float::ExtendedFloat(static_cast<std::uint64_t>(str[currentPosition] - u'0'));
            while(currentPosition != lastNonSpace)
            {
                currentPosition++;
                retval = scalbn(retval, 3);
                if(str[currentPosition] < u'0' || str[currentPosition] > u'7')
                    return std::numeric_limits<double>::quiet_NaN();
                if(str[currentPosition] != u'0')
                    retval += soft_float::ExtendedFloat(
                        static_cast<std::uint64_t>(str[currentPosition] - u'0'));
            }
            return static_cast<double>(retval);
        }
        if(str[currentPosition] == u'x' || str[currentPosition] == u'X')
        {
            if(currentPosition == lastNonSpace)
                return std::numeric_limits<double>::quiet_NaN();
            currentPosition++;
            if((str[currentPosition] < u'0' || str[currentPosition] > u'9')
               && (str[currentPosition] < u'a' || str[currentPosition] > u'f')
               && (str[currentPosition] < u'A' || str[currentPosition] > u'F'))
                return std::numeric_limits<double>::quiet_NaN();
            std::uint64_t digit;
            if(str[currentPosition] >= u'0' && str[currentPosition] <= u'9')
                digit = str[currentPosition] - u'0';
            else if(str[currentPosition] >= u'A' && str[currentPosition] <= u'F')
                digit = str[currentPosition] - u'A' + 0xA;
            else
                digit = str[currentPosition] - u'a' + 0xA;
            auto retval = soft_float::ExtendedFloat(digit);
            while(currentPosition != lastNonSpace)
            {
                currentPosition++;
                retval = scalbn(retval, 4);
                if((str[currentPosition] < u'0' || str[currentPosition] > u'9')
                   && (str[currentPosition] < u'a' || str[currentPosition] > u'f')
                   && (str[currentPosition] < u'A' || str[currentPosition] > u'F'))
                    return std::numeric_limits<double>::quiet_NaN();
                if(str[currentPosition] != u'0')
                {
                    if(str[currentPosition] >= u'0' && str[currentPosition] <= u'9')
                        digit = str[currentPosition] - u'0';
                    else if(str[currentPosition] >= u'A' && str[currentPosition] <= u'F')
                        digit = str[currentPosition] - u'A' + 0xA;
                    else
                        digit = str[currentPosition] - u'a' + 0xA;
                    retval += soft_float::ExtendedFloat(digit);
                }
            }
            return static_cast<double>(retval);
        }
        currentPosition--;
    }
    bool negative = false;
    if(str[currentPosition] == u'-')
    {
        negative = true;
        currentPosition++;
    }
    else if(str[currentPosition] == u'+')
    {
        currentPosition++;
    }
    auto retval = soft_float::ExtendedFloat::Zero();
    bool gotDigitsBeforeFraction = false;
    while(currentPosition <= lastNonSpace && str[currentPosition] >= u'0'
          && str[currentPosition] <= u'9')
    {
        gotDigitsBeforeFraction = true;
        retval *= soft_float::ExtendedFloat(static_cast<std::uint64_t>(10));
        retval +=
            soft_float::ExtendedFloat(static_cast<std::uint64_t>(str[currentPosition] - u'0'));
        currentPosition++;
    }
    bool gotDigitsAfterFraction = false;
    if(currentPosition <= lastNonSpace && str[currentPosition] == '.')
    {
        currentPosition++;
        const auto oneTenth = soft_float::ExtendedFloat::One()
                              / soft_float::ExtendedFloat(static_cast<std::uint64_t>(10));
        auto factor = oneTenth;
        while(currentPosition <= lastNonSpace && str[currentPosition] >= u'0'
              && str[currentPosition] <= u'9')
        {
            gotDigitsAfterFraction = true;
            retval += factor * soft_float::ExtendedFloat(
                                   static_cast<std::uint64_t>(str[currentPosition] - u'0'));
            factor *= oneTenth;
            currentPosition++;
        }
    }
    if(!gotDigitsBeforeFraction && !gotDigitsAfterFraction)
        return std::numeric_limits<double>::quiet_NaN();
    if(currentPosition <= lastNonSpace
       && (str[currentPosition] == u'e' || str[currentPosition] == u'E'))
    {
        currentPosition++;
        bool exponentIsNegative = false;
        if(currentPosition > lastNonSpace)
            return std::numeric_limits<double>::quiet_NaN();
        if(str[currentPosition] == u'-')
        {
            exponentIsNegative = true;
            currentPosition++;
        }
        else if(str[currentPosition] == u'+')
        {
            currentPosition++;
        }
        if(currentPosition > lastNonSpace)
            return std::numeric_limits<double>::quiet_NaN();
        if(str[currentPosition] < u'0' || str[currentPosition] > u'9')
            return std::numeric_limits<double>::quiet_NaN();
        int exponentValue = 0;
        while(currentPosition <= lastNonSpace && str[currentPosition] >= u'0'
              && str[currentPosition] <= u'9')
        {
            if(exponentValue < 10000)
            {
                exponentValue *= 10;
                exponentValue += str[currentPosition] - u'0';
            }
            currentPosition++;
        }
        if(exponentIsNegative)
            exponentValue = -exponentValue;
        retval *= pow(soft_float::ExtendedFloat(static_cast<std::uint64_t>(10)),
                      static_cast<std::int64_t>(exponentValue));
    }
    if(negative)
        return static_cast<double>(-retval);
    return static_cast<double>(retval);
}

PrimitiveHandle ObjectHandle::ordinaryToPrimitive(ToPrimitivePreferredType preferredType,
                                                  GC &gc) const
{
    HandleScope handleScope(gc);
    const StringHandle toStringString = gc.internString(u"toString");
    const StringHandle valueOfString = gc.internString(u"valueOf");
    std::initializer_list<const StringHandle *> methodNames;
    switch(preferredType)
    {
    case ToPrimitivePreferredType::Default:
    case ToPrimitivePreferredType::Number:
        methodNames = {&valueOfString, &toStringString};
        break;
    case ToPrimitivePreferredType::String:
        methodNames = {&toStringString, &valueOfString};
        break;
    }
    for(const StringHandle *methodName : methodNames)
    {
        ValueHandle method = getValue(*methodName, *this, gc);
        if(method.isObject() && method.getObject().isCallable(gc))
        {
            ValueHandle result = method.getObject().call(*this, {}, gc);
            if(result.isPrimitive())
                return handleScope.escapeHandle(result.getPrimitive());
        }
    }
    throwTypeError(u"Cannot convert object to primitive value", gc);
    constexpr_assert(false);
    return PrimitiveHandle();
}

PrimitiveHandle ObjectHandle::toPrimitive(ToPrimitivePreferredType preferredType, GC &gc) const
{
    HandleScope handleScope(gc);
    auto exoticToPrimitive = getMethod(SymbolHandle::getToPrimitive(gc), gc);
    if(exoticToPrimitive.isNull())
        return handleScope.escapeHandle(ordinaryToPrimitive(preferredType, gc));
    ValueHandle arg;
    switch(preferredType)
    {
    case ToPrimitivePreferredType::Default:
        arg = StringHandle(gc.internString(u"default"));
        break;
    case ToPrimitivePreferredType::Number:
        arg = StringHandle(gc.internString(u"number"));
        break;
    case ToPrimitivePreferredType::String:
        arg = StringHandle(gc.internString(u"string"));
        break;
    }
    ValueHandle retval = exoticToPrimitive.getObject().call(*this, {std::move(arg)}, gc);
    if(!retval.isPrimitive())
        throwTypeError(u"Cannot convert object to primitive value", gc);
    return handleScope.escapeHandle(retval.getPrimitive());
}
}
}

#if 0
#include <cstdlib>
#include <iostream>
namespace
{
using namespace javascript_tasklets::value;
using namespace javascript_tasklets;
void mainFn()
{
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     std::numeric_limits<double>::infinity())) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     -std::numeric_limits<double>::infinity())) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     std::numeric_limits<double>::quiet_NaN())) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(0.0)) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(-0.0)) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(1.2301e300)) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(1.2301)) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(0.02301)) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(12301)) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(1234567890123456.0))
              << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(3.14159265358979323, 16))
              << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(0x1.FFFFFFFFFFFFFp100, 2))
              << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(0x1.FFFFFFFFFFFFFp400, 16))
              << std::endl;
    std::cout << std::endl;
    std::cout << string_cast<std::string>(
                     DoubleHandle::toStringValue(StringHandle::toNumberValue(""_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" Infinity "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" +Infinity "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" -Infinity "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 1e2 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 1e200000000000000000000000 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 1e-200000000000000000000000 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 1e+2 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 01e-2 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 1 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue("1.0000000000000001"_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue("1.0000000000000002"_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue("1.0000000000000003"_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue("1.0000000000000004"_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 0 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" -0 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" +0 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" +.e0 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 0x5f5E100 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 0X5F5e100 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 0b100101 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 0o9 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 0O0 "_js))) << std::endl;
    std::cout << string_cast<std::string>(DoubleHandle::toStringValue(
                     StringHandle::toNumberValue(" 0o77 "_js))) << std::endl;
}
struct Initializer final
{
    Initializer()
    {
        mainFn();
        std::exit(0);
    }
} init;
}
#endif
