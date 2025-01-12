#pragma once

#include <string_view>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

#include "TVMRegister.h"

namespace TVM
{
	struct VM;
}

namespace IO
{
	void WriteOut(TVM::VM& vm, const std::vector<TVM::Register>& args);

	void Register(TVM::VM& vm);
}
