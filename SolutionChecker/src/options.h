#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>



class OptionsManager
{
public:
	OptionsManager(const std::string& path_to_options_file_) : m_path_to_options_file(path_to_options_file_) { GetOptions(); }
	OptionsManager() {}
	~OptionsManager() {}

	bool GetOptions();
	bool UpdateOptionsFile();
	void SetDefaults();

	float& WindowPosX() { return m_window_pos_x; }
	float& WindowPosY() { return m_window_pos_y; }
	bool& UseOnlyOneTest() { return m_use_only_one_test; }
	bool& UseMultipleTests() { return m_use_multiple_tests; }
	int& TestMemoryLimit() { return m_test_memory_limit; }
	int& LastProblem() { return m_last_problem; }

	std::string& CppCompilerDir() { return m_cpp_compiler_dir; }
	std::string& CCompilerDir() { return m_c_compiler_dir; }
	std::string& PascalCompilerDir() { return m_pascal_compiler_dir; }
	std::string& JavaCompilerDir() { return m_java_compiler_dir; }

	std::string& CppDefaultArgs() { return m_cpp_default_args; }
	std::string& CDefaultArgs() { return m_c_default_args; }
	std::string& PascalDefaultArgs() { return m_pascal_default_args; }
	std::string& JavaDefaultArgs() { return m_java_default_args; }

	std::string& ThemeName() { return m_theme; }
	std::string& WorkingDir() { return m_working_dir; }
	std::string& LastExecutableDir() { return m_last_exe_dir; }
	std::string& ProblemDir() { return m_problem_dir; }
	std::string& OptionsFilePath() { return m_path_to_options_file; }
	std::string& AppPath() { return m_app_path; }
	std::string& TempDir() { return m_temp_dir; }

	std::string GetThemeName(int id_);
	int GetThemeId(const std::string& theme_);
	int GetThemeId() { return GetThemeId(m_theme); }

private:
	std::string m_path_to_options_file;
	std::string m_app_path;

	float m_window_pos_x;
	float m_window_pos_y;
	int m_last_problem;
	int m_test_memory_limit;
	bool m_use_only_one_test;
	bool m_use_multiple_tests;
	std::string m_working_dir;
	std::string m_problem_dir;
	std::string m_temp_dir;
	std::string m_last_exe_dir;
	std::string m_theme;

	std::string m_cpp_compiler_dir;
	std::string m_c_compiler_dir;
	std::string m_pascal_compiler_dir;
	std::string m_java_compiler_dir;

	std::string m_cpp_default_args;
	std::string m_c_default_args;
	std::string m_pascal_default_args;
	std::string m_java_default_args;
};