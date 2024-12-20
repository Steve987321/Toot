#pragma once

#include "Lexer.h"
#include <vector>
#include "TVMByteCodes.h"
#include "TVM.h"

namespace Compiler
{
	bool Parse(const std::vector<Token>& tokens, std::vector<std::pair<TVM::OP_CODE, std::vector<TVM::Register>>>& op_codes_res);
}