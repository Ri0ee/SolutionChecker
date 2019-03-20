#pragma once

#define WIN32
#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Button.H"



class ProblemCreatorWindow
{
public:
	ProblemCreatorWindow() { Initialize(); }
	~ProblemCreatorWindow() { Shutdown(); }

	void Show();
	void Hide();

private:
	void Initialize();
	void Shutdown();

	Fl_Double_Window* m_window;
};