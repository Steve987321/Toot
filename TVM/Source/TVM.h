#pragma once

#include <vector>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <stack>

#include "Lib/IO.h"

enum OP_CODE
{
	OP_LABEL,
	OP_MOVE,
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_NEGATE,
	OP_NOT,
	OP_CALL,
	OP_JUMP,
	OP_JUMPIFNOTZERO,
	OP_JUMPIFZERO,
	OP_JUMPIFGREATER,
	OP_JUMPIFLESS,
	OP_JUMPIFNOTEQUAL,
	OP_JUMPIFEQUAL,
	//OP_SETRELREGINDEX, // for the vm to handle 
	OP_RETURN,
	BYTE_CODE_COUNT,
};

enum VMRegisterType
{
	REGISTER, // for args
	FUNCTION, // for parser
	STRING,
	FLOAT,
	INT,
};

union VMRegisterValue
{
	int32_t num;
	float flt;
	const char* str;
};

// Register value info 
struct VMRegister
{
	VMRegisterValue value;
	VMRegisterType type;
};

class VM
{
public:
	std::array<VMRegister, 1024> registers;

	uint64_t relative_register_index = 0;
	std::stack<uint64_t> rel_reg_stack;

	uint64_t instruction_pointer = 0;
	struct Instruction
	{
		OP_CODE op;
		std::vector<VMRegister> args;
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

	void OpMove(const VMRegister& dst, const VMRegister& src);
	void OpAdd(const VMRegister& dst, const VMRegister& a, const VMRegister& b);
	void OpSubtract(const VMRegister& dst, const VMRegister& a, const VMRegister& b);
	void OpMultiply(const VMRegister& dst, const VMRegister& a, const VMRegister& b);
	void OpDivide(const VMRegister& dst, const VMRegister& a, const VMRegister& b); 
	void OpLabel(const VMRegister& a);
	void OpJump(const VMRegister& a);
	void OpJumpIfZero(const VMRegister& jump, const VMRegister& a, const VMRegister& b);
	void OpCall(const std::vector<VMRegister>& args);

	VMRegister& GetReg(uint64_t index);

	// returns anything other then type REGISTER
	VMRegister& GetValueAtReg();
private:
	// arg is usually ip 
	void BeginRelativeRegIndex(uint64_t rel);
	void EndRelativeRegIndex();
};
