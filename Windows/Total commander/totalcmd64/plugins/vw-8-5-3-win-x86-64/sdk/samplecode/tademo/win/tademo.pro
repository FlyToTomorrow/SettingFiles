/* TADEMO.C 27/08/98 09.01.38 */
int WINMAIN_ENTRYMOD WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	 LPSTR lpszCmdLine, int nCmdShow);
WIN_ENTRYSC LRESULT WIN_ENTRYMOD MainWndProc (HWND hwnd, UINT message, WPARAM
	 wParam, LPARAM lParam);
long PASCAL DisplayWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM
	 lParam);
DAERR DoTextOpen (HWND hwnd);
void DoTextClose (void);
DAERR DoTextBlockAccess (HWND hwnd, WORD wType);
int PASCAL SizeDlg (HWND hDlg, WORD wMsg, WORD wParam, LONG lParam);
BOOL PASCAL AboutDlgProc (HWND hDlg, unsigned message, WORD wParam, LONG
	 lParam);
WORD DoOpenFileDlg (LPSTR szFileName, HWND hWnd, LPSTR szCaption);
VOID DoSaveTextAs (HWND hwnd);
LPSTR lstrrchr (LPSTR str, char ch);
