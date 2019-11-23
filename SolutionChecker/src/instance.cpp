#include "instance.h"



void Instance::Initialize(const char* file_location_)
{
	std::string file_location(file_location_);
	m_current_dir = file_location.substr(0, file_location.find_last_of('\\', file_location.size()));

	m_options_manager = std::make_shared<OptionsManager>(m_current_dir + "\\options.txt");
	m_options_manager->AppPath() = m_current_dir;

	m_error_manager = std::make_shared<ErrorManager>(m_options_manager);
	m_problem_manager = std::make_shared<ProblemManager>(m_options_manager, m_error_manager);
	m_test_manager = std::make_shared<TestManager>(m_options_manager, m_problem_manager, m_error_manager);
	m_gui_manager = std::make_shared<GuiManager>(m_options_manager, m_problem_manager, m_test_manager, m_error_manager);
}

int Instance::Run()
{
	return m_gui_manager->Run();
}