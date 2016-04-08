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

#ifndef JAVASCRIPT_TASKLETS_CONSTEXPR_ASSERT_H_
#define JAVASCRIPT_TASKLETS_CONSTEXPR_ASSERT_H_

namespace javascript_tasklets
{
struct constexpr_assert_return_type final
{
    static constexpr_assert_return_type assert_failed(const char *location,
                                                      const char *func,
                                                      const char *message) noexcept;
    static constexpr constexpr_assert_return_type assert_helper(bool condition,
                                                                const char *location,
                                                                const char *func,
                                                                const char *message) noexcept
    {
        return condition ? constexpr_assert_return_type() : assert_failed(location, func, message);
    }
};
}

#define constexpr_assert_stringify(v) #v
#ifdef NDEBUG
#define constexpr_assert(v) (::javascript_tasklets::constexpr_assert_return_type())
#else
#ifdef __GNUC__
#if __GNUC__ > 4
#define constexpr_assert_func_name __PRETTY_FUNCTION__
#else
#define constexpr_assert_func_name \
    nullptr // __PRETTY_FUNCTION__ and __func__ trigger a compiler bug in gcc versions before 5.x
#endif
#else
#define constexpr_assert_func_name __func__
#endif
#if defined(__GNUC__) && !defined(__CDT_PARSER__)
#define constexpr_assert_file_name __BASE_FILE__
#else
#define constexpr_assert_file_name __FILE__
#endif
#define constexpr_assert(v)                                                  \
    (::javascript_tasklets::constexpr_assert_return_type::assert_helper(     \
        (v) ? true : false,                                                  \
        constexpr_assert_file_name ":" constexpr_assert_stringify(__LINE__), \
        constexpr_assert_func_name,                                          \
        "assertion '" constexpr_assert_stringify(v) "' failed."))
#endif

#endif /* JAVASCRIPT_TASKLETS_CONSTEXPR_ASSERT_H_ */
