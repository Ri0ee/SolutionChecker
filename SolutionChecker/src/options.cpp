#include "options.h"



bool OptionsManager::GetOptions()
{
	if (!m_options_manager.Reload(m_path_to_options_file))
		SetDefaults();
	
	return true;
}

void OptionsManager::SetDefaults()
{
	m_options_manager.Update("WindowPosX", "100");
	m_options_manager.Update("WindowPosY", "100");

	m_options_manager.Update("WorkingDir", "");
	m_options_manager.Update("ProblemDir", "");
	m_options_manager.Update("TempDir", "");
	m_options_manager.Update("LastSolutionDir", "");

	m_options_manager.Update("LastProblem", "0");
	m_options_manager.Update("UseMultipleTests", "1");
	m_options_manager.Update("Theme", "none");
	m_options_manager.Update("TestMemoryLimit", "512");

	m_options_manager.Update("CppCompilerDir", "");
	m_options_manager.Update("CCompilerDir", "");
	m_options_manager.Update("PascalCompilerDir", "");
	m_options_manager.Update("JavaCompilerDir", "");
	m_options_manager.Update("JavaVMPath", "");

	m_options_manager.Update("CppDefaultArgs", "");
	m_options_manager.Update("CDefaultArgs", "");
	m_options_manager.Update("PascalDefaultArgs", "");
	m_options_manager.Update("JavaDefaultArgs", "");

	m_options_manager.Update("AutofillInputFile", "input.txt");
	m_options_manager.Update("AutofillOutputFile", "output.txt");
	m_options_manager.Update("AutofillDescriptionFile", "Description.txt");
	m_options_manager.Update("AutofillCheckerSrc", "Checker.pas");
	m_options_manager.Update("AutofillCheckerExe", "Checker.exe");
	m_options_manager.Update("AutofillSolution", "Solution.pas");
	m_options_manager.Update("AutofillPoints", "1");
	m_options_manager.Update("AutofillBonusPoints", "1");
	m_options_manager.Update("AutofillTimeLimit", "1.5");
	m_options_manager.Update("AutofillMemLimit", "512");
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