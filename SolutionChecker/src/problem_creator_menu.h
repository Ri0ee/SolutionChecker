#pragma once

class ProblemCreatorWindow
{
public:
	ProblemCreatorWindow() { Initialize(); }
	~ProblemCreatorWindow() { Shutdown(); }

private:
	void Initialize();
	void Shutdown();

};