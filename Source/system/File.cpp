#include "File.h"

#include <fstream>
#include <sstream>

namespace fs = std::filesystem; 

bool ReadFile(const fs::path& file_path, std::string& out)
{
    std::ifstream file(file_path);
    if (!file)
        return false; 
        
    std::stringstream buffer;
    buffer << file.rdbuf();
    out = buffer.str();
    return true;
}
