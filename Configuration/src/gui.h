#pragma once

#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Input.H"
#include "FL/fl_ask.H"
#include <memory>
#include <functional>
#include <filesystem>
#include <iostream>
#include "options.h"
#include "util.h"
#include "problems.h"



namespace GUI {

	class GenericButton {
	public: 
		void Enable() {
			button->deactivate();
		};

		void Disable() {
			button->activate();
		};

	protected:
		Fl_Button* button;
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
		Fl_Input* input;
	};
	
	class GuiManager {
	public:
		GuiManager(	std::shared_ptr<OPTIONS::OptionsManager> options_manager_, 
					std::shared_ptr<PROBLEMS::ProblemManager> problem_manager_, 
					std::shared_ptr<SERROR::ErrorManager> error_manager_) : 
					options_manager(options_manager_), 
					problem_manager(problem_manager_),
					error_manager(error_manager_) 
		{
			Fl::get_system_colors();
		};

		int Run();

	private:
		std::shared_ptr<OPTIONS::OptionsManager> options_manager;
		std::shared_ptr<PROBLEMS::ProblemManager> problem_manager;
		std::shared_ptr<SERROR::ErrorManager> error_manager;

		class MainWindow {
		public:
			MainWindow(	std::shared_ptr<PROBLEMS::ProblemManager> problem_manager_,
						std::shared_ptr<OPTIONS::OptionsManager> options_manager_) :
						problem_manager(problem_manager_),
						options_manager(options_manager_)
			{ Initialize(); };

			int Run();

		private:
			void Initialize();

			std::shared_ptr<PROBLEMS::ProblemManager> problem_manager;
			std::shared_ptr<OPTIONS::OptionsManager> options_manager;

			class ApplyBtn : public GenericButton {
			public:
				ApplyBtn(int x, int y, int w, int h, std::function<void()> action_) {
					action = action_;
					button = new Fl_Button(x, y, w, h, "Apply settings");
					button->callback(Callback, this);
				};

			private:
				static void Callback(Fl_Widget* w, void* data) { ((ApplyBtn*)data)->action(); }
			};

			class ResetBtn : public GenericButton {
			public:
				ResetBtn(int x, int y, int w, int h, std::function<void()> action_) {
					action = action_;
					button = new Fl_Button(x, y, w, h, "Reset settings");
					button->callback(Callback, this);
				};

			private:
				static void Callback(Fl_Widget* w, void* data) { ((ResetBtn*)data)->action(); }
			};
			
			class ImportProblemBtn : public GenericButton {
			public:
				ImportProblemBtn(int x, int y, int w, int h, std::function<void()> action_) {
					action = action_;
					button = new Fl_Button(x, y, w, h, "Import problem");
					button->callback(Callback, this);
				};

			private:
				static void Callback(Fl_Widget* w, void* data) { ((ImportProblemBtn*)data)->action(); }
			};

			class ImportAllProblemsBtn : public GenericButton {
			public:
				ImportAllProblemsBtn(int x, int y, int w, int h, std::function<void()> action_) {
					action = action_;
					button = new Fl_Button(x, y, w, h, "Import all problems");
					button->callback(Callback, this);
				};

			private:
				static void Callback(Fl_Widget* w, void* data) { ((ImportAllProblemsBtn*)data)->action(); }
			};

