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

#ifndef JAVASCRIPT_TASKLETS_VM_INTERPRETER_H_
#define JAVASCRIPT_TASKLETS_VM_INTERPRETER_H_

#include "../value.h"
#include "../parser/location.h"

namespace javascript_tasklets
{
namespace vm
{
namespace interpreter
{
struct BaseIndex
{
    std::size_t index;
    static constexpr std::size_t noIndex = static_cast<std::size_t>(-1);
    explicit constexpr BaseIndex(std::size_t index) noexcept
        : index((constexpr_assert(index != noIndex), index))
    {
    }
    explicit constexpr BaseIndex() noexcept : index(noIndex)
    {
    }
    constexpr bool empty() const noexcept
    {
        return index == noIndex;
    }
    constexpr bool operator==(BaseIndex rt) const noexcept
    {
        return index == rt.index;
    }
    constexpr bool operator!=(BaseIndex rt) const noexcept
    {
        return index != rt.index;
    }
    constexpr bool operator==(std::nullptr_t) const noexcept
    {
        return empty();
    }
    constexpr bool operator!=(std::nullptr_t) const noexcept
    {
        return !empty();
    }
    friend constexpr bool operator==(std::nullptr_t, BaseIndex v) noexcept
    {
        return v.empty();
    }
    friend constexpr bool operator!=(std::nullptr_t, BaseIndex v) noexcept
    {
        return !v.empty();
    }
};

struct RegisterIndex final : public BaseIndex
{
    using BaseIndex::BaseIndex;
};

struct InstructionAddress final : public BaseIndex
{
    using BaseIndex::BaseIndex;
};

#define JAVASCRIPT_TASKLETS_INSTRUCTIONS()                                           \
    JAVASCRIPT_TASKLETS_BINARY(AddU) /* uint32 + uint32 -> uint32 */                 \
    JAVASCRIPT_TASKLETS_BINARY(AddI) /* int32 + int32 -> int32 */                    \
    JAVASCRIPT_TASKLETS_BINARY(AddD) /* double + double -> double */                 \
    JAVASCRIPT_TASKLETS_BINARY(SubU) /* uint32 - uint32 -> uint32 */                 \
    JAVASCRIPT_TASKLETS_BINARY(SubI) /* int32 - int32 -> int32 */                    \
    JAVASCRIPT_TASKLETS_BINARY(SubD) /* double - double -> double */                 \
    JAVASCRIPT_TASKLETS_BINARY(MulU) /* uint32 * uint32 -> uint32 */                 \
    JAVASCRIPT_TASKLETS_BINARY(MulI) /* int32 * int32 -> int32 */                    \
    JAVASCRIPT_TASKLETS_BINARY(MulD) /* double * double -> double */                 \
    JAVASCRIPT_TASKLETS_BINARY(DivU) /* uint32 / uint32 -> uint32 */                 \
    JAVASCRIPT_TASKLETS_BINARY(DivI) /* int32 / int32 -> int32 */                    \
    JAVASCRIPT_TASKLETS_BINARY(DivD) /* double / double -> double */                 \
    JAVASCRIPT_TASKLETS_BINARY(ModU) /* uint32 % uint32 -> uint32 */                 \
    JAVASCRIPT_TASKLETS_BINARY(ModI) /* int32 % int32 -> int32 */                    \
    JAVASCRIPT_TASKLETS_BINARY(ModD) /* double % double -> double */                 \
    JAVASCRIPT_TASKLETS_BINARY(ShlU) /* uint32 << uint32 -> uint32 */                \
    JAVASCRIPT_TASKLETS_BINARY(ShlI) /* int32 << int32 -> int32 */                   \
    JAVASCRIPT_TASKLETS_BINARY(ShrU) /* uint32 >> uint32 -> uint32 */                \
    JAVASCRIPT_TASKLETS_BINARY(ShrI) /* uint32 >> int32 -> int32 */                  \
    JAVASCRIPT_TASKLETS_BINARY(SarU) /* int32 >> uint32 -> uint32 */                 \
    JAVASCRIPT_TASKLETS_BINARY(SarI) /* int32 >> int32 -> int32 */                   \
    JAVASCRIPT_TASKLETS_BINARY(AndU) /* uint32 & uint32 -> uint32 */                 \
    JAVASCRIPT_TASKLETS_BINARY(AndI) /* int32 & int32 -> int32 */                    \
    JAVASCRIPT_TASKLETS_BINARY(OrU) /* uint32 | uint32 -> uint32 */                  \
    JAVASCRIPT_TASKLETS_BINARY(OrI) /* int32 | int32 -> int32 */                     \
    JAVASCRIPT_TASKLETS_BINARY(XorU) /* uint32 ^ uint32 -> uint32 */                 \
    JAVASCRIPT_TASKLETS_BINARY(XorI) /* int32 ^ int32 -> int32 */                    \
    JAVASCRIPT_TASKLETS_BINARY(CmpEqI) /* int32 == int32 -> bool */                  \
    JAVASCRIPT_TASKLETS_BINARY(CmpEqUI) /* uint32 == int32 -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(CmpEqD) /* double == double -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(CmpEqS) /* string == string -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(CmpEqON) /* object|null == object|null -> bool */     \
    JAVASCRIPT_TASKLETS_BINARY(CmpEqSy) /* symbol == symbol -> bool */               \
    JAVASCRIPT_TASKLETS_BINARY(CmpNEI) /* int32 != int32 -> bool */                  \
    JAVASCRIPT_TASKLETS_BINARY(CmpNEUI) /* uint32 != int32 -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(CmpNED) /* double != double -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(CmpNES) /* string != string -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(CmpNEON) /* object|null != object|null -> bool */     \
    JAVASCRIPT_TASKLETS_BINARY(CmpNESy) /* symbol != symbol -> bool */               \
    JAVASCRIPT_TASKLETS_BINARY(CmpLTI) /* int32 < int32 -> bool */                   \
    JAVASCRIPT_TASKLETS_BINARY(CmpLTUI) /* uint32 < int32 -> bool */                 \
    JAVASCRIPT_TASKLETS_BINARY(CmpLTIU) /* int32 < uint32 -> bool */                 \
    JAVASCRIPT_TASKLETS_BINARY(CmpLTU) /* uint32 < uint32 -> bool */                 \
    JAVASCRIPT_TASKLETS_BINARY(CmpLTD) /* double < double -> bool */                 \
    JAVASCRIPT_TASKLETS_BINARY(CmpNLTD) /* !(double < double) -> bool */             \
    JAVASCRIPT_TASKLETS_BINARY(CmpLTS) /* string < string -> bool */                 \
    JAVASCRIPT_TASKLETS_BINARY(CmpLEI) /* int32 <= int32 -> bool */                  \
    JAVASCRIPT_TASKLETS_BINARY(CmpLEUI) /* uint32 <= int32 -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(CmpLEIU) /* int32 <= uint32 -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(CmpLEU) /* uint32 <= uint32 -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(CmpLED) /* double <= double -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(CmpNLED) /* !(double <= double) -> bool */            \
    JAVASCRIPT_TASKLETS_BINARY(CmpLES) /* string <= string -> bool */                \
    JAVASCRIPT_TASKLETS_BINARY(ConcatS) /* string + string -> string */              \
    JAVASCRIPT_TASKLETS_BINARY(MathATan2D) /* Math.atan2(double, double -> double */ \
    JAVASCRIPT_TASKLETS_UNARY(MathAbsI) /* Math.abs(int32) -> int32 */               \
    JAVASCRIPT_TASKLETS_UNARY(MathAbsD) /* Math.abs(double) -> double */             \
    JAVASCRIPT_TASKLETS_UNARY(MathACosD) /* Math.acos(double) -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(MathACosHD) /* Math.acosh(double) -> double */         \
    JAVASCRIPT_TASKLETS_UNARY(MathASinD) /* Math.asin(double) -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(MathASinHD) /* Math.asinh(double) -> double */         \
    JAVASCRIPT_TASKLETS_UNARY(MathATanD) /* Math.atan(double) -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(MathATanHD) /* Math.atanh(double) -> double */         \
    JAVASCRIPT_TASKLETS_UNARY(MathCbrtD) /* Math.cbrt(double) -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(MathCeilD) /* Math.ceil(double) -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(MathClz32U) /* Math.clz32(uint32) -> uint32 */         \
    JAVASCRIPT_TASKLETS_UNARY(MathCosD) /* Math.cos(double) -> double */             \
    JAVASCRIPT_TASKLETS_UNARY(MathCosHD) /* Math.cosh(double) -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(MathExpD) /* Math.exp(double) -> double */             \
    JAVASCRIPT_TASKLETS_UNARY(MathExpM1D) /* Math.expm1(double) -> double */         \
    JAVASCRIPT_TASKLETS_UNARY(MathFloorD) /* Math.floor(double) -> double */         \
    JAVASCRIPT_TASKLETS_UNARY(MathFRoundD) /* Math.fround(double) -> double */       \
    JAVASCRIPT_TASKLETS_UNARY(MathLogD) /* Math.log(double) -> double */             \
    JAVASCRIPT_TASKLETS_UNARY(MathLog1PD) /* Math.log1p(double) -> double */         \
    JAVASCRIPT_TASKLETS_UNARY(MathLog10D) /* Math.log10(double) -> double */         \
    JAVASCRIPT_TASKLETS_UNARY(MathLog2D) /* Math.log2(double) -> double */           \
    JAVASCRIPT_TASKLETS_BINARY(MathMaxD) /* Math.max(double, double) -> double */    \
    JAVASCRIPT_TASKLETS_BINARY(MathMinD) /* Math.min(double, double) -> double */    \
    JAVASCRIPT_TASKLETS_BINARY(MathPowD) /* Math.pow(double, double) -> double */    \
    JAVASCRIPT_TASKLETS_UNARY(MathRoundD) /* Math.round(double) -> double */         \
    JAVASCRIPT_TASKLETS_UNARY(MathSignD) /* Math.sign(double) -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(MathSignI) /* Math.sign(int32) -> int32 */             \
    JAVASCRIPT_TASKLETS_UNARY(MathSinD) /* Math.sin(double) -> double */             \
    JAVASCRIPT_TASKLETS_UNARY(MathSinHD) /* Math.sinh(double) -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(MathSqrtD) /* Math.sqrt(double) -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(MathTanD) /* Math.tan(double) -> double */             \
    JAVASCRIPT_TASKLETS_UNARY(MathTanHD) /* Math.tanh(double) -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(MathTruncD) /* Math.trunc(double) -> double */         \
    JAVASCRIPT_TASKLETS_UNARY_DEOPT(StringLengthU) /* string.length -> uint32 */     \
    JAVASCRIPT_TASKLETS_UNARY(StringLengthD) /* string.length -> double */           \
    JAVASCRIPT_TASKLETS_UNARY(Move) /* any -> any */                                 \
    JAVASCRIPT_TASKLETS_JUMP(Jump) /* goto target */                                 \
    JAVASCRIPT_TASKLETS_COND_JUMP(JumpIfTrue) /* if(bool) goto target */             \
    JAVASCRIPT_TASKLETS_COND_JUMP(JumpIfFalse) /* if(!bool) goto target */           \
    JAVASCRIPT_TASKLETS_RETURN(Return) /* return any */                              \
    JAVASCRIPT_TASKLETS_END()


struct Instruction
{
    struct DestReg
    {
        RegisterIndex dest;
        constexpr explicit DestReg(RegisterIndex dest) : dest(dest)
        {
        }
        template <typename Fn>
        void forEachSourceRegister(Fn &&fn) const noexcept
        {
        }
        template <typename Fn>
        void forEachDestRegister(Fn &&fn) const
            noexcept(noexcept(std::declval<Fn>()(std::declval<const RegisterIndex &>())))
        {
            std::forward<Fn>(fn)(dest);
        }
    };
    struct UnaryOp : public DestReg
    {
        RegisterIndex source1;
        constexpr UnaryOp(RegisterIndex dest, RegisterIndex source1) noexcept : DestReg(dest),
                                                                                source1(source1)
        {
        }
        template <typename Fn>
        void forEachSourceRegister(Fn &&fn) const
            noexcept(noexcept(std::declval<DestReg>().forEachSourceRegister(std::declval<Fn>()))
                     && noexcept(std::declval<Fn>()(std::declval<const RegisterIndex &>())))
        {
            DestReg::forEachSourceRegister(std::forward<Fn>(fn));
            std::forward<Fn>(fn)(source1);
        }
    };
    struct BinaryOp : public UnaryOp
    {
        RegisterIndex source2;
        constexpr BinaryOp(RegisterIndex dest,
                           RegisterIndex source1,
                           RegisterIndex source2) noexcept : UnaryOp(dest, source1),
                                                             source2(source2)
        {
        }
        template <typename Fn>
        void forEachSourceRegister(Fn &&fn) const
            noexcept(noexcept(std::declval<UnaryOp>().forEachSourceRegister(std::declval<Fn>()))
                     && noexcept(std::declval<Fn>()(std::declval<const RegisterIndex &>())))
        {
            UnaryOp::forEachSourceRegister(std::forward<Fn>(fn));
            std::forward<Fn>(fn)(source2);
        }
    };
    struct UnaryOpThrow : public UnaryOp
    {
        using UnaryOp::UnaryOp;
    };
    struct BinaryOpThrow : public BinaryOp
    {
        using BinaryOp::BinaryOp;
    };
    struct UnaryOpDeoptimize : public UnaryOp
    {
        InstructionAddress deoptimizeTarget;
        constexpr UnaryOpDeoptimize(RegisterIndex dest,
                                    RegisterIndex source1,
                                    InstructionAddress deoptimizeTarget) noexcept
            : UnaryOp(dest, source1),
              deoptimizeTarget(deoptimizeTarget)
        {
        }
    };
    struct BinaryOpDeoptimize : public BinaryOp
    {
        InstructionAddress deoptimizeTarget;
        constexpr BinaryOpDeoptimize(RegisterIndex dest,
                                     RegisterIndex source1,
                                     RegisterIndex source2,
                                     InstructionAddress deoptimizeTarget) noexcept
            : BinaryOp(dest, source1, source2),
              deoptimizeTarget(deoptimizeTarget)
        {
        }
    };
    struct Jump
    {
        InstructionAddress jumpTarget;
        constexpr explicit Jump(InstructionAddress jumpTarget) noexcept : jumpTarget(jumpTarget)
        {
        }
        template <typename Fn>
        void forEachSourceRegister(Fn &&fn) const noexcept
        {
        }
        template <typename Fn>
        void forEachDestRegister(Fn &&fn) const noexcept
        {
        }
    };
    struct ConditionalJump : public Jump
    {
        RegisterIndex condition;
        constexpr ConditionalJump(RegisterIndex condition, InstructionAddress jumpTarget) noexcept
            : Jump(jumpTarget),
              condition(condition)
        {
        }
        template <typename Fn>
        void forEachSourceRegister(Fn &&fn) const
            noexcept(noexcept(std::declval<Jump>().forEachSourceRegister(std::declval<Fn>()))
                     && noexcept(std::declval<Fn>()(std::declval<const RegisterIndex &>())))
        {
            Jump::forEachSourceRegister(std::forward<Fn>(fn));
            std::forward<Fn>(fn)(condition);
        }
    };
    struct Return
    {
        RegisterIndex returnValue;
        constexpr explicit Return(RegisterIndex returnValue) noexcept : returnValue(returnValue)
        {
        }
        template <typename Fn>
        void forEachSourceRegister(Fn &&fn) const
            noexcept(noexcept(std::declval<Fn>()(std::declval<const RegisterIndex &>())))
        {
            std::forward<Fn>(fn)(returnValue);
        }
        template <typename Fn>
        void forEachDestRegister(Fn &&fn) const noexcept
        {
        }
    };
#define JAVASCRIPT_TASKLETS_BINARY(name)    \
    struct Op##name final : public BinaryOp \
    {                                       \
        using BinaryOp::BinaryOp;           \
    };
#define JAVASCRIPT_TASKLETS_UNARY(name)    \
    struct Op##name final : public UnaryOp \
    {                                      \
        using UnaryOp::UnaryOp;            \
    };
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)   \
    struct Op##name final : public BinaryOpThrow \
    {                                            \
        using BinaryOpThrow::BinaryOpThrow;      \
    };
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)   \
    struct Op##name final : public UnaryOpThrow \
    {                                           \
        using UnaryOpThrow::UnaryOpThrow;       \
    };
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)        \
    struct Op##name final : public BinaryOpDeoptimize \
    {                                                 \
        using BinaryOpDeoptimize::BinaryOpDeoptimize; \
    };
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)        \
    struct Op##name final : public UnaryOpDeoptimize \
    {                                                \
        using UnaryOpDeoptimize::UnaryOpDeoptimize;  \
    };
