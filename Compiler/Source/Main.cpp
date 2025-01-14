#include "Compiler.h"

const char* source =
R"(
	// string s = "stringa"; // X
	 int a = 5;
	 int b = 10;
	// int c = 2 / 2 + 10 + 10;
	 int c = a + b * a;
	 int f = c * 10;
	// WriteOut(f"c = {c}"); // Test the format string 
	// WriteOut(c); 
	 WriteOut(f - 5); 
	// WriteOut(1000);
	// WriteOut(100*20);
)";

int main()
{
	Compiler::CompileString(source);
}
