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

ObjectDescriptor::ObjectDescriptor(const GC *gc)
    : gc(gc), embeddedMemberCount(0), namedMembers(), symbolMembers(), internalSlots()
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
{
#error finish
}

GC::~GC()
{
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
        delete string;
    }
    for(Object *object : objects)
    {

    }
#error finish
}

void GC::collect() noexcept
{
    assert(!immutable);
    allocationsLeftTillNextCollect = startingAllocationsLeftTillNextCollect;
    memoryLeftTillNextCollect = startingMemoryLeftTillNextCollect;
    oldObjects.swap(objects);
    objects.resize(oldObjects.size());
    for(Object *&object : objects)
        object = nullptr;
    oldStrings.swap(strings);
    strings.resize(oldStrings.size());
    for(String *&string : strings)
        string = nullptr;
    objectsWorklist.clear();
    for(const HandleScope *handleScope = handleScopesStack; handleScope != nullptr; handleScope = handleScope->parent)
    {
        assert(&handleScope->gc == this);
        for(ObjectReference object : handleScope->objectReferences)
        {
            assert(object.index < objects.size());
            if(objects[object.index] == nullptr)
            {
                objectsWorklist.push_back(object);
                auto pObject = oldObjects[object.index];
                assert(pObject);
                objects[object.index] = pObject;
            }
        }
        for(StringOrSymbolReference stringOrSymbol : handleScope->stringOrSymbolReferences)
        {
            assert(stringOrSymbol.index < strings.size());
            auto pString = oldStrings[stringOrSymbol.index];
            assert(pString);
            strings[stringOrSymbol.index] = pString;
        }
    }
    while(!objectsWorklist.empty())
    {
        Object *object = oldObjects[objectsWorklist.back()];
        objectsWorklist.pop_back();
        assert(object);
        const ObjectDescriptor *objectDescriptor = object->objectDescriptor;
        assert(objectDescriptor);
        for()
    }
#error finish
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

ObjectHandle GC::create(const ObjectDescriptor *objectDescriptor)
{
    assert(!immutable);
    assert(objectDescriptor);
    ObjectReference objectReference = allocateObjectIndex();
    try
    {
        Object *object = createInternal(objectDescriptor, nullptr);
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
}
}
