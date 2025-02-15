#include "Compiler.h"
#include "Lexer.h"
#include "PreProcess.h"
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

		std::vector<VM::Instruction> res;

		//PreProcess(tokens);

		Parse(tokens, res);

		// tEMP  
		VM vm;
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
				case RegisterType::FLOAT:
					s += std::to_string(arg.value.flt);
					break;
				case RegisterType::INT:
					s += std::to_string(arg.value.num);
					break;
				case RegisterType::STRING:
					s += arg.value.str;
					break;
				case RegisterType::REGISTER:
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
