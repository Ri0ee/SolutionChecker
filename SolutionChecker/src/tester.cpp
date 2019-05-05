#include "tester.h"



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
		m_testing_state = TESTING_STATE_FINISHING;
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

bool TestManager::CreateJob(HANDLE& job_handle_, HANDLE& job_port_handle_, long int memory_limit_)
{
	HANDLE job_handle = CreateJobObject(0, "SolutionCheckerJobObject");
	if (job_handle == NULL)
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "CreateJobObject function", 0, 0, Fatal });
		return false;
	}
		
	// Create completion port to link it with job later
	HANDLE job_port_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (job_port_handle == NULL)
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "CreateIoCompletionPort function", 0, 0, Fatal });
		return false;
	}

	// Create job completion port to get notified about limit break later
	JOBOBJECT_ASSOCIATE_COMPLETION_PORT job_port = { 0 };
	job_port.CompletionPort = job_port_handle;
	//job_port.CompletionKey = (PVOID)((UINT_PTR)2);
	job_port.CompletionKey = 0;

	// Set up job object and process limits (memory)
	JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };
	jeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_MEMORY;
	jeli.ProcessMemoryLimit = memory_limit_;

	// Link job object with limit info
	if (SetInformationJobObject(job_handle, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)) != TRUE)
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "SetInformationJobObject function (JobObjectExtendedLimitInformation)", 0, 0, Fatal });
		return false;
	}

	// Link job object with competion port
	if (SetInformationJobObject(job_handle, JobObjectAssociateCompletionPortInformation, &job_port, sizeof(job_port)) != TRUE)
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "SetInformationJobObject function (JobObjectAssociateCompletionPortInformation)", 0, 0, Fatal });
		return false;
	}

	job_handle_ = job_handle;
	job_port_handle_ = job_port_handle;

	return true;
}

