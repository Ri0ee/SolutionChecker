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
	std::string& LastSolutionDir() { return m_last_solution_dir; }
	std::string& ProblemDir() { return m_problem_dir; }
	std::string& OptionsFilePath() { return m_path_to_options_file; }
	std::string& AppPath() { return m_app_path; }
	std::string& TempDir() { return m_temp_dir; }

	std::string& AutofillInputFile() { return m_autofill_inputfile; }
	std::string& AutofillOutputFile() { return m_autofill_outputfile; }
	std::string& AutofillDescriptionFile() { return m_autofill_descriptionfile; }
	std::string& AutofillCheckerSrc() { return m_autofill_checkersrc; }
	std::string& AutofillCheckerExe() { return m_autofill_checkerexe; }
	std::string& AutofillSolution() { return m_autofill_solution; }
	int& AutofillPoints() { return m_autofill_points; }
	int& AutofillBonusPoints() { return m_autofill_bonuspoints; }
	int& AutofillMemLimit() { return m_autofill_memlimit; }
	double& AutofillTimeLimit() { return m_autofill_timelimit; }

	std::string& JavaVMPath() { return m_javavm_path; }

	std::string GetThemeName(int id_);
	int GetThemeId(const std::string& theme_);
	int GetThemeId() { return GetThemeId(m_theme); }

private:
	std::string m_path_to_options_file;
	std::string m_app_path;

	float m_window_pos_x = 0;
	float m_window_pos_y = 0;
	int m_last_problem = 0;
	int m_test_memory_limit = 0;
	bool m_use_only_one_test = 0;
	bool m_use_multiple_tests = 0;
	std::string m_working_dir;
	std::string m_problem_dir;
	std::string m_temp_dir;
	std::string m_last_solution_dir;
	std::string m_theme;

	std::string m_cpp_compiler_dir;
	std::string m_c_compiler_dir;
	std::string m_pascal_compiler_dir;
	std::string m_java_compiler_dir;

	std::string m_cpp_default_args;
	std::string m_c_default_args;
	std::string m_pascal_default_args;
	std::string m_java_default_args;

	std::string m_autofill_inputfile;
	std::string m_autofill_outputfile;
	std::string m_autofill_descriptionfile;
	std::string m_autofill_checkersrc;
	std::string m_autofill_checkerexe;
	std::string m_autofill_solution;
	int m_autofill_points = 0;
	int m_autofill_bonuspoints = 0;
	double m_autofill_timelimit = 0;
	int m_autofill_memlimit = 0;

	std::string m_javavm_path;
};