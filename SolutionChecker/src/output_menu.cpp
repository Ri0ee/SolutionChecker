#include "output_menu.h"



void OutputWindow::ListElement::Initialize()
{
	int header_width = (int)fl_width(m_header.c_str());
	int info_width = (int)fl_width(m_info.c_str());
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