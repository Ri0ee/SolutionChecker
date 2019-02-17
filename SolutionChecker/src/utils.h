#pragma once

#include <string>
#include <Windows.h>



struct ErrorMessage
{
	std::string m_location;
	DWORD m_error_id;
};

std::string FormatError(DWORD error_code_, const std::string& location_);