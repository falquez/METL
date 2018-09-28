/*
@file DynamicExpression.h
Defines class DynamicExpression, which is a variant-type to contain std::functions returning different values.
This is used to contain the results of parsing.

Copyright 2017-2018 Till Heinzel

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

#include "ThirdParty/Variant/variant.hpp"

#include "src/Utility/TypeList.h"
#include "src/std17/remove_cvref.h"

#include "src/TypeEnum.h"
#include "src/StaticExpression.h"

#include "CategoryEnum.h"

namespace metl
{
	template<class... Ts>
	class DynamicExpression
	{
	public:
		template<class T, std::enable_if_t<!internal::isInList<T, Ts...>(), int> = 0>
		DynamicExpression(const StaticExpression<T>& t, CATEGORY category = CATEGORY::DYNEXPR)
		{
			static_assert(!internal::isInList<T, Ts...>(), "cannot construct Varexpression with this type");
		}

		template<class T, std::enable_if_t<internal::isInList<T, Ts...>(), int> = 0>
		DynamicExpression(const StaticExpression<T>& t, CATEGORY category = CATEGORY::DYNEXPR) :
			type_(classToType2<T, Ts...>()),
			category_(category),
			vals_(t)
		{
		}

		template<class T> StaticExpression<T> get() const
		{
			constexpr auto index = internal::findFirstIndex<T>(internal::TypeList<Ts...>{});
			static_assert(index < sizeof...(Ts), "Error: Requested Type is not a valid type!");

			if (mpark::holds_alternative<StaticExpression<T>>(vals_)) return mpark::get<StaticExpression<T>>(vals_);
			throw std::runtime_error("this is not the correct type");
		}

		DynamicExpression evaluatedExpression() const 
		{
			auto visitor = [](const auto& expr)
			{
				using exprType_T = std17::remove_cvref_t<decltype(expr)>;
				auto value = expr();
				auto constExpr = exprType_T([value]() {return value; });
				return DynamicExpression<Ts...>(constExpr, CATEGORY::CONSTEXPR);
			};

			return mpark::visit(visitor, vals_);
		}

		template<class T>
		bool isType() const { return type_ == toType<T>(); }

		TYPE type() const { return type_; }
		CATEGORY category() const { return category_; }

		template<class T>
		constexpr static TYPE toType() { return classToType2<T, Ts...>(); }
	private:
		TYPE type_;
		CATEGORY category_;

		mpark::variant<StaticExpression<Ts>...> vals_;
	};

	namespace internal
	{
		template<class... Ts>
		constexpr TypeList<Ts...> getTypeList(Type<DynamicExpression<Ts...>>) { return TypeList<Ts...>{}; }
	}
}