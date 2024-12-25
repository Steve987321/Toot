#pragma once

#include "Lexer.h"
#include <vector>
#include "TVMByteCodes.h"
#include "TVM.h"

static std::unordered_map<TVM::OP_CODE, std::string> op_code_names
{
	{TVM::OP_CODE::OP_LABEL, "OP_LABEL"},
	{TVM::OP_CODE::OP_MOVE, "OP_MOVE"},
	{TVM::OP_CODE::OP_ADD, "OP_ADD"},
	{TVM::OP_CODE::OP_SUBTRACT, "OP_SUBTRACT"},
	{TVM::OP_CODE::OP_MULTIPLY, "OP_MULTIPLY"},
	{TVM::OP_CODE::OP_DIVIDE, "OP_DIVIDE"},
	{TVM::OP_CODE::OP_NEGATE, "OP_NEGATE"},
	{TVM::OP_CODE::OP_NOT, "OP_NOT"},
	{TVM::OP_CODE::OP_CALL, "OP_CALL"},
	{TVM::OP_CODE::OP_JUMP, "OP_JUMP"},
	{TVM::OP_CODE::OP_JUMPIFNOTZERO, "OP_JUMPIFNOTZERO"},
	{TVM::OP_CODE::OP_JUMPIFZERO, "OP_JUMPIFZERO"},
	{TVM::OP_CODE::OP_RETURN, "OP_RETURN"},
	{TVM::OP_CODE::BYTE_CODE_COUNT, "BYTE_CODE_COUNT"},
};

namespace Compiler
{
	bool Parse(const std::vector<Token>& tokens, std::vector<TVM::VM::Instruction>& op_codes_res);
}