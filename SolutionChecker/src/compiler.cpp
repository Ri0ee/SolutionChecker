#include "compiler.h"



bool Compiler::Compile(const std::string& file_name_, CompilerLanguage compiler_language_)
{
	switch (compiler_language_)
	{
	case Pascal:	return CompilePascal(file_name_);
	case Cpp:		return CompileCpp(file_name_);
	case C:			return CompileC(file_name_);
	case Java:		return CompileJava(file_name_);
	default:		return false;
	}

	return false;
}

bool Compiler::CompilePascal(const std::string& file_name_)
{

	return true;
}

bool Compiler::CompileCpp(const std::string& file_name_)
{

	return true;
}

bool Compiler::CompileC(const std::string& file_name_)
{

	return true;
}

bool Compiler::CompileJava(const std::string& file_name_)
{

	return true;
}