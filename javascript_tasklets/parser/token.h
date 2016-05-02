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

#ifndef JAVASCRIPT_TASKLETS_PARSER_TOKEN_H_
#define JAVASCRIPT_TASKLETS_PARSER_TOKEN_H_

#include "location.h"
#include "../gc.h"

namespace javascript_tasklets
{
namespace parser
{
struct Token final
{
    enum class Type
    {
        EndOfFile,
        IdentifierName,
        LBrace,
        RBrace,
        Ellipses,
        LParen,
        RParen,
        LBracket,
        RBracket,
        Period,
        Semicolon,
        Comma,
        LAngle,
        RAngle,
        LEq,
        REq,
        DoubleEqual,
        NotEqual,
        TripleEqual,
        NotDoubleEqual,
        Plus,
        Minus,
        Star,
        Percent,
        Inc,
        Dec,
        LShift,
        ARShift,
        URShift,
        Amp,
        Pipe,
        Caret,
        EMark,
        Tilde,
        LAnd,
        LOr,
        QMark,
        Colon,
        SingleEqual,
        PlusEqual,
        MinusEqual,
        StarEqual,
        PercentEqual,
        LShiftEqual,
        ARShiftEqual,
        URShiftEqual,
        AmpEqual,
        PipeEqual,
        CaretEqual,
        Arrow,
        Div,
        DivEqual,
        NumericLiteral,
        StringLiteral,
        RegExLiteral,
        NoSubstitutionTemplate,
        TemplateHead,
        TemplateMiddle,
        TemplateTail,
        BreakKW,
        CaseKW,
        CatchKW,
        ClassKW,
        ConstKW,
        ContinueKW,
        DebuggerKW,
        DefaultKW,
        DeleteKW,
        DoKW,
        ElseKW,
        ExportKW,
        ExtendsKW,
        FinallyKW,
        ForKW,
        FunctionKW,
        IfKW,
        ImportKW,
        InKW,
        InstanceOfKW,
        NewKW,
        ReturnKW,
        SuperKW,
        SwitchKW,
        ThisKW,
        ThrowKW,
        TryKW,
        TypeOfKW,
        VarKW,
        VoidKW,
        WhileKW,
        WithKW,
        YieldKW,
        EnumKW,
        AwaitKW,
        NullLiteral,
        TrueLiteral,
        FalseLiteral,
    };
    Type type;
    Location location;
    gc::StringReference processedValue;
    bool precededByLineTerminator;
    Token(Type type,
          Location location,
          gc::StringReference processedValue,
          bool precededByLineTerminator) noexcept
        : type(type),
          location(location),
          processedValue(processedValue),
          precededByLineTerminator(precededByLineTerminator)
    {
    }
};
}
namespace gc
{
template <>
struct AddHandleToHandleScope<parser::Token> final
{
    void operator()(HandleScope &handleScope, const parser::Token &value) const
    {
        AddHandleToHandleScope<parser::Location>()(handleScope, value.location);
        AddHandleToHandleScope<StringReference>()(handleScope, value.processedValue);
    }
};
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_TOKEN_H_ */
