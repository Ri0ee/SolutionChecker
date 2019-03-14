#include "utils.h"



std::string FormatError(DWORD error_code_, const std::string& location_)
{
	std::string temp_str_buf("");
	LPTSTR error_text = NULL;

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
		error_code_, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&error_text, 0, NULL);

	if (NULL != error_text)
	{
		temp_str_buf = location_ + ": " + std::string(error_text);
		LocalFree(error_text);
		error_text = NULL;
	}

	return temp_str_buf;
}

void ShowTextFile(const std::string& file_name_)
{
	ShellExecuteA(0, "open", file_name_.c_str(), 0, 0, SW_SHOW);
}