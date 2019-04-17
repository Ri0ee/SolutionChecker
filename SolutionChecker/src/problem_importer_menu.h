#pragma once

#define WIN32
#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Input.H"
#include "FL/fl_message.H"

#include <string>



class ProblemImporterWindow
{
public:
	ProblemImporterWindow() { Initialize(); }
	~ProblemImporterWindow() { Shutdown(); }

	void Show();
	void Hide();

private:
	void Initialize();
	void Shutdown();

	void ButtonClick(Fl_Widget* w);
	static void ButtonCallback(Fl_Widget* w, void* f) { ((ProblemImporterWindow*)f)->ButtonClick(w); }

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
	Fl_Input* m_checker_src_input;
	Fl_Input* m_checker_exe_input;
	Fl_Input* m_solution_input;
	Fl_Input* m_points_input;
	Fl_Input* m_bonus_points_input;

	Fl_Button* m_import_button;
};