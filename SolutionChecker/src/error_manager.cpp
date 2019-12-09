#include "error_manager.h"



void ErrorManager::SaveToFile()
{
	std::fstream output_file(m_options_manager->AppPath() + "\\log.txt", std::ios::out);
	if (!output_file.is_open()) return;

	for (auto error : m_error_deque)
	{
		std::string severity_message;
		switch (error.severity) {
		case Severity::Fatal:
		{
			severity_message = "Fatal";
			break;
		}
		case Severity::Info: 
		{
			severity_message = "Info";
			break;
		}
		case Severity::Warning: 
		{
			severity_message = "Warning";
			break; 
		}
		default: severity_message = "Undefined";
		}

		output_file << "[" << error.time << "]" << " [Error code]: " << error.code <<
			" [Error location]: " << error.location << " [Error message]: " << error.message << "\n";
	}

	output_file.close();
}

ErrorManager::Error ErrorManager::GetError()
{
	if (m_error_deque.empty()) return {};

	Error temp_error = *m_error_deque.begin();
	m_error_deque.pop_front();
	return temp_error;
}