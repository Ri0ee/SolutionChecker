#include "settings_menu.h"



void SettingsWindow::Initialize()
{
	m_problem_browser_update_needed = false;

	m_problem_importer_window = new ProblemImporterWindow(m_problem_manager);

	int y = 10, h = 20;
	int w = 700;

	double s1 = fl_width("Cpp Compiler:");
	double s2 = fl_width("C Compiler:");
	double s3 = fl_width("Pascal Compiler:");
	double s4 = fl_width("Java Compiler:");
	int compiler_selector_spacing = (int)max(max(s1, s2), max(s3, s4)) + 7;
	int selector_spacing = compiler_selector_spacing;

	s1 = fl_width("Cpp default args:");
	s2 = fl_width("C default args:");
	s3 = fl_width("Pascal default args:");
	s4 = fl_width("Java default args:");
	int compiler_default_arg_spacing = (int)max(max(s1, s2), max(s3, s4)) + 7;

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

	m_temp_dir_selector = new Fl_Input(selector_spacing, y, 400, h, "Temp dir:");
	m_temp_dir_selector->value(m_options_manager->TempDir().c_str());

	m_temp_dir_selector_button = new Fl_Button(selector_spacing + 403, y, w - selector_spacing - 400 - 10, 20, "Temp dir...");
	m_temp_dir_selector_button->callback(ButtonCallback, this);
	m_temp_dir_selector_button->clear_visible_focus();

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

	m_cpp_default_arg_selector = new Fl_Input(compiler_default_arg_spacing, y, w - compiler_default_arg_spacing - 7, h, "Cpp default args:");
	m_cpp_default_arg_selector->value(m_options_manager->CppDefaultArgs().c_str());

	y += h + 10;

	m_c_default_arg_selector = new Fl_Input(compiler_default_arg_spacing, y, w - compiler_default_arg_spacing - 7, h, "C default args:");
	m_c_default_arg_selector->value(m_options_manager->CDefaultArgs().c_str());

	y += h + 10;

	m_pascal_default_arg_selector = new Fl_Input(compiler_default_arg_spacing, y, w - compiler_default_arg_spacing - 7, h, "Pascal default args:");
	m_pascal_default_arg_selector->value(m_options_manager->PascalDefaultArgs().c_str());

	y += h + 10;

	m_java_default_arg_selector = new Fl_Input(compiler_default_arg_spacing, y, w - compiler_default_arg_spacing - 7, h, "Java default args:");
	m_java_default_arg_selector->value(m_options_manager->JavaDefaultArgs().c_str());

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

	m_import_problem_button = new Fl_Button(230, y, 100, h, "Import problem");
	m_import_problem_button->callback(ButtonCallback, this);
	m_import_problem_button->clear_visible_focus();

	y += h + 10;

	m_window->size(w, y);
	m_window->hide();
	m_window->end();
}

void SettingsWindow::Shutdown()
{
	if (m_problem_importer_window)
	{
		m_problem_importer_window->Hide();
		delete m_problem_importer_window;
	}
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

	ofn.lpstrInitialDir = m_options_manager->LastSolutionDir().c_str();
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
		m_options_manager->TempDir() = std::string(m_temp_dir_selector->value());

		m_options_manager->CppCompilerDir() = std::string(m_cpp_compiler_dir_selector->value());
		m_options_manager->CCompilerDir() = std::string(m_c_compiler_dir_selector->value());
		m_options_manager->PascalCompilerDir() = std::string(m_pascal_compiler_dir_selector->value());
		m_options_manager->JavaCompilerDir() = std::string(m_java_compiler_dir_selector->value());

		m_options_manager->CppDefaultArgs() = std::string(m_cpp_default_arg_selector->value());
		m_options_manager->CDefaultArgs() = std::string(m_c_default_arg_selector->value());
		m_options_manager->PascalDefaultArgs() = std::string(m_pascal_default_arg_selector->value());
		m_options_manager->JavaDefaultArgs() = std::string(m_java_default_arg_selector->value());

		m_options_manager->TestMemoryLimit() = (int)m_test_memory_limit_input->value();
		m_options_manager->ThemeName() = m_options_manager->GetThemeName(m_theme_choice->value());
		m_options_manager->UpdateOptionsFile();

		m_problem_browser_update_needed = true;
		Fl::scheme(m_options_manager->ThemeName().c_str());
		Fl::reload_scheme();
		return;
	}

	if (button_label == "Import problem")
	{
		m_problem_importer_window->Show();
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

	if (button_label == "Temp dir...")
	{
		m_temp_dir_selector->value(SelectDirectory().c_str());
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
	m_temp_dir_selector->value(m_options_manager->TempDir().c_str());

	m_cpp_compiler_dir_selector->value(m_options_manager->CppCompilerDir().c_str());
	m_c_compiler_dir_selector->value(m_options_manager->CCompilerDir().c_str());
	m_pascal_compiler_dir_selector->value(m_options_manager->PascalCompilerDir().c_str());
	m_java_compiler_dir_selector->value(m_options_manager->JavaCompilerDir().c_str());

	m_theme_choice->value(m_options_manager->GetThemeId());
}