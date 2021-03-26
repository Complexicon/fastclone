#define ID_EDIT 150
#define ID_TEXT 200

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	if(message == WM_COMMAND) {
		switch(LOWORD(wParam)) {
		case IDOK: {
			char* buffer = new char[8192];
			GetDlgItemTextA(hwndDlg, ID_EDIT, buffer, 8192);
			EndDialog(hwndDlg, (INT_PTR)buffer);
			break;
		}
		case IDCANCEL: EndDialog(hwndDlg, 0);
		}
	}
	return 0;
}

short* lpwAlign(short* lpIn) {
	unsigned ul = (unsigned)lpIn;
	ul++;
	ul >>= 1;
	ul <<= 1;
	return (short*)ul;
}

void AddItem(short*& begin, short x, short y, short w, short h, short id, int styles, short cls, const char* initialStr) {
	short* pShort = lpwAlign(begin); // Align DLGITEMTEMPLATE on DWORD boundary
	LPDLGITEMTEMPLATE pItem = (LPDLGITEMTEMPLATE)pShort;
	pItem->x = x;
	pItem->y = y;
	pItem->cx = w;
	pItem->cy = h;
	pItem->id = id; // OK button identifier
	pItem->style = WS_CHILD | WS_VISIBLE | styles;

	pShort = (short*)(pItem + 1);
	*pShort++ = 0xFFFF;
	*pShort++ = cls; // Button class
	pShort += 1 + MultiByteToWideChar(CP_ACP, 0, initialStr, -1, (wchar_t*)pShort,
									  MultiByteToWideChar(CP_ACP, 0, initialStr, -1, 0, 0));
	*pShort++ = 0; // No creation data
	begin = pShort;
}

char* DisplayMyMessage(const char* message, const char* title) {
	LPDLGTEMPLATE pDlg = (LPDLGTEMPLATE)malloc(1024);
	LPDLGITEMTEMPLATE pItem;
	short* pShort;
	LRESULT ret;

	memset(pDlg, 0, 1024);

	// Define a dialog box.

	pDlg->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
	pDlg->cdit = 3; // Number of controls
	pDlg->x = 50;
	pDlg->y = 50;
	pDlg->cx = 120;
	pDlg->cy = 50;

	pShort = (short*)(pDlg + 1);
	*pShort++ = 0; // No menu
	*pShort++ = 0; // Predefined dialog box class (by default)
	pShort += 1 + MultiByteToWideChar(CP_ACP, 0, title, -1, (wchar_t*)pShort, MultiByteToWideChar(CP_ACP, 0, title, -1, 0, 0));

	AddItem(pShort, 8, 15, 104, 10, ID_EDIT, WS_BORDER | ES_AUTOHSCROLL, 0x81, "");
	AddItem(pShort, 8, 30, 40, 15, IDOK, BS_DEFPUSHBUTTON, 0x80, "Okay");
	AddItem(pShort, 10, 5, 100, 10, ID_TEXT, SS_LEFT, 0x82, message);

	ret = DialogBoxIndirectParamA(0, pDlg, 0, (DLGPROC)DialogProc, 0);
	free(pDlg);
	return ret == -1 ? 0 : (char*)ret;
}