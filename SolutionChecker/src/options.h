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

	std::string GetThemeName(int id_) 
	{ 
		switch (id_)
		{
		case 0:		return std::string("none");
		case 1:		return std::string("gtk+");
		case 2:		return std::string("gleam");
		case 3:		return std::string("plastic");
		default:	return std::string("none");
		}
	}

	int GetThemeId(const std::string& theme_)
	{
		if (theme_ == "none") return 0;
		else if (theme_ == "gtk+") return 1;
		else if (theme_ == "gleam") return 2;
		else if (theme_ == "plastic") return 3;
		else return 0;
	}

	std::string GetThemeName() { return m_theme; }
	int GetThemeId() { return GetThemeId(m_theme); }
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