void TestManager::TestingSequence(Problem problem_, const std::string& solution_location_, bool all_tests_)
{
	std::error_code err_c;

	std::string working_dir = m_options_manager->WorkingDir() + "\\"; // Directory for testing executable files
	std::string new_executable_dir = working_dir + "solution.exe";
	std::string new_input_file_dir = working_dir + problem_.m_input_file;
	std::string new_output_file_dir = working_dir + problem_.m_output_file;

	std::string temp_solution_file_dir;
	std::string solution_file_type = solution_location_.substr(solution_location_.find_last_of(".") + 1, solution_location_.size());
	bool is_executable_file_temp = false;

	// Compilation if given solution file is not executable
	if (solution_file_type != "exe")
	{
		std::string new_source_file_dir = m_options_manager->TempDir() + "\\" + "solution_source." + solution_file_type;
		std::filesystem::remove(new_source_file_dir, err_c);
		std::filesystem::copy(solution_location_, new_source_file_dir, err_c);
		m_created_file_list.push_back(new_source_file_dir);
		Compiler compiler(m_options_manager, m_error_manager);

		if (solution_file_type == "pas")
			temp_solution_file_dir = compiler.Compile(new_source_file_dir, Pascal);
		else if (solution_file_type == "cpp")
			temp_solution_file_dir = compiler.Compile(new_source_file_dir, Cpp);
		else if (solution_file_type == "c")
			temp_solution_file_dir = compiler.Compile(new_source_file_dir, C);
		else if (solution_file_type == "java")
			temp_solution_file_dir = compiler.Compile(new_source_file_dir, Java);
		else
			m_error_manager->PushError({ "Unknown file type", "Testing", 0, 0, Fatal });

		// Failed to compile for some reason
		if (temp_solution_file_dir.empty())
			m_error_manager->PushError({ "Compilation failed", "Testing", 0, 0, Fatal });

		is_executable_file_temp = true;

		// Remove created temporary source file
		std::filesystem::remove(new_source_file_dir);
	}
	else temp_solution_file_dir = solution_location_;

	// Copy solution file to working directory
	std::filesystem::copy(temp_solution_file_dir, new_executable_dir, err_c);
	m_created_file_list.push_back(new_executable_dir);

	// Remove executable file if it was created as a temporary file
	if (is_executable_file_temp) 
		std::filesystem::remove(temp_solution_file_dir);

	for (int current_test = 0; current_test < problem_.m_test_count; current_test++)
	{
		// Update testing stage
		m_testing_stage.store(current_test);

		Test test;
		test.m_status = 0;
		test.m_id = current_test;

		// If executable file does not exist
		if (!std::filesystem::exists(new_executable_dir))
		{
			test.m_status |= TEST_STATUS_TESTING_SEQUENCE_ERROR;
			m_test_list.push_back(test);
			continue;
		}

		// Create job object to limit process memory usage later
		HANDLE job_handle, job_port_handle;
		if (!CreateJob(job_handle, job_port_handle, min(m_options_manager->TestMemoryLimit() * 1024 * 1024, problem_.m_memory_limit * 1024 * 1024)))
		{
			test.m_status |= TEST_STATUS_TESTING_SEQUENCE_ERROR;
			m_test_list.push_back(test);
			continue;
		}

		std::string input_file_dir = problem_.m_path + "\\" + std::to_string(current_test) + ".in";
		std::string correct_output_file_dir = problem_.m_path + "\\" + std::to_string(current_test) + ".out";

		// Copy input file to working directory
		std::filesystem::copy(input_file_dir, new_input_file_dir, err_c);

		STARTUPINFO sui = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		sui.cb = sizeof(STARTUPINFO);

		// Execute solution file
		if (CreateProcessA(new_executable_dir.c_str(), 0, 0, 0, false, CREATE_NO_WINDOW, 0, working_dir.c_str(), &sui, &pi) == TRUE)
		{
			AssignProcessToJobObject(job_handle, pi.hProcess);

			// Wait until process terminates itself
			WaitForSingleObject(pi.hProcess, (DWORD)(500 + problem_.m_time_limit * 1000));

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
					m_error_manager->PushError({ GetErrorMessage(GetLastError()), "GetQueuedCompletionStatus function", 0, 0, Fatal });

				if (completion_code == JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT)
					test.m_status |= TEST_STATUS_MEMORY_LIMIT;

				if (completion_code == JOB_OBJECT_MSG_NEW_PROCESS)
					flag = false;
			}

			JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };
			QueryInformationJobObject(job_handle, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli), NULL);
			test.m_peak_memory_used = jeli.PeakProcessMemoryUsed;

			CloseHandle(job_port_handle);
			CloseHandle(job_handle);

			GetExitCodeProcess(pi.hProcess, &test.m_exit_code);
			if (test.m_exit_code != 0 && !(test.m_status & TEST_STATUS_MEMORY_LIMIT))
				test.m_status |= TEST_STATUS_RUNTIME_ERROR;

			FILETIME start_time = { 0 }, end_time = { 0 }, kernel_time = { 0 }, user_time = { 0 };
			GetProcessTimes(pi.hProcess, &start_time, &end_time, &kernel_time, &user_time);

			SYSTEMTIME s_start_time, s_end_time;
			FileTimeToSystemTime(&start_time, &s_start_time);
			FileTimeToSystemTime(&end_time, &s_end_time);

			long long int diff_time = (s_end_time.wSecond * 1000 + s_end_time.wMilliseconds) - (s_start_time.wSecond * 1000 + s_start_time.wMilliseconds);
			if ((double)diff_time > problem_.m_time_limit * 1000)
				test.m_status |= TEST_STATUS_TIME_LIMIT;

			test.m_run_time = diff_time;

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		} 
		else // Unable to create process
		{
			m_error_manager->PushError({ GetErrorMessage(GetLastError()), "CreateProcess function", 0, 0, Fatal });
			test.m_status |= TEST_STATUS_TESTING_SEQUENCE_ERROR;
		}

		std::fstream input_file(new_input_file_dir);
		std::fstream result_output_file(new_output_file_dir);
		std::fstream correct_output_file(correct_output_file_dir);

		// Load input data into test structure
		if (input_file.is_open())
		{
			test.m_input_data = std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
			input_file.close();
		}

		// Check if result is correct
		if (result_output_file.is_open() && correct_output_file.is_open())
		{
			test.m_output_data = std::string((std::istreambuf_iterator<char>(result_output_file)), std::istreambuf_iterator<char>());
			test.m_destination_data = std::string((std::istreambuf_iterator<char>(correct_output_file)), std::istreambuf_iterator<char>());

			if (test.m_output_data == test.m_destination_data)
				test.m_status |= TEST_STATUS_OK;
			else
				test.m_status |= TEST_STATUS_FAIL;

			result_output_file.close();
			correct_output_file.close();
		} 
		else // Unable to open result file or correct output file
			m_error_manager->PushError({ "Unable to open result files", "Result checking", 0, 0, Fatal });
			
		m_test_list.push_back(test);

		// Delete output file in working directory
		std::filesystem::remove(new_output_file_dir);

		// Delete input file in working directory
		std::filesystem::remove(new_input_file_dir);

		if (m_testing_state == TESTING_STATE_FINISHING)
		{
			ShutdownTestingSequence();
			return;
		}

		// If checkbox "use one test only" is checked
		if (all_tests_ == false) break;
	}
	
	// Delete executable file in working directory
	std::filesystem::remove(new_executable_dir);
}

void TestManager::ShutdownTestingSequence()
{
	for (auto file : m_created_file_list)
		std::filesystem::remove(file);

	m_testing_state = TESTING_STATE_OFFLINE;
}