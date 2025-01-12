#include "Compiler.h"

const char* source =
R"(
	// string s = "stringa";
	int a = 5;
	int b = 10;
	int c = a + b * a;
	// WriteOut(f"c = {c}"); // Test the format string 
	WriteOut(12);
)";

int main()
{
	Compiler::CompileString(source);
}
