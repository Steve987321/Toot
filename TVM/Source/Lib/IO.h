#pragma once

#include <string_view>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

#include "TVMRegister.h"
#include "CPPBinding.h"

namespace TVM
{
	struct VM;
}

namespace IO
{
	void WriteOut(TVM::VM& vm, const std::vector<TVM::Register>& args);

	// for the parser plzs 
	TVM::CPPLib GetIOLib();

	void Register(TVM::VM& vm);
}
