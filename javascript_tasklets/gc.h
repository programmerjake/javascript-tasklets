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
#include "constexpr_assert.h"
#include "variant.h"

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

struct ObjectMemberIndex final : public BaseReference
{
    constexpr explicit ObjectMemberIndex(std::size_t index) noexcept : BaseReference(index)
    {
    }
    constexpr ObjectMemberIndex(std::nullptr_t = nullptr) noexcept : BaseReference(nullptr)
    {
    }
};

struct StringOrSymbolReference : public BaseReference
{
    constexpr explicit StringOrSymbolReference(std::size_t index) noexcept : BaseReference(index)
    {
    }
    constexpr StringOrSymbolReference(std::nullptr_t = nullptr) noexcept : BaseReference(nullptr)
    {
    }
};

struct StringReference final : public StringOrSymbolReference
{
    constexpr explicit StringReference(std::size_t index) noexcept : StringOrSymbolReference(index)
    {
    }
    constexpr StringReference(std::nullptr_t = nullptr) noexcept : StringOrSymbolReference(nullptr)
    {
    }
};

struct SymbolReference final : public StringOrSymbolReference
{
    constexpr explicit SymbolReference(std::size_t index) noexcept : StringOrSymbolReference(index)
    {
    }
    constexpr SymbolReference(std::nullptr_t = nullptr) noexcept : StringOrSymbolReference(nullptr)
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
struct hash<javascript_tasklets::gc::ObjectMemberIndex> final
    : public hash<javascript_tasklets::gc::BaseReference>
{
};

template <>
struct hash<javascript_tasklets::gc::StringOrSymbolReference>
    : public hash<javascript_tasklets::gc::BaseReference>
{
};

template <>
struct hash<javascript_tasklets::gc::StringReference> final
    : public hash<javascript_tasklets::gc::StringOrSymbolReference>
{
};

template <>
struct hash<javascript_tasklets::gc::SymbolReference> final
    : public hash<javascript_tasklets::gc::StringOrSymbolReference>
{
};
}

namespace javascript_tasklets
{
namespace gc
{
typedef variant<ObjectReference,
                double,
                std::int32_t,
                std::uint32_t,
                StringReference,
                SymbolReference,
                bool,
                std::nullptr_t> Value;

class InternalNameMaker final
{
private:
    template <typename Tag>
    struct Implementation final
    {
        static const char variable = 0;
    };

public:
    typedef const void *InternalName;
    template <typename Tag>
    static constexpr InternalName get() noexcept
    {
        return &Implementation<Tag>::variable;
    }
};

template <typename Tag>
const char InternalNameMaker::Implementation<Tag>::variable;

typedef InternalNameMaker::InternalName InternalName;

typedef variant<StringReference, SymbolReference, InternalName> Name;

struct ObjectMemberDescriptor final
{
    struct AccessorInDescriptorT final
    {
        ObjectReference getter;
        ObjectReference setter;
        constexpr AccessorInDescriptorT(ObjectReference getter, ObjectReference setter) noexcept
            : getter(getter),
              setter(setter)
        {
        }
        constexpr AccessorInDescriptorT() noexcept : getter(), setter()
        {
        }
    };
    struct Data
    {
        bool writable : 1;
        constexpr explicit Data(bool writable = false) noexcept : writable(writable)
        {
        }
    };
    struct DataInDescriptorT final : public Data
    {
        Value value;
        explicit DataInDescriptorT(Value value = Value(), bool writable = false) noexcept
            : Data(writable),
              value(value)
        {
        }
    };
    struct DataInObjectT final : public Data
    {
        ObjectMemberIndex memberIndex;
        constexpr explicit DataInObjectT(ObjectMemberIndex memberIndex = ObjectMemberIndex(),
                                         bool writable = false) noexcept : Data(writable),
                                                                           memberIndex(memberIndex)
        {
        }
    };

