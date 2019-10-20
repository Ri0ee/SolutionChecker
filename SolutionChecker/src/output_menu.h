#pragma once

#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Button.H"

#include <string>
#include <vector>
#include <fstream>

#include "tester.h"
#include "options.h"
#include "utils.h"



class OutputWindow
{
	struct ListElement
	{
		ListElement(int x_, int y_, std::shared_ptr<OptionsManager> options_manager_, const Test& test_, int* created_file_count_) :
			m_x(x_), m_y(y_), m_options_manager(options_manager_), m_test(test_), m_created_file_count(created_file_count_) {
			Initialize();
		}
		~ListElement() { Shutdown(); }

		int width()
		{
			return m_width;
		}

	private:
		void Initialize();
		void Shutdown();

		static void ButtonCallback(Fl_Widget* w, void* f) { ((OutputWindow::ListElement*)f)->ButtonClick(w); }
		void ButtonClick(Fl_Widget* w);

		std::shared_ptr<OptionsManager> m_options_manager;

		Fl_Box*		m_header_display;
		Fl_Box*		m_info_display;
		Fl_Button*	m_show_input_data_button;
		Fl_Button*	m_show_output_data_button;
		Fl_Button*	m_show_destination_data_button;

		std::vector<std::string> m_created_file_list;

		std::string m_header;
		std::string m_info;
		Fl_Color m_text_color = FL_DARK_GREEN;

		Test m_test;
		int m_x = 0, m_y = 0;
		int m_width = 0;
		int* m_created_file_count;
	};

public:
	OutputWindow(std::vector<Test>& test_result_list_, std::shared_ptr<OptionsManager> options_manager_, int* created_file_count_) :
		m_test_result_list(test_result_list_), m_options_manager(options_manager_), m_created_file_count(created_file_count_) { 
		Initialize(); 
	}
	~OutputWindow() { Shutdown(); }

	void Show();
	void Hide();

	bool IsVisible() { return m_window->visible(); }

private:
	void Initialize();
	void Shutdown();
	void ResetResultList(std::vector<Test>& test_result_list_);

	std::shared_ptr<OptionsManager> m_options_manager;

	Fl_Double_Window* m_window;
	Fl_Box* m_summary_box;

	int* m_created_file_count = nullptr;
	int m_width = 0, m_height = 0;
	std::vector<Test> m_test_result_list;
	std::vector<ListElement*> m_text_unions;
};