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
        error_msgs.clear();
        
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
        
        for (const std::string& err : error_msgs)
        {
            std::cout << err << std::endl; 
        }

		return CompileResult::NONE;
	}

}
