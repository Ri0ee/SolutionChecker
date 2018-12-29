#pragma once

#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <string>

#include "options.h"
#include "problems.h"



struct Test
{
	int m_status;
};

class TestManager
{
public:
	TestManager() {}
	~TestManager() {}

	bool Initialize(OptionsManager* options_manager_, ProblemManager* problem_manager_);
	void StartTesting(int problem_id_, const std::string& solution_location_);
	void FinishTesting();
	void Shutdown();
	void TestingSequence(Problem problem_, const std::string& solution_location_);

	void GetResultData(std::vector<Test>& data_) { data_.assign(m_test_list.begin(), m_test_list.end()); }
	int GetTestingStage() { return m_testing_stage.load(); }
	bool GetTestingState() { return m_testing_state; }

private:
	OptionsManager* m_options_manager = nullptr;
	ProblemManager* m_problem_manager = nullptr;

	std::thread* m_testing_thread = nullptr;

	bool m_testing_state;
	std::vector<Test> m_test_list;
	std::atomic_int m_testing_stage;
};