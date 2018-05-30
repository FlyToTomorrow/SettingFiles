/* OPTIONS.C 27/08/98 09.01.34 */
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
WIN_ENTRYSC BOOL WIN_ENTRYMOD FontScalingProc (HWND hDlg, UINT message, WPARAM
	 wParam, LPARAM lParam);
void myitoa (int iInteger, TCHAR *sString);
WORD TextToHex(TCHAR *szString);
WIN_ENTRYSC BOOL WIN_ENTRYMOD ResourceIDProc (HWND hDlg, UINT message, WPARAM
	 wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD FileIDProc (HWND hDlg, UINT message, WPARAM
	 wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD UnmappableCharProc (HWND hDlg, UINT message, WPARAM
	 wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD GetOutputPathProc (HWND hDlg, UINT message, WPARAM
	 wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD AddFontAliasProc (HWND hDlg, UINT message, WPARAM
	 wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD RemoveFontAliasProc (HWND hDlg, UINT message,
	 WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD GetTimezoneProc(HWND hDlg, UINT message, 
	 WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD GetIOBufSizeProc(HWND hDlg, UINT message, 
	 WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD DefaultCharsetProc(HWND hDlg, UINT message, 
	 WPARAM wParam, LPARAM lParam);
VOID BuildFontAliasMap (HANDLE hWnd, PSCCVWFONTALIASMAP pFontAliasMap, BOOL
	 bIsDisplay);
VOID DestroyFontAliasMap (PSCCVWFONTALIASMAP pFontAliasMap);
WIN_ENTRYSC BOOL WIN_ENTRYMOD CustomEmailHeaderProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD NonStandardHeaderProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
VOID CopyByteStringToWordString (WORD *pStr1, BYTE *pStr2);
VOID CopyWordStringToByteString (BYTE *pStr1, WORD *pStr2);
VOID CopyWordStringToWordString (WORD *pStr1, WORD *pStr2);
WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout (HWND hDlg, UINT message, WPARAM wParam
	, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD PropertyDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD PropertyDlgByIDProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD PropertyDlgByIndexProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD InfoMsgDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

VTVOID add_process_to_active_list(PPROCESS_INFORMATION pi, PSCCVWVIEWFILE80 vf, HWND hWnd);
VTVOID remove_active_process_from_list(PCHILD_PROCESS_DATA cd);
VTVOID check_active_child_processes(HWND hWnd);
VTVOID close_child_processes(BOOL bForce);