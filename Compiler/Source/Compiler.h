#pragma once

#include <string_view>

namespace Compiler
{
	enum class CompileResult 
	{
		NONE,
		ERR
	};

	CompileResult CompileString(std::string_view str);
}