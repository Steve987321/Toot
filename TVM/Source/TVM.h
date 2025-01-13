#pragma once

#include <vector>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <stack>

#include "TVMByteCodes.h"
#include "TVMRegister.h"

#include "Lib/IO.h"


namespace TVM
{
	class VM
	{
	public:
		std::array<Register, 1024> registers;

		uint64_t relative_register_index = 0;
		std::stack<uint64_t> rel_reg_stack;

		uint64_t instruction_pointer = 0;
		struct Instruction
		{
			OP_CODE op;
			std::vector<Register> args;
		};

		std::vector<Instruction> instructions;

		std::unordered_map<std::string_view, uint64_t> labels;
		//using function = std::function<void(VM& vm, const std::vector<Register>&)>;
		//// ... means vector of registers
		//// . means 1 register 
		//// int, string, explicit type requires that type 
		/// #todo cpp binder 
		std::unordered_map<std::string_view, CPPFunction> functions;
		void Init();

		void Run();

		void OpMove(const Register& dst, const Register& src);
		void OpAdd(const Register& dst, const Register& a, const Register& b);
		void OpSubtract(const Register& dst, const Register& a, const Register& b);
		void OpMultiply(const Register& dst, const Register& a, const Register& b);
		void OpDivide(const Register& dst, const Register& a, const Register& b); 
		void OpLabel(const Register& a);
		void OpJump(const Register& a);
		void OpJumpIfZero(const Register& jump, const Register& a, const Register& b);
		void OpCall(const std::vector<Register>& args);

		Register& GetReg(uint64_t index);

		// returns anything other then type REGISTER
		Register& GetValueAtReg();
	private:
		// arg is usually ip 
		void BeginRelativeRegIndex(uint64_t rel);
		void EndRelativeRegIndex();
	};
}
