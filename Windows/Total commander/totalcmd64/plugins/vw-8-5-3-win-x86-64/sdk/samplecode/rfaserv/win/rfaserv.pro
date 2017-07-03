/* RFASERV.C 27/08/98 09.01.36 */
int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR
	 lpCmdLine, int nCmdShow);
LONG CALLBACK MainWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM
	 lParam);
VOID ServerProc (HWND hWnd);
VTHDOC ProcessClientMessage (HWND hWnd, PMYMESSAGEHDR pinMessage, HANDLE hPipe
	, DWORD ClientIndex);
VOID DrawBranch (HDC hDC);
VOID DrawBitmap (HDC hDC, HBITMAP hBitmap, SHORT X, SHORT Y);
