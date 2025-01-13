#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <string_view>

namespace TVM
{
	class VM;
	struct Register;

class CPPFunction
{
public:
	using TFunction = std::function<void(VM& vm, const std::vector<Register>&)>;

	TFunction function = nullptr;
	const char* function_name;
	const char* accepted_args;
	const char* function_sig;
};

struct CPPLib
{
	std::unordered_map<std::string_view, Register> vars;
	std::vector<CPPFunction> functions;
};

}
