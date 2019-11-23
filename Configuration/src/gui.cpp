#include "gui.h"

using namespace GUI;

int GuiManager::Run() {
	window_ptr = std::make_shared<MainWindow>(problem_manager, options_manager);
	return window_ptr->Run();
}

int GuiManager::MainWindow::Run() {
	window->show();
	return Fl::run();
}

void GuiManager::MainWindow::Initialize() {
	import_window = std::make_shared<ImportWindow>(problem_manager, options_manager);

	window = new Fl_Double_Window(500, 500, "Configuration");
	window->begin();

	int y = 10;
	int h = 20;
	int inp_spacing = 150;
	int inp_width = 400;

	apply_btn = std::make_shared<ApplyBtn>(10, y, 100, h, [this]() {
		options_manager->SetOption("ProblemDir", problems_dir_inp->GetText());
		options_manager->SetOption("WorkingDir", working_dir_inp->GetText());
		options_manager->SetOption("TempDir", temp_dir_inp->GetText());

		options_manager->SetOption("CppCompilerDir", cpp_compiler_path_inp->GetText());
		options_manager->SetOption("CCompilerDir", c_compiler_path_inp->GetText());
		options_manager->SetOption("PascalCompilerDir", pascal_compiler_path_inp->GetText());
		options_manager->SetOption("JavaCompilerDir", java_compiler_path_inp->GetText());
		options_manager->SetOption("JavaVMDir", java_vm_path_inp->GetText());

		options_manager->SetOption("CppDefaultArgs", cpp_def_args_inp->GetText());
		options_manager->SetOption("CDefaultArgs", c_def_args_inp->GetText());
		options_manager->SetOption("PascalDefaultArgs", pascal_def_args_inp->GetText());
		options_manager->SetOption("JavaDefaultArgs", java_def_args_inp->GetText());

		options_manager->UpdateOptionsFile();
	});

	reset_btn = std::make_shared<ResetBtn>(120, y, 100, h, [this]() {
		options_manager->SetDefaults();

		problems_dir_inp->SetText(options_manager->GetOption("ProblemDir"));
		working_dir_inp->SetText(options_manager->GetOption("WorkingDir"));
		temp_dir_inp->SetText(options_manager->GetOption("TempDir"));

		cpp_compiler_path_inp->SetText(options_manager->GetOption("CppCompilerDir"));
		c_compiler_path_inp->SetText(options_manager->GetOption("CCompilerDir"));
		pascal_compiler_path_inp->SetText(options_manager->GetOption("PascalCompilerDir"));
		java_compiler_path_inp->SetText(options_manager->GetOption("JavaCompilerDir"));
		java_vm_path_inp->SetText(options_manager->GetOption("JavaVMDir"));

		cpp_def_args_inp->SetText(options_manager->GetOption("CppDefaultArgs"));
		c_def_args_inp->SetText(options_manager->GetOption("CDefaultArgs"));
		pascal_def_args_inp->SetText(options_manager->GetOption("PascalDefaultArgs"));
		java_def_args_inp->SetText(options_manager->GetOption("JavaDefaultArgs"));
	});

	import_problem_btn = std::make_shared<ImportProblemBtn>(230, y, 150, h, [this]() { 
		import_window->Show(); 
	});

	import_all_problems_btn = std::make_shared<ImportAllProblemsBtn>(390, y, 150, h, [this]() {
		auto import_directory = SelectDirectory("Select directory to import");
		if (!import_directory.has_value()) return;

		auto export_directory = options_manager->GetOption("ProblemDir");
		if (export_directory.empty()) {
			fl_alert("Problem dir setting is not set");
			return;
		}

		int id = 0;
		std::filesystem::path fsdirectory(import_directory.value());
		for (auto& dir : std::filesystem::directory_iterator(fsdirectory))
		{
			std::cout << "\n" << dir.path().string() << " ";
			if (!dir.is_directory())
				continue;

			if (!std::filesystem::exists(dir.path().string() + "\\Problem.xml"))
				continue;

			if (!Convert1251fileToUTF(dir.path().string() + "\\Problem.xml", dir.path().string() + "\\ProblemUTF.xml"))
				continue;

			PROBLEMS::Problem problem;
			problem.m_internal_id = id;
			problem.m_base_path = dir.path().string();
			if (!problem_manager->ReadProblem(problem, dir.path().string() + "\\ProblemUTF.xml"))
				continue;

			problem_manager->CreateProblem(problem, "ProblemLayout.xml");

			std::filesystem::remove(dir.path().string() + "\\ProblemUTF.xml");

			id++;
			std::cout << "OK";
		}
	});

	y += h + 20;

	working_dir_inp = std::make_shared<WorkingDirInp>(inp_spacing, y, inp_width, h);
	working_dir_inp->SetText(options_manager->GetOption("WorkingDir"));
	y += h + 20;

	problems_dir_inp = std::make_shared<ProblemsDirInp>(inp_spacing, y, inp_width, h);
	problems_dir_inp->SetText(options_manager->GetOption("ProblemDir"));
	y += h + 20;

	temp_dir_inp = std::make_shared<TempDirInp>(inp_spacing, y, inp_width, h);
	temp_dir_inp->SetText(options_manager->GetOption("TempDir"));
	y += h + 30;

	cpp_compiler_path_inp = std::make_shared<CppCompilerPathInp>(inp_spacing, y, inp_width, h);
	cpp_compiler_path_inp->SetText(options_manager->GetOption("CppCompilerDir"));
	y += h + 20;

	c_compiler_path_inp = std::make_shared<CCompilerPathInp>(inp_spacing, y, inp_width, h);
	c_compiler_path_inp->SetText(options_manager->GetOption("CCompilerDir"));
	y += h + 20;

	pascal_compiler_path_inp = std::make_shared<PascalCompilerPathInp>(inp_spacing, y, inp_width, h);
	pascal_compiler_path_inp->SetText(options_manager->GetOption("PascalCompilerDir"));
	y += h + 20;

	java_compiler_path_inp = std::make_shared<JavaCompilerPathInp>(inp_spacing, y, inp_width, h);
	java_compiler_path_inp->SetText(options_manager->GetOption("JavaCompilerDir"));
	y += h + 20;

	java_vm_path_inp = std::make_shared<JavaVMPathInp>(inp_spacing, y, inp_width, h);
	java_vm_path_inp->SetText(options_manager->GetOption("JavaVMDir"));
	y += h + 30;

	cpp_def_args_inp = std::make_shared<CppDefArgsInp>(inp_spacing, y, inp_width, h);
	cpp_def_args_inp->SetText(options_manager->GetOption("CppDefaultArgs"));
	y += h + 20;

	c_def_args_inp = std::make_shared<CDefArgsInp>(inp_spacing, y, inp_width, h);
	c_def_args_inp->SetText(options_manager->GetOption("CDefaultArgs"));
	y += h + 20;

	pascal_def_args_inp = std::make_shared<PascalDefArgsInp>(inp_spacing, y, inp_width, h);
	pascal_def_args_inp->SetText(options_manager->GetOption("PascalDefaultArgs"));
	y += h + 20;

	java_def_args_inp = std::make_shared<JavaDefArgsInp>(inp_spacing, y, inp_width, h);
	java_def_args_inp->SetText(options_manager->GetOption("JavaDefaultArgs"));
	y += h + 20;

	window->end();
	window->size(700, y);
}

