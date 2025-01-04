#include "Compiler.h"

const char* source =
R"(
	string s = "stringa";
	int a = 5;
	int b = 10;
	int c = a + b * a;
	print(f"c = {c}");
)";

int main()
{
	Compiler::CompileString(source);
}
