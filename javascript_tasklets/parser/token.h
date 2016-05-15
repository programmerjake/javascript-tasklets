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
    static String getTypeString(Type type)
    {
        switch(type)
        {
        case Type::EndOfFile:
            return u"EndOfFile";
        case Type::Identifier:
            return u"Identifier";
        case Type::LBrace:
            return u"LBrace";
        case Type::RBrace:
            return u"RBrace";
        case Type::Ellipses:
            return u"Ellipses";
        case Type::LParen:
            return u"LParen";
        case Type::RParen:
            return u"RParen";
        case Type::LBracket:
            return u"LBracket";
        case Type::RBracket:
            return u"RBracket";
        case Type::Period:
            return u"Period";
        case Type::Semicolon:
            return u"Semicolon";
        case Type::Comma:
            return u"Comma";
        case Type::LAngle:
            return u"LAngle";
        case Type::RAngle:
            return u"RAngle";
        case Type::LAngleEqual:
            return u"LAngleEqual";
        case Type::RAngleEqual:
            return u"RAngleEqual";
        case Type::DoubleEqual:
            return u"DoubleEqual";
        case Type::NotEqual:
            return u"NotEqual";
        case Type::TripleEqual:
            return u"TripleEqual";
        case Type::NotDoubleEqual:
            return u"NotDoubleEqual";
        case Type::Plus:
            return u"Plus";
        case Type::Minus:
            return u"Minus";
        case Type::Star:
            return u"Star";
        case Type::Percent:
            return u"Percent";
        case Type::Inc:
            return u"Inc";
        case Type::Dec:
            return u"Dec";
        case Type::LShift:
            return u"LShift";
        case Type::ARShift:
            return u"ARShift";
        case Type::URShift:
            return u"URShift";
        case Type::Amp:
            return u"Amp";
        case Type::Pipe:
            return u"Pipe";
        case Type::Caret:
            return u"Caret";
        case Type::EMark:
            return u"EMark";
        case Type::Tilde:
            return u"Tilde";
        case Type::LAnd:
            return u"LAnd";
        case Type::LOr:
            return u"LOr";
        case Type::QMark:
            return u"QMark";
        case Type::Colon:
            return u"Colon";
        case Type::SingleEqual:
            return u"SingleEqual";
        case Type::PlusEqual:
            return u"PlusEqual";
        case Type::MinusEqual:
            return u"MinusEqual";
        case Type::StarEqual:
            return u"StarEqual";
        case Type::PercentEqual:
            return u"PercentEqual";
        case Type::LShiftEqual:
            return u"LShiftEqual";
        case Type::ARShiftEqual:
            return u"ARShiftEqual";
        case Type::URShiftEqual:
            return u"URShiftEqual";
        case Type::AmpEqual:
            return u"AmpEqual";
        case Type::PipeEqual:
            return u"PipeEqual";
        case Type::CaretEqual:
            return u"CaretEqual";
        case Type::Arrow:
            return u"Arrow";
        case Type::Div:
            return u"Div";
        case Type::DivEqual:
            return u"DivEqual";
        case Type::NumericLiteral:
            return u"NumericLiteral";
        case Type::StringLiteral:
            return u"StringLiteral";
        case Type::RegExpLiteral:
            return u"RegExpLiteral";
        case Type::NoSubstitutionTemplate:
            return u"NoSubstitutionTemplate";
        case Type::TemplateHead:
            return u"TemplateHead";
        case Type::TemplateMiddle:
            return u"TemplateMiddle";
        case Type::TemplateTail:
            return u"TemplateTail";
        case Type::BreakKW:
            return u"BreakKW";
        case Type::CaseKW:
            return u"CaseKW";
        case Type::CatchKW:
            return u"CatchKW";
        case Type::ClassKW:
            return u"ClassKW";
        case Type::ConstKW:
            return u"ConstKW";
        case Type::ContinueKW:
            return u"ContinueKW";
        case Type::DebuggerKW:
            return u"DebuggerKW";
        case Type::DefaultKW:
            return u"DefaultKW";
        case Type::DeleteKW:
            return u"DeleteKW";
        case Type::DoKW:
            return u"DoKW";
        case Type::ElseKW:
            return u"ElseKW";
        case Type::ExportKW:
            return u"ExportKW";
        case Type::ExtendsKW:
            return u"ExtendsKW";
        case Type::FinallyKW:
            return u"FinallyKW";
        case Type::ForKW:
            return u"ForKW";
        case Type::FunctionKW:
            return u"FunctionKW";
        case Type::IfKW:
            return u"IfKW";
        case Type::ImportKW:
            return u"ImportKW";
        case Type::InKW:
            return u"InKW";
        case Type::InstanceOfKW:
            return u"InstanceOfKW";
        case Type::NewKW:
            return u"NewKW";
        case Type::ReturnKW:
            return u"ReturnKW";
        case Type::SuperKW:
            return u"SuperKW";
        case Type::SwitchKW:
            return u"SwitchKW";
        case Type::ThisKW:
            return u"ThisKW";
        case Type::ThrowKW:
            return u"ThrowKW";
        case Type::TryKW:
            return u"TryKW";
        case Type::TypeOfKW:
            return u"TypeOfKW";
        case Type::VarKW:
            return u"VarKW";
        case Type::VoidKW:
            return u"VoidKW";
        case Type::WhileKW:
            return u"WhileKW";
        case Type::WithKW:
            return u"WithKW";
        case Type::YieldKW:
            return u"YieldKW";
        case Type::EnumKW:
            return u"EnumKW";
        case Type::AwaitKW:
            return u"AwaitKW";
        case Type::NullLiteral:
            return u"NullLiteral";
        case Type::TrueLiteral:
            return u"TrueLiteral";
        case Type::FalseLiteral:
            return u"FalseLiteral";
        }
        return u"<unknown>";
    }
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
        explicit operator String() const
        {
            return String(begin(), end());
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
