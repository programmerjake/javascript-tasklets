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
#include "jsmath.h"

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

namespace
{
struct Registers final
{
    gc::Object &closure;
    const std::size_t registerCount;
    gc::Value &operator[](RegisterIndex index) noexcept
    {
        constexpr_assert(index.index < registerCount);
        return closure.getMembersArray()[index.index];
    }
    std::uint32_t getIntegerAsUInt32(RegisterIndex index) noexcept
    {
        auto &value = operator[](index);
        if(value.is<std::uint32_t>())
            return value.get<std::uint32_t>();
        return value.get<std::int32_t>();
    }
    std::int32_t getIntegerAsInt32(RegisterIndex index) noexcept
    {
        auto &value = operator[](index);
        if(value.is<std::uint32_t>())
            return value.get<std::uint32_t>();
        return value.get<std::int32_t>();
    }
    template <typename T>
    T &get(RegisterIndex index) noexcept
    {
        auto &value = operator[](index);
        return value.get<T>();
    }
    Registers(gc::Object &closure, const std::size_t registerCount) noexcept
        : closure(closure),
          registerCount(registerCount)
    {
    }
};
}

value::ValueHandle FunctionCode::run(const value::ValueHandle &thisValue,
                                     ArrayRef<const value::ValueHandle> arguments,
                                     GC &gc) const
{
    HandleScope handleScope(gc);
    InstructionAddress pc(0);
    typedef Instruction::Type Type;
    constexpr_assert(closureObjectDescriptor.get()->getEmbeddedMemberCount() == registerCount);
    const Handle<gc::ObjectReference> closureHandle = gc.create(closureObjectDescriptor);
    gc::Object &closure = gc.writeObject(closureHandle);
    Registers registers(closure, registerCount);
    for(RegisterIndex i(0); i.index < namedArgumentCount; i.index++)
    {
        if(i.index >= arguments.size())
            registers[i] = gc::Value();
        else
            registers[i] = arguments[i.index].get().get();
    }
    for(;;)
    {
        constexpr_assert(&closure == &gc.writeObject(closureHandle));
        constexpr_assert(gc.getObjectDescriptor(closureHandle).get()
                         == closureObjectDescriptor.get());
        constexpr_assert(pc.index < instructions.size());
        const Instruction &instruction = instructions[pc.index++];
        switch(instruction.getType())
        {
        case Type::Empty:
            constexpr_assert(false);
            continue;
        case Type::AddU:
        {
            auto &args = instruction.getAddU();
            registers[args.dest] =
                gc::Value::make<std::uint32_t>(registers.getIntegerAsUInt32(args.source1)
                                               + registers.getIntegerAsUInt32(args.source2));
            continue;
        }
        case Type::AddI:
        {
            auto &args = instruction.getAddI();
            registers[args.dest] =
                gc::Value::make<std::int32_t>(registers.getIntegerAsInt32(args.source1)
                                              + registers.getIntegerAsInt32(args.source2));
            continue;
        }
        case Type::AddD:
        {
            auto &args = instruction.getAddD();
            registers[args.dest] = gc::Value::make<double>(registers.get<double>(args.source1)
                                                           + registers.get<double>(args.source2));
            continue;
        }
        case Type::SubU:
        {
            auto &args = instruction.getSubU();
            registers[args.dest] =
                gc::Value::make<std::uint32_t>(registers.getIntegerAsUInt32(args.source1)
                                               - registers.getIntegerAsUInt32(args.source2));
            continue;
        }
        case Type::SubI:
        {
            auto &args = instruction.getSubI();
            registers[args.dest] =
                gc::Value::make<std::int32_t>(registers.getIntegerAsInt32(args.source1)
                                              - registers.getIntegerAsInt32(args.source2));
            continue;
        }
        case Type::SubD:
        {
            auto &args = instruction.getSubD();
            registers[args.dest] = gc::Value::make<double>(registers.get<double>(args.source1)
                                                           - registers.get<double>(args.source2));
            continue;
        }
        case Type::MulU:
        {
            auto &args = instruction.getMulU();
            registers[args.dest] =
                gc::Value::make<std::uint32_t>(registers.getIntegerAsUInt32(args.source1)
                                               * registers.getIntegerAsUInt32(args.source2));
            continue;
        }
        case Type::MulI:
        {
            auto &args = instruction.getMulI();
            registers[args.dest] =
                gc::Value::make<std::int32_t>(registers.getIntegerAsUInt32(args.source1)
                                              * registers.getIntegerAsUInt32(args.source2));
            continue;
        }
        case Type::MulD:
        {
            auto &args = instruction.getMulD();
            registers[args.dest] = gc::Value::make<double>(registers.get<double>(args.source1)
                                                           * registers.get<double>(args.source2));
            continue;
        }
        case Type::DivU:
        {
            auto &args = instruction.getDivU();
            registers[args.dest] = gc::Value::make<std::uint32_t>(
                math::div(registers.getIntegerAsUInt32(args.source1),
                          registers.getIntegerAsUInt32(args.source2)));
            continue;
        }
        case Type::DivI:
        {
            auto &args = instruction.getDivI();
            registers[args.dest] =
                gc::Value::make<std::int32_t>(math::div(registers.getIntegerAsInt32(args.source1),
                                                        registers.getIntegerAsInt32(args.source2)));
            continue;
        }
        case Type::DivD:
        {
            auto &args = instruction.getDivD();
            registers[args.dest] = gc::Value::make<double>(registers.get<double>(args.source1)
                                                           / registers.get<double>(args.source2));
            continue;
        }
        case Type::ModU:
        {
            auto &args = instruction.getModU();
            registers[args.dest] = gc::Value::make<std::uint32_t>(
                math::mod(registers.getIntegerAsUInt32(args.source1),
                          registers.getIntegerAsUInt32(args.source2)));
            continue;
        }
        case Type::ModI:
        {
            auto &args = instruction.getModI();
            registers[args.dest] =
                gc::Value::make<std::int32_t>(math::mod(registers.getIntegerAsInt32(args.source1),
                                                        registers.getIntegerAsInt32(args.source2)));
            continue;
        }
        case Type::ModD:
        {
            auto &args = instruction.getModD();
            registers[args.dest] = gc::Value::make<double>(math::mod(
                registers.get<double>(args.source1), registers.get<double>(args.source2)));
            continue;
        }
        case Type::ShlU:
        {
            auto &args = instruction.getShlU();
            registers[args.dest] = gc::Value::make<std::uint32_t>(
                registers.getIntegerAsUInt32(args.source1)
                << (0x1F & registers.getIntegerAsUInt32(args.source2)));
            continue;
        }
        case Type::ShlI:
        {
            auto &args = instruction.getShlI();
            registers[args.dest] = gc::Value::make<std::int32_t>(
                registers.getIntegerAsInt32(args.source1)
                << (0x1F & registers.getIntegerAsInt32(args.source2)));
            continue;
        }
        case Type::ShrU:
        {
            auto &args = instruction.getShrU();
            registers[args.dest] = gc::Value::make<std::uint32_t>(
                registers.getIntegerAsUInt32(args.source1)
                >> (0x1F & registers.getIntegerAsUInt32(args.source2)));
            continue;
        }
        case Type::ShrI:
        {
            auto &args = instruction.getShrI();
            registers[args.dest] = gc::Value::make<std::int32_t>(
                registers.getIntegerAsUInt32(args.source1)
                >> (0x1F & registers.getIntegerAsInt32(args.source2)));
            continue;
        }
        case Type::SarU:
        {
            auto &args = instruction.getSarU();
            registers[args.dest] = gc::Value::make<std::uint32_t>(
                registers.getIntegerAsInt32(args.source1)
                >> (0x1F & registers.getIntegerAsUInt32(args.source2)));
            continue;
        }
        case Type::SarI:
        {
            auto &args = instruction.getSarI();
            registers[args.dest] = gc::Value::make<std::int32_t>(
                registers.getIntegerAsInt32(args.source1)
                >> (0x1F & registers.getIntegerAsInt32(args.source2)));
            continue;
        }
        case Type::AndU:
        {
            auto &args = instruction.getAndU();
            registers[args.dest] =
                gc::Value::make<std::uint32_t>(registers.getIntegerAsUInt32(args.source1)
                                               & registers.getIntegerAsUInt32(args.source2));
            continue;
        }
        case Type::AndI:
        {
            auto &args = instruction.getAndI();
            registers[args.dest] =
                gc::Value::make<std::int32_t>(registers.getIntegerAsInt32(args.source1)
                                              & registers.getIntegerAsInt32(args.source2));
            continue;
        }
        case Type::OrU:
        {
            auto &args = instruction.getOrU();
            registers[args.dest] =
                gc::Value::make<std::uint32_t>(registers.getIntegerAsUInt32(args.source1)
                                               | registers.getIntegerAsUInt32(args.source2));
            continue;
        }
        case Type::OrI:
        {
            auto &args = instruction.getOrI();
            registers[args.dest] =
                gc::Value::make<std::int32_t>(registers.getIntegerAsInt32(args.source1)
                                              | registers.getIntegerAsInt32(args.source2));
            continue;
        }
        case Type::XorU:
        {
            auto &args = instruction.getXorU();
            registers[args.dest] =
                gc::Value::make<std::uint32_t>(registers.getIntegerAsUInt32(args.source1)
                                               ^ registers.getIntegerAsUInt32(args.source2));
            continue;
        }
        case Type::XorI:
        {
            auto &args = instruction.getXorI();
            registers[args.dest] =
                gc::Value::make<std::int32_t>(registers.getIntegerAsInt32(args.source1)
                                              ^ registers.getIntegerAsInt32(args.source2));
            continue;
        }
        case Type::CmpEqI:
        {
            auto &args = instruction.getCmpEqI();
            registers[args.dest] =
                gc::Value::make<bool>(registers.getIntegerAsInt32(args.source1)
                                      == registers.getIntegerAsInt32(args.source2));
            continue;
        }
        case Type::CmpEqUI:
        {
            auto &args = instruction.getCmpEqI();
            auto source1 = registers.getIntegerAsUInt32(args.source1);
            auto source2 = registers.getIntegerAsInt32(args.source2);
            registers[args.dest] = gc::Value::make<bool>(
                source2 >= 0 && source1 == static_cast<std::uint32_t>(source2));
            continue;
        }
        case Type::CmpEqD:
        {
            auto &args = instruction.getCmpEqD();
            registers[args.dest] = gc::Value::make<bool>(registers.get<double>(args.source1)
                                                         == registers.get<double>(args.source2));
            continue;
        }
        case Type::CmpEqS:
        {
            auto &args = instruction.getCmpEqS();
            registers[args.dest] =
                gc::Value::make<bool>(registers.get<gc::StringReference>(args.source1)
                                      == registers.get<gc::StringReference>(args.source2));
            continue;
        }
        case Type::CmpEqON:
        {
            auto &args = instruction.getCmpEqON();
            const gc::Value &source1Value = registers[args.source1];
            const gc::Value &source2Value = registers[args.source2];
            gc::ObjectReference source1 = nullptr;
            if(source1Value.is<gc::ObjectReference>())
                source1 = source1Value.get<gc::ObjectReference>();
            gc::ObjectReference source2 = nullptr;
            if(source2Value.is<gc::ObjectReference>())
                source2 = source2Value.get<gc::ObjectReference>();
            registers[args.dest] = gc::Value::make<bool>(source1 == source2);
            continue;
        }
        case Type::CmpEqSy:
        {
            auto &args = instruction.getCmpEqSy();
            registers[args.dest] =
                gc::Value::make<bool>(registers.get<gc::SymbolReference>(args.source1)
                                      == registers.get<gc::SymbolReference>(args.source2));
            continue;
        }
        case Type::CmpNEI:
        {
            auto &args = instruction.getCmpNEI();
            registers[args.dest] =
                gc::Value::make<bool>(registers.getIntegerAsInt32(args.source1)
                                      != registers.getIntegerAsInt32(args.source2));
            continue;
        }
        case Type::CmpNEUI:
        {
            auto &args = instruction.getCmpNEUI();
            auto source1 = registers.getIntegerAsUInt32(args.source1);
            auto source2 = registers.getIntegerAsInt32(args.source2);
            registers[args.dest] = gc::Value::make<bool>(
                source2 < 0 || source1 != static_cast<std::uint32_t>(source2));
            continue;
        }
        case Type::CmpNED:
        {
            auto &args = instruction.getCmpNED();
            registers[args.dest] = gc::Value::make<bool>(registers.get<double>(args.source1)
                                                         != registers.get<double>(args.source2));
            continue;
        }
        case Type::CmpNES:
        {
            auto &args = instruction.getCmpNES();
            registers[args.dest] =
                gc::Value::make<bool>(registers.get<gc::StringReference>(args.source1)
                                      != registers.get<gc::StringReference>(args.source2));
            continue;
        }
        case Type::CmpNEON:
        {
            auto &args = instruction.getCmpNEON();
            const gc::Value &source1Value = registers[args.source1];
            const gc::Value &source2Value = registers[args.source2];
            gc::ObjectReference source1 = nullptr;
            if(source1Value.is<gc::ObjectReference>())
                source1 = source1Value.get<gc::ObjectReference>();
            gc::ObjectReference source2 = nullptr;
            if(source2Value.is<gc::ObjectReference>())
                source2 = source2Value.get<gc::ObjectReference>();
            registers[args.dest] = gc::Value::make<bool>(source1 != source2);
            continue;
        }
        case Type::CmpNESy:
        {
            auto &args = instruction.getCmpNESy();
            registers[args.dest] =
                gc::Value::make<bool>(registers.get<gc::SymbolReference>(args.source1)
                                      != registers.get<gc::SymbolReference>(args.source2));
            continue;
        }
        case Type::CmpLTI:
        {
            auto &args = instruction.getCmpLTI();
            registers[args.dest] =
                gc::Value::make<bool>(registers.getIntegerAsInt32(args.source1)
                                      < registers.getIntegerAsInt32(args.source2));
            continue;
        }
        case Type::CmpLTUI:
        {
            auto &args = instruction.getCmpLTUI();
            auto source1 = registers.getIntegerAsUInt32(args.source1);
            auto source2 = registers.getIntegerAsInt32(args.source2);
            registers[args.dest] = gc::Value::make<bool>(
                source2 >= 0 && source1 < static_cast<std::uint32_t>(source2));
            continue;
        }
        case Type::CmpLTIU:
        {
            auto &args = instruction.getCmpLTIU();
            auto source1 = registers.getIntegerAsInt32(args.source1);
            auto source2 = registers.getIntegerAsUInt32(args.source2);
            registers[args.dest] =
                gc::Value::make<bool>(source1 < 0 || static_cast<std::uint32_t>(source1) < source2);
            continue;
        }
        case Type::CmpLTU:
        {
            auto &args = instruction.getCmpLTU();
            registers[args.dest] =
                gc::Value::make<bool>(registers.getIntegerAsUInt32(args.source1)
                                      < registers.getIntegerAsUInt32(args.source2));
            continue;
        }
        case Type::CmpLTD:
        {
            auto &args = instruction.getCmpLTD();
            registers[args.dest] = gc::Value::make<bool>(registers.get<double>(args.source1)
                                                         < registers.get<double>(args.source2));
            continue;
        }
        case Type::CmpNLTD:
        {
            auto &args = instruction.getCmpNLTD();
            registers[args.dest] = gc::Value::make<bool>(
                !(registers.get<double>(args.source1) < registers.get<double>(args.source2)));
            continue;
        }
        case Type::CmpLTS:
        {
            auto &args = instruction.getCmpLTS();
            HandleScope handleScope(gc);
            Handle<gc::StringReference> source1(closureHandle,
                                                registers.get<gc::StringReference>(args.source1));
            Handle<gc::StringReference> source2(closureHandle,
                                                registers.get<gc::StringReference>(args.source2));
            registers[args.dest] =
                gc::Value::make<bool>(gc.readString(source1) < gc.readString(source2));
            continue;
        }
        case Type::CmpLEI:
        {
            auto &args = instruction.getCmpLEI();
            registers[args.dest] =
                gc::Value::make<bool>(registers.getIntegerAsInt32(args.source1)
                                      <= registers.getIntegerAsInt32(args.source2));
            continue;
        }
        case Type::CmpLEUI:
        {
            auto &args = instruction.getCmpLEUI();
            auto source1 = registers.getIntegerAsUInt32(args.source1);
            auto source2 = registers.getIntegerAsInt32(args.source2);
            registers[args.dest] = gc::Value::make<bool>(
                source2 >= 0 && source1 <= static_cast<std::uint32_t>(source2));
            continue;
        }
        case Type::CmpLEIU:
        {
            auto &args = instruction.getCmpLEIU();
            auto source1 = registers.getIntegerAsInt32(args.source1);
            auto source2 = registers.getIntegerAsUInt32(args.source2);
            registers[args.dest] = gc::Value::make<bool>(
                source1 < 0 || static_cast<std::uint32_t>(source1) <= source2);
            continue;
        }
        case Type::CmpLEU:
        {
            auto &args = instruction.getCmpLEU();
            registers[args.dest] =
                gc::Value::make<bool>(registers.getIntegerAsUInt32(args.source1)
                                      < registers.getIntegerAsUInt32(args.source2));
            continue;
        }
        case Type::CmpLED:
        {
            auto &args = instruction.getCmpLED();
            registers[args.dest] = gc::Value::make<bool>(registers.get<double>(args.source1)
                                                         <= registers.get<double>(args.source2));
            continue;
        }
        case Type::CmpNLED:
        {
            auto &args = instruction.getCmpNLED();
            registers[args.dest] = gc::Value::make<bool>(
                !(registers.get<double>(args.source1) <= registers.get<double>(args.source2)));
            continue;
        }
        case Type::CmpLES:
        {
            auto &args = instruction.getCmpLES();
            HandleScope handleScope(gc);
            Handle<gc::StringReference> source1(closureHandle,
                                                registers.get<gc::StringReference>(args.source1));
            Handle<gc::StringReference> source2(closureHandle,
                                                registers.get<gc::StringReference>(args.source2));
            registers[args.dest] =
                gc::Value::make<bool>(gc.readString(source1) <= gc.readString(source2));
            continue;
        }
        case Type::ConcatS:
        {
            auto &args = instruction.getConcatS();
            HandleScope handleScope(gc);
            Handle<gc::StringReference> source1(closureHandle,
                                                registers.get<gc::StringReference>(args.source1));
            Handle<gc::StringReference> source2(closureHandle,
                                                registers.get<gc::StringReference>(args.source2));
            registers[args.dest] = gc::Value::make<gc::StringReference>(
                gc.internString(gc.readString(source1) + gc.readString(source2)).get());
            continue;
        }
        case Type::MathATan2D:
        {
            auto &args = instruction.getMathATan2D();
            registers[args.dest] = gc::Value::make<double>(math::atan2(
                registers.get<double>(args.source1), registers.get<double>(args.source2)));
            continue;
        }
        case Type::MathAbsI:
        {
            auto &args = instruction.getMathAbsI();
            registers[args.dest] =
                gc::Value::make<std::int32_t>(math::abs(registers.get<std::int32_t>(args.source1)));
            continue;
        }
        case Type::MathAbsD:
        {
            auto &args = instruction.getMathAbsD();
            registers[args.dest] =
                gc::Value::make<double>(math::abs(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathACosD:
        {
            auto &args = instruction.getMathACosD();
            registers[args.dest] =
                gc::Value::make<double>(math::acos(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathACosHD:
        {
            auto &args = instruction.getMathACosHD();
            registers[args.dest] =
                gc::Value::make<double>(math::acosh(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathASinD:
        {
            auto &args = instruction.getMathASinD();
            registers[args.dest] =
                gc::Value::make<double>(math::asin(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathASinHD:
        {
            auto &args = instruction.getMathASinHD();
            registers[args.dest] =
                gc::Value::make<double>(math::asinh(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathATanD:
        {
            auto &args = instruction.getMathATanD();
            registers[args.dest] =
                gc::Value::make<double>(math::atan(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathATanHD:
        {
            auto &args = instruction.getMathATanHD();
            registers[args.dest] =
                gc::Value::make<double>(math::atanh(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathCbrtD:
        {
            auto &args = instruction.getMathCbrtD();
            registers[args.dest] =
                gc::Value::make<double>(math::cbrt(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathCeilD:
        {
            auto &args = instruction.getMathCeilD();
            registers[args.dest] =
                gc::Value::make<double>(math::ceil(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathClz32U:
        {
            auto &args = instruction.getMathClz32U();
            registers[args.dest] = gc::Value::make<std::uint32_t>(
                math::clz32(registers.get<std::uint32_t>(args.source1)));
            continue;
        }
        case Type::MathCosD:
        {
            auto &args = instruction.getMathCosD();
            registers[args.dest] =
                gc::Value::make<double>(math::cos(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathCosHD:
        {
            auto &args = instruction.getMathCosHD();
            registers[args.dest] =
                gc::Value::make<double>(math::cosh(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathExpD:
        {
            auto &args = instruction.getMathExpD();
            registers[args.dest] =
                gc::Value::make<double>(math::exp(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathExpM1D:
        {
            auto &args = instruction.getMathExpM1D();
            registers[args.dest] =
                gc::Value::make<double>(math::expm1(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathFloorD:
        {
            auto &args = instruction.getMathFloorD();
            registers[args.dest] =
                gc::Value::make<double>(math::floor(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathFRoundD:
        {
            auto &args = instruction.getMathFRoundD();
            float v = static_cast<float>(registers.get<double>(args.source1));
            registers[args.dest] = gc::Value::make<double>(v);
            continue;
        }
        case Type::MathLogD:
        {
            auto &args = instruction.getMathLogD();
            registers[args.dest] =
                gc::Value::make<double>(math::log(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathLog1PD:
        {
            auto &args = instruction.getMathLog1PD();
            registers[args.dest] =
                gc::Value::make<double>(math::log1p(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathLog10D:
        {
            auto &args = instruction.getMathLog10D();
            registers[args.dest] =
                gc::Value::make<double>(math::log10(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathLog2D:
        {
            auto &args = instruction.getMathLog2D();
            registers[args.dest] =
                gc::Value::make<double>(math::log2(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathMaxD:
        {
            auto &args = instruction.getMathMaxD();
            registers[args.dest] = gc::Value::make<double>(math::max(
                registers.get<double>(args.source1), registers.get<double>(args.source2)));
            continue;
        }
        case Type::MathMinD:
        {
            auto &args = instruction.getMathMinD();
            registers[args.dest] = gc::Value::make<double>(math::min(
                registers.get<double>(args.source1), registers.get<double>(args.source2)));
            continue;
        }
        case Type::MathPowD:
        {
            auto &args = instruction.getMathPowD();
            registers[args.dest] = gc::Value::make<double>(math::pow(
                registers.get<double>(args.source1), registers.get<double>(args.source2)));
            continue;
        }
        case Type::MathRoundD:
        {
            auto &args = instruction.getMathRoundD();
            registers[args.dest] =
                gc::Value::make<double>(math::round(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathSignD:
        {
            auto &args = instruction.getMathSignD();
            registers[args.dest] =
                gc::Value::make<double>(math::sign(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathSignI:
        {
            auto &args = instruction.getMathSignI();
            registers[args.dest] = gc::Value::make<std::int32_t>(
                math::sign(registers.get<std::int32_t>(args.source1)));
            continue;
        }
        case Type::MathSinD:
        {
            auto &args = instruction.getMathSinD();
            registers[args.dest] =
                gc::Value::make<double>(math::sin(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathSinHD:
        {
            auto &args = instruction.getMathSinHD();
            registers[args.dest] =
                gc::Value::make<double>(math::sinh(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathSqrtD:
        {
            auto &args = instruction.getMathSqrtD();
            registers[args.dest] =
                gc::Value::make<double>(math::sqrt(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathTanD:
        {
            auto &args = instruction.getMathTanD();
            registers[args.dest] =
                gc::Value::make<double>(math::tan(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathTanHD:
        {
            auto &args = instruction.getMathTanHD();
            registers[args.dest] =
                gc::Value::make<double>(math::tanh(registers.get<double>(args.source1)));
            continue;
        }
        case Type::MathTruncD:
        {
            auto &args = instruction.getMathTruncD();
            registers[args.dest] =
                gc::Value::make<double>(math::trunc(registers.get<double>(args.source1)));
            continue;
        }
        case Type::StringLengthU:
        {
            auto &args = instruction.getStringLengthU();
            HandleScope handleScope(gc);
            Handle<gc::StringReference> source1(closureHandle,
                                                registers.get<gc::StringReference>(args.source1));
            std::size_t length = gc.readString(source1).size();
            std::uint32_t lengthU32 = length;
            registers[args.dest] = gc::Value::make<std::uint32_t>(lengthU32);
            if(lengthU32 != length)
                pc = args.deoptimizeTarget;
            continue;
        }
        case Type::StringLengthD:
        {
            auto &args = instruction.getStringLengthD();
            HandleScope handleScope(gc);
            Handle<gc::StringReference> source1(closureHandle,
                                                registers.get<gc::StringReference>(args.source1));
            registers[args.dest] = gc::Value::make<double>(gc.readString(source1).size());
            continue;
        }
        case Type::Move:
        {
            auto &args = instruction.getMove();
            registers[args.dest] = registers[args.source1];
            continue;
        }
        case Type::Jump:
        {
            auto &args = instruction.getJump();
            pc = args.jumpTarget;
            continue;
        }
        case Type::JumpIfTrue:
        {
            auto &args = instruction.getJumpIfTrue();
            if(registers[args.condition].get<bool>())
                pc = args.jumpTarget;
            continue;
        }
        case Type::JumpIfFalse:
        {
            auto &args = instruction.getJumpIfTrue();
            if(!registers[args.condition].get<bool>())
                pc = args.jumpTarget;
            continue;
        }
        case Type::Return:
        {
            auto &args = instruction.getReturn();
            return handleScope.escapeHandle(
                value::ValueHandle(Handle<gc::Value>(closureHandle, registers[args.returnValue])));
        }
        }
        constexpr_assert(false);
    }
    constexpr_assert(false);
    return value::ValueHandle();
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
    return handleScope.escapeHandle(Handle<gc::ObjectDescriptorReference>(objectDescriptor));
}
}
}
}
