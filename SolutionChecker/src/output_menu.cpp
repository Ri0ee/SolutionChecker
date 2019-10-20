#include "output_menu.h"



void OutputWindow::ListElement::Initialize()
{ 
	m_header = std::to_string(m_test.m_id) + ":";
	if (m_test.m_status & TEST_STATUS_OK)
	{
		m_info.append("TEST_STATUS_OK; ");
		m_text_color = FL_DARK_GREEN;
	}

	if (m_test.m_status & TEST_STATUS_FAIL)
	{
		m_info.append("TEST_STATUS_FAIL; ");
		m_text_color = FL_DARK_RED;
	}

	if (m_test.m_status & TEST_STATUS_MEMORY_LIMIT)
	{
		m_info.append("TEST_STATUS_MEMORY_LIMIT; ");
		m_text_color = FL_DARK_RED;
	}

	if (m_test.m_status & TEST_STATUS_RUNTIME_ERROR)
	{
		m_info.append("TEST_STATUS_RUNTIME_ERROR; ");
		m_text_color = FL_DARK_RED;
	}

	if (m_test.m_status & TEST_STATUS_TIME_LIMIT)
	{
		m_info.append("TEST_STATUS_TIME_LIMIT; ");
		m_text_color = FL_DARK_RED;
	}

	if (m_test.m_status & TEST_STATUS_TESTING_SEQUENCE_ERROR)
	{
		m_info.append("TEST_STATUS_TESTING_SEQUENCE_ERROR; ");
		m_text_color = FL_DARK_YELLOW;
	}

	if (m_test.m_status & TEST_STATUS_UNKNOWN)
	{
		m_info.append("TEST_STATUS_UNKNOWN; ");
		m_text_color = FL_DARK_YELLOW;
	}

	int header_width = (int)fl_width(m_header.c_str());
	int info_width = (int)fl_width(m_info.c_str());
	m_width = header_width + info_width + 360;

	m_header_display = new Fl_Box(m_x, m_y, header_width, 20);
	m_info_display = new Fl_Box(m_x + header_width + 10, m_y, info_width, 20);

	m_header_display->box(FL_BORDER_FRAME);
	m_info_display->box(FL_BORDER_BOX);

	m_header_display->copy_label(m_header.c_str());
	m_info_display->copy_label(m_info.c_str());

	m_header_display->labelcolor(m_text_color);
	m_info_display->labelcolor(m_text_color);

	m_header_display->labelfont(FL_HELVETICA_BOLD);
	m_info_display->labelfont(FL_HELVETICA_BOLD);

	m_show_input_data_button = new Fl_Button(m_x + header_width + info_width + 20, m_y, 100, 20, "Input");
	m_show_input_data_button->clear_visible_focus();
	m_show_input_data_button->callback(ButtonCallback, this);

	m_show_output_data_button = new Fl_Button(m_x + header_width + info_width + 130, m_y, 100, 20, "Output");
	m_show_output_data_button->clear_visible_focus();
	m_show_output_data_button->callback(ButtonCallback, this);

	m_show_destination_data_button = new Fl_Button(m_x + header_width + info_width + 240, m_y, 100, 20, "Answer");
	m_show_destination_data_button->clear_visible_focus();
	m_show_destination_data_button->callback(ButtonCallback, this);
}

void OutputWindow::ListElement::ButtonClick(Fl_Widget* w) 
{
	std::string button_label(w->label());
	if (button_label == "Input")
	{
		std::string dest_filename = m_options_manager->GetOption("WorkingDir");
		dest_filename.append("\\temp_output" + std::to_string(*m_created_file_count) + ".sctmp");
		m_created_file_list.push_back(dest_filename);
		(*m_created_file_count)++;

		std::fstream output_file(dest_filename, std::ios::out);
		if (!output_file.is_open()) return;
		output_file << m_test.m_input_data;
		output_file.close();
		ShowTextFile(dest_filename);

		return;
	}

	if (button_label == "Output")
	{
		std::string dest_filename = m_options_manager->GetOption("WorkingDir");
		dest_filename.append("\\temp_output" + std::to_string(*m_created_file_count) + ".sctmp");
		m_created_file_list.push_back(dest_filename);
		(*m_created_file_count)++;

		std::fstream output_file(dest_filename, std::ios::out);
		if (!output_file.is_open()) return;
		output_file << m_test.m_output_data;
		output_file.close();
		ShowTextFile(dest_filename);

		return;
	}

	if (button_label == "Answer")
	{
		std::string dest_filename = m_options_manager->GetOption("WorkingDir");
		dest_filename.append("\\temp_output" + std::to_string(*m_created_file_count) + ".sctmp");
		m_created_file_list.push_back(dest_filename);
		(*m_created_file_count)++;

		std::fstream output_file(dest_filename, std::ios::out);
		if (!output_file.is_open()) return;
		output_file << m_test.m_destination_data;
		output_file.close();
		ShowTextFile(dest_filename);

		return;
	}
}

void OutputWindow::ListElement::Shutdown()
{
	for (auto filename : m_created_file_list)
		std::remove(filename.c_str());
}

void OutputWindow::Initialize()
{
	m_window = new Fl_Double_Window(500, 520, "Test results");

	ResetResultList(m_test_result_list);

	m_window->size(m_width, m_height);
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

void OutputWindow::ResetResultList(std::vector<Test>& test_result_list_)
{
	int success = 0, points = 0;
	for (auto& test : test_result_list_)
		if (test.m_status & TEST_STATUS_OK) 
		{
			success++;
			points += test.m_points;
		}

	if (success == test_result_list_.size())
		points += test_result_list_[0].m_problem_bonus_points;

	m_height = 10;
	std::string summary_text = std::to_string(success) + " of " + std::to_string(test_result_list_.size()) + " tests were successful, points: " + std::to_string(points);
	int width = (int)fl_width(summary_text.c_str());
	m_summary_box = new Fl_Box(20, m_height, width, 20, "");
	m_summary_box->copy_label(summary_text.c_str());
	m_summary_box->labelfont(FL_HELVETICA_BOLD);
	m_height += 30;

	for (auto test : test_result_list_)
	{
		ListElement* temp_text_union = new ListElement(10, m_height, m_options_manager, test, m_created_file_count);
		m_text_unions.push_back(temp_text_union);
		m_height += 30;

		if (m_width < temp_text_union->width()) m_width = temp_text_union->width();
	}
}

void OutputWindow::Shutdown()
{
	for (auto list_element : m_text_unions) 
		delete list_element;
}