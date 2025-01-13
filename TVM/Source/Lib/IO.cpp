#include "IO.h"

#include "TVM.h"
#include "TVMRegister.h"

#include <cassert>

using namespace TVM;

namespace IO
{
	void WriteOut(VM& vm, const std::vector<TVM::Register>& args)
	{
		// first arg is the function
		TVM::Register reg = args[1];
		while (reg.type == REGISTER)
			reg = vm.GetReg(reg.value.num);
			//reg = vm.GetReg(reg.value.num);

		switch (reg.type)
		{
		case FUNCTION:
			break;
		case STRING:
			std::cout << reg.value.str;
			break;
		case FLOAT:
			std::cout << reg.value.flt;
			break;
		case INT:
			std::cout << reg.value.num;
			break;
		default:
			break;
		}
	}

	CPPLib GetIOLib()
	{
		CPPLib l;
		l.functions.emplace_back(WriteOut, "WriteOut", "...");

		TVM::Register pi{};
		pi.type = FLOAT;
		pi.value.flt = 3.141f;

		l.vars["PI"] = pi;

		return l;
	}
	
	void Register(VM& vm)
	{
		CPPFunction FWriteOut{WriteOut, "WriteOut", "...", "WriteOut ..."};
		vm.functions[FWriteOut.function_sig] = FWriteOut;
	}
}
