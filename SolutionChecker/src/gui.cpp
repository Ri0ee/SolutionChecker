#include "gui.h"



void Gui::SelectFile()
{
	OPENFILENAME ofn = {};

	if (ofn.lpstrFile) 
		delete[] ofn.lpstrFile; 

	if (ofn.lpstrInitialDir) 
		delete[] ofn.lpstrInitialDir;

	memset((void*)&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);

	ofn.Flags |= OFN_NOVALIDATE;
	ofn.Flags |= OFN_HIDEREADONLY;
	ofn.Flags |= OFN_EXPLORER;
	ofn.Flags |= OFN_ENABLESIZING;
	ofn.Flags |= OFN_NOCHANGEDIR;

	ofn.nMaxFile = 4096 - 1;
	ofn.lpstrFile = new char[4096];
	ofn.lpstrFile[0] = 0;
	ofn.hwndOwner = GetForegroundWindow();

	ofn.lpstrInitialDir = m_options_manager->GetLastExecutableDir().c_str();
	ofn.lpstrTitle = "Select exe-file";
	ofn.lpstrFilter = "Executable .exe\0*.exe\0\0";

	int err = GetOpenFileName(&ofn);
	if (err == 0) return;

	m_exefile_selector_value->value(ofn.lpstrFile);
	m_options_manager->SetLastExecutableDir(std::string(ofn.lpstrFile));
}

void Gui::ButtonClick(Fl_Widget* w)
{
	std::string button_label(w->label());
	if (button_label == "Use only first test") 
	{
		m_first_test_selector->value(true);
		m_all_test_selector->value(false);
		m_options_manager->SetUseOnlyOneTest(true);
		m_options_manager->SetUseMultipleTests(false);
		return;
	}

	if (button_label == "Use all tests")
	{
		m_first_test_selector->value(false);
		m_all_test_selector->value(true);
		m_options_manager->SetUseOnlyOneTest(false);
		m_options_manager->SetUseMultipleTests(true);
		return;
	}

	if (button_label == "Start testing")
	{

		return;
	}

	if (button_label == "Settings")
	{
		if (m_settings_window->IsVisible())
			m_settings_window->Hide();
		else
			m_settings_window->Show();
		return;
	}
}

bool Gui::Initialize(OptionsManager* options_manager_)
{	
	m_options_manager = options_manager_;

	int y = 10, h = 20;

	Fl::scheme(m_options_manager->GetTheme().c_str());
	Fl::get_system_colors();
	fl_font(FL_HELVETICA, 16);

	m_main_window = new Fl_Double_Window(0, 0, "Solution Checker");

	int selector_spacing = (int)fl_width("Path to exe-file:") + 5;
	m_exefile_selector_value = new Fl_Input(selector_spacing, y, 350, h, "Path to exe-file:");
	m_exefile_selector_value->value(m_options_manager->GetLastExecutableDir().c_str());
	m_exefile_selector_button = new Fl_Button(selector_spacing + 355, y, 500 - (selector_spacing + 355) - 5, h, "...");
	m_exefile_selector_button->callback(this->SelectFileCallback, (void*)this);
	m_exefile_selector_button->clear_visible_focus();

	m_problem_browser = new Fl_Hold_Browser(510, y, 0, 0);
	for (int i = 0; i < 100; i++) m_problem_browser->add((std::string("problem") + std::to_string(i)).c_str());
	m_problem_browser->value(0);

	y += h + 10;

	m_first_test_selector = new Fl_Round_Button(5, y, 150, h, "Use only first test");
	m_first_test_selector->callback(this->ButtonCallback, (void*)this);
	m_first_test_selector->clear_visible_focus();
	if (m_options_manager->GetUseOnlyOneTest())
		m_first_test_selector->set();
	
	y += h + 10;

	m_all_test_selector = new Fl_Round_Button(5, y, 150, h, "Use all tests");
	m_all_test_selector->callback(this->ButtonCallback, (void*)this);
	m_all_test_selector->clear_visible_focus();
	if(m_options_manager->GetUseMultipleTests())
		m_all_test_selector->set();

	y += h + 10;

	m_start_test_button = new Fl_Button(5, y, 100, h, "Start testing");
	m_start_test_button->callback(this->ButtonCallback, (void*)this);
	m_start_test_button->clear_visible_focus();

	m_settings_button = new Fl_Button(110, y, 100, h, "Settings");
	m_settings_button->callback(this->ButtonCallback, (void*)this);
	m_settings_button->clear_visible_focus();

	m_testing_progress = new Fl_Progress(220, y, 275, h, "Testing progress");
	m_testing_progress->deactivate();

	y += h + 10;

	m_problem_browser->size(180, y - 20);
	m_main_window->size(700, y);
	m_main_window->position((int)m_options_manager->GetWindowPosX(), (int)m_options_manager->GetWindowPosY());

	m_main_window->end();

	m_settings_window = new SettingsWindow();
	m_settings_window->Initialize(m_options_manager);

	return true;
}

