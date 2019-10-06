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

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		std::string tmp = (const char*)lpData;
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

std::string ProblemCreatorWindow::SelectDirectory()
{
	TCHAR path[MAX_PATH];
	std::string path_param(m_problem_manager->GetPath());

	BROWSEINFO bi = { 0 };

	bi.lpszTitle = ("Browse for folder...");

	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)path_param.c_str();

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		//get the name of the folder and put it in path
		SHGetPathFromIDList(pidl, path);

		//free memory used
		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}

		return std::string(path);
	}

	return std::string("");
}

void ProblemCreatorWindow::Initialize()
{
	m_window = new Fl_Double_Window(500, 500, "Problem importer");

	int y = 10, h = 20;

	m_base_dir_input = new Fl_Input(100, y, 300, h, "base dir");
	m_base_dir_selector_button = new Fl_Button(410, y, 200, h, "select problem base dir...");
	m_base_dir_selector_button->callback(ButtonCallback, this);

	y += h + 10;

	m_name_input = new Fl_Input(100, y, 300, h, "name");

	y += h + 10;

	m_id_input = new Fl_Input(100, y, 300, h, "id");
	// TODO: advise an ID

	y += h + 10;

	m_description_input = new Fl_Input(100, y, 300, h, "description");
	m_description_input->value(m_options_manager->AutofillDescriptionFile().c_str());

	y += h + 10;

	m_time_limit_input = new Fl_Input(100, y, 300, h, "time limit");
	m_time_limit_input->value(std::to_string(m_options_manager->AutofillTimeLimit()).c_str());

	y += h + 10;

	m_memory_limit_input = new Fl_Input(100, y, 300, h, "memory limit");
	m_memory_limit_input->value(std::to_string(m_options_manager->AutofillMemLimit()).c_str());

	y += h + 10;

	m_input_file_input = new Fl_Input(100, y, 300, h, "input file");
	m_input_file_input->value(m_options_manager->AutofillInputFile().c_str());

	y += h + 10;

	m_output_file_input = new Fl_Input(100, y, 300, h, "output file");
	m_output_file_input->value(m_options_manager->AutofillOutputFile().c_str());

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
	m_checker_src_input->value(m_options_manager->AutofillCheckerSrc().c_str());

	y += h + 10;

	m_checker_exe_input = new Fl_Input(100, y, 300, h, "checker exe");
	m_checker_exe_input->value(m_options_manager->AutofillCheckerExe().c_str());

	y += h + 10;

	m_solution_input = new Fl_Input(100, y, 300, h, "solution");
	m_solution_input->value(m_options_manager->AutofillSolution().c_str());

	y += h + 10;

	m_points_input = new Fl_Input(100, y, 300, h, "points");
	m_points_input->value(std::to_string(m_options_manager->AutofillPoints()).c_str());

	y += h + 10;

	m_bonus_points_input = new Fl_Input(100, y, 300, h, "bonus points");
	m_bonus_points_input->value(std::to_string(m_options_manager->AutofillBonusPoints()).c_str());

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

	if (button_label == "select problem base dir...") 
	{
		m_base_dir_input->value(SelectDirectory().c_str());
		return;
	}

	if (button_label == "select answer files...")
	{
		auto path = (std::string(m_base_dir_input->value()).empty() ? m_problem_manager->GetPath() : std::string(m_base_dir_input->value()));
		auto file_vec = SelectMultipleFiles(path.c_str());
		std::string file_str;
		for (auto& file : file_vec)
			file_str += file + " ";

		m_answer_files_input->value(file_str.c_str());
		return;
	}

	if (button_label == "select input files...")
	{
		auto path = (std::string(m_base_dir_input->value()).empty() ? m_problem_manager->GetPath() : std::string(m_base_dir_input->value()));
		auto file_vec = SelectMultipleFiles(path.c_str());
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
		problem.m_base_path = m_base_dir_input->value();

		std::string answer_files = m_answer_files_input->value();
		std::string input_files = m_input_files_input->value();

		while (!answer_files.empty()) {
			Problem::Test test;
			test.m_answer_file = answer_files.substr(0, answer_files.find_first_of(" "));
			test.m_input_file = input_files.substr(0, input_files.find_first_of(" "));

			answer_files.erase(0, answer_files.find_first_of(" ") + 1);
			input_files.erase(0, input_files.find_first_of(" ") + 1);

			sst << m_points_input->value();
			sst >> test.points;

			problem.m_tests.push_back(test);
		}

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