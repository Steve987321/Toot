#include "Compiler.h"
#include "TVM.h"

#include "Lib/IO.h"

const char* source =
R"(
	#include "somejuice.txt"

	int abc = a * 2;
)";

int main()
{
	std::vector<VM::Instruction> byte_codes;
	
	CPPLib io_lib = IO::GetIOLib();
	
	VM vm;
	vm.RegisterLib(IO::GetIOLib());
	
	if (Compiler::CompileString(source, byte_codes, &vm) != Compiler::CompileResult::ERR)
	{
		std::cout << "RUN\n";
		vm.instructions = byte_codes;
		vm.Init();
		vm.Run();
//		vm.instruction_pointer = 0;
//		vm.Run();
	}
	
	for (const std::string& err : Compiler::error_msgs)
	{
		std::cout << err << std::endl;
	}

}
