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
#define JAVASCRIPT_TASKLETS_VM_INTERPRETER_KEEP_INSTRUCTIONS
#include "interpreter.h"
#include "vm.h"

namespace javascript_tasklets
{
namespace vm
{
namespace interpreter
{
void Instruction::addHandleToHandleScope(HandleScope &handleScope) const
{
    gc::AddHandleToHandleScope<parser::Location>()(handleScope, location);
    switch(type)
    {
#define JAVASCRIPT_TASKLETS_BINARY(name)                                  \
    case Type::name:                                                      \
        gc::AddHandleToHandleScope<Op##name>()(handleScope, value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY(name)                                   \
    case Type::name:                                                      \
        gc::AddHandleToHandleScope<Op##name>()(handleScope, value##name); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)                            \
    case Type::name:                                                      \
        gc::AddHandleToHandleScope<Op##name>()(handleScope, value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)                             \
    case Type::name:                                                      \
        gc::AddHandleToHandleScope<Op##name>()(handleScope, value##name); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)                            \
    case Type::name:                                                      \
        gc::AddHandleToHandleScope<Op##name>()(handleScope, value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)                             \
    case Type::name:                                                      \
        gc::AddHandleToHandleScope<Op##name>()(handleScope, value##name); \
        return;
#define JAVASCRIPT_TASKLETS_JUMP(name)                                    \
    case Type::name:                                                      \
        gc::AddHandleToHandleScope<Op##name>()(handleScope, value##name); \
        return;
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)                               \
    case Type::name:                                                      \
        gc::AddHandleToHandleScope<Op##name>()(handleScope, value##name); \
        return;
#define JAVASCRIPT_TASKLETS_RETURN(name)                                  \
    case Type::name:                                                      \
        gc::AddHandleToHandleScope<Op##name>()(handleScope, value##name); \
        return;
#define JAVASCRIPT_TASKLETS_END()
        JAVASCRIPT_TASKLETS_INSTRUCTIONS()
#undef JAVASCRIPT_TASKLETS_BINARY
#undef JAVASCRIPT_TASKLETS_UNARY
#undef JAVASCRIPT_TASKLETS_BINARY_THROW
#undef JAVASCRIPT_TASKLETS_UNARY_THROW
#undef JAVASCRIPT_TASKLETS_BINARY_DEOPT
#undef JAVASCRIPT_TASKLETS_UNARY_DEOPT
#undef JAVASCRIPT_TASKLETS_JUMP
#undef JAVASCRIPT_TASKLETS_COND_JUMP
#undef JAVASCRIPT_TASKLETS_RETURN
#undef JAVASCRIPT_TASKLETS_END
    case Type::Empty:
        return;
    }
    constexpr_assert(false);
}

value::ValueHandle FunctionCode::run(const std::vector<value::ValueHandle> &arguments, GC &gc) const
{
    HandleScope handleScope(gc);
    InstructionAddress pc(0);
    typedef Instruction::Type Type;
    for(;;)
    {
        constexpr_assert(pc.index < instructions.size());
        const Instruction &instruction = instructions[pc.index++];
        switch(instruction.getType())
        {
        case Type::Empty:
            constexpr_assert(false);
            continue;
        case Type::AddU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::AddI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::AddD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::SubU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::SubI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::SubD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MulU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MulI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MulD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::DivU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::DivI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::DivD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::ModU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::ModI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::ModD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::ShlU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::ShlI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::ShrU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::ShrI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::SarU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::SarI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::AndU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::AndI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::OrU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::OrI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::XorU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::XorI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpEqI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpEqUI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpEqD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpNEI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpNEUI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpNED:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpLTI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpLTUI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpLTIU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpLTU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpLTD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpNLTD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpLEI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpLEUI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpLEIU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpLEU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpLED:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpNLED:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpGTD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpNGTD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpGED:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::CmpNGED:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::ConcatS:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathATan2D:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathAbsI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathAbsD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathACosD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathACosHD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathASinD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathASinHD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathATanD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathATanHD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathCbrtD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathCeilD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathClz32U:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathCosD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathCosHD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathExpD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathExpM1D:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathFloorD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathFRoundD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathLogD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathLog1PD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathLog10D:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathLog2D:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathMaxD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathMinD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathPowD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathRoundD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathSignD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathSignI:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathSinD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathSinHD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathSqrtD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathTanD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathTanHD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::MathTruncD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::StringLengthU:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::StringLengthD:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::Move:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::Jump:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::JumpIfTrue:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::JumpIfFalse:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        case Type::Return:
        {
            HandleScope handleScope(gc);
            constexpr_assert(!"finish");
#warning finish
            continue;
        }
        }
        constexpr_assert(false);
    }
}

Handle<gc::ObjectDescriptorReference> FunctionCode::makeClosureObjectDescriptor(
    GC &gc, std::size_t registerCount)
{
    constexpr_assert(static_cast<std::uint32_t>(registerCount) == registerCount);
    HandleScope handleScope(gc);
    Handle<gc::ObjectDescriptor *> objectDescriptor = gc.createObjectDescriptor();
    objectDescriptor.get()->addMember(gc::ObjectDescriptor::Member(
        gc::Name(gc::InternalNameMaker::get<ClosureInternalNameTag>()),
        gc::ObjectMemberDescriptor::DataInDescriptor(false, false, gc::Value(), false)));
    constexpr_assert(objectDescriptor.get()->getEmbeddedMemberCount() == 0);
    for(RegisterIndex registerIndex(0); registerIndex.index < registerCount; registerIndex.index++)
    {
        Handle<gc::Name> name(
            gc.internString(value::UInt32Handle::toStringValue(registerIndex.index)));
        std::size_t memberIndex = objectDescriptor.get()->addMember(gc::ObjectDescriptor::Member(
            name.get(),
            gc::ObjectMemberDescriptor::DataInObject(false, false, gc::ObjectMemberIndex(), true)));
        constexpr_assert(
            objectDescriptor.get()->getMember(memberIndex).descriptor.getMemberIndex().index
            == registerIndex.index);
    }
    constexpr_assert(false);
#warning finish
}
}
}
}
