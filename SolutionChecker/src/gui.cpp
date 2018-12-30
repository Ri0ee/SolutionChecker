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
		if (m_problem_browser->value() == 0) return;

		Problem problem = m_problem_list[m_problem_browser->value() - 1];

		w->deactivate();
		m_problem_browser->deactivate();
		m_all_test_selector->deactivate();
		m_first_test_selector->deactivate();

		m_testing_progress->activate();
		m_testing_progress->minimum(0);
		if (m_all_test_selector->value() == true)
			m_testing_progress->maximum((float)problem.m_test_count);
		else
			m_testing_progress->maximum(1);
		m_testing_progress->value(0);

		m_test_manager->StartTesting(m_problem_browser->value() - 1, std::string(m_exefile_selector_value->value()), m_all_test_selector->value());

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

	if (button_label == "...")
	{
		SelectFile();
		return;
	}

	if (button_label == "Show info")
	{	
		if (m_problem_browser->value() == 0) return;

		int id = m_problem_browser->value() - 1;
		std::string message_buffer = "caption: " + m_problem_list[id].m_caption + "\n" +
			"id: " + std::to_string(m_problem_list[id].m_id) + "\n" +
			"path: " + m_problem_list[id].m_path + "\n" +
			"input file name: " + m_problem_list[id].m_input_file_name + "\n" +
			"output file name: " + m_problem_list[id].m_output_file_name + "\n" +
			"test count: " + std::to_string(m_problem_list[id].m_test_count) + "\n" +
			"time limit: " + std::to_string(m_problem_list[id].m_time_limit) + " seconds\n" +
			"memory limit: " + std::to_string(m_problem_list[id].m_memory_limit) + " MB\n";
		fl_alert(message_buffer.c_str());	
		return;
	}

	if (button_label == "Show task description")
	{
		if (m_problem_browser->value() == 0) return;
		m_problem_manager->ShowTaskDescription(m_problem_browser->value() - 1);
		return;
	}
}

void Gui::WindowAction() // Close button pressed
{
	if (m_settings_window != nullptr)
		if(m_settings_window->IsVisible()) 
			m_settings_window->Hide();

	m_options_manager->SetLastProblem(m_problem_browser->value());

	m_main_window->hide();
}

