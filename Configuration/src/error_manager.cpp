#include "error_manager.h"
using namespace SERROR;



void ErrorManager::SaveToFile()
{
	std::fstream output_file(m_options_manager->AppPath() + "log.txt", std::ios::out);

	if (!output_file.is_open()) return;

	for (auto error : m_error_deque)
	{
		std::string severity_message;
		switch (error.severity) {
		case Severity::Fatal: severity_message = "Fatal";
		case Severity::Info: severity_message = "Info";
		case Severity::Warning: severity_message = "Warning";
		default: severity_message = "Undefined";
		}

		output_file << "[" << error.time << "]" << "error code:" << error.code <<
			" error location:" << error.location << " error message:" << error.message <<
			" error severity:" << severity_message << "\n";
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