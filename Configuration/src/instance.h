#pragma once

#include <string>
#include <memory>

#include "gui.h"
#include "options.h"



class Instance
{
public:
	Instance(const char* file_location_) { Initialize(file_location_); }

	int Run();

private:
	void Initialize(const char* file_location_);

	std::shared_ptr<OPTIONS::OptionsManager> options_manager;
	std::shared_ptr<GUI::GuiManager> gui_manager;
	std::shared_ptr<PROBLEMS::ProblemManager> problem_manager;
	std::shared_ptr<SERROR::ErrorManager> error_manager;

	std::string m_current_dir;
};