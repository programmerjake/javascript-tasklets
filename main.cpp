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
#include "javascript_tasklets/gc.h"
#include "javascript_tasklets/string.h"
#include "javascript_tasklets/vm/interpreter.h"
#include <iostream>
#include <sstream>

using namespace javascript_tasklets;

namespace test
{
void main()
{
    gc::GC gc;
    HandleScope handleScope(gc);
    typedef vm::interpreter::Instruction Instruction;
    typedef vm::interpreter::RegisterIndex RegisterIndex;
    typedef vm::interpreter::InstructionAddress InstructionAddress;
    std::vector<Instruction> instructions;
    instructions.push_back(
        Instruction::makeMathATanHD(parser::Location(), RegisterIndex(0), RegisterIndex(0)));
    instructions.push_back(Instruction::makeReturn(parser::Location(), RegisterIndex(0)));
    vm::interpreter::FunctionCode fn(std::move(instructions), 1, 1, gc);
    std::cout << fn.run(std::vector<value::ValueHandle>{value::DoubleHandle(0.2, gc)}, gc)
                     .getDouble()
                     .getValue(gc) << std::endl;
}
}

int main()
{
    test::main();
    return 0;
}
