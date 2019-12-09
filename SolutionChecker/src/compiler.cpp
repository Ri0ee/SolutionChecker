#include "compiler.h"



std::optional<std::string> Compiler::Compile(const std::string& file_name_)
{
	std::string solution_file_type = file_name_.substr(file_name_.find_last_of(".") + 1);

	if (solution_file_type == "java" || solution_file_type == "exe") return file_name_;
	if (solution_file_type == "pas") return CompilePascal(file_name_);
	if (solution_file_type == "cpp") return CompileCpp(file_name_);
	if (solution_file_type == "c") return CompileC(file_name_);

	m_error_manager->PushError({ "Unknown file type", "Testing", 0, 0, Severity::Fatal });
	return std::nullopt;
}

std::optional<std::string> Compiler::CompilePascal(const std::string& file_name_)
{
	if (file_name_.empty()) return std::nullopt;

	std::string compiler_path = m_options_manager->GetOption("PascalCompilerDir");
	std::string compiler_dir = compiler_path.substr(0, compiler_path.find_last_of("\\"));
	std::string result_executable_path = file_name_.substr(0, file_name_.find_last_of(".")) + ".exe";
	std::string result_object_path = file_name_.substr(0, file_name_.find_last_of(".")) + ".o";
	std::string compiler_args = "\"" + compiler_path + "\" " +	// args[0] - path to the compiler executable file
								"\"" + file_name_ + "\" ";		// args[1] - path to the source code

	// Convert string to LPSTR
	LPSTR compiler_args_lpstr = new char[compiler_args.size()];
	strcpy_s(compiler_args_lpstr, compiler_args.size() + 1, compiler_args.c_str());

	// Remove executable file if there was any
	std::filesystem::remove(result_executable_path);

	STARTUPINFO startup_info = { 0 };
	PROCESS_INFORMATION process_info = { 0 };
	startup_info.cb = sizeof(STARTUPINFO);

	if (CreateProcess(	compiler_path.c_str(),
						compiler_args_lpstr,
						0,
						0,
						false,
						CREATE_NO_WINDOW,
						0,
						compiler_dir.c_str(),
						&startup_info,
						&process_info))
	{
		WaitForSingleObject(process_info.hProcess, INFINITE);
		CloseHandle(process_info.hProcess);
		CloseHandle(process_info.hThread);
	}
	else
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "CreateProcess (Compilation)", 0, 0, Severity::Fatal });
		return std::nullopt;
	}

	// Delete object file {file_name}.o
	std::filesystem::remove(result_object_path); 

	// Remove temporary source code file
	std::filesystem::remove(file_name_);

	if (!std::filesystem::exists(result_executable_path))
	{
		m_error_manager->PushError({ "Result file does not exist", "Compilation", 0, 0, Severity::Fatal });
		return std::nullopt;
	}
		
	return result_executable_path;
}

std::optional<std::string> Compiler::CompileCpp(const std::string& file_name_)
{
	if (file_name_.empty()) return std::nullopt;

	std::string compiler_path = m_options_manager->GetOption("CppCompilerDir");
	std::string compiler_dir = m_options_manager->GetOption("TempDir");
	std::string result_executable_path = m_options_manager->GetOption("TempDir") + "\\a.exe";
	std::string compiler_args = 
		"\"" + compiler_path + "\" " +						// args[0] - path to the compiler executable file
		"\"" + file_name_ + "\" " +							// args[1] - path to the source code
		m_options_manager->GetOption("CppDefaultArgs");		// other arguments

	// Convert string to LPSTR
	LPSTR compiler_args_lpstr = new char[compiler_args.size()];
	strcpy_s(compiler_args_lpstr, compiler_args.size() + 1, compiler_args.c_str());

	// Remove executable file if there was any
	std::filesystem::remove(result_executable_path);

	STARTUPINFO startup_info = { 0 };
	PROCESS_INFORMATION process_info = { 0 };
	startup_info.cb = sizeof(STARTUPINFO);

	if (CreateProcess(compiler_path.c_str(),
		compiler_args_lpstr,
		0,
		0,
		false,
		CREATE_NO_WINDOW,
		0,
		compiler_dir.c_str(),
		&startup_info,
		&process_info))
	{
		WaitForSingleObject(process_info.hProcess, INFINITE);
		CloseHandle(process_info.hProcess);
		CloseHandle(process_info.hThread);
	}
	else
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "CreateProcess (Compilation)", 0, 0, Severity::Fatal });
		return std::nullopt;
	}

	// Remove temporary source code file
	std::filesystem::remove(file_name_);

	if (!std::filesystem::exists(result_executable_path))
	{
		m_error_manager->PushError({ "Result file does not exist", "Compilation", 0, 0, Severity::Fatal });
		return std::nullopt;
	}

	return result_executable_path;
}

