#pragma once

#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <stack>
#include <string>
#include <filesystem>

#include "options.h"
#include "problems.h"
#include "utils.h"
#include "compiler.h"
#include "error_manager.h"

#define TESTING_STATE_OFFLINE	0
#define TESTING_STATE_ONLINE	1
#define TESTING_STATE_FINISHING	2

#define TEST_STATUS_UNKNOWN			0
#define TEST_STATUS_OK				1 << 0
#define TEST_STATUS_FAIL			1 << 1
#define TEST_STATUS_RUNTIME_ERROR	1 << 2
#define TEST_STATUS_TIME_LIMIT		1 << 3
#define TEST_STATUS_MEMORY_LIMIT	1 << 4
#define TEST_STATUS_TESTING_SEQUENCE_ERROR	1 << 5 



struct Test
{
	unsigned int m_status = 0; // 32bit storage for TEST_STATUS flags
	long long int m_run_time = 0;
	DWORD m_exit_code = -1;
	int m_peak_memory_used = 0;
	int m_id = -1;
	std::string m_input_data;
	std::string m_output_data;
	std::string m_destination_data;
};

class TestManager
{
public:
	TestManager(OptionsManager* options_manager_, ProblemManager* problem_manager_, ErrorManager* error_manager_) :
		m_options_manager(options_manager_), m_problem_manager(problem_manager_), m_error_manager(error_manager_) {}
	TestManager() {}
	~TestManager() { Shutdown(); }

	void StartTesting(int problem_id_, const std::string& solution_location_, bool all_tests_);
	void FinishTesting();
	void TestingSequence(Problem problem_, const std::string& solution_location_, bool all_tests_);

	void GetResultData(std::vector<Test>& data_) { data_.assign(m_test_list.begin(), m_test_list.end()); }
	int GetTestingStage() { return m_testing_stage.load(); }
	bool GetTestingState() { return m_testing_state; }

private:
	void Shutdown();
	void ShutdownTestingSequence();
	bool CreateJob(HANDLE& job_handle_, HANDLE& job_port_handle_, long int memory_limit_);
	std::string SelectCompilerAndCompile(const std::string& solution_location_);
	long long GetExecutionTime(PROCESS_INFORMATION& pi_);

	OptionsManager* m_options_manager = nullptr;
	ProblemManager* m_problem_manager = nullptr;
	ErrorManager*	m_error_manager = nullptr;

	std::thread* m_testing_thread = nullptr;

	unsigned int m_testing_state = TESTING_STATE_OFFLINE; 
	std::vector<Test> m_test_list;
	std::atomic_int m_testing_stage;
	std::vector<std::string> m_created_file_list;
};