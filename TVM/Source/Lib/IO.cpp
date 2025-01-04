#include "IO.h"

#include "TVM.h"

#include <cassert>

using namespace TVM;

namespace IO
{
	void Print(VM& vm, const std::vector<Register>& args)
	{
		std::stringstream ss;

		for (const Register& r : args)
		{
			switch (r.type)
			{
			case STRING:
			{
				ss << r.value.str;
				break;
			}
			case INT:
			{
				ss << r.value.num;
				break;
			}
			case FLOAT:
			{
				ss << r.value.flt;
				break;
			}
			case REGISTER:
			{
				Register* reg = &vm.registers[r.value.num];
				while (reg->type == REGISTER)
				{
					reg = &vm.registers[r.value.num];
					assert(reg && "Register not found");
				}

				switch (reg->type)
				{
				case INT:
					ss << reg->value.num;
					break;
				case STRING:
					ss << reg->value.str;
					break;
				case FLOAT:
					ss << reg->value.flt;
					break;
				default:
					assert(0 && "Invalid register type");
					break;
				}

				break;
			}
			
			}
		}

		std::cout << ss.str() << std::endl;
	}
}
