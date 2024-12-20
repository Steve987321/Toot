#include "Compiler.h"
#include "Lexer.h"
#include "Parser.h"

#include <iostream>

namespace Compiler
{

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

	CompileResult CompileString(std::string_view str)
	{
		LexerInit(str);

		std::vector<Token> tokens;
		if (!LexerScan(tokens))
			return CompileResult::ERR;

		// to vm bytecodes

		std::vector<std::pair<TVM::OP_CODE, std::vector<TVM::Register>>> res;

		Parse(tokens, res);

		for (const auto& [op_code, args] : res)
		{
			std::string s = op_code_names[op_code] + ' ';
			for (const auto& arg : args)
			{
				switch (arg.type)
				{
				case TVM::RegisterType::FLOAT:
					s += std::to_string(arg.value.flt);
					break;
				case TVM::RegisterType::INT:
					s += std::to_string(arg.value.num);
					break;
				case TVM::RegisterType::STRING:
					s += arg.value.str;
					break;
				case TVM::RegisterType::REGISTER:
					s += 'r' + std::to_string(arg.value.num);
					break;
				}
				s += ' ';
			}

			std::cout << s << std::endl;
		}

		return CompileResult::NONE;
	}

}
