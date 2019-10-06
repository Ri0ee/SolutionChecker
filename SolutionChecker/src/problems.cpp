#include "problems.h"



void ProblemManager::Initialize()
{
	if(!m_path.empty()) SearchForProblems();
}

void ProblemManager::SearchForProblems()
{
	m_problem_list.clear();

	for (auto& dir : std::filesystem::directory_iterator(m_path))
	{
		if (!dir.is_directory())
			continue;

		Problem problem;
		problem.m_path = dir.path().string();
		if (!ReadProblem(problem, dir.path().string() + "\\ProblemLayout.xml"))
			continue;
		
		m_problem_list.push_back(problem);
	}
}

void ProblemManager::ShowTaskDescription(int problem_id_)
{
	std::string file_dir = m_problem_list[problem_id_].m_path + "\\" + m_problem_list[problem_id_].m_description_file;
	ShowTextFile(file_dir);
}

void ProblemManager::CreateProblem(Problem& problem_, const std::string& problem_layout_file_)
{
	std::filesystem::path result_path = m_path / std::filesystem::path(std::to_string(problem_.m_id));
	std::filesystem::create_directory(result_path);

	// Creating layout xml file and saving it to newly created dir ProblemsDir/<id>/.
	tinyxml2::XMLDocument problem_layout;

	auto declaration_element = problem_layout.NewDeclaration(nullptr);
	auto format_element = problem_layout.NewComment("Problem exchange format 0.1");

	problem_layout.InsertFirstChild(declaration_element);
	problem_layout.InsertEndChild(format_element);

	auto problem_element = problem_layout.NewElement("problem");
	problem_element->SetValue("Problem");
	problem_element->SetAttribute("Gid", problem_.m_id);
	problem_element->SetAttribute("Name", problem_.m_name.c_str());
	problem_element->SetAttribute("TestCount", problem_.m_test_count);
	problem_element->SetAttribute("BonusPoints", problem_.m_bonus_points);
	problem_element->SetAttribute("CheckerExe", problem_.m_checker_exe.c_str());
	problem_element->SetAttribute("CheckerSrc", problem_.m_checker_src.c_str());
	problem_element->SetAttribute("Description", problem_.m_description_file.c_str());
	problem_element->SetAttribute("TimeLimit", problem_.m_time_limit);
	problem_element->SetAttribute("MemoryLimit", problem_.m_memory_limit);
	problem_element->SetAttribute("InputFile", problem_.m_input_file.c_str());
	problem_element->SetAttribute("OutputFile", problem_.m_output_file.c_str());

	for (auto& test : problem_.m_tests) 
	{
		auto test_element = problem_layout.NewElement("Test");
		test_element->SetValue("Test");

		test_element->SetAttribute("Input", test.m_input_file.c_str());
		test_element->SetAttribute("Answer", test.m_answer_file.c_str());
		test_element->SetAttribute("Points", test.points);

		problem_element->InsertEndChild(test_element);
	}

	problem_layout.InsertEndChild(problem_element);

	problem_layout.SaveFile((result_path.string() + "\\" + problem_layout_file_).c_str());

	std::error_code ec;

	// Copy problem description
	std::filesystem::copy_file(problem_.m_base_path + "\\" + problem_.m_description_file, result_path / problem_.m_description_file, ec);

	// Copy test input and answer files
	for (auto& p_test: problem_.m_tests) {
		std::filesystem::copy_file(problem_.m_base_path + "\\" + p_test.m_answer_file, result_path / p_test.m_answer_file, ec);
		std::filesystem::copy_file(problem_.m_base_path + "\\" + p_test.m_input_file, result_path / p_test.m_input_file, ec);
	}

	// Copy solution src
	std::filesystem::copy_file(problem_.m_base_path + "\\" + problem_.m_solution_src, result_path / problem_.m_solution_src, ec);

	// Copy checker executable and source code
	std::filesystem::copy_file(problem_.m_base_path + "\\" + problem_.m_checker_exe, result_path / problem_.m_checker_exe, ec);
	std::filesystem::copy_file(problem_.m_base_path + "\\" + problem_.m_checker_src, result_path / problem_.m_checker_src, ec);
}

bool ProblemManager::ReadProblem(Problem& problem_, const std::string& problem_layout_file_path_)
{
	const char* temp_str_buf;
	tinyxml2::XMLDocument problem_layout;
	if (problem_layout.LoadFile(problem_layout_file_path_.c_str()) != tinyxml2::XML_SUCCESS)
		return false;

	auto declaration_element = problem_layout.FirstChild()->ToDeclaration();
	auto comment_element = declaration_element->NextSibling()->ToComment();
	if (strcmp(comment_element->Value(), "Problem exchange format 0.1") != 0)
		return false;

	auto problem_element = comment_element->NextSibling()->ToElement();

	problem_element->QueryIntAttribute("Gid", &problem_.m_id);
	problem_element->QueryIntAttribute("TestCount", &problem_.m_test_count);
	problem_element->QueryIntAttribute("BonusPoints", &problem_.m_bonus_points);
	problem_element->QueryDoubleAttribute("TimeLimit", &problem_.m_time_limit);
	problem_element->QueryIntAttribute("MemoryLimit", &problem_.m_memory_limit);

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

	if (problem_.m_test_count == 0)
		return true;

	problem_.m_tests.resize(problem_.m_test_count);
	auto test = problem_element->FirstChild()->ToElement();
	for (int i = 0; i < problem_.m_test_count - 1; i++)
	{
		test->QueryIntAttribute("Points", &problem_.m_tests[i].points);

		test->QueryStringAttribute("Answer", &temp_str_buf);
		problem_.m_tests[i].m_answer_file = std::string(temp_str_buf);

		test->QueryStringAttribute("Input", &temp_str_buf);
		problem_.m_tests[i].m_input_file = std::string(temp_str_buf);

		test = test->NextSibling()->ToElement();
	}

	return true;
}