/* WELCOME.C 27/08/98 09.01.38 */
WIN_ENTRYSC LONG WIN_ENTRYMOD BlandFrameWndProc (register HWND hwnd, UINT msg,
	 register WPARAM wParam, LPARAM lParam);
VOID CommandHandler (HWND hwnd, WPARAM wParam, LPARAM lParam);
VOID CloseAllChildren (VOID);
VOID SendMessageToKids (WORD msg, WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC LRESULT WIN_ENTRYMOD MDIClientSubclass (HWND hwnd, UINT msg, WPARAM
	 wParam, LPARAM lParam);
BOOL InitializeApplication (void);
BOOL InitializeInstance (LPSTR lpCmdLine, int nCmdShow);
HWND DoOpenFile (void);
HWND CreateMDIView (LPSTR szFilename, PSCCVWVIEWFILE40 lpViewFile);
VOID DoCloseFile (HWND hWnd);
HWND GetActiveChild (HWND hMDIClient);
WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout (HWND hDlg, UINT message, WPARAM wParam
	, LPARAM lParam);
