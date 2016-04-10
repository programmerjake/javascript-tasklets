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

#ifndef JAVASCRIPT_TASKLETS_VARIANT_H_
#define JAVASCRIPT_TASKLETS_VARIANT_H_

#include "constexpr_assert.h"
#include <type_traits>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <new>
#include <functional>

namespace javascript_tasklets
{
struct variant_base
{
    std::size_t type;
    template <typename... Types>
    struct implementation;
    template <typename FindType, typename... Types>
    struct get;
    struct test;
};

template <>
struct variant_base::implementation<> final
{
    implementation(const implementation &) = delete;
    implementation &operator=(const implementation &) = delete;
    implementation() noexcept
    {
    }
    ~implementation() noexcept
    {
    }
    void assign(std::size_t id, const implementation &) noexcept
    {
        constexpr_assert(id == 0);
    }
    void assign(std::size_t id, implementation &&) noexcept
    {
        constexpr_assert(id == 0);
    }
    void construct(std::size_t id, const implementation &) noexcept
    {
        constexpr_assert(id == 0);
    }
    void construct(std::size_t id, implementation &&) noexcept
    {
        constexpr_assert(id == 0);
    }
    void destruct(std::size_t id) noexcept
    {
        constexpr_assert(id == 0);
    }
    template <typename Fn, typename ImpType>
    static void apply(std::size_t id,
                      ImpType &&imp,
                      Fn &&fn) noexcept(noexcept(std::forward<Fn>(fn)()))
    {
        static_assert(std::is_same<typename std::decay<ImpType>::type, implementation>::value,
                      "invalid ImpType");
        constexpr_assert(id == 0);
        std::forward<Fn>(fn)();
    }
    std::size_t hash(std::size_t id) const noexcept
    {
        constexpr_assert(id == 0);
        return 0;
    }
    bool equals(std::size_t id, const implementation &) const noexcept
    {
        constexpr_assert(id == 0);
        return true;
    }
};

template <typename T, typename... Types>
struct variant_base::implementation<T, Types...> final
{
    static_assert(!std::is_void<T>::value, "void is not allowed in variant");
    static_assert(std::is_same<T, typename std::decay<T>::type>::value,
                  "const, volatile, array, or reference types are not allowed in variant");
    union
    {
        T currentValue;
        implementation<Types...> rest;
    };
    implementation() noexcept : rest()
    {
    }
    implementation(const implementation &) = delete;
    implementation &operator=(const implementation &) = delete;
    ~implementation() noexcept
    {
    }
    void assign(std::size_t id,
                const implementation &rt) noexcept(noexcept(currentValue = rt.currentValue)
                                                   && noexcept(rest.assign(id - 1, rt.rest)))
    {
        if(id == 0)
            currentValue = rt.currentValue;
        else
            rest.assign(id - 1, rt.rest);
    }
    void assign(std::size_t id,
                implementation &&rt) noexcept(noexcept(currentValue = std::move(rt.currentValue))
                                              && noexcept(rest.assign(id - 1, std::move(rt.rest))))
    {
        if(id == 0)
            currentValue = std::move(rt.currentValue);
        else
            rest.assign(id - 1, std::move(rt.rest));
    }
    void construct(std::size_t id,
                   const implementation &rt) noexcept(noexcept(T(rt.currentValue))
                                                      && noexcept(rest.construct(id - 1, rt.rest)))
    {
        if(id == 0)
            ::new(&currentValue) T(rt.currentValue);
        else
            rest.construct(id - 1, rt.rest);
    }
    void construct(std::size_t id,
                   implementation &&rt) noexcept(noexcept(T(std::move(rt.currentValue)))
                                                 && noexcept(rest.construct(id - 1,
                                                                            std::move(rt.rest))))
    {
        if(id == 0)
            ::new(&currentValue) T(std::move(rt.currentValue));
        else
            rest.construct(id - 1, std::move(rt.rest));
    }
    void destruct(std::size_t id) noexcept
    {
        if(id == 0)
            currentValue.~T();
        else
            rest.destruct(id - 1);
    }
    template <typename Fn, typename ImpType>
    static void apply(std::size_t id, ImpType &&imp, Fn &&fn) noexcept(
        noexcept(std::declval<Fn>()(std::declval<ImpType &&>().currentValue))
        && noexcept(implementation<Types...>::apply(std::declval<std::size_t>(),
                                                    std::declval<ImpType &&>().rest,
                                                    std::declval<Fn>())))
    {
        static_assert(std::is_same<typename std::decay<ImpType>::type, implementation>::value,
                      "invalid ImpType");
        if(id == 0)
            std::forward<Fn>(fn)(std::forward<ImpType>(imp).currentValue);
        else
            implementation<Types...>::apply(
                id - 1, std::forward<ImpType>(imp).rest, std::forward<Fn>(fn));
    }
    std::size_t hash(std::size_t id) const
        noexcept(noexcept(std::hash<T>()(currentValue)) && noexcept(rest.hash(id - 1)))
    {
        if(id == 0)
            return std::hash<T>()(currentValue);
        else
            return rest.hash(id - 1);
    }
    bool equals(std::size_t id, const implementation &rt) const
        noexcept(noexcept(currentValue == rt.currentValue)
                 && noexcept(rest.equals(id - 1, rt.rest)))
    {
        if(id == 0)
            return currentValue == rt.currentValue;
        else
            return rest.equals(id - 1, rt.rest);
    }
};

template <typename FindType>
struct variant_base::get<FindType> final
{
    static constexpr bool found = std::is_void<FindType>::value;
    static constexpr std::size_t id = 0;
    template <typename ImpType>
    static void getValue(ImpType &&) = delete;
};

template <typename FindType, typename... Types>
struct variant_base::get<FindType, FindType, Types...> final
{
    static constexpr bool found = true;
    static constexpr std::size_t id = 0;
    static FindType &&getValue(implementation<FindType, Types...> &&imp) noexcept
    {
        return std::move(imp.currentValue);
    }
    static FindType &getValue(implementation<FindType, Types...> &imp) noexcept
    {
        return imp.currentValue;
    }
    static const FindType &getValue(const implementation<FindType, Types...> &imp) noexcept
    {
        return imp.currentValue;
    }
};

template <typename FindType, typename T2, typename... Types>
struct variant_base::get<FindType, T2, Types...> final
{
    static constexpr bool found = get<FindType, Types...>::found;
    static constexpr std::size_t id = get<FindType, Types...>::id + 1;
    static FindType &&getValue(implementation<T2, Types...> &&imp) noexcept
    {
        return get<FindType, Types...>::getValue(std::move(imp.rest));
    }
    static FindType &getValue(implementation<T2, Types...> &imp) noexcept
    {
        return get<FindType, Types...>::getValue(imp.rest);
    }
    static const FindType &getValue(const implementation<T2, Types...> &imp) noexcept
    {
        return get<FindType, Types...>::getValue(imp.rest);
    }
};

template <typename T2, typename... Types>
struct variant_base::get<void, T2, Types...> final
{
    static constexpr bool found = get<void, Types...>::found;
    static constexpr std::size_t id = get<void, Types...>::id + 1;
};

template <typename... Types>
class variant final : private variant_base
{
    template <typename...>
    friend class variant;
    friend struct variant_base::test;

private:
    std::size_t id;
    typedef implementation<Types...> imp;
    imp values;
    template <typename... Types2>
    struct cast_helper final
    {
        variant &value;
        cast_helper(variant &value) noexcept : value(value)
        {
        }
        void operator()() noexcept
        {
        }
        template <typename T>
        static constexpr bool isValidType()
        {
            return variant_base::get<typename std::decay<T>::type, Types...>::found;
        }
        template <typename T, typename = typename std::enable_if<isValidType<T>()>::type>
        void operator()(T &&rt) noexcept(
            noexcept(typename std::decay<T>::type(std::forward<T>(rt))))
        {
            typedef typename std::decay<T>::type construct_type;
            ::new(&value.getNoCheck<construct_type>()) construct_type(std::forward<T>(rt));
            value.id = idFor<construct_type>(); // if construction succeeds
        }
        template <typename T, typename = typename std::enable_if<!isValidType<T>()>::type>
        void operator()(T &&, int = 0) noexcept
        {
            constexpr_assert(!"invalid variant cast");
#ifdef __GNUC__
            __builtin_unreachable();
#endif
        }
    };
    template <typename FindType>
    const FindType &getNoCheck() const &noexcept
    {
        return variant_base::get<FindType, Types...>::getValue(values);
    }
    template <typename FindType>
        FindType &getNoCheck() & noexcept
    {
        return variant_base::get<FindType, Types...>::getValue(values);
    }
    template <typename FindType>
        FindType &&getNoCheck() && noexcept
    {
        return variant_base::get<FindType, Types...>::getValue(std::move(values));
    }

public:
    template <typename FindType>
    static constexpr std::size_t idFor() noexcept
    {
        return variant_base::get<FindType, Types...>::id;
    }
    std::size_t getId() const noexcept
    {
        return id;
    }
    template <typename FindType>
    bool is() const noexcept
    {
        return id == idFor<FindType>();
    }
    template <typename FindType>
    const FindType &get() const &noexcept
    {
        constexpr_assert(id == idFor<FindType>());
        return variant_base::get<FindType, Types...>::getValue(values);
    }
    template <typename FindType>
        FindType &get() & noexcept
    {
        constexpr_assert(id == idFor<FindType>());
        return variant_base::get<FindType, Types...>::getValue(values);
    }
    template <typename FindType>
        FindType &&get() && noexcept
    {
        constexpr_assert(id == idFor<FindType>());
        return variant_base::get<FindType, Types...>::getValue(std::move(values));
    }
    variant() noexcept : id(idFor<void>()), values()
    {
    }
    variant(const variant &rt) noexcept(noexcept(values.construct(id, rt.values)))
        : id(rt.id), values()
    {
        values.construct(id, rt.values);
    }
    variant(variant &&rt) noexcept(noexcept(values.construct(id, std::move(rt.values))))
        : id(rt.id), values()
    {
        values.construct(id, std::move(rt.values));
    }
    template <typename... Types2>
    explicit variant(const variant<Types2...> &rt) noexcept(
        noexcept(rt.apply(cast_helper<Types2...>(std::declval<variant &>()))))
        : id(idFor<void>()), values()
    {
        rt.apply(cast_helper<Types2...>(*this));
    }
    template <typename... Types2>
    explicit variant(variant<Types2...> &&rt) noexcept(
        noexcept(std::move(rt).apply(cast_helper<Types2...>(std::declval<variant &>()))))
        : id(idFor<void>()), values()
    {
        std::move(rt).apply(cast_helper<Types2...>(*this));
    }
    template <typename T, typename = typename std::enable_if<variant_base::get<typename std::decay<T>::type, Types...>::found>::type>
    explicit variant(T &&rt) noexcept(noexcept(typename std::decay<T>::type(std::declval<T &&>())))
        : id(idFor<typename std::decay<T>::type>())
    {
        ::new(&getNoCheck<typename std::decay<T>::type>())
            typename std::decay<T>::type(std::forward<T>(rt));
    }
    ~variant() noexcept
    {
        values.destruct(id);
    }
    variant &operator=(const variant &rt) noexcept(noexcept(values.construct(id, rt.values))
                                                   && noexcept(values.assign(id, rt.values)))
    {
        if(id != rt.id)
        {
            clear();
            values.construct(rt.id, rt.values);
            id = rt.id; // if construction succeeds
        }
        else
        {
            values.assign(id, rt.values);
        }
        return *this;
    }
    variant &operator=(variant &&rt) noexcept(noexcept(values.construct(id, std::move(rt.values)))
                                              && noexcept(values.assign(id, std::move(rt.values))))
    {
        if(id != rt.id)
        {
            clear();
            values.construct(rt.id, std::move(rt.values));
            id = rt.id; // if construction succeeds
        }
        else
        {
            values.assign(id, std::move(rt.values));
        }
        return *this;
    }
    void clear() noexcept
    {
        values.destruct(id);
        id = idFor<void>();
    }
    template <typename MakeType, typename... ArgTypes>
    static variant make(ArgTypes &&... arg) noexcept(
        noexcept(MakeType(std::declval<ArgTypes &&>()...)))
    {
        variant retval;
        ::new(&retval.getNoCheck<MakeType>()) MakeType(std::forward<ArgTypes>(arg)...);
        retval.id = idFor<MakeType>(); // set id after construction succeeded
        return retval;
    }
    template <typename Fn>
    Fn &&apply(Fn &&fn) const &noexcept(noexcept(imp::apply(std::declval<std::size_t>(),
                                                            std::declval<const imp &>(),
                                                            std::declval<Fn &&>())))
    {
        imp::apply(id, values, std::forward<Fn>(fn));
        return std::forward<Fn>(fn);
    }
    template <typename Fn>
        Fn &&apply(Fn &&fn) & noexcept(noexcept(imp::apply(std::declval<std::size_t>(),
                                                           std::declval<imp &>(),
                                                           std::declval<Fn &&>())))
    {
        imp::apply(id, values, std::forward<Fn>(fn));
        return std::forward<Fn>(fn);
    }
    template <typename Fn>
        Fn &&apply(Fn &&fn) && noexcept(noexcept(imp::apply(std::declval<std::size_t>(),
                                                            std::declval<imp &&>(),
                                                            std::declval<Fn &&>())))
    {
        imp::apply(id, std::move(values), std::forward<Fn>(fn));
        return std::forward<Fn>(fn);
    }
    std::size_t hash() const noexcept(noexcept(values.hash(id)))
    {
        return id + values.hash(id);
    }
    bool operator==(const variant &rt) const noexcept(noexcept(values.equals(id, rt.values)))
    {
        if(id == rt.id)
            return values.equals(id, rt.values);
        return false;
    }
    bool operator!=(const variant &rt) const noexcept(noexcept(values.equals(id, rt.values)))
    {
        if(id == rt.id)
            return !values.equals(id, rt.values);
        return true;
    }
    explicit operator bool() const noexcept
    {
        return id != idFor<void>();
    }
    bool operator!() const noexcept
    {
        return id == idFor<void>();
    }
    bool empty() const noexcept
    {
        return id == idFor<void>();
    }
};
}

namespace std
{
template <typename... Types>
struct hash<javascript_tasklets::variant<Types...>> final
{
    std::size_t operator()(const javascript_tasklets::variant<Types...> &value) const
        noexcept(noexcept(value.hash()))
    {
        return value.hash();
    }
};
}

#endif /* JAVASCRIPT_TASKLETS_VARIANT_H_ */
