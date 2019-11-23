#pragma once

#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Round_Button.H"
#include "FL/Fl_Input.H"
#include "FL/Fl_Choice.H"
#include "FL/Fl_Progress.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Scroll.H"
#include "FL/Fl_Hold_Browser.H"
#include "FL/fl_ask.H"
#include "FL/Fl_Value_Input.H"
#include "FL/Fl_Box.H"

#include <shlobj.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>

#include "options.h"
#include "tester.h"
#include "problems.h"
#include "compiler.h"
#include "error_manager.h"


class GenericButton {
public:
	void Enable() {
		button->activate();
	};

	void Disable() {
		button->deactivate();
	};

protected:
	Fl_Button* button = nullptr;
	std::function<void()> action;
};

class GenericInput {
public:
	void Enable() {
		input->activate();
	};

	void Disable() {
		input->deactivate();
	};

	void SetText(const std::string& text_) {
		input->value(text_.c_str());
	}

	std::string GetText() {
		return input->value();
	}

protected:
	Fl_Input* input = nullptr;
};

class GenericRoundBtn {
public:
	void Enable() {
		button->activate();
	};

	void Disable() {
		button->deactivate();
	};

	bool State() {
		return button->value();
	}

	void SetState(bool state_) {
		button->value(state_);
	}

protected:
	Fl_Round_Button* button = nullptr;
	std::function<void()> action;
};

class GenericProgressBar {
public:
	void Enable() {
		progress_bar->activate();
	};

	void Disable() {
		progress_bar->deactivate();
	};

	void SetMaximum(int maximum_) {
		progress_bar->maximum((float)maximum_);
	};

	void SetValue(int value_) {
		progress_bar->value((float)value_);
	}

protected:
	Fl_Progress* progress_bar = nullptr;
};

class GenericHoldBrowser {
public:
	void Enable() {
		browser->activate();
	};

	void Disable() {
		browser->deactivate();
	};

	int GetValue() {
		return browser->value();
	};

	void SetValue(int value_ = 0) {
		browser->value(value_);
	};

	void AddOption(const std::string& option_, unsigned long long id_) {
		browser->add(option_.c_str(), (void*)id_);
	}

protected:
	Fl_Hold_Browser* browser = nullptr;
	std::function<void()> action;
};

class GenericBox {
public:
	void Enable() {
		box->activate();
	};

	void Disable() {
		box->deactivate();
	};

	void SetText(const std::string& text_) {
		box->copy_label(text_.c_str());
	};

	void SetColor(Fl_Color color_) {
		box->labelcolor(color_);
	};

	void SetFont(Fl_Font font_) {
		box->labelfont(font_);
	};

protected:
	Fl_Box* box = nullptr;
};

class GuiManager
{
public:
	GuiManager(	std::shared_ptr<OptionsManager> options_manager_,
				std::shared_ptr<ProblemManager> problem_manager_, 
				std::shared_ptr<TestManager> test_manager_, 
				std::shared_ptr<ErrorManager> error_manager_) :
				m_options_manager(options_manager_), 
				m_problem_manager(problem_manager_), 
				m_test_manager(test_manager_),
				m_error_manager(error_manager_) 
	{
		Fl::scheme(m_options_manager->GetOption("Theme").c_str());
		Fl::get_system_colors();
		fl_font(FL_HELVETICA, 16);
		main_window = std::make_shared<MainWindow>(m_options_manager, m_problem_manager, m_error_manager, m_test_manager);
	}

	int Run();
	
private:
	std::shared_ptr<OptionsManager> m_options_manager;
	std::shared_ptr<ProblemManager> m_problem_manager;
	std::shared_ptr<TestManager> m_test_manager;
	std::shared_ptr<ErrorManager> m_error_manager;

	class MainWindow {
	public:
		MainWindow(	std::shared_ptr<OptionsManager> options_manager_,
					std::shared_ptr<ProblemManager> problem_manager_,
					std::shared_ptr<ErrorManager> error_manager_,
					std::shared_ptr<TestManager> test_manager_) :
					m_options_manager(options_manager_),
					m_problem_manager(problem_manager_),
					m_error_manager(error_manager_),
					m_test_manager(test_manager_) 
					{
						m_problem_list = m_problem_manager->GetProblemList();
						Initialize(); 
					};

