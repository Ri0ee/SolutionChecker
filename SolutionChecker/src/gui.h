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
#include "FL/Fl_Multi_Label.H"

#include <shlobj.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>

#include "options.h"
#include "tester.h"
#include "problems.h"

#define SELECT_WORKING_DIRECTORY 1
#define SELECT_PROBLEM_DIRECTORY 2



class OutputWindow
{
public:
	OutputWindow() {
		Initialize();
	}

	~OutputWindow() {
		Shutdown();
	}

private:
	bool Initialize();
	void Shutdown();
};

class SettingsWindow
{
public:
	SettingsWindow(OptionsManager* options_manager_) {
		Initialize(options_manager_);
	}

	SettingsWindow() {}

	~SettingsWindow() {}

	void Show();
	void Hide();

	bool IsVisible() { return m_window->visible(); }
	bool IsProblemBrowserUpdateNeeded() { return m_problem_browser_update_needed; }
	void SetProblemBrowserUpdateNeeded(bool value_) { m_problem_browser_update_needed = value_; }

private:
	void Initialize(OptionsManager* options_manager_);

	void ButtonClick(Fl_Widget* w);
	static void ButtonCallback(Fl_Widget* w, void* f) { ((SettingsWindow*)f)->ButtonClick(w); }

	void UpdateWidgetInfo();
	void SelectDirectory(int detail_);

	OptionsManager* m_options_manager = nullptr;

	Fl_Double_Window* m_window;

	Fl_Input* m_working_dir_selector;
	Fl_Input* m_problem_dir_selector;

	Fl_Button* m_working_dir_selector_button;
	Fl_Button* m_problem_dir_selector_button;
	Fl_Button* m_reset_settings_button;
	Fl_Button* m_apply_settings_button;

	Fl_Choice* m_theme_choice;

	Fl_Value_Input* m_test_memory_limit_input;

	bool m_problem_browser_update_needed;
};

class Gui
{
public:
	Gui(OptionsManager* options_manager_, ProblemManager* problem_manager_, TestManager* test_manager_) {
		Initialize(options_manager_, problem_manager_, test_manager_);
	}

	Gui() {}

	~Gui() {
		Shutdown();
	}

	bool Run();
	
private:
	void Initialize(OptionsManager* options_manager_, ProblemManager* problem_manager_, TestManager* test_manager_);
	void Shutdown();

	static void ButtonCallback(Fl_Widget* w, void* f) { ((Gui*)f)->ButtonClick(w); }
	static void WindowCallback(Fl_Widget* w, void* f) { ((Gui*)f)->WindowAction(); }

	void ButtonClick(Fl_Widget* w);
	void WindowAction();
	void SelectFile();

	OptionsManager* m_options_manager = nullptr;
	ProblemManager* m_problem_manager = nullptr;
	TestManager* m_test_manager = nullptr;

	SettingsWindow* m_settings_window = nullptr;

	Fl_Double_Window* m_main_window;

	Fl_Button* m_start_test_button;
	Fl_Button* m_settings_button;
	Fl_Button* m_show_problem_info_button;
	Fl_Button* m_show_problem_description_button;
	Fl_Button* m_exefile_selector_button;

	Fl_Round_Button* m_first_test_selector;
	Fl_Round_Button* m_all_test_selector;

	Fl_Input* m_exefile_selector_value;
	Fl_Progress* m_testing_progress;
	Fl_Hold_Browser* m_problem_browser;

	std::vector<Problem> m_problem_list;
};