#pragma once

#include <functional>
#include <array>

namespace TVM
{
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
		OP_SETRELREGINDEX, 
		OP_RETURN,
		BYTE_CODE_COUNT,
	};

	inline std::array<std::pair<OP_CODE, std::function<void()>>, OP_CODE::BYTE_CODE_COUNT> bytecodes;

	void Add();
	void Subtract();

	void InitBytecodes();
}
