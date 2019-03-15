#include "options.h"



bool OptionsManager::GetOptions()
{
	if (m_path_to_options_file.empty()) return false;

	std::fstream in_file;
	in_file.open(m_path_to_options_file, std::ios::in);
	if (!in_file.is_open())
	{
		SetDefaults();
		return false;
	}

	std::string line;
	while (std::getline(in_file, line))
	{
		int delim_pos = line.find_first_of(":");
		std::string param_name = line.substr(0, delim_pos);
		
		if (param_name == "WindowPosX")
		{
			std::stringstream param_value(line.substr(delim_pos + 1, line.size()));
			float window_pos_x = 0;
			param_value >> window_pos_x;
			m_window_pos_x = window_pos_x;
			continue;
		}

		if (param_name == "WindowPosY")
		{
			std::stringstream param_value(line.substr(delim_pos + 1, line.size()));
			float window_pos_y = 0;
			param_value >> window_pos_y;
			m_window_pos_y = window_pos_y;
			continue;
		}

		if (param_name == "WorkingDir")
		{
			m_working_dir = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "LastProblem")
		{
			std::stringstream param_value(line.substr(delim_pos + 1, line.size()));
			int last_problem = 0;
			param_value >> last_problem;
			m_last_problem = last_problem;
			continue;
		}

		if (param_name == "ProblemDir")
		{
			m_problem_dir = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "LastExecutableDir")
		{
			m_last_exe_dir = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "UseOnlyOneTest")
		{
			std::string temp_param_value = line.substr(delim_pos + 1, line.size());
			if (temp_param_value == "true")
			{
				m_use_only_one_test = true;
				m_use_multiple_tests = false;
			}
			else
			{
				m_use_only_one_test = false;
				m_use_multiple_tests = true;
			}
			continue;
		}

		if (param_name == "Theme")
		{
			m_theme = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "TestMemoryLimit")
		{
			std::stringstream param_value(line.substr(delim_pos + 1, line.size()));
			int test_memory_limit = 0;
			param_value >> test_memory_limit;
			m_test_memory_limit = test_memory_limit;
			continue;
		}

		if (param_name == "CppCompilerDir")
		{
			m_cpp_compiler_dir = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "CCompilerDir")
		{
			m_c_compiler_dir = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "PascalCompilerDir")
		{
			m_pascal_compiler_dir = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "JavaCompilerDir")
		{
			m_java_compiler_dir = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "CppDefaultArgs")
		{
			m_cpp_default_args = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "CDefaultArgs")
		{
			m_c_default_args = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "PascalDefaultArgs")
		{
			m_pascal_default_args = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "JavaDefaultArgs")
		{
			m_java_default_args = line.substr(delim_pos + 1, line.size());
			continue;
		}

		if (param_name == "TempDir")
		{
			m_temp_dir = line.substr(delim_pos + 1, line.size());
			continue;
		}
	}

	in_file.close();
	
	return true;
}

void OptionsManager::SetDefaults()
{
	m_window_pos_x = 100;
	m_window_pos_y = 100;
	m_last_exe_dir = "";
	m_last_problem = 0;
	m_problem_dir = "";
	m_working_dir = "";
	m_temp_dir = "";

	m_cpp_compiler_dir = "";
	m_c_compiler_dir = "";
	m_pascal_compiler_dir = "";
	m_java_compiler_dir = "";

	m_cpp_default_args = "";
	m_c_default_args = "";
	m_pascal_default_args = "";
	m_java_default_args = "";

	m_theme = "none";
	m_use_only_one_test = false;
	m_test_memory_limit = 500;
}

bool OptionsManager::UpdateOptionsFile()
{
	if (m_path_to_options_file.empty()) return false;

	std::fstream out_file;
	out_file.open(m_path_to_options_file, std::ios::out);
	if (!out_file.is_open()) return false;

	out_file << "WindowPosX:" << m_window_pos_x << "\n";
	out_file << "WindowPosY:" << m_window_pos_y << "\n";
	out_file << "WorkingDir:" << m_working_dir << "\n";
	out_file << "ProblemDir:" << m_problem_dir << "\n";
	out_file << "TempDir:" << m_temp_dir << "\n";
	out_file << "LastExecutableDir:" << m_last_exe_dir << "\n";
	out_file << "LastProblem:" << m_last_problem << "\n";
	out_file << "UseOnlyOneTest:" << (m_use_only_one_test ? "true" : "false") << "\n";
	out_file << "Theme:" << m_theme << "\n";
	out_file << "TestMemoryLimit:" << m_test_memory_limit << "\n";

	out_file << "CppCompilerDir:" << m_cpp_compiler_dir << "\n";
	out_file << "CCompilerDir:" << m_c_compiler_dir << "\n";
	out_file << "PascalCompilerDir:" << m_pascal_compiler_dir << "\n";
	out_file << "JavaCompilerDir:" << m_java_compiler_dir << "\n";

	out_file << "CppDefaultArgs:" << m_cpp_default_args << "\n";
	out_file << "CDefaultArgs:" << m_c_default_args << "\n";
	out_file << "PascalDefaultArgs:" << m_pascal_default_args << "\n";
	out_file << "JavaDefaultArgs:" << m_java_default_args << "\n";

	out_file.close();

	return true;
}

std::string OptionsManager::GetThemeName(int id_)
{
	switch (id_)
	{
	case 0:		return std::string("none");
	case 1:		return std::string("gtk+");
	case 2:		return std::string("gleam");
	case 3:		return std::string("plastic");
	default:	return std::string("none");
	}
}

int OptionsManager::GetThemeId(const std::string& theme_)
{
	if (theme_ == "none") return 0;
	else if (theme_ == "gtk+") return 1;
	else if (theme_ == "gleam") return 2;
	else if (theme_ == "plastic") return 3;
	else return 0;
}