#define JAVASCRIPT_TASKLETS_JUMP(name)  \
    struct Op##name final : public Jump \
    {                                   \
        using Jump::Jump;               \
    };
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)        \
    struct Op##name final : public ConditionalJump \
    {                                              \
        using ConditionalJump::ConditionalJump;    \
    };
#define JAVASCRIPT_TASKLETS_RETURN(name)  \
    struct Op##name final : public Return \
    {                                     \
        using Return::Return;             \
    };
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

    enum class Type
    {
#define JAVASCRIPT_TASKLETS_BINARY(name) name,
#define JAVASCRIPT_TASKLETS_UNARY(name) name,
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name) name,
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name) name,
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name) name,
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name) name,
#define JAVASCRIPT_TASKLETS_JUMP(name) name,
#define JAVASCRIPT_TASKLETS_COND_JUMP(name) name,
#define JAVASCRIPT_TASKLETS_RETURN(name) name,
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
            Empty
    };

private:
    Type type;
    union
    {
#define JAVASCRIPT_TASKLETS_BINARY(name) Op##name value##name;
#define JAVASCRIPT_TASKLETS_UNARY(name) Op##name value##name;
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name) Op##name value##name;
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name) Op##name value##name;
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name) Op##name value##name;
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name) Op##name value##name;
#define JAVASCRIPT_TASKLETS_JUMP(name) Op##name value##name;
#define JAVASCRIPT_TASKLETS_COND_JUMP(name) Op##name value##name;
#define JAVASCRIPT_TASKLETS_RETURN(name) Op##name value##name;
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
        char valueEmpty;
    };

