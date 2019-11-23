#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <tinyxml2.h>
#include <filesystem>
#include <memory>
#include "options.h"
#include "error_manager.h"



namespace PROBLEMS {

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
		ProblemManager(	std::shared_ptr<OPTIONS::OptionsManager> options_manager_, 
						std::shared_ptr<SERROR::ErrorManager> error_manager_) :
						m_options_manager(options_manager_), 
						m_error_manager(error_manager_) {}

		void CreateProblem(Problem& problem_, const std::string& problem_layout_file_);
		bool ReadProblem(Problem& problem_, const std::string& problem_layout_file_);
		void SearchForProblems(std::vector<Problem>& problem_list_);
		int GetFreeID();

	private:
		std::shared_ptr<OPTIONS::OptionsManager> m_options_manager;
		std::shared_ptr<SERROR::ErrorManager> m_error_manager;
	};

}