#include "Compiler.h"

const char* source =
R"(
	// string s = "stringa";
	// int a = 5;
	// int b = 10;
	int c = 2 / 2 + 10 + 10; // X
	// int c = a + b * a;
	// int f = c * 10;
	// WriteOut(f"c = {c}"); // Test the format string 
	WriteOut(c); 
	// WriteOut(1000);
)";

int main()
{
	Compiler::CompileString(source);
}