			class WorkingDirInp : public GenericInput {
			public:
				WorkingDirInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "Working dir:");
					browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, [this]() {
						auto res = SelectDirectory();
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

			class ProblemsDirInp : public GenericInput {
			public:
				ProblemsDirInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "Problems dir:");
					browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, [this]() {
						auto res = SelectDirectory();
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

			class TempDirInp : public GenericInput {
			public:
				TempDirInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "Temp dir:");
					browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, [this]() {
						auto res = SelectDirectory();
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

			class CppCompilerPathInp : public GenericInput {
			public:
				CppCompilerPathInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "c++ compiler path:");
					browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, [this]() {
						auto res = SelectFile();
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

			class CCompilerPathInp : public GenericInput {
			public:
				CCompilerPathInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "c compiler path:");
					browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, [this]() { 
						auto res = SelectFile();
						if(res.has_value()) SetText(res.value());
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

			class PascalCompilerPathInp : public GenericInput {
			public:
				PascalCompilerPathInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "Pascal compiler path:");
					browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, [this]() {
						auto res = SelectFile();
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

			class JavaCompilerPathInp : public GenericInput {
			public:
				JavaCompilerPathInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "Java compiler path:");
					browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, [this]() {
						auto res = SelectFile();
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

			class JavaVMPathInp : public GenericInput {
			public:
				JavaVMPathInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "Java VM path:");
					browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, [this]() {
						auto res = SelectFile();
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

			class CppDefArgsInp : public GenericInput {
			public:
				CppDefArgsInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "c++ default args:");
				}
			};

			class CDefArgsInp : public GenericInput {
			public:
				CDefArgsInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "c default args:");
				}
			};

			class PascalDefArgsInp : public GenericInput {
			public:
				PascalDefArgsInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "pascal default args:");
				}
			};

			class JavaDefArgsInp : public GenericInput {
			public:
				JavaDefArgsInp(int x, int y, int w, int h) {
					input = new Fl_Input(x, y, w, h, "Java default args:");
				}
			};

			Fl_Double_Window* window;

			std::shared_ptr<ApplyBtn> apply_btn;
			std::shared_ptr<ResetBtn> reset_btn;
			std::shared_ptr<ImportProblemBtn> import_problem_btn;
			std::shared_ptr<ImportAllProblemsBtn> import_all_problems_btn;

			std::shared_ptr<WorkingDirInp> working_dir_inp;
			std::shared_ptr<ProblemsDirInp> problems_dir_inp;
			std::shared_ptr<TempDirInp> temp_dir_inp;

			std::shared_ptr<CppCompilerPathInp> cpp_compiler_path_inp;
			std::shared_ptr<CCompilerPathInp> c_compiler_path_inp;
			std::shared_ptr<PascalCompilerPathInp> pascal_compiler_path_inp;
			std::shared_ptr<JavaCompilerPathInp> java_compiler_path_inp;
			std::shared_ptr<JavaVMPathInp> java_vm_path_inp;

			std::shared_ptr<CppDefArgsInp> cpp_def_args_inp;
			std::shared_ptr<CDefArgsInp> c_def_args_inp;
			std::shared_ptr<PascalDefArgsInp> pascal_def_args_inp;
			std::shared_ptr<JavaDefArgsInp> java_def_args_inp;

			class ImportWindow {
			public:
				ImportWindow(	std::shared_ptr<PROBLEMS::ProblemManager> problem_manager_,
								std::shared_ptr<OPTIONS::OptionsManager> options_manger_) :
								problem_manager(problem_manager_),
								options_manager(options_manger_)
								{ Initialize(); };

				void Show() { window->show(); };
				void Hide() { window->hide(); };

			private:
				void Initialize();

				std::shared_ptr<PROBLEMS::ProblemManager> problem_manager;
				std::shared_ptr<OPTIONS::OptionsManager> options_manager;

				class ImportBtn : public GenericButton {
				public:
					ImportBtn(int x, int y, int w, int h, std::function<void()> action_) {
						action = action_;
						button = new Fl_Button(x, y, w, h, "Import");
						button->callback(Callback, this);
					};

				private:
					static void Callback(Fl_Widget* w, void* data) { ((ImportBtn*)data)->action(); }
				};

				class ImportExistingBtn : public GenericButton {
				public:
					ImportExistingBtn(int x, int y, int w, int h, std::function<void()> action_) {
						action = action_;
						button = new Fl_Button(x, y, w, h, "Import existing layout");
						button->callback(Callback, this);
					};

				private:
					static void Callback(Fl_Widget* w, void* data) { ((ImportExistingBtn*)data)->action(); }
				};

				class ResetFields : public GenericButton {
				public:
					ResetFields(int x, int y, int w, int h, std::function<void()> action_) {
						action = action_;
						button = new Fl_Button(x, y, w, h, "Import existing layout");
						button->callback(Callback, this);
					};

				private:
					static void Callback(Fl_Widget* w, void* data) { ((ResetFields*)data)->action(); }
				};

				class BaseDirInp : public GenericInput {
				public:
					BaseDirInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Base dir:");
						browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, [this]() {
							auto res = SelectDirectory();
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

				class AnswerFilesInp : public GenericInput {
				public:
					AnswerFilesInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Answers:");
						browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, []() {});
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

				class InputFilesInp : public GenericInput {
				public:
					InputFilesInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Input files:");
						browse_btn = std::make_shared<BrowseBtn>(x + w + 10, y, 50, 20, []() {
							SelectMultipleFiles();
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

				class NameInp : public GenericInput {
				public:
					NameInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Name:");
					}
				};

				class IdInp : public GenericInput {
				public:
					IdInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "ID:");
					}
				};

				class DescriptionInp : public GenericInput {
				public:
					DescriptionInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Description:");
					}
				};

				class TimeLimitInp : public GenericInput {
				public:
					TimeLimitInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Time limit:");
					}
				};

				class MemoryLimitInp : public GenericInput {
				public:
					MemoryLimitInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Memory limit:");
					}
				};

				class InputFileInp : public GenericInput {
				public:
					InputFileInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Input file:");
					}
				};

				class OutputFileInp : public GenericInput {
				public:
					OutputFileInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Output file:");
					}
				};

