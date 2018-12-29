#include "tester.h"



bool TestManager::Initialize(OptionsManager* options_manager_, ProblemManager* problem_manager_)
{
	m_options_manager = options_manager_;
	m_problem_manager = problem_manager_;

	return true;
}

void TestManager::StartTesting(int problem_id_, const std::string& solution_location_)
{
	Problem problem = m_problem_manager->GetProblem(problem_id_);

	m_test_list.clear();
	m_testing_state = true;

	m_testing_thread = new std::thread(&TestManager::TestingSequence, this, problem, solution_location_);
}

void TestManager::FinishTesting()
{
	if (m_testing_thread != nullptr)
	{
		m_testing_thread->join();
		delete m_testing_thread;
		m_testing_thread = nullptr;
		m_testing_state = false;
	}
}

void TestManager::Shutdown()
{
	if (m_testing_thread != nullptr)
	{
		delete m_testing_thread;
		m_testing_thread = nullptr;
	}
}

void TestManager::TestingSequence(Problem problem_, const std::string& solution_location_)
{
	// Copy solution executable into working dir
	CopyFile(solution_location_.c_str(), (m_options_manager->GetWorkingDir() + "\\solution.exe").c_str(), false);

	for (int current_test = 0; current_test < problem_.m_test_count; current_test++)
	{
		m_testing_stage.store(current_test);

		CopyFile((problem_.m_path + "\\" + std::to_string(current_test) + ".in").c_str(), 
			(m_options_manager->GetWorkingDir() + "\\" + problem_.m_input_file_name).c_str(), false);

		STARTUPINFO sui;
		ZeroMemory(&sui, sizeof(STARTUPINFO));
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

		if (CreateProcess((m_options_manager->GetWorkingDir() + "\\solution.exe").c_str(), 0, 0, 0, true, CREATE_NO_WINDOW, 0,
			(m_options_manager->GetWorkingDir() + "\\").c_str(), &sui, &pi) == TRUE)
		{
			
		} 
		else
		{
			// error
		}

		// check resulting file comparing it with correct data
		// delete files other than solution executable

		Test test;
		m_test_list.push_back(test);
	}
	// Clear every file in working dir
}