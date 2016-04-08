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
      members()
{
}

Object *GC::objectCopyOnWrite(ObjectReference objectReference)
{
    constexpr_assert(!immutable);
    constexpr_assert(objectReference.index < objects.size());
    constexpr_assert(parent);
    const Object *oldObject = objects[objectReference.index];
    constexpr_assert(oldObject);
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
#if 1
#warning GC always collects
      startingAllocationsLeftTillNextCollect(0), // always collect
      startingMemoryLeftTillNextCollect(0),
#else
      startingAllocationsLeftTillNextCollect(1 << 12),
      startingMemoryLeftTillNextCollect(1 << 16),
#endif
      allocationsLeftTillNextCollect(startingAllocationsLeftTillNextCollect),
      memoryLeftTillNextCollect(startingMemoryLeftTillNextCollect)
{
    if(parent)
    {
        constexpr_assert(parent->immutable);
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
        globalValuesMap = parent->globalValuesMap;
    }
    else
    {
        const std::size_t transitionsHashTableSize = 8191;
        objectDescriptorStringTransitions.resize(transitionsHashTableSize);
        objectDescriptorSymbolTransitions.resize(transitionsHashTableSize);
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
        constexpr_assert(strings.size() >= parent->strings.size());
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
        if(objectDescriptor)
        {
            delete objectDescriptor;
        }
    }
}

void GC::collect() noexcept
{
    // setup
    constexpr_assert(!immutable);
    allocationsLeftTillNextCollect = startingAllocationsLeftTillNextCollect;
    memoryLeftTillNextCollect = startingMemoryLeftTillNextCollect;
    oldObjects.swap(objects);
    constexpr_assert(objects.capacity() >= oldObjects.size());
    objects.resize(oldObjects.size());
    for(Object *&object : objects)
        object = nullptr;
    oldStrings.swap(strings);
    constexpr_assert(strings.capacity() >= oldStrings.size());
    strings.resize(oldStrings.size());
    for(String *&string : strings)
        string = nullptr;
    oldObjectDescriptors.swap(objectDescriptors);
    constexpr_assert(objectDescriptors.capacity() >= oldObjectDescriptors.size());
    objectDescriptors.resize(oldObjectDescriptors.size());
    for(ObjectDescriptor *&objectDescriptor : objectDescriptors)
        objectDescriptor = nullptr;
    objectsWorklist.clear();
    objectDescriptorsWorklist.clear();

    // mark roots
    for(const HandleScope *handleScope = handleScopesStack; handleScope != nullptr;
        handleScope = handleScope->parent)
    {
        constexpr_assert(&handleScope->gc == this);
        for(std::size_t i = 0;
            i < handleScope->objectReferenceCount && i < HandleScope::embeddedHandleCount;
            i++)
            collectorMarkObject(handleScope->embeddedObjectReferences[i]);
        for(std::size_t i = HandleScope::embeddedHandleCount; i < handleScope->objectReferenceCount;
            i++)
            collectorMarkObject(handleScope->objectReferences[i]);

        for(std::size_t i = 0;
            i < handleScope->stringOrSymbolReferenceCount && i < HandleScope::embeddedHandleCount;
            i++)
            collectorMarkStringOrSymbol(handleScope->embeddedStringOrSymbolReferences[i]);
        for(std::size_t i = HandleScope::embeddedHandleCount;
            i < handleScope->stringOrSymbolReferenceCount;
            i++)
            collectorMarkStringOrSymbol(handleScope->stringOrSymbolReferences[i]);

        for(std::size_t i = 0;
            i < handleScope->objectDescriptorCount && i < HandleScope::embeddedHandleCount;
            i++)
            collectorMarkObjectDescriptor(handleScope->embeddedObjectDescriptors[i]);
        for(std::size_t i = HandleScope::embeddedHandleCount;
            i < handleScope->objectDescriptorCount;
            i++)
            collectorMarkObjectDescriptor(handleScope->objectDescriptors[i]);
    }
    for(const auto &globalValue : globalValuesMap)
    {
        const Value &value = std::get<1>(globalValue);
        collectorMarkValue(value);
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
                    constexpr_assert(transition.stringOrSymbol.index < strings.size());
                    if(strings[transition.stringOrSymbol.index] == nullptr)
                        return true;
                    constexpr_assert(transition.sourceDescriptor);
                    if(transition.sourceDescriptor->gc == this)
                    {
                        std::size_t index = transition.sourceDescriptor->index;
                        constexpr_assert(index < objectDescriptors.size());
                        if(objectDescriptors[index] == nullptr)
                            return true;
                    }
                    if(transition.destDescriptor)
                    {
                        if(transition.destDescriptor->gc == this)
                        {
                            std::size_t index = transition.destDescriptor->index;
                            constexpr_assert(index < objectDescriptors.size());
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
        constexpr_assert(stringReference.index < strings.size());
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
    constexpr_assert(!immutable);
    constexpr_assert(objectDescriptor);
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
    constexpr_assert(!immutable);
    constexpr_assert(object);
    constexpr_assert(object->gc == this);
    for(std::size_t i = Object::getMemberGroupCount(object->objectDescriptor); i > 0; i--)
    {
        object->getMembersArray()[i - 1].~ObjectMemberGroup();
    }
    object->~Object();
    ::operator delete(object);
}

ObjectReference GC::allocateObjectIndex()
{
    constexpr_assert(!immutable);
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
    constexpr_assert(!immutable);
    freeObjectIndexesList.push_back(object.index);
}

std::size_t GC::allocateStringIndex()
{
    constexpr_assert(!immutable);
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
    constexpr_assert(!immutable);
    freeStringsIndexesList.push_back(stringIndex);
}

std::size_t GC::allocateObjectDescriptorIndex()
{
    constexpr_assert(!immutable);
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
    constexpr_assert(!immutable);
    freeObjectDescriptorIndexesList.push_back(objectDescriptorIndex);
}

void GC::collectorMarkObject(ObjectReference object) noexcept
{
    constexpr_assert(object != nullptr);
    constexpr_assert(object.index < objects.size());
    if(objects[object.index] == nullptr)
    {
        constexpr_assert(objectsWorklist.size() < objectsWorklist.capacity());
        objectsWorklist.push_back(object);
        auto pObject = oldObjects[object.index];
        constexpr_assert(pObject);
        objects[object.index] = pObject;
    }
}

void GC::collectorMarkStringOrSymbol(StringOrSymbolReference stringOrSymbol) noexcept
{
    constexpr_assert(stringOrSymbol != nullptr);
    constexpr_assert(stringOrSymbol.index < strings.size());
    auto pString = oldStrings[stringOrSymbol.index];
    constexpr_assert(pString);
    strings[stringOrSymbol.index] = pString;
}

void GC::collectorMarkObjectDescriptor(const ObjectDescriptor *objectDescriptor) noexcept
{
    constexpr_assert(objectDescriptor != nullptr);
    if(objectDescriptor->gc == this)
    {
        const std::size_t index = objectDescriptor->index;
        constexpr_assert(index < objectDescriptors.size());
        constexpr_assert(oldObjectDescriptors[index] == objectDescriptor);
        if(objectDescriptors[index] == nullptr)
        {
            constexpr_assert(objectDescriptorsWorklist.size()
                             < objectDescriptorsWorklist.capacity());
            objectDescriptorsWorklist.push_back(index);
            objectDescriptors[index] = oldObjectDescriptors[index];
        }
    }
}

void GC::collectorMarkObjectDescriptorMember(const ObjectDescriptor::Member &member) noexcept
{
    collectorMarkName(member.name);
    collectorMarkObjectMemberDescriptor(member.descriptor);
}

void GC::collectorMarkObjectMember(Value::Type type, const TypelessValue &value) noexcept
{
    collectorMarkValue(Value(type, value));
}

void GC::collectorMarkValue(const Value &value) noexcept
{
    switch(value.getType())
    {
    case Value::Type::Object:
        collectorMarkObject(value.getObject());
        return;
    case Value::Type::Double:
    case Value::Type::Int32:
    case Value::Type::UInt32:
    case Value::Type::Boolean:
    case Value::Type::Undefined:
    case Value::Type::Null:
        return;
    case Value::Type::Symbol:
        collectorMarkStringOrSymbol(value.getSymbol());
        return;
    case Value::Type::String:
        collectorMarkStringOrSymbol(value.getString());
        return;
    }
    constexpr_assert(false);
}

void GC::collectorScanObject(ObjectReference objectReference) noexcept
{
    constexpr_assert(objectReference.index < objects.size());
    const Object *object = objects[objectReference.index];
    constexpr_assert(object);
    const ObjectDescriptor *objectDescriptor = object->objectDescriptor;
    constexpr_assert(objectDescriptor);
    collectorMarkObjectDescriptor(objectDescriptor);
    for(std::size_t embeddedMemberIndex = 0;
        embeddedMemberIndex < objectDescriptor->getEmbeddedMemberCount();
        embeddedMemberIndex++)
    {
        collectorMarkValue(object->getMemberValue(ObjectMemberIndex(embeddedMemberIndex)));
    }
}

void GC::collectorScanObjectDescriptor(ObjectDescriptor *objectDescriptor) noexcept
{
    constexpr_assert(objectDescriptor);
    constexpr_assert(objectDescriptor->gc == this);
    if(objectDescriptor->parent)
        collectorMarkObjectDescriptor(objectDescriptor->parent);
    for(const auto &member : objectDescriptor->members)
    {
        collectorMarkObjectDescriptorMember(member);
    }
}

ObjectDescriptor::Member GC::addObjectMember(StringHandle nameHandle,
                                             ObjectHandle object,
                                             ValueHandle value,
                                             bool isEmbedded)
{
    HandleScope scope(*this);
    constexpr_assert(!immutable);
    constexpr_assert(!nameHandle.empty());
    constexpr_assert(object.object.index < objects.size());
    Object *originalObject = objects[object.object.index];
    constexpr_assert(originalObject);
    ObjectDescriptorHandle<> originalObjectDescriptor(originalObject->objectDescriptor);
    ObjectDescriptorHandle<> newObjectDescriptorHandle;
    constexpr_assert(!originalObjectDescriptor.empty());
    constexpr_assert(originalObjectDescriptor.get()->findMember(Name())getNamedMember(readString(nameHandle)).kind
                     == ObjectDescriptor::Member::Kind::Empty);
    auto &transition = findOrAddObjectDescriptorStringOrSymbolTransition(
        objectDescriptorStringTransitions, nameHandle.string, originalObjectDescriptor.get());
    ObjectDescriptor::Member member;
    if(transition.destDescriptor == nullptr)
    {
        newObjectDescriptorHandle =
            transition.sourceDescriptor->duplicate(originalObjectDescriptor, *this);
        transition.destDescriptor = newObjectDescriptorHandle.get();
        auto newDescriptor = const_cast<ObjectDescriptor *>(transition.destDescriptor);
        constexpr_assert(newDescriptor);
        constexpr_assert(newDescriptor->gc == this);
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
    HandleScope scope(*this);
    constexpr_assert(!immutable);
    constexpr_assert(!nameHandle.empty());
    constexpr_assert(object.object.index < objects.size());
    Object *originalObject = objects[object.object.index];
    constexpr_assert(originalObject);
    ObjectDescriptorHandle<> originalObjectDescriptor(originalObject->objectDescriptor);
    ObjectDescriptorHandle<> newObjectDescriptorHandle;
    constexpr_assert(!originalObjectDescriptor.empty());
    constexpr_assert(originalObjectDescriptor.get()->getSymbolMember(nameHandle.symbol).kind
                     == ObjectDescriptor::Member::Kind::Empty);
    auto &transition = findOrAddObjectDescriptorStringOrSymbolTransition(
        objectDescriptorSymbolTransitions, nameHandle.symbol, originalObjectDescriptor.get());
    ObjectDescriptor::Member member;
    if(transition.destDescriptor == nullptr)
    {
        newObjectDescriptorHandle =
            transition.sourceDescriptor->duplicate(originalObjectDescriptor, *this);
        transition.destDescriptor = newObjectDescriptorHandle.get();
        auto newDescriptor = const_cast<ObjectDescriptor *>(transition.destDescriptor);
        constexpr_assert(newDescriptor);
        constexpr_assert(newDescriptor->gc == this);
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
    constexpr_assert(!immutable);
    auto iteratorRange = stringHashToStringReferenceMap.equal_range(valueHash);
    for(auto iter = std::get<0>(iteratorRange); iter != std::get<1>(iteratorRange); ++iter)
    {
        StringOrSymbolReference stringReference = std::get<1>(*iter);
        constexpr_assert(stringReference.index < strings.size());
        const String *string = strings[stringReference.index];
        constexpr_assert(string);
        if(*string == value)
        {
            return StringHandle(*this, stringReference);
        }
    }
    return StringHandle();
}

StringHandle GC::internString(const String &value)
{
    constexpr_assert(!immutable);
    std::size_t valueHash = std::hash<String>()(value);
    StringHandle retval = internStringHelper(value, valueHash);
    if(!retval.empty())
        return retval;
    std::unique_ptr<String> stringValue(new String(value));
    std::size_t index = allocateStringIndex();
    StringOrSymbolReference stringReference(index);
    strings[index] = stringValue.release();
    stringHashToStringReferenceMap.emplace(valueHash, stringReference);
    return StringHandle(*this, stringReference);
}

StringHandle GC::internString(String &&value)
{
    constexpr_assert(!immutable);
    std::size_t valueHash = std::hash<String>()(value);
    StringHandle retval = internStringHelper(value, valueHash);
    if(!retval.empty())
        return retval;
    std::unique_ptr<String> stringValue(new String(std::move(value)));
    std::size_t index = allocateStringIndex();
    StringOrSymbolReference stringReference(index);
    strings[index] = stringValue.release();
    stringHashToStringReferenceMap.emplace(valueHash, stringReference);
    return StringHandle(*this, stringReference);
}

SymbolHandle GC::createSymbol(String description)
{
    constexpr_assert(!immutable);
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
    constexpr_assert(!immutable);
    constexpr_assert(!transitions.empty());
    constexpr_assert(stringOrSymbol != nullptr);
    constexpr_assert(sourceDescriptor != nullptr);
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

ObjectHandle GC::create(ObjectDescriptorHandle<> objectDescriptor,
                        std::unique_ptr<Object::ExtraData> extraData)
{
    constexpr_assert(!immutable);
    constexpr_assert(!objectDescriptor.empty());
    ObjectReference objectReference = allocateObjectIndex();
    try
    {
        auto object = createInternal(objectDescriptor.get(), std::move(extraData));
        objects[objectReference.index] = object;
    }
    catch(...)
    {
        freeObjectIndex(objectReference);
        throw;
    }
    return ObjectHandle(*this, objectReference);
}

ObjectDescriptorHandle<> ObjectDescriptor::duplicate(ObjectDescriptorHandle<> self, GC &gc) const
{
    constexpr_assert(self.get() == this);
    constexpr_assert(typeid(*this) == typeid(ObjectDescriptor));
    return gc.createObjectDescriptor(self);
}

template <typename T>
void HandleScope::expandArrayImp(std::size_t &allocated, T *&array, std::size_t count)
{
    if(allocated < initialDynamicAllocationSize + embeddedHandleCount)
    {
        allocated = initialDynamicAllocationSize + embeddedHandleCount;
    }
    else
    {
        allocated = 2 * allocated - embeddedHandleCount;
        // equivalent to 2 * (allocated - embeddedHandleCount) + embeddedHandleCount
    }
    auto temp = new T[allocated - embeddedHandleCount];
    for(std::size_t i = 0; i < count - embeddedHandleCount; i++)
        temp[i] = array[i];
    delete[] array;
    array = temp;
}

void HandleScope::expandObjectReferences()
{
    expandArrayImp(objectReferencesAllocated, objectReferences, objectReferenceCount);
}

void HandleScope::expandStringOrSymbolReferences()
{
    expandArrayImp(
        stringOrSymbolReferencesAllocated, stringOrSymbolReferences, stringOrSymbolReferenceCount);
}

void HandleScope::expandObjectDescriptors()
{
    expandArrayImp(objectDescriptorsAllocated, objectDescriptors, objectDescriptorCount);
}

ValueHandle GC::getGlobalValue(const void *key)
{
    auto iter = globalValuesMap.find(key);
    if(iter == globalValuesMap.end())
        return ValueHandle();
    Value value = std::get<1>(*iter);
    return ValueHandle(*this, value);
}

void GC::setGlobalValue(const void *key, ValueHandle valueHandle)
{
    globalValuesMap[key] = valueHandle;
}
}
}
