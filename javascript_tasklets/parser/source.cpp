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
#include "source.h"
#include <sstream>
#include <algorithm>

namespace javascript_tasklets
{
namespace parser
{
std::vector<std::size_t> Source::calculateLineStartingPositions(const String &text)
{
    std::size_t lineStartingPositionCount = 0;
    for(std::size_t i = 0; i < text.size(); i++)
    {
        if(text[i] == u'\r')
        {
            lineStartingPositionCount++;
            if(i + 1 < text.size() && text[i + 1] == u'\n')
                i++;
            continue;
        }
        if(text[i] == u'\n' || text[i] == u'\u2028' || text[i] == u'\u2029')
            lineStartingPositionCount++;
    }
    std::vector<std::size_t> lineStartingPositions;
    lineStartingPositions.reserve(lineStartingPositionCount);
    for(std::size_t i = 0; i < text.size(); i++)
    {
        if(text[i] == u'\r')
        {
            if(i + 1 < text.size() && text[i + 1] == u'\n')
                i++;
            lineStartingPositions.push_back(i + 1);
            continue;
        }
        if(text[i] == u'\n' || text[i] == u'\u2028' || text[i] == u'\u2029')
            lineStartingPositions.push_back(i + 1);
    }
    return lineStartingPositions;
}

SourceHandle Source::load(String fileName, std::istream &is, GC &gc)
{
    std::string contents;
    while(is.peek() != std::char_traits<char>::eof())
    {
        contents.push_back(is.get());
    }
    return gc.createSource(std::move(fileName), string_cast<String>(contents));
}

String Source::getLocationString(std::size_t position) const
{
    std::ostringstream ss;
    writeLocation(ss, position);
    return string_cast<String>(ss.str());
}

void Source::writeLocation(std::ostream &os, std::size_t position) const
{
    writeString(os, fileName);
    os << ":";
    os << getLineNumber(position);
    os << ":";
    os << getColumnNumber(position);
}

std::size_t Source::getLineNumber(std::size_t position) const noexcept
{
    return 1 + lineStartingPositions.size()
           + (lineStartingPositions.rbegin() - std::lower_bound(lineStartingPositions.rbegin(),
                                                                lineStartingPositions.rend(),
                                                                position,
                                                                [](std::size_t a, std::size_t b)
                                                                {
                                                                    return a > b;
                                                                }));
}

std::size_t Source::getColumnNumber(std::size_t position) const noexcept
{
    std::size_t lineNumber = getLineNumber(position);
    if(lineNumber <= 1)
        return position + 1;
    return position - lineStartingPositions[lineNumber - 2] + 1;
}
}
}
