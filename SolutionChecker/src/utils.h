#pragma once

#include <Windows.h>
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

std::string FormatError(DWORD error_code_, const std::string& location_);
std::string GetErrorMessage(DWORD error_code_);
void ShowTextFile(const std::string& file_name_);

std::optional<std::string> SelectFile(const std::string& title_ = "");
std::optional<std::string> SelectDirectory(const std::string& title_ = "");