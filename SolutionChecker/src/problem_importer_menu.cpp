#include "problem_importer_menu.h"

std::vector<std::string> ProblemCreatorWindow::SelectMultipleFiles(const std::string& initial_dir_)
{
	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);

	if (ofn.lpstrFile)
		delete[] ofn.lpstrFile;

	if (ofn.lpstrInitialDir)
		delete[] ofn.lpstrInitialDir;

	ofn.Flags |= OFN_NOVALIDATE;
	ofn.Flags |= OFN_HIDEREADONLY;
	ofn.Flags |= OFN_EXPLORER;
	ofn.Flags |= OFN_ENABLESIZING;
	ofn.Flags |= OFN_ALLOWMULTISELECT;
	ofn.Flags |= OFN_FILEMUSTEXIST;

	ofn.nMaxFile = 4096 - 1;
	ofn.lpstrFile = new char[4096];
	ofn.lpstrFile[0] = 0;
	ofn.hwndOwner = NULL;

	ofn.lpstrInitialDir = initial_dir_.c_str();
	ofn.lpstrTitle = "Select files";

	int err = GetOpenFileName(&ofn);
	if (err == 0) return std::vector<std::string>();

	std::vector<std::string> res;
	char* str = ofn.lpstrFile;
	std::string directory = str;
	str += (directory.length() + 1);
	while (*str) {
		std::string filename = str;
		str += (filename.length() + 1);
		res.push_back(filename);
	}

	return res;
}

void ProblemCreatorWindow::Initialize()
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
	m_answer_file_selector_button = new Fl_Button(410, y, 200, h, "select answer files...");
	m_answer_file_selector_button->callback(ButtonCallback, this);

	y += h + 10;

	m_input_files_input = new Fl_Input(100, y, 300, h, "input files");
	m_input_file_selector_button = new Fl_Button(410, y, 200, h, "select input files...");
	m_input_file_selector_button->callback(ButtonCallback, this);

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

	m_create_button = new Fl_Button(100, y, 100, h, "Create");
	m_create_button->callback(ButtonCallback, this);
	m_create_button->clear_visible_focus();

	y += h + 10;

	m_window->end();
	m_window->size(620, y);
	m_window->hide();
}

void ProblemCreatorWindow::Shutdown()
{

}

void ProblemCreatorWindow::ButtonClick(Fl_Widget* w)
{
	std::string button_label(w->label());

	if (button_label == "select answer files...")
	{
		auto file_vec = SelectMultipleFiles(m_problem_manager->GetPath().c_str());
		std::string file_str;
		for (auto& file : file_vec)
			file_str += file + " ";

		m_answer_files_input->value(file_str.c_str());
		return;
	}

	if (button_label == "select input files...")
	{
		auto file_vec = SelectMultipleFiles(m_problem_manager->GetPath().c_str());
		std::string file_str;
		for (auto& file : file_vec)
			file_str += file + " ";

		m_input_files_input->value(file_str.c_str());
		return;
	}

	if (button_label == "Create")
	{
		std::stringstream sst;

		Problem problem;
		try 
		{
			sst << m_bonus_points_input->value();
			sst >> problem.m_bonus_points;
			sst.clear();

			sst << m_id_input->value();
			sst >> problem.m_id;
			sst.clear();

			sst << m_memory_limit_input->value();
			sst >> problem.m_memory_limit;
			sst.clear();

			sst << m_time_limit_input->value();
			sst >> problem.m_time_limit;
			sst.clear();

			sst << m_test_count_input->value();
			sst >> problem.m_test_count;
		}
		catch (const std::exception& e)
		{
			fl_alert(e.what());
			return;
		}
		
		problem.m_description_file = m_description_input->value();
		problem.m_input_file = m_input_file_input->value();
		problem.m_output_file = m_output_file_input->value();
		problem.m_name = m_name_input->value();

		m_problem_manager->CreateProblem(problem, "ProblemLayout.xml");
		fl_alert("Problem created successfully");
		return;
	}
}

void ProblemCreatorWindow::Show()
{
	m_window->show();
}

void ProblemCreatorWindow::Hide()
{
	m_window->hide();
}