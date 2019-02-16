#include "gui.h"



// Output window

void OutputWindow::ListElement::Initialize()
{
	int header_width =	(int)fl_width(m_header.c_str());
	int info_width =	(int)fl_width(m_info.c_str());
	m_header_display =	new Fl_Box(m_x, m_y, header_width, 20);
	m_info_display =	new Fl_Box(m_x + header_width + 10, m_y, info_width, 20);

	m_header_display->box(FL_BORDER_FRAME);
	m_info_display->box(FL_BORDER_BOX);

	m_header_display->copy_label(m_header.c_str());
	m_info_display->copy_label(m_info.c_str());

	m_header_display->labelcolor(m_text_color);
	m_info_display->labelcolor(m_text_color);

	m_header_display->labelfont(FL_HELVETICA_BOLD);
	m_info_display->labelfont(FL_HELVETICA_BOLD);
}

void OutputWindow::Initialize()
{
	m_window = new Fl_Double_Window(500, 520, "Test results");
	
	int h = ResetResultList(m_test_result_list);

	m_window->size(400, h);
	m_window->hide();
	m_window->end();
}

void OutputWindow::Hide()
{
	m_window->hide();
}

void OutputWindow::Show()
{
	m_window->show();
}

int OutputWindow::ResetResultList(std::vector<Test>& test_result_list_)
{
	int h = 10;
	for (auto test : test_result_list_)
	{
		Fl_Color color;
		std::string temp_id_buf = std::to_string(test.m_id) + ":";
		std::string temp_str_buf("");
		if (test.m_status & TEST_STATUS_OK)
		{
			temp_str_buf.append("TEST_STATUS_OK; ");
			color = FL_DARK_GREEN;
		}

		if (test.m_status & TEST_STATUS_FAIL)
		{
			temp_str_buf.append("TEST_STATUS_FAIL; ");
			color = FL_DARK_RED;
		}

		if (test.m_status & TEST_STATUS_MEMORY_LIMIT)
		{
			temp_str_buf.append("TEST_STATUS_MEMORY_LIMIT; ");
			color = FL_DARK_RED;
		}

		if (test.m_status & TEST_STATUS_RUNTIME_ERROR)
		{
			temp_str_buf.append("TEST_STATUS_RUNTIME_ERROR; ");
			color = FL_DARK_RED;
		}
			
		if (test.m_status & TEST_STATUS_TIME_LIMIT)
		{
			temp_str_buf.append("TEST_STATUS_TIME_LIMIT; ");
			color = FL_DARK_RED;
		}
			
		if (test.m_status & TEST_STATUS_UNKNOWN)
		{
			temp_str_buf.append("TEST_STATUS_UNKNOWN; ");
			color = FL_DARK_YELLOW;
		}

		ListElement temp_text_union(10, h, temp_id_buf, temp_str_buf, color);
		m_text_unions.push_back(temp_text_union);
		h += 30;
	}

	return h;
}

// Settings Window

