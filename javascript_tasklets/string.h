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

#ifndef JAVASCRIPT_TASKLETS_STRING_H_
#define JAVASCRIPT_TASKLETS_STRING_H_

#include <cstdint>
#include <string>
#include <cstring>
#include <functional>
#include <iosfwd>

#if 0
namespace javascript_tasklets
{
struct Char16Traits final
{
    typedef std::uint16_t char_type;
    typedef std::int_least32_t int_type;
    typedef std::streamoff off_type;
    typedef std::streampos pos_type;
    typedef std::mbstate_t state_type;
    static void assign(char_type &dest, const char_type &source) noexcept
    {
        dest = source;
    }
    static char_type *assign(char_type *dest, std::size_t count, char_type value) noexcept
    {
        for(std::size_t i = 0; i < count; i++)
        {
            dest[i] = value;
        }
        return dest;
    }
    static constexpr bool eq(char_type l, char_type r) noexcept
    {
        return l == r;
    }
    static constexpr bool lt(char_type l, char_type r) noexcept
    {
        return l < r;
    }
    static char_type *move(char_type *dest, const char_type *src, std::size_t count) noexcept
    {
        std::memmove(
            static_cast<void *>(dest), static_cast<const void *>(src), count * sizeof(char_type));
        return dest;
    }
    static char_type *copy(char_type *dest, const char_type *src, std::size_t count) noexcept
    {
        std::memcpy(
            static_cast<void *>(dest), static_cast<const void *>(src), count * sizeof(char_type));
        return dest;
    }
    static int compare(const char_type *l, const char_type *r, std::size_t count) noexcept
    {
        for(std::size_t i = 0; i < count; i++)
        {
            if(l[i] < r[i])
                return -1;
            if(l[i] > r[i])
                return 1;
        }
        return 0;
    }
    static std::size_t length(const char_type *str) noexcept
    {
        std::size_t retval = 0;
        while(*str++ != 0)
        {
            retval++;
        }
        return retval;
    }
    static const char_type *find(const char_type *str,
                                 std::size_t count,
                                 const char_type &searchFor) noexcept
    {
        for(std::size_t i = 0; i < count; i++)
        {
            if(str[i] == searchFor)
            {
                return &str[i];
            }
        }
        return nullptr;
    }
    static constexpr char_type to_char_type(int_type v) noexcept
    {
        return v;
    }
    static constexpr int_type to_int_type(char_type v) noexcept
    {
        return v;
    }
    static constexpr bool eq_int_type(int_type l, int_type r) noexcept
    {
        return l == r;
    }
    static constexpr int_type eof() noexcept
    {
        return -1;
    }
    static constexpr int_type not_eof(int_type v) noexcept
    {
        return (v == eof()) ? 0 : v;
    }
};

typedef std::basic_string<std::uint16_t, Char16Traits> String;
}

namespace std
{
template <>
struct hash<javascript_tasklets::String> final
{
    std::size_t operator()(const javascript_tasklets::String &str) const
    {
        std::size_t retval = str.size();
        for(auto ch : str)
        {
            retval *= 1361;
            retval ^= ch;
        }
        return retval;
    }
};
}
#else
namespace javascript_tasklets
{
typedef std::u16string String;
}
#endif

