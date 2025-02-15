#pragma once

#include <string_view>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

#include "CPPBinding.h"

class VM;
struct VMRegister;

namespace IO
{
	// Writes to stdout
	void WriteOut(VM& vm, const std::vector<VMRegister>& args);

	// testing the explicit argument types 
	inline void Nothing(VM& vm, const std::vector<VMRegister>& args)
	{
	}

	// for the parser and vm 
	CPPLib GetIOLib();

	void RegisterToVM(VM& vm);
}
