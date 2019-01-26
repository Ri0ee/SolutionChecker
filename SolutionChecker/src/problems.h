#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <Windows.h>



struct Problem
{
	int m_id;
	int m_test_count;
	int m_memory_limit;
	double m_time_limit;
	std::string m_caption;
	std::string m_path;
	std::string m_folder_name;
	std::string m_input_file_name;
	std::string m_output_file_name;
	std::string m_description_file_name;
};

class ProblemManager
{
public:
	ProblemManager(const std::string& path_to_problems_folder_) {
		Initialize(path_to_problems_folder_);
	}

	ProblemManager() {}

	~ProblemManager() {}

	void SearchForProblems();
	void ShowTaskDescription(int problem_id_);
	void ChangeDir(const std::string& path_to_problems_folder_) { m_path = path_to_problems_folder_; }

	std::vector<Problem> GetProblemList() { return m_problem_list; }
	Problem GetProblem(unsigned int problem_id_) { 
		problem_id_ = min(problem_id_, m_problem_list.size() - 1);
		return m_problem_list[problem_id_]; 
	}

private:
	void Initialize(const std::string& path_to_problems_folder_);

	std::string m_path;
	std::vector<Problem> m_problem_list;
};