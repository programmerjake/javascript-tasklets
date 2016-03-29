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

struct Object
{
    const ObjectDescriptorBase *objectDescriptor;
    explicit constexpr Object(const ObjectDescriptorBase *objectDescriptor);
};

struct ObjectMemberDescriptor final
{
	enum class Type
	{
		Pointer,
		Int32,
		Double,
	};
	Type type;
	std::size_t arrayLength;
	char Object::*member;
	Object *&getPointer(Object *object, std::size_t index) const
	{
		assert(type == Type::Pointer);
		assert(index < arrayLength || (arrayLength == 0 && index == 0));
		return (object->*reinterpret_cast<Object *(Object::*)[1]>(member))[index];
	}
	std::int32_t &getInt32(Object *object, std::size_t index) const
	{
		assert(type == Type::Int32);
		assert(index < arrayLength || (arrayLength == 0 && index == 0));
		return (object->*reinterpret_cast<std::int32_t(Object::*)[1]>(member))[index];
	}
	double &getDouble(Object *object, std::size_t index) const
	{
		assert(type == Type::Double);
		assert(index < arrayLength || (arrayLength == 0 && index == 0));
		return (object->*reinterpret_cast<double(Object::*)[1]>(member))[index];
	}
	Object *getIfPointer(Object *object, std::size_t index) const
	{
		if(type != Type::Pointer)
			return nullptr;
		return getPointer(object, index);
	}
};

class Allocator;

struct ObjectDescriptorBase
{
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
