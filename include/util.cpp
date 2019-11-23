#include "util.h"



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

bool Convert1251fileToUTF(const std::string& file_name_, const std::string& result_file_name_) {
	std::fstream inf(file_name_, std::ios::in), ouf(result_file_name_, std::ios::out);

	if (!inf.is_open() || !ouf.is_open()) return false;
	inf.seekg(0, inf.end);
	int length = inf.tellg();
	std::vector<char> raw_data(length);
	inf.seekg(0, inf.beg);
	inf.read(raw_data.data(), length);

	auto result = ToUTF8(std::string(raw_data.begin(), raw_data.end()), "windows-1251", false);
	if (!result.has_value())
		return false;

	ouf.write(result.value().c_str(), result.value().size());

	return true;
}

std::optional<std::string> ToUTF8(const std::string& source_str, const std::string& charset, bool lowercase)
{
	const std::string::size_type srclen = source_str.size();
	std::vector<UChar> target(srclen);

	UErrorCode status = U_ZERO_ERROR;
	UConverter* conv = ucnv_open(charset.c_str(), &status);
	if (!U_SUCCESS(status))
		return std::nullopt;

	int32_t len = ucnv_toUChars(conv, target.data(), srclen, source_str.c_str(), srclen, &status);
	if (!U_SUCCESS(status))
		return std::nullopt;

	ucnv_close(conv);

	icu::UnicodeString ustr(target.data(), len);

	if (lowercase)
		ustr.toLower();

	std::string retval;
	ustr.toUTF8String(retval);

	return retval;
}

std::vector<std::string> SelectMultipleFiles(const std::string& initial_dir_)
{
	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);

	if (ofn.lpstrFile)
		delete[] ofn.lpstrFile;

	if (ofn.lpstrInitialDir)
		delete[] ofn.lpstrInitialDir;

	ofn.Flags |= OFN_NOVALIDATE;
	ofn.Flags |= OFN_HIDEREADONLY;
	ofn.Flags |= OFN_EXPLORER;
	ofn.Flags |= OFN_ENABLESIZING;
	ofn.Flags |= OFN_ALLOWMULTISELECT;
	ofn.Flags |= OFN_FILEMUSTEXIST;

	ofn.nMaxFile = 4096 - 1;
	ofn.lpstrFile = new char[4096];
	ofn.lpstrFile[0] = 0;
	ofn.hwndOwner = NULL;

	ofn.lpstrInitialDir = initial_dir_.c_str();
	ofn.lpstrTitle = "Select files";

	int err = GetOpenFileName(&ofn);
	if (err == 0) return std::vector<std::string>();

	std::vector<std::string> res;
	char* str = ofn.lpstrFile;
	std::string directory = str;
	str += (directory.length() + 1);
	while (*str) {
		std::string filename = str;
		str += (filename.length() + 1);
		res.push_back(filename);
	}

	return res;
}

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