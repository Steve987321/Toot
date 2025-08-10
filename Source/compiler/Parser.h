#pragma once

#include <vector>
#include <set>
#include <expected>

#include "Lexer.h"
#include "tvm/TVM.h"

inline std::unordered_map<OP_CODE, std::string_view> op_code_names
{
	{OP_CODE::OP_DEFINE_LABEL, "OP_DEFINE_LABEL"},
	{OP_CODE::OP_MOVE, "OP_MOVE"},
	{OP_CODE::OP_MOVE_ONCE, "OP_MOVE_ONCE"},
	{OP_CODE::OP_ADD, "OP_ADD"},
	{OP_CODE::OP_SUBTRACT, "OP_SUBTRACT"},
	{OP_CODE::OP_MULTIPLY, "OP_MULTIPLY"},
	{OP_CODE::OP_DIVIDE, "OP_DIVIDE"},
	{OP_CODE::OP_NOT, "OP_NOT"},
	{OP_CODE::OP_CALL, "OP_CALL"},
	{OP_CODE::OP_CALL_MOVE, "OP_CALL_MOVE"},
	{OP_CODE::OP_JUMP, "OP_JUMP"},
	{OP_CODE::OP_JUMP_ONCE, "OP_JUMP_ONCE"},
	{OP_CODE::OP_JUMP_IF_NOT_EQUAL, "OP_JUMP_IF_NOT_EQUAL"},
	{OP_CODE::OP_JUMP_IF_EQUAL, "OP_JUMP_IF_EQUAL"},
	{OP_CODE::OP_JUMP_IF_LESS, "OP_JUMP_IF_LESS"},
	{OP_CODE::OP_JUMP_IF_GREATER, "OP_JUMP_IF_GREATER"},
	{OP_CODE::OP_RETURN, "OP_RETURN"},
	{OP_CODE::BYTE_CODE_COUNT, "BYTE_CODE_COUNT"},
};

namespace Compiler
{
	void AddLibToParserCtx(const CPPLib& lib);

	// returns true if there is no errors
	// use optional vm arg to add context
	std::expected<std::vector<VM::Instruction>, std::vector<std::string>> Parse(const std::vector<Token>& tokens, VM* vm = nullptr);
}
