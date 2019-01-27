#pragma once

#include <string>

#include "gui.h"
#include "options.h"
#include "problems.h"



class Instance
{
public:
	Instance(const char* file_location_) { Initialize(file_location_); }
	Instance() {}
	~Instance() { Shutdown(); }

	bool Run();

private:
	void Initialize(const char* file_location_);
	void Shutdown();

	Gui* m_gui_ptr = nullptr;
	OptionsManager* m_options_manager = nullptr;
	ProblemManager* m_problem_manager = nullptr;
	TestManager* m_test_manager = nullptr;

	std::string m_current_dir;
};