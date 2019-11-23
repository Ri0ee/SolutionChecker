#pragma once

#include <string>
#include <memory>

#include "gui.h"
#include "options.h"
#include "problems.h"
#include "error_manager.h"



class Instance
{
public:
	Instance(const char* file_location_) { Initialize(file_location_); }

	int Run();

private:
	void Initialize(const char* file_location_);

	std::shared_ptr<OptionsManager> m_options_manager;
	std::shared_ptr<ProblemManager> m_problem_manager;
	std::shared_ptr<TestManager> m_test_manager;
	std::shared_ptr<ErrorManager> m_error_manager;
	std::shared_ptr<GuiManager> m_gui_manager;

	std::string m_current_dir;
};