#pragma once

#include <string>
#include <deque>
#include <fstream>

#include "options.h"



enum class Severity
{
	Fatal,
	Info,
	Warning
};

class ErrorManager
{
public:
	struct Error
	{
		std::string message;
		std::string location;
		
		int code;
		double time;
		Severity severity;
	};

	ErrorManager(std::shared_ptr<OptionsManager> options_manager_) : m_options_manager(options_manager_) { Initialize(); }
	~ErrorManager() { Shutdown(); }

	void PushError(Error error_) { m_error_deque.push_back(error_); };
	Error GetError();
	void SaveToFile();
	void Clear() { m_error_deque.clear(); }
	bool CheckForErrors() { return !m_error_deque.empty(); }

private:
	void Initialize();
	void Shutdown();

	std::shared_ptr<OptionsManager> m_options_manager;

	std::deque<Error> m_error_deque;
};