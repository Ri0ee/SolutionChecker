#pragma once

#include <string>
#include <optional>
#include <shlobj.h>
#include <fstream>
#include <vector>
#include "unicode/ucnv.h"
#include "unicode/unistr.h"

std::optional<std::string> SelectFile(const std::string& title_ = "");
std::optional<std::string> SelectDirectory(const std::string& title_ = "");
bool Convert1251fileToUTF(const std::string& file_name_, const std::string& result_file_name_);
std::optional<std::string> ToUTF8(const std::string& source_str, const std::string& charset, bool lowercase);
std::vector<std::string> SelectMultipleFiles(const std::string& initial_dir_ = "");