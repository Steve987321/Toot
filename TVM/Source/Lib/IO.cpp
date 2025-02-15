#include "IO.h"
#include "TVM.h"

#include <cassert>

#define REGISTER_LIBFUNC(lib, function, sig)								\
lib.functions.emplace_back(function, #function, sig, #function " " sig);	\

namespace IO
{
	void WriteOut(VM& vm, const std::vector<VMRegister>& args)
	{
		// first arg is the function
		VMRegister reg = args[1];
		while (reg.type == VMRegisterType::REGISTER)
			reg = vm.GetReg(reg.value.num);

		switch (reg.type)
		{
		case VMRegisterType::FUNCTION:
			assert(false && "redart");
			break;
		case VMRegisterType::STRING:
			std::cout << reg.value.str;
			break;
		case VMRegisterType::FLOAT:
			std::cout << reg.value.flt;
			break;
		case VMRegisterType::INT:
			std::cout << reg.value.num;
			break;
		default:
			break;
		}
	}

	CPPLib GetIOLib()
	{
		CPPLib l;
		l.name = "io";

		REGISTER_LIBFUNC(l, WriteOut, "...");
		REGISTER_LIBFUNC(l, Nothing, "int int");

		VMRegister pi{};
		pi.type = VMRegisterType::FLOAT;
		pi.value.flt = 3.141f;

		l.vars["PI"] = pi;

		return l;
	}
	
	void RegisterToVM(VM& vm)
	{
		CPPLib lib = GetIOLib();
		for (const CPPFunction& f : lib.functions)
		{
			vm.functions[f.function_sig] = f;
		}

		// #TODO: vars
	}
}
