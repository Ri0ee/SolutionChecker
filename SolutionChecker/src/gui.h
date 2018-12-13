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

#include <shlobj.h>
#include <iostream>
#include <string>

#include "options.h"
#include "tester.h"



class OutputWindow
{
public:
	OutputWindow() {}
	~OutputWindow() {}

	bool Initialize();
	void Shutdown();

private:

};

class SettingsWindow
{
public:
	SettingsWindow() {}
	~SettingsWindow() {}

	bool Initialize(OptionsManager* options_manager_);
	void Shutdown();

	void Show();
	void Hide();
	void UpdateWidgetInfo();

	bool IsVisible() { return m_window->visible(); }

	void SelectDirectory(int detail_);
	static void SelectDirectoryCallback(Fl_Widget* w, void* f) { 
		std::string label(w->label());
		if (label == "Select working dir")
		{
			((SettingsWindow*)f)->SelectDirectory(1);
			return;
		}
		
		if (label == "Select problem dir")
		{
			((SettingsWindow*)f)->SelectDirectory(2);
			return;
		}
	}

	void ButtonClick(Fl_Widget* w);
	static void ButtonCallback(Fl_Widget* w, void* f) { ((SettingsWindow*)f)->ButtonClick(w); }

private:
	Fl_Double_Window* m_window;

	Fl_Input* m_working_dir_selector;
	Fl_Button* m_working_dir_selector_button;

	Fl_Input* m_problem_dir_selector;
	Fl_Button* m_problem_dir_selector_button;

	Fl_Button* m_reset_settings_button;

	OptionsManager* m_options_manager;
};

class Gui
{
public:
	Gui() {}
	~Gui() {}

	bool Initialize(OptionsManager* options_manager_);
	bool Run();
	void Shutdown();

	void SelectFile();
	static void SelectFileCallback(Fl_Widget* w, void* f) { ((Gui*)f)->SelectFile(); }

	void ButtonClick(Fl_Widget *w);
	static void ButtonCallback(Fl_Widget* w, void* f) { ((Gui*)f)->ButtonClick(w); }

private:
	OptionsManager* m_options_manager;

	Fl_Double_Window* m_main_window;

	Fl_Button* m_start_test_button;
	Fl_Button* m_settings_button;

	Fl_Round_Button* m_first_test_selector;
	Fl_Round_Button* m_all_test_selector;

	Fl_Input* m_exefile_selector_value;
	Fl_Button* m_exefile_selector_button;

	Fl_Progress* m_testing_progress;

	SettingsWindow* m_settings_window;

	Fl_Hold_Browser* m_problem_browser;

};