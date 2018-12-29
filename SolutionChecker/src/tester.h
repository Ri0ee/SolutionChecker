#pragma once

#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <stack>
#include <string>
#include <assert.h>

#include "options.h"
#include "problems.h"

#define TESTING_STATE_OFFLINE	0
#define TESTING_STATE_ONLINE	1
#define TESTING_STATE_ERROR		2 // Internal testing sequence error

#define TEST_STATUS_UNKNOWN			0
#define TEST_STATUS_OK				1 << 0
#define TEST_STATUS_FAIL			1 << 1
#define TEST_STATUS_RUNTIME_ERROR	1 << 2
#define TEST_STATUS_TIME_LIMIT		1 << 3
#define TEST_STATUS_MEMORY_LIMIT	1 << 4



struct Test
{
	unsigned int m_status; // 32bit storage for TEST_STATUS flags
};

struct ErrorMessage
{
	std::string m_location;
	DWORD m_error_id;
};

class TestManager
{
public:
	TestManager() {}
	~TestManager() {}

	bool Initialize(OptionsManager* options_manager_, ProblemManager* problem_manager_);
	void StartTesting(int problem_id_, const std::string& solution_location_, bool all_tests_);
	void FinishTesting();
	void Shutdown();
	void TestingSequence(Problem problem_, const std::string& solution_location_, bool all_tests_);

	void GetResultData(std::vector<Test>& data_) { data_.assign(m_test_list.begin(), m_test_list.end()); }
	int GetTestingStage() { return m_testing_stage.load(); }
	bool GetTestingState() { return m_testing_state; }
	std::string GetErrorMessage() 
	{ 
		if (!m_last_error_stack.empty())
		{
			std::string temp_str_buf;
			DWORD temp_err_id = m_last_error_stack.top().m_error_id;
			std::string location = m_last_error_stack.top().m_location;
			m_last_error_stack.pop();

			LPTSTR error_text = NULL;

			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, 
				temp_err_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&error_text, 0, NULL);

			if (NULL != error_text)
			{
				temp_str_buf = location + ": " + std::string(error_text);
				LocalFree(error_text);
				error_text = NULL;
			}
			return temp_str_buf;
		}
		else return std::string("No errors");
	}

private:
	OptionsManager* m_options_manager = nullptr;
	ProblemManager* m_problem_manager = nullptr;

	std::thread* m_testing_thread = nullptr;

	unsigned int m_testing_state; // 32bit storage for TESTING_STATE flags
	std::vector<Test> m_test_list;
	std::atomic_int m_testing_stage;
	std::stack<ErrorMessage> m_last_error_stack;
};