#include "IO.h"

#include "TVM.h"
#include "TVMRegister.h"

#include <cassert>

using namespace TVM;

namespace IO
{
	void WriteOut(VM& vm, const std::vector<TVM::Register>& args)
	{
		TVM::Register reg = args[0];
		while (reg.type == REGISTER)
			reg = vm.registers[reg.value.num];

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

	void Register(VM& vm)
	{
		vm.functions["WriteOut(...)"] = WriteOut;
	}
}
