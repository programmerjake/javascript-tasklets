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
#include "gc.h"
#include <type_traits>

namespace javascript_tasklets
{
namespace gc
{
static_assert(std::is_trivially_destructible<ObjectMemberGroup>::value,
              "ObjectMemberGroup should be trivially destructable");

ObjectDescriptor::ObjectDescriptor(const ObjectDescriptorInitializer &initializer)
    : gc(initializer.gc),
      parent(initializer.parent),
      index(initializer.index),
      embeddedMemberCount(0),
      namedMembers(),
      symbolMembers(),
      internalSlots()
{
}

Object *GC::objectCopyOnWrite(ObjectReference objectReference)
{
    assert(!immutable);
    assert(objectReference.index < objects.size());
    assert(parent);
    const Object *oldObject = objects[objectReference.index];
    assert(oldObject);
    std::unique_ptr<Object::ExtraData> newExtraData = nullptr;
    if(oldObject->extraData)
    {
        newExtraData = oldObject->extraData->clone();
    }
    Object *newObject = createInternal(oldObject->objectDescriptor, std::move(newExtraData));
    objects[objectReference.index] = newObject;
    return newObject;
}

GC::GC(std::shared_ptr<const GC> parent)
    : objects(),
      oldObjects(),
      freeObjectIndexesList(),
      objectsWorklist(),
      strings(),
      oldStrings(),
      freeStringsIndexesList(),
      objectDescriptors(),
      oldObjectDescriptors(),
      freeObjectDescriptorIndexesList(),
      immutable(false),
      parent(parent),
      handleScopesStack(nullptr),
      startingAllocationsLeftTillNextCollect(1 << 12),
      startingMemoryLeftTillNextCollect(1 << 16),
      allocationsLeftTillNextCollect(startingAllocationsLeftTillNextCollect),
      memoryLeftTillNextCollect(startingMemoryLeftTillNextCollect)
{
    if(parent)
    {
        assert(parent->immutable);
        objects = parent->objects;
        oldObjects.reserve(objects.capacity());
        freeObjectIndexesList = parent->freeObjectIndexesList;
        freeObjectIndexesList.reserve(objects.capacity());
        objectsWorklist.reserve(objects.capacity());
        strings = parent->strings;
        oldStrings.reserve(strings.capacity());
        freeStringsIndexesList = parent->freeStringsIndexesList;
        freeStringsIndexesList.reserve(strings.capacity());
        stringHashToStringReferenceMap = parent->stringHashToStringReferenceMap;
        objectDescriptorSymbolTransitions = parent->objectDescriptorSymbolTransitions;
        objectDescriptorStringTransitions = parent->objectDescriptorStringTransitions;
    }
}

GC::~GC()
{
    for(Object *object : objects)
    {
        if(object && object->gc == this)
        {
            freeInternal(object);
        }
    }
    if(parent)
    {
        assert(strings.size() >= parent->strings.size());
        for(std::size_t i = 0; i < parent->strings.size(); i++)
        {
            if(parent->strings[i] == strings[i])
                strings[i] = nullptr; // parent handles destruction
        }
    }
    for(String *string : strings)
    {
        if(string)
        {
            delete string;
        }
    }
    for(ObjectDescriptor *objectDescriptor : objectDescriptors)
    {
        assert(objectDescriptor);
        delete objectDescriptor;
    }
}

void GC::collect() noexcept
{
    // setup
    assert(!immutable);
    allocationsLeftTillNextCollect = startingAllocationsLeftTillNextCollect;
    memoryLeftTillNextCollect = startingMemoryLeftTillNextCollect;
    oldObjects.swap(objects);
    assert(objects.capacity() >= oldObjects.size());
    objects.resize(oldObjects.size());
    for(Object *&object : objects)
        object = nullptr;
    oldStrings.swap(strings);
    assert(strings.capacity() >= oldStrings.size());
    strings.resize(oldStrings.size());
    for(String *&string : strings)
        string = nullptr;
    oldObjectDescriptors.swap(objectDescriptors);
    assert(objectDescriptors.capacity() >= oldObjectDescriptors.size());
    objectDescriptors.resize(oldObjectDescriptors.size());
    for(ObjectDescriptor *&objectDescriptor : objectDescriptors)
        objectDescriptor = nullptr;
    objectsWorklist.clear();
    objectDescriptorsWorklist.clear();

    // mark roots
    for(const HandleScope *handleScope = handleScopesStack; handleScope != nullptr;
        handleScope = handleScope->parent)
    {
        assert(&handleScope->gc == this);
        for(ObjectReference object : handleScope->objectReferences)
        {
            collectorMarkObject(object);
        }
        for(StringOrSymbolReference stringOrSymbol : handleScope->stringOrSymbolReferences)
        {
            collectorMarkStringOrSymbol(stringOrSymbol);
        }
    }

    // mark heap
    for(;;)
    {
        if(!objectsWorklist.empty())
        {
            ObjectReference object = objectsWorklist.back();
            objectsWorklist.pop_back();
            collectorScanObject(object);
        }
        else if(!objectDescriptorsWorklist.empty())
        {
            ObjectDescriptor *objectDescriptor =
                oldObjectDescriptors[objectDescriptorsWorklist.back()];
            objectDescriptorsWorklist.pop_back();
            collectorScanObjectDescriptor(objectDescriptor);
        }
        else
        {
            break;
        }
    }

    // sweep objects
    for(std::size_t objectIndex = 0; objectIndex < objects.size(); objectIndex++)
    {
        if(objects[objectIndex] == nullptr && oldObjects[objectIndex] != nullptr)
        {
            freeInternal(oldObjects[objectIndex]);
            freeObjectIndex(ObjectReference(objectIndex));
        }
    }
    // sweep object descriptor transitions
    for(std::vector<std::forward_list<ObjectDescriptorStringOrSymbolTransition>> *pTransitions :
        {&objectDescriptorSymbolTransitions, &objectDescriptorStringTransitions})
    {
        for(std::forward_list<ObjectDescriptorStringOrSymbolTransition> &transitionList :
            *pTransitions)
        {
            transitionList.remove_if(
                [this](const ObjectDescriptorStringOrSymbolTransition &transition) -> bool
                {
                    assert(transition.stringOrSymbol.index < strings.size());
                    if(strings[transition.stringOrSymbol.index] == nullptr)
                        return true;
                    assert(transition.sourceDescriptor);
                    if(transition.sourceDescriptor->gc == this)
                    {
                        std::size_t index = transition.sourceDescriptor->index;
                        assert(index < objectDescriptors.size());
                        if(objectDescriptors[index] == nullptr)
                            return true;
                    }
                    if(transition.destDescriptor)
                    {
                        if(transition.destDescriptor->gc == this)
                        {
                            std::size_t index = transition.destDescriptor->index;
                            assert(index < objectDescriptors.size());
                            if(objectDescriptors[index] == nullptr)
                                return true;
                        }
                    }
                    return false;
                });
        }
    }
    // sweep object descriptors
    for(std::size_t objectDescriptorIndex = 0; objectDescriptorIndex < objectDescriptors.size();
        objectDescriptorIndex++)
    {
        if(objectDescriptors[objectDescriptorIndex] == nullptr
           && oldObjectDescriptors[objectDescriptorIndex] != nullptr)
        {
            ObjectDescriptor *objectDescriptor = oldObjectDescriptors[objectDescriptorIndex];
            delete objectDescriptor;
            freeObjectDescriptorIndex(objectDescriptorIndex);
        }
    }
    // sweep strings
    for(auto iter = stringHashToStringReferenceMap.begin();
        iter != stringHashToStringReferenceMap.end();)
    {
        StringOrSymbolReference stringReference = std::get<1>(*iter);
        assert(stringReference.index < strings.size());
        if(strings[stringReference.index] == nullptr)
            iter = stringHashToStringReferenceMap.erase(iter);
        else
            ++iter;
    }
    for(std::size_t stringIndex = 0; stringIndex < strings.size(); stringIndex++)
    {
        if(strings[stringIndex] == nullptr && oldStrings[stringIndex] != nullptr)
        {
            String *string = oldStrings[stringIndex];
            if(parent)
            {
                if(stringIndex < parent->strings.size())
                {
                    if(parent->strings[stringIndex] == string)
                    {
                        continue; // string owned by parent
                    }
                }
            }
            delete string;
            freeStringIndex(stringIndex);
        }
    }
}

Object *GC::createInternal(const ObjectDescriptor *objectDescriptor,
                           std::unique_ptr<Object::ExtraData> extraData)
{
    assert(!immutable);
    assert(objectDescriptor);
    const std::size_t newObjectSize = Object::getSize(objectDescriptor);
    void *mem = ::operator new(newObjectSize);
    Object *retval;
    try
    {
        retval = ::new(mem) Object(objectDescriptor, this, std::move(extraData));
    }
    catch(...)
    {
        ::operator delete(mem);
        throw;
    }
    static_assert(std::is_nothrow_constructible<ObjectMemberGroup>::value,
                  "ObjectMemberGroup default constructor needs to be noexcept");
    ::new(retval->getMembersArray())
        ObjectMemberGroup[Object::getMemberGroupCount(objectDescriptor)];
    if(allocationsLeftTillNextCollect <= 0 || memoryLeftTillNextCollect <= newObjectSize)
    {
        collect();
    }
    else
    {
        allocationsLeftTillNextCollect--;
        memoryLeftTillNextCollect -= newObjectSize;
    }
    return retval;
}

void GC::freeInternal(Object *object) noexcept
{
    assert(!immutable);
    assert(object);
    assert(object->gc == this);
    for(std::size_t i = Object::getMemberGroupCount(object->objectDescriptor); i > 0; i--)
    {
        object->getMembersArray()[i - 1].~ObjectMemberGroup();
    }
    object->~Object();
    ::operator delete(object);
}

ObjectReference GC::allocateObjectIndex()
{
    assert(!immutable);
    if(!freeObjectIndexesList.empty())
    {
        ObjectReference retval(freeObjectIndexesList.back());
        freeObjectIndexesList.pop_back();
        return retval;
    }
    ObjectReference retval(objects.size());
    objects.push_back(nullptr);
    freeObjectIndexesList.reserve(objects.capacity());
    oldObjects.reserve(objects.capacity());
    objectsWorklist.reserve(objects.capacity());
    return retval;
}

void GC::freeObjectIndex(ObjectReference object) noexcept
{
    assert(!immutable);
    freeObjectIndexesList.push_back(object.index);
}

std::size_t GC::allocateStringIndex()
{
    assert(!immutable);
    if(!freeStringsIndexesList.empty())
    {
        std::size_t retval = freeStringsIndexesList.back();
        freeStringsIndexesList.pop_back();
        return retval;
    }
    std::size_t retval = strings.size();
    strings.push_back(nullptr);
    freeStringsIndexesList.reserve(strings.capacity());
    oldStrings.reserve(strings.capacity());
    return retval;
}

void GC::freeStringIndex(std::size_t stringIndex) noexcept
{
    assert(!immutable);
    freeStringsIndexesList.push_back(stringIndex);
}

std::size_t GC::allocateObjectDescriptorIndex()
{
    assert(!immutable);
    if(!freeObjectDescriptorIndexesList.empty())
    {
        std::size_t retval = freeObjectDescriptorIndexesList.back();
        freeObjectDescriptorIndexesList.pop_back();
        return retval;
    }
    std::size_t retval = objectDescriptors.size();
    objectDescriptors.push_back(nullptr);
    freeObjectDescriptorIndexesList.reserve(objectDescriptors.capacity());
    oldObjectDescriptors.reserve(objectDescriptors.capacity());
    objectDescriptorsWorklist.reserve(objectDescriptors.capacity());
    return retval;
}

void GC::freeObjectDescriptorIndex(std::size_t objectDescriptorIndex) noexcept
{
    assert(!immutable);
    freeObjectDescriptorIndexesList.push_back(objectDescriptorIndex);
}

void GC::collectorMarkObject(ObjectReference object) noexcept
{
    assert(object != nullptr);
    assert(object.index < objects.size());
    if(objects[object.index] == nullptr)
    {
        assert(objectsWorklist.size() < objectsWorklist.capacity());
        objectsWorklist.push_back(object);
        auto pObject = oldObjects[object.index];
        assert(pObject);
        objects[object.index] = pObject;
    }
}

void GC::collectorMarkStringOrSymbol(StringOrSymbolReference stringOrSymbol) noexcept
{
    assert(stringOrSymbol != nullptr);
    assert(stringOrSymbol.index < strings.size());
    auto pString = oldStrings[stringOrSymbol.index];
    assert(pString);
    strings[stringOrSymbol.index] = pString;
}

void GC::collectorMarkObjectDescriptor(const ObjectDescriptor *objectDescriptor) noexcept
{
    assert(objectDescriptor != nullptr);
    if(objectDescriptor->gc == this)
    {
        const std::size_t index = objectDescriptor->index;
        assert(index < objectDescriptors.size());
        assert(oldObjectDescriptors[index] == objectDescriptor);
        if(objectDescriptors[index] == nullptr)
        {
            assert(objectDescriptorsWorklist.size() < objectDescriptorsWorklist.capacity());
            objectDescriptorsWorklist.push_back(index);
            objectDescriptors[index] = oldObjectDescriptors[index];
        }
    }
}

void GC::collectorMarkObjectDescriptorMember(const ObjectDescriptor::Member &member) noexcept
{
    switch(member.kind)
    {
    case ObjectDescriptor::Member::Kind::Empty:
        return;
    case ObjectDescriptor::Member::Kind::Embedded:
        return;
    case ObjectDescriptor::Member::Kind::Constant:
        collectorMarkObjectMember(member.value.constantMember.type,
                                  member.value.constantMember.value);
        return;
    }
    assert(false);
}

void GC::collectorMarkObjectMember(ObjectMember::Type type, const ObjectMember &value) noexcept
{
    switch(type)
    {
    case ObjectMember::Type::Object:
        collectorMarkObject(value.objectValue);
        return;
    case ObjectMember::Type::Double:
    case ObjectMember::Type::Int32:
    case ObjectMember::Type::UInt32:
    case ObjectMember::Type::Boolean:
        return;
    case ObjectMember::Type::Symbol:
    case ObjectMember::Type::String:
        if(value.stringOrSymbolValue != nullptr)
        {
            collectorMarkStringOrSymbol(value.stringOrSymbolValue);
        }
        return;
    }
    assert(false);
}

void GC::collectorScanObject(ObjectReference objectReference) noexcept
{
    assert(objectReference.index < objects.size());
    const Object *object = objects[objectReference.index];
    assert(object);
    const ObjectDescriptor *objectDescriptor = object->objectDescriptor;
    assert(objectDescriptor);
    collectorMarkObjectDescriptor(objectDescriptor);
    for(std::size_t embeddedMemberIndex = 0;
        embeddedMemberIndex < objectDescriptor->getEmbeddedMemberCount();
        embeddedMemberIndex++)
    {
        collectorMarkObjectMember(object->getMemberType(embeddedMemberIndex),
                                  object->getMemberValue(embeddedMemberIndex));
    }
}

void GC::collectorScanObjectDescriptor(ObjectDescriptor *objectDescriptor) noexcept
{
    assert(objectDescriptor);
    assert(objectDescriptor->gc == this);
    if(objectDescriptor->parent)
        collectorMarkObjectDescriptor(objectDescriptor->parent);
    for(const auto &keyAndMember : objectDescriptor->namedMembers)
    {
        const auto &member = std::get<1>(keyAndMember);
        collectorMarkObjectDescriptorMember(member);
    }
    for(const auto &keyAndMember : objectDescriptor->symbolMembers)
    {
        const auto &member = std::get<1>(keyAndMember);
        collectorMarkObjectDescriptorMember(member);
        collectorMarkStringOrSymbol(std::get<0>(keyAndMember));
    }
    for(const auto &keyAndMember : objectDescriptor->internalSlots)
    {
        const auto &member = std::get<1>(keyAndMember);
        collectorMarkObjectDescriptorMember(member);
    }
    // don't scan transition members; they are weakly referenced
}

ObjectDescriptor::Member GC::addObjectMember(StringHandle nameHandle,
                                             ObjectHandle object,
                                             ValueHandle value,
                                             bool isEmbedded)
{
    assert(!immutable);
    assert(!nameHandle.empty());
    assert(object.object.index < objects.size());
    Object *originalObject = objects[object.object.index];
    assert(originalObject);
    ObjectDescriptorHandle<> originalObjectDescriptor(originalObject->objectDescriptor);
    assert(!originalObjectDescriptor.empty());
    assert(originalObjectDescriptor.get()->getNamedMember(readString(nameHandle)).kind
           == ObjectDescriptor::Member::Kind::Empty);
    auto &transition = findOrAddObjectDescriptorStringOrSymbolTransition(
        objectDescriptorStringTransitions, nameHandle.string, originalObjectDescriptor.get());
    ObjectDescriptor::Member member;
    if(transition.destDescriptor == nullptr)
    {
        transition.destDescriptor =
            transition.sourceDescriptor->duplicate(originalObjectDescriptor, *this).get();
        auto newDescriptor = const_cast<ObjectDescriptor *>(transition.destDescriptor);
        assert(newDescriptor);
        assert(newDescriptor->gc == this);
        if(isEmbedded)
        {
            member = ObjectDescriptor::Member::makeEmbedded(newDescriptor->embeddedMemberCount++);
        }
        else
        {
            member = ObjectDescriptor::Member::makeConstant(Value(value));
        }
        newDescriptor->namedMembers.emplace(readString(nameHandle), member);
    }
    if(!isEmbedded && originalObject->gc == this)
    {
        originalObject->objectDescriptor = transition.destDescriptor;
        return member;
    }
    std::unique_ptr<Object::ExtraData> extraData;
    if(originalObject->gc == this)
    {
        extraData = std::move(originalObject->extraData);
    }
    else
    {
        extraData = originalObject->extraData->clone();
    }
    Object *newObject = createInternal(transition.destDescriptor, std::move(extraData));
    for(std::size_t i = 0; i < originalObjectDescriptor.get()->embeddedMemberCount; i++)
    {
        newObject->getMemberType(i) = originalObject->getMemberType(i);
        newObject->getMemberValue(i) = originalObject->getMemberValue(i);
    }
    if(isEmbedded)
    {
        newObject->getMemberType(member.value.embeddedMember) = value.type;
        newObject->getMemberValue(member.value.embeddedMember) = value.value;
    }
    objects[object.object.index] = newObject;
    freeInternal(originalObject);
    return member;
}

ObjectDescriptor::Member GC::addObjectMember(SymbolHandle nameHandle,
                                             ObjectHandle object,
                                             ValueHandle value,
                                             bool isEmbedded)
{
    assert(!immutable);
    assert(!nameHandle.empty());
    assert(object.object.index < objects.size());
    Object *originalObject = objects[object.object.index];
    assert(originalObject);
    ObjectDescriptorHandle<> originalObjectDescriptor(originalObject->objectDescriptor);
    assert(!originalObjectDescriptor.empty());
    assert(originalObjectDescriptor.get()->getSymbolMember(nameHandle.symbol).kind
           == ObjectDescriptor::Member::Kind::Empty);
    auto &transition = findOrAddObjectDescriptorStringOrSymbolTransition(
        objectDescriptorSymbolTransitions, nameHandle.symbol, originalObjectDescriptor.get());
    ObjectDescriptor::Member member;
    if(transition.destDescriptor == nullptr)
    {
        transition.destDescriptor =
            transition.sourceDescriptor->duplicate(originalObjectDescriptor, *this).get();
        auto newDescriptor = const_cast<ObjectDescriptor *>(transition.destDescriptor);
        assert(newDescriptor);
        assert(newDescriptor->gc == this);
        if(isEmbedded)
        {
            member = ObjectDescriptor::Member::makeEmbedded(newDescriptor->embeddedMemberCount++);
        }
        else
        {
            member = ObjectDescriptor::Member::makeConstant(Value(value));
        }
        newDescriptor->namedMembers.emplace(readSymbol(nameHandle), member);
    }
    if(!isEmbedded && originalObject->gc == this)
    {
        originalObject->objectDescriptor = transition.destDescriptor;
        return member;
    }
    std::unique_ptr<Object::ExtraData> extraData;
    if(originalObject->gc == this)
    {
        extraData = std::move(originalObject->extraData);
    }
    else
    {
        extraData = originalObject->extraData->clone();
    }
    Object *newObject = createInternal(transition.destDescriptor, std::move(extraData));
    for(std::size_t i = 0; i < originalObjectDescriptor.get()->embeddedMemberCount; i++)
    {
        newObject->getMemberType(i) = originalObject->getMemberType(i);
        newObject->getMemberValue(i) = originalObject->getMemberValue(i);
    }
    if(isEmbedded)
    {
        newObject->getMemberType(member.value.embeddedMember) = value.type;
        newObject->getMemberValue(member.value.embeddedMember) = value.value;
    }
    objects[object.object.index] = newObject;
    freeInternal(originalObject);
    return member;
}

StringHandle GC::internStringHelper(const String &value, std::size_t valueHash) noexcept
{
    assert(!immutable);
    auto iteratorRange = stringHashToStringReferenceMap.equal_range(valueHash);
    for(auto iter = std::get<0>(iteratorRange); iter != std::get<1>(iteratorRange); ++iter)
    {
        StringOrSymbolReference stringReference = std::get<1>(*iter);
        assert(stringReference.index < strings.size());
        const String *string = strings[stringReference.index];
        assert(string);
        if(*string == value)
        {
            return StringHandle(*this, stringReference);
        }
    }
    return StringHandle();
}

StringHandle GC::internString(const String &value)
{
    assert(!immutable);
    std::size_t valueHash = std::hash<String>()(value);
    StringHandle retval = internStringHelper(value, valueHash);
    if(!retval.empty())
        return retval;
    std::unique_ptr<String> stringValue(new String(value));
    std::size_t index = allocateStringIndex();
    StringOrSymbolReference stringReference(index);
    strings[index] = stringValue.release();
    return StringHandle(*this, stringReference);
}

StringHandle GC::internString(String &&value)
{
    assert(!immutable);
    std::size_t valueHash = std::hash<String>()(value);
    StringHandle retval = internStringHelper(value, valueHash);
    if(!retval.empty())
        return retval;
    std::unique_ptr<String> stringValue(new String(std::move(value)));
    std::size_t index = allocateStringIndex();
    StringOrSymbolReference stringReference(index);
    strings[index] = stringValue.release();
    return StringHandle(*this, stringReference);
}

SymbolHandle GC::createSymbol(String description)
{
    assert(!immutable);
    std::unique_ptr<String> stringValue(new String(std::move(description)));
    std::size_t index = allocateStringIndex();
    StringOrSymbolReference stringReference(index);
    strings[index] = stringValue.release();
    return SymbolHandle(*this, stringReference);
}

GC::ObjectDescriptorStringOrSymbolTransition &GC::findOrAddObjectDescriptorStringOrSymbolTransition(
    std::vector<std::forward_list<ObjectDescriptorStringOrSymbolTransition>> &transitions,
    StringOrSymbolReference stringOrSymbol,
    const ObjectDescriptor *sourceDescriptor)
{
    assert(!immutable);
    assert(!transitions.empty());
    assert(stringOrSymbol != nullptr);
    assert(sourceDescriptor != nullptr);
    ObjectDescriptorStringOrSymbolTransition key(stringOrSymbol, sourceDescriptor);
    auto &transitionList = transitions[key.hash() % transitions.size()];
    for(auto prevIter = transitionList.before_begin(), iter = transitionList.begin();
        iter != transitionList.end();
        ++prevIter, ++iter)
    {
        if(*iter == key)
        {
            transitionList.splice_after(transitionList.before_begin(), transitionList, prevIter);
            return transitionList.front();
        }
    }
    transitionList.push_front(key);
    return transitionList.front();
}

ObjectHandle GC::create(ObjectDescriptorHandle<> objectDescriptor)
{
    assert(!immutable);
    assert(!objectDescriptor.empty());
    ObjectReference objectReference = allocateObjectIndex();
    try
    {
        Object *object = createInternal(objectDescriptor.get(), nullptr);
        try
        {
            objects.push_back(object);
        }
        catch(...)
        {
            freeInternal(object);
            throw;
        }
        return ObjectHandle(*this, objectReference);
    }
    catch(...)
    {
        freeObjectIndex(objectReference);
        throw;
    }
}

ObjectDescriptorHandle<> ObjectDescriptor::duplicate(ObjectDescriptorHandle<> self, GC &gc) const
{
    assert(self.get() == this);
    assert(typeid(*this) == typeid(ObjectDescriptor));
    return gc.createObjectDescriptor(self);
}
}
}
