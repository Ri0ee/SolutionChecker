#pragma once

#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Input.H"
#include "FL/fl_message.H"

#include <shlobj.h>
#include <string>
#include <sstream>
#include <filesystem>

#include "problems.h"
#include "options.h"



class ProblemCreatorWindow
{
public:
	ProblemCreatorWindow(ProblemManager* problem_manager_, OptionsManager* options_manager_) :
		m_problem_manager(problem_manager_), m_options_manager(options_manager_) { Initialize(); }
	~ProblemCreatorWindow() { Shutdown(); }

	void Show();
	void Hide();

private:
	void Initialize();
	void Shutdown();

	void ButtonClick(Fl_Widget* w);
	static void ButtonCallback(Fl_Widget* w, void* f) { ((ProblemCreatorWindow*)f)->ButtonClick(w); }

	std::vector<std::string> SelectMultipleFiles(const std::string& initial_dir_);
	std::string SelectDirectory();

	ProblemManager* m_problem_manager = nullptr;
	OptionsManager* m_options_manager = nullptr;

	Fl_Double_Window* m_window;

	Fl_Input* m_name_input;
	Fl_Input* m_id_input;
	Fl_Input* m_description_input;
	Fl_Input* m_time_limit_input;
	Fl_Input* m_memory_limit_input;
	Fl_Input* m_input_file_input;
	Fl_Input* m_output_file_input;
	Fl_Input* m_test_count_input;
	Fl_Input* m_answer_files_input;
	Fl_Input* m_input_files_input;
	Fl_Input* m_checker_src_input;
	Fl_Input* m_checker_exe_input;
	Fl_Input* m_solution_input;
	Fl_Input* m_points_input;
	Fl_Input* m_bonus_points_input;
	Fl_Input* m_base_dir_input;

	Fl_Button* m_create_button;
	Fl_Button* m_input_file_selector_button;
	Fl_Button* m_answer_file_selector_button;
	Fl_Button* m_base_dir_selector_button;
};