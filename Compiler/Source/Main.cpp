#include "Compiler.h"

const char* source =
R"(
	// string s = "stringa"; // X
	int a = -5;
    Nothing(a,-a);
	
	// WriteOut(f"c = {c}"); // Test the format string 
	// WriteOut(c); 
	// WriteOut(f); 
	// WriteOut(1000);
	// WriteOut(100*20);
)";

#include <iostream>
int main()
{
	Compiler::CompileString(source);
}