bool Gui::Run()
{
	m_main_window->show();
	Fl::run();
	return true;
}

void Gui::Shutdown()
{
	
}

// Settings Window

bool SettingsWindow::Initialize(OptionsManager* options_manager_)
{
	m_options_manager = options_manager_;

	int y = 10, h = 20;

	m_window = new Fl_Double_Window(500, 500, "Settings");
	
	int selector_spacing = (int)fl_width("Working dir:") + 5;
	m_working_dir_selector = new Fl_Input(selector_spacing, y, 295, h, "Working dir:");
	m_working_dir_selector->value(m_options_manager->GetWorkingDir().c_str());
	m_working_dir_selector_button = new Fl_Button(selector_spacing + 298, y, 117, h, "Select working dir");
	m_working_dir_selector_button->callback(this->SelectDirectoryCallback, (void*)this);

	y += h + 10;

	m_problem_dir_selector = new Fl_Input(selector_spacing, y, 295, h, "Problems:");
	m_problem_dir_selector->value(m_options_manager->GetProblemDir().c_str());
	m_problem_dir_selector_button = new Fl_Button(selector_spacing + 298, y, 117, h, "Select problem dir");
	m_problem_dir_selector_button->callback(this->SelectDirectoryCallback, (void*)this);

	y += h + 10;

	m_reset_settings_button = new Fl_Button(10, y, 100, h, "Reset settings");
	m_reset_settings_button->callback(this->ButtonCallback, (void*)this);
	m_reset_settings_button->clear_visible_focus();

	y += h + 10;

	m_window->size(500, y);
	m_window->hide();
	m_window->end();
	
	return true;
}

void SettingsWindow::Shutdown()
{

}

void SettingsWindow::Show()
{
	m_window->show();
}

void SettingsWindow::Hide()
{
	m_window->hide();
}

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		std::string tmp = (const char *)lpData;
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

void SettingsWindow::SelectDirectory(int detail_)
{
	TCHAR path[MAX_PATH];
	const char* path_param = "C:\\";

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = ("Browse for folder...");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)path_param;

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

		if (detail_ == 1) // Select working dir
		{
			m_options_manager->SetWorkingDir(std::string(path));
			m_working_dir_selector->value(path);
			return;
		}

		if (detail_ == 2) // Select problem dir
		{
			m_options_manager->SetProblemDir(std::string(path));
			m_problem_dir_selector->value(path);
			return;
		}

		return;
	}
}

void SettingsWindow::ButtonClick(Fl_Widget* w)
{
	std::string button_label(w->label());
	if (button_label == "Reset settings")
	{
		if (fl_ask("Do you really want to reset all settings?"))
		{
			m_options_manager->SetDefaults();
			UpdateWidgetInfo();
		}	
	}
}

void SettingsWindow::UpdateWidgetInfo()
{
	m_problem_dir_selector->value(m_options_manager->GetProblemDir().c_str());
	m_working_dir_selector->value(m_options_manager->GetWorkingDir().c_str());
}