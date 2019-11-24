#include "windows.h"
#include "instance.h"



int main() {
	char file_location[_MAX_PATH + 1];
	GetModuleFileName(NULL, file_location, _MAX_PATH);

	Instance current_instance(file_location);
	return current_instance.Run();
}