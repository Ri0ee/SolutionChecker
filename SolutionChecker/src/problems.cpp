#include "problems.h"



bool ProblemManager::Initialize(const std::string& path_to_problems_folder_)
{
	m_path = path_to_problems_folder_;
	if(!m_path.empty()) SearchForProblems();

	return true;
}

void ProblemManager::Shutdown()
{

}

void ProblemManager::SearchForProblems()
{
	m_problem_list.clear();

	WIN32_FIND_DATA findfiledata;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	char fullpath[MAX_PATH];
	GetFullPathName(m_path.c_str(), MAX_PATH, fullpath, 0);
	std::string fp(fullpath);

	int id_counter = 0;

	hFind = FindFirstFile((LPCSTR)(fp + "\\*").c_str(), &findfiledata);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (findfiledata.cFileName[0] != '.'))
			{
				Problem temp_problem;
				temp_problem.m_id = id_counter;
				temp_problem.m_folder_name = findfiledata.cFileName;
				temp_problem.m_path = fp + "\\" + findfiledata.cFileName;
				temp_problem.m_caption = "";
				temp_problem.m_input_file_name = "";
				temp_problem.m_memory_limit = 0;
				temp_problem.m_output_file_name = "";
				temp_problem.m_test_count = 0;
				temp_problem.m_time_limit = 0;

				m_problem_list.push_back(temp_problem);
				id_counter++;
			}
		} while (FindNextFile(hFind, &findfiledata) != 0);
	}

	for (unsigned i = 0; i < m_problem_list.size(); i++)
	{
		std::fstream input_file(m_problem_list[i].m_path + "\\problem_layout.txt", std::ios::in);
		if (input_file.is_open())
		{
			std::string line;
			while (std::getline(input_file, line))
			{
				int delim_pos = line.find_first_of(":");
				std::string param_name = line.substr(0, delim_pos);

				if (param_name == "Caption")
				{
					m_problem_list[i].m_caption = line.substr(delim_pos + 1, line.size());
					continue;
				}

				if (param_name == "InputFileName")
				{
					m_problem_list[i].m_input_file_name = line.substr(delim_pos + 1, line.size());
					continue;
				}

				if (param_name == "OutputFileName")
				{
					m_problem_list[i].m_output_file_name = line.substr(delim_pos + 1, line.size());
					continue;
				}

				if (param_name == "TestCount")
				{
					std::stringstream param_value(line.substr(delim_pos + 1, line.size()));
					int test_count = 0;
					param_value >> test_count;
					m_problem_list[i].m_test_count = test_count;
					continue;
				}

				if (param_name == "TimeLimit")
				{
					std::stringstream param_value(line.substr(delim_pos + 1, line.size()));
					float time_limit = 0;
					param_value >> time_limit;
					m_problem_list[i].m_time_limit = time_limit;
					continue;
				}

				if (param_name == "MemoryLimit")
				{
					std::stringstream param_value(line.substr(delim_pos + 1, line.size()));
					int memory_limit = 0;
					param_value >> memory_limit;
					m_problem_list[i].m_memory_limit = memory_limit;
					continue;
				}

				if (param_name == "TaskDescriptionFileName")
				{
					m_problem_list[i].m_description_file_name = line.substr(delim_pos + 1, line.size());
					continue;
				}
			}

			input_file.close();
		} // else do nothing
	}
}

void ProblemManager::ShowTaskDescription(int problem_id_)
{
	std::string temp_command_buffer = m_problem_list[problem_id_].m_path + "\\" + m_problem_list[problem_id_].m_description_file_name;
	ShellExecuteA(0, "edit", temp_command_buffer.c_str(), 0, 0, SW_SHOW);
}