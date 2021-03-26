#include "comutils.h"
#include "str.h"

const char* GetClipboardText() {
	if(!OpenClipboard(nullptr)) return 0;
	HANDLE hData = GetClipboardData(CF_TEXT);
	if(hData == 0) return 0;
	const char* pszText = (char*)GlobalLock(hData);
	if(pszText == 0) return 0;
	const char* lol = eternalstr(pszText);
	pszText = lol;
	GlobalUnlock(hData);
	CloseClipboard();
	return pszText;
}

int main(int argc, char const* argv[]) {

	Str cd;

	if(argc == 2) cd = argv[1];
	else {
		char* buffer = new char[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, buffer);
		cd = buffer;
		delete[] buffer;
	}

	SetCurrentDirectoryA(cd.c_str());

	cstr clipboard = GetClipboardText();
	if(IDYES == MsgBox(str("do you want to clone '" + clipboard + "' ?"), "fastclone v1", MB_YESNO | MB_ICONQUESTION)) {
		system(str("git clone --recursive " + clipboard));
		Str s = strn(clipboard);
		s.substring(s.lastIndexOf('/') + 1);
		if(s.contains(".git")) s.substring(0, s.lastIndexOf('.'));
		system(str("explorer " + s));
	}
	return 0;
}