namespace javascript_tasklets
{
template <typename Dest, typename Src>
struct StringCastHelper final
{
    static Dest convert(const Src &src) = delete;
};

template <typename CharT, typename TraitsT, typename AllocT>
struct StringCastHelper<std::basic_string<CharT, TraitsT, AllocT>,
                        std::basic_string<CharT, TraitsT, AllocT>> final
{
    static std::basic_string<CharT, TraitsT, AllocT> convert(
        const std::basic_string<CharT, TraitsT, AllocT> &v)
    {
        return v;
    }
};

template <>
struct StringCastHelper<std::string, String> final
{
    template <typename Fn>
    static void convertImplementation(const String &src, Fn &&fn)
    {
        for(std::size_t i = 0; i < src.size(); i++)
        {
            std::uint16_t ch = src[i];
            std::uint_fast32_t value = ch;
            if(ch >= 0xD800U && ch < 0xDC00U && i + 1 < src.size() && src[i + 1] >= 0xDC00U
               && src[i + 1] < 0xC000U)
            {
                value = ch & 0x3FFU;
                value <<= 10;
                value += src[i++] & 0x3FFU;
                value += 0x10000UL;
            }
            if(value < 0x80)
            {
                fn(static_cast<char>(value));
            }
            else if(value < 0x800)
            {
                fn(static_cast<char>((value >> 6) | 0xC0));
                fn(static_cast<char>((value & 0x3F) | 0x80));
            }
            else if(value < 0x10000UL)
            {
                fn(static_cast<char>((value >> 12) | 0xE0));
                fn(static_cast<char>(((value >> 6) & 0x3F) | 0x80));
                fn(static_cast<char>((value & 0x3F) | 0x80));
            }
            else
            {
                fn(static_cast<char>((value >> 18) | 0xF0));
                fn(static_cast<char>(((value >> 12) & 0x3F) | 0xE0));
                fn(static_cast<char>(((value >> 6) & 0x3F) | 0x80));
                fn(static_cast<char>((value & 0x3F) | 0x80));
            }
        }
    }
    static std::string convert(const String &src)
    {
        std::size_t retvalSize = 0;
        convertImplementation(src,
                              [&](char)
                              {
                                  retvalSize++;
                              });
        std::string retval;
        retval.reserve(retvalSize);
        convertImplementation(src,
                              [&](char ch)
                              {
                                  retval += ch;
                              });
        return retval;
    }
};

template <>
struct StringCastHelper<String, std::string> final
{
    static void writeValue(String &retval, std::uint_fast32_t value)
    {
        if(value > 0x10000UL)
        {
            value -= 0x10000UL;
            retval += static_cast<String::value_type>(((value >> 10) & 0x3FF) | 0xD800U);
            retval += static_cast<String::value_type>((value & 0x3FF) | 0xDC00U);
        }
        else
        {
            retval += static_cast<String::value_type>(value);
        }
    }
    static String convert(const std::string &src)
    {
        String retval;
        retval.reserve(src.size());
        std::uint_fast32_t value = 0;
        bool hasValue = false;
        for(std::size_t i = 0; i < src.size(); i++)
        {
            unsigned currentByte = static_cast<unsigned char>(src[i]);
            if(currentByte >= 0x80)
            {
                if(currentByte >= 0xC0)
                {
                    if(hasValue)
                    {
                        writeValue(retval, value);
                    }
                    hasValue = true;
                    switch(currentByte)
                    {
                    case 0xC0:
                    case 0xC1:
                    case 0xC2:
                    case 0xC3:
                    case 0xC4:
                    case 0xC5:
                    case 0xC6:
                    case 0xC7:
                    case 0xC8:
                    case 0xC9:
                    case 0xCA:
                    case 0xCB:
                    case 0xCC:
                    case 0xCD:
                    case 0xCE:
                    case 0xCF:
                    case 0xD0:
                    case 0xD1:
                    case 0xD2:
                    case 0xD3:
                    case 0xD4:
                    case 0xD5:
                    case 0xD6:
                    case 0xD7:
                    case 0xD8:
                    case 0xD9:
                    case 0xDA:
                    case 0xDB:
                    case 0xDC:
                    case 0xDD:
                    case 0xDE:
                    case 0xDF:
                        value = currentByte & 0x1F;
                        break;
                    case 0xE0:
                    case 0xE1:
                    case 0xE2:
                    case 0xE3:
                    case 0xE4:
                    case 0xE5:
                    case 0xE6:
                    case 0xE7:
                    case 0xE8:
                    case 0xE9:
                    case 0xEA:
                    case 0xEB:
                    case 0xEC:
                    case 0xED:
                    case 0xEE:
                    case 0xEF:
                        value = currentByte & 0x0F;
                        break;
                    case 0xF0:
                    case 0xF1:
                    case 0xF2:
                    case 0xF3:
                    case 0xF4:
                    case 0xF5:
                    case 0xF6:
                    case 0xF7:
                        value = currentByte & 0x07;
                        break;
                    case 0xF8:
                    case 0xF9:
                    case 0xFA:
                    case 0xFB:
                        value = currentByte & 0x03;
                        break;
                    case 0xFC:
                    case 0xFD:
                        value = currentByte & 0x01;
                        break;
                    case 0xFE:
                    case 0xFF:
                        value = 0;
                        break;
                    }
                }
                else if(!hasValue)
                {
                    writeValue(retval, currentByte);
                }
                else
                {
                    value <<= 6;
                    value |= currentByte & 0x3F;
                }
            }
            else
            {
                if(hasValue)
                {
                    hasValue = false;
                    writeValue(retval, value);
                }
                writeValue(retval, currentByte);
            }
        }
        if(hasValue)
        {
            writeValue(retval, value);
        }
        return retval;
    }
};

template <typename Dest, typename Src>
Dest string_cast(const Src &src)
{
    return StringCastHelper<Dest, Src>::convert(src);
}

inline String operator"" _js(const char *str, std::size_t length)
{
    return string_cast<String>(std::string(str, length));
}

inline String operator"" _js(const char16_t *str, std::size_t length)
{
    return string_cast<String>(std::u16string(str, length));
}

std::ostream &writeString(std::ostream &os, const String &string);
String appendCodePoint(String str, std::uint32_t codePoint)
{
    StringCastHelper<String, std::string>::writeValue(str, codePoint);
    return str;
}
}

#endif /* JAVASCRIPT_TASKLETS_STRING_H_ */
