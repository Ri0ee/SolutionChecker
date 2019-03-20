#include "problem_creator_menu.h"

void ProblemCreatorWindow::Initialize()
{
	m_window = new Fl_Double_Window(500, 500, "Problem creator");

	m_window->end();
	m_window->hide();
}

void ProblemCreatorWindow::Shutdown()
{

}

void ProblemCreatorWindow::Show()
{
	m_window->show();
}

void ProblemCreatorWindow::Hide()
{
	m_window->hide();
}