#include "windows.h"
#include "instance.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char file_location[_MAX_PATH + 1];
	GetModuleFileName(NULL, file_location, _MAX_PATH);

	Instance* current_instance = new Instance(file_location);
	bool ret_state = current_instance->Run();
	delete current_instance;

	return ret_state;
}