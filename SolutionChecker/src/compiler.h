#pragma once

#include <string>
#include <stack>
#include <Windows.h>
#include <filesystem>

#include "options.h"
#include "utils.h"
#include "error_manager.h"



enum class CompilerLanguage{
	Pascal, Cpp, C, Java
};

class Compiler
{
public:
	Compiler(OptionsManager* options_manager_, ErrorManager* error_manager_) : 
		m_options_manager(options_manager_), m_error_manager(error_manager_) {}
	~Compiler() {}
	
	std::string Compile(const std::string& file_name_, CompilerLanguage compiler_language_);

private:
	std::string CompileCpp(const std::string& file_name_);
	std::string CompilePascal(const std::string& file_name_);
	std::string CompileJava(const std::string& file_name_);
	std::string CompileC(const std::string& file_name_);

	OptionsManager* m_options_manager = nullptr;
	ErrorManager* m_error_manager = nullptr;
};