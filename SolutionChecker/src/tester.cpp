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
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "CreateJobObject function", 0, 0, Severity::Fatal });
		return false;
	}
		
	// Create completion port to link it with job later
	HANDLE job_port_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (job_port_handle == NULL)
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "CreateIoCompletionPort function", 0, 0, Severity::Fatal });
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
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "SetInformationJobObject function (JobObjectExtendedLimitInformation)", 0, 0, Severity::Fatal });
		return false;
	}

	// Link job object with competion port
	if (SetInformationJobObject(job_handle, JobObjectAssociateCompletionPortInformation, &job_port, sizeof(job_port)) != TRUE)
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "SetInformationJobObject function (JobObjectAssociateCompletionPortInformation)", 0, 0, Severity::Fatal });
		return false;
	}	

	job_handle_ = job_handle;
	job_port_handle_ = job_port_handle;

	return true;
}

std::optional<std::string> TestManager::SelectCompilerAndCompile(const std::string& solution_location_) 
{
	std::error_code err_c;
	std::string solution_file_type = solution_location_.substr(solution_location_.find_last_of(".") + 1);
	std::string new_source_file_dir = m_options_manager->GetOption("TempDir") + "\\" + "ss." + solution_file_type;

	std::filesystem::remove(new_source_file_dir, err_c);
	std::filesystem::copy(solution_location_, new_source_file_dir, err_c);
	m_created_file_list.push_back(new_source_file_dir);

	Compiler compiler(m_options_manager, m_error_manager);
	auto solution_file_dir = compiler.Compile(new_source_file_dir);

	// Failed to compile for some reason
	if (!solution_file_dir.has_value())
		m_error_manager->PushError({ "Compilation failed", "Testing", 0, 0, Severity::Fatal });

	return solution_file_dir;
}

long long TestManager::GetExecutionTime(PROCESS_INFORMATION& pi_) 
{
	FILETIME start_time = { 0 }, end_time = { 0 }, kernel_time = { 0 }, user_time = { 0 };
	GetProcessTimes(pi_.hProcess, &start_time, &end_time, &kernel_time, &user_time);

	SYSTEMTIME s_start_time, s_end_time, s_user_time;
	FileTimeToSystemTime(&start_time, &s_start_time);
	FileTimeToSystemTime(&end_time, &s_end_time);
	FileTimeToSystemTime(&user_time, &s_user_time);

	//long long int diff_time = (s_end_time.wSecond * (long long int)1000 + s_end_time.wMilliseconds) - (s_start_time.wSecond * (long long int)1000 + s_start_time.wMilliseconds);
	long long int time = 1000 * s_user_time.wSecond + s_user_time.wMilliseconds;

	return time;
}

