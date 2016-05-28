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
#include "parser/source.h"
#include "value.h"
#include <type_traits>
#include <iostream>

namespace javascript_tasklets
{
namespace gc
{
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
    for(std::size_t i = 0; i < Object::getMemberCount(oldObject->objectDescriptor); i++)
    {
        newObject->getMember(ObjectMemberIndex(i)) = oldObject->getMember(ObjectMemberIndex(i));
    }
    objects[objectReference.index] = newObject;
    return newObject;
}

GC::GC(PrivateAccessTag, std::shared_ptr<const GC> parent)
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
      memoryLeftTillNextCollect(startingMemoryLeftTillNextCollect),
      exceptionListHead(nullptr),
      exceptionListTail(nullptr),
      locationGetterStack(nullptr)
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
        sources = parent->sources;
        oldSources.reserve(sources.capacity());
        freeSourcesIndexesList = parent->freeSourcesIndexesList;
        freeSourcesIndexesList.reserve(sources.capacity());
        objectDescriptors = parent->objectDescriptors;
        oldObjectDescriptors.reserve(objectDescriptors.capacity());
        freeObjectDescriptorIndexesList = parent->freeObjectDescriptorIndexesList;
        freeObjectDescriptorIndexesList.reserve(objectDescriptors.capacity());
        objectDescriptorsWorklist.reserve(objectDescriptors.capacity());
        stringHashToStringReferenceMap = parent->stringHashToStringReferenceMap;
        builtinSourceHashToSourceReferenceMap = parent->builtinSourceHashToSourceReferenceMap;
        objectDescriptorTransitions = parent->objectDescriptorTransitions;
        globalValuesMap = parent->globalValuesMap;
    }
    else
    {
        const std::size_t transitionsHashTableSize = 8191;
        objectDescriptorTransitions.resize(transitionsHashTableSize);
    }
}

