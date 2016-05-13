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

#ifndef JAVASCRIPT_TASKLETS_PARSER_TOKENIZER_H_
#define JAVASCRIPT_TASKLETS_PARSER_TOKENIZER_H_

#include "token.h"
#include "location.h"
#include "source.h"
#include "../gc.h"

namespace javascript_tasklets
{
namespace parser
{
class Tokenizer final
{
    Tokenizer(const Tokenizer &rt) = delete;
    Tokenizer &operator=(const Tokenizer &rt) = delete;
    void operator delete(void *) = delete;

private:
    struct State final
    {
        std::size_t currentLocation;
        std::uint32_t peekCodePoint;
        constexpr State() noexcept : currentLocation(), peekCodePoint()
        {
        }
        constexpr State(std::size_t currentLocation, std::uint32_t peekCodePoint) noexcept
            : currentLocation(currentLocation),
              peekCodePoint(peekCodePoint)
        {
        }
    };

private:
    const SourceHandle source;
    State currentState;
    static constexpr std::uint32_t eofCodePoint = 0xFFFFFFFFUL;
    void getCurrentCodePoint() noexcept;
    void nextCodePoint() noexcept;
    std::uint32_t parseUnicodeEscapeSequence(GC &gc);
    void parseEscapeSequence(String &processedValue, GC &gc);
    void parseExponentPart(GC &gc);

public:
    explicit Tokenizer(const SourceHandle &source);
    SourceHandle getSource() const
    {
        return source;
    }
    Handle<Token> next(GC &gc);
    Handle<Token> reparseAsTemplateContinuation(Handle<Token> token, GC &gc);
    Handle<Token> reparseAsRegExp(Handle<Token> token, GC &gc);
};
}
namespace gc
{
template <>
struct AddHandleToHandleScope<parser::Tokenizer> final
{
    void operator()(HandleScope &handleScope, const parser::Tokenizer &value) const
    {
        AddHandleToHandleScope<parser::SourceHandle>()(handleScope, value.getSource());
    }
};
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_TOKENIZER_H_ */
