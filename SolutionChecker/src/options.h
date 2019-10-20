#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "setting_guru.h"



class OptionsManager
{
public:
	OptionsManager(const std::string& path_to_options_file_) : m_path_to_options_file(path_to_options_file_) { GetOptions(); }
	~OptionsManager() { UpdateOptionsFile(); }

	bool GetOptions();
	bool UpdateOptionsFile() { return m_options_manager.Save(m_path_to_options_file); }
	void SetDefaults();

	std::string& AppPath() { return m_app_path; }
	std::string& OptionsFilePath() { return m_path_to_options_file; }

	std::string GetOption(const std::string& key_) { return m_options_manager.Pull(key_).value; }
	void SetOption(const std::string& key_, const std::string& value_) { m_options_manager.Update(key_, value_); }

	std::string GetThemeName(int id_);
	int GetThemeId(const std::string& theme_);
	int GetThemeId() { return GetThemeId(m_options_manager.Pull("Theme").value); }

private:
	std::string m_path_to_options_file;
	std::string m_app_path;

	SG::SettingGuru m_options_manager;
};