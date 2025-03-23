#include "Compiler.h"
#include "TVM.h"

const char* source =
R"(
	// string s = "stringa"; // X
    once float f = 0.0;
    f = f + 1.0;
    WriteOut(f);

    // Nothing();
	//int a = -5;
    //Nothing(a,-a);
	
	// WriteOut(f"c = {c}"); // Test the format string 
	// WriteOut(c); 
	// WriteOut(f); 
	// WriteOut(1000);
	// WriteOut(100*20);
)";

#include <iostream>
int main()
{
    std::vector<VM::Instruction> byte_codes;
    
    if (Compiler::CompileString(source, byte_codes) != Compiler::CompileResult::ERR)
    {
        VM vm;
        IO::RegisterToVM(vm);

        vm.instructions = byte_codes;
        vm.Init();
        vm.Run();
        vm.instruction_pointer =0;
        vm.Run();
    }
    
    for (const std::string& err : Compiler::error_msgs)
    {
        std::cout << err << std::endl;
    }

}
