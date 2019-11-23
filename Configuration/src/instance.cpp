#include "instance.h"



void Instance::Initialize(const char* file_location_)
{
	std::string file_location(file_location_);
	m_current_dir = file_location.substr(0, file_location.find_last_of('\\', file_location.size()));

	options_manager = std::make_shared<OPTIONS::OptionsManager>(m_current_dir + "\\options.txt");
	options_manager->AppPath() = m_current_dir;

	error_manager = std::make_shared<SERROR::ErrorManager>(options_manager);
	problem_manager = std::make_shared<PROBLEMS::ProblemManager>(options_manager, error_manager);
	gui_manager = std::make_shared<GUI::GuiManager>(options_manager, problem_manager, error_manager);
}

int Instance::Run()
{
	return gui_manager->Run();
}