public:
    constexpr Type getType() const noexcept
    {
        return type;
    }
#define JAVASCRIPT_TASKLETS_BINARY(name)                                \
    static Instruction make##name(parser::Location location,            \
                                  RegisterIndex dest,                   \
                                  RegisterIndex source1,                \
                                  RegisterIndex source2)                \
    {                                                                   \
        return Instruction(location, Op##name(dest, source1, source2)); \
    }
#define JAVASCRIPT_TASKLETS_UNARY(name)                                       \
    static Instruction make##name(                                            \
        parser::Location location, RegisterIndex dest, RegisterIndex source1) \
    {                                                                         \
        return Instruction(location, Op##name(dest, source1));                \
    }
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)                          \
    static Instruction make##name(parser::Location location,            \
                                  RegisterIndex dest,                   \
                                  RegisterIndex source1,                \
                                  RegisterIndex source2)                \
    {                                                                   \
        return Instruction(location, Op##name(dest, source1, source2)); \
    }
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)                                 \
    static Instruction make##name(                                            \
        parser::Location location, RegisterIndex dest, RegisterIndex source1) \
    {                                                                         \
        return Instruction(location, Op##name(dest, source1));                \
    }
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)                                            \
    static Instruction make##name(parser::Location location,                              \
                                  RegisterIndex dest,                                     \
                                  RegisterIndex source1,                                  \
                                  RegisterIndex source2,                                  \
                                  InstructionAddress deoptimizeTarget)                    \
    {                                                                                     \
        return Instruction(location, Op##name(dest, source1, source2, deoptimizeTarget)); \
    }
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)                                    \
    static Instruction make##name(parser::Location location,                     \
                                  RegisterIndex dest,                            \
                                  RegisterIndex source1,                         \
                                  InstructionAddress deoptimizeTarget)           \
    {                                                                            \
        return Instruction(location, Op##name(dest, source1, deoptimizeTarget)); \
    }
#define JAVASCRIPT_TASKLETS_JUMP(name)                                                      \
    static Instruction make##name(parser::Location location, InstructionAddress jumpTarget) \
    {                                                                                       \
        return Instruction(location, Op##name(jumpTarget));                                 \
    }
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)                                                \
    static Instruction make##name(                                                         \
        parser::Location location, RegisterIndex condition, InstructionAddress jumpTarget) \
    {                                                                                      \
        return Instruction(location, Op##name(condition, jumpTarget));                     \
    }
#define JAVASCRIPT_TASKLETS_RETURN(name)                                                \
    static Instruction make##name(parser::Location location, RegisterIndex returnValue) \
    {                                                                                   \
        return Instruction(location, Op##name(returnValue));                            \
    }
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

#define JAVASCRIPT_TASKLETS_BINARY(name)       \
    Op##name &get##name() noexcept             \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }                                          \
    const Op##name &get##name() const noexcept \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }
#define JAVASCRIPT_TASKLETS_UNARY(name)        \
    Op##name &get##name() noexcept             \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }                                          \
    const Op##name &get##name() const noexcept \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name) \
    Op##name &get##name() noexcept             \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }                                          \
    const Op##name &get##name() const noexcept \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)  \
    Op##name &get##name() noexcept             \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }                                          \
    const Op##name &get##name() const noexcept \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name) \
    Op##name &get##name() noexcept             \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }                                          \
    const Op##name &get##name() const noexcept \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)  \
    Op##name &get##name() noexcept             \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }                                          \
    const Op##name &get##name() const noexcept \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }
#define JAVASCRIPT_TASKLETS_JUMP(name)         \
    Op##name &get##name() noexcept             \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }                                          \
    const Op##name &get##name() const noexcept \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)    \
    Op##name &get##name() noexcept             \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }                                          \
    const Op##name &get##name() const noexcept \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }
#define JAVASCRIPT_TASKLETS_RETURN(name)       \
    Op##name &get##name() noexcept             \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }                                          \
    const Op##name &get##name() const noexcept \
    {                                          \
        constexpr_assert(type == Type::name);  \
        return value##name;                    \
    }
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

#define JAVASCRIPT_TASKLETS_BINARY(name)                                            \
    constexpr Instruction(parser::Location location, Op##name value##name) noexcept \
        : type(Type::name),                                                         \
          value##name(value##name),                                                 \
          location(location)                                                        \
    {                                                                               \
    }
#define JAVASCRIPT_TASKLETS_UNARY(name)                                             \
    constexpr Instruction(parser::Location location, Op##name value##name) noexcept \
        : type(Type::name),                                                         \
          value##name(value##name),                                                 \
          location(location)                                                        \
    {                                                                               \
    }
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)                                      \
    constexpr Instruction(parser::Location location, Op##name value##name) noexcept \
        : type(Type::name),                                                         \
          value##name(value##name),                                                 \
          location(location)                                                        \
    {                                                                               \
    }
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)                                       \
    constexpr Instruction(parser::Location location, Op##name value##name) noexcept \
        : type(Type::name),                                                         \
          value##name(value##name),                                                 \
          location(location)                                                        \
    {                                                                               \
    }
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)                                      \
    constexpr Instruction(parser::Location location, Op##name value##name) noexcept \
        : type(Type::name),                                                         \
          value##name(value##name),                                                 \
          location(location)                                                        \
    {                                                                               \
    }
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)                                       \
    constexpr Instruction(parser::Location location, Op##name value##name) noexcept \
        : type(Type::name),                                                         \
          value##name(value##name),                                                 \
          location(location)                                                        \
    {                                                                               \
    }
#define JAVASCRIPT_TASKLETS_JUMP(name)                                              \
    constexpr Instruction(parser::Location location, Op##name value##name) noexcept \
        : type(Type::name),                                                         \
          value##name(value##name),                                                 \
          location(location)                                                        \
    {                                                                               \
    }
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)                                         \
    constexpr Instruction(parser::Location location, Op##name value##name) noexcept \
        : type(Type::name),                                                         \
          value##name(value##name),                                                 \
          location(location)                                                        \
    {                                                                               \
    }
#define JAVASCRIPT_TASKLETS_RETURN(name)                                            \
    constexpr Instruction(parser::Location location, Op##name value##name) noexcept \
        : type(Type::name),                                                         \
          value##name(value##name),                                                 \
          location(location)                                                        \
    {                                                                               \
    }
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

private:
    void destruct() noexcept
    {
        switch(type)
        {
#define JAVASCRIPT_TASKLETS_BINARY(name) \
    case Type::name:                     \
        value##name.~Op##name();         \
        return;
#define JAVASCRIPT_TASKLETS_UNARY(name) \
    case Type::name:                    \
        value##name.~Op##name();        \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name) \
    case Type::name:                           \
        value##name.~Op##name();               \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name) \
    case Type::name:                          \
        value##name.~Op##name();              \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name) \
    case Type::name:                           \
        value##name.~Op##name();               \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name) \
    case Type::name:                          \
        value##name.~Op##name();              \
        return;
#define JAVASCRIPT_TASKLETS_JUMP(name) \
    case Type::name:                   \
        value##name.~Op##name();       \
        return;
#define JAVASCRIPT_TASKLETS_COND_JUMP(name) \
    case Type::name:                        \
        value##name.~Op##name();            \
        return;
#define JAVASCRIPT_TASKLETS_RETURN(name) \
    case Type::name:                     \
        value##name.~Op##name();         \
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

    void copyConstruct(const Instruction &rt) noexcept
    {
        type = rt.type;
        switch(type)
        {
#define JAVASCRIPT_TASKLETS_BINARY(name)              \
    case Type::name:                                  \
        ::new(&value##name) Op##name(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY(name)               \
    case Type::name:                                  \
        ::new(&value##name) Op##name(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)        \
    case Type::name:                                  \
        ::new(&value##name) Op##name(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)         \
    case Type::name:                                  \
        ::new(&value##name) Op##name(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)        \
    case Type::name:                                  \
        ::new(&value##name) Op##name(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)         \
    case Type::name:                                  \
        ::new(&value##name) Op##name(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_JUMP(name)                \
    case Type::name:                                  \
        ::new(&value##name) Op##name(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)           \
    case Type::name:                                  \
        ::new(&value##name) Op##name(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_RETURN(name)              \
    case Type::name:                                  \
        ::new(&value##name) Op##name(rt.value##name); \
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

    void moveConstruct(Instruction &rt) noexcept
    {
        type = rt.type;
        switch(type)
        {
#define JAVASCRIPT_TASKLETS_BINARY(name)                         \
    case Type::name:                                             \
        ::new(&value##name) Op##name(std::move(rt.value##name)); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY(name)                          \
    case Type::name:                                             \
        ::new(&value##name) Op##name(std::move(rt.value##name)); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)                   \
    case Type::name:                                             \
        ::new(&value##name) Op##name(std::move(rt.value##name)); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)                    \
    case Type::name:                                             \
        ::new(&value##name) Op##name(std::move(rt.value##name)); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)                   \
    case Type::name:                                             \
        ::new(&value##name) Op##name(std::move(rt.value##name)); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)                    \
    case Type::name:                                             \
        ::new(&value##name) Op##name(std::move(rt.value##name)); \
        return;
#define JAVASCRIPT_TASKLETS_JUMP(name)                           \
    case Type::name:                                             \
        ::new(&value##name) Op##name(std::move(rt.value##name)); \
        return;
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)                      \
    case Type::name:                                             \
        ::new(&value##name) Op##name(std::move(rt.value##name)); \
        return;
#define JAVASCRIPT_TASKLETS_RETURN(name)                         \
    case Type::name:                                             \
        ::new(&value##name) Op##name(std::move(rt.value##name)); \
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

    void copyAssign(const Instruction &rt) noexcept
    {
        constexpr_assert(type == rt.type);
        switch(type)
        {
#define JAVASCRIPT_TASKLETS_BINARY(name) \
    case Type::name:                     \
        value##name = rt.value##name;    \
        return;
#define JAVASCRIPT_TASKLETS_UNARY(name) \
    case Type::name:                    \
        value##name = rt.value##name;   \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name) \
    case Type::name:                           \
        value##name = rt.value##name;          \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name) \
    case Type::name:                          \
        value##name = rt.value##name;         \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name) \
    case Type::name:                           \
        value##name = rt.value##name;          \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name) \
    case Type::name:                          \
        value##name = rt.value##name;         \
        return;
#define JAVASCRIPT_TASKLETS_JUMP(name) \
    case Type::name:                   \
        value##name = rt.value##name;  \
        return;
#define JAVASCRIPT_TASKLETS_COND_JUMP(name) \
    case Type::name:                        \
        value##name = rt.value##name;       \
        return;
#define JAVASCRIPT_TASKLETS_RETURN(name) \
    case Type::name:                     \
        value##name = rt.value##name;    \
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

    void moveAssign(Instruction &rt) noexcept
    {
        constexpr_assert(type == rt.type);
        switch(type)
        {
#define JAVASCRIPT_TASKLETS_BINARY(name)         \
    case Type::name:                             \
        value##name = std::move(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY(name)          \
    case Type::name:                             \
        value##name = std::move(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)   \
    case Type::name:                             \
        value##name = std::move(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)    \
    case Type::name:                             \
        value##name = std::move(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)   \
    case Type::name:                             \
        value##name = std::move(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)    \
    case Type::name:                             \
        value##name = std::move(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_JUMP(name)           \
    case Type::name:                             \
        value##name = std::move(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)      \
    case Type::name:                             \
        value##name = std::move(rt.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_RETURN(name)         \
    case Type::name:                             \
        value##name = std::move(rt.value##name); \
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

public:
    parser::Location location;

public:
    constexpr Instruction() noexcept : type(Type::Empty), valueEmpty(), location()
    {
    }
    Instruction(const Instruction &rt) noexcept : type(rt.type), valueEmpty(), location(rt.location)
    {
        copyConstruct(rt);
    }
    Instruction(Instruction &&rt) noexcept : type(rt.type),
                                             valueEmpty(),
                                             location(std::move(rt.location))
    {
        moveConstruct(rt);
    }
    Instruction &operator=(const Instruction &rt) noexcept
    {
        location = rt.location;
        if(type == rt.type)
        {
            copyAssign(rt);
        }
        else
        {
            destruct();
            type = rt.type;
            copyConstruct(rt);
        }
        return *this;
    }
    Instruction &operator=(Instruction &&rt) noexcept
    {
        location = std::move(rt.location);
        if(type == rt.type)
        {
            moveAssign(rt);
        }
        else
        {
            destruct();
            type = rt.type;
            moveConstruct(rt);
        }
        return *this;
    }
    template <typename Fn>
    static void apply(Instruction &instruction,
                      Fn &&fn) noexcept(noexcept(std::forward<Fn>(fn)())
#define JAVASCRIPT_TASKLETS_BINARY(name) \
    &&noexcept(std::declval<Fn>()(std::declval<parser::Location &>(), std::declval<Op##name &>()))
#define JAVASCRIPT_TASKLETS_UNARY(name) \
    &&noexcept(std::declval<Fn>()(std::declval<parser::Location &>(), std::declval<Op##name &>()))
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name) \
    &&noexcept(std::declval<Fn>()(std::declval<parser::Location &>(), std::declval<Op##name &>()))
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name) \
    &&noexcept(std::declval<Fn>()(std::declval<parser::Location &>(), std::declval<Op##name &>()))
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name) \
    &&noexcept(std::declval<Fn>()(std::declval<parser::Location &>(), std::declval<Op##name &>()))
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name) \
    &&noexcept(std::declval<Fn>()(std::declval<parser::Location &>(), std::declval<Op##name &>()))
#define JAVASCRIPT_TASKLETS_JUMP(name) \
    &&noexcept(std::declval<Fn>()(std::declval<parser::Location &>(), std::declval<Op##name &>()))
#define JAVASCRIPT_TASKLETS_COND_JUMP(name) \
    &&noexcept(std::declval<Fn>()(std::declval<parser::Location &>(), std::declval<Op##name &>()))
#define JAVASCRIPT_TASKLETS_RETURN(name) \
    &&noexcept(std::declval<Fn>()(std::declval<parser::Location &>(), std::declval<Op##name &>()))
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
                                                )
    {
        switch(instruction.type)
        {
#define JAVASCRIPT_TASKLETS_BINARY(name)                                     \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY(name)                                      \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)                               \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)                                \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)                               \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)                                \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_JUMP(name)                                       \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)                                  \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_RETURN(name)                                     \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
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
            std::forward<Fn>(fn)();
            return;
        }
        constexpr_assert(false);
    }
    template <typename Fn>
    static void apply(const Instruction &instruction,
                      Fn &&fn) noexcept(noexcept(std::forward<Fn>(fn)())
#define JAVASCRIPT_TASKLETS_BINARY(name)                                    \
    &&noexcept(std::declval<Fn>()(std::declval<const parser::Location &>(), \
                                  std::declval<const Op##name &>()))
#define JAVASCRIPT_TASKLETS_UNARY(name)                                     \
    &&noexcept(std::declval<Fn>()(std::declval<const parser::Location &>(), \
                                  std::declval<const Op##name &>()))
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)                              \
    &&noexcept(std::declval<Fn>()(std::declval<const parser::Location &>(), \
                                  std::declval<const Op##name &>()))
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)                               \
    &&noexcept(std::declval<Fn>()(std::declval<const parser::Location &>(), \
                                  std::declval<const Op##name &>()))
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)                              \
    &&noexcept(std::declval<Fn>()(std::declval<const parser::Location &>(), \
                                  std::declval<const Op##name &>()))
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)                               \
    &&noexcept(std::declval<Fn>()(std::declval<const parser::Location &>(), \
                                  std::declval<const Op##name &>()))
#define JAVASCRIPT_TASKLETS_JUMP(name)                                      \
    &&noexcept(std::declval<Fn>()(std::declval<const parser::Location &>(), \
                                  std::declval<const Op##name &>()))
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)                                 \
    &&noexcept(std::declval<Fn>()(std::declval<const parser::Location &>(), \
                                  std::declval<const Op##name &>()))
#define JAVASCRIPT_TASKLETS_RETURN(name)                                    \
    &&noexcept(std::declval<Fn>()(std::declval<const parser::Location &>(), \
                                  std::declval<const Op##name &>()))
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
                                                )
    {
        switch(instruction.type)
        {
#define JAVASCRIPT_TASKLETS_BINARY(name)                                     \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY(name)                                      \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)                               \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)                                \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)                               \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)                                \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_JUMP(name)                                       \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)                                  \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
        return;
#define JAVASCRIPT_TASKLETS_RETURN(name)                                     \
    case Type::name:                                                         \
        std::forward<Fn>(fn)(instruction.location, instruction.value##name); \
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
            std::forward<Fn>(fn)();
            return;
        }
        constexpr_assert(false);
    }
    void addHandleToHandleScope(HandleScope &handleScope) const;
    template <typename Fn>
    void apply(Fn &&fn) noexcept(noexcept(apply(std::declval<Instruction &>(), std::declval<Fn>())))
    {
        apply(*this, std::forward<Fn>(fn));
    }
    template <typename Fn>
    void apply(Fn &&fn) const
        noexcept(noexcept(apply(std::declval<const Instruction &>(), std::declval<Fn>())))
    {
        apply(*this, std::forward<Fn>(fn));
    }

private:
    template <typename Fn>
    struct ForEachDestRegisterCallback final
    {
        Fn &&fn;
        explicit constexpr ForEachDestRegisterCallback(Fn &&fn) noexcept : fn(std::forward<Fn>(fn))
        {
        }
        void operator()() const noexcept
        {
        }
        template <typename T>
        void operator()(const parser::Location &, const T &value) const
            noexcept(noexcept(value.forEachDestRegister(std::declval<Fn>())))
        {
            value.forEachDestRegister(std::forward<Fn>(fn));
        }
    };

    template <typename Fn>
    struct ForEachSourceRegisterCallback final
    {
        Fn &&fn;
        explicit constexpr ForEachSourceRegisterCallback(Fn &&fn) noexcept
            : fn(std::forward<Fn>(fn))
        {
        }
        void operator()() const noexcept
        {
        }
        template <typename T>
        void operator()(const parser::Location &, const T &value) const
            noexcept(noexcept(value.forEachSourceRegister(std::declval<Fn>())))
        {
            value.forEachSourceRegister(std::forward<Fn>(fn));
        }
    };

public:
    template <typename Fn>
    void forEachDestRegister(Fn &&fn) const
        noexcept(noexcept(apply(std::declval<const Instruction &>(),
                                std::declval<ForEachDestRegisterCallback<Fn>>())))
    {
        apply(ForEachDestRegisterCallback<Fn>(std::forward<Fn>(fn)));
    }
    template <typename Fn>
    void forEachSourceRegister(Fn &&fn) const
        noexcept(noexcept(apply(std::declval<const Instruction &>(),
                                std::declval<ForEachSourceRegisterCallback<Fn>>())))
    {
        apply(ForEachSourceRegisterCallback<Fn>(std::forward<Fn>(fn)));
    }
    template <typename Fn>
    void forEachRegister(Fn &&fn) const
        noexcept(noexcept(apply(std::declval<const Instruction &>(),
                                std::declval<ForEachDestRegisterCallback<Fn>>()))
                 && noexcept(apply(std::declval<const Instruction &>(),
                                   std::declval<ForEachSourceRegisterCallback<Fn>>())))
    {
        forEachDestRegister(std::forward<Fn>(fn));
        forEachSourceRegister(std::forward<Fn>(fn));
    }
};
struct FunctionCode final
{
    std::vector<Instruction> instructions;
    std::size_t registerCount;
    std::size_t namedArgumentCount;
    Handle<gc::ObjectDescriptorReference> closureObjectDescriptor;
    static Handle<gc::ObjectDescriptorReference> makeClosureObjectDescriptor(
        GC &gc, std::size_t registerCount);
    value::ValueHandle run(const std::vector<value::ValueHandle> &arguments, GC &gc) const;
    FunctionCode(std::vector<Instruction> instructions,
                 std::size_t registerCount,
                 std::size_t namedArgumentCount,
                 GC &gc)
        : instructions(std::move(instructions)),
          registerCount(registerCount),
          namedArgumentCount(namedArgumentCount),
          closureObjectDescriptor(makeClosureObjectDescriptor(gc, registerCount))
    {
    }
};
}
}
namespace gc
{
template <>
struct AddHandleToHandleScope<vm::interpreter::Instruction> final
{
    void operator()(HandleScope &handleScope, const vm::interpreter::Instruction &value) const
    {
        value.addHandleToHandleScope(handleScope);
    }
};
template <>
struct AddHandleToHandleScope<vm::interpreter::FunctionCode> final
{
    void operator()(HandleScope &handleScope, const vm::interpreter::FunctionCode &value) const
    {
        for(const vm::interpreter::Instruction &instruction : value.instructions)
        {
            AddHandleToHandleScope<vm::interpreter::Instruction>()(handleScope, instruction);
        }
        AddHandleToHandleScope<Handle<gc::ObjectDescriptorReference>>()(
            handleScope, value.closureObjectDescriptor);
    }
};
template <>
struct AddHandleToHandleScope<vm::interpreter::Instruction::UnaryOp> final
{
    void operator()(HandleScope &handleScope,
                    const vm::interpreter::Instruction::UnaryOp &value) const
    {
    }
};
template <>
struct AddHandleToHandleScope<vm::interpreter::Instruction::BinaryOp> final
{
    void operator()(HandleScope &handleScope,
                    const vm::interpreter::Instruction::BinaryOp &value) const
    {
        AddHandleToHandleScope<vm::interpreter::Instruction::UnaryOp>()(handleScope, value);
    }
};
template <>
struct AddHandleToHandleScope<vm::interpreter::Instruction::BinaryOpDeoptimize> final
{
    void operator()(HandleScope &handleScope,
                    const vm::interpreter::Instruction::BinaryOpDeoptimize &value) const
    {
        AddHandleToHandleScope<vm::interpreter::Instruction::BinaryOp>()(handleScope, value);
    }
};
template <>
struct AddHandleToHandleScope<vm::interpreter::Instruction::BinaryOpThrow> final
{
    void operator()(HandleScope &handleScope,
                    const vm::interpreter::Instruction::BinaryOpThrow &value) const
    {
        AddHandleToHandleScope<vm::interpreter::Instruction::BinaryOp>()(handleScope, value);
    }
};
template <>
struct AddHandleToHandleScope<vm::interpreter::Instruction::UnaryOpDeoptimize> final
{
    void operator()(HandleScope &handleScope,
                    const vm::interpreter::Instruction::UnaryOpDeoptimize &value) const
    {
        AddHandleToHandleScope<vm::interpreter::Instruction::UnaryOp>()(handleScope, value);
    }
};
template <>
struct AddHandleToHandleScope<vm::interpreter::Instruction::UnaryOpThrow> final
{
    void operator()(HandleScope &handleScope,
                    const vm::interpreter::Instruction::UnaryOpThrow &value) const
    {
        AddHandleToHandleScope<vm::interpreter::Instruction::UnaryOp>()(handleScope, value);
    }
};
template <>
struct AddHandleToHandleScope<vm::interpreter::Instruction::Jump> final
{
    void operator()(HandleScope &handleScope, const vm::interpreter::Instruction::Jump &value) const
    {
    }
};
template <>
struct AddHandleToHandleScope<vm::interpreter::Instruction::ConditionalJump> final
{
    void operator()(HandleScope &handleScope,
                    const vm::interpreter::Instruction::ConditionalJump &value) const
    {
        AddHandleToHandleScope<vm::interpreter::Instruction::Jump>()(handleScope, value);
    }
};
template <>
struct AddHandleToHandleScope<vm::interpreter::Instruction::Return> final
{
    void operator()(HandleScope &handleScope,
                    const vm::interpreter::Instruction::Return &value) const
    {
    }
};
#define JAVASCRIPT_TASKLETS_BINARY(name)                                                          \
    template <>                                                                                   \
    struct AddHandleToHandleScope<vm::interpreter::Instruction::Op##name> final                   \
    {                                                                                             \
        void operator()(HandleScope & handleScope,                                                \
                        const vm::interpreter::Instruction::Op##name &value) const                \
        {                                                                                         \
            AddHandleToHandleScope<vm::interpreter::Instruction::BinaryOp>()(handleScope, value); \
        }                                                                                         \
    };
#define JAVASCRIPT_TASKLETS_UNARY(name)                                                          \
    template <>                                                                                  \
    struct AddHandleToHandleScope<vm::interpreter::Instruction::Op##name> final                  \
    {                                                                                            \
        void operator()(HandleScope & handleScope,                                               \
                        const vm::interpreter::Instruction::Op##name &value) const               \
        {                                                                                        \
            AddHandleToHandleScope<vm::interpreter::Instruction::UnaryOp>()(handleScope, value); \
        }                                                                                        \
    };
#define JAVASCRIPT_TASKLETS_BINARY_THROW(name)                                                 \
    template <>                                                                                \
    struct AddHandleToHandleScope<vm::interpreter::Instruction::Op##name> final                \
    {                                                                                          \
        void operator()(HandleScope & handleScope,                                             \
                        const vm::interpreter::Instruction::Op##name &value) const             \
        {                                                                                      \
            AddHandleToHandleScope<vm::interpreter::Instruction::BinaryOpThrow>()(handleScope, \
                                                                                  value);      \
        }                                                                                      \
    };
#define JAVASCRIPT_TASKLETS_UNARY_THROW(name)                                                 \
    template <>                                                                               \
    struct AddHandleToHandleScope<vm::interpreter::Instruction::Op##name> final               \
    {                                                                                         \
        void operator()(HandleScope & handleScope,                                            \
                        const vm::interpreter::Instruction::Op##name &value) const            \
        {                                                                                     \
            AddHandleToHandleScope<vm::interpreter::Instruction::UnaryOpThrow>()(handleScope, \
                                                                                 value);      \
        }                                                                                     \
    };
#define JAVASCRIPT_TASKLETS_BINARY_DEOPT(name)                                          \
    template <>                                                                         \
    struct AddHandleToHandleScope<vm::interpreter::Instruction::Op##name> final         \
    {                                                                                   \
        void operator()(HandleScope & handleScope,                                      \
                        const vm::interpreter::Instruction::Op##name &value) const      \
        {                                                                               \
            AddHandleToHandleScope<vm::interpreter::Instruction::BinaryOpDeoptimize>()( \
                handleScope, value);                                                    \
        }                                                                               \
    };
#define JAVASCRIPT_TASKLETS_UNARY_DEOPT(name)                                                      \
    template <>                                                                                    \
    struct AddHandleToHandleScope<vm::interpreter::Instruction::Op##name> final                    \
    {                                                                                              \
        void operator()(HandleScope & handleScope,                                                 \
                        const vm::interpreter::Instruction::Op##name &value) const                 \
        {                                                                                          \
            AddHandleToHandleScope<vm::interpreter::Instruction::UnaryOpDeoptimize>()(handleScope, \
                                                                                      value);      \
        }                                                                                          \
    };
#define JAVASCRIPT_TASKLETS_JUMP(name)                                                        \
    template <>                                                                               \
    struct AddHandleToHandleScope<vm::interpreter::Instruction::Op##name> final               \
    {                                                                                         \
        void operator()(HandleScope & handleScope,                                            \
                        const vm::interpreter::Instruction::Op##name &value) const            \
        {                                                                                     \
            AddHandleToHandleScope<vm::interpreter::Instruction::Jump>()(handleScope, value); \
        }                                                                                     \
    };
#define JAVASCRIPT_TASKLETS_COND_JUMP(name)                                                      \
    template <>                                                                                  \
    struct AddHandleToHandleScope<vm::interpreter::Instruction::Op##name> final                  \
    {                                                                                            \
        void operator()(HandleScope & handleScope,                                               \
                        const vm::interpreter::Instruction::Op##name &value) const               \
        {                                                                                        \
            AddHandleToHandleScope<vm::interpreter::Instruction::ConditionalJump>()(handleScope, \
                                                                                    value);      \
        }                                                                                        \
    };
#define JAVASCRIPT_TASKLETS_RETURN(name)                                                        \
    template <>                                                                                 \
    struct AddHandleToHandleScope<vm::interpreter::Instruction::Op##name> final                 \
    {                                                                                           \
        void operator()(HandleScope & handleScope,                                              \
                        const vm::interpreter::Instruction::Op##name &value) const              \
        {                                                                                       \
            AddHandleToHandleScope<vm::interpreter::Instruction::Return>()(handleScope, value); \
        }                                                                                       \
    };
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
}
}

#ifndef JAVASCRIPT_TASKLETS_VM_INTERPRETER_KEEP_INSTRUCTIONS
#undef JAVASCRIPT_TASKLETS_INSTRUCTIONS
#endif

#endif /* JAVASCRIPT_TASKLETS_VM_INTERPRETER_H_ */
