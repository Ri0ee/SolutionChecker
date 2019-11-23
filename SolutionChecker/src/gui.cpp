#include "gui.h"



int GuiManager::Run() 
{
	main_window->Run();
	return 0;
}

void GuiManager::MainWindow::Run() 
{
	window->show();

	while (Fl::wait() > 0)
	{
		unsigned int testing_state = m_test_manager->GetTestingState();
		if (testing_state == TESTING_STATE_ONLINE)
		{
			int testing_stage = m_test_manager->GetTestingStage();

			testing_progress_bar->SetValue(testing_stage + 1);
			if (testing_stage == m_problem_list[problem_browser->GetValue() - 1].m_test_count - 1 || (all_test_btn->State() == false && testing_stage == 0))
			{
				m_test_manager->FinishTesting();

				std::vector<Test> result_data;
				m_test_manager->GetResultData(result_data);

				std::shared_ptr<OutputWindow> output_window_ptr = std::make_shared<OutputWindow>(result_data, m_options_manager);
				output_window_ptr->Show();

				bool f = false;
				for (auto& output_window : output_windows)
					if (!output_window->IsVisible())
					{
						output_window.reset();
						output_window = output_window_ptr;
						f = true;
						break;
					}
				if (!f) output_windows.push_back(output_window_ptr);

				problem_browser->Enable();
				start_test_btn->Enable();
				all_test_btn->Enable();
				only_first_test_btn->Enable();

				testing_progress_bar->Disable();
				testing_progress_bar->SetValue(0);
			}
		}
	}
}

void GuiManager::MainWindow::Initialize() {
	window = new Fl_Double_Window(0, 0, "Solution checker");
	window->callback(WindowCallback, this);
	window->begin();
	
	int y = 10;

	solution_path_inp = std::make_shared<SolutionPathInp>(100, y, 350, 20);
	solution_path_inp->SetText(m_options_manager->GetOption("LastSolutionDir"));

	y += 30;

	only_first_test_btn = std::make_shared<OnlyFirstTestBtn>(10, y, 150, 20, [this]() {
		all_test_btn->SetState(false);
		only_first_test_btn->SetState(true);

		m_options_manager->SetOption("UseMultipleTests", "0");
	});
	only_first_test_btn->SetState((m_options_manager->GetOption("UseMultipleTests") == "0") ? true : false);

	y += 30;

	all_test_btn = std::make_shared<AllTestBtn>(10, y, 150, 20, [this]() {
		all_test_btn->SetState(true);
		only_first_test_btn->SetState(false);

		m_options_manager->SetOption("UseMultipleTests", "1");
	});
	all_test_btn->SetState((m_options_manager->GetOption("UseMultipleTests") == "1") ? true : false);

	y += 30;

	start_test_btn = std::make_shared<StartTestBtn>(10, y, 175, 20, [this]() {
		if (problem_browser->GetValue() == 0) return;

		Problem problem = m_problem_list[problem_browser->GetValue() - 1];

		start_test_btn->Disable();
		problem_browser->Disable();
		all_test_btn->Disable();
		only_first_test_btn->Disable();
		testing_progress_bar->Enable();

		if (all_test_btn->State() == true) // Checked
			testing_progress_bar->SetMaximum(problem.m_test_count);
		else
			testing_progress_bar->SetMaximum(1);

		testing_progress_bar->SetValue(0);

		m_test_manager->StartTesting(problem_browser->GetValue() - 1, std::string(solution_path_inp->GetText()), all_test_btn->State());
	});

	show_description_btn = std::make_shared<ShowDescriptionBtn>(195, y, 175, 20, [this]() {
		if (problem_browser->GetValue() == 0) return;
		m_problem_manager->ShowTaskDescription(problem_browser->GetValue() - 1);
	});

	show_info_btn = std::make_shared<ShowInfoBtn>(380, y, 120, 20, [this]() {
		if (problem_browser->GetValue() == 0) return;

		int id = problem_browser->GetValue() - 1;
		std::string message_buffer = 
			"name: " + m_problem_list[id].m_name + "\n" +
			"gid: " + m_problem_list[id].m_id + "\n" +
			"path: " + m_problem_list[id].m_path + "\n" +
			"input file name: " + m_problem_list[id].m_input_file + "\n" +
			"output file name: " + m_problem_list[id].m_output_file + "\n" +
			"test count: " + std::to_string(m_problem_list[id].m_test_count) + "\n" +
			"time limit: " + std::to_string(m_problem_list[id].m_time_limit) + " seconds\n" +
			"memory limit: " + std::to_string(m_problem_list[id].m_memory_limit) + " MB\n";

		fl_alert(message_buffer.c_str());
	});

	y += 30;

	testing_progress_bar = std::make_shared<TestingProgressBar>(10, y, 490, 20);

	problem_browser = std::make_shared<ProblemBrowser>(510, 10, 280, y + 10);
	for (auto problem : m_problem_list)
		problem_browser->AddOption(std::to_string(problem.m_internal_id) + ":" + problem.m_name, problem.m_internal_id);
	problem_browser->SetValue(std::stoi(m_options_manager->GetOption("LastProblem")));

	y += 30;

	window->end();
	window->size(800, y);
}

