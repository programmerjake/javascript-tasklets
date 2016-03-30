/*
 * gc.h
 *
 *  Created on: Mar 27, 2016
 *      Author: jacob
 */

#ifndef JAVASCRIPT_TASKLETS_GC_H_
#define JAVASCRIPT_TASKLETS_GC_H_

#include <cassert>
#include <iterator>
#include <cstdint>
#include <vector>
#include <new>

namespace javascript_tasklets
{
namespace gc
{
struct ObjectDescriptorBase;

struct Object;

struct ObjectMemberDescriptor final
{
    enum class Type
    {
        ReferenceIndex, Int32, Double,
    };
    Type type;
    std::size_t arrayLength;
    std::size_t offset;
    constexpr bool isArray() const
    {
        return arrayLength > 0;
    }
    constexpr bool isScalar() const
    {
        return arrayLength == 0;
    }
    static std::size_t getTypeSize(Type type)
    {
        switch(type)
        {
        case Type::ReferenceIndex:
            return sizeof(std::size_t);
        case Type::Int32:
            return sizeof(std::int32_t);
        case Type::Double:
            return sizeof(double);
        }
        assert(false);
        return sizeof(std::int32_t);
    }
    std::size_t getSize() const
    {
        std::size_t typeSize = getTypeSize(type);
        if(isScalar())
        {
            return typeSize;
        }
        else if(isArray())
        {
            return typeSize * arrayLength;
        }
        else
        {
            assert(false);
            return typeSize;
        }
    }
    static std::size_t getTypeAlignment(Type type)
    {
        switch(type)
        {
        case Type::ReferenceIndex:
            return alignof(std::size_t);
        case Type::Int32:
            return alignof(std::int32_t);
        case Type::Double:
            return alignof(double);
        }
        assert(false);
        return alignof(std::int32_t);
    }
    std::size_t getAlignment() const
    {
        return getTypeAlignment(type);
    }
    std::size_t &getReferenceIndex(Object *object, std::size_t index) const
    {
        assert(type == Type::ReferenceIndex);
        assert(index < arrayLength || (arrayLength == 0 && index == 0));
        return reinterpret_cast<std::size_t *>(reinterpret_cast<char *>(object)
            + offset)[index];
    }
    std::int32_t &getInt32(Object *object, std::size_t index) const
    {
        assert(type == Type::Int32);
        assert(index < arrayLength || (arrayLength == 0 && index == 0));
        return reinterpret_cast<std::int32_t *>(reinterpret_cast<char *>(object)
            + offset)[index];
    }
    double &getDouble(Object *object, std::size_t index) const
    {
        assert(type == Type::Double);
        assert(index < arrayLength || (arrayLength == 0 && index == 0));
        return reinterpret_cast<double *>(reinterpret_cast<char *>(object)
            + offset)[index];
    }
    std::size_t getIfReferenceIndex(Object *object, std::size_t index) const
    {
        if(type != Type::ReferenceIndex)
            return 0;
        return getReferenceIndex(object, index);
    }
    constexpr ObjectMemberDescriptor(Type type,
        std::size_t arrayLength,
        std::size_t offset)
        : type(type),
          arrayLength(arrayLength),
          offset(offset)
    {
    }
    static ObjectMemberDescriptor make(std::size_t &currentSize,
        Type type,
        std::size_t arrayLength = 0)
    {
        std::size_t alignment = getTypeAlignment(type);
        std::size_t size = getTypeSize(type);
        if(arrayLength > 0)
            size *= arrayLength;
        assert((alignment & (alignment - 1)) == 0); // assert alignment is a power of 2
        currentSize = (currentSize + alignment - 1) & ~(alignment - 1); // round up to next multiple of alignment (only works for powers of 2)
        ObjectMemberDescriptor retval(type, arrayLength, currentSize);
        currentSize += size;
        return retval;
    }
};

class Allocator;

struct ObjectDescriptor
{
    std::size_t size;
    std::vector<ObjectMemberDescriptor> members;
    void addMember(ObjectMemberDescriptor::Type type)
    {
        members.push_back(ObjectMemberDescriptor::make(size, type));
    }
    void addMember(ObjectMemberDescriptor::Type type, std::size_t arrayLength)
    {
        assert(arrayLength >= 1);
        members.push_back(ObjectMemberDescriptor::make(size,
            type,
            arrayLength));
    }
    virtual ~ObjectDescriptor() = default;
    virtual void destructObject(Object *object) const
    {
    }
    virtual ObjectDescriptor *clone(Allocator &allocator) const;
};

class Allocator
{
    Allocator(const Allocator &) = delete;
    Allocator &operator=(const Allocator &) = delete;

public:
    Allocator() = default;
    virtual ~Allocator() = default;
    virtual void *allocate(std::size_t size) = 0;
    virtual void free(void *memory) noexcept = 0;
    template <typename T, typename ...Args>
    T *create(Args &&...args)
    {
        void *mem = allocate(sizeof(T));
        if(!mem)
            return nullptr;
        try
        {
            return ::new (mem) T(std::forward<Args>(args)...);
        }
        catch(...)
        {
            free(mem);
            throw;
        }
    }
    template <typename T>
    void destroy(T *object) noexcept(std::is_nothrow_destructible<T>::value)
    {
        if(!object)
            return;
        try
        {
            object->~T();
        }
        catch(...)
        {
            free(object);
            throw;
        }
        free(object);
    }
};

struct NewDeleteAllocator final : public Allocator
{
    virtual void *allocate(std::size_t size) override
    {
        return ::operator new(size);
    }
    virtual void free(void *memory) noexcept override
    {
        ::operator delete(memory);
    }
};
}
}

#endif /* JAVASCRIPT_TASKLETS_GC_H_ */
