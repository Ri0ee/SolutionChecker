#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <tinyxml2.h>
#include <filesystem>

#include "utils.h"
#include "error_manager.h"



struct Problem
{
	struct Test 
	{
		int points;
		std::string m_input_file;
		std::string m_answer_file;
	};

	int m_id;
	int m_test_count;
	int m_bonus_points;
	int m_memory_limit;
	double m_time_limit;

	std::string m_caption;
	std::string m_path;
	std::string m_folder;

	std::string m_input_file;
	std::string m_output_file;
	std::string m_description_file;
	std::string m_checker_src;
	std::string m_checker_exe;

	std::vector<Test> m_tests;
};

class ProblemManager
{
public:
	ProblemManager(const std::string& path_to_problems_folder_, ErrorManager* error_manager_) : 
		m_path(path_to_problems_folder_), m_error_manager(error_manager_) { 
		Initialize(); 
	}
	ProblemManager() {}
	~ProblemManager() {}

	void SearchForProblems();
	void ShowTaskDescription(int problem_id_);
	void ChangeDir(const std::string& path_to_problems_folder_) { m_path = path_to_problems_folder_; }

	void CreateProblem(Problem& problem_, const std::string& problem_layout_file_);
	bool ReadProblem(Problem& problem_, const std::string& problem_layout_file_path_);

	std::vector<Problem> GetProblemList() { return m_problem_list; }
	Problem GetProblem(unsigned int problem_id_) { 
		problem_id_ = min(problem_id_, m_problem_list.size() - 1);
		return m_problem_list[problem_id_]; 
	}

private:
	void Initialize();

	ErrorManager* m_error_manager = nullptr;

	std::filesystem::path m_path;
	std::vector<Problem> m_problem_list;
};