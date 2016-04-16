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
namespace parser
{
class Source;
}
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

struct SourceReference final : public BaseReference
{
    const parser::Source *ptr;
    constexpr explicit SourceReference(std::size_t index, const parser::Source *ptr) noexcept
        : BaseReference(index),
          ptr(ptr)
    {
    }
    constexpr SourceReference(std::nullptr_t = nullptr) noexcept : BaseReference(nullptr),
                                                                   ptr(nullptr)
    {
    }
    const parser::Source *operator->() const noexcept
    {
        return ptr;
    }
    const parser::Source &operator*() const noexcept
    {
        return *ptr;
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
struct hash<javascript_tasklets::gc::SourceReference>
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
        static constexpr char variable = 0;
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
constexpr char InternalNameMaker::Implementation<Tag>::variable;

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
        bool operator==(const AccessorInDescriptorT &rt) const noexcept
        {
            return getter == rt.getter && setter == rt.setter;
        }
        bool operator!=(const AccessorInDescriptorT &rt) const noexcept
        {
            return !operator==(rt);
        }
        std::size_t hash() const noexcept
        {
            return std::hash<ObjectReference>()(getter)
                   + 12679U * std::hash<ObjectReference>()(setter);
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
        bool operator==(const DataInDescriptorT &rt) const noexcept
        {
            return writable == rt.writable && value == rt.value;
        }
        bool operator!=(const DataInDescriptorT &rt) const noexcept
        {
            return !operator==(rt);
        }
        std::size_t hash() const noexcept
        {
            return std::hash<bool>()(writable) + 12679U * std::hash<Value>()(value);
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
        bool operator==(const DataInObjectT &rt) const noexcept
        {
            return writable == rt.writable && memberIndex == rt.memberIndex;
        }
        bool operator!=(const DataInObjectT &rt) const noexcept
        {
            return !operator==(rt);
        }
        std::size_t hash() const noexcept
        {
            return std::hash<bool>()(writable)
                   + 12679U * std::hash<ObjectMemberIndex>()(memberIndex);
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
    static ObjectMemberDescriptor Empty() noexcept
    {
        return ObjectMemberDescriptor();
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
    bool empty() const noexcept
    {
        return value.empty();
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
    std::size_t hash() const noexcept;
    bool operator==(const ObjectMemberDescriptor &rt) const noexcept
    {
        return configurable == rt.configurable && enumerable == rt.enumerable && value == rt.value;
    }
    bool operator!=(const ObjectMemberDescriptor &rt) const noexcept
    {
        return !operator==(rt);
    }
};
}
}

namespace std
{
template <>
struct hash<javascript_tasklets::gc::ObjectMemberDescriptor> final
{
    std::size_t operator()(const javascript_tasklets::gc::ObjectMemberDescriptor &v) const noexcept
    {
        return v.hash();
    }
};

template <>
struct hash<javascript_tasklets::gc::ObjectMemberDescriptor::AccessorInDescriptorT> final
{
    std::size_t operator()(
        const javascript_tasklets::gc::ObjectMemberDescriptor::AccessorInDescriptorT &v) const
        noexcept
    {
        return v.hash();
    }
};

template <>
struct hash<javascript_tasklets::gc::ObjectMemberDescriptor::DataInDescriptorT> final
{
    std::size_t operator()(
        const javascript_tasklets::gc::ObjectMemberDescriptor::DataInDescriptorT &v) const noexcept
    {
        return v.hash();
    }
};

template <>
struct hash<javascript_tasklets::gc::ObjectMemberDescriptor::DataInObjectT> final
{
    std::size_t operator()(
        const javascript_tasklets::gc::ObjectMemberDescriptor::DataInObjectT &v) const noexcept
    {
        return v.hash();
    }
};
}

namespace javascript_tasklets
{
namespace gc
{
inline std::size_t ObjectMemberDescriptor::hash() const noexcept
{
    return std::hash<bool>()(configurable) * 126478UL + std::hash<bool>()(enumerable) * 12623U
           + std::hash<ValueType>()(value);
}

class GC;
struct ObjectDescriptor;
typedef const ObjectDescriptor *ObjectDescriptorReference;
class GCReferencesCallback;

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
        virtual void getGCReferences(GCReferencesCallback &callback) const = 0;
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
            constexpr_assert(!name.empty() || descriptor.empty());
        }
        bool empty() const noexcept
        {
            return name.empty() && descriptor.empty();
        }
        bool isDeleted() const noexcept
        {
            return !name.empty() && descriptor.empty();
        }
    };
    const GC *const gc;
    const ObjectDescriptorReference parent;
    const std::size_t index;

private:
    std::size_t embeddedMemberCount;
    std::vector<Member> members;

private:
    void recalculateEmbeddedMembers() noexcept
    {
        embeddedMemberCount = 0;
        for(Member &member : members)
        {
            if(member.descriptor.isEmbedded())
            {
                member.descriptor.setMemberIndex(ObjectMemberIndex(embeddedMemberCount++));
            }
        }
    }

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
        constexpr_assert(!member.isDeleted());
        constexpr_assert(member.name == members[index].name);
        bool embeddedMembersModified =
            members[index].descriptor.isEmbedded() != member.descriptor.isEmbedded();
        members[index] = member;
        if(embeddedMembersModified)
            recalculateEmbeddedMembers();
    }
    void deleteMember(std::size_t index)
    {
        constexpr_assert(index < members.size());
        if(members[index].descriptor.isEmbedded())
        {
            members.erase(members.begin() + index);
            recalculateEmbeddedMembers();
        }
        else
        {
            members.erase(members.begin() + index);
        }
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
                                                 Handle<ObjectDescriptorReference> newParent,
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

class HandleScope;

template <typename T>
struct AddHandleToHandleScope final
{
    void operator()(HandleScope &handleScope, const T &value) const = delete;
};

template <typename T>
struct AddHandleToHandleScope<Handle<T>> final
{
    void operator()(HandleScope &handleScope, const Handle<T> &value) const
    {
        AddHandleToHandleScope<T>()(handleScope, value.get());
    }
};

template <>
struct AddHandleToHandleScope<std::nullptr_t> final
{
    void operator()(HandleScope &handleScope, std::nullptr_t) const
    {
    }
};

template <>
struct AddHandleToHandleScope<InternalName> final
{
    void operator()(HandleScope &handleScope, InternalName) const
    {
    }
};

template <>
struct AddHandleToHandleScope<std::int32_t> final
{
    void operator()(HandleScope &handleScope, std::int32_t) const
    {
    }
};

template <>
struct AddHandleToHandleScope<std::uint32_t> final
{
    void operator()(HandleScope &handleScope, std::uint32_t) const
    {
    }
};

template <>
struct AddHandleToHandleScope<double> final
{
    void operator()(HandleScope &handleScope, double) const
    {
    }
};

template <>
struct AddHandleToHandleScope<bool> final
{
    void operator()(HandleScope &handleScope, bool) const
    {
    }
};

template <typename... Types>
struct AddHandleToHandleScope<variant<Types...>> final
{
    struct AddHandleFn final
    {
        HandleScope &handleScope;
        AddHandleFn(HandleScope &handleScope) : handleScope(handleScope)
        {
        }
        void operator()()
        {
        }
        template <typename T>
        void operator()(const T &value)
        {
            AddHandleToHandleScope<T>()(handleScope, value);
        }
    };
    void operator()(HandleScope &handleScope, const variant<Types...> &value) const
    {
        value.apply(AddHandleFn(handleScope));
    }
};

class HandleScope final
{
    friend class GC;
    template <typename T>
    friend class Handle;
    HandleScope(const HandleScope &rt) = delete;
    HandleScope &operator=(const HandleScope &rt) = delete;
    void *operator new(std::size_t) = delete;

private:
    template <typename T,
              std::size_t EmbeddedReferenceCount,
              std::size_t InitialDynamicAllocationSize = 32>
    class ReferenceList final
    {
        ReferenceList(const ReferenceList &) = delete;
        ReferenceList &operator=(const ReferenceList &) = delete;

    public:
        static constexpr std::size_t embeddedReferenceCount = EmbeddedReferenceCount;
        static constexpr std::size_t initialDynamicAllocationSize = InitialDynamicAllocationSize;

    private:
        T *references;
        std::size_t referenceCount;
        std::size_t referencesAllocated;
        T embeddedReferences[embeddedReferenceCount];

    public:
        ReferenceList() noexcept : references(nullptr),
                                   referenceCount(0),
                                   referencesAllocated(embeddedReferenceCount),
                                   embeddedReferences()
        {
        }
        ~ReferenceList() noexcept
        {
            delete[] references;
        }
        template <typename Fn>
        void forEach(Fn &&fn) const
            noexcept(noexcept(std::forward<Fn>(fn)(std::declval<const T &>())))
        {
            for(std::size_t i = 0; i < referenceCount && i < embeddedReferenceCount; i++)
            {
                std::forward<Fn>(fn)(static_cast<const T &>(embeddedReferences[i]));
            }
            for(std::size_t i = embeddedReferenceCount; i < referenceCount; i++)
            {
                std::forward<Fn>(fn)(
                    static_cast<const T &>(references[i - embeddedReferenceCount]));
            }
        }
        void add(T value)
        {
            if(referenceCount >= referencesAllocated)
                expandArray();
            if(referenceCount >= embeddedReferenceCount)
                references[referenceCount++ - embeddedReferenceCount] = value;
            else
                embeddedReferences[referenceCount++] = value;
        }

    private:
        void expandArray()
        {
            std::size_t newReferencesAllocated;
            if(referencesAllocated < initialDynamicAllocationSize + embeddedReferenceCount)
            {
                newReferencesAllocated = initialDynamicAllocationSize + embeddedReferenceCount;
            }
            else
            {
                newReferencesAllocated = 2 * referencesAllocated - embeddedReferenceCount;
                // equivalent to
                // 2 * (referencesAllocated - embeddedHandleCount) + embeddedReferenceCount
            }
            auto temp = new T[newReferencesAllocated - embeddedReferenceCount];
            try
            {
                for(std::size_t i = 0; i < referenceCount - embeddedReferenceCount; i++)
                    temp[i] = references[i];
            }
            catch(...)
            {
                delete[] temp;
                throw;
            }
            delete[] references;
            references = temp;
            referencesAllocated = newReferencesAllocated;
        }
    };

private:
    ReferenceList<ObjectReference, 13> objectReferences;
    ReferenceList<StringOrSymbolReference, 13> stringOrSymbolReferences;
    ReferenceList<ObjectDescriptorReference, 5> objectDescriptorReferences;
    ReferenceList<SourceReference, 2> sourceReferences;
    HandleScope *parent;
    GC &gc;
    inline void addToGC();
    inline void removeFromGC();
    void addReference(ObjectReference reference)
    {
        if(reference == nullptr)
            return;
        objectReferences.add(reference);
    }
    void addReference(StringOrSymbolReference reference)
    {
        if(reference == nullptr)
            return;
        stringOrSymbolReferences.add(reference);
    }
    void addReference(SourceReference reference)
    {
        if(reference == nullptr)
            return;
        sourceReferences.add(reference);
    }
    void addReference(ObjectDescriptorReference reference)
    {
        if(reference == nullptr || reference->gc != &gc)
            return;
        objectDescriptorReferences.add(reference);
    }
    void addReference(const StringReference &reference)
    {
        addReference(static_cast<const StringOrSymbolReference &>(reference));
    }
    void addReference(const SymbolReference &reference)
    {
        addReference(static_cast<const StringOrSymbolReference &>(reference));
    }

public:
    explicit HandleScope(GC &gc)
        : objectReferences(),
          stringOrSymbolReferences(),
          objectDescriptorReferences(),
          sourceReferences(),
          parent(nullptr),
          gc(gc)
    {
        addToGC();
    }
    ~HandleScope()
    {
        removeFromGC();
    }
    template <typename T>
    T escapeHandle(const T &handle)
    {
        constexpr_assert(parent);
        AddHandleToHandleScope<T>()(*parent, handle);
        return handle;
    }
    struct ObjectHandleAdder
    {
        void operator()(HandleScope &handleScope, ObjectReference value) const
        {
            handleScope.addReference(value);
        }
    };
    struct StringHandleAdder
    {
        void operator()(HandleScope &handleScope, StringReference value) const
        {
            handleScope.addReference(value);
        }
    };
    struct SymbolHandleAdder
    {
        void operator()(HandleScope &handleScope, SymbolReference value) const
        {
            handleScope.addReference(value);
        }
    };
    struct SourceHandleAdder
    {
        void operator()(HandleScope &handleScope, SourceReference value) const
        {
            handleScope.addReference(value);
        }
    };
    struct ObjectDescriptorHandleAdder
    {
        void operator()(HandleScope &handleScope, ObjectDescriptorReference value) const
        {
            handleScope.addReference(value);
        }
    };
};

template <>
struct AddHandleToHandleScope<ObjectDescriptorReference> final
    : public HandleScope::ObjectDescriptorHandleAdder
{
};

template <>
struct AddHandleToHandleScope<ObjectDescriptor *> final
{
    void operator()(HandleScope &handleScope, ObjectDescriptor *value) const
    {
        HandleScope::ObjectDescriptorHandleAdder()(handleScope, value);
    }
};

template <>
struct AddHandleToHandleScope<ObjectReference> final : public HandleScope::ObjectHandleAdder
{
};

template <>
struct AddHandleToHandleScope<StringReference> final : public HandleScope::StringHandleAdder
{
};

template <>
struct AddHandleToHandleScope<SourceReference> final : public HandleScope::SourceHandleAdder
{
};

template <>
struct AddHandleToHandleScope<SymbolReference> final : public HandleScope::SymbolHandleAdder
{
};

class GCReferencesCallback final
{
    friend class GC;
    GCReferencesCallback(const GCReferencesCallback &) = delete;
    GCReferencesCallback &operator=(const GCReferencesCallback &) = delete;

private:
    HandleScope &handleScope;

    explicit GCReferencesCallback(HandleScope &handleScope) noexcept : handleScope(handleScope)
    {
    }

public:
    template <typename T>
    void operator()(const T &reference) const
    {
        AddHandleToHandleScope<T>()(handleScope, reference);
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
        ObjectDescriptor::Member member;
        ObjectDescriptorReference sourceDescriptor;
        ObjectDescriptorReference
            destDescriptor; // destDescriptor is ignored for hash and equality comparison
        ObjectDescriptorTransition(ObjectDescriptor::Member member,
                                   ObjectDescriptorReference sourceDescriptor,
                                   ObjectDescriptorReference destDescriptor = nullptr) noexcept
            : member(member),
              sourceDescriptor(sourceDescriptor),
              destDescriptor(destDescriptor)
        {
        }
        std::size_t hash() const noexcept
        {
            return std::hash<Name>()(member.name)
                   + 3923 * std::hash<ObjectDescriptorReference>()(sourceDescriptor);
        }
        bool operator==(const ObjectDescriptorTransition &rt) const noexcept
        {
            return member.name == rt.member.name && member.descriptor == rt.member.descriptor
                   && sourceDescriptor == rt.sourceDescriptor;
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
    std::vector<parser::Source *> sources;
    std::vector<parser::Source *> oldSources;
    std::vector<std::size_t> freeSourcesIndexesList;
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
    std::size_t allocateSourceIndex();
    void freeSourceIndex(std::size_t sourceIndex) noexcept;
    std::size_t allocateObjectDescriptorIndex();
    void freeObjectDescriptorIndex(std::size_t objectDescriptorIndex) noexcept;
    Object *objectCopyOnWrite(ObjectReference objectReference);
    Object *createInternal(ObjectDescriptorReference objectDescriptor,
                           std::unique_ptr<Object::ExtraData> extraData);
    void freeInternal(Object *object) noexcept;
    void collectorMarkHandleScope(const HandleScope &handleScope) noexcept;
    void collectorMarkObject(ObjectReference object) noexcept;
    void collectorMarkSource(SourceReference source) noexcept;
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
        const ObjectDescriptor::Member &member,
        ObjectDescriptorReference sourceDescriptor);
    template <typename Tag>
    static const void *getGlobalValueMapKey() noexcept
    {
        static char key;
        return &key;
    }
    Handle<Value> getGlobalValue(const void *key);
    void setGlobalValue(const void *key, Handle<Value> value);
    ObjectDescriptor::Member modifyObject(Handle<Name> nameHandle,
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
    const ObjectDescriptor &readObjectDescriptor(Handle<ObjectDescriptorReference> handle) const
        noexcept
    {
        constexpr_assert(handle.get());
        return *handle.get();
    }
    ObjectDescriptor &writeObjectDescriptor(Handle<ObjectDescriptor *> handle) noexcept
    {
        constexpr_assert(handle.get());
        return *handle.get();
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
    Handle<SourceReference> createSource(String fileName, String contents);
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
    void removeObjectMember(Handle<Name> nameHandle, Handle<ObjectReference> object)
    {
        modifyObject(nameHandle, object, ObjectMemberDescriptor::Empty());
    }
    ObjectDescriptor::Member addOrChangeObjectMemberAccessorInDescriptor(
        Handle<Name> nameHandle,
        Handle<ObjectReference> object,
        bool configurable,
        bool enumerable,
        Handle<ObjectReference> getter,
        Handle<ObjectReference> setter)
    {
        return modifyObject(nameHandle,
                            object,
                            ObjectMemberDescriptor::AccessorInDescriptor(
                                configurable, enumerable, getter.get(), setter.get()));
    }
    ObjectDescriptor::Member addOrChangeObjectMemberDataInDescriptor(Handle<Name> nameHandle,
                                                                     Handle<ObjectReference> object,
                                                                     bool configurable,
                                                                     bool enumerable,
                                                                     Handle<Value> value,
                                                                     bool writable)
    {
        return modifyObject(nameHandle,
                            object,
                            ObjectMemberDescriptor::DataInDescriptor(
                                configurable, enumerable, value.get(), writable));
    }
    ObjectDescriptor::Member addOrChangeObjectMemberDataInObject(Handle<Name> nameHandle,
                                                                 Handle<ObjectReference> object,
                                                                 bool configurable,
                                                                 bool enumerable,
                                                                 bool writable)
    {
        return modifyObject(nameHandle,
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
    AddHandleToHandleScope<T>()(*gc.handleScopesStack, static_cast<const T &>(this->value));
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
}
using gc::GC;
using gc::HandleScope;
using gc::Handle;
}

#endif /* JAVASCRIPT_TASKLETS_GC_H_ */