bool Gui::Initialize(OptionsManager* options_manager_, ProblemManager* problem_manager_, TestManager* test_manager_)
{	
	m_options_manager = options_manager_;
	m_problem_manager = problem_manager_;
	m_test_manager = test_manager_;

	m_problem_list = m_problem_manager->GetProblemList();

	int y = 10, h = 20;
	Fl::scheme(m_options_manager->GetThemeName().c_str());
	Fl::get_system_colors();
	fl_font(FL_HELVETICA, 16);

	m_main_window = new Fl_Double_Window(0, 0, "Solution Checker");
	m_main_window->callback(WindowCallback, this);

	int selector_spacing = (int)fl_width("Path to exe-file:") + 5;
	m_exefile_selector_value = new Fl_Input(selector_spacing, y, 350, h, "Path to exe-file:");
	m_exefile_selector_value->value(m_options_manager->GetLastExecutableDir().c_str());
	m_exefile_selector_button = new Fl_Button(selector_spacing + 355, y, 500 - (selector_spacing + 355) - 5, h, "...");
	m_exefile_selector_button->callback(ButtonCallback, this);
	m_exefile_selector_button->clear_visible_focus();

	m_problem_browser = new Fl_Hold_Browser(510, y, 0, 0);
	for (unsigned i = 0; i < m_problem_list.size(); i++)
		m_problem_browser->add(std::string(m_problem_list[i].m_folder_name + ": " + m_problem_list[i].m_caption).c_str());
	m_problem_browser->value(m_options_manager->GetLastProblem());

	y += h + 10;

	m_first_test_selector = new Fl_Round_Button(5, y, 150, h, "Use only first test");
	m_first_test_selector->callback(ButtonCallback, this);
	m_first_test_selector->clear_visible_focus();
	if (m_options_manager->GetUseOnlyOneTest())
		m_first_test_selector->set();
	
	y += h + 10;

	m_all_test_selector = new Fl_Round_Button(5, y, 150, h, "Use all tests");
	m_all_test_selector->callback(ButtonCallback, this);
	m_all_test_selector->clear_visible_focus();
	if(m_options_manager->GetUseMultipleTests())
		m_all_test_selector->set();

	y += h + 10;

	m_start_test_button = new Fl_Button(5, y, 100, h, "Start testing");
	m_start_test_button->callback(ButtonCallback, this);
	m_start_test_button->clear_visible_focus();

	m_settings_button = new Fl_Button(110, y, 100, h, "Settings");
	m_settings_button->callback(ButtonCallback, this);
	m_settings_button->clear_visible_focus();

	m_show_problem_info_button = new Fl_Button(215, y, 100, h, "Show info");
	m_show_problem_info_button->callback(ButtonCallback, this);
	m_show_problem_info_button->clear_visible_focus();

	m_show_problem_description_button = new Fl_Button(320, y, 175, h, "Show task description");
	m_show_problem_description_button->callback(ButtonCallback, this);
	m_show_problem_description_button->clear_visible_focus();

	y += h + 10;

	m_testing_progress = new Fl_Progress(5, y, 490, h, "Testing progress");
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

	while (Fl::wait() > 0)
	{
		unsigned int testing_state = m_test_manager->GetTestingState();
		if (testing_state == TESTING_STATE_ONLINE || testing_state == TESTING_STATE_ERROR)
		{
			if (testing_state == TESTING_STATE_ERROR)
			{
				std::string error_message = m_test_manager->GetErrorMessage();
				while (error_message != "No errors")
				{
					fl_alert(error_message.c_str());
					error_message = m_test_manager->GetErrorMessage();
				}
			}
			else
			{
				int testing_stage = m_test_manager->GetTestingStage();

				m_testing_progress->value((float)testing_stage + 1);
				if (testing_stage == m_problem_list[m_problem_browser->value() - 1].m_test_count - 1 || (m_all_test_selector->value() == false && testing_stage == 0))
				{
					m_test_manager->FinishTesting();

					std::vector<Test> temp_result_data;
					std::string temp_output_buf("");
					m_test_manager->GetResultData(temp_result_data);

					for (unsigned i = 0; i < temp_result_data.size(); i++)
					{
						temp_output_buf = temp_output_buf + std::to_string(i + 1) + ": " +
							(temp_result_data[i].m_status & TEST_STATUS_OK ? "OK\n" : "FAIL\n");
					}

					fl_alert(temp_output_buf.c_str());

					m_problem_browser->activate();
					m_start_test_button->activate();
					m_all_test_selector->activate();
					m_first_test_selector->activate();

					m_testing_progress->deactivate();
					m_testing_progress->value(0);
				}
			}
		}

		if (m_settings_window->IsProblemBrowserUpdateNeeded())
		{
			m_problem_manager->ChangeDir(m_options_manager->GetProblemDir());
			m_problem_manager->SearchForProblems();
			m_problem_list = m_problem_manager->GetProblemList();

			m_problem_browser->clear();
			for (unsigned i = 0; i < m_problem_list.size(); i++)
				m_problem_browser->add(std::string(m_problem_list[i].m_folder_name + ": " + m_problem_list[i].m_caption).c_str());

			m_settings_window->SetProblemBrowserUpdateNeeded(false);
		}
	}

	return true;
}

void Gui::Shutdown()
{
	if (m_settings_window != nullptr)
	{
		m_settings_window->Shutdown();
		delete m_settings_window;
		m_settings_window = nullptr;
	}
}

// Settings Window

