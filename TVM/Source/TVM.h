#pragma once

#include <vector>
#include <cstdint>

#include "TVMByteCodes.h"

namespace TVM
{

	enum RegisterType
	{
		REGISTER, // for args
		FUNCTION, // for parser
		STRING,
		FLOAT,
		INT,
	};

	union RegisterValue
	{
		int32_t num;
		float flt;
		const char* str;
	};

	// Register value info 
	struct Register
	{
		RegisterValue value;
		RegisterType type;
	};

	struct VM
	{
		Register registers[16];
		Register stack[1024];

		uint64_t instruction_pointer = 0;
		uint64_t stack_pointer = 0;

		struct Instruction
		{
			OP_CODE op;
			std::array<Register, 2> args;
		};

		std::vector<Instruction> instructions;

		void Run();
	};

}
