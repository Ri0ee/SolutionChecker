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
	if (file_name_.empty()) return false;

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

	if (DeleteFile(result_executable_path.c_str()) != TRUE)
	{
		m_last_error_stack.push({ "DeleteFile", GetLastError() });
	}

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
		m_last_error_stack.push({ "CreateProcess", GetLastError() });
		return false;
	}

	if (DeleteFile(result_object_path.c_str()) != TRUE) // Delete object file {file_name}.o
	{
		m_last_error_stack.push({ "DeleteFile", GetLastError() });
	}

	return true;
}

bool Compiler::CompileCpp(const std::string& file_name_)
{
	if (file_name_.empty()) return false;

	return true;
}

bool Compiler::CompileC(const std::string& file_name_)
{
	if (file_name_.empty()) return false;

	return true;
}

bool Compiler::CompileJava(const std::string& file_name_)
{
	if (file_name_.empty()) return false;

	return true;
}