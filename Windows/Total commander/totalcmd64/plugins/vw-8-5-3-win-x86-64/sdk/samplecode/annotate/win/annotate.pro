/* ANNOTATE.C 27/08/98 09.01.20 */
int WINMAIN_ENTRYMOD WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	 LPTSTR lpCmdLine, int nCmdShow);
WIN_ENTRYSC LRESULT WIN_ENTRYMOD SccWndProc (HWND hWnd, UINT message, WPARAM
	 wParam, LPARAM lParam);
VOID DoCreate (HWND hWnd);
VOID DoSize (HWND hWnd, WORD wWidth, WORD wHeight);
VOID DoDestroy (HWND hWnd);
VOID DoCloseFile (HWND hWnd);
VOID DoFileChange (HWND hWnd);
VOID DoOpenFile (HWND hWnd);
VOID DoRawText (HWND hWnd, DWORD dwCharCount);
BOOL isLeadByte(DWORD dwCharSet, BYTE bChar);
WORD nextChar(DWORD dwCharSet, LPBYTE *p);
LPBYTE DoMatchIt (DWORD dwCharSet, LPBYTE pBuffer, LPBYTE pText, LPDWORD pCharCount, LPDWORD
	 pMatchLen);
VOID DoAnnotationEvent (HWND hWnd, PSCCVWANNOTATIONEVENT40 pEvent);
VOID DoGoto (HWND hWnd, WORD wId);
VOID DoOpenAnnotate (HWND hWnd);
WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout (HWND hDlg, UINT message, WPARAM wParam
	, LPARAM lParam);
