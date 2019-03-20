#pragma once

#define WIN32
#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Input.H"
#include "FL/Fl_Choice.H"
#include "FL/fl_draw.H"
#include "FL/fl_ask.H"
#include "FL/Fl_Value_Input.H"

#include <shlobj.h>
#include <string>

#include "options.h"
#include "problem_creator_menu.h"



class SettingsWindow
{
public:
	SettingsWindow(OptionsManager* options_manager_) : m_options_manager(options_manager_) { Initialize(); }
	~SettingsWindow() {}

	void Show();
	void Hide();

	bool IsVisible() { return m_window->visible(); }
	bool IsProblemBrowserUpdateNeeded() { return m_problem_browser_update_needed; }
	void SetProblemBrowserUpdateNeeded(bool value_) { m_problem_browser_update_needed = value_; }

private:
	void Initialize();

	void ButtonClick(Fl_Widget* w);
	static void ButtonCallback(Fl_Widget* w, void* f) { ((SettingsWindow*)f)->ButtonClick(w); }

	void UpdateWidgetInfo();
	std::string SelectDirectory();
	std::string SelectFile();

	OptionsManager* m_options_manager = nullptr;

	ProblemCreatorWindow* m_problem_creator_window = nullptr;

	Fl_Double_Window* m_window;

	Fl_Input* m_working_dir_selector;
	Fl_Input* m_problem_dir_selector;
	Fl_Input* m_temp_dir_selector;

	Fl_Input* m_pascal_compiler_dir_selector;
	Fl_Input* m_cpp_compiler_dir_selector;
	Fl_Input* m_c_compiler_dir_selector;
	Fl_Input* m_java_compiler_dir_selector;
	Fl_Input* m_c_default_arg_selector;
	Fl_Input* m_cpp_default_arg_selector;
	Fl_Input* m_pascal_default_arg_selector;
	Fl_Input* m_java_default_arg_selector;

	Fl_Button* m_working_dir_selector_button;
	Fl_Button* m_problem_dir_selector_button;
	Fl_Button* m_temp_dir_selector_button;

	Fl_Button* m_pascal_compiler_dir_selector_button;
	Fl_Button* m_cpp_compiler_dir_selector_button;
	Fl_Button* m_c_compiler_dir_selector_button;
	Fl_Button* m_java_compiler_dir_selector_button;
	Fl_Button* m_reset_settings_button;
	Fl_Button* m_apply_settings_button;

	Fl_Choice* m_theme_choice;

	Fl_Value_Input* m_test_memory_limit_input;

	bool m_problem_browser_update_needed;
};