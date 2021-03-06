// File: nameMangling.h
// 
// Copyright 2017-2018 Till Heinzel
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include <vector>
#include <functional>
#include <string>

#include "src/TypeErasure/TypeEnum.h"

namespace metl
{
	namespace internal {
		constexpr char separator = '@';

		inline std::string mangleCast(const TYPE from, const TYPE to)
		{
			return typeToString(from) + separator + typeToString(to);
		}

		inline std::string mangleName(std::string functionName, const std::vector<TYPE>& paramTypes)
		{
			for (const auto& t : paramTypes)
			{
				functionName += separator + typeToString(t);
			}

			return functionName;
		}

		template<class Expr>
		std::string mangleName(std::string functionName, const std::vector<Expr>& params)
		{
			for (const auto& t : params)
			{
				functionName += separator + typeToString(t.type());
			}

			return functionName;
		}

		inline std::string mangleSuffix(std::string suffix, TYPE from)
		{
			return typeToString(from) + separator + suffix;
		}
	}
}
