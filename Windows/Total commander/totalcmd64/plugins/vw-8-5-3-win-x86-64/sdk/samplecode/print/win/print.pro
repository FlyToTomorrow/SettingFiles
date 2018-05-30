/* PRINT.C 27/08/98 09.01.34 */
int WINMAIN_ENTRYMOD WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	 LPSTR lpCmdLine, int nCmdShow);
WIN_ENTRYSC LRESULT WIN_ENTRYMOD SccWndProc (HWND hWnd, UINT message, WPARAM
	 wParam, LPARAM lParam);
VOID DoCreate (HWND hWnd);
VOID DoSize (HWND hWnd, WORD wWidth, WORD wHeight);
VOID DoDestroy (HWND hWnd);
VOID DoCloseFile (HWND hWnd);
VOID DoFileChange (HWND hWnd);
VOID DoOpenFile (HWND hWnd);
VOID DoPrintEx (HWND hWnd);
WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout (HWND hDlg, UINT message, WPARAM wParam
	, LPARAM lParam);