void GuiManager::MainWindow::WindowAction() 
{
	window->hide();
}

void GuiManager::MainWindow::OutputWindow::Initialize() 
{
	window = new Fl_Double_Window(800, 600, "Results");
	window->callback(WindowCallback, this);
	window->begin();

	int successful_test_count = 0;
	for (auto& test : test_result_list) {
		if (test.m_status & TEST_STATUS_OK)
			successful_test_count++;
	}

	std::string result_overview_text;
	result_overview_text = std::to_string(successful_test_count) + " of " +
		std::to_string(test_result_list.size()) + " tests were successful";
	result_overview = std::make_shared<ResultOverviewBox>(10, 10, 300, 20, result_overview_text);

	scroll = new Fl_Scroll(10, 30, 780, 560);
	scroll->begin();
	int y = 10;
	for (auto& test : test_result_list) {
		std::shared_ptr<ListElement> list_element = std::make_shared<ListElement>(10, y, test, options_manager);
		list_elements.push_back(list_element);
		y += 30;
	}
	scroll->end();

	window->end();
}

void GuiManager::MainWindow::OutputWindow::WindowAction() 
{
	window->hide();
}

void GuiManager::MainWindow::OutputWindow::ListElement::Initialize()
{
	answer_btn = std::make_shared<OpenAnswerBtn>(x, y, 100, 20, [this]() {
		std::string dest_filename = options_manager->GetOption("WorkingDir");
		dest_filename.append("\\temp_answer" + std::to_string(test.m_id) + ".sctmp");
		created_files.push_back(dest_filename);

		std::fstream output_file(dest_filename, std::ios::out);
		if (!output_file.is_open()) return;
		output_file << test.m_destination_data;
		output_file.close();
		ShowTextFile(dest_filename);
	});

	input_btn = std::make_shared<OpenInputBtn>(x + 110, y, 100, 20, [this]() {
		std::string dest_filename = options_manager->GetOption("WorkingDir");
		dest_filename.append("\\temp_input" + std::to_string(test.m_id) + ".sctmp");
		created_files.push_back(dest_filename);

		std::fstream output_file(dest_filename, std::ios::out);
		if (!output_file.is_open()) return;
		output_file << test.m_input_data;
		output_file.close();
		ShowTextFile(dest_filename);
	});

	output_btn = std::make_shared<OpenOutputBtn>(x + 220, y, 100, 20, [this]() {
		std::string dest_filename = options_manager->GetOption("WorkingDir");
		dest_filename.append("\\temp_output" + std::to_string(test.m_id) + ".sctmp");
		created_files.push_back(dest_filename);

		std::fstream output_file(dest_filename, std::ios::out);
		if (!output_file.is_open()) return;
		output_file << test.m_output_data;
		output_file.close();
		ShowTextFile(dest_filename);
	});

	header = std::make_shared<HeaderTextBox>(x + 330, y, 50, 20, std::to_string(test.m_id));

	std::string info_text;
	Fl_Color info_text_color = FL_DARK_GREEN;
	if (test.m_status & TEST_STATUS_OK)
	{
		info_text.append("OK; ");
		info_text_color = FL_DARK_GREEN;
	}

	if (test.m_status & TEST_STATUS_FAIL)
	{
		info_text.append("FAIL; ");
		info_text_color = FL_DARK_RED;
	}

	if (test.m_status & TEST_STATUS_MEMORY_LIMIT)
	{
		info_text.append("MEMORY LIMIT; ");
		info_text_color = FL_DARK_RED;
	}

	if (test.m_status & TEST_STATUS_RUNTIME_ERROR)
	{
		info_text.append("RUNTIME ERROR; ");
		info_text_color = FL_DARK_RED;
	}

	if (test.m_status & TEST_STATUS_TIME_LIMIT)
	{
		info_text.append("TIME LIMIT; ");
		info_text_color = FL_DARK_RED;
	}

	if (test.m_status & TEST_STATUS_TESTING_SEQUENCE_ERROR)
	{
		info_text.append("TESTING SEQUENCE ERROR; ");
		info_text_color = FL_DARK_YELLOW;
	}

	if (test.m_status & TEST_STATUS_UNKNOWN)
	{
		info_text.append("UNKNOWN; ");
		info_text_color = FL_DARK_YELLOW;
	}

	double info_text_width = fl_width(info_text.c_str());
	info = std::make_shared<InfoTextBox>(x + 390, y, info_text_width + 30, 20, info_text);
	info->SetColor(info_text_color);
	info->SetFont(FL_BOLD);
}