void SettingsWindow::Initialize()
{
	m_problem_browser_update_needed = false;

	int y = 10, h = 20;
	int w = 700;
	int selector_spacing = (int)fl_width("Working dir:") + 5;

	double s1 = fl_width("Cpp Compiler:");
	double s2 = fl_width("C Compiler:");
	double s3 = fl_width("Pascal Compiler:");
	double s4 = fl_width("Java Compiler:");
	int compiler_selector_spacing = (int)max(max(s1, s2), max(s3, s4)) + 5;

	m_window = new Fl_Double_Window(w, 500, "Settings");

	m_working_dir_selector = new Fl_Input(selector_spacing, y, 400, h, "Working dir:");
	m_working_dir_selector->value(m_options_manager->WorkingDir().c_str());

	m_working_dir_selector_button = new Fl_Button(selector_spacing + 403, y, w - selector_spacing - 400 - 10, h, "Working dir...");
	m_working_dir_selector_button->callback(ButtonCallback, this);
	m_working_dir_selector_button->clear_visible_focus();

	y += h + 10;

	m_problem_dir_selector = new Fl_Input(selector_spacing, y, 400, h, "Problems:");
	m_problem_dir_selector->value(m_options_manager->ProblemDir().c_str());

	m_problem_dir_selector_button = new Fl_Button(selector_spacing + 403, y, w - selector_spacing - 400 - 10, h, "Problem dir...");
	m_problem_dir_selector_button->callback(ButtonCallback, this);
	m_problem_dir_selector_button->clear_visible_focus();

	y += h + 10;
	y += h + 10;

	m_cpp_compiler_dir_selector = new Fl_Input(compiler_selector_spacing, y, 400, h, "Cpp Compiler:");
	m_cpp_compiler_dir_selector->value(m_options_manager->CppCompilerDir().c_str());

	m_cpp_compiler_dir_selector_button = new Fl_Button(compiler_selector_spacing + 403, y, w - compiler_selector_spacing - 400 - 10, h, "Cpp compiler...");
	m_cpp_compiler_dir_selector_button->callback(ButtonCallback, this);
	m_cpp_compiler_dir_selector_button->clear_visible_focus();

	y += h + 10;

	m_c_compiler_dir_selector = new Fl_Input(compiler_selector_spacing, y, 400, h, "C Compiler:");
	m_c_compiler_dir_selector->value(m_options_manager->CCompilerDir().c_str());

	m_c_compiler_dir_selector_button = new Fl_Button(compiler_selector_spacing + 403, y, w - compiler_selector_spacing - 400 - 10, h, "C compiler...");
	m_c_compiler_dir_selector_button->callback(ButtonCallback, this);
	m_c_compiler_dir_selector_button->clear_visible_focus();

	y += h + 10;

	m_pascal_compiler_dir_selector = new Fl_Input(compiler_selector_spacing, y, 400, h, "Pascal Compiler:");
	m_pascal_compiler_dir_selector->value(m_options_manager->PascalCompilerDir().c_str());

	m_pascal_compiler_dir_selector_button = new Fl_Button(compiler_selector_spacing + 403, y, w - compiler_selector_spacing - 400 - 10, h, "Pascal compiler...");
	m_pascal_compiler_dir_selector_button->callback(ButtonCallback, this);
	m_pascal_compiler_dir_selector_button->clear_visible_focus();

	y += h + 10;

	m_java_compiler_dir_selector = new Fl_Input(compiler_selector_spacing, y, 400, h, "Java Compiler:");
	m_java_compiler_dir_selector->value(m_options_manager->JavaCompilerDir().c_str());

	m_java_compiler_dir_selector_button = new Fl_Button(compiler_selector_spacing + 403, y, w - compiler_selector_spacing - 400 - 10, h, "Java compiler...");
	m_java_compiler_dir_selector_button->callback(ButtonCallback, this);
	m_java_compiler_dir_selector_button->clear_visible_focus();

	y += h + 10;
	y += h + 10;

	m_theme_choice = new Fl_Choice(selector_spacing, y, 295, 22, "Themes:");
	m_theme_choice->clear_visible_focus();
	m_theme_choice->callback(ButtonCallback, this);
	m_theme_choice->add("none");
	m_theme_choice->add("gtk+");
	m_theme_choice->add("gleam");
	m_theme_choice->add("plastic");
	m_theme_choice->value(m_options_manager->GetThemeId());

	y += h + 12;

	m_test_memory_limit_input = new Fl_Value_Input(selector_spacing, y, 295, h, "Mem limit:");
	m_test_memory_limit_input->value(m_options_manager->TestMemoryLimit());

	y += h + 10;

	m_reset_settings_button = new Fl_Button(10, y, 100, h, "Reset settings");
	m_reset_settings_button->callback(ButtonCallback, this);
	m_reset_settings_button->clear_visible_focus();

	m_apply_settings_button = new Fl_Button(120, y, 100, h, "Apply settings");
	m_apply_settings_button->callback(ButtonCallback, this);
	m_apply_settings_button->clear_visible_focus();

	y += h + 10;

	m_window->size(w, y);
	m_window->hide();
	m_window->end();
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

std::string SettingsWindow::SelectDirectory()
{
	TCHAR path[MAX_PATH];
	std::string path_param(m_options_manager->AppPath().c_str());

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

std::string SettingsWindow::SelectFile()
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

	ofn.lpstrInitialDir = m_options_manager->LastExecutableDir().c_str();
	ofn.lpstrTitle = "Select exe-file";
	ofn.lpstrFilter = "Executable .exe\0*.exe\0\0";

	int err = GetOpenFileName(&ofn);
	if (err == 0) return std::string("");

	return std::string(ofn.lpstrFile);
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

	if (button_label == "Apply settings")
	{
		m_options_manager->WorkingDir() = std::string(m_working_dir_selector->value());
		m_options_manager->ProblemDir() = std::string(m_problem_dir_selector->value());

		m_options_manager->CppCompilerDir() = std::string(m_cpp_compiler_dir_selector->value());
		m_options_manager->CCompilerDir() = std::string(m_c_compiler_dir_selector->value());
		m_options_manager->PascalCompilerDir() = std::string(m_pascal_compiler_dir_selector->value());
		m_options_manager->JavaCompilerDir() = std::string(m_java_compiler_dir_selector->value());

		m_options_manager->TestMemoryLimit() = (int)m_test_memory_limit_input->value();
		m_options_manager->ThemeName() = m_options_manager->GetThemeName(m_theme_choice->value());
		m_options_manager->UpdateOptionsFile();

		m_problem_browser_update_needed = true;
		Fl::scheme(m_options_manager->ThemeName().c_str());
		Fl::reload_scheme();
		return;
	}

	if (button_label == "Working dir...")
	{
		m_working_dir_selector->value(SelectDirectory().c_str());
		return;
	}

	if (button_label == "Problem dir...")
	{
		m_problem_dir_selector->value(SelectDirectory().c_str());
		m_problem_browser_update_needed = true;
		return;
	}

	if (button_label == "Cpp compiler...")
	{
		m_cpp_compiler_dir_selector->value(SelectFile().c_str());
		return;
	}

	if (button_label == "C compiler...")
	{
		m_c_compiler_dir_selector->value(SelectFile().c_str());
		return;
	}

	if (button_label == "Pascal compiler...")
	{
		m_pascal_compiler_dir_selector->value(SelectFile().c_str());
		return;
	}

	if (button_label == "Java compiler...")
	{
		m_java_compiler_dir_selector->value(SelectFile().c_str());
		return;
	}
}

void SettingsWindow::UpdateWidgetInfo()
{
	m_problem_dir_selector->value(m_options_manager->ProblemDir().c_str());
	m_working_dir_selector->value(m_options_manager->WorkingDir().c_str());

	m_cpp_compiler_dir_selector->value(m_options_manager->CppCompilerDir().c_str());
	m_c_compiler_dir_selector->value(m_options_manager->CCompilerDir().c_str());
	m_pascal_compiler_dir_selector->value(m_options_manager->PascalCompilerDir().c_str());
	m_java_compiler_dir_selector->value(m_options_manager->JavaCompilerDir().c_str());

	m_theme_choice->value(m_options_manager->GetThemeId());
}

// Main GUI

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

	ofn.lpstrInitialDir = m_options_manager->LastExecutableDir().c_str();
	ofn.lpstrTitle = "Select exe-file";
	ofn.lpstrFilter = "Executable .exe\0*.exe\0\0";

	int err = GetOpenFileName(&ofn);
	if (err == 0) return;

	m_exefile_selector_value->value(ofn.lpstrFile);
	m_options_manager->LastExecutableDir() = std::string(ofn.lpstrFile);
}

