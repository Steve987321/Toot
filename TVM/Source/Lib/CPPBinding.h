#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <string_view>
#include <string>

#define REGISTER_LIBFUNC(lib, function, args)										\
lib.functions.push_back(CPPFunction(function, #function, args, #function " " args))	\

class VM;
struct VMRegister;
enum class VMRegisterType;

class CPPFunction
{
public:
	using TFunction = std::function<VMRegister(VM& vm, const std::vector<VMRegister>&)>;

	CPPFunction() = default;

	CPPFunction(TFunction f, const char* name, const char* args, const char* sig)
		: func(f), function_name(name), accepted_args(args), function_sig(sig)
	{}

	TFunction func {};
	const char* function_name = nullptr;
	const char* accepted_args = nullptr;
	const char* function_sig = nullptr; // function_name + accepted_args
};

struct CPPLib
{
	std::string name;
	std::unordered_map<std::string_view, VMRegister> vars;
	std::vector<CPPFunction> functions;
};

