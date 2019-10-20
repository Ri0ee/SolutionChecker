#pragma once

#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Round_Button.H"
#include "FL/Fl_Input.H"
#include "FL/Fl_Choice.H"
#include "FL/Fl_Progress.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Scroll.H"
#include "FL/Fl_Hold_Browser.H"
#include "FL/fl_ask.H"
#include "FL/Fl_Value_Input.H"
#include "FL/Fl_Box.H"

#include <shlobj.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

#include "options.h"
#include "tester.h"
#include "problems.h"
#include "compiler.h"
#include "settings_menu.h"
#include "output_menu.h"
#include "error_manager.h"



class Gui
{
public:
	Gui(std::shared_ptr<OptionsManager> options_manager_, std::shared_ptr<ProblemManager> problem_manager_, 
		std::shared_ptr<TestManager> test_manager_, std::shared_ptr<ErrorManager> error_manager_) :
		m_options_manager(options_manager_), m_problem_manager(problem_manager_), 
		m_test_manager(test_manager_), m_error_manager(error_manager_) { Initialize(); }

	int Run();
	
private:
	void Initialize();

	static void ButtonCallback(Fl_Widget* w, void* f) { ((Gui*)f)->ButtonClick(w); }
	static void WindowCallback(Fl_Widget* w, void* f) { ((Gui*)f)->WindowAction(); }

	void ButtonClick(Fl_Widget* w);
	void WindowAction();
	std::string SelectFile(const std::string& initial_dir_);

	std::shared_ptr<OptionsManager> m_options_manager;
	std::shared_ptr<ProblemManager> m_problem_manager;
	std::shared_ptr<TestManager> m_test_manager;
	std::shared_ptr<SettingsWindow> m_settings_window;
	std::shared_ptr<ErrorManager> m_error_manager;

	Fl_Double_Window*	m_main_window;
	Fl_Button*			m_start_test_button;
	Fl_Button*			m_settings_button;
	Fl_Button*			m_show_problem_info_button;
	Fl_Button*			m_show_problem_description_button;
	Fl_Button*			m_solution_file_selector_button;
	Fl_Round_Button*	m_first_test_selector;
	Fl_Round_Button*	m_all_test_selector;
	Fl_Input*			m_solution_selector;
	Fl_Progress*		m_testing_progress;
	Fl_Hold_Browser*	m_problem_browser;

	std::vector<Problem> m_problem_list;
	std::vector<std::shared_ptr<OutputWindow>> m_output_windows;
	int m_output_window_created_file_count = 0;
};