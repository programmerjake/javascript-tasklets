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

#ifndef JAVASCRIPT_TASKLETS_VM_VM_H_
#define JAVASCRIPT_TASKLETS_VM_VM_H_

#include "../gc.h"
#include "../array_ref.h"

namespace javascript_tasklets
{
namespace value
{
struct ValueHandle;
struct ObjectOrNullHandle;
}
namespace vm
{
struct ClosureInternalNameTag final
{
};
struct Code
{
    virtual ~Code() = default;
    virtual value::ValueHandle run(const value::ValueHandle &thisValue,
                                   const value::ObjectOrNullHandle &newTarget,
                                   ArrayRef<const value::ValueHandle> arguments,
                                   GC &gc) const = 0;
    virtual void getGCReferences(gc::GCReferencesCallback &callback) const = 0;
};
}
}

#endif /* JAVASCRIPT_TASKLETS_VM_VM_H_ */
