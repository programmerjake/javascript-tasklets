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

#ifndef JAVASCRIPT_TASKLETS_PARSER_SOURCE_H_
#define JAVASCRIPT_TASKLETS_PARSER_SOURCE_H_

#include "../string.h"
#include "../gc.h"
#include <vector>
#include <iosfwd>

namespace javascript_tasklets
{
namespace parser
{
typedef Handle<gc::SourceReference> SourceHandle;
class Source final
{
    Source(const Source &) = delete;
    Source &operator =(const Source &) = delete;
    friend class gc::GC;
public:
    String fileName;
    String contents;
    std::vector<std::size_t> lineStartingPositions;
private:
    Source(String fileName, String contents)
        : fileName(std::move(fileName)),
          contents(std::move(contents)),
          lineStartingPositions(calculateLineStartingPositions(this->contents))
    {
    }
public:
    static std::vector<std::size_t> calculateLineStartingPositions(const String &text);
    static SourceHandle load(String fileName, std::istream &is, GC &gc);
    String getLocationString(std::size_t position) const;
    void writeLocation(std::ostream &os, std::size_t position) const;
    std::size_t getLineNumber(std::size_t position) const noexcept;
    std::size_t getColumnNumber(std::size_t position) const noexcept;
};
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_SOURCE_H_ */
