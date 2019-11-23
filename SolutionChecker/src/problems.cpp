#include "problems.h"



void ProblemManager::SearchForProblems()
{
	if (m_options_manager->GetOption("ProblemDir").empty()) return;
	m_problem_list.clear();

	for (auto& dir : std::filesystem::directory_iterator(m_options_manager->GetOption("ProblemDir")))
	{
		if (!dir.is_directory())
			continue;

		Problem problem;
		problem.m_path = dir.path().string();
		if (!ReadProblem(problem, dir.path().string() + "\\ProblemLayout.xml"))
			continue;
		
		m_problem_list.push_back(problem);
	}

	// Sort problems by id
	std::sort(m_problem_list.begin(), m_problem_list.end(), [](Problem& p1, Problem& p2) -> bool {
		return p1.m_internal_id < p2.m_internal_id;
	});
}

void ProblemManager::ShowTaskDescription(int problem_id_)
{
	std::string file_dir = m_problem_list[problem_id_].m_path + "\\" + m_problem_list[problem_id_].m_description_file;
	ShowTextFile(file_dir);
}

bool ProblemManager::ReadProblem(Problem& problem_, const std::string& problem_layout_file_path_)
{
	const char* temp_str_buf;
	int temp_int_attribute = 0;

	tinyxml2::XMLDocument problem_layout;
	if (problem_layout.LoadFile(problem_layout_file_path_.c_str()) != tinyxml2::XML_SUCCESS)
		return false;

	auto declaration_element = problem_layout.FirstChild()->ToDeclaration();
	auto comment_element = declaration_element->NextSibling()->ToComment();
	auto problem_element = comment_element->NextSiblingElement();
	
	problem_element->QueryIntAttribute("IID", &problem_.m_internal_id);
	problem_element->QueryIntAttribute("TestCount", &problem_.m_test_count);
	problem_element->QueryIntAttribute("BonusPoints", &problem_.m_bonus_points);
	problem_element->QueryDoubleAttribute("TimeLimit", &problem_.m_time_limit);
	problem_element->QueryIntAttribute("MemoryLimit", &problem_.m_memory_limit);

	problem_element->QueryStringAttribute("Gid", &temp_str_buf);
	problem_.m_id = std::string(temp_str_buf);

	problem_element->QueryStringAttribute("Name", &temp_str_buf);
	problem_.m_name = std::string(temp_str_buf);

	problem_element->QueryStringAttribute("CheckerSrc", &temp_str_buf);
	problem_.m_checker_src = std::string(temp_str_buf);

	problem_element->QueryStringAttribute("CheckerExe", &temp_str_buf);
	problem_.m_checker_exe = std::string(temp_str_buf);

	problem_element->QueryStringAttribute("Description", &temp_str_buf);
	problem_.m_description_file = std::string(temp_str_buf);

	problem_element->QueryStringAttribute("InputFile", &temp_str_buf);
	problem_.m_input_file = std::string(temp_str_buf);

	problem_element->QueryStringAttribute("OutputFile", &temp_str_buf);
	problem_.m_output_file = std::string(temp_str_buf);

	int test_index = 0;
	for (auto test_element = problem_element->FirstChildElement(); test_element != NULL;
		test_element = test_element->NextSiblingElement(), test_index++)
	{
		Problem::Test temp_test;

		test_element->QueryIntAttribute("Points", &temp_int_attribute);
		temp_test.points = temp_int_attribute;

		test_element->QueryStringAttribute("Answer", &temp_str_buf);
		temp_test.m_answer_file = std::string(temp_str_buf);

		test_element->QueryStringAttribute("Input", &temp_str_buf);
		temp_test.m_input_file = std::string(temp_str_buf);

		problem_.m_tests.push_back(temp_test);
	}

	return true;
}

int ProblemManager::GetFreeID()
{
	if (m_problem_list.empty()) 
		SearchForProblems();

	int id = 0;
	for (auto& it : m_problem_list)
		if (it.m_internal_id - id > 1)
			return it.m_internal_id - 1;
		else id = it.m_internal_id;

	return id + 1;
}
