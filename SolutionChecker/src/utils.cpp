#include "utils.h"



std::string FormatError(DWORD error_code_, const std::string& location_)
{
	return std::string(location_ + ": " + GetErrorMessage(error_code_));
}

std::string GetErrorMessage(DWORD error_code_)
{
	std::string temp_str_buf("");
	LPTSTR error_text = NULL;

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
		error_code_, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&error_text, 0, NULL);

	if (error_text != NULL)
	{
		temp_str_buf = std::string(error_text);
		LocalFree(error_text);
		error_text = NULL;
	}

	return temp_str_buf;
}

void ShowTextFile(const std::string& file_name_)
{
	ShellExecuteA(0, "open", file_name_.c_str(), 0, 0, SW_SHOW);
}

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED) {
		std::string tmp = (const char*)lpData;
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

std::optional<std::string> SelectDirectory(const std::string& title_)
{
	TCHAR path[MAX_PATH];
	std::string path_param("");

	BROWSEINFO bi = { 0 };

	bi.lpszTitle = title_.c_str();

	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)path_param.c_str();

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0) {
		//get the name of the folder and put it in path
		SHGetPathFromIDList(pidl, path);

		//free memory used
		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc))) {
			imalloc->Free(pidl);
			imalloc->Release();
		}

		return std::string(path);
	}

	return std::nullopt;
}

std::optional<std::string> SelectFile(const std::string& title_)
{
	OPENFILENAME ofn = {};

	if (ofn.lpstrFile)
		delete[] ofn.lpstrFile;

	if (ofn.lpstrInitialDir)
		delete[] ofn.lpstrInitialDir;

	memset((void*)&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);

	ofn.Flags |= OFN_NOVALIDATE;
	ofn.Flags |= OFN_HIDEREADONLY;
	ofn.Flags |= OFN_EXPLORER;
	ofn.Flags |= OFN_ENABLESIZING;
	ofn.Flags |= OFN_NOCHANGEDIR;

	ofn.nMaxFile = 4096 - 1;
	ofn.lpstrFile = new char[4096];
	ofn.lpstrFile[0] = 0;
	ofn.hwndOwner = GetForegroundWindow();

	ofn.lpstrInitialDir = "";
	ofn.lpstrTitle = title_.c_str();
	ofn.lpstrFilter = "";

	int err = GetOpenFileName(&ofn);
	if (err == 0)
		return std::nullopt;

	return std::string(ofn.lpstrFile);
}