void GuiManager::MainWindow::ImportWindow::Initialize() {
	window = new Fl_Double_Window(500, 500, "Problem manager");
	window->begin();

	int y = 10;
	int h = 20;
	int inp_spacing = 150;
	int inp_width = 400;

	import_btn = std::make_shared<ImportBtn>(10, y, 150, h, [this]() {
		PROBLEMS::Problem problem;
		problem.m_base_path = base_dir_inp->GetText();
		problem.m_name = name_inp->GetText();
		problem.m_id = id_inp->GetText();
		problem.m_description_file = description_inp->GetText();
		problem.m_time_limit = std::stoi(time_limit_inp->GetText());
		problem.m_memory_limit = std::stoi(memory_limit_inp->GetText());
		problem.m_input_file = input_file_inp->GetText();
		problem.m_output_file = output_file_inp->GetText();
		problem.m_test_count = std::stoi(test_count_inp->GetText());
		problem.m_checker_exe = checker_exe_inp->GetText();
		problem.m_checker_src = checker_src_inp->GetText();
		problem.m_solution_src = solution_src_inp->GetText();
		problem.m_bonus_points = std::stoi(bonus_points_inp->GetText());
		problem.m_internal_id = problem_manager->GetFreeID();

		std::string answer_files = answer_files_inp->GetText();
		std::string input_files = input_files_inp->GetText();
		std::string points = points_inp->GetText();

		while (!answer_files.empty()) {
			PROBLEMS::Problem::Test test;
			test.m_answer_file = answer_files.substr(0, answer_files.find_first_of(" "));
			test.m_input_file = input_files.substr(0, input_files.find_first_of(" "));
			test.points = std::stoi(points.substr(0, points.find_first_of(" ")));

			answer_files.erase(0, answer_files.find_first_of(" ") + 1);
			input_files.erase(0, input_files.find_first_of(" ") + 1);
			points.erase(0, points.find_first_of(" ") + 1);

			problem.m_tests.push_back(test);
		}

		problem_manager->CreateProblem(problem, "ProblemLayout.xml");
		fl_alert("Problem created successfully");
	});

	import_existing_btn = std::make_shared<ImportExistingBtn>(170, y, 150, h, [this]() {
		auto layout_file = SelectFile("Select problem layout file");
		if (!layout_file.has_value()) return;

		std::string base_path = layout_file.value().substr(0, layout_file.value().find_last_of('\\') + 1);

		PROBLEMS::Problem problem;
		problem.m_base_path = base_path;
		problem.m_internal_id = problem_manager->GetFreeID();

		if (!problem_manager->ReadProblem(problem, layout_file.value()))
			return;

		problem_manager->CreateProblem(problem, "ProblemLayout.xml");

		fl_alert("Successfully imported given problem");
	});

	y += h + 20;

	base_dir_inp = std::make_shared<BaseDirInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	name_inp = std::make_shared<NameInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	id_inp = std::make_shared<IdInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	description_inp = std::make_shared<DescriptionInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	time_limit_inp = std::make_shared<TimeLimitInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	memory_limit_inp = std::make_shared<MemoryLimitInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	input_file_inp = std::make_shared<InputFileInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	output_file_inp = std::make_shared<OutputFileInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	test_count_inp = std::make_shared<TestCountInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	answer_files_inp = std::make_shared<AnswerFilesInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	input_files_inp = std::make_shared<InputFilesInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	checker_exe_inp = std::make_shared<CheckerExeInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	checker_src_inp = std::make_shared<CheckerSrcInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	solution_src_inp = std::make_shared<SolutionSrcInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	points_inp = std::make_shared<PointsInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	bonus_points_inp = std::make_shared<BonusPointsInp>(inp_spacing, y, inp_width, h);
	y += h + 20;

	window->end();
	window->size(700, y);
}