/*
@file
TypeList.h
Defines utilities for doing operations on lists of types.

Copyright 2017 Till Heinzel

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

namespace metl
{
	namespace detail
	{
		template<class TrueBranch, class FalseBranch>
		decltype(auto) constexpr constexpr_if_invoke(const std::true_type&, const TrueBranch& trueBranch, const FalseBranch&)
		{
			return trueBranch([](const auto& t) {return t; });
		}

		template<class TrueBranch, class FalseBranch>
		decltype(auto) constexpr constexpr_if_invoke(const std::false_type&, const TrueBranch&, const FalseBranch& falseBranch)
		{
			return falseBranch([](const auto& t) {return t; });
		}
	}
	template<class Condition, class TrueBranch, class FalseBranch>
	decltype(auto) constexpr constexpr_if(const Condition& condition, const TrueBranch& trueBranch, const FalseBranch& falseBranch)
	{
		return detail::constexpr_if_invoke(std::integral_constant<bool, Condition::value>{}, trueBranch, falseBranch);
	}
}

namespace metl
{

	template<class...> struct TypeList { constexpr TypeList() = default; };

	template<size_t I, class ToFind>
	constexpr size_t findFirstIndex()
	{
		return I;
	}

	template<size_t I, class ToFind, class T, class... Ts>
	constexpr size_t findFirstIndex()
	{
		return std::is_same<ToFind, T>::value ? I : findFirstIndex<I + 1, ToFind, Ts...>();
	}

	template<class ToFind, class... Ts>
	constexpr size_t findFirstIndex(TypeList<Ts...>)
	{
		return findFirstIndex<0, ToFind, Ts...>();
	}

	template<class ToFind, class... Ts>
	constexpr bool isInList()
	{
		return findFirstIndex<ToFind>(TypeList<Ts...>{}) < sizeof...(Ts);
	}

	template<unsigned index, class... Ts>
	struct Get
	{
		using type = std::remove_cv_t<std::remove_reference_t<decltype(std::get<index>(std::tuple<Ts...>(std::declval<Ts>()...)))>>;
	};


	template<unsigned index, class... Ts> using Get_t = typename Get<index, Ts...>::type;

}

namespace metl
{
	template<class T>
	struct Type
	{
		using type = T;
	};
}
