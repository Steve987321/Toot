#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <string_view>

class VM;
struct VMRegister;

class CPPFunction
{
public:
	using TFunction = std::function<void(VM& vm, const std::vector<VMRegister>&)>;

	TFunction func = nullptr;
	const char* function_name;
	const char* accepted_args;
	const char* function_sig;
};

struct CPPLib
{
	std::string name;
	std::unordered_map<std::string_view, VMRegister> vars;
	std::vector<CPPFunction> functions;
};

