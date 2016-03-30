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
    std::size_t &getReferenceIndex(Object *object, std::size_t index) const
    {
        assert(type == Type::ReferenceIndex);
        assert(index < arrayLength || (arrayLength == 0 && index == 0));
        return reinterpret_cast<std::size_t *>(reinterpret_cast<char *>(object) + offset)[index];
    }
    std::int32_t &getInt32(Object *object, std::size_t index) const
    {
        assert(type == Type::Int32);
        assert(index < arrayLength || (arrayLength == 0 && index == 0));
        return reinterpret_cast<std::int32_t *>(reinterpret_cast<char *>(object) + offset)[index];
    }
    double &getDouble(Object *object, std::size_t index) const
    {
        assert(type == Type::Double);
        assert(index < arrayLength || (arrayLength == 0 && index == 0));
        return reinterpret_cast<double *>(reinterpret_cast<char *>(object) + offset)[index];
    }
    std::size_t getIfReferenceIndex(Object *object, std::size_t index) const
    {
        if(type != Type::ReferenceIndex)
            return 0;
        return getReferenceIndex(object, index);
    }
    ObjectMemberDescriptor
};

class Allocator;

struct ObjectDescriptorBase
{
    std::size_t size;
    std::vector<ObjectMemberDescriptor> members;

};

class Allocator
{
    Allocator(const Allocator &) = delete;
    Allocator &operator=(const Allocator &) = delete;

public:
    Allocator() = default;
    virtual ~Allocator() = default;
    virtual void *allocate(std::size_t size) = 0;
    virtual void free(void *memory);
};
}
}

#endif /* JAVASCRIPT_TASKLETS_GC_H_ */