bool SettingsWindow::Initialize(OptionsManager* options_manager_)
{
	m_options_manager = options_manager_;

	m_problem_browser_update_needed = false;

	int y = 10, h = 20;
	int w = 520;

	m_window = new Fl_Double_Window(w, 500, "Settings");
	
	int selector_spacing = (int)fl_width("Working dir:") + 5;
	m_working_dir_selector = new Fl_Input(selector_spacing, y, 295, h, "Working dir:");
	m_working_dir_selector->value(m_options_manager->GetWorkingDir().c_str());

	m_working_dir_selector_button = new Fl_Button(selector_spacing + 298, y, w - selector_spacing - 298 - 10, h, "Select working dir");
	m_working_dir_selector_button->callback(this->ButtonCallback, this);
	m_working_dir_selector_button->clear_visible_focus();

	y += h + 10;

	m_problem_dir_selector = new Fl_Input(selector_spacing, y, 295, h, "Problems:");
	m_problem_dir_selector->value(m_options_manager->GetProblemDir().c_str());

	m_problem_dir_selector_button = new Fl_Button(selector_spacing + 298, y, w - selector_spacing - 298 - 10, h, "Select problem dir");
	m_problem_dir_selector_button->callback(this->ButtonCallback, this);
	m_problem_dir_selector_button->clear_visible_focus();

	y += h + 10;

	m_theme_choice = new Fl_Choice(selector_spacing, y, 295, 22, "Themes:");
	m_theme_choice->clear_visible_focus();
	m_theme_choice->callback(this->ButtonCallback, this);
	m_theme_choice->add("none");
	m_theme_choice->add("gtk+");
	m_theme_choice->add("gleam");
	m_theme_choice->add("plastic");
	m_theme_choice->value(m_options_manager->GetThemeId());

	y += h + 12;

	m_reset_settings_button = new Fl_Button(10, y, 100, h, "Reset settings");
	m_reset_settings_button->callback(this->ButtonCallback, this);
	m_reset_settings_button->clear_visible_focus();

	y += h + 10;

	m_window->size(w, y);
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
	std::string path_param("C:\\");
	if(detail_ == SELECT_WORKING_DIRECTORY)
		path_param = m_options_manager->GetWorkingDir();

	if (detail_ == SELECT_PROBLEM_DIRECTORY)
		path_param = m_options_manager->GetProblemDir();

	BROWSEINFO bi = { 0 };

	if (detail_ == SELECT_WORKING_DIRECTORY)
		bi.lpszTitle = ("Browse for working folder...");
	else if (detail_ == SELECT_PROBLEM_DIRECTORY)
		bi.lpszTitle = ("Browse for problem folder...");
	else
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

		if (detail_ == SELECT_WORKING_DIRECTORY) 
		{
			m_options_manager->SetWorkingDir(std::string(path));
			m_working_dir_selector->value(path);
			return;
		}

		if (detail_ == SELECT_PROBLEM_DIRECTORY)
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
		if (fl_ask("Do you really want to reset settings?"))
		{
			m_options_manager->SetDefaults();
			UpdateWidgetInfo();
		}
		return;
	}

	if (button_label == "Select working dir")
	{
		SelectDirectory(SELECT_WORKING_DIRECTORY);
		return;
	}

	if (button_label == "Select problem dir")
	{
		SelectDirectory(SELECT_PROBLEM_DIRECTORY);
		m_problem_browser_update_needed = true;
		return;
	}

	if (m_theme_choice->changed())
	{
		m_options_manager->SetTheme(m_options_manager->GetThemeName(m_theme_choice->value()));
		Fl::scheme(m_options_manager->GetThemeName().c_str());
		Fl::get_system_colors();
		Fl::reload_scheme();
		return;
	}
}

void SettingsWindow::UpdateWidgetInfo()
{
	m_problem_dir_selector->value(m_options_manager->GetProblemDir().c_str());
	m_working_dir_selector->value(m_options_manager->GetWorkingDir().c_str());
	m_theme_choice->value(m_options_manager->GetThemeId());
}