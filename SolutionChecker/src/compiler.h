#pragma once

#include <string>
#include "options.h"

enum CompilerLanguage{
	Pascal, Cpp, C, Java
};



class Compiler
{
public:
	Compiler(OptionsManager* options_manager_) : m_options_manager(options_manager_) {}
	~Compiler() {}
	
	bool Compile(const std::string& file_name_, CompilerLanguage compiler_language_);

private:
	bool CompileCpp(const std::string& file_name_);
	bool CompilePascal(const std::string& file_name_);
	bool CompileJava(const std::string& file_name_);
	bool CompileC(const std::string& file_name_);

	OptionsManager* m_options_manager = nullptr;
};