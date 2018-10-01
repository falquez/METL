/*
@file
CompilersHelpers.h
Bunch of helpers for the compiler. Named so because included by CompilerApi-files

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


#include "src/TypeErasure/UntypedExpression.h"
#include "src/TypeErasure/TypedExpression.h"

namespace metl
{
	namespace internal
	{
		template<class T>
		struct VariableExpression
		{
			T* v;

			T operator() () { return *v; }
		};

		template<class UntypedExpression_t, class T>
		UntypedExpression_t makeVariableExpression(T* v)
		{
			auto f = VariableExpression<T>{ v };

			return UntypedExpression_t::makeNonConstexpr(TypedExpression<T>(f));
		}
	}
}
