#include "windows.h"
#include "instance.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char file_location[_MAX_PATH + 1];
	GetModuleFileName(NULL, file_location, _MAX_PATH);

	Instance current_instance(file_location);
	return current_instance.Run();
}