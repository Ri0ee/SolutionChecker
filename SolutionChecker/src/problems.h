#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <tinyxml2.h>
#include <filesystem>
#include <memory>

#include "utils.h"
#include "error_manager.h"



struct Problem
{
	Problem() { Clear(); }
	void Clear()
	{
		m_internal_id = 0;
		m_id = "";
		m_test_count = 0;
		m_bonus_points = 0;
		m_memory_limit = 0;
		m_time_limit = 0;
		m_name = "";
		m_path = "";
		m_input_file = "";
		m_output_file = "";
		m_description_file = "";
		m_checker_exe = "";
		m_checker_src = "";
		m_tests.clear();
	}

	struct Test 
	{
		int points;
		std::string m_input_file;
		std::string m_answer_file;
	};

	std::string m_path;
	std::string m_base_path;

	int m_internal_id;
	int m_test_count;
	int m_bonus_points;
	int m_memory_limit;
	double m_time_limit;
	std::string m_id;
	std::string m_name;
	std::string m_input_file;
	std::string m_output_file;
	std::string m_description_file;
	std::string m_checker_src;
	std::string m_checker_exe;
	std::string m_solution_src;

	std::vector<Test> m_tests;
};

class ProblemManager
{
public:
	ProblemManager(	std::shared_ptr<OptionsManager> options_manager_, 
					std::shared_ptr<ErrorManager> error_manager_) : 
					m_options_manager(options_manager_), 
					m_error_manager(error_manager_) 
					{ 
						if (m_options_manager->GetOption("ProblemDir") != "")
							SearchForProblems();
					}

	void SearchForProblems();
	void ShowTaskDescription(int problem_id_);
	bool ReadProblem(Problem& problem_, const std::string& problem_layout_file_path_);
	int GetFreeID();

	std::vector<Problem> GetProblemList() { return m_problem_list; }
	Problem GetProblem(unsigned int problem_id_) { 
		problem_id_ = min(problem_id_, (unsigned int)(m_problem_list.size() - 1));
		return m_problem_list[problem_id_]; 
	}

private:
	std::shared_ptr<ErrorManager> m_error_manager;
	std::shared_ptr<OptionsManager> m_options_manager;

	std::vector<Problem> m_problem_list;
};