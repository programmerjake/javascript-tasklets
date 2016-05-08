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
#include "../value.h"

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
    const SourceHandle source;
    std::size_t currentLocation;
    std::uint32_t peekCodePoint;
    constexpr std::uint32_t eofCodePoint = 0xFFFFFFFFUL;
    void getCurrentCodePoint() const noexcept
    {
        if(currentLocation < source.get()->contents.size())
        {
            peekCodePoint = source.get()->contents[currentLocation] & 0xFFFFUL;
            if(peekCodePoint >= 0xD800U && peekCodePoint <= 0xDBFFU
               && currentLocation + 1 < source.get()->contents.size())
            {
                std::uint32_t nextValue = source.get()->contents[currentLocation + 1] & 0xFFFFUL;
                if(nextValue >= 0xDC00U && nextValue <= 0xDFFFU)
                {
                    peekCodePoint <<= 10;
                    peekCodePoint &= 0xFFC00UL;
                    peekCodePoint |= nextValue & 0x3FFU;
                    peekCodePoint += 0x10000UL;
                }
            }
        }
        else
        {
            peekCodePoint = eofCodePoint;
        }
    }
    void nextCodePoint() const noexcept
    {
        if(peekCodePoint == eofCodePoint)
            return;
        if(peekCodePoint >= 0x10000UL)
            currentLocation += 2; // surrogate pair
        else
            currentLocation += 1;
        getCurrentCodePoint();
    }

public:
    explicit Tokenizer(const SourceHandle &source)
        : source(source), currentLocation(0), peekCodePoint(0)
    {
        getCurrentCodePoint();
    }
#error finish
};
}
namespace gc
{
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_TOKENIZER_H_ */
