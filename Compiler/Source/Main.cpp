#include "Compiler.h"

const char* source =
R"(
	int a = 5;
	int b = 10;
	int c = a + b * a;
)";

int main()
{
	Compiler::CompileString(source);
}