		~MainWindow() 
		{
			m_options_manager->SetOption("LastProblem", std::to_string(problem_browser->GetValue()));
			m_options_manager->SetOption("LastSolutionDir", solution_path_inp->GetText());
			m_options_manager->SetOption("UseMultipleTests", all_test_btn->State() ? "1" : "0");
		}

		void Run();
		
	private:
		void Initialize();

		static void WindowCallback(Fl_Widget* w, void* f) { ((MainWindow*)f)->WindowAction(); }
		void WindowAction();

		std::shared_ptr<OptionsManager> m_options_manager;
		std::shared_ptr<ProblemManager> m_problem_manager;
		std::shared_ptr<ErrorManager> m_error_manager;
		std::shared_ptr<TestManager> m_test_manager;

		std::vector<Problem> m_problem_list;
		
		Fl_Double_Window* window;

		class StartTestBtn : public GenericButton {
		public:
			StartTestBtn(int x, int y, int w, int h, std::function<void()> action_) {
				action = action_;
				button = new Fl_Button(x, y, w, h, "Start Testing");
				button->labelfont(FL_BOLD);
				button->labelcolor(FL_DARK_GREEN);
				button->callback(Callback, this);
			};

		private:
			static void Callback(Fl_Widget* w, void* data) { ((StartTestBtn*)data)->action(); }

		};

		class ShowDescriptionBtn : public GenericButton {
		public:
			ShowDescriptionBtn(int x, int y, int w, int h, std::function<void()> action_) {
				action = action_;
				button = new Fl_Button(x, y, w, h, "Show description");
				button->callback(Callback, this);
			};

		private:
			static void Callback(Fl_Widget* w, void* data) { ((ShowDescriptionBtn*)data)->action(); }

		};

		class ShowInfoBtn : public GenericButton {
		public:
			ShowInfoBtn(int x, int y, int w, int h, std::function<void()> action_) {
				action = action_;
				button = new Fl_Button(x, y, w, h, "Show info");
				button->callback(Callback, this);
			};

		private:
			static void Callback(Fl_Widget* w, void* data) { ((ShowInfoBtn*)data)->action(); }

		};

		class OnlyFirstTestBtn : public GenericRoundBtn {
		public:
			OnlyFirstTestBtn(int x, int y, int w, int h, std::function<void()> action_) {
				action = action_;
				button = new Fl_Round_Button(x, y, w, h, "Only first test");
				button->callback(Callback, this);
			};

		private:
			static void Callback(Fl_Widget* w, void* data) { ((OnlyFirstTestBtn*)data)->action(); }

		};

		class AllTestBtn : public GenericRoundBtn {
		public:
			AllTestBtn(int x, int y, int w, int h, std::function<void()> action_) {
				action = action_;
				button = new Fl_Round_Button(x, y, w, h, "Test all");
				button->callback(Callback, this);
			};

		private:
			static void Callback(Fl_Widget* w, void* data) { ((AllTestBtn*)data)->action(); }

		};

		class SolutionPathInp : public GenericInput {
		public:
			SolutionPathInp(int x, int y, int w, int h) {
				input = new Fl_Input(x, y, w, h, "Solution path:");
				browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 40, 20, [this]() {
					auto res = SelectFile("Select solution file");
					if (res.has_value()) SetText(res.value());
				});
			}

		private:
			class BrowseBtn : public GenericButton {
			public:
				BrowseBtn(int x, int y, int w, int h, std::function<void()> action_) {
					action = action_;
					button = new Fl_Button(x, y, w, h, "...");
					button->callback(Callback, this);
				}

			private:
				static void Callback(Fl_Widget* w, void* data) { ((BrowseBtn*)data)->action(); }
			};

			std::shared_ptr<BrowseBtn> browse_btn;
		};

		class TestingProgressBar : public GenericProgressBar {
		public:
			TestingProgressBar(int x, int y, int w, int h) {
				progress_bar = new Fl_Progress(x, y, w, h, "Testing progress");
			};
		};

		class ProblemBrowser : public GenericHoldBrowser {
		public:
			ProblemBrowser(int x, int y, int w, int h) {
				browser = new Fl_Hold_Browser(x, y, w, h, "");
			};
		};

		class OutputWindow {
		public:
			OutputWindow(	std::vector<Test>& test_result_list_, 
							std::shared_ptr<OptionsManager> options_manager_) :
							test_result_list(test_result_list_), 
							options_manager(options_manager_)
							{
								Initialize();
							}