void TestManager::TestingSequence(Problem problem_, const std::string& solution_location_, bool all_tests_)
{
	std::error_code err_c;

	std::string solution_file_type = solution_location_.substr(solution_location_.find_last_of(".") + 1);
	std::string solution_file_name = solution_location_.substr(solution_location_.find_last_of("\\") + 1);
	bool is_java = (solution_file_type == "java");

	std::string working_dir = m_options_manager->GetOption("WorkingDir") + "\\"; // Directory for testing executable files
	std::string new_executable_dir = working_dir + (is_java ? solution_file_name : "solution.exe");
	std::string new_input_file_dir = working_dir + problem_.m_input_file;
	std::string new_output_file_dir = working_dir + problem_.m_output_file;

	auto temp_solution_file_dir = SelectCompilerAndCompile(solution_location_);
	if (!temp_solution_file_dir.has_value())
	{
		m_error_manager->PushError({ GetErrorMessage(GetLastError()), "Compilation routine", 0, 0, Severity::Fatal });
		return;
	}

	// Copy solution file to working directory
	std::filesystem::copy(temp_solution_file_dir.value(), new_executable_dir, err_c);
	m_created_file_list.push_back(new_executable_dir);
	
	// Remove executable file if it was created as a temporary file
	if (temp_solution_file_dir.value() != solution_location_) 
		std::filesystem::remove(temp_solution_file_dir.value());

	for (int current_test = 0; current_test < problem_.m_test_count; current_test++)
	{
		m_testing_stage.store(current_test); // Update testing stage
		Test test = { 0, 0, (DWORD)-1, 0, current_test, "", "", "", problem_.m_tests[current_test].points, problem_.m_bonus_points };

		// If executable file does not exist
		if (!std::filesystem::exists(new_executable_dir))
		{
			test.m_status |= TEST_STATUS_TESTING_SEQUENCE_ERROR;
			m_test_list.push_back(test);
			continue;
		}

		// Create job object to limit process memory usage later
		HANDLE job_handle, job_port_handle;
		if (!CreateJob(job_handle, job_port_handle, min(std::stoi(m_options_manager->GetOption("TestMemoryLimit")) * 1024 * 1024, problem_.m_memory_limit * 1024 * 1024)))
		{
			test.m_status |= TEST_STATUS_TESTING_SEQUENCE_ERROR;
			m_test_list.push_back(test);
			continue;
		}

		std::string input_file_dir = problem_.m_path + "\\" + problem_.m_tests[current_test].m_input_file;
		std::string correct_output_file_dir = problem_.m_path + "\\" + problem_.m_tests[current_test].m_answer_file;

		// Copy input file to working directory
		std::filesystem::copy(input_file_dir, new_input_file_dir, err_c);

		STARTUPINFO sui = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		sui.cb = sizeof(STARTUPINFO);

		std::string executable = (is_java) ? m_options_manager->GetOption("JavaVMPath") : new_executable_dir;
		std::string command_line = (is_java) ? "\"" + m_options_manager->GetOption("JavaVMPath") + "\" " + "\"" + new_executable_dir + "\"": "";

		LPSTR command_line_lpstr = new char[command_line.size()];
		strcpy_s(command_line_lpstr, command_line.size() + 1, command_line.c_str());

		// Execute solution file
		if (CreateProcessA(	executable.c_str(),
							command_line_lpstr,
							0,
							0,
							false,
							CREATE_NO_WINDOW, 
							0,
							working_dir.c_str(), 
							&sui, 
							&pi))
		{
			if (!AssignProcessToJobObject(job_handle, pi.hProcess))
				m_error_manager->PushError({ GetErrorMessage(GetLastError()), "AssignProcessToJobObject function", 0, 0, Severity::Fatal });

			// Wait until process terminates itself
			WaitForSingleObject(pi.hProcess, (DWORD)(problem_.m_time_limit * 1000) + 200);

			// Initialize process termination
			if (!TerminateProcess(pi.hProcess, 0))
				m_error_manager->PushError({ GetErrorMessage(GetLastError()), "TerminateProcess function", 0, 0, Severity::Fatal });

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
					m_error_manager->PushError({ GetErrorMessage(GetLastError()), "GetQueuedCompletionStatus function", 0, 0, Severity::Fatal });

				if (completion_code == JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT)
					test.m_status |= TEST_STATUS_MEMORY_LIMIT | TEST_STATUS_FAIL;

				if (completion_code == JOB_OBJECT_MSG_NEW_PROCESS)
					flag = false;
			}

			JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };
			if(!QueryInformationJobObject(job_handle, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli), NULL))
				m_error_manager->PushError({ GetErrorMessage(GetLastError()), "QueryInformationJobObject function", 0, 0, Severity::Fatal });
			test.m_peak_memory_used = (int)jeli.PeakProcessMemoryUsed;

			CloseHandle(job_port_handle);
			CloseHandle(job_handle);

			GetExitCodeProcess(pi.hProcess, &test.m_exit_code);
			if (test.m_exit_code != 0 && !(test.m_status & TEST_STATUS_MEMORY_LIMIT))
				test.m_status |= TEST_STATUS_RUNTIME_ERROR | TEST_STATUS_FAIL;

			test.m_run_time = GetExecutionTime(pi);

			if (test.m_run_time > problem_.m_time_limit * 1000)
				test.m_status |= TEST_STATUS_TIME_LIMIT | TEST_STATUS_FAIL;

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		} 
		else // Unable to create process
		{
			m_error_manager->PushError({ GetErrorMessage(GetLastError()), "CreateProcess function", 0, 0, Severity::Fatal });
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
			result_output_file.close();
			correct_output_file.close();

			std::stringstream sst_ans(test.m_output_data), sst_out(test.m_destination_data);
			std::string answer_data_piece;

			bool correctness_flag = true;
			while (sst_ans >> answer_data_piece) {
				std::string output_data_piece;
				sst_out >> output_data_piece;

				if (answer_data_piece != output_data_piece)
				{
					correctness_flag = false;
					break;
				}
			}

			if(correctness_flag == true && !(test.m_status & TEST_STATUS_FAIL))
				test.m_status |= TEST_STATUS_OK;
			else 
				test.m_status |= TEST_STATUS_FAIL;
		} 
		else // Unable to open result file or correct output file
			m_error_manager->PushError({ "Unable to open result files", "Result checking", 0, 0, Severity::Fatal });
			
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