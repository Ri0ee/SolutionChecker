#include "tester.h"



void TestManager::Initialize(OptionsManager* options_manager_, ProblemManager* problem_manager_)
{
	m_options_manager = options_manager_;
	m_problem_manager = problem_manager_;
}

void TestManager::StartTesting(int problem_id_, const std::string& solution_location_, bool all_tests_)
{
	Problem problem = m_problem_manager->GetProblem(problem_id_);

	m_test_list.clear();
	m_testing_state = TESTING_STATE_ONLINE;
	m_testing_stage.store(0);

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
		m_testing_stage.store(0);
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
	std::string working_dir = m_options_manager->WorkingDir() + "\\";
	std::string new_executable_dir = working_dir + "solution.exe";
	std::string new_input_file_dir = working_dir + problem_.m_input_file_name;
	std::string new_output_file_dir = working_dir + problem_.m_output_file_name;

	// Copy solution file to working directory
	if (CopyFile(solution_location_.c_str(), new_executable_dir.c_str(), false) != TRUE)
	{
		m_testing_state = TESTING_STATE_ERROR;
		m_last_error_stack.push({ "CopyFile function (solution executable file)", GetLastError() });
	}

	for (int current_test = 0; current_test < problem_.m_test_count; current_test++)
	{
		Test test;
		test.m_status = 0;
		test.m_id = current_test;

		std::string input_file_dir = problem_.m_path + "\\" + std::to_string(current_test) + ".in";
		std::string correct_output_file_dir = problem_.m_path + "\\" + std::to_string(current_test) + ".out";

		// Update testing stage
		m_testing_stage.store(current_test);

		// Copy input file to working directory
		if (CopyFile(input_file_dir.c_str(), new_input_file_dir.c_str(), false) != TRUE)
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "CopyFile function (input file)", GetLastError() });
		}

		// Create job object to limit process memory usage later
		std::string job_name = "SolutionCheckerJobObject";
		HANDLE job_handle = CreateJobObject(0, job_name.c_str());
		if (job_handle == NULL)
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "CreateJobObject function", GetLastError() });
		}

		// Create completion port to link it with job later
		HANDLE job_port_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if (job_port_handle == NULL)
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "CreateIoCompletionPort function", GetLastError() });
		}

		// Create job completion port to get notified about limit break later
		JOBOBJECT_ASSOCIATE_COMPLETION_PORT job_port = { 0 };
		job_port.CompletionPort = job_port_handle;
		job_port.CompletionKey = (PVOID)((UINT_PTR)2);

		// Set up job object and process limits (memory)
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };
		jeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_MEMORY;
		jeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
		if (problem_.m_memory_limit > m_options_manager->TestMemoryLimit())
		{
			jeli.JobMemoryLimit = m_options_manager->TestMemoryLimit() * 1024 * 1024;
			jeli.ProcessMemoryLimit = m_options_manager->TestMemoryLimit() * 1024 * 1024;
		}
		else
		{
			jeli.JobMemoryLimit = problem_.m_memory_limit * 1024 * 1024;
			jeli.ProcessMemoryLimit = problem_.m_memory_limit * 1024 * 1024;
		}

		// Link job object with limit info
		if (SetInformationJobObject(job_handle, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)) != TRUE)
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "SetInformationJobObject function (JobObjectExtendedLimitInformation)", GetLastError() });
		}

		// Link job object with competion port
		if (SetInformationJobObject(job_handle, JobObjectAssociateCompletionPortInformation, &job_port, sizeof(job_port)) != TRUE)
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "SetInformationJobObject function (JobObjectAssociateCompletionPortInformation)", GetLastError() });
		}

		STARTUPINFO sui;
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
		ZeroMemory(&sui, sizeof(STARTUPINFO));
		sui.cb = sizeof(STARTUPINFO);

		// Execute solution file
		if (CreateProcess(new_executable_dir.c_str(), 0, 0, 0, false, CREATE_NO_WINDOW, 0, working_dir.c_str(), &sui, &pi) == TRUE)
		{
			AssignProcessToJobObject(job_handle, pi.hProcess);

			// Wait until process terminates itself
			WaitForSingleObject(pi.hProcess, (DWORD)(1000 + problem_.m_time_limit * 1000));

			// Initialize process termination
			TerminateProcess(pi.hProcess, 0);

			// Wait for process to terminate, if not terminated already
			WaitForSingleObject(pi.hProcess, INFINITE);

			DWORD completion_code = 0;
			ULONG_PTR completion_key = 0;
			LPOVERLAPPED overlapped;

			// Check memory limit break
			bool flag = false;
			while (!flag)
			{
				flag = true;
				if (GetQueuedCompletionStatus(job_port_handle, &completion_code, &completion_key, &overlapped, 0) != TRUE)
				{
					m_testing_state = TESTING_STATE_ERROR;
					m_last_error_stack.push({ "GetQueuedCompletionStatus function", GetLastError() });
				}

				if (completion_code == JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT || completion_code == JOB_OBJECT_MSG_JOB_MEMORY_LIMIT)
					test.m_status |= TEST_STATUS_MEMORY_LIMIT;

				if (completion_code == JOB_OBJECT_MSG_NEW_PROCESS)
					flag = false;
			}

			JOBOBJECT_EXTENDED_LIMIT_INFORMATION temp_jeli = { 0 };
			QueryInformationJobObject(job_handle, JobObjectExtendedLimitInformation, &temp_jeli, sizeof(temp_jeli), NULL);
			test.m_peak_memory_used = temp_jeli.PeakProcessMemoryUsed;

			CloseHandle(job_port_handle);
			CloseHandle(job_handle);

			GetExitCodeProcess(pi.hProcess, &test.m_exit_code);
			if (test.m_exit_code != 0 && !(test.m_status & TEST_STATUS_MEMORY_LIMIT))
				test.m_status |= TEST_STATUS_RUNTIME_ERROR;

			FILETIME start_time = { 0 };
			FILETIME end_time = { 0 };
			FILETIME kernel_time = { 0 };
			FILETIME user_time = { 0 };
			if (GetProcessTimes(pi.hProcess, &start_time, &end_time, &kernel_time, &user_time) == TRUE)
			{
				SYSTEMTIME s_start_time;
				SYSTEMTIME s_end_time;
				bool convert_success = true;

				if (FileTimeToSystemTime(&start_time, &s_start_time) != TRUE)
				{
					m_testing_state = TESTING_STATE_ERROR;
					m_last_error_stack.push({ "FileTimeToSystemTime function", GetLastError() });
					convert_success = false;
				}

				if (FileTimeToSystemTime(&end_time, &s_end_time) != TRUE)
				{
					m_testing_state = TESTING_STATE_ERROR;
					m_last_error_stack.push({ "FileTimeToSystemTime function", GetLastError() });
					convert_success = false;
				}

				if (convert_success)
				{
					long long int diff_time = (s_end_time.wSecond * 1000 + s_end_time.wMilliseconds) - (s_start_time.wSecond * 1000 + s_start_time.wMilliseconds);
					if ((float)diff_time > problem_.m_time_limit * 1000)
						test.m_status |= TEST_STATUS_TIME_LIMIT;

					test.m_run_time = diff_time;
				}
			}
			else
			{
				m_testing_state = TESTING_STATE_ERROR;
				m_last_error_stack.push({ "GetProcessTimes function", GetLastError() });
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		} 
		else // Unable to create process
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "CreateProcess function", GetLastError() });
		}

		// Check if result is correct
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

		// Delete output file in working directory
		if (DeleteFileA(new_output_file_dir.c_str()) != TRUE)
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "DeleteFileA function (output file)", GetLastError() });
		}

		// Delete input file in working directory
		if (DeleteFileA(new_input_file_dir.c_str()) != TRUE)
		{
			m_testing_state = TESTING_STATE_ERROR;
			m_last_error_stack.push({ "DeleteFileA function (input file)", GetLastError() });
		}

		// If checkbox "use one test only" is checked
		if (all_tests_ == false) break;
	}
	
	// Delete executable file in working directory
	if (DeleteFileA(new_executable_dir.c_str()) != TRUE)
	{
		m_testing_state = TESTING_STATE_ERROR;
		m_last_error_stack.push({ "DeleteFileA function (solution executable file)", GetLastError() });
	}
}