GC::~GC()
{
    constexpr_assert(!handleScopesStack);
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
        constexpr_assert(sources.size() >= parent->sources.size());
        for(std::size_t i = 0; i < parent->sources.size(); i++)
        {
            if(parent->sources[i] == sources[i])
                sources[i] = nullptr; // parent handles destruction
        }
    }
    for(String *string : strings)
    {
        if(string)
        {
            delete string;
        }
    }
    for(parser::Source *source : sources)
    {
        if(source)
        {
            delete source;
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

struct GC::CheckTransition final
{
    GC &gc;
    bool needRemoval = false;
    explicit CheckTransition(GC &gc) : gc(gc)
    {
    }
    void operator()()
    {
    }
    template <typename T>
    void operator()(T) = delete;
    void operator()(InternalName)
    {
    }
    void operator()(std::nullptr_t)
    {
    }
    void operator()(std::uint32_t)
    {
    }
    void operator()(std::int32_t)
    {
    }
    void operator()(bool)
    {
    }
    void operator()(double)
    {
    }
    void operator()(const StringReference &name)
    {
        constexpr_assert(name.index < gc.strings.size());
        if(gc.strings[name.index] == nullptr)
            needRemoval = true;
    }
    void operator()(const SymbolReference &name)
    {
        constexpr_assert(name.index < gc.strings.size());
        if(gc.strings[name.index] == nullptr)
            needRemoval = true;
    }
    void operator()(const ObjectReference &value)
    {
        constexpr_assert(value.index < gc.objects.size());
        if(gc.objects[value.index] == nullptr)
            needRemoval = true;
    }
    void operator()(const ObjectMemberDescriptor::AccessorInDescriptorT &value)
    {
        if(value.getter != nullptr)
            (*this)(value.getter);
        if(value.setter != nullptr)
            (*this)(value.setter);
    }
    void operator()(const ObjectMemberDescriptor::DataInDescriptorT &value)
    {
        value.value.apply(*this);
    }
    void operator()(const ObjectMemberDescriptor::DataInObjectT &value)
    {
    }
    bool operator()(const ObjectDescriptorTransition &transition)
    {
        needRemoval = false;
        transition.member.name.apply(*this);
        transition.member.descriptor.value.apply(*this);
        if(needRemoval)
            return true;
        constexpr_assert(transition.sourceDescriptor);
        if(transition.sourceDescriptor->gc == &gc)
        {
            std::size_t index = transition.sourceDescriptor->index;
            constexpr_assert(index < gc.objectDescriptors.size());
            if(gc.objectDescriptors[index] == nullptr)
                return true;
        }
        if(transition.destDescriptor)
        {
            if(transition.destDescriptor->gc == &gc)
            {
                std::size_t index = transition.destDescriptor->index;
                constexpr_assert(index < gc.objectDescriptors.size());
                if(gc.objectDescriptors[index] == nullptr)
                    return true;
            }
        }
        return false;
    }
};


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
    oldSources.swap(sources);
    constexpr_assert(sources.capacity() >= oldSources.size());
    sources.resize(oldSources.size());
    for(parser::Source *&source : sources)
        source = nullptr;
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
        collectorMarkHandleScope(*handleScope);
    }
    for(const auto &globalValue : globalValuesMap)
    {
        const Value &value = std::get<1>(globalValue);
        collectorMarkValue(value);
    }
    for(ExceptionBase *exception = exceptionListHead; exception != nullptr;
        exception = exception->next)
    {
        constexpr_assert(exception->gc.get() == this);
        HandleScope handleScope(*this);
        GCReferencesCallback callback(handleScope);
        exception->getGCReferences(callback);
        collectorMarkHandleScope(handleScope);
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
    for(std::forward_list<ObjectDescriptorTransition> &transitionList : objectDescriptorTransitions)
    {
        transitionList.remove_if(CheckTransition(*this));
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
    // sweep sources
    for(auto iter = builtinSourceHashToSourceReferenceMap.begin();
        iter != builtinSourceHashToSourceReferenceMap.end();)
    {
        SourceReference sourceReference = std::get<1>(*iter);
        constexpr_assert(sourceReference.index < sources.size());
        if(sources[sourceReference.index] == nullptr)
            iter = builtinSourceHashToSourceReferenceMap.erase(iter);
        else
            ++iter;
    }
    for(std::size_t sourceIndex = 0; sourceIndex < sources.size(); sourceIndex++)
    {
        if(sources[sourceIndex] == nullptr && oldSources[sourceIndex] != nullptr)
        {
            parser::Source *source = oldSources[sourceIndex];
            if(parent)
            {
                if(sourceIndex < parent->sources.size())
                {
                    if(parent->sources[sourceIndex] == source)
                    {
                        continue; // source owned by parent
                    }
                }
            }
#if 0
#warning debugging source collection
            std::cout << "destroyed source (" << sourceIndex << "): ";
            writeString(std::cout, source->fileName);
            std::cout << std::endl;
#endif
            delete source;
            freeSourceIndex(sourceIndex);
        }
    }
}

Object *GC::createInternal(ObjectDescriptorReference objectDescriptor,
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
    static_assert(std::is_nothrow_constructible<Value>::value,
                  "Value default constructor needs to be noexcept");
    ::new(retval->getMembersArray()) Value[Object::getMemberCount(objectDescriptor)];
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

namespace
{
template <typename T>
void destructObject(T &object)
{
    object.~T();
}
}

void GC::freeInternal(Object *object) noexcept
{
    constexpr_assert(!immutable);
    constexpr_assert(object);
    constexpr_assert(object->gc == this);
    for(std::size_t i = Object::getMemberCount(object->objectDescriptor); i > 0; i--)
    {
        destructObject(object->getMembersArray()[i - 1]);
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

std::size_t GC::allocateSourceIndex()
{
    constexpr_assert(!immutable);
    if(!freeSourcesIndexesList.empty())
    {
        std::size_t retval = freeSourcesIndexesList.back();
        freeSourcesIndexesList.pop_back();
        return retval;
    }
    std::size_t retval = sources.size();
    sources.push_back(nullptr);
    freeSourcesIndexesList.reserve(sources.capacity());
    oldSources.reserve(sources.capacity());
    return retval;
}

void GC::freeSourceIndex(std::size_t sourceIndex) noexcept
{
    constexpr_assert(!immutable);
    freeSourcesIndexesList.push_back(sourceIndex);
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

void GC::collectorMarkObjectDescriptor(ObjectDescriptorReference objectDescriptor) noexcept
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

struct GC::ValueMarker final
{
    GC &gc;
    explicit ValueMarker(GC &gc) noexcept : gc(gc)
    {
    }
    void operator()() noexcept
    {
    }
    template <typename T>
    void operator()(T &&) = delete;
    void operator()(ObjectReference v) noexcept
    {
        gc.collectorMarkObject(v);
    }
    void operator()(StringReference v) noexcept
    {
        gc.collectorMarkStringOrSymbol(v);
    }
    void operator()(SymbolReference v) noexcept
    {
        gc.collectorMarkStringOrSymbol(v);
    }
    void operator()(double) noexcept
    {
    }
    void operator()(std::int32_t) noexcept
    {
    }
    void operator()(std::uint32_t) noexcept
    {
    }
    void operator()(std::nullptr_t) noexcept
    {
    }
    void operator()(bool) noexcept
    {
    }
    void operator()(InternalName) noexcept
    {
    }
};

void GC::collectorMarkValue(const Value &value) noexcept
{
    value.apply(ValueMarker(*this));
}

void GC::collectorScanObject(ObjectReference objectReference) noexcept
{
    constexpr_assert(objectReference.index < objects.size());
    const Object *object = objects[objectReference.index];
    constexpr_assert(object);
    ObjectDescriptorReference objectDescriptor = object->objectDescriptor;
    constexpr_assert(objectDescriptor);
    collectorMarkObjectDescriptor(objectDescriptor);
    for(std::size_t embeddedMemberIndex = 0;
        embeddedMemberIndex < objectDescriptor->getEmbeddedMemberCount();
        embeddedMemberIndex++)
    {
        collectorMarkValue(object->getMember(ObjectMemberIndex(embeddedMemberIndex)));
    }
    if(object->extraData)
    {
        HandleScope handleScope(*this);
        GCReferencesCallback callback(handleScope);
        object->extraData->getGCReferences(callback);
        collectorMarkHandleScope(handleScope);
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

ObjectDescriptor::Member GC::modifyObject(Handle<Name> nameHandle,
                                          Handle<ObjectReference> object,
                                          ObjectMemberDescriptor memberDescriptor)
{
    HandleScope scope(*this);
    constexpr_assert(!immutable);
    constexpr_assert(!nameHandle.get().empty());
    constexpr_assert(object.get().index < objects.size());
    Object *originalObject = objects[object.get().index];
    constexpr_assert(originalObject);
    Handle<ObjectDescriptorReference> originalObjectDescriptor(originalObject->objectDescriptor);
    Handle<ObjectDescriptor *> newObjectDescriptorHandle;
    constexpr_assert(originalObjectDescriptor.get() != nullptr);
    if(memberDescriptor.isEmbedded())
        memberDescriptor.setMemberIndex(ObjectMemberIndex());
    std::size_t memberIndex = originalObjectDescriptor.get()->findMember(nameHandle.get());
    if(memberIndex == ObjectDescriptor::npos
       && memberDescriptor.empty()) // deleting a nonexistent member
    {
        return ObjectDescriptor::Member(nameHandle.get(), memberDescriptor);
    }
    if(memberIndex != ObjectDescriptor::npos)
    {
        ObjectMemberDescriptor originalMemberDescriptor =
            originalObjectDescriptor.get()->getMember(memberIndex).descriptor;
        if(originalMemberDescriptor.isEmbedded() && memberDescriptor.isEmbedded())
            memberDescriptor.setMemberIndex(originalMemberDescriptor.getMemberIndex());
        if(originalMemberDescriptor == memberDescriptor)
            return ObjectDescriptor::Member(nameHandle.get(), memberDescriptor);
    }
    ObjectDescriptor::Member member(nameHandle.get(), memberDescriptor);
    auto &transition = findOrAddObjectDescriptorTransition(
        objectDescriptorTransitions, member, originalObjectDescriptor.get());
    if(transition.destDescriptor == nullptr)
    {
        newObjectDescriptorHandle = transition.sourceDescriptor->duplicate(
            originalObjectDescriptor, originalObjectDescriptor, *this);
        transition.destDescriptor = newObjectDescriptorHandle.get();
        auto newDescriptor = const_cast<ObjectDescriptor *>(transition.destDescriptor);
        constexpr_assert(newDescriptor);
        constexpr_assert(newDescriptor->gc == this);
        if(memberIndex == ObjectDescriptor::npos)
        {
            member = newDescriptor->getMember(newDescriptor->addMember(member));
        }
        else if(member.isDeleted())
        {
            newDescriptor->deleteMember(memberIndex);
        }
        else
        {
            newDescriptor->setMember(memberIndex, member);
            member = newDescriptor->getMember(memberIndex);
        }
    }
    if(transition.sourceDescriptor->getEmbeddedMemberCount()
           == transition.destDescriptor->getEmbeddedMemberCount()
       && originalObject->gc == this)
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
    if(memberIndex == ObjectDescriptor::npos
       || transition.sourceDescriptor->getEmbeddedMemberCount()
              == transition.destDescriptor->getEmbeddedMemberCount()) // append embedded member or
    // don't change embedded
    // members
    {
        for(std::size_t i = 0; i < originalObjectDescriptor.get()->embeddedMemberCount; i++)
        {
            newObject->getMember(ObjectMemberIndex(i)) =
                originalObject->getMember(ObjectMemberIndex(i));
        }
        if(member.descriptor.isEmbedded())
        {
            newObject->getMember(member.descriptor.getMemberIndex()) = Value();
        }
    }
    else if(member.descriptor.isEmbedded()
            && !transition.sourceDescriptor->getMember(memberIndex)
                    .descriptor.isEmbedded()) // insert embedded member
    {
        std::size_t insertLocation = member.descriptor.getMemberIndex().index;
        constexpr_assert(insertLocation
                         <= originalObjectDescriptor.get()->getEmbeddedMemberCount());
        for(std::size_t i = 0; i < insertLocation; i++)
        {
            newObject->getMember(ObjectMemberIndex(i)) =
                originalObject->getMember(ObjectMemberIndex(i));
        }
        newObject->getMember(ObjectMemberIndex(insertLocation)) = Value();
        for(std::size_t i = insertLocation;
            i < originalObjectDescriptor.get()->getEmbeddedMemberCount();
            i++)
        {
            newObject->getMember(ObjectMemberIndex(i + 1)) =
                originalObject->getMember(ObjectMemberIndex(i));
        }
    }
    else // delete embedded member
    {
        constexpr_assert(!member.descriptor.isEmbedded());
        constexpr_assert(
            transition.sourceDescriptor->getMember(memberIndex).descriptor.isEmbedded());
        std::size_t deleteLocation =
            transition.sourceDescriptor->getMember(memberIndex).descriptor.getMemberIndex().index;
        constexpr_assert(deleteLocation < originalObjectDescriptor.get()->getEmbeddedMemberCount());
        for(std::size_t i = 0; i < deleteLocation; i++)
        {
            newObject->getMember(ObjectMemberIndex(i)) =
                originalObject->getMember(ObjectMemberIndex(i));
        }
        for(std::size_t i = deleteLocation; i < transition.destDescriptor->getEmbeddedMemberCount();
            i++)
        {
            newObject->getMember(ObjectMemberIndex(i)) =
                originalObject->getMember(ObjectMemberIndex(i + 1));
        }
    }
    objects[object.get().index] = newObject;
    if(originalObject->gc == this)
        freeInternal(originalObject);
    return member;
}

Handle<StringReference> GC::internStringHelper(const String &value, std::size_t valueHash) noexcept
{
    constexpr_assert(!immutable);
    auto iteratorRange = stringHashToStringReferenceMap.equal_range(valueHash);
    for(auto iter = std::get<0>(iteratorRange); iter != std::get<1>(iteratorRange); ++iter)
    {
        StringReference stringReference = std::get<1>(*iter);
        constexpr_assert(stringReference.index < strings.size());
        const String *string = strings[stringReference.index];
        constexpr_assert(string);
        if(*string == value)
        {
            return Handle<StringReference>(*this, stringReference);
        }
    }
    return Handle<StringReference>();
}

Handle<SourceReference> GC::internBuiltinSource(String functionName)
{
    constexpr_assert(!immutable);
    String name = std::move(functionName) + u" (native)";
    std::size_t nameHash = std::hash<String>()(name);
    auto iteratorRange = builtinSourceHashToSourceReferenceMap.equal_range(nameHash);
    for(auto iter = std::get<0>(iteratorRange); iter != std::get<1>(iteratorRange); ++iter)
    {
        SourceReference sourceReference = std::get<1>(*iter);
        constexpr_assert(sourceReference.index < sources.size());
        constexpr_assert(sourceReference.ptr != nullptr);
        constexpr_assert(sourceReference.ptr == sources[sourceReference.index]);
        if(sourceReference->fileName == name)
        {
            return Handle<SourceReference>(*this, sourceReference);
        }
    }
    Handle<SourceReference> retval = createSource(std::move(name), u"");
    builtinSourceHashToSourceReferenceMap.emplace(nameHash, retval.get());
    return retval;
}

Handle<StringReference> GC::internString(const String &value)
{
    constexpr_assert(!immutable);
    std::size_t valueHash = std::hash<String>()(value);
    Handle<StringReference> retval = internStringHelper(value, valueHash);
    if(retval.get() != nullptr)
        return retval;
    std::unique_ptr<String> stringValue(new String(value));
    std::size_t index = allocateStringIndex();
    StringReference stringReference(index);
    strings[index] = stringValue.release();
    stringHashToStringReferenceMap.emplace(valueHash, stringReference);
    return Handle<StringReference>(*this, stringReference);
}

Handle<StringReference> GC::internString(String &&value)
{
    constexpr_assert(!immutable);
    std::size_t valueHash = std::hash<String>()(value);
    Handle<StringReference> retval = internStringHelper(value, valueHash);
    if(retval.get() != nullptr)
        return retval;
    std::unique_ptr<String> stringValue(new String(std::move(value)));
    std::size_t index = allocateStringIndex();
    StringReference stringReference(index);
    strings[index] = stringValue.release();
    stringHashToStringReferenceMap.emplace(valueHash, stringReference);
    return Handle<StringReference>(*this, stringReference);
}

Handle<SymbolReference> GC::createSymbol(String description)
{
    constexpr_assert(!immutable);
    std::unique_ptr<String> stringValue(new String(std::move(description)));
    std::size_t index = allocateStringIndex();
    SymbolReference symbolReference(index);
    strings[index] = stringValue.release();
    return Handle<SymbolReference>(*this, symbolReference);
}

GC::ObjectDescriptorTransition &GC::findOrAddObjectDescriptorTransition(
    std::vector<std::forward_list<ObjectDescriptorTransition>> &transitions,
    const ObjectDescriptor::Member &member,
    ObjectDescriptorReference sourceDescriptor)
{
    constexpr_assert(!immutable);
    constexpr_assert(!transitions.empty());
    constexpr_assert(!member.name.empty());
    constexpr_assert(sourceDescriptor != nullptr);
    ObjectDescriptorTransition key(member, sourceDescriptor);
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

void GC::collectorMarkHandleScope(const HandleScope &handleScope) noexcept
{
    constexpr_assert(&handleScope.gc == this);
    handleScope.objectReferences.forEach([this](ObjectReference reference)
                                         {
                                             collectorMarkObject(reference);
                                         });
    handleScope.stringOrSymbolReferences.forEach([this](StringOrSymbolReference reference)
                                                 {
                                                     collectorMarkStringOrSymbol(reference);
                                                 });
    handleScope.objectDescriptorReferences.forEach([this](ObjectDescriptorReference reference)
                                                   {
                                                       collectorMarkObjectDescriptor(reference);
                                                   });
    handleScope.sourceReferences.forEach([this](SourceReference reference)
                                         {
                                             collectorMarkSource(reference);
                                         });
}

void GC::collectorMarkSource(SourceReference source) noexcept
{
    constexpr_assert(source != nullptr);
    constexpr_assert(source.index < sources.size());
    auto pSource = oldSources[source.index];
    constexpr_assert(pSource);
    constexpr_assert(pSource == source.ptr);
    sources[source.index] = pSource;
}

Handle<SourceReference> GC::createSource(String fileName, String contents)
{
    constexpr_assert(!immutable);
    std::unique_ptr<parser::Source> sourceValue(
        new parser::Source(std::move(fileName), std::move(contents)));
    std::size_t index = allocateSourceIndex();
    SourceReference sourceReference(index, sourceValue.get());
    sources[index] = sourceValue.release();
#if 0
    std::cout << "created source (" << index << "): ";
    writeString(std::cout, sourceReference->fileName);
    std::cout << std::endl;
#warning source creation debugging enabled
#endif
    return Handle<SourceReference>(*this, sourceReference);
}

Handle<ObjectReference> GC::create(Handle<ObjectDescriptorReference> objectDescriptor,
                                   std::unique_ptr<Object::ExtraData> extraData)
{
    constexpr_assert(!immutable);
    constexpr_assert(objectDescriptor.get() != nullptr);
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
    return Handle<ObjectReference>(*this, objectReference);
}

Handle<ObjectDescriptor *> ObjectDescriptor::duplicate(Handle<ObjectDescriptorReference> self,
                                                       Handle<ObjectDescriptorReference> newParent,
                                                       GC &gc) const
{
    constexpr_assert(self.get() == this);
    constexpr_assert(typeid(*this) == typeid(ObjectDescriptor));
    auto retval = gc.createObjectDescriptor(newParent);
    retval.get()->embeddedMemberCount = embeddedMemberCount;
    retval.get()->members = members;
    return retval;
}

Handle<Value> GC::getGlobalValue(const void *key)
{
    auto iter = globalValuesMap.find(key);
    if(iter == globalValuesMap.end())
        return Handle<Value>();
    Value value = std::get<1>(*iter);
    return Handle<Value>(*this, value);
}

void GC::setGlobalValue(const void *key, Handle<Value> value)
{
    globalValuesMap[key] = value.get();
}

void GC::collectorMarkName(const Name &value) noexcept
{
    value.apply(ValueMarker(*this));
}

struct GC::ObjectMemberDescriptorMarker final
{
    GC &gc;
    ObjectMemberDescriptorMarker(GC &gc) noexcept : gc(gc)
    {
    }
    void operator()() noexcept
    {
    }
    template <typename T>
    void operator()(T) = delete;
    void operator()(const ObjectMemberDescriptor::AccessorInDescriptorT &value) noexcept
    {
        gc.collectorMarkObject(value.getter);
        gc.collectorMarkObject(value.setter);
    }
    void operator()(const ObjectMemberDescriptor::DataInDescriptorT &value) noexcept
    {
        gc.collectorMarkValue(value.value);
    }
    void operator()(const ObjectMemberDescriptor::DataInObjectT &value) noexcept
    {
    }
};

void GC::collectorMarkObjectMemberDescriptor(const ObjectMemberDescriptor &descriptor) noexcept
{
    descriptor.value.apply(ObjectMemberDescriptorMarker(*this));
}

char *ScriptException::calculateWhat(GC &gc, const Handle<Value> &value) noexcept
{
    try
    {
        HandleScope handleScope(gc);
        String message;
        try
        {
            value::PrimitiveHandle primitive =
                value::ValueHandle(value).toPrimitive(value::ToPrimitivePreferredType::String, gc);
            if(primitive.isSymbol())
                message = primitive.getSymbol().getDescriptiveString(gc);
            else
                message = gc.readString(primitive.toString(gc).get());
        }
        catch(gc::ExceptionBase &)
        {
            message = u"<toString threw exception>";
        }
        std::string convertedMessage = string_cast<std::string>(message);
        char *retval = new char[convertedMessage.size() + 1];
        for(std::size_t i = 0; i < convertedMessage.size(); i++)
            retval[i] = convertedMessage[i];
        retval[convertedMessage.size()] = '\0';
        return retval;
    }
    catch(std::exception &)
    {
        return nullptr;
    }
}
}
}
