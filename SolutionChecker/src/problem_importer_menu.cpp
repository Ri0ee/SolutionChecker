#include "problem_importer_menu.h"

void ProblemImporterWindow::Initialize()
{
	m_window = new Fl_Double_Window(500, 500, "Problem importer");

	int y = 10, h = 20;

	m_name_input = new Fl_Input(100, y, 300, h, "name");

	y += h + 10;

	m_id_input = new Fl_Input(100, y, 300, h, "id");

	y += h + 10;

	m_description_input = new Fl_Input(100, y, 300, h, "description");

	y += h + 10;

	m_time_limit_input = new Fl_Input(100, y, 300, h, "time limit");

	y += h + 10;

	m_memory_limit_input = new Fl_Input(100, y, 300, h, "memory limit");

	y += h + 10;

	m_input_file_input = new Fl_Input(100, y, 300, h, "input file");

	y += h + 10;

	m_output_file_input = new Fl_Input(100, y, 300, h, "output file");

	y += h + 10;

	m_test_count_input = new Fl_Input(100, y, 300, h, "test count");

	y += h + 10;

	m_answer_files_input = new Fl_Input(100, y, 300, h, "answer files");

	y += h + 10;

	m_checker_src_input = new Fl_Input(100, y, 300, h, "checker src");

	y += h + 10;

	m_checker_exe_input = new Fl_Input(100, y, 300, h, "checker exe");

	y += h + 10;

	m_solution_input = new Fl_Input(100, y, 300, h, "solution");

	y += h + 10;

	m_points_input = new Fl_Input(100, y, 300, h, "points");

	y += h + 10;

	m_bonus_points_input = new Fl_Input(100, y, 300, h, "bonus points");

	y += h + 10;

	m_import_button = new Fl_Button(100, y, 100, h, "Import");
	m_import_button->callback(ButtonCallback, this);
	m_import_button->clear_visible_focus();

	y += h + 10;

	m_window->end();
	m_window->size(410, y);
	m_window->hide();
}

void ProblemImporterWindow::Shutdown()
{

}

void ProblemImporterWindow::ButtonClick(Fl_Widget* w)
{
	std::string button_label(w->label());

	if (button_label == "Import")
	{
		fl_alert("Import successful");
		return;
	}
}

void ProblemImporterWindow::Show()
{
	m_window->show();
}

void ProblemImporterWindow::Hide()
{
	m_window->hide();
}