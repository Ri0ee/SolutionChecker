#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>



class OptionsManager
{
public:
	OptionsManager() {}
	~OptionsManager() {}

	bool GetOptions();
	bool UpdateOptionsFile();
	void SetDefaults();

	std::string GetOptionsFilePath() { return m_path_to_options_file; }
	void SetOptionsFilePath(const std::string& path_to_options_file_) { m_path_to_options_file = path_to_options_file_; }
	
	float GetWindowPosX() { return m_window_pos_x; }
	void SetWindowPosX(float window_pos_x_) { m_window_pos_x = window_pos_x_; }

	float GetWindowPosY() { return m_window_pos_y; }
	void SetWindowPosY(float window_pos_y_) { m_window_pos_y = window_pos_y_; }

	std::string GetWorkingDir() { return m_working_dir; }
	void SetWorkingDir(const std::string& working_dir_) { m_working_dir = working_dir_; }

	std::string GetLastExecutableDir() { return m_last_exe_dir; }
	void SetLastExecutableDir(const std::string& last_exe_dir_) { m_last_exe_dir = last_exe_dir_; }

	std::string GetProblemDir() { return m_problem_dir; }
	void SetProblemDir(const std::string& problem_dir_) { m_problem_dir = problem_dir_; }

	std::string GetLastProblem() { return m_last_problem; }
	void SetLastProblem(const std::string& last_problem_) {	m_last_problem = last_problem_; }

	bool GetUseOnlyOneTest() { return m_use_only_one_test; }
	void SetUseOnlyOneTest(const bool use_only_one_test_) { m_use_only_one_test = use_only_one_test_; }

	bool GetUseMultipleTests() { return m_use_multiple_tests; }
	void SetUseMultipleTests(const bool use_multiple_tests_) { m_use_multiple_tests = use_multiple_tests_; }

	std::string GetTheme() { return m_theme; }
	void SetTheme(const std::string& theme_) { m_theme = theme_; }

private:
	std::string m_path_to_options_file;

	float m_window_pos_x;
	float m_window_pos_y;
	std::string m_working_dir;
	std::string m_last_problem;
	std::string m_problem_dir;
	std::string m_last_exe_dir;
	std::string m_theme;
	bool m_use_only_one_test;
	bool m_use_multiple_tests;
};