    bool configurable : 1;
    bool enumerable : 1;
    typedef variant<AccessorInDescriptorT, DataInObjectT, DataInDescriptorT> ValueType;
    ValueType value;
    ObjectMemberDescriptor(bool configurable,
                           bool enumerable,
                           AccessorInDescriptorT accessorInDescriptor) noexcept
        : configurable(configurable),
          enumerable(enumerable),
          value(ValueType::make<AccessorInDescriptorT>(std::move(accessorInDescriptor)))
    {
    }
    ObjectMemberDescriptor(bool configurable, bool enumerable, DataInObjectT dataInObject) noexcept
        : configurable(configurable),
          enumerable(enumerable),
          value(ValueType::make<DataInObjectT>(std::move(dataInObject)))
    {
    }
    ObjectMemberDescriptor(bool configurable,
                           bool enumerable,
                           DataInDescriptorT dataInDescriptor) noexcept
        : configurable(configurable),
          enumerable(enumerable),
          value(ValueType::make<DataInDescriptorT>(std::move(dataInDescriptor)))
    {
    }
    ObjectMemberDescriptor() noexcept : configurable(false), enumerable(false), value()
    {
    }
    static ObjectMemberDescriptor AccessorInDescriptor(bool configurable,
                                                       bool enumerable,
                                                       ObjectReference getter,
                                                       ObjectReference setter) noexcept
    {
        return ObjectMemberDescriptor(
            configurable, enumerable, AccessorInDescriptorT(getter, setter));
    }
    static ObjectMemberDescriptor DataInObject(bool configurable,
                                               bool enumerable,
                                               ObjectMemberIndex memberIndex,
                                               bool writable) noexcept
    {
        return ObjectMemberDescriptor(
            configurable, enumerable, DataInObjectT(memberIndex, writable));
    }
    static ObjectMemberDescriptor DataInDescriptor(bool configurable,
                                                   bool enumerable,
                                                   Value value,
                                                   bool writable) noexcept
    {
        return ObjectMemberDescriptor(configurable, enumerable, DataInDescriptorT(value, writable));
    }
    bool isAccessorInDescriptor() const noexcept
    {
        return value.is<AccessorInDescriptorT>();
    }
    bool isAccessor() const noexcept
    {
        return value.is<AccessorInDescriptorT>();
    }
    bool isDataInObject() const noexcept
    {
        return value.is<DataInObjectT>();
    }
    bool isEmbedded() const noexcept
    {
        return value.is<DataInObjectT>();
    }
    bool isDataInDescriptor() const noexcept
    {
        return value.is<DataInDescriptorT>();
    }
    bool isData() const noexcept
    {
        return isDataInDescriptor() || isDataInObject();
    }
    bool isConfigurable() const noexcept
    {
        return configurable;
    }
    void setConfigurable(bool value) noexcept
    {
        configurable = value;
    }
    bool isEnumerable() const noexcept
    {
        return enumerable;
    }
    void setEnumerable(bool value) noexcept
    {
        enumerable = value;
    }
    ObjectReference getGetter() const noexcept
    {
        return value.get<AccessorInDescriptorT>().getter;
    }
    void setGetter(ObjectReference newValue) noexcept
    {
        value.get<AccessorInDescriptorT>().getter = newValue;
    }
    ObjectReference getSetter() const noexcept
    {
        return value.get<AccessorInDescriptorT>().setter;
    }
    void setSetter(ObjectReference newValue) noexcept
    {
        value.get<AccessorInDescriptorT>().setter = newValue;
    }
    bool isWritable() const noexcept
    {
        return isDataInDescriptor() ? value.get<DataInDescriptorT>().writable :
                                      value.get<DataInObjectT>().writable;
    }
    void setWritable(bool newValue) noexcept
    {
        if(isDataInDescriptor())
        {
            value.get<DataInDescriptorT>().writable = newValue;
        }
        else
        {
            value.get<DataInObjectT>().writable = newValue;
        }
    }
    ObjectMemberIndex getMemberIndex() const noexcept
    {
        return value.get<DataInObjectT>().memberIndex;
    }
    void setMemberIndex(ObjectMemberIndex newValue) noexcept
    {
        value.get<DataInObjectT>().memberIndex = newValue;
    }
    Value getValueInDescriptor() const noexcept
    {
        return value.get<DataInDescriptorT>().value;
    }
    void setValueInDescriptor(Value newValue) noexcept
    {
        value.get<DataInDescriptorT>().value = newValue;
    }
};

class GC;
struct ObjectDescriptor;
typedef const ObjectDescriptor *ObjectDescriptorReference;

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
    Object(ObjectDescriptorReference objectDescriptor,
           const GC *gc,
           std::unique_ptr<ExtraData> extraData)
        : objectDescriptor(objectDescriptor), gc(gc), extraData(std::move(extraData))
    {
    }
    ~Object() = default;

private:
    ObjectDescriptorReference objectDescriptor;

public:
    const GC *const gc;
    std::unique_ptr<ExtraData> extraData;
    Value *getMembersArray() noexcept;
    const Value *getMembersArray() const noexcept;
    Value &getMember(ObjectMemberIndex memberIndex) noexcept
    {
        constexpr_assert(memberIndex.index < getMemberCount(objectDescriptor));
        return getMembersArray()[memberIndex.index];
    }
    const Value &getMember(ObjectMemberIndex memberIndex) const noexcept
    {
        constexpr_assert(memberIndex.index < getMemberCount(objectDescriptor));
        return getMembersArray()[memberIndex.index];
    }
    static std::size_t getSize(ObjectDescriptorReference objectDescriptor);
    static std::size_t getMemberCount(ObjectDescriptorReference objectDescriptor);
};

inline Value *Object::getMembersArray() noexcept
{
    return reinterpret_cast<Value *>(this + 1);
}

inline const Value *Object::getMembersArray() const noexcept
{
    return reinterpret_cast<const Value *>(this + 1);
}

template <typename T>
class Handle final
{
    friend class GC;
    static_assert(std::is_same<T, typename std::decay<T>::type>::value, "invalid type for Handle");

private:
    T value;
    constexpr explicit Handle(T value) noexcept(noexcept(T(std::move(value))))
        : value(std::move(value))
    {
    }

public:
    inline Handle(GC &gc, T value);
    template <typename T2>
    constexpr Handle(const Handle<T2> &, T value) noexcept(noexcept(T(std::move(value))))
        : value(std::move(value))
    {
    }
    template <typename T2,
              typename = typename std::enable_if<!std::is_convertible<T2, T>::value>::type>
    constexpr explicit Handle(Handle<T2> value) noexcept(noexcept(T(std::move(value.get()))))
        : value(std::move(value.get()))
    {
    }
    template <typename T2,
              typename = typename std::enable_if<std::is_convertible<T2, T>::value>::type>
    constexpr Handle(const Handle<T2> &value) noexcept(noexcept(T(value.get())))
        : value(value.get())
    {
    }
    template <typename T2,
              typename = typename std::enable_if<std::is_convertible<T2, T>::value>::type>
    constexpr Handle(Handle<T2> &&value) noexcept(noexcept(T(std::move(value.get()))))
        : value(std::move(value.get()))
    {
    }
    constexpr Handle() noexcept(noexcept(T())) : value()
    {
    }
    T &get() & noexcept
    {
        return value;
    }
    T &&get() && noexcept
    {
        return std::move(value);
    }
    const T &get() const &noexcept
    {
        return value;
    }
};

template <typename Dest, typename Src>
constexpr Handle<Dest *> static_handle_cast(Handle<Src *> src) noexcept
{
    return Handle<Dest *>(src, static_cast<Dest *>(src.get()));
}

template <typename Dest, typename Src>
Handle<Dest *> dynamic_handle_cast(Handle<Src *> src) noexcept
{
    return Handle<Dest *>(src, dynamic_cast<Dest *>(src.get()));
}

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
        const ObjectDescriptorReference parent;
        const std::size_t index;

