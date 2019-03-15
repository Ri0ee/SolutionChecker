#include "gui.h"



std::string Gui::SelectFile()
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

	ofn.lpstrInitialDir = m_options_manager->LastSolutionDir().c_str();
	ofn.lpstrTitle = "Select exe or source file";
	ofn.lpstrFilter = "Executable .exe\0*.exe\0Source .pas\0*.pas\0Source .cpp\0*.cpp\0Source .c\0*.c\0Source .java\0*.java\0\0";

	int err = GetOpenFileName(&ofn);
	if (err == 0) return std::string("");

	return std::string(ofn.lpstrFile);
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
		if (m_all_test_selector->value() == 0) // Checked
			m_testing_progress->maximum((float)problem.m_test_count);
		else
			m_testing_progress->maximum(1);
		m_testing_progress->value(0);

		m_test_manager->StartTesting(m_problem_browser->value() - 1, std::string(m_solution_selector->value()), m_all_test_selector->value());

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
		m_solution_selector->value(SelectFile().c_str());
		m_options_manager->LastSolutionDir() = std::string(m_solution_selector->value());
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
	m_options_manager->LastSolutionDir() = m_solution_selector->value();

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
	m_solution_selector = new Fl_Input(selector_spacing, y, 350, h, "Path to solution:");
	m_solution_selector->value(m_options_manager->LastSolutionDir().c_str());
	m_solution_file_selector_button = new Fl_Button(selector_spacing + 355, y, 500 - (selector_spacing + 355) - 5, h, "...");
	m_solution_file_selector_button->callback(ButtonCallback, this);
	m_solution_file_selector_button->clear_visible_focus();

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

int Gui::Run()
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

					OutputWindow* output_window_ptr = new OutputWindow(temp_result_data, m_options_manager, &m_output_window_created_file_count);
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
			for (auto problem : m_problem_list)
				m_problem_browser->add(std::string(problem.m_folder_name + ": " + problem.m_caption).c_str());

			m_settings_window->SetProblemBrowserUpdateNeeded(false);
		}
	}

	return 0;
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