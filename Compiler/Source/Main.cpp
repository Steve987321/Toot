#include "Compiler.h"

int main()
{
	Compiler::CompileString(
		"int a = 0;"
		"int b = 123;"
		"int c = b * a + b;");
}
