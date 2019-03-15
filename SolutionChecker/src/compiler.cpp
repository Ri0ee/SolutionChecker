#include "compiler.h"



std::string Compiler::Compile(const std::string& file_name_, CompilerLanguage compiler_language_)
{
	switch (compiler_language_)
	{
	case Pascal:	return CompilePascal(file_name_);
	case Cpp:		return CompileCpp(file_name_);
	case C:			return CompileC(file_name_);
	case Java:		return CompileJava(file_name_);
	default:		return std::string();
	}

	return std::string();
}

std::string Compiler::CompilePascal(const std::string& file_name_)
{
	if (file_name_.empty()) return std::string();

	std::string compiler_path = m_options_manager->PascalCompilerDir();
	std::string compiler_dir = compiler_path.substr(0, compiler_path.find_last_of("\\"));
	std::string compiler_args("");
	std::string result_executable_path = file_name_.substr(0, file_name_.find_last_of(".")) + ".exe";
	std::string result_object_path = file_name_.substr(0, file_name_.find_last_of(".")) + ".o";

	compiler_args = compiler_args + 
		"\"" + compiler_path + "\" " + // args[0] - path to the compiler executable file
		"\"" + file_name_ + "\" ";

	// Convert string to LPSTR
	LPSTR compiler_args_lpstr = new char[compiler_args.size()];
	strcpy_s(compiler_args_lpstr, compiler_args.size() + 1, compiler_args.c_str());

	// Remove executable file if there was any
	std::filesystem::remove(result_executable_path);

	STARTUPINFO startup_info;
	PROCESS_INFORMATION process_info;
	ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&startup_info, sizeof(STARTUPINFO));
	startup_info.cb = sizeof(STARTUPINFO);

	if (CreateProcess(compiler_path.c_str(), compiler_args_lpstr, 0, 0, false, CREATE_NO_WINDOW, 0, compiler_dir.c_str(), &startup_info, &process_info) == TRUE)
	{
		WaitForSingleObject(process_info.hProcess, INFINITE);
	}
	else
	{
		m_error_stack.push({ "CreateProcess", GetLastError() });
		return std::string();
	}

	// Delete object file {file_name}.o
	std::filesystem::remove(result_object_path); 

	return result_executable_path;
}

std::string Compiler::CompileCpp(const std::string& file_name_)
{
	if (file_name_.empty()) return std::string();

	return std::string();
}

std::string Compiler::CompileC(const std::string& file_name_)
{
	if (file_name_.empty()) return std::string();

	return std::string();
}

std::string Compiler::CompileJava(const std::string& file_name_)
{
	if (file_name_.empty()) return std::string();

	return std::string();
}