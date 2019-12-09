#pragma once

#include <string>
#include <stack>
#include <Windows.h>
#include <filesystem>

#include "options.h"
#include "util.h"
#include "error_manager.h"



enum class CompilerLanguage{
	Pascal, Cpp, C, Java
};

class Compiler
{
public:
	Compiler(std::shared_ptr<OptionsManager> options_manager_, std::shared_ptr<ErrorManager> error_manager_) :
		m_options_manager(options_manager_), m_error_manager(error_manager_) {}

	std::optional<std::string> Compile(const std::string& file_name_);

private:
	std::optional<std::string> CompileCpp(const std::string& file_name_);
	std::optional<std::string> CompilePascal(const std::string& file_name_);
	std::optional<std::string> CompileJava(const std::string& file_name_);
	std::optional<std::string> CompileC(const std::string& file_name_);

	std::shared_ptr<OptionsManager> m_options_manager;
	std::shared_ptr<ErrorManager> m_error_manager;
};