std::optional<std::string> Compiler::CompileC(const std::string& file_name_)
{
	if (file_name_.empty()) return std::nullopt;

	std::string compiler_path = m_options_manager->GetOption("CCompilerDir");
	std::string compiler_dir = m_options_manager->GetOption("TempDir");
	std::string result_executable_path = m_options_manager->GetOption("TempDir") + "\\a.exe";
	std::string compiler_args =
		"\"" + compiler_path + "\" " +						// args[0] - path to the compiler executable file
		"\"" + file_name_ + "\" " +							// args[1] - path to the source code
		m_options_manager->GetOption("CDefaultArgs");		// other arguments

	// Convert string to LPSTR
	LPSTR compiler_args_lpstr = new char[compiler_args.size()];
	strcpy_s(compiler_args_lpstr, compiler_args.size() + 1, compiler_args.c_str());

	// Remove executable file if there was any
	std::filesystem::remove(result_executable_path);

	STARTUPINFO startup_info = { 0 };
	PROCESS_INFORMATION process_info = { 0 };
	startup_info.cb = sizeof(STARTUPINFO);

	if (CreateProcess(compiler_path.c_str(),
		compiler_args_lpstr,
		0,
		0,
		false,
		CREATE_NO_WINDOW,
		0,
		compiler_dir.c_str(),
		&startup_info,
		&process_info))
	{
		WaitForSingleObject(process_info.hProcess, INFINITE);
		CloseHandle(process_info.hProcess);
		CloseHandle(process_info.hThread);
	}
	else
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "CreateProcess (Compilation)", 0, 0, Severity::Fatal });
		return std::nullopt;
	}

	// Remove temporary source code file
	std::filesystem::remove(file_name_);

	if (!std::filesystem::exists(result_executable_path))
	{
		m_error_manager->PushError({ "Result file does not exist", "Compilation", 0, 0, Severity::Fatal });
		return std::nullopt;
	}

	return result_executable_path;
}

std::optional<std::string> Compiler::CompileJava(const std::string& file_name_)
{
	if (file_name_.empty()) return std::nullopt;

	std::string compiler_path = m_options_manager->GetOption("JavaCompilerDir");
	std::string compiler_dir = m_options_manager->GetOption("TempDir");
	std::string result_executable_path = file_name_.substr(0, file_name_.find_last_of(".")) + ".class";
	std::string compiler_args =
		"\"" + compiler_path + "\" " +						// args[0] - path to the compiler executable file
		"\"" + file_name_ + "\" " +							// args[1] - path to the source code
		m_options_manager->GetOption("JavaDefaultArgs");	// other arguments

	// Convert string to LPSTR
	LPSTR compiler_args_lpstr = new char[compiler_args.size()];
	strcpy_s(compiler_args_lpstr, compiler_args.size() + 1, compiler_args.c_str());

	// Remove executable file if there was any
	std::filesystem::remove(result_executable_path);

	STARTUPINFO startup_info = { 0 };
	PROCESS_INFORMATION process_info = { 0 };
	startup_info.cb = sizeof(STARTUPINFO);

	if (CreateProcess(compiler_path.c_str(),
		compiler_args_lpstr,
		0,
		0,
		false,
		CREATE_NO_WINDOW,
		0,
		compiler_dir.c_str(),
		&startup_info,
		&process_info))
	{
		DWORD exit_code;
		WaitForSingleObject(process_info.hProcess, INFINITE);
		GetExitCodeProcess(process_info.hProcess, &exit_code);
		CloseHandle(process_info.hProcess);
		CloseHandle(process_info.hThread);
	}
	else
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "CreateProcess (Compilation)", 0, 0, Severity::Fatal });
		return std::nullopt;
	}

	// Remove temporary source code file
	std::filesystem::remove(file_name_);

	if (!std::filesystem::exists(result_executable_path))
	{
		m_error_manager->PushError({ "Result file does not exist", "Compilation", 0, 0, Severity::Fatal });
		return std::nullopt;
	}

	return result_executable_path;
}