    private:
        ObjectDescriptorInitializer(const GC *gc,
                                    ObjectDescriptorReference parent,
                                    std::size_t index)
            : gc(gc), parent(parent), index(index)
        {
        }
    };
    struct Member final
    {
        Name name;
        ObjectMemberDescriptor descriptor;
        Member() noexcept : name(), descriptor()
        {
        }
        Member(Name name, ObjectMemberDescriptor descriptor) noexcept : name(name),
                                                                        descriptor(descriptor)
        {
        }
        bool empty() const noexcept
        {
            return name.empty();
        }
    };
    const GC *const gc;
    const ObjectDescriptorReference parent;
    const std::size_t index;

private:
    std::size_t embeddedMemberCount;
    std::vector<Member> members;

public:
    explicit ObjectDescriptor(const ObjectDescriptorInitializer &initializer);
    virtual ~ObjectDescriptor() = default;
    std::size_t getEmbeddedMemberCount() const
    {
        return embeddedMemberCount;
    }
    const std::vector<Member> &getMembers() const
    {
        return members;
    }
    const Member &getMember(std::size_t index) const noexcept
    {
        constexpr_assert(index < members.size());
        return members[index];
    }
    void setMember(std::size_t index, const Member &member) noexcept
    {
        constexpr_assert(index < members.size());
        constexpr_assert(!member.empty());
        if(members[index].descriptor.isEmbedded())
            embeddedMemberCount--;
        if(member.descriptor.isEmbedded())
            embeddedMemberCount++;
        members[index] = member;
    }
    std::size_t addMember(const Member &member)
    {
        constexpr_assert(!member.empty());
        std::size_t retval = members.size();
        members.push_back(member);
        if(members.back().descriptor.isEmbedded())
        {
            members.back().descriptor.setMemberIndex(ObjectMemberIndex(embeddedMemberCount));
            embeddedMemberCount++;
        }
        return retval;
    }
    static constexpr std::size_t npos = ~static_cast<std::size_t>(0);
    std::size_t findMember(const Name &name) const noexcept
    {
        constexpr_assert(!name.empty());
        for(std::size_t index = 0; index < members.size(); index++)
        {
            if(members[index].name == name)
                return index;
        }
        return npos;
    }
    virtual Handle<ObjectDescriptor *> duplicate(Handle<ObjectDescriptorReference> self,
                                                 GC &gc) const;
};

