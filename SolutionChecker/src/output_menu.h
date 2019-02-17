#pragma once

#define WIN32
#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Box.H"

#include <string>
#include <vector>

#include "tester.h"



class OutputWindow
{
	struct ListElement
	{
		ListElement(int x_, int y_, std::string& header_, std::string& info_, Fl_Color text_color_) :
			m_x(x_), m_y(y_), m_header(header_), m_info(info_), m_text_color(text_color_) {
			Initialize();
		}

	private:
		void Initialize();

		Fl_Box* m_header_display;
		Fl_Box* m_info_display;
		std::string m_header;
		std::string m_info;
		Fl_Color m_text_color;
		int m_x, m_y;
	};

public:
	OutputWindow(std::vector<Test>& test_result_list_) : m_test_result_list(test_result_list_) { Initialize(); }
	~OutputWindow() {}

	void Show();
	void Hide();

	bool IsVisible() { return m_window->visible(); }

private:
	void Initialize();
	int ResetResultList(std::vector<Test>& test_result_list_);

	Fl_Double_Window* m_window;
	std::vector<Test> m_test_result_list;
	std::vector<ListElement> m_text_unions;
};