				class TestCountInp : public GenericInput {
				public:
					TestCountInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Test count:");
					}
				};

				class CheckerSrcInp : public GenericInput {
				public:
					CheckerSrcInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Checker src:");
					}
				};

				class CheckerExeInp : public GenericInput {
				public:
					CheckerExeInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Checker exe:");
					}
				};

				class SolutionSrcInp : public GenericInput {
				public:
					SolutionSrcInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Solution src:");
					}
				};

				class PointsInp : public GenericInput {
				public:
					PointsInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Points:");
					}
				};

				class BonusPointsInp : public GenericInput {
				public:
					BonusPointsInp(int x, int y, int w, int h) {
						input = new Fl_Input(x, y, w, h, "Bonus points:");
					}
				};

				Fl_Double_Window* window;

				std::shared_ptr<ImportBtn> import_btn;
				std::shared_ptr<ImportExistingBtn> import_existing_btn;

				std::shared_ptr<BaseDirInp> base_dir_inp;
				std::shared_ptr<NameInp> name_inp;
				std::shared_ptr<IdInp> id_inp;
				std::shared_ptr<DescriptionInp> description_inp;
				std::shared_ptr<TimeLimitInp> time_limit_inp;
				std::shared_ptr<MemoryLimitInp> memory_limit_inp;
				std::shared_ptr<InputFileInp> input_file_inp;
				std::shared_ptr<OutputFileInp> output_file_inp;
				std::shared_ptr<TestCountInp> test_count_inp;
				std::shared_ptr<AnswerFilesInp> answer_files_inp;
				std::shared_ptr<InputFilesInp> input_files_inp;
				std::shared_ptr<CheckerExeInp> checker_exe_inp;
				std::shared_ptr<CheckerSrcInp> checker_src_inp;
				std::shared_ptr<SolutionSrcInp> solution_src_inp;
				std::shared_ptr<PointsInp> points_inp;
				std::shared_ptr<BonusPointsInp> bonus_points_inp;

			};

			std::shared_ptr<ImportWindow> import_window;
		};

		std::shared_ptr<MainWindow> window_ptr;
	};
}