#pragma once

#include <string>
#include <optional>
#include <shlobj.h>
#include <fstream>
#include <vector>
#include "unicode/ucnv.h"
#include "unicode/unistr.h"



struct ErrorMessage
{
	std::string m_location;
	DWORD m_error_id;
};

std::optional<std::string> SelectFile(const std::string& title_ = "");
std::vector<std::string> SelectMultipleFiles(const std::string& initial_dir_ = "");
std::optional<std::string> SelectDirectory(const std::string& title_ = "");

bool Convert1251fileToUTF(const std::string& file_name_, const std::string& result_file_name_);
std::optional<std::string> ToUTF8(const std::string& source_str, const std::string& charset, bool lowercase);

std::string FormatError(DWORD error_code_, const std::string& location_);
std::string GetErrorMessage(DWORD error_code_);
void ShowTextFile(const std::string& file_name_);