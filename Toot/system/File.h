#pragma once

#include <filesystem>
#include <string>

bool ReadFile(const std::filesystem::path& file_path, std::string& out);