inline std::size_t Object::getSize(ObjectDescriptorReference objectDescriptor)
{
    return sizeof(Object) + sizeof(Value) * getMemberCount(objectDescriptor);
}

inline std::size_t Object::getMemberCount(ObjectDescriptorReference objectDescriptor)
{
    constexpr_assert(objectDescriptor);
    return objectDescriptor->getEmbeddedMemberCount();
}

class HandleScope final
{
    friend class GC;
    template <typename T>
    friend class Handle;
    HandleScope(const HandleScope &rt) = delete;
    HandleScope &operator=(const HandleScope &rt) = delete;
    void *operator new(std::size_t) = delete;

private:
    HandleScope *parent = nullptr;
    GC &gc;
    ObjectReference *objectReferences = nullptr;
    StringOrSymbolReference *stringOrSymbolReferences = nullptr;
    ObjectDescriptorReference *objectDescriptorReferences = nullptr;
    static constexpr std::size_t embeddedHandleCount = 7, initialDynamicAllocationSize = 32;
    std::size_t objectReferenceCount = 0;
    std::size_t stringOrSymbolReferenceCount = 0;
    std::size_t objectDescriptorReferenceCount = 0;
    std::size_t objectReferencesAllocated = embeddedHandleCount;
    std::size_t stringOrSymbolReferencesAllocated = embeddedHandleCount;
    std::size_t objectDescriptorReferencesAllocated = embeddedHandleCount;
    ObjectReference embeddedObjectReferences[embeddedHandleCount];
    StringOrSymbolReference embeddedStringOrSymbolReferences[embeddedHandleCount];
    ObjectDescriptorReference embeddedObjectDescriptorReferences[embeddedHandleCount];
    inline void addToGC();
    inline void removeFromGC();
    void init()
    {
        addToGC();
    }
    template <typename T>
    static void expandArrayImp(std::size_t &allocated, T *&array, std::size_t count);
    void expandObjectReferences();
    void expandStringOrSymbolReferences();
    void expandObjectDescriptorReferences();
    void addReference(ObjectReference reference)
    {
        constexpr_assert(reference != nullptr);
        if(objectReferenceCount < embeddedHandleCount)
        {
            embeddedObjectReferences[objectReferenceCount++] = reference;
        }
        else
        {
            if(objectReferenceCount >= objectReferencesAllocated)
                expandObjectReferences();
            objectReferences[objectReferenceCount++ - embeddedHandleCount] = reference;
        }
    }
    void addReference(StringOrSymbolReference reference)
    {
        constexpr_assert(reference != nullptr);
        if(stringOrSymbolReferenceCount < embeddedHandleCount)
        {
            embeddedStringOrSymbolReferences[stringOrSymbolReferenceCount++] = reference;
        }
        else
        {
            if(stringOrSymbolReferenceCount >= stringOrSymbolReferencesAllocated)
                expandStringOrSymbolReferences();
            stringOrSymbolReferences[stringOrSymbolReferenceCount++ - embeddedHandleCount] =
                reference;
        }
    }
    void addReference(ObjectDescriptorReference reference)
    {
        constexpr_assert(reference != nullptr);
        if(objectDescriptorReferenceCount < embeddedHandleCount)
        {
            embeddedObjectDescriptorReferences[objectDescriptorReferenceCount++] = reference;
        }
        else
        {
            if(objectDescriptorReferenceCount >= objectDescriptorReferencesAllocated)
                expandObjectDescriptorReferences();
            objectDescriptorReferences[objectDescriptorReferenceCount++ - embeddedHandleCount] =
                reference;
        }
    }
    void addHandle(const Handle<ObjectReference> &handle)
    {
        if(handle.get() == nullptr)
            return;
        addReference(handle.get());
    }
    void addHandle(const Handle<StringReference> &handle)
    {
        if(handle.get() == nullptr)
            return;
        addReference(handle.get());
    }
    void addHandle(const Handle<SymbolReference> &handle)
    {
        if(handle.get() == nullptr)
            return;
        addReference(handle.get());
    }
    void addHandle(const Handle<ObjectDescriptorReference> &handle);
    void addHandle(const Handle<ObjectDescriptor *> &handle)
    {
        addHandle(Handle<ObjectDescriptorReference>(handle));
    }
    template <typename T>
    void addHandle(const Handle<T> &) = delete;
    template <typename T>
    void addReference(T) = delete;
    void addReference(std::int32_t)
    {
    }
    void addReference(std::uint32_t)
    {
    }
    void addReference(std::nullptr_t)
    {
    }
    void addReference(bool)
    {
    }
    void addReference(double)
    {
    }
    void addReference(const StringReference &reference)
    {
        addReference(static_cast<const StringOrSymbolReference &>(reference));
    }
    void addReference(const SymbolReference &reference)
    {
        addReference(static_cast<const StringOrSymbolReference &>(reference));
    }
    struct ReferenceAdder final
    {
        HandleScope &scope;
        explicit ReferenceAdder(HandleScope &scope) : scope(scope)
        {
        }
        void operator()()
        {
        }
        template <typename T>
        void operator()(T &&v)
        {
            scope.addReference(std::forward<T>(v));
        }
    };
    template <typename... Types>
    void addHandle(const Handle<variant<Types...>> &value)
    {
        value.get().apply(ReferenceAdder(*this));
    }
    template <typename... Types>
    void addReference(const variant<Types...> &value)
    {
        value.apply(ReferenceAdder(*this));
    }

public:
    explicit HandleScope(GC &gc)
        : gc(gc),
          embeddedObjectReferences(),
          embeddedStringOrSymbolReferences(),
          embeddedObjectDescriptorReferences()
    {
        init();
    }
    ~HandleScope()
    {
        removeFromGC();
        delete[] objectReferences;
        delete[] stringOrSymbolReferences;
        delete[] objectDescriptorReferences;
    }
    template <typename T>
    Handle<T> escapeHandle(const Handle<T> &handle)
    {
        constexpr_assert(parent);
        parent->addHandle(handle);
        return handle;
    }
};

