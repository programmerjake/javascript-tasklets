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

ObjectDescriptor::ObjectDescriptor(const GC *gc) : gc(gc), memberCount(0)
{
#error finish
}

Object *GC::objectCopyOnWrite(ObjectReference objectReference)
{
    assert(!immutable);
    assert(objectReference.index < objects.size());
    assert(parent);
#error finish
}

GC::GC()
{
#error finish
}

GC::GC(std::shared_ptr<const GC> parent)
{
#error finish
}

GC::~GC()
{
#error finish
}

void GC::collect()
{
    assert(!immutable);
#error finish
}

ObjectHandle GC::create(ObjectDescriptor *objectDescriptor)
{
#error finish
}
}
}
