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
        Identifier,
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
        LAngleEqual,
        RAngleEqual,
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
        RegExpLiteral,
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
    gc::StringReference supplementaryValue; // raw value for templates, flags for RegExp
    bool precededByLineTerminator;
    Token(Type type,
          Location location,
          Handle<gc::StringReference> processedValue,
          Handle<gc::StringReference> supplementaryValue,
          bool precededByLineTerminator) noexcept
        : type(type),
          location(location),
          processedValue(processedValue.get()),
          supplementaryValue(supplementaryValue.get()),
          precededByLineTerminator(precededByLineTerminator)
    {
    }
    Token() : type(), location(), processedValue(), precededByLineTerminator()
    {
    }
    class TokenSource final
    {
        friend class Token;
        TokenSource(const TokenSource &) = delete;
        TokenSource &operator=(const TokenSource &) = delete;

    private:
        const Token &token;
        explicit TokenSource(const Token &token) noexcept : token(token)
        {
        }

    public:
        TokenSource(TokenSource &&) = default;
        typedef String::const_iterator iterator;
        typedef String::const_iterator const_iterator;
        const_iterator begin() const
        {
            return token.location.source->contents.cbegin() + token.location.beginPosition;
        }
        const_iterator cbegin() const
        {
            return token.location.source->contents.cbegin() + token.location.beginPosition;
        }
        const_iterator end() const
        {
            return token.location.source->contents.cbegin() + token.location.endPosition;
        }
        const_iterator cend() const
        {
            return token.location.source->contents.cbegin() + token.location.endPosition;
        }
        std::size_t size() const
        {
            return token.location.endPosition - token.location.beginPosition;
        }
    };
    TokenSource sourceValue() const noexcept
    {
        return TokenSource(*this);
    }
    static bool isIdentifierName(Type type) noexcept
    {
        switch(type)
        {
        case Type::EndOfFile:
        case Type::LBrace:
        case Type::RBrace:
        case Type::Ellipses:
        case Type::LParen:
        case Type::RParen:
        case Type::LBracket:
        case Type::RBracket:
        case Type::Period:
        case Type::Semicolon:
        case Type::Comma:
        case Type::LAngle:
        case Type::RAngle:
        case Type::LAngleEqual:
        case Type::RAngleEqual:
        case Type::DoubleEqual:
        case Type::NotEqual:
        case Type::TripleEqual:
        case Type::NotDoubleEqual:
        case Type::Plus:
        case Type::Minus:
        case Type::Star:
        case Type::Percent:
        case Type::Inc:
        case Type::Dec:
        case Type::LShift:
        case Type::ARShift:
        case Type::URShift:
        case Type::Amp:
        case Type::Pipe:
        case Type::Caret:
        case Type::EMark:
        case Type::Tilde:
        case Type::LAnd:
        case Type::LOr:
        case Type::QMark:
        case Type::Colon:
        case Type::SingleEqual:
        case Type::PlusEqual:
        case Type::MinusEqual:
        case Type::StarEqual:
        case Type::PercentEqual:
        case Type::LShiftEqual:
        case Type::ARShiftEqual:
        case Type::URShiftEqual:
        case Type::AmpEqual:
        case Type::PipeEqual:
        case Type::CaretEqual:
        case Type::Arrow:
        case Type::Div:
        case Type::DivEqual:
        case Type::NumericLiteral:
        case Type::StringLiteral:
        case Type::RegExpLiteral:
        case Type::NoSubstitutionTemplate:
        case Type::TemplateHead:
        case Type::TemplateMiddle:
        case Type::TemplateTail:
            return false;
        case Type::Identifier:
        case Type::BreakKW:
        case Type::CaseKW:
        case Type::CatchKW:
        case Type::ClassKW:
        case Type::ConstKW:
        case Type::ContinueKW:
        case Type::DebuggerKW:
        case Type::DefaultKW:
        case Type::DeleteKW:
        case Type::DoKW:
        case Type::ElseKW:
        case Type::ExportKW:
        case Type::ExtendsKW:
        case Type::FinallyKW:
        case Type::ForKW:
        case Type::FunctionKW:
        case Type::IfKW:
        case Type::ImportKW:
        case Type::InKW:
        case Type::InstanceOfKW:
        case Type::NewKW:
        case Type::ReturnKW:
        case Type::SuperKW:
        case Type::SwitchKW:
        case Type::ThisKW:
        case Type::ThrowKW:
        case Type::TryKW:
        case Type::TypeOfKW:
        case Type::VarKW:
        case Type::VoidKW:
        case Type::WhileKW:
        case Type::WithKW:
        case Type::YieldKW:
        case Type::EnumKW:
        case Type::AwaitKW:
        case Type::NullLiteral:
        case Type::TrueLiteral:
        case Type::FalseLiteral:
            return true;
        }
        constexpr_assert(false);
        return false;
    }
    bool isIdentifierName() const noexcept
    {
        return isIdentifierName(type);
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
