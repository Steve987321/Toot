#pragma once

#include <vector>

namespace Compiler
{
    struct Token;

    bool PreProcess(std::vector<Token>& tokens);    
}