void Gui::ButtonClick(Fl_Widget* w)
{
	std::string button_label(w->label());
	if (button_label == "Use only first test") 
	{
		m_first_test_selector->value(true);
		m_all_test_selector->value(false);
		m_options_manager->UseOnlyOneTest() = true;
		m_options_manager->UseMultipleTests() = false;
		return;
	}

	if (button_label == "Use all tests")
	{
		m_first_test_selector->value(false);
		m_all_test_selector->value(true);
		m_options_manager->UseOnlyOneTest() = false;
		m_options_manager->UseMultipleTests() = true;
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
		m_settings_window->Hide();

	for (auto output_window : m_output_windows)
		if(output_window != nullptr)
			output_window->Hide();

	m_options_manager->LastProblem() = m_problem_browser->value();

	m_main_window->hide();
}

void Gui::Initialize()
{	
	m_problem_list = m_problem_manager->GetProblemList();

	int y = 10, h = 20;
	Fl::scheme(m_options_manager->ThemeName().c_str());
	Fl::get_system_colors();
	fl_font(FL_HELVETICA, 16);

	m_main_window = new Fl_Double_Window(0, 0, "Solution Checker");
	m_main_window->callback(WindowCallback, this);

	int selector_spacing = (int)fl_width("Path to exe-file:") + 5;
	m_exefile_selector_value = new Fl_Input(selector_spacing, y, 350, h, "Path to exe-file:");
	m_exefile_selector_value->value(m_options_manager->LastExecutableDir().c_str());
	m_exefile_selector_button = new Fl_Button(selector_spacing + 355, y, 500 - (selector_spacing + 355) - 5, h, "...");
	m_exefile_selector_button->callback(ButtonCallback, this);
	m_exefile_selector_button->clear_visible_focus();

	m_problem_browser = new Fl_Hold_Browser(510, y, 0, 0);
	for (unsigned i = 0; i < m_problem_list.size(); i++)
		m_problem_browser->add(std::string(m_problem_list[i].m_folder_name + ": " + m_problem_list[i].m_caption).c_str());
	m_problem_browser->value(m_options_manager->LastProblem());

	y += h + 10;

	m_first_test_selector = new Fl_Round_Button(5, y, 150, h, "Use only first test");
	m_first_test_selector->callback(ButtonCallback, this);
	m_first_test_selector->clear_visible_focus();
	if (m_options_manager->UseOnlyOneTest())
		m_first_test_selector->set();
	
	y += h + 10;

	m_all_test_selector = new Fl_Round_Button(5, y, 150, h, "Use all tests");
	m_all_test_selector->callback(ButtonCallback, this);
	m_all_test_selector->clear_visible_focus();
	if(m_options_manager->UseMultipleTests())
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
	m_testing_progress->minimum(0);
	m_testing_progress->deactivate();

	y += h + 10;

	m_problem_browser->size(180, y - 20);
	m_main_window->size(700, y);
	m_main_window->position((int)m_options_manager->WindowPosX(), (int)m_options_manager->WindowPosY());

	m_main_window->end();

	m_settings_window = new SettingsWindow(m_options_manager);
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
					m_test_manager->GetResultData(temp_result_data);

					OutputWindow* output_window_ptr = new OutputWindow(temp_result_data);
					output_window_ptr->Show();

					bool f = false;
					for (unsigned int i = 0; i < m_output_windows.size(); i++)
					{
						if (m_output_windows[i] == nullptr)
						{
							m_output_windows[i] = output_window_ptr;

							f = true;
							break;
						}
						else if (!m_output_windows[i]->IsVisible())
						{
							delete m_output_windows[i];

							m_output_windows[i] = output_window_ptr;

							f = true;
							break;
						}
					}

					if (f == false) m_output_windows.push_back(output_window_ptr);


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
			m_problem_manager->ChangeDir(m_options_manager->ProblemDir());
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
		delete m_settings_window;
		m_settings_window = nullptr;
	}

	for (auto output_window : m_output_windows)
		if (output_window != nullptr)
		{
			delete output_window;
			output_window = nullptr;
		}
}