class GC final : public std::enable_shared_from_this<GC>
{
    friend class HandleScope;
    template <typename T>
    friend class Handle;
    GC &operator=(const GC &) = delete;
    GC(const GC &) = delete;

private:
    struct ObjectDescriptorTransition final
    {
        Name name;
        ObjectDescriptorReference sourceDescriptor;
        ObjectDescriptorReference
            destDescriptor; // destDescriptor is ignored for hash and equality comparison
        ObjectDescriptorTransition(Name name,
                                   ObjectDescriptorReference sourceDescriptor,
                                   ObjectDescriptorReference destDescriptor = nullptr) noexcept
            : name(name),
              sourceDescriptor(sourceDescriptor),
              destDescriptor(destDescriptor)
        {
        }
        std::size_t hash() const noexcept
        {
            return std::hash<Name>()(name)
                   + 3923 * std::hash<ObjectDescriptorReference>()(sourceDescriptor);
        }
        bool operator==(const ObjectDescriptorTransition &rt) const noexcept
        {
            return name == rt.name && sourceDescriptor == rt.sourceDescriptor;
        }
        bool operator!=(const ObjectDescriptorTransition &rt) const noexcept
        {
            return !operator==(rt);
        }
    };
    struct CheckTransition;
    struct ValueMarker;
    struct ObjectMemberDescriptorMarker;

private:
    std::vector<Object *> objects, oldObjects;
    std::vector<std::size_t> freeObjectIndexesList;
    std::vector<ObjectReference> objectsWorklist;
    std::vector<String *> strings;
    std::vector<String *> oldStrings;
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
    std::vector<std::forward_list<ObjectDescriptorTransition>> objectDescriptorTransitions;
    std::unordered_multimap<std::size_t, StringReference> stringHashToStringReferenceMap;
    std::unordered_map<const void *, Value> globalValuesMap;

private:
    ObjectReference allocateObjectIndex();
    void freeObjectIndex(ObjectReference object) noexcept;
    std::size_t allocateStringIndex();
    void freeStringIndex(std::size_t stringIndex) noexcept;
    std::size_t allocateObjectDescriptorIndex();
    void freeObjectDescriptorIndex(std::size_t objectDescriptorIndex) noexcept;
    Object *objectCopyOnWrite(ObjectReference objectReference);
    Object *createInternal(ObjectDescriptorReference objectDescriptor,
                           std::unique_ptr<Object::ExtraData> extraData);
    void freeInternal(Object *object) noexcept;
    void collectorMarkObject(ObjectReference object) noexcept;
    void collectorMarkStringOrSymbol(StringOrSymbolReference stringOrSymbol) noexcept;
    void collectorMarkObjectDescriptor(ObjectDescriptorReference objectDescriptor) noexcept;
    void collectorMarkName(const Name &value) noexcept;
    void collectorMarkValue(const Value &value) noexcept;
    void collectorMarkObjectDescriptorMember(const ObjectDescriptor::Member &value) noexcept;
    void collectorMarkObjectMemberDescriptor(const ObjectMemberDescriptor &descriptor) noexcept;
    void collectorScanObject(ObjectReference objectReference) noexcept;
    void collectorScanObjectDescriptor(ObjectDescriptor *objectDescriptor) noexcept;
    Handle<StringReference> internStringHelper(const String &value, std::size_t valueHash) noexcept;
    ObjectDescriptorTransition &findOrAddObjectDescriptorTransition(
        std::vector<std::forward_list<ObjectDescriptorTransition>> &transitions,
        Name name,
        ObjectDescriptorReference sourceDescriptor);
    template <typename Tag>
    static const void *getGlobalValueMapKey() noexcept
    {
        static char key;
        return &key;
    }
    Handle<Value> getGlobalValue(const void *key);
    void setGlobalValue(const void *key, Handle<Value> value);
    ObjectDescriptor::Member addObjectMember(Handle<Name> nameHandle,
                                             Handle<ObjectReference> object,
                                             ObjectMemberDescriptor memberDescriptor);

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
    const Object &readObject(Handle<ObjectReference> handle) const noexcept
    {
        constexpr_assert(handle.get().index < objects.size());
        const Object *retval = objects[handle.get().index];
        constexpr_assert(retval);
        return *retval;
    }
    Object &writeObject(Handle<ObjectReference> handle)
    {
        constexpr_assert(!immutable);
        constexpr_assert(handle.get().index < objects.size());
        Object *&retval = objects[handle.get().index];
        constexpr_assert(retval);
        if(retval->gc == this)
            return *retval;
        return *objectCopyOnWrite(handle.get());
    }
    Handle<ObjectDescriptorReference> getObjectDescriptor(Handle<ObjectReference> handle) const
        noexcept
    {
        return Handle<ObjectDescriptorReference>(readObject(handle).objectDescriptor);
    }
    const String &readString(Handle<StringReference> handle) const noexcept
    {
        constexpr_assert(handle.get().index < strings.size());
        const String *retval = strings[handle.get().index];
        constexpr_assert(retval);
        return *retval;
    }
    const String &readSymbol(Handle<SymbolReference> handle) const noexcept
    {
        constexpr_assert(handle.get().index < strings.size());
        const String *retval = strings[handle.get().index];
        constexpr_assert(retval);
        return *retval;
    }
    Handle<StringReference> internString(const String &value);
    Handle<StringReference> internString(String &&value);
    Handle<SymbolReference> createSymbol(String description);
    Handle<ObjectReference> create(Handle<ObjectDescriptorReference> objectDescriptor,
                                   std::unique_ptr<Object::ExtraData> extraData = nullptr);
    template <typename T = ObjectDescriptor, typename... Args>
    Handle<T *> createObjectDescriptor(Handle<const T *> parentObjectDescriptor, Args &&... args)
    {
        static_assert(std::is_base_of<ObjectDescriptor, T>::value,
                      "T is not derived from ObjectDescriptor");
        constexpr_assert(parentObjectDescriptor.get() == nullptr
                         || typeid(*parentObjectDescriptor.get()) == typeid(T));
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
        return Handle<T *>(*this, retval);
    }
    Handle<ObjectDescriptor *> createObjectDescriptor()
    {
        return createObjectDescriptor(Handle<ObjectDescriptorReference>());
    }
    template <typename Tag>
    Handle<Value> getGlobalValue()
    {
        return getGlobalValue(getGlobalValueMapKey<Tag>());
    }
    template <typename Tag>
    void setGlobalValue(Handle<Value> value)
    {
        setGlobalValue(getGlobalValueMapKey<Tag>(), std::move(value));
    }
    ObjectDescriptor::Member addObjectMemberAccessorInDescriptor(Handle<Name> nameHandle,
                                                                 Handle<ObjectReference> object,
                                                                 bool configurable,
                                                                 bool enumerable,
                                                                 Handle<ObjectReference> getter,
                                                                 Handle<ObjectReference> setter)
    {
        return addObjectMember(nameHandle,
                               object,
                               ObjectMemberDescriptor::AccessorInDescriptor(
                                   configurable, enumerable, getter.get(), setter.get()));
    }
    ObjectDescriptor::Member addObjectMemberDataInDescriptor(Handle<Name> nameHandle,
                                                                 Handle<ObjectReference> object,
                                                                 bool configurable,
                                                                 bool enumerable,
                                                                 Handle<Value> value,
                                                                 bool writable)
    {
        return addObjectMember(nameHandle,
                               object,
                               ObjectMemberDescriptor::DataInDescriptor(
                                   configurable, enumerable, value.get(), writable));
    }
    ObjectDescriptor::Member addObjectMemberDataInObject(Handle<Name> nameHandle,
                                                                 Handle<ObjectReference> object,
                                                                 bool configurable,
                                                                 bool enumerable,
                                                                 bool writable)
    {
        return addObjectMember(nameHandle,
                               object,
                               ObjectMemberDescriptor::DataInObject(
                                   configurable, enumerable, ObjectMemberIndex(), writable));
    }
};

template <typename T>
inline Handle<T>::Handle(GC &gc, T value)
    : value(std::move(value))
{
    constexpr_assert(gc.handleScopesStack);
    gc.handleScopesStack->addHandle(*this);
}
inline void HandleScope::addToGC()
{
    constexpr_assert(!parent);
    parent = gc.handleScopesStack;
    gc.handleScopesStack = this;
}

inline void HandleScope::removeFromGC()
{
    constexpr_assert(this == gc.handleScopesStack);
    gc.handleScopesStack = parent;
    parent = nullptr;
}

inline void HandleScope::addHandle(const Handle<ObjectDescriptorReference> &handle)
{
    if(handle.get() != nullptr && handle.get()->gc == &gc)
    {
        addReference(handle.get());
    }
}
}
}

#endif /* JAVASCRIPT_TASKLETS_GC_H_ */
