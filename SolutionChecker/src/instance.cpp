#include "instance.h"



void Instance::Initialize(const char* file_location_)
{
	std::string file_location(file_location_);
	m_current_dir = file_location.substr(0, file_location.find_last_of('\\', file_location.size()));

	m_options_manager = new OptionsManager(m_current_dir + "\\options.txt");
	m_problem_manager = new ProblemManager(m_options_manager->GetProblemDir());
	m_test_manager =	new TestManager(m_options_manager, m_problem_manager);
	m_gui_ptr =			new Gui(m_options_manager, m_problem_manager, m_test_manager);
}

bool Instance::Run()
{
	return m_gui_ptr->Run();
}

void Instance::Shutdown()
{
	if (m_gui_ptr != nullptr)
	{
		delete m_gui_ptr;
		m_gui_ptr = nullptr;
	}

	if (m_test_manager != nullptr)
	{
		delete m_test_manager;
		m_test_manager = nullptr;
	}

	if (m_problem_manager != nullptr)
	{
		delete m_problem_manager;
		m_problem_manager = nullptr;
	}

	if (m_options_manager != nullptr)
	{
		m_options_manager->UpdateOptionsFile();
		delete m_options_manager;
		m_options_manager = nullptr;
	}
}