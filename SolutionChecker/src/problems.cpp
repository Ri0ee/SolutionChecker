#include "problems.h"



void ProblemManager::Initialize()
{
	if(!m_path.empty()) SearchForProblems();
}

void ProblemManager::SearchForProblems()
{
	return;

	//m_problem_list.clear();

	//WIN32_FIND_DATA findfiledata;
	//HANDLE hFind = INVALID_HANDLE_VALUE;

	//char fullpath[MAX_PATH];
	//GetFullPathName(m_path.c_str(), MAX_PATH, fullpath, 0);
	//std::string fp(fullpath);

	//int id_counter = 0;

	//hFind = FindFirstFile((LPCSTR)(fp + "\\*").c_str(), &findfiledata);
	//if (hFind != INVALID_HANDLE_VALUE)
	//{
	//	do
	//	{
	//		if ((findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (findfiledata.cFileName[0] != '.'))
	//		{
	//			Problem temp_problem;
	//			temp_problem.m_id = id_counter;
	//			temp_problem.m_folder = findfiledata.cFileName;
	//			temp_problem.m_path = fp + "\\" + findfiledata.cFileName;
	//			temp_problem.m_caption = "";
	//			temp_problem.m_input_file = "";
	//			temp_problem.m_memory_limit = 0;
	//			temp_problem.m_output_file = "";
	//			temp_problem.m_test_count = 0;
	//			temp_problem.m_time_limit = 0;

	//			m_problem_list.push_back(temp_problem);
	//			id_counter++;
	//		}
	//	} while (FindNextFile(hFind, &findfiledata) != 0);
	//}

	//for (unsigned i = 0; i < m_problem_list.size(); i++)
	//{
	//	std::fstream input_file(m_problem_list[i].m_path + "\\problem_layout.txt", std::ios::in);
	//	if (input_file.is_open())
	//	{
	//		std::string line;
	//		while (std::getline(input_file, line))
	//		{
	//			int delim_pos = line.find_first_of(":");
	//			std::string param_name = line.substr(0, delim_pos);

	//			if (param_name == "Caption")
	//			{
	//				m_problem_list[i].m_caption = line.substr(delim_pos + 1, line.size());
	//				continue;
	//			}

	//			if (param_name == "InputFileName")
	//			{
	//				m_problem_list[i].m_input_file = line.substr(delim_pos + 1, line.size());
	//				continue;
	//			}

	//			if (param_name == "OutputFileName")
	//			{
	//				m_problem_list[i].m_output_file = line.substr(delim_pos + 1, line.size());
	//				continue;
	//			}

	//			if (param_name == "TestCount")
	//			{
	//				std::stringstream param_value(line.substr(delim_pos + 1, line.size()));
	//				int test_count = 0;
	//				param_value >> test_count;
	//				m_problem_list[i].m_test_count = test_count;
	//				continue;
	//			}

	//			if (param_name == "TimeLimit")
	//			{
	//				std::stringstream param_value(line.substr(delim_pos + 1, line.size()));
	//				float time_limit = 0;
	//				param_value >> time_limit;
	//				m_problem_list[i].m_time_limit = time_limit;
	//				continue;
	//			}

	//			if (param_name == "MemoryLimit")
	//			{
	//				std::stringstream param_value(line.substr(delim_pos + 1, line.size()));
	//				int memory_limit = 0;
	//				param_value >> memory_limit;
	//				m_problem_list[i].m_memory_limit = memory_limit;
	//				continue;
	//			}

	//			if (param_name == "TaskDescriptionFileName")
	//			{
	//				m_problem_list[i].m_description_file = line.substr(delim_pos + 1, line.size());
	//				continue;
	//			}
	//		}

	//		input_file.close();
	//	} // else do nothing
	//}
}

void ProblemManager::ShowTaskDescription(int problem_id_)
{
	std::string file_dir = m_problem_list[problem_id_].m_path + "\\" + m_problem_list[problem_id_].m_description_file;
	ShowTextFile(file_dir);
}

void ProblemManager::CreateProblem(Problem& problem_, const std::string& problem_layout_file_)
{
	std::filesystem::path problem_folder = problem_.m_folder;
	std::filesystem::path result_path = m_path / problem_folder;
	std::filesystem::create_directory(result_path);

	tinyxml2::XMLDocument problem_layout;

	auto declaration_element = problem_layout.NewDeclaration(nullptr);
	auto format_element = problem_layout.NewComment("Problem exchange format 0.1");

	problem_layout.InsertFirstChild(declaration_element);
	problem_layout.InsertEndChild(format_element);

	auto problem_element = problem_layout.NewElement("problem");
	problem_element->SetValue("Problem");
	problem_element->SetAttribute("Gid", problem_.m_id);
	problem_element->SetAttribute("Name", problem_.m_caption.c_str());
	problem_element->SetAttribute("TestCount", problem_.m_test_count);
	problem_element->SetAttribute("BonusPoints", problem_.m_bonus_points);
	problem_element->SetAttribute("CheckerExe", problem_.m_checker_exe.c_str());
	problem_element->SetAttribute("CheckerSrc", problem_.m_checker_src.c_str());
	problem_element->SetAttribute("Description", problem_.m_description_file.c_str());
	problem_element->SetAttribute("TimeLimit", problem_.m_time_limit);
	problem_element->SetAttribute("MemoryLimit", problem_.m_memory_limit);
	problem_element->SetAttribute("InputFile", problem_.m_input_file.c_str());
	problem_element->SetAttribute("OutputFile", problem_.m_output_file.c_str());

	for (auto test : problem_.m_tests) 
	{
		auto test_element = problem_layout.NewElement("Test");
		test_element->SetValue("Test");

		test_element->SetAttribute("Input", test.m_input_file.c_str());
		test_element->SetAttribute("Answer", test.m_answer_file.c_str());
		test_element->SetAttribute("Points", test.points);

		problem_element->InsertEndChild(test_element);
	}

	problem_layout.InsertEndChild(problem_element);

	problem_layout.SaveFile((result_path.string() + problem_layout_file_).c_str());
}

bool ProblemManager::ReadProblem(Problem& problem_, const std::string& problem_layout_file_path_)
{
	tinyxml2::XMLDocument problem_layout;
	problem_layout.LoadFile(problem_layout_file_path_.c_str());



	return true;
}