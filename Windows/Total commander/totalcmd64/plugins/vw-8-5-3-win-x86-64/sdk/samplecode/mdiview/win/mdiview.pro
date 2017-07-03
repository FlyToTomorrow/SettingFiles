/* MDIVIEW.C 30/10/98 16.55.18 */
WIN_ENTRYSC LONG WIN_ENTRYMOD BlandFrameWndProc (register HWND hwnd, UINT msg,
	 register WPARAM wParam, LPARAM lParam);
VOID CommandHandler (HWND hwnd, WPARAM wParam, LPARAM lParam);
VOID CloseAllChildren (VOID);
int WINMAIN_ENTRYMOD WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR
	 lpszCmdLine, int nCmdShow);
VOID SendMessageToKids (UINT msg, WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC LRESULT WIN_ENTRYMOD MDIClientSubclass (HWND hwnd, UINT msg, WPARAM
	 wParam, LPARAM lParam);
BOOL InitializeApplication (void);
BOOL InitializeInstance (LPSTR lpCmdLine, int nCmdShow);
HWND MakeNewChild (void);
DWORD MDIViewAs (WORD wMenuId);
WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout (HWND hDlg, UINT message, WPARAM wParam
	, LPARAM lParam);
