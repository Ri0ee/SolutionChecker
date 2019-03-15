#pragma once

#include <string>
#include <stack>
#include <Windows.h>
#include <filesystem>

#include "options.h"
#include "utils.h"



enum CompilerLanguage{
	Pascal, Cpp, C, Java
};

class Compiler
{
public:
	Compiler(OptionsManager* options_manager_) : m_options_manager(options_manager_) {}
	~Compiler() {}
	
	std::string Compile(const std::string& file_name_, CompilerLanguage compiler_language_);

	std::string GetErrorMessage()
	{
		if (!m_error_stack.empty())
		{
			std::string temp_str_buf = FormatError(m_error_stack.top().m_error_id, m_error_stack.top().m_location);
			m_error_stack.pop();
			return temp_str_buf;
		}
		else return std::string("No errors");
	}

private:
	std::string CompileCpp(const std::string& file_name_);
	std::string CompilePascal(const std::string& file_name_);
	std::string CompileJava(const std::string& file_name_);
	std::string CompileC(const std::string& file_name_);

	OptionsManager* m_options_manager = nullptr;

	std::stack<ErrorMessage> m_error_stack;
};