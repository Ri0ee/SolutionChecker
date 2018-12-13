#include "instance.h"



bool Instance::Initialize(char file_location_[])
{
	std::string file_location(file_location_);
	m_current_dir = file_location.substr(0, file_location.find_last_of('\\', file_location.size()));

	m_options_manager = new OptionsManager();
	m_options_manager->SetOptionsFilePath(m_current_dir + "\\options.txt");
	m_options_manager->GetOptions();

	m_gui_ptr = new Gui();
	if (!m_gui_ptr->Initialize(m_options_manager))
	{
		m_gui_ptr->Shutdown();
		delete m_gui_ptr;
		m_gui_ptr = nullptr;
		return false;
	}

	return true;
}

bool Instance::Run()
{
	m_gui_ptr->Run();
	return true;
}

void Instance::Shutdown()
{
	if (m_options_manager != nullptr)
	{
		m_options_manager->UpdateOptionsFile();
		delete m_options_manager;
		m_options_manager = nullptr;
	}

	if (m_gui_ptr != nullptr)
	{
		m_gui_ptr->Shutdown();
		delete m_gui_ptr;
		m_gui_ptr = nullptr;
	}
}