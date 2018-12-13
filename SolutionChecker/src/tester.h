#pragma once

#include <vector>

#include "options.h"



struct Test
{
	int m_status;
	int m_id;

};

class TestManager
{
public:
	TestManager() {}
	~TestManager() {}

	bool Initialize(OptionsManager* options_manager_, int test_id_);
	bool Run();
	void Shutdown();

	void GetResultData(std::vector<Test>& data_);

private:
	std::vector<Test> m_test_list;

};