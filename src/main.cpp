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

	if(argc == 2 && cmplx::strcmp(argv[1], "install-context")) {

		if(MessageBoxA(0, "Install fastclone into Contextmenu?", "fastclone v1", MB_YESNO | MB_ICONQUESTION) == IDNO) return 0;

		HKEY shell;
		RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Classes\\Directory\\Background\\shell", 0, KEY_ALL_ACCESS, &shell);

		HKEY contextKey;
		RegCreateKeyA(shell, "fastclone", &contextKey);
		RegSetValueExA(contextKey, "Extended", 0, REG_SZ, (LPBYTE) "", 1);
		RegSetValueExA(contextKey, 0, 0, REG_SZ, (LPBYTE) "Fastclone (Git)", 15);

		HKEY command;
		RegCreateKeyA(contextKey, "command", &command);

		char buf[MAX_PATH];
		GetModuleFileNameA(0, buf, MAX_PATH);

		RegSetValueExA(command, 0, 0, REG_SZ, (LPBYTE)buf, cmplx::strlen(buf) + 1);

		return 0;
	}

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
