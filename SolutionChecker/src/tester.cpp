#include "tester.h"



bool TestManager::Initialize(OptionsManager* options_manager_, int test_id_)
{

	return true;
}

bool TestManager::Run()
{

	return true;
}

void TestManager::Shutdown()
{

}

void TestManager::GetResultData(std::vector<Test>& data_)
{
	data_.assign(m_test_list.begin(), m_test_list.end());
}