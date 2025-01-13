#include "Compiler.h"
#include "Lexer.h"
#include "Parser.h"

#include <iostream>
#include <vector>

namespace Compiler
{

	CompileResult CompileString(std::string_view str)
	{
		std::cout << str << std::endl;
		LexerInit(str);

		std::vector<Token> tokens;
		if (!LexerScan(tokens))
			return CompileResult::ERR;

		// to vm bytecodes

		std::vector<TVM::VM::Instruction> res;

		Parse(tokens, res);

		// tEMP  
		TVM::VM vm;
		vm.instructions = res;
		vm.Init();
		vm.Run();

		/*for (const auto& [op_code, args] : res)
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
		}*/

		return CompileResult::NONE;
	}

}
