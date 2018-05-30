/* RFACLNT.C 27/08/98 09.01.34 */
int WINMAIN_ENTRYMOD WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	 LPSTR lpCmdLine, int nCmdShow);
LONG CALLBACK InitDlgProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
LONG CALLBACK OpenRFADlgProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM
	 lParam);
WIN_ENTRYSC LRESULT WIN_ENTRYMOD SccWndProc (HWND hWnd, UINT message, WPARAM
	 wParam, LPARAM lParam);
VOID DoCreate (HWND hWnd);
VOID DoSize (HWND hWnd, WORD wWidth, WORD wHeight);
VOID DoDestroy (HWND hWnd);
VOID DoCloseFile (HWND hWnd);
VOID DoFileChange (HWND hWnd);
VOID DoOpenFile (HWND hWnd);
IO_ENTRYSC IOERR IO_ENTRYMOD myGetRemoteData (HIOFILE hFile, PIORFAREQUEST
	 pRequest, LPVOID *ppData, LPDWORD pdwDataSize);
IO_ENTRYSC IOERR IO_ENTRYMOD myCloseRemoteData (HIOFILE hFile);
IO_ENTRYSC IOERR IO_ENTRYMOD myOpenRemoteSubObject (HIOFILE hFile, VTDWORD
	 dwObjectId, VTDWORD dwStreamId, VTDWORD dwReserved1, VTDWORD
	 dwReserved2, DWORD *pdwSpecType, LPVOID *ppSpec);
DWORD RFAOpenPipe (HANDLE *phPipe, LPSTR szFileName);
DWORD RFAClosePipe (HANDLE hPipe);
DWORD RFASendMessage (HANDLE hPipe, VOID *lpData, DWORD dwDataSize, LPDWORD
	 lpBytesWritten);
DWORD RFAGetMessage (HANDLE hPipe, VOID *lpData, DWORD dwDataSize, LPDWORD
	 lpBytesRead);
WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout (HWND hDlg, UINT message, WPARAM wParam
	, LPARAM lParam);
