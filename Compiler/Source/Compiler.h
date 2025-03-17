#pragma once

#include <vector>
#include <string>

namespace Compiler
{
	enum class CompileResult 
	{
		NONE,
		ERR
	};

    inline std::vector<std::string> error_msgs;
	CompileResult CompileString(std::string_view str);
}
