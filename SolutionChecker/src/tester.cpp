#include "tester.h"



bool TestManager::Initialize(OptionsManager* options_manager_, ProblemManager* problem_manager_)
{
	m_options_manager = options_manager_;
	m_problem_manager = problem_manager_;

	return true;
}

void TestManager::StartTesting(int problem_id_, const std::string& solution_location_, bool all_tests_)
{
	Problem problem = m_problem_manager->GetProblem(problem_id_);

	m_test_list.clear();
	m_testing_state = TESTING_STATE_ONLINE;

	m_testing_thread = new std::thread(&TestManager::TestingSequence, this, problem, solution_location_, all_tests_);
}

void TestManager::FinishTesting()
{
	if (m_testing_thread != nullptr)
	{
		m_testing_thread->join();
		delete m_testing_thread;
		m_testing_thread = nullptr;
		m_testing_state = TESTING_STATE_OFFLINE;
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

void TestManager::TestingSequence(Problem problem_, const std::string& solution_location_, bool all_tests_)
{
	std::string working_dir = m_options_manager->GetWorkingDir() + "\\";
	std::string new_executable_dir = working_dir + "solution.exe";
	std::string new_input_file_dir = working_dir + problem_.m_input_file_name;
	std::string new_output_file_dir = working_dir + problem_.m_output_file_name;

	if (CopyFile(solution_location_.c_str(), new_executable_dir.c_str(), false) != TRUE)
	{
		m_testing_state = TESTING_STATE_ERROR;
		m_last_error_stack.push({ "Executable copy", GetLastError() });
	}

	for (int current_test = 0; current_test < problem_.m_test_count; current_test++)
	{
		Test test;
		test.m_status = 0;

		std::string input_file_dir = problem_.m_path + "\\" + std::to_string(current_test) + ".in";
		std::string correct_output_file_dir = problem_.m_path + "\\" + std::to_string(current_test) + ".out";

		m_testing_stage.store(current_test);

		if (CopyFile(input_file_dir.c_str(), new_input_file_dir.c_str(), false) != TRUE)
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "Input file copy", GetLastError() });
		}

		STARTUPINFO sui;
		ZeroMemory(&sui, sizeof(STARTUPINFO));
		sui.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

		if (CreateProcess(new_executable_dir.c_str(), 0, 0, 0, false, CREATE_NO_WINDOW, 0, working_dir.c_str(), &sui, &pi) == TRUE)
		{
			WaitForSingleObject(pi.hProcess, 1000 + problem_.m_time_limit * 1000);
			TerminateProcess(pi.hProcess, 0);
			WaitForSingleObject(pi.hProcess, INFINITE);

			FILETIME start_time;
			FILETIME end_time;
			FILETIME kernel_time;
			FILETIME user_time;
			GetProcessTimes(pi.hProcess, &start_time, &end_time, &kernel_time, &user_time);

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			SYSTEMTIME s_start_time;
			SYSTEMTIME s_end_time;
			FileTimeToSystemTime(&start_time, &s_start_time);
			FileTimeToSystemTime(&end_time, &s_end_time);

			int diff_time = (s_end_time.wSecond * 1000 + s_end_time.wMilliseconds) - (s_start_time.wSecond * 1000 + s_start_time.wMilliseconds);
			if ((float)diff_time > problem_.m_time_limit * 1000)
				test.m_status |= TEST_STATUS_TIME_LIMIT;
		} 
		else // Unable to create process
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "Process creation", GetLastError() });
		}

		std::fstream result_output_file(new_output_file_dir);
		std::fstream correct_output_file(correct_output_file_dir);
		if (result_output_file.is_open() && correct_output_file.is_open())
		{
			std::string result_data((std::istreambuf_iterator<char>(result_output_file)), std::istreambuf_iterator<char>());
			std::string correct_data((std::istreambuf_iterator<char>(correct_output_file)), std::istreambuf_iterator<char>());

			if (result_data == correct_data)
				test.m_status |= TEST_STATUS_OK;
			else
				test.m_status |= TEST_STATUS_FAIL;

			result_output_file.close();
			correct_output_file.close();
		} 
		else // Unable to open result file or correct output file
		{
			m_testing_state = TESTING_STATE_ERROR;
		}

		m_test_list.push_back(test);

		if (DeleteFileA(new_output_file_dir.c_str()) != TRUE)
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "Output file deletion", GetLastError() });
		}

		if (DeleteFileA(new_input_file_dir.c_str()) != TRUE)
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "Input file deletion", GetLastError() });
		}

		if (all_tests_ == false) break;
	}
	
	if (DeleteFileA(new_executable_dir.c_str()) != TRUE)
	{
		m_testing_state = TESTING_STATE_ERROR;
		m_last_error_stack.push({ "Executable file deletion", GetLastError() });
	}
}