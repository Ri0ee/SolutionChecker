#pragma once

#define WIN32
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



class Gui
{
public:
	Gui(OptionsManager* options_manager_, ProblemManager* problem_manager_, TestManager* test_manager_) :
		m_options_manager(options_manager_), m_problem_manager(problem_manager_), m_test_manager(test_manager_) { Initialize(); }
	~Gui() { Shutdown(); }

	int Run();
	
private:
	void Initialize();
	void Shutdown();

	static void ButtonCallback(Fl_Widget* w, void* f) { ((Gui*)f)->ButtonClick(w); }
	static void WindowCallback(Fl_Widget* w, void* f) { ((Gui*)f)->WindowAction(); }

	void ButtonClick(Fl_Widget* w);
	void WindowAction();
	std::string SelectFile();

	OptionsManager* m_options_manager = nullptr;
	ProblemManager* m_problem_manager = nullptr;
	TestManager*	m_test_manager = nullptr;
	SettingsWindow* m_settings_window = nullptr;

	Fl_Double_Window*	m_main_window;
	Fl_Button*			m_start_test_button;
	Fl_Button*			m_settings_button;
	Fl_Button*			m_show_problem_info_button;
	Fl_Button*			m_show_problem_description_button;
	Fl_Button*			m_exefile_selector_button;
	Fl_Round_Button*	m_first_test_selector;
	Fl_Round_Button*	m_all_test_selector;
	Fl_Input*			m_exefile_selector;
	Fl_Progress*		m_testing_progress;
	Fl_Hold_Browser*	m_problem_browser;

	std::vector<Problem>		m_problem_list;
	std::vector<OutputWindow*>	m_output_windows;
	int m_output_window_created_file_count = 0;
};