			~OutputWindow() { window->hide(); }

			void Show() { window->show(); };
			void Hide() { window->hide(); };
			bool IsVisible() { return window->visible(); };

		private:
			void Initialize();

			static void WindowCallback(Fl_Widget* w, void* f) { ((OutputWindow*)f)->WindowAction(); }
			void WindowAction();

			std::shared_ptr<OptionsManager> options_manager;
			std::vector<Test> test_result_list;
			Fl_Double_Window* window;
			Fl_Scroll* scroll;

			class ListElement {
			public:
				ListElement(int x_, int y_, Test& test_, std::shared_ptr<OptionsManager> options_manger_) : 
							x(x_), y(y_), test(test_), options_manager(options_manger_) { Initialize(); };

				~ListElement() 
				{
					for (auto& file : created_files) {
						std::error_code errc;
						std::filesystem::remove(file, errc);
					}
				}

			private:
				void Initialize();

				std::shared_ptr<OptionsManager> options_manager;
				Test test;
				int x, y;
				std::vector<std::string> created_files;

				class OpenAnswerBtn : public GenericButton {
				public:
					OpenAnswerBtn(int x, int y, int w, int h, std::function<void()> action_) {
						action = action_;
						button = new Fl_Button(x, y, w, h, "Answer");
						button->callback(Callback, this);
					};

				private:
					static void Callback(Fl_Widget* w, void* data) { ((OpenAnswerBtn*)data)->action(); }

				};

				class OpenInputBtn : public GenericButton {
				public:
					OpenInputBtn(int x, int y, int w, int h, std::function<void()> action_) {
						action = action_;
						button = new Fl_Button(x, y, w, h, "Input");
						button->callback(Callback, this);
					};

				private:
					static void Callback(Fl_Widget* w, void* data) { ((OpenInputBtn*)data)->action(); }

				};

				class OpenOutputBtn : public GenericButton {
				public:
					OpenOutputBtn(int x, int y, int w, int h, std::function<void()> action_) {
						action = action_;
						button = new Fl_Button(x, y, w, h, "Output");
						button->callback(Callback, this);
					};

				private:
					static void Callback(Fl_Widget* w, void* data) { ((OpenOutputBtn*)data)->action(); }

				};

				class HeaderTextBox : public GenericBox {
				public:
					HeaderTextBox(int x, int y, int w, int h, const std::string& text_ = "") {
						box = new Fl_Box(x, y, w, h);
						box->copy_label(text_.c_str());
						box->box(FL_BORDER_BOX);
					};
				};

				class InfoTextBox : public GenericBox {
				public:
					InfoTextBox(int x, int y, int w, int h, const std::string& text_ = "") {
						box = new Fl_Box(x, y, w, h);
						box->copy_label(text_.c_str());
						box->box(FL_BORDER_BOX);
					};
				};

				std::shared_ptr<OpenAnswerBtn> answer_btn;
				std::shared_ptr<OpenInputBtn> input_btn;
				std::shared_ptr<OpenOutputBtn> output_btn;
				std::shared_ptr<HeaderTextBox> header;
				std::shared_ptr<InfoTextBox> info;
			};

			class ResultOverviewBox : public GenericBox {
			public:
				ResultOverviewBox(int x_, int y_, int w_, int h_, const std::string& text_) {
					box = new Fl_Box(x_, y_, w_, h_);
					box->copy_label(text_.c_str());
					box->labelfont(FL_BOLD);
				}
			};

			std::vector<std::shared_ptr<ListElement>> list_elements;
			std::shared_ptr<ResultOverviewBox> result_overview;
		};

		std::shared_ptr<StartTestBtn> start_test_btn;
		std::shared_ptr<ShowDescriptionBtn> show_description_btn;
		std::shared_ptr<ShowInfoBtn> show_info_btn;
		std::shared_ptr<OnlyFirstTestBtn> only_first_test_btn;
		std::shared_ptr<AllTestBtn> all_test_btn;
		std::shared_ptr<SolutionPathInp> solution_path_inp;
		std::shared_ptr<TestingProgressBar> testing_progress_bar;
		std::shared_ptr<ProblemBrowser> problem_browser;

		std::vector<std::shared_ptr<OutputWindow>> output_windows;
	};

	std::shared_ptr<MainWindow> main_window;
};