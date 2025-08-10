#include "Compiler.h"
#include "Lexer.h"
#include "PreProcess.h"
#include "Parser.h"

#include <iostream>
#include <vector>

namespace Compiler
{

CompileResult CompileString(std::string_view str, std::vector<VM::Instruction>& res, VM* vm)
{
	// #TODO: do something 
	error_msgs.clear();
	for (void* p : strings)
		free(p);
	strings.clear();

	std::cout << str << std::endl;

	LexerInit(str);

	std::vector<Token> tokens;
	if (!LexerScan(tokens))
		return CompileResult::ERR;

	if (!PreProcess(tokens))
		return CompileResult::ERR;

	// to vm bytecodes

	auto parse_res = Parse(tokens, vm);

	if (parse_res)
	{
		res = *parse_res;
	}
	else
	{
		error_msgs.insert(error_msgs.end(), parse_res.error().begin(), parse_res.error().end());
		return CompileResult::ERR;
	}

	return CompileResult::NONE;
}

}
