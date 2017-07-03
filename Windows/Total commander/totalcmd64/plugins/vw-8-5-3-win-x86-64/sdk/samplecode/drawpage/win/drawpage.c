/*
|  Outside In Viewer Technology
|  Sample Application
|  Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.
|
|  DRAWPAGE
|  Viewer Technology sample application
|
|  Purpose:
|  Show how to use the Draw Page feature of the view window to display
|  thumbnails of documents and write pages to bitmaps.
| 
|  You have a royalty-free right to use, modify, reproduce and
|  distribute the Sample Applications (and/or any modified version)
|  in any way you find useful, provided that you agree that
|  Oracle has no warranty obligations or liability for any
|  Sample Application files which are modified.
*/

/*
|
|  Defines
|
*/

#define DEFAULTOEMID  "99"
//#define DRAWPAGETEST

#define WIN_ENTRYSC __declspec(dllexport)
#define WIN_ENTRYMOD __stdcall
#define WINMAIN_ENTRYMOD APIENTRY

/*
|
|  Includes
|  Includes
|  Includes
|
*/

#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sccvw.h"

#include "dib.h"

#include "drawpage.h"
#include "drawpage.pro"
#include "verbuild.h"

/*
|
|  Command line arguments
|  Command line arguments
|  Command line arguments
|
*/

#define ARGC  __argc
#define ARGV(x)  (__argv[x])

#define SETWINDOWEXT(a,b,c) SetWindowExtEx(a,b,c,NULL)
#define SETVIEWPORTEXT(a,b,c) SetViewportExtEx(a,b,c,NULL)
#define SETWINDOWORG(a,b,c) SetWindowOrgEx(a,b,c,NULL)
#define SETVIEWPORTORG(a,b,c) SetViewportOrgEx(a,b,c,NULL)

#define SCC_INVALIDPAGE  0xffffffff

/* Page width and height magic numbers. */

#define PAGE_WIDTH	12240	// 8.5 inches
#define PAGE_HEIGHT	15840	// 11 inches
#define PAGE_WIDTH_MINUS_MARGIN		10800	// 7.5 inches (8.5 with 0.5 left and right margins)
#define PAGE_HEIGHT_MINUS_MARGIN	14400	// 10 inches (11 with 0.5 top and bottom margins)

RGBQUAD colorTable[] =    {
                            { 0x00, 0x00, 0x00, 0x00 }, // RGB Place Holder
                            { 0x00, 0x00, 0x00, 0x01 }, // Black
                            { 0x00, 0x00, 0xff, 0x02 }, // Red
                            { 0x00, 0xff, 0x00, 0x03 }, // Green
                            { 0xff, 0x00, 0x00, 0x04 }, // Blue
                            { 0x00, 0xff, 0xff, 0x05 }, // Yellow
                            { 0xff, 0xff, 0x00, 0x06 }, // Cyan
                            { 0xff, 0x00, 0xff, 0x07 }, // Magenta
                            { 0xff, 0xff, 0xff, 0x08 }, // White
                            { 0x80, 0x80, 0x80, 0x09 }, // Gray
                            { 0x00, 0x00, 0x80, 0x0a }, // DkRed
                            { 0x00, 0x80, 0x00, 0x0b }, // DkGreen
                            { 0x80, 0x00, 0x00, 0x0c }, // DkBlue
                            { 0x00, 0x80, 0x80, 0x0d }, // DkYellow
                            { 0x80, 0x80, 0x00, 0x0e }, // DkCyan
                            { 0x80, 0x00, 0x80, 0x0f }, // DkMagenta
                            { 0xc0, 0xc0, 0xc0, 0x10 }, // LtGray
                        };

char *colorNames[] =    {
                            "RGB 0x00000000",
                            "Black",
                            "Red",
                            "Green",
                            "Blue",
                            "Yellow",
                            "Cyan",
                            "Magenta",
                            "White",
                            "Gray",
                            "DkRed",
                            "DkGreen",
                            "DkBlue",
                            "DkYellow",
                            "DkCyan",
                            "DkMagenta",
                            "LtGray"
                        };
#define NUM_COLORNAMES (sizeof(colorNames)/sizeof(char*))

        
/*
|
|  Globals
|  Globals
|  Globals
|
*/

HANDLE      hInst;             /* Handle of the current instance */
HWND        hMainWnd;          /* Handle to top level window */
HWND        hViewWnd = NULL;   /* Handle to the view window */
char        szExePath[256];    /* Path to this executable */
HANDLE      hViewerLibrary;    /* Handle to SCCVW.DLL */
DWORD       dwCurrentPage;
DWORD       dwNewPage;
DWORD       dwGoToPage;
DWORD       dwMaxPage;
BOOL        bMaxFound;
BOOL        bFileOpen;
LONG        lPageWidth;
LONG        lPageHeight;

RGBQUAD     fColor = { 0x00, 0xff, 0xff, 0x05 }; // Yellow
RGBQUAD     bColor = { 0xff, 0x00, 0x00, 0x04 }; // Blue
RGBQUAD     dialogColor;
char        *dialogColorName;

BOOL        bHaveAnnos;
BOOL        bDisplay = TRUE;
BOOL        bWorking = FALSE;
BOOL        bHandleNoBlank;
BOOL        bSinglePageHtmlEmail = FALSE;

HWND hRightWnd = NULL;
char  locChildClassName[256] = "leftWindowClass";
BITMAP bm = {0};
HBITMAP  hCurBitmap = NULL;
char *defStr = "OIT Page Sample";

/*
|
|  Routines
|  Routines
|  Routines
|
*/

LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
VOID DoChildPaint(HWND hWnd);
VOID DoSaveFile(HWND hWnd,WORD wId);
VOID DoSavePageToBitmap(WORD wFormat, HWND hWnd, LPTSTR filename);
VOID DoSavePageToMetafile(WORD wFormat, HWND hWnd, LPTSTR filename);
VOID DrawPageErrorBox(LRESULT lResult, DWORD dwPage);

INT_PTR CALLBACK GoToDialogProc(
  HWND hwndDlg,  // handle to dialog box
  UINT uMsg,     // message
  WPARAM wParam, // first message parameter
  LPARAM lParam  // second message parameter
);

INT_PTR CALLBACK ColorDialogProc(
  HWND hwndDlg,  // handle to dialog box
  UINT uMsg,     // message
  WPARAM wParam, // first message parameter
  LPARAM lParam  // second message parameter
);

static VOID DoPrintDP(void);
static VOID PrintDoc(HDC hDCPrinter, HDC hICPrinter);
WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

VOID DoHighlight(HWND hWnd);
VOID SetHighlightColors(VOID);
VOID  DoHideSelection( HWND hWnd );
VOID  ResetDrawPage();

#include "scclink.c"

int WINMAIN_ENTRYMOD WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG  locMsg;
	HACCEL hAccelTable;
	bFileOpen = FALSE;
	lPageWidth = lPageHeight = 0;
	bHandleNoBlank = FALSE;

    UNUSED(lpCmdLine);
    UNUSED(nCmdShow);

	/*
	| Register window class if necessary
	*/
	if (!hPrevInstance)
	{
		WNDCLASS WndClass;

		WndClass.style = 0;
		WndClass.lpfnWndProc = SccWndProc;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hInstance = hInstance;
		WndClass.hIcon = LoadIcon(hInstance,"SCC_ICON");
		WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
		WndClass.hbrBackground = GetStockObject(LTGRAY_BRUSH);
		WndClass.lpszMenuName = (LPSTR) "SCC_MENU";
		WndClass.lpszClassName = (LPSTR) "SCC_MAIN";

		if (!RegisterClass(&WndClass))
			return(0);

		WndClass.lpfnWndProc   = ChildWndProc;
		WndClass.hIcon         = NULL;
		WndClass.lpszClassName = locChildClassName;
		if (!RegisterClass (&WndClass))
			return(0);
	}

	/*
	| Save instance in global
	*/

	hInst = hInstance;

	
	/*
	| Create main window
	*/

	hMainWnd = CreateWindow(
						(LPSTR) "SCC_MAIN",                      /* window class    */
						"OIVT Page Sample",                       /* window name      */
						WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,   /* window type     */
						CW_USEDEFAULT,                           /* x position       */
						CW_USEDEFAULT,                           /* y position     */
						CW_USEDEFAULT,                           /* width           */
						CW_USEDEFAULT,                           /* height        */
						NULL,                                    /* parent handle   */
						NULL,                                    /* menu or child ID*/
						hInstance,                               /* instance        */
						NULL);                                   /* additional info */

	/*
	|  Confirm window was created
	*/

	if (!hMainWnd)
		return (0);

	ShowWindow(hMainWnd,SW_SHOW);
	UpdateWindow(hMainWnd);

	/*
	|  GetMessage loop
	*/

	hAccelTable = LoadAccelerators(hInstance, "SCC_MENU");
	while (GetMessage(&locMsg,NULL,0,0))
	{
		if (!TranslateAccelerator(hMainWnd, hAccelTable, &locMsg))
		{
			TranslateMessage(&locMsg);
			DispatchMessage(&locMsg);
		}
	}

	DestroyAcceleratorTable(hAccelTable);

	return (int)(locMsg.wParam);     /* Returns the value from PostQuitMessage */
}


WIN_ENTRYSC LRESULT WIN_ENTRYMOD SccWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WORD locId;
	WORD locEvent;
	HWND  locCtrl;

	switch (message)
	{
		case WM_DESTROY:

			DoDestroy(hWnd);
			PostQuitMessage(0);
			break;

		case WM_CLOSE:

			DestroyWindow(hWnd);
			break;

		case WM_CREATE:

			DoCreate(hWnd);
			break;

		case WM_SIZE:

			if (wParam != SIZEICONIC)
				DoSize(hWnd,LOWORD(lParam),HIWORD(lParam));
			break;

		case WM_PAINT:
			{
			PAINTSTRUCT locPS={0};
			BeginPaint(hWnd,&locPS);
			EndPaint(hWnd,&locPS);
			}
			UpdateWindow(hViewWnd);
			UpdateWindow(hRightWnd);
			break;

		case WM_PALETTECHANGED:
		case WM_SYSCOLORCHANGE:
		case WM_QUERYNEWPALETTE:
		case WM_SETFOCUS:

			if (IsWindow(hViewWnd))
				return(SendMessage(hViewWnd,message,wParam,lParam));
			break;

		case SCCVW_FILECHANGE:

			DoFileChange(hWnd);
			break;

		case SCCVW_OPTIONCHANGE:
			/*
			|  Some options will affect the appearance of the drawtorect window.
			|  Refresh the window whenever one of these options changes.
			*/
			switch(lParam)
			{
				case SCCID_BMPFITMODE:
				case SCCID_ARCSORTORDER:
				case SCCID_BMPDITHER:
				case SCCID_BMPPRINTASPECT:
				case SCCID_BMPPRINTBORDER:
				case SCCID_DAYNAMES:
				case SCCID_DBPRINTGRIDLINES:
				case SCCID_DBPRINTHEADINGS:
				case SCCID_DEFAULTPRINTFONT:
				case SCCID_DEFAULTPRINTMARGINS:
				case SCCID_INTLFLAGS:
				case SCCID_MONTHNAMES:
				case SCCID_NUMBERFORMAT:
				case SCCID_SSPRINTGRIDLINES:
				case SCCID_SSPRINTHEADINGS:
				case SCCID_VECPRINTASPECT:
				case SCCID_VECPRINTBORDER:
				case SCCID_USEDOCPAGESETTINGS:
				case SCCID_BMPROTATION:
					InvalidateRect(hRightWnd,NULL,TRUE);
					break;
			}
			break;

		case WM_INITMENUPOPUP:
		{
			if (!HIWORD(lParam))
			{
				UINT openfile = bFileOpen ? MF_ENABLED : MF_GRAYED;
				HMENU hMenu = (HMENU)wParam;

				switch(LOWORD(lParam))
				{
					case 0: /* File */
						EnableMenuItem(hMenu,MENU_FILE_CLOSE   ,openfile);
						EnableMenuItem(hMenu,MENU_FILE_WRITE1  ,openfile);
						EnableMenuItem(hMenu,MENU_FILE_WRITE4  ,openfile);
						EnableMenuItem(hMenu,MENU_FILE_WRITE8  ,openfile);
						EnableMenuItem(hMenu,MENU_FILE_WRITE24 ,openfile);
					    EnableMenuItem(hMenu,MENU_FILE_WRITEWMF,openfile);
						EnableMenuItem(hMenu,MENU_FILE_WRITEEMF,openfile);
						EnableMenuItem(hMenu,MENU_FILE_PRINTDP ,openfile);

						if (bFileOpen)
						{
							EnableMenuItem(hMenu,MENU_FILE_PREVPAGE,(dwCurrentPage ? MF_ENABLED : MF_GRAYED));
							EnableMenuItem(hMenu,MENU_FILE_NEXTPAGE,((!bMaxFound || (dwCurrentPage < dwMaxPage)) ? MF_ENABLED : MF_GRAYED));
							EnableMenuItem(hMenu,MENU_FILE_GOTOPAGE,((!bMaxFound || (dwMaxPage > 0)) ? MF_ENABLED : MF_GRAYED));
						}
						else
						{
							EnableMenuItem(hMenu,MENU_FILE_PREVPAGE,openfile);
							EnableMenuItem(hMenu,MENU_FILE_NEXTPAGE,openfile);
							EnableMenuItem(hMenu,MENU_FILE_GOTOPAGE,openfile);
						}
						break;

					case 1: /* Edit */
						if (bFileOpen)
						{
							BOOL ok = (BOOL)(SendMessage(hViewWnd,SCCVW_GETCLIPINFO,0,0L) & SCCVW_CANCOPYTOCLIP);
							EnableMenuItem(hMenu, MENU_EDIT_CUT,  (ok ? MF_ENABLED : MF_GRAYED));
							EnableMenuItem(hMenu, MENU_EDIT_COPY, (ok ? MF_ENABLED : MF_GRAYED));

							ok = FALSE;
							if (OpenClipboard(NULL))
							{
								if (IsClipboardFormatAvailable(CF_UNICODETEXT) ||
									 IsClipboardFormatAvailable(CF_TEXT))
									ok = TRUE;

								CloseClipboard();
							}
							EnableMenuItem(hMenu, MENU_EDIT_PASTE, (ok ? MF_ENABLED : MF_GRAYED));
						}
						else
						{
							EnableMenuItem(hMenu,MENU_EDIT_CUT,  openfile);
							EnableMenuItem(hMenu,MENU_EDIT_COPY, openfile);
							EnableMenuItem(hMenu,MENU_EDIT_PASTE,openfile);
						}
						break;

					case 2: /* Anno */
						if (bFileOpen)
						{
							BOOL bTest;

							EnableMenuItem(hMenu, MENU_ANNO_CLEAR, (bHaveAnnos ? MF_ENABLED : MF_GRAYED));

							bTest = (BOOL)(SendMessage(hViewWnd,SCCVW_GETCLIPINFO,0,0L) & SCCVW_CANCOPYTOCLIP);
							EnableMenuItem(hMenu, MENU_ANNO_HIGHLIGHT, (bTest ? MF_ENABLED : MF_GRAYED));
						}
						else
						{
							EnableMenuItem(hMenu,MENU_ANNO_CLEAR    ,openfile);
							EnableMenuItem(hMenu,MENU_ANNO_HIGHLIGHT,openfile);
						}
						break;

					case 3: /* Info */
						EnableMenuItem(hMenu,MENU_INFO_PAGEINFO,openfile);
                        EnableMenuItem(hMenu,MENU_INFO_PAGECOUNT,openfile);
						break;

                    case 4: /* Options */
                        /* Disable these menu options by default */
                        EnableMenuItem(hMenu, MENU_OPTIONS_FILTERNOBLANK, MF_GRAYED);
                        EnableMenuItem(hMenu, MENU_OPTIONS_SINGLEPAGEHTMLEMAIL, MF_GRAYED);

                        {
                            /* Update the check marks */
                            SCCVWOPTIONSPEC40  locOption;

                            DWORD  val = 0;

                            locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                            locOption.dwId = SCCID_FILTERNOBLANK;
                            locOption.dwFlags = SCCVWOPTION_CURRENT;
                            locOption.pData = &val;
                            SendMessage(hViewWnd, SCCVW_GETOPTION, 0, (LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                            if (val)
                            {
                                CheckMenuItem(hMenu, MENU_OPTIONS_FILTERNOBLANK, MF_CHECKED);
                                bHandleNoBlank = TRUE;
                            }
                            if (bSinglePageHtmlEmail)
                                CheckMenuItem(hMenu, MENU_OPTIONS_SINGLEPAGEHTMLEMAIL, MF_CHECKED);
                        }
                        if (bFileOpen)
                        {
                            SCCVWDISPLAYINFO40  locDisplayInfo = {0};

                            /* Get the display info and use that to decide which menu options to enable */
                            locDisplayInfo.dwSize = sizeof(locDisplayInfo);
                            if (SCCVWERR_OK != SendMessage(hViewWnd, SCCVW_GETDISPLAYINFO, 0, (LPARAM)(PSCCVWDISPLAYINFO40)&locDisplayInfo))
                                locDisplayInfo.dwType = SCCVWTYPE_NONE;

                            if ((locDisplayInfo.dwType == SCCVWTYPE_SS) ||
                                (locDisplayInfo.dwType == SCCVWTYPE_DB))
                            {
                                EnableMenuItem(hMenu, MENU_OPTIONS_FILTERNOBLANK, MF_ENABLED);
                            }
                            else if ((locDisplayInfo.dwType == SCCVWTYPE_HTML) ||
                                     (locDisplayInfo.dwType == SCCVWTYPE_EMAIL))
                            {
                                EnableMenuItem(hMenu, MENU_OPTIONS_SINGLEPAGEHTMLEMAIL, MF_ENABLED);
                            }
                        }
                        break;

					case 5: /* Help */
						break;
				}
			}
		}
		break;

		case WM_COMMAND:

			locId    = LOWORD(wParam);
			locEvent = HIWORD(wParam);
			locCtrl  = (HWND) lParam;

			switch(locId)
			{
				case MENU_FILE_EXIT:
					DoCloseFile(hWnd);
					PostQuitMessage(0);
					break;

				case MENU_FILE_OPEN:

					DoOpenFile(hWnd);
					break;

				case MENU_FILE_CLOSE:

					DoCloseFile(hWnd);
					break;

                case MENU_INFO_PAGECOUNT:
                {
                     SCCVWPAGECOUNT   locPageCount = {0};
                     LRESULT          lResult = SCCVWERR_OK;
                     HDC          hdc;
                     HCURSOR      hCursor;

                     /* Get a DC for DrawPage to work with and set
                      *   up the wait cursor for the user
                      */

                     hdc = GetDC(hRightWnd);
                        

                     hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
                     ShowCursor(TRUE);

                     /* Set up the SCCVWPAGECOUNT structure */

                     locPageCount.dwSize        = sizeof(SCCVWPAGECOUNT);
                     locPageCount.lBottom       = 1100;
                     locPageCount.lRight        = 850;
                     locPageCount.lFormatWidth  = PAGE_WIDTH;
                     locPageCount.lFormatHeight = PAGE_HEIGHT;                                          
                     locPageCount.dwFlags       = SCCVW_DPFLAG_DETERMINEOUTPUTTYPE;
                     locPageCount.lUnitsPerInch = 1440;
                     locPageCount.hFormatDC     = hdc;

                     lResult = SendMessage(hViewWnd, SCCVW_GETPAGECOUNT, 0, (LPARAM)(PSCCVWPAGECOUNT)&locPageCount);

                     SetCursor(hCursor);
                     DeleteDC(hdc);

                     if (lResult == SCCERR_OK)
                     {
                        char  locStr[128];
                        wsprintf(locStr,"Page Count: %d",locPageCount.dwNumPages);
						MessageBox(hWnd, locStr, "Page Count", MB_OK);

                        if (!bMaxFound)
                        {
                            /* Found the last page; store its index */
                            dwMaxPage = locPageCount.dwNumPages-1;
                            bMaxFound = TRUE;
                        }

                     }
                     else
                     {
                        switch (lResult)
                        {
                        default:                MessageBox(hWnd, "Unknown Error", "Page Count", MB_OK);  break;
                        case SCCERR_NOFILE:     MessageBox(hWnd, "No File Open", "Page Count", MB_OK);  break;
                        case SCCERR_EMPTYFILE:  MessageBox(hWnd, "Empty File", "Page Count", MB_OK);  break;
                        case SCCERR_ALLOCFAILED:
                        case SCCERR_OUTOFMEMORY:
                                MessageBox(hWnd, "Out of Memory Error", "Page Count", MB_OK);  break;
                        case SCCERR_NOINIT:     MessageBox(hWnd, "Uninitialized","Page Count",MB_OK); break;
                        }                        						                         
                     }                   
                }
                break;

				case MENU_FILE_PREVPAGE:

					if (!bWorking && (dwCurrentPage > 0) && (dwCurrentPage != SCC_INVALIDPAGE))
					{
						bWorking = TRUE;
                        dwNewPage = dwCurrentPage - 1;
						InvalidateRect(hRightWnd, NULL, TRUE);
					}
					break;

				case MENU_FILE_NEXTPAGE:

					if (!bWorking && (!bMaxFound || dwCurrentPage < dwMaxPage))
					{
						bWorking = TRUE;
                        dwNewPage = dwCurrentPage + 1;
						InvalidateRect(hRightWnd, NULL, TRUE);
					}
					break;

				case MENU_FILE_GOTOPAGE:
					if (!bWorking && (IDOK == DialogBox(hInst, MAKEINTRESOURCE(GOTO_DIALOG), hMainWnd, GoToDialogProc)))
					{
                        bWorking = TRUE;
                        if (bMaxFound || (dwGoToPage <= dwCurrentPage))
                        {
                            /* Requested page is within the range of pages we
                             *   have processed; set up to display it
                             */
                            dwNewPage = dwGoToPage;
                        }
                        else
                        {
                            /* Requested page is outside the range of pages we
                             *   have processed; have DrawPage process pages until
                             *   we either find the requested page or find the
                             *   last page, whichever comes first
                             */
                            HDC          hdc;
                            HCURSOR      hCursor;

                            SCCVWDRAWPAGE41    locDrawPage = {0};
                            LRESULT            lResult = SCCVWERR_OK;

                            /* Get a DC for DrawPage to work with and set
                             *   up the wait cursor for the user
                             */
                            hdc = GetDC(hRightWnd);
                            hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
                            ShowCursor(TRUE);

                            /* Set up the SCCVWDRAWPAGE41 structure */
                            locDrawPage.lBottom       = 1100;
                            locDrawPage.lRight        = 850;
                            locDrawPage.lFormatWidth  = PAGE_WIDTH;
                            locDrawPage.lFormatHeight = PAGE_HEIGHT;
                            locDrawPage.dwSize        = sizeof(SCCVWDRAWPAGE41);
                            locDrawPage.dwPageToDraw  = dwCurrentPage;
                            locDrawPage.dwFlags       = SCCVW_DPFLAG_DETERMINEOUTPUTTYPE;
                            locDrawPage.lUnitsPerInch = 1440;
                            locDrawPage.hFormatDC = hdc;

                            /* Look for the page to go to, starting with the current page */
                            dwNewPage = dwCurrentPage;
                            while (dwNewPage < dwGoToPage)
                            {
                                dwNewPage++;
                                locDrawPage.dwPageToDraw = dwNewPage;

                                lResult = SendMessage(hViewWnd, SCCVW_DRAWPAGE, 0, (LPARAM)(PSCCVWDRAWPAGE41)&locDrawPage);
                                if (lResult == SCCVWERR_LASTPAGE)
                                {
                                    /* Found the last page */
                                    bMaxFound = TRUE;

                                    /* If the requested page is greater than this page,
                                     *   let the user know
                                     */
                                    if (dwGoToPage > dwNewPage)
                                        DrawPageErrorBox(lResult, dwNewPage);

                                    /* Set up to render this page */
                                    dwMaxPage = dwNewPage;
                                    dwGoToPage = dwNewPage;
                                }
                                else if (lResult != SCCVWERR_OK)
                                {
                                    /* Error occurred; report it and set up to render
                                     *   the last good page
                                     */
                                    DrawPageErrorBox(lResult, dwGoToPage);
                                    dwNewPage--;
                                    dwGoToPage = dwNewPage;
                                }
                            }

                            /* Set up to render the requested page */
                            dwNewPage = dwGoToPage;

                            /* End drawing and restore the normal cursor */
                            DeleteDC(hdc);
                            SetCursor(hCursor);
                        }

                        /* Clean up and invalidate the DrawPage window to
                         *   render the new page
                         */
                        dwGoToPage = SCC_INVALIDPAGE;
                        InvalidateRect(hRightWnd, NULL, TRUE);
					}
					break;

				case MENU_FILE_WRITE1:
				case MENU_FILE_WRITE4:
				case MENU_FILE_WRITE8:
				case MENU_FILE_WRITE24:
                case MENU_FILE_WRITEWMF:
                case MENU_FILE_WRITEEMF:
                    DoSaveFile(hWnd, locId);
                    break;

				case MENU_FILE_PRINTDP:
					DoPrintDP();
					InvalidateRect(hRightWnd,NULL,TRUE);
					break;

				case MENU_ANNO_CLEAR:
					SendMessage(hViewWnd,SCCVW_CLEARANNOTATIONS,0,0);
					bHaveAnnos = FALSE;
					ResetDrawPage();
					InvalidateRect(hRightWnd,NULL,TRUE);
					break;

				case MENU_ANNO_HIGHLIGHT:
					DoHighlight(hWnd);
					InvalidateRect(hRightWnd,NULL,TRUE);
					break;

				case MENU_ANNO_FORECOLOR:
					dialogColorName = "Foreground";
					dialogColor = fColor;
					if (IDOK == DialogBox(hInst,MAKEINTRESOURCE(COLOR_DIALOG),hMainWnd,ColorDialogProc))
					{
						fColor = dialogColor;
						SetHighlightColors();
						InvalidateRect(hRightWnd,NULL,TRUE);
						InvalidateRect(hViewWnd,NULL,TRUE);
						UpdateWindow(hViewWnd);
						UpdateWindow(hRightWnd);
					}
					break;

				case MENU_ANNO_BACKCOLOR:
					dialogColorName = "Background";
					dialogColor = bColor;
					if (IDOK == DialogBox(hInst,MAKEINTRESOURCE(COLOR_DIALOG),hMainWnd,ColorDialogProc))
					{
						bColor = dialogColor;
						SetHighlightColors();
						InvalidateRect(hRightWnd,NULL,TRUE);
						InvalidateRect(hViewWnd,NULL,TRUE);
						UpdateWindow(hViewWnd);
						UpdateWindow(hRightWnd);
					}
					break;

				case MENU_EDIT_CUT:
					SendMessage( hViewWnd, SCCVW_COPYTOCLIP, 0, 0L );
					DoHideSelection( hViewWnd );
					ResetDrawPage();
					InvalidateRect(hRightWnd,NULL,TRUE);
					break;

				case MENU_EDIT_COPY:
					SendMessage(hViewWnd,SCCVW_COPYTOCLIP,0,0L);
					break;

				case MENU_EDIT_PASTE:
					{
					HGLOBAL   hwClipboardText  = (HGLOBAL)NULL;
					HGLOBAL   hwText           = (HGLOBAL)NULL;
					HGLOBAL   hClipboardText   = (HGLOBAL)NULL;
					LPTSTR    szClipboardText  = 0;
					LPWORD    pwzClipboardText = 0;
					LPWORD    pwzText          = 0;
					int       len              = 0;

					/*
					| Insert pasted text using the edit annotations.
					| The text annotation API expects Unicode. If we have
					| Unicode on the clipboard (which we should), use it.
					| Otherwise, copy the single-byte text into a wide
					| buffer.
					*/

					if( OpenClipboard( hViewWnd ) )
					{
						if( IsClipboardFormatAvailable( CF_UNICODETEXT ) )
						{
							hwClipboardText = GetClipboardData( CF_UNICODETEXT );
							if( hwClipboardText )
							{
								pwzClipboardText = (LPWORD)GlobalLock( hwClipboardText );
								len              = lstrlenW( pwzClipboardText );

								/* We need to copy the clipboard text. */
								hwText = GlobalAlloc( GHND, sizeof(WORD) * ( len + 1 ) );
								if( hwText )
								{
									pwzText = (LPWORD)GlobalLock( hwText );
									CopyMemory( pwzText, pwzClipboardText, sizeof(WORD) * (len + 1 ) );
								}

								GlobalUnlock( hwClipboardText );
							}
						}
						else if( IsClipboardFormatAvailable( CF_TEXT ) )
						{
							/* 
							|  No Unicode on the clipboard. We'll have to 
							|  convert the single-byte string.
							*/
							hClipboardText = GetClipboardData( CF_TEXT );
							if( hClipboardText )
							{
								szClipboardText = (LPTSTR)GlobalLock( hClipboardText );
								len             = lstrlen( szClipboardText );
								hwText          = GlobalAlloc( GHND, sizeof(WORD) * ( len + 1 ) );
								if( hwText )
								{
									pwzText = (LPWORD)GlobalLock( hwText );
									MultiByteToWideChar( CP_ACP, 0, szClipboardText, -1, pwzText, len + 1 );
								}
								GlobalUnlock( hClipboardText );
							}
						}

						if( pwzText )
						{
							SCCVWFINDPOSITION41   sFindCurrPos;

							memset( &sFindCurrPos, 0, sizeof( SCCVWFINDPOSITION41 ) );
							sFindCurrPos.dwSize        = sizeof( SCCVWFINDPOSITION41 );
							sFindCurrPos.dwFindOptions = SCCVW_FINDANCHORSELECTPOS;
							if( SendMessage( hViewWnd, SCCVW_FINDPOSITION, 0, (LPARAM)&sFindCurrPos ) == SCCVWERR_OK )
							{
								SCCVWINSERTTEXT80        sInsertText;
								SCCVWINSERTPARABREAK80   sInsertBreak;
								WORD                     wTextIndex      = 0;
								DWORD                    dwAnnoTextStart = 0;
								DWORD                    dwAnnoTextLen   = 0;

								/* Hide the current selection. */
								DoHideSelection( hViewWnd );

								/* Initialize the text annotation. */
								memset( &sInsertText, 0, sizeof( SCCVWINSERTTEXT80 ) );
								sInsertText.dwSize   = sizeof( SCCVWINSERTTEXT80 );
								sInsertText.dwUser   = 0;
								sInsertText.sTextPos = sFindCurrPos.sResultPos;
								sInsertText.dwData   = 0;

								/* Initialize the para break annotation. */
								memset( &sInsertBreak, 0, sizeof( SCCVWINSERTPARABREAK80 ) );
								sInsertBreak.dwSize   = sizeof( SCCVWINSERTPARABREAK80 );
								sInsertBreak.dwUser   = 0;
								sInsertBreak.sParaPos = sFindCurrPos.sResultPos;
								sInsertBreak.dwData   = 0;

								while( wTextIndex < len )
								{
									if( pwzText[ wTextIndex ] == 0x000D || pwzText[ wTextIndex ] == 0x000A )
									{
										/* Paragraph break */
										if( dwAnnoTextLen > 0 )
										{
											/* Create the text annotation for the text before the break. */
											sInsertText.pText    = &pwzText[ dwAnnoTextStart ];
											sInsertText.dwLength = dwAnnoTextLen;
											SendMessage( hViewWnd, SCCVW_ADDANNOTATION, SCCVW_INSERTTEXT, (LPARAM)&sInsertText );

											dwAnnoTextLen = 0;
										}

										/* Create the paragraph break annotation. */
										SendMessage( hViewWnd, SCCVW_ADDANNOTATION, SCCVW_INSERTPARABREAK, (LPARAM)&sInsertBreak );

										/* Skip over DOS-style line ends. */
										if( wTextIndex < len - 1 &&
											( ( pwzText[ wTextIndex ] == 0x000D && pwzText[ wTextIndex + 1 ] == 0x000A ) ||
											( pwzText[ wTextIndex ] == 0x000A && pwzText[ wTextIndex + 1 ] == 0x000D ) ) )
										{
											++wTextIndex;
										}
									}
									else
									{
										if( dwAnnoTextLen == 0 )
										{
											dwAnnoTextStart = wTextIndex;
										}

										++dwAnnoTextLen;
									}

									++wTextIndex;
								}

								if( dwAnnoTextLen > 0 )
								{
									/* Create the last text annotation. */
									sInsertText.pText    = &pwzText[ dwAnnoTextStart ];
									sInsertText.dwLength = dwAnnoTextLen;
									SendMessage( hViewWnd, SCCVW_ADDANNOTATION, SCCVW_INSERTTEXT, (LPARAM)&sInsertText );
								}
							}
						}

						if( hwText )
						{
							GlobalUnlock( hwText );
							GlobalFree( hwText );
						}

						CloseClipboard();
						ResetDrawPage();
						InvalidateRect(hRightWnd,NULL,TRUE);
					}
					break;
					}

				case MENU_INFO_PAGEINFO:
					if ((lPageWidth != 0) && (lPageHeight != 0))
					{
						char  locStr[128];
						wsprintf(locStr,"Width:  %6d twips \nHeight: %6d twips",lPageWidth,lPageHeight);
						MessageBox(hWnd, locStr, "Page Size", MB_OK);
					}
					break;

				case MENU_OPTIONS_FILTERNOBLANK :
					{
					SCCVWOPTIONSPEC40     locOptionSpec;
					DWORD val;

					locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
					locOptionSpec.dwId = SCCID_FILTERNOBLANK;
					locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;

					if (bHandleNoBlank)
					{
						CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_FILTERNOBLANK, MF_UNCHECKED);
						bHandleNoBlank = FALSE;
						val = 0;
					}
					else
					{
						CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_FILTERNOBLANK, MF_CHECKED);
						bHandleNoBlank = TRUE;
						val = 1;
					}
					locOptionSpec.pData = &val;
					SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

					}
				break;
                case MENU_OPTIONS_SINGLEPAGEHTMLEMAIL:
                    bSinglePageHtmlEmail = bSinglePageHtmlEmail ? FALSE : TRUE;
                    CheckMenuItem(GetMenu(hWnd), MENU_OPTIONS_SINGLEPAGEHTMLEMAIL, (bSinglePageHtmlEmail ? MF_CHECKED : MF_UNCHECKED));
                    ResetDrawPage();
                    dwCurrentPage = SCC_INVALIDPAGE;
                    dwNewPage = 0;
                    dwMaxPage = 0;
                    bMaxFound = FALSE;
                    InvalidateRect(hRightWnd, NULL, TRUE);
                    break;

				case MENU_HELP_ABOUT:

					DialogBox(hInst, "HELP_ABOUTBOX", hViewWnd, (DLGPROC)HelpAbout);
					break;

				default:
					return (DefWindowProc(hWnd, message, wParam, lParam));
			}
			break;

		default:
			return (DefWindowProc(hWnd, message, wParam, lParam));
	}

	return (0L);
}

void SetScrollBars(HWND hwnd, RECT rect)
{
	SCROLLINFO info = { sizeof(info) };

	info.fMask = SIF_ALL;
	GetScrollInfo (hwnd, SB_VERT, &info);

	if( (bm.bmHeight <= (rect.bottom-rect.top)) || bm.bmHeight <= 0 )
	{ // no vert scrolling
		if ( info.nMax > 0 )
		{
			info.nMin = 0;
			info.nMax = 0;
			info.fMask = SIF_RANGE | SIF_DISABLENOSCROLL;
			SetScrollInfo(hwnd, SB_VERT, &info, TRUE);
		}
	}
	else
	{ // vert scrolling
		if ( info.nMax != bm.bmHeight || info.nPage != (UINT)(rect.bottom-rect.top) )
		{
			info.nMin = 0;
			info.nMax = bm.bmHeight;
			info.nPage = (rect.bottom-rect.top);
			info.fMask = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
			SetScrollInfo(hwnd, SB_VERT, &info, TRUE);
		}
	}

	info.fMask = SIF_ALL;
	GetScrollInfo (hwnd, SB_HORZ, &info);

	if( (bm.bmWidth <= (rect.right-rect.left)) || (bm.bmWidth <= 0) )
	{ // no horz scrolling
		if ( info.nMax > 0 )
		{
			info.nMin = 0;
			info.nMax = 0;
			info.fMask = SIF_RANGE | SIF_DISABLENOSCROLL;
			SetScrollInfo(hwnd, SB_HORZ, &info, TRUE);
		}
	}
	else
	{ // horz scrolling
		if ( info.nMax != bm.bmWidth || info.nPage != (UINT)(rect.right-rect.left) )
		{
			info.nMin = 0;
			info.nMax = bm.bmWidth;
			info.nPage = (rect.right-rect.left);
			info.fMask = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
			SetScrollInfo(hwnd, SB_HORZ, &info, TRUE);
		}
	} 

}

LRESULT CALLBACK ChildWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT  lRet = 0;

    switch (msg)
    {
    case WM_VSCROLL:
        {
            int yPos = 0;
            int yDist = GetSystemMetrics(SM_CXVSCROLL);
            SCROLLINFO si = {0}; 

            // Get all the vertial scroll bar information
            si.cbSize = sizeof (si);
            si.fMask  = SIF_ALL;
            GetScrollInfo (hwnd, SB_VERT, &si);

            // Save the position for comparison later on
            yPos = si.nPos;

            switch (LOWORD(wParam))
            {
                // user clicked the HOME keyboard key
            case SB_TOP:
                si.nPos = si.nMin;
                break;

                // user clicked the END keyboard key
            case SB_BOTTOM:
                si.nPos = si.nMax;
                break;

                // user clicked the top arrow
            case SB_LINEUP:
                si.nPos -= yDist;
                break;

                // user clicked the bottom arrow
            case SB_LINEDOWN:
                si.nPos += yDist;
                break;

                // user clicked the scroll bar shaft above the scroll box
            case SB_PAGEUP:
                si.nPos -= si.nPage;
                break;

                // user clicked the scroll bar shaft below the scroll box
            case SB_PAGEDOWN:
                si.nPos += si.nPage;
                break;

                // user dragged the scroll box
            case SB_THUMBTRACK:
                si.nPos = si.nTrackPos;
                break;
            }

            if (si.nPos < 0)
                si.nPos = 0;
            if (si.nPos > si.nMax)
                si.nPos = si.nMax;

            si.fMask = SIF_POS;
            SetScrollInfo (hwnd, SB_VERT, &si, TRUE);
            GetScrollInfo (hwnd, SB_VERT, &si);
            // If the position has changed, update the window
            if (si.nPos != yPos)
                InvalidateRect(hwnd, NULL, TRUE);
        }
        break;

    case WM_HSCROLL:
        {
            int yPos = 0;
            int yDist = GetSystemMetrics(SM_CYHSCROLL);
            SCROLLINFO si = {0};

            // Get all the vertial scroll bar information
            si.cbSize = sizeof (si);
            si.fMask  = SIF_ALL;
            GetScrollInfo (hwnd, SB_HORZ, &si);

            // Save the position for comparison later on
            yPos = si.nPos;

            switch (LOWORD (wParam))
            {
                // user clicked the HOME keyboard key
            case SB_LEFT:
                si.nPos = si.nMin;
                break;

                // user clicked the END keyboard key
            case SB_RIGHT:
                si.nPos = si.nMax;
                break;

                // user clicked the top arrow
            case SB_LINELEFT:
                si.nPos -= yDist;
                break;

                // user clicked the bottom arrow
            case SB_LINERIGHT:
                si.nPos += yDist;
                break;

                // user clicked the scroll bar shaft above the scroll box
            case SB_PAGELEFT:
                si.nPos -= si.nPage;
                break;

                // user clicked the scroll bar shaft below the scroll box
            case SB_PAGERIGHT:
                si.nPos += si.nPage;
                break;

                // user dragged the scroll box
            case SB_THUMBTRACK:
                si.nPos = si.nTrackPos;
                break;
            }

            if ( si.nPos < 0 )
                si.nPos = 0;

            if ( si.nPos > si.nMax )
                si.nPos = si.nMax;

            si.fMask = SIF_POS;
            SetScrollInfo (hwnd, SB_HORZ, &si, TRUE);
            GetScrollInfo (hwnd, SB_HORZ, &si);
            // If the position has changed, update the window
            if (si.nPos != yPos)
                InvalidateRect(hwnd, NULL, TRUE);
        }
        break;

    case WM_ERASEBKGND:
        lRet = 1;
        break;

    case WM_PAINT:
        DoChildPaint(hwnd);
        break;

    case WM_SIZE:
        {
            RECT rect={0};
            DefWindowProc(hwnd, msg, wParam, lParam);
            GetClientRect(hwnd, &rect);
            SetScrollBars(hwnd, rect);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        lRet = DefWindowProc(hwnd, msg, wParam, lParam);
        break;
    }

    return (lRet);
}

/*
 | Handle WM_PAINT for the child window
 */
VOID DoChildPaint (HWND hwnd)
{
    PAINTSTRUCT  ps;          /* generic PAINTSTRUCT structure             */
    HDC          hWindowDC;   /* child window DC (for final output)        */
    HDC          hMemDC;      /* memory DC (for rendering)                 */
    RECT         clientRect;  /* child window dimensions (in device units) */

    LRESULT  lResult = SCCVWERR_OK;  /* catch return values */

    /* Initialize required data */
    bWorking = TRUE;
    hWindowDC = BeginPaint(hwnd, &ps);
    hMemDC = CreateCompatibleDC(hWindowDC);
    GetClientRect(hwnd, &clientRect);

    /* Clear the output window */
    PatBlt(hWindowDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, WHITENESS);

    /* Sanity check */
    if (IsWindow(hViewWnd) && bFileOpen)
    {
        HBITMAP  hOldBitmap;  /* handle to the default bitmap for the memory DC */

        if (!hCurBitmap || (dwCurrentPage != dwNewPage))
        {
            /* Render the new page */
            LONG  lOutputWidth, lOutputHeight;

            SCCVWDISPLAYINFO40  locDisplayInfo = {0};
            SCCVWDRAWPAGEINFO   locDrawPageInfo = {0};
            SCCVWDRAWPAGE41     locDrawPage = {0};

            /* Ask DrawPage for information about the current page (in twips) */
            locDrawPageInfo.dwSize        = sizeof(locDrawPageInfo);
            locDrawPageInfo.dwPageToDraw  = dwNewPage;
            locDrawPageInfo.lUnitsPerInch = 1440;
            locDrawPageInfo.hFormatDC     = hWindowDC;
            locDrawPageInfo.hOutputDC     = hMemDC;

            lResult = SendMessage(hViewWnd, SCCVW_GETDRAWPAGEINFO, 0, (LPARAM)(PSCCVWDRAWPAGEINFO)&locDrawPageInfo);
            if ((lResult != SCCVWERR_OK) && (lResult != SCCVWERR_LASTPAGE))
                goto dochildpaint_error;

            /* Get the current display info */
            locDisplayInfo.dwSize = sizeof(locDisplayInfo);
            if (SCCVWERR_OK != SendMessage(hViewWnd, SCCVW_GETDISPLAYINFO, 0, (LPARAM)(PSCCVWDISPLAYINFO40)&locDisplayInfo))
                locDisplayInfo.dwType = SCCVWTYPE_NONE;

            /* Set up to have DrawPage output to a rectangle with a
             *   PAGE_WIDTH x PAGE_HEIGHT aspect ratio (in twips)
             *   by default
             */
            lOutputWidth  = clientRect.right - clientRect.left;
            lOutputHeight = clientRect.bottom - clientRect.top;
            if ((PAGE_HEIGHT * lOutputWidth) < (PAGE_WIDTH * lOutputHeight))
                lOutputHeight = lOutputWidth * PAGE_HEIGHT / PAGE_WIDTH;
            else
                lOutputWidth = lOutputHeight * PAGE_WIDTH / PAGE_HEIGHT;

            locDrawPage.dwSize        = sizeof(locDrawPage);
            locDrawPage.lRight        = lOutputWidth;
            locDrawPage.lBottom       = lOutputHeight;
            locDrawPage.lFormatWidth  = PAGE_WIDTH;
            locDrawPage.lFormatHeight = PAGE_HEIGHT;
            locDrawPage.dwPageToDraw  = dwNewPage;
            locDrawPage.dwFlags       = SCCVW_DPFLAG_DETERMINEOUTPUTTYPE;
            locDrawPage.lUnitsPerInch = 1440;
            locDrawPage.hFormatDC     = hWindowDC;
            locDrawPage.hOutputDC     = hMemDC;

            /* Adjust the page dimensions, if necessary */
            if (((locDisplayInfo.dwType == SCCVWTYPE_HTML) ||
                 (locDisplayInfo.dwType == SCCVWTYPE_EMAIL)) &&
                 bSinglePageHtmlEmail)
            {
                /* Use the largest of the default page width and draw
                 *   page format width as the final format width
                 */
                if (locDrawPageInfo.lFormatWidth > locDrawPage.lFormatWidth)
                {
                    locDrawPage.lFormatWidth = locDrawPageInfo.lFormatWidth;
                    lPageWidth = locDrawPageInfo.lFormatWidth;

                    /* Set the final output width */
                    locDrawPage.lRight = (LONG)(((double)lOutputWidth * (double)locDrawPage.lFormatWidth) / (double)PAGE_WIDTH);
                }

                /* Use the largest of the default page height and draw
                 *   page format height as the final document height
                 */
                if (locDrawPageInfo.lFormatHeight > locDrawPage.lFormatHeight)
                {
                    locDrawPage.lFormatHeight = locDrawPageInfo.lFormatHeight;
                    lPageHeight = locDrawPageInfo.lFormatHeight;

                    /* Set the final output height */
                    locDrawPage.lBottom = (LONG)(((double)lOutputHeight * (double)locDrawPage.lFormatHeight) / (double)PAGE_HEIGHT);
                }
            }

            /* Store the DrawPage format dimensions as the page dimensions */
            lPageWidth = locDrawPage.lFormatWidth;
            lPageHeight = locDrawPage.lFormatHeight;

            /* Destroy the previous bitmap, if necessary */
            if (hCurBitmap)
                DeleteObject(hCurBitmap);

            /* Get a new output bitmap for DrawPage to render into */
            hCurBitmap = CreateCompatibleBitmap(hWindowDC, locDrawPage.lRight, locDrawPage.lBottom);
            if (hCurBitmap)
            {
                /* Select the new bitmap into the memory DC */
                hOldBitmap = SelectObject(hMemDC, hCurBitmap);

                /* Clear the bitmap */
                PatBlt(hMemDC, 0, 0, locDrawPage.lRight, locDrawPage.lBottom, WHITENESS);

                /* Have DrawPage render the page into the bitmap */
                lResult = SendMessage(hViewWnd, SCCVW_DRAWPAGE, 0, (LPARAM)(PSCCVWDRAWPAGE41)&locDrawPage);
                if ((lResult == SCCVWERR_OK) || (lResult == SCCVWERR_LASTPAGE))
                {
                    /* Update the current page */
                    dwCurrentPage = dwNewPage;

                    if ((lResult == SCCVWERR_LASTPAGE) && !bMaxFound)
                    {
                        /* Found the last page; store its index */
                        dwMaxPage = dwCurrentPage;
                        bMaxFound = TRUE;
                    }
                }

                /* Select the old bitmap back into the memory DC */
                SelectObject(hMemDC, hOldBitmap);
            }
        }

        if (hCurBitmap)
        {
            /* Draw the rendered page to the output window */
            SCROLLINFO  siVert;
            SCROLLINFO  siHorz;

            BITMAP  bmPrev = bm;  /* data for previous bitmap */

            /* Get the current state of the scrollbars */
            /* - vertical */
            siVert.cbSize = sizeof(SCROLLINFO);
            siVert.fMask = SIF_POS;
            GetScrollInfo(hwnd, SB_VERT, &siVert);
            /* - horizontal */
            siHorz.cbSize = sizeof(SCROLLINFO);
            siHorz.fMask = SIF_POS;
            GetScrollInfo(hwnd, SB_HORZ, &siHorz);

            /* Get the data for the current bitmap */
            GetObject(hCurBitmap, sizeof(bm), &bm);

            /* Select the current bitmap into the memory DC */
            hOldBitmap = SelectObject(hMemDC, hCurBitmap);

            /* Blit the current bitmap to the output window at the current scrollbar offsets */
            BitBlt(hWindowDC,
                   0, 0,
                   bm.bmWidth - siHorz.nPos, bm.bmHeight - siVert.nPos,
                   hMemDC,
                   siHorz.nPos, siVert.nPos,
                   SRCCOPY);

            /* Select the old bitmap back into the memory DC */
            SelectObject(hMemDC, hOldBitmap);

            /* Update the scrollbar positions, if necessary */
            if ((bm.bmHeight != bmPrev.bmHeight) || (bm.bmWidth != bmPrev.bmWidth))
                SetScrollBars(hwnd, clientRect);

            /* Update window title to include current page on multipage documents */
            if ((lResult == SCCVWERR_OK) || (lResult == SCCVWERR_LASTPAGE))
            {
                SCCVWFILEINFO84  locFileInfo;

                locFileInfo.dwSize = sizeof(locFileInfo);

                if (SCCVWERR_OK == SendMessage(hViewWnd, SCCVW_GETFILEINFO, 0, (LPARAM)&locFileInfo))
                {
                    char *locStr = (char *)malloc(wcslen(locFileInfo.szDisplayName) + wcslen(locFileInfo.szFileIdName) + 29);   /* big enough to hold everything */
                    if (locStr)
                        wsprintf(locStr, "OIT Page - %S - %S (Page %d)", locFileInfo.szDisplayName, locFileInfo.szFileIdName, dwCurrentPage + 1);
                    else
                        locStr = defStr;

                    SetWindowText(hMainWnd, locStr);

                    if (locStr != defStr)
                        free(locStr);   

                    SendMessage(hViewWnd, SCCVW_FREEFILEINFO, 0, (LPARAM)&locFileInfo);
                }
                else
                {
                    SetWindowText(hMainWnd, defStr);
                }
            }
        }
        else
        {
            /* Error getting a bitmap; report it */
            lResult = SCCVWERR_ALLOCFAILED;
        }
    }

dochildpaint_error:
    /* Report any errors */
    if ((lResult != SCCVWERR_OK) && (lResult != SCCVWERR_LASTPAGE))
    {
        /* Clear the output window */
        PatBlt(hWindowDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, WHITENESS);

        /* Output an error message in the center of the window */
        SetTextAlign(hWindowDC, TA_BASELINE | TA_CENTER);
        SetTextColor(hWindowDC, RGB(0, 0, 0));
        SetBkMode(hWindowDC, TRANSPARENT);
        switch (lResult)
        {
        case SCCVWERR_NONEFOUND:
        case SCCVWERR_NOPAGE:
            TextOut(hWindowDC,
                    ((clientRect.right - clientRect.left) / 2), ((clientRect.bottom - clientRect.top) / 2),
                    "Could not get page information.", 31);
            break;
        case SCCVWERR_ALLOCFAILED:
            TextOut(hWindowDC,
                    ((clientRect.right - clientRect.left) / 2), ((clientRect.bottom - clientRect.top) / 2),
                    "Could not allocate memory.", 26);
            break;
        case SCCVWERR_NOFILE:
            TextOut(hWindowDC,
                    ((clientRect.right - clientRect.left) / 2), ((clientRect.bottom - clientRect.top) / 2),
                    "No file is being viewed.", 24);
            break;
        case SCCVWERR_NOINIT:
            TextOut(hWindowDC,
                    ((clientRect.right - clientRect.left) / 2), ((clientRect.bottom - clientRect.top) / 2),
                    "DrawPage not initialized.", 25);
            break;
        case SCCVWERR_FEATURENOTAVAIL:
            TextOut(hWindowDC,
                    ((clientRect.right - clientRect.left) / 2), ((clientRect.bottom - clientRect.top) / 2),
                    "DrawPage feature unavailable.", 26);
            break;
        default:
            TextOut(hWindowDC,
                    ((clientRect.right - clientRect.left) / 2), ((clientRect.bottom - clientRect.top) / 2),
                    "Could not draw this page.", 25);
            break;
        }
    }

    /* Clean up */
    DeleteDC(hMemDC);
    EndPaint(hwnd, &ps);
    bWorking = FALSE;
}

/*
 | Handle MENU_FILE_WRITE1, MENU_FILE_WRITE4, MENU_FILE_WRITE8,
 |   and MENU_FILE_WRITE24 for the child window
 */
VOID DoSavePageToBitmap (WORD wFormat, HWND hWnd, LPTSTR filename)
{
    SCCVWDISPLAYINFO40  locDisplayInfo;
    SCCVWDRAWPAGE41     locDrawPage;
    BITMAPINFOHEADER    locBIH;

    LONG  lDpi, lOutputWidth, lOutputHeight, lMargin;

    HDC      hDC = 0;            /* window DC handle      */
    HDC      hCDC = 0;           /* output DC handle      */
    HBITMAP  hBitmap = 0;        /* generic bitmap handle */
    HBITMAP  hOutputBitmap = 0;  /* output bitmap handle  */
    HANDLE   hDIB = 0;           /* DIB handle            */

    /* Clear structures */
    memset(&locBIH, 0, sizeof(locBIH));
    memset(&locDisplayInfo, 0, sizeof(locDisplayInfo));
    memset(&locDrawPage, 0, sizeof(locDrawPage));

    /* Get the output dpi */
    if (wFormat == MENU_FILE_WRITE1)
        lDpi = 300;
    else
        lDpi = 72;

    /* Initialize output dimensions and margin size to
     *   8.5" x 11" and a 0.5" margin at the output dpi
     */
    lOutputWidth = ((17 * lDpi) / 2);
    lOutputHeight = (11 * lDpi);
    lMargin = (lDpi / 2);

    /* Get contexts for output */
    hDC = GetDC(hWnd);
    hCDC = CreateCompatibleDC(hDC);

    /* Get the display info */
    locDisplayInfo.dwSize = sizeof(locDisplayInfo);
    if (SCCVWERR_OK != SendMessage(hViewWnd, SCCVW_GETDISPLAYINFO, 0, (LPARAM)((PSCCVWDISPLAYINFO40)&locDisplayInfo)))
        locDisplayInfo.dwType = SCCVWTYPE_NONE;

    /* Set up the DrawPage structure */
    locDrawPage.dwSize = sizeof(locDrawPage);
    locDrawPage.dwPageToDraw = dwCurrentPage;
    locDrawPage.dwReserved = 0;
    locDrawPage.dwFlags = SCCVW_DPFLAG_DETERMINEOUTPUTTYPE;
    if (wFormat != MENU_FILE_WRITE1)
        locDrawPage.dwFlags |= SCCVW_DPFLAG_RETURNPALETTE;
    locDrawPage.lUnitsPerInch = 1440;
    if ((locDisplayInfo.dwType == SCCVWTYPE_WP) ||
        (locDisplayInfo.dwType == SCCVWTYPE_SS) ||
        (locDisplayInfo.dwType == SCCVWTYPE_DB))
    {
        /* Default output to 8.5" x 11" */
        locDrawPage.lFormatWidth = PAGE_WIDTH;
        locDrawPage.lFormatHeight = PAGE_HEIGHT;

        locDrawPage.lTop = 0;
        locDrawPage.lLeft = 0;
        locDrawPage.lRight = lOutputWidth;
        locDrawPage.lBottom = lOutputHeight;
    }
    else if ((locDisplayInfo.dwType == SCCVWTYPE_HTML) ||
             (locDisplayInfo.dwType == SCCVWTYPE_EMAIL))
    {
        /* Default output to 8.5" x 11" */
        locDrawPage.lFormatWidth = PAGE_WIDTH;
        locDrawPage.lFormatHeight = PAGE_HEIGHT;

        locDrawPage.lTop = 0;
        locDrawPage.lLeft = 0;
        locDrawPage.lRight = lOutputWidth;
        locDrawPage.lBottom = lOutputHeight;

        if (bSinglePageHtmlEmail)
        {
            LONG  lTempWidth, lTempHeight;

            /* Set the size of the full document */
            locDrawPage.lFormatWidth = lPageWidth;
            locDrawPage.lFormatHeight = lPageHeight;

            /* Set the output size to the greater of the default
            *   output size and the size of the full document
            */
            locDrawPage.lTop = 0;
            locDrawPage.lLeft = 0;
            /* - convert the full document size from twips to the output dpi */
            lTempWidth = (LONG)((((double)lPageWidth * (double)lDpi) + 720.0) / 1440.0);
            lTempHeight = (LONG)((((double)lPageHeight * (double)lDpi) + 720.0) / 1440.0);
            if (locDrawPage.lRight < lTempWidth)
            {
                locDrawPage.lRight = lTempWidth;
                lOutputWidth = lTempWidth;
            }
            if (locDrawPage.lBottom < lTempHeight)
            {
                locDrawPage.lBottom = lTempHeight;
                lOutputHeight = lTempHeight;
            }
        }
    }
    else
    {
        /* Default output to 8.5" x 11" with a 0.5" margin */
        locDrawPage.lFormatWidth = PAGE_WIDTH_MINUS_MARGIN;
        locDrawPage.lFormatHeight = PAGE_HEIGHT_MINUS_MARGIN;

        locDrawPage.lTop = lMargin;
        locDrawPage.lLeft = lMargin;
        locDrawPage.lRight = lOutputWidth - lMargin;
        locDrawPage.lBottom = lOutputHeight - lMargin;
    }
    locDrawPage.hOutputDC = hCDC;
    locDrawPage.hFormatDC = hDC;

    /* Get a bitmap for output */
    if (wFormat == MENU_FILE_WRITE1)
    {
        hOutputBitmap = CreateBitmap(lOutputWidth, lOutputHeight, 1, 1, NULL);
    }
    else
    {
        locBIH.biSize = sizeof(BITMAPINFOHEADER);
        locBIH.biWidth = lOutputWidth;
        locBIH.biHeight = lOutputHeight;
        locBIH.biPlanes = 1;
        switch (wFormat)
        {
        case MENU_FILE_WRITE4:
            locBIH.biBitCount = 4;
            break;
        case MENU_FILE_WRITE8:
            locBIH.biBitCount = 8;
            break;
        case MENU_FILE_WRITE24:
            locBIH.biBitCount = 24;
            break;
        }
        locBIH.biCompression = BI_RGB;
        locBIH.biSizeImage = 0;
        locBIH.biXPelsPerMeter = 0;
        locBIH.biYPelsPerMeter = 0;
        locBIH.biClrUsed = 0;
        locBIH.biClrImportant = 0;

        hOutputBitmap = CreateDIBitmap(hDC, &locBIH, 0, NULL, NULL, DIB_RGB_COLORS);
    }

    if (hOutputBitmap)
    {
        /* Select the bitmap into the output context and clear it */
        hBitmap = SelectObject(hCDC, hOutputBitmap);
        Rectangle(hCDC, 0, 0, lOutputWidth, lOutputHeight);

        /* Tell DrawPage to render the page */
        SendMessage(hViewWnd, SCCVW_DRAWPAGE, 0, (LPARAM)((PSCCVWDRAWPAGE41)&locDrawPage));

        /* Put the original bitmap back into the output context */
        SelectObject(hCDC, hBitmap);

        /* Write out the contents of the bitmap */
        switch (wFormat)
        {
        case MENU_FILE_WRITE1:
            hDIB = DibFromBitmap(hOutputBitmap, BI_RGB, 1, NULL);
            if (hDIB)
                WriteDIB(filename, hDIB);
            break;
        case MENU_FILE_WRITE4:
            hDIB = DibFromBitmap(hOutputBitmap, BI_RGB, 4, locDrawPage.hPalette);
            if (hDIB)
                WriteDIB(filename, hDIB);
            break;
        case MENU_FILE_WRITE8:
            hDIB = DibFromBitmap(hOutputBitmap, BI_RGB, 8, locDrawPage.hPalette);
            if (hDIB)
                WriteDIB(filename, hDIB);
            break;
        case MENU_FILE_WRITE24:
            hDIB = DibFromBitmap(hOutputBitmap, BI_RGB, 24, locDrawPage.hPalette);
            if (hDIB)
                WriteDIB(filename, hDIB);
            break;
        }
    }

    /* Clean up */
    if (hDIB)
        GlobalFree(hDIB);
    if (locDrawPage.hPalette)
        DeleteObject(locDrawPage.hPalette);
    if (hOutputBitmap)
        DeleteObject(hOutputBitmap);
    if (hCDC)
        DeleteDC(hCDC);
    if (hDC)
        ReleaseDC(hWnd, hDC);
}

/*
 | Handle MENU_FILE_WRITEWMF and MENU_FILE_WRITEEMF for the child window
 */
VOID DoSavePageToMetafile (WORD wFormat, HWND hWnd, LPTSTR filename)
{
    SCCVWDISPLAYINFO40  locDisplayInfo;
    SCCVWDRAWPAGE41     locDrawPage;
    SCCVWOPTIONSPEC40   locOption;

    HDC   hDC = 0;                      /* window DC handle               */
    HDC   hMDC = 0;                     /* metafile DC handle             */
    BOOL  bPrintBackground = FALSE;     /* print background option flag   */
    BOOL  bRenderEmbeddedFonts = FALSE; /* embedded font rendering option */
    LONG  lOutputWidth = 12240;         /* default 8.5" (in twips)        */
    LONG  lOutputHeight = 15840;        /* default 11" (in twips)         */
    LONG  lMargin = 720;                /* default 0.5" (in twips)        */

    /* Clear structures */
    memset(&locDisplayInfo, 0, sizeof(locDisplayInfo));
    memset(&locDrawPage, 0, sizeof(locDrawPage));

    /* Get the display info */
    locDisplayInfo.dwSize = sizeof(locDisplayInfo);
    if (SCCVWERR_OK != SendMessage(hViewWnd, SCCVW_GETDISPLAYINFO, 0, (LPARAM)((PSCCVWDISPLAYINFO40)&locDisplayInfo)))
        locDisplayInfo.dwType = SCCVWTYPE_NONE;

    /* Set up the DrawPage structure */
    locDrawPage.dwSize = sizeof(locDrawPage);
    locDrawPage.dwPageToDraw = dwCurrentPage;
    locDrawPage.dwReserved = 0;
    locDrawPage.dwFlags = SCCVW_DPFLAG_DETERMINEOUTPUTTYPE;
    if (wFormat == MENU_FILE_WRITEEMF)
        locDrawPage.dwFlags |= SCCVW_DPFLAG_NOTMETAFILE;
    locDrawPage.lUnitsPerInch = 1440;
    if ((locDisplayInfo.dwType == SCCVWTYPE_WP) ||
        (locDisplayInfo.dwType == SCCVWTYPE_SS) ||
        (locDisplayInfo.dwType == SCCVWTYPE_DB))
    {
        /* Default output to 8.5" x 11" */
        locDrawPage.lFormatWidth = PAGE_WIDTH;
        locDrawPage.lFormatHeight = PAGE_HEIGHT;

        locDrawPage.lTop = 0;
        locDrawPage.lLeft = 0;
        locDrawPage.lRight = lOutputWidth;
        locDrawPage.lBottom = lOutputHeight;
    }
    else if ((locDisplayInfo.dwType == SCCVWTYPE_HTML) ||
             (locDisplayInfo.dwType == SCCVWTYPE_EMAIL))
    {
        /* Default output to 8.5" x 11" */
        locDrawPage.lFormatWidth = PAGE_WIDTH;
        locDrawPage.lFormatHeight = PAGE_HEIGHT;

        locDrawPage.lTop = 0;
        locDrawPage.lLeft = 0;
        locDrawPage.lRight = lOutputWidth;
        locDrawPage.lBottom = lOutputHeight;

        if (bSinglePageHtmlEmail)
        {
            /* Set the size of the full document */
            locDrawPage.lFormatWidth = lPageWidth;
            locDrawPage.lFormatHeight = lPageHeight;

            /* Set the output size to the greater of the default
             *   output size and the size of the full document
             */
            if (locDrawPage.lRight < lPageWidth)
            {
                locDrawPage.lRight = lPageWidth;
                lOutputWidth = lPageWidth;
            }
            if (locDrawPage.lBottom < lPageHeight)
            {
                locDrawPage.lBottom = lPageHeight;
                lOutputHeight = lPageHeight;
            }
        }
    }
    else
    {
        /* Default output to 8.5" x 11" with a 0.5" margin */
        locDrawPage.lFormatWidth = PAGE_WIDTH_MINUS_MARGIN;
        locDrawPage.lFormatHeight = PAGE_HEIGHT_MINUS_MARGIN;

        locDrawPage.lTop = lMargin;
        locDrawPage.lLeft = lMargin;
        locDrawPage.lRight = lOutputWidth - lMargin;
        locDrawPage.lBottom = lOutputHeight - lMargin;
    }
    /* - get contexts for output */
    hDC = GetDC(hWnd);
    switch (wFormat)
    {
    case MENU_FILE_WRITEWMF:
        /* WMF */
        hMDC = CreateMetaFile(filename);
        break;
    case MENU_FILE_WRITEEMF:
        /* EMF */
        {
            RECT  rcMF;  /* EMF frame rectangle */

            /* Convert the DrawPage output size to screen units */
            LONG  lHDpi = (LONG)((((double)GetDeviceCaps(hDC, HORZRES) * (double)254) / ((double)GetDeviceCaps(hDC, HORZSIZE) * (double)10)) + 0.5);
            LONG  lVDpi = (LONG)((((double)GetDeviceCaps(hDC, VERTRES) * (double)254) / ((double)GetDeviceCaps(hDC, VERTSIZE) * (double)10)) + 0.5);

            locDrawPage.lTop = (locDrawPage.lTop * lVDpi) / 1440;
            locDrawPage.lLeft = (locDrawPage.lLeft * lHDpi) / 1440;
            locDrawPage.lBottom = (locDrawPage.lBottom * lVDpi) / 1440;
            locDrawPage.lRight = (locDrawPage.lRight * lHDpi) / 1440;

            /* Set up the EMF frame
             *
             * - NOTE: We have to convert the output size from twips to
             *         .01mm units, as that is the unit measure that
             *         EMF uses for the frame.
             */
            rcMF.top = 0;
            rcMF.left = 0;
            rcMF.right = (LONG)(((double)lOutputWidth * (double)2540) / (double)1440);
            rcMF.bottom = (LONG)((double)(lOutputHeight * (double)2540) / (double)1440);

            /* Create the metafile */
            hMDC = CreateEnhMetaFile(hDC, filename, &rcMF, 0);
        }
        break;
    }
    locDrawPage.hOutputDC = hMDC;
    locDrawPage.hFormatDC = hDC;

    if (hMDC)
    {
        if (wFormat == MENU_FILE_WRITEEMF)
        {
            /* Turn on the print background option */
            bPrintBackground = TRUE;
            locOption.dwSize = sizeof(locOption);
            locOption.dwId = SCCID_VECPRINTBACKGROUND;
            locOption.dwFlags = SCCVWOPTION_CURRENT;
            locOption.pData = &bPrintBackground;
            SendMessage(hViewWnd, SCCVW_SETOPTION, 0, (LPARAM)((PSCCVWOPTIONSPEC40)&locOption));

            /* Turn off embedded font rendering.
			   OIVT handles embedded fonts by rendering the glyphs to off-screen bitmaps and
			   blitting them to the destination graphics context. When drawing to an EMF
			   context this can result in lower quality font rendering. */
            bRenderEmbeddedFonts = FALSE;
            locOption.dwId = SCCID_RENDER_EMBEDDED_FONTS;
            locOption.pData = &bRenderEmbeddedFonts;
            SendMessage(hViewWnd, SCCVW_SETOPTION, 0, (LPARAM)((PSCCVWOPTIONSPEC40)&locOption));
        }

        /* Tell DrawPage to render the page */
        SendMessage(hViewWnd, SCCVW_DRAWPAGE, 0, (LPARAM)((PSCCVWDRAWPAGE41)&locDrawPage));

        if (wFormat == MENU_FILE_WRITEEMF)
        {
            /* Turn off the print background option */
            bPrintBackground = FALSE;
            locOption.dwId = SCCID_VECPRINTBACKGROUND;
            locOption.pData = &bPrintBackground;
            SendMessage(hViewWnd, SCCVW_SETOPTION, 0, (LPARAM)((PSCCVWOPTIONSPEC40)&locOption));

            /* Turn embedded font rendering back on. */
            bRenderEmbeddedFonts = TRUE;
            locOption.dwId = SCCID_RENDER_EMBEDDED_FONTS;
            locOption.pData = &bRenderEmbeddedFonts;
            SendMessage(hViewWnd, SCCVW_SETOPTION, 0, (LPARAM)((PSCCVWOPTIONSPEC40)&locOption));
        }
    }

    /* Clean up */
    if (hMDC)
    {
        switch (wFormat)
        {
        case MENU_FILE_WRITEWMF:
            {
                HMETAFILE  hMetafile = CloseMetaFile(hMDC);
                DeleteMetaFile(hMetafile);
            }
            break;
        case MENU_FILE_WRITEEMF:
            {
                HENHMETAFILE  hEnhMetaFile = CloseEnhMetaFile(hMDC);
                DeleteEnhMetaFile(hEnhMetaFile);
            }
            break;
        }
    }
    if (hDC)
        ReleaseDC(hWnd, hDC);
}

/*
|  Handle WM_CREATE
*/

VOID DoCreate( HWND hWnd )
{
	char  locViewerPath[256]={0};
	char  locViewerClass[256]={0};

	/*
	|  Load the Outside In Viewer Technology DLL (SCxxVW.DLL)
	|
	|  This code loads SCxxVW.DLL from the same directory
	|  the sample application is in. It uses the two routines
	|  SCCGetExePath and SCCLoadViewerDLL that are in SCCLINK.C.
	|  SCCLINK.C is #included above (just before WinMain).
	*/

	SCCGetExePath(hInst,(LPSTR)szExePath,256);
	lstrcpy(locViewerPath,szExePath);
	hViewerLibrary = SCCLoadViewerDLL(locViewerPath);

	/*
	|  If the Viewer Technology can not be loaded, bail out.
	*/

	if (hViewerLibrary == NULL)
	{
		return;
	}

	/*
	|  Create the view window here
	*/

	/*
	|  In Win32, the view window's CLASS name does not have
	|  to be different for each OEM since global class names
	|  are process specific.
	*/

	lstrcpy((LPSTR)locViewerClass,"SCCVIEWER");

	hViewWnd = CreateWindow(locViewerClass, NULL,
							WS_CHILD | WS_BORDER | WS_VISIBLE | WS_CLIPCHILDREN,
							0, 0, 0, 0,
							hWnd,
							0,
							hInst,
							NULL);

	{
	SCCVWOPTIONSPEC40  locOptionSpec;
	BOOL    locBool = TRUE;

	locOptionSpec.dwSize = sizeof(locOptionSpec);
	locOptionSpec.dwId = SCCID_VECPRINTBACKGROUND;
	locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
	locOptionSpec.pData = (VTVOID *)&locBool;
	SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
	}

	hRightWnd = CreateWindow( locChildClassName, NULL,
							WS_CHILD | WS_BORDER | WS_VISIBLE | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL,
							0, 0, 0, 0,
							hWnd, 
							NULL,
							hInst, 
							NULL) ;

}

VOID DoSize( HWND hWnd, WORD wWidth, WORD wHeight )
{
	DWORD middle = wWidth/2;

	InvalidateRect(hWnd,NULL,FALSE);
	if (IsWindow(hViewWnd))
	{
		MoveWindow(hViewWnd,10,10,middle-20,wHeight-20,TRUE);
		InvalidateRect(hViewWnd, NULL, TRUE);
		ShowWindow(hViewWnd,SW_SHOW);
	}

	if (IsWindow(hRightWnd))
	{
		MoveWindow(hRightWnd, middle+10, 10, wWidth-middle-20, wHeight-20, TRUE);
		InvalidateRect(hRightWnd,NULL,TRUE);
		ShowWindow(hRightWnd,SW_SHOW);
	}
}

 
/*
|  Handle WM_DESTROY
*/

VOID DoDestroy (HWND hWnd)
{
    UNUSED(hWnd);

    if (hCurBitmap)
    {
        DeleteObject(hCurBitmap);
        hCurBitmap = NULL;
    }

    if (IsWindow(hViewWnd))
    {
        if (bFileOpen)
            SendMessage(hViewWnd, SCCVW_DEINITDRAWPAGE, 0, 0);
        SendMessage(hViewWnd, SCCVW_CLOSEFILE, 0, 0L);
        DestroyWindow(hViewWnd);
    }

    if (hViewerLibrary != NULL)
        FreeLibrary(hViewerLibrary);
}

	/*
	|  Handle Close menu item
	*/

VOID DoCloseFile (HWND hWnd)
{
    UNUSED(hWnd);

    if (hCurBitmap)
    {
        DeleteObject(hCurBitmap);
        hCurBitmap = NULL;
    }

    if (IsWindow(hViewWnd))
    {
        SendMessage(hViewWnd, SCCVW_DEINITDRAWPAGE, 0, 0);
        SendMessage(hViewWnd, SCCVW_CLOSEFILE, 0, 0L);
        InvalidateRect(hViewWnd, NULL, TRUE);
    }

    if (IsWindow(hRightWnd))
        InvalidateRect(hRightWnd, NULL, TRUE);

    if (IsWindow(hMainWnd))
        InvalidateRect(hMainWnd, NULL, TRUE);

    bFileOpen = FALSE;
}

/*
|  Handle SCCVW_FILECHANGE message
*/

VOID DoFileChange( HWND hWnd )
{
    SCCVWFILEINFO84  locFileInfo;

    locFileInfo.dwSize = sizeof(locFileInfo);

    if (SendMessage(hViewWnd, SCCVW_GETFILEINFO, 0, (LPARAM)&locFileInfo) == SCCVWERR_OK)
    {
        char *locStr = (char *)malloc(wcslen(locFileInfo.szDisplayName)+wcslen(locFileInfo.szFileIdName)+24);   /* big enough to hold everything */
        if (locStr)
            wsprintf(locStr,"OIVT Page - %S - %S",locFileInfo.szDisplayName,locFileInfo.szFileIdName);
        else
            locStr = defStr;

        SetWindowText(hWnd,locStr);

        if (locStr != defStr)
            free(locStr);   

        SendMessage(hViewWnd,SCCVW_FREEFILEINFO,0,(LPARAM)&locFileInfo);
    }
    else
    {
        SetWindowText(hWnd,defStr);
    }
}

/*
|  Handle Open menu item
*/

VOID DoOpenFile( HWND hWnd )
{
#define MAXFILENAME 256

	OPENFILENAME  locOFN;
	char        locFilterSpec[128] = "All files\0*.*\0";
	char        locFileName[MAXFILENAME];
	char        locFileTitle[MAXFILENAME];

	locFileName[0] = 0x00;
	locFileTitle[0] = 0x00;

	locOFN.lStructSize = sizeof(OPENFILENAME);
	locOFN.hwndOwner = hWnd;
	locOFN.hInstance = hInst;
	locOFN.lpstrFilter = locFilterSpec;
	locOFN.lpstrCustomFilter = NULL;
	locOFN.nMaxCustFilter = 0L;
	locOFN.nFilterIndex = 0L;
	locOFN.lpstrFile = locFileName;
	locOFN.nMaxFile = MAXFILENAME;
	locOFN.lpstrFileTitle = locFileTitle;
	locOFN.nMaxFileTitle = MAXFILENAME;
	locOFN.lpstrInitialDir = NULL;
	locOFN.lpstrTitle = "Open...";
	locOFN.Flags = OFN_FILEMUSTEXIST;
	locOFN.lpstrDefExt = NULL;
	locOFN.lCustData = 0;
	locOFN.lpfnHook = NULL;
	locOFN.lpTemplateName = NULL;

	if (GetOpenFileName(&locOFN) == TRUE)
	{
		if (IsWindow(hViewWnd))
		{
			SCCVWVIEWFILE40  locViewFile;

			if( bFileOpen )
			{
				bFileOpen = FALSE;
				SendMessage(hViewWnd,SCCVW_DEINITDRAWPAGE,0,0);
			}

			locViewFile.dwSize = sizeof(SCCVWVIEWFILE40);
			locViewFile.dwSpecType = IOTYPE_ANSIPATH;
			locViewFile.pSpec = (VTVOID *)locFileName;
			locViewFile.dwViewAs = 0;
			locViewFile.bUseDisplayName = FALSE;
			locViewFile.bDeleteOnClose = FALSE;
			locViewFile.dwFlags = 0;
			locViewFile.dwReserved1 = 0;
			locViewFile.dwReserved2 = 0;

			SendMessage(hViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&locViewFile);

			SetFocus(hViewWnd);

			SendMessage(hViewWnd,SCCVW_INITDRAWPAGE,0,0);
			bFileOpen = TRUE;
			dwCurrentPage = SCC_INVALIDPAGE;
            dwNewPage = 0;
			dwGoToPage = SCC_INVALIDPAGE;
			bMaxFound = FALSE;
			dwMaxPage = 0;

			bHaveAnnos = FALSE;
			SetHighlightColors();

			InvalidateRect(hRightWnd,NULL,TRUE);
		}
	}
}


VOID DoSaveFile( HWND hWnd, WORD wId )
{
	OPENFILENAME  locOFN;
	char *      locFilterSpec = "All files\0*.*\0";
    char *      locExt = NULL;
	char        locFileName[MAXFILENAME];
	char        locFileTitle[MAXFILENAME];
    char        locStr[256];

    if (!IsWindow(hViewWnd))
        return;

    switch (wId)
    {
    case MENU_FILE_WRITE1:
	case MENU_FILE_WRITE4:
	case MENU_FILE_WRITE8:
	case MENU_FILE_WRITE24:
        locFilterSpec = "Bitmap files\0*.bmp\0";
        locExt = "bmp";
        break;
    case MENU_FILE_WRITEEMF:
        locFilterSpec ="EMF files\0*.emf\0";
        locExt = "emf";
        break;
	case MENU_FILE_WRITEWMF:
    
        if ((lPageWidth > 65535) || (lPageHeight > 65535))
        {
            /* Put up an error message for the user */            
            wsprintf(locStr,"Document too large for WMF. Please save as EMF instead.\n\nDocument Size: %6d x %6d\n(WMF Maximum Size: 65535 x 65535)", lPageWidth, lPageHeight);
            MessageBox(hWnd, locStr, "Error", MB_ICONERROR | MB_OK);
            return;
        }
        locFilterSpec ="WMF files\0*.wmf\0";
        locExt = "wmf";
        break;
    default://???
        return;
    }

	locFileName[0] = 0x00;
	locFileTitle[0] = 0x00;

	locOFN.lStructSize = sizeof(OPENFILENAME);
	locOFN.hwndOwner = hWnd;
	locOFN.hInstance = hInst;
	locOFN.lpstrFilter = locFilterSpec;
	locOFN.lpstrCustomFilter = NULL;
	locOFN.nMaxCustFilter = 0L;
	locOFN.nFilterIndex = 0L;
	locOFN.lpstrFile = locFileName;
	locOFN.nMaxFile = MAXFILENAME;
	locOFN.lpstrFileTitle = locFileTitle;
	locOFN.nMaxFileTitle = MAXFILENAME;
	locOFN.lpstrInitialDir = NULL;
	locOFN.lpstrTitle = "Save as...";
	locOFN.Flags = OFN_OVERWRITEPROMPT;
	locOFN.lpstrDefExt = locExt;
	locOFN.lCustData = 0;
	locOFN.lpfnHook = NULL;
	locOFN.lpTemplateName = NULL;

	if (GetSaveFileName(&locOFN) == FALSE)
    {
        wsprintf(locStr,"Cannot open '%s' for writing.", locOFN.lpstrFile);
        MessageBox(hWnd, locStr, "Error", MB_ICONERROR | MB_OK);
        return;
    }

    if (locOFN.Flags&OFN_EXTENSIONDIFFERENT)
    {
        /* Force correct extension */
        locOFN.lpstrFile[lstrlen(locOFN.lpstrFile)-3] = locExt[0];
        locOFN.lpstrFile[lstrlen(locOFN.lpstrFile)-2] = locExt[1];
        locOFN.lpstrFile[lstrlen(locOFN.lpstrFile)-1] = locExt[2];
    }

    switch (wId)
    {
    case MENU_FILE_WRITE1:
	case MENU_FILE_WRITE4:
	case MENU_FILE_WRITE8:
	case MENU_FILE_WRITE24:
        DoSavePageToBitmap(wId, hWnd, locOFN.lpstrFile);
        break;

	case MENU_FILE_WRITEWMF:        
	case MENU_FILE_WRITEEMF:        
        DoSavePageToMetafile(wId, hWnd, locOFN.lpstrFile);
        break;
    }
}



/*
 |  DoPrintDP()
 |  Use DrawPage to print the document.
 */

static VOID   DoPrintDP()
{
    PRINTDLG   pd;

    memset(&pd, 0, sizeof(PRINTDLG));
    pd.lStructSize = sizeof(PRINTDLG);
    pd.hwndOwner = hViewWnd;
    pd.Flags = PD_NOPAGENUMS | PD_NOSELECTION | PD_RETURNDC;
    if(PrintDlg(&pd))
    {
        /* Get printer name (for CreateIC). */
        LPDEVNAMES   lpDevNames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
        if(lpDevNames)
        {
            LPSTR   szDriver, szDevice, szOutput;
            HDC     hICPrinter;

            /*
            |   Don't deinit DrawPage until it's certain that printing
            |   will proceed.
            |   Fixes  USER100100074  jrw 12/19/05
            */
            SendMessage(hViewWnd, SCCVW_DEINITDRAWPAGE, 0, 0);

            szDriver = (LPSTR)lpDevNames + lpDevNames->wDriverOffset;
            szDevice = (LPSTR)lpDevNames + lpDevNames->wDeviceOffset;
            szOutput = (LPSTR)lpDevNames + lpDevNames->wOutputOffset;

            /* Get an information context for the printer. */
            hICPrinter = CreateIC(szDriver, szDevice, szOutput, NULL);

            /* Print the document. */
            if(hICPrinter && pd.hDC)
            {
                PrintDoc(pd.hDC, hICPrinter);

                DeleteDC(hICPrinter);
                DeleteDC(pd.hDC);
            }

            GlobalUnlock(pd.hDevNames);
            GlobalFree(pd.hDevNames);

            /* Completely re-initialize */
            SendMessage(hViewWnd, SCCVW_INITDRAWPAGE, 0, 0);
            dwCurrentPage = SCC_INVALIDPAGE;
            dwNewPage = 0;
            dwMaxPage = 0;
            bMaxFound = FALSE;
            dwGoToPage = SCC_INVALIDPAGE;
        }
    }
}

/*
 |  PrintDoc()
 |  Use DRAWPAGE to print each page of the current document.
 |
 |      hDCPrinter - device context for the printer (used as the DRAWPAGE output DC)
 |      hICPrinter - information context for the printer (used as the DRAWPAGE format DC)
 */

static VOID    PrintDoc(HDC hDCPrinter, HDC hICPrinter)
{
    DOCINFO           di;
    char              szDocName[16];
    int               nDPI, nPageWidth, nPageHeight;
    LRESULT           lResult;
    SCCVWDRAWPAGE41   locDrawPage;

    /* Make sure we have a valid view window. */
    if(!IsWindow(hViewWnd))
        return;

    /* Start the print job. */
    memset(&di, 0, sizeof(DOCINFO));
    di.cbSize = sizeof(DOCINFO);
    lstrcpy(szDocName, "DrawPage Sample");
    di.lpszDocName = szDocName;
    if(StartDoc(hDCPrinter, (LPDOCINFO)&di) < 0)
        return;

    /* Get page dimensions. */
    nDPI = GetDeviceCaps(hDCPrinter, LOGPIXELSX);
    nPageWidth = GetDeviceCaps(hDCPrinter, HORZRES);
    nPageHeight = GetDeviceCaps(hDCPrinter, VERTRES);

    /* Set up DRAWPAGE. */
    SendMessage(hViewWnd, SCCVW_INITDRAWPAGE, 0, 0);

    memset(&locDrawPage, 0, sizeof(SCCVWDRAWPAGE41));
    locDrawPage.dwSize = sizeof(SCCVWDRAWPAGE41);
    locDrawPage.dwPageToDraw = 0;
    locDrawPage.dwReserved = 0;
    locDrawPage.dwFlags = SCCVW_DPFLAG_DETERMINEOUTPUTTYPE;
    locDrawPage.lUnitsPerInch = 1440;
    locDrawPage.lFormatWidth = (VTLONG)( (float)nPageWidth/(float)nDPI* (float)1440 );
    locDrawPage.lFormatHeight = (VTLONG)( (float)nPageHeight/(float)nDPI* (float)1440 );
    locDrawPage.hOutputDC = hDCPrinter;
    locDrawPage.hFormatDC = hICPrinter;

    /* Loop through each page in the document. */
    do
    {
        locDrawPage.lTop = 0;
        locDrawPage.lLeft = 0;
        locDrawPage.lBottom = nPageHeight;
        locDrawPage.lRight = nPageWidth;
        locDrawPage.hOutputDC = hDCPrinter;

        StartPage(hDCPrinter);
        lResult = SendMessage(hViewWnd, SCCVW_DRAWPAGE, 0, (LPARAM)(PSCCVWDRAWPAGE41)&locDrawPage);
        EndPage(hDCPrinter);

        if(lResult == SCCVWERR_OK)
            locDrawPage.dwPageToDraw++;
    }while(lResult == SCCVWERR_OK);

    EndDoc(hDCPrinter);

    SendMessage(hViewWnd, SCCVW_DEINITDRAWPAGE, 0, 0);
    return;
}

WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
    switch (message)
        {
        case WM_INITDIALOG:
            {

            char    buffer[512];

            // Display sample app. version
            lstrcpy(buffer, "Page Sample Application\n");
            lstrcat(lstrcat(buffer, S_PRODUCT), "\n");

            lstrcat(lstrcat(buffer, "Build:\t"), S_FILEVERSION);
            SetDlgItemText(hDlg,HELP_ABOUTBOXVERINFO,buffer);
            }
            return (TRUE);
        case WM_COMMAND:
            {

            switch(wParam)
                {
                case IDOK:

                    EndDialog(hDlg, 0);
                    return (TRUE);

                default:
                    break;
                }
            }
            break;

        default:
            break;
        }

    return(FALSE);
}

INT_PTR CALLBACK GoToDialogProc(
    HWND hwndDlg,  // handle to dialog box
    UINT uMsg,     // message
    WPARAM wParam, // first message parameter
    LPARAM lParam  // second message parameter
)
{
    BOOL  bRet = FALSE;  /* return value */

    UNUSED(lParam);

    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            char rtext[20];
            if (bMaxFound)
                sprintf(rtext, "1 - %d", dwMaxPage + 1);
            else
                rtext[0] = 0;
            SetDlgItemText(hwndDlg, IDC_RANGE, rtext);
            SetDlgItemInt(hwndDlg ,IDC_PAGENUM, dwCurrentPage + 1, FALSE);
            bRet = TRUE;
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            dwGoToPage = GetDlgItemInt(hwndDlg, IDC_PAGENUM, NULL, FALSE);

            if ((dwGoToPage == 0) ||
                (bMaxFound && ((dwGoToPage - 1) > dwMaxPage)))
            {
                /* The requested page is outside the range of pages in this
                 *   document; report an error
                 */
                char  szErr[128];

                wsprintf(szErr, "Page out of range: %d (max: %d)", dwGoToPage, (dwMaxPage + 1));
                MessageBox(hwndDlg, szErr, "Error", MB_ICONWARNING | MB_OK);

                dwGoToPage = SCC_INVALIDPAGE;
            }
            else
            {
                /* Either the requested page is within the range of pages in
                 *   this document, or we don't yet know what the maximum
                 *   page is; accept it
                 */
                dwGoToPage--;
                EndDialog(hwndDlg, wParam);
            }
            bRet = TRUE;
            break;
        case IDCANCEL:
            EndDialog(hwndDlg, wParam);
            bRet = TRUE;
            break;
        }
        break;
    }

    return (bRet);
}

INT_PTR CALLBACK ColorDialogProc(
  HWND hwndDlg,  // handle to dialog box
  UINT uMsg,     // message
  WPARAM wParam, // first message parameter
  LPARAM lParam  // second message parameter
)
{
    int i;
    DWORD rgb;
    char buffer[40];
    HWND hlb = GetDlgItem(hwndDlg,IDC_COLORNAME);
    UNUSED(lParam);


    switch(uMsg)
    {
    case WM_INITDIALOG:
        {
            /* Init the dialog title */
            sprintf(buffer,"Annotation %s Color",dialogColorName);
            SetWindowText(hwndDlg,buffer);

            /* Init the "By Name" combo box */
            rgb = (dialogColor.rgbRed << 16)
                + (dialogColor.rgbGreen << 8)
                + (dialogColor.rgbBlue);

            sprintf(buffer,"RGB 0x%08X",rgb);
            SendMessage(hlb,CB_ADDSTRING,0,(LPARAM)buffer);
            for (i = 1; i < NUM_COLORNAMES; i++)
                SendMessage(hlb,CB_ADDSTRING,0,(LPARAM)colorNames[i]);

            SendMessage(hlb,CB_SETCURSEL,(WPARAM)dialogColor.rgbReserved,0);
            return TRUE;
        }
        break;

    case WM_COMMAND:
        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
            DWORD index = (DWORD)SendMessage(hlb,CB_GETCURSEL,0,0);
            if (index)
                dialogColor = colorTable[index];
            return TRUE;
        }
        else switch(LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            EndDialog(hwndDlg,wParam);
            return TRUE;

        case IDC_CHOOSER:
            {
                CHOOSECOLOR cc;
                static COLORREF stock[16];
                static BOOL bStockInit = FALSE;
                int j;

                if (!bStockInit)
                {
                    bStockInit = TRUE;
                    for (i = 0, j = 1; i < 16; i++, j++)
                        stock[i] = RGB(colorTable[j].rgbRed,colorTable[j].rgbGreen,colorTable[j].rgbBlue);
                }

                memset(&cc,0,sizeof(cc));
                cc.lStructSize = sizeof(cc);
                cc.hwndOwner = hwndDlg;
                cc.rgbResult = RGB(dialogColor.rgbRed,dialogColor.rgbGreen,dialogColor.rgbBlue);
                cc.lpCustColors = stock;
                cc.Flags = CC_RGBINIT | CC_FULLOPEN;
                if (ChooseColor(&cc))
                {
                    dialogColor.rgbRed      = GetRValue(cc.rgbResult);
                    dialogColor.rgbGreen    = GetGValue(cc.rgbResult);
                    dialogColor.rgbBlue     = GetBValue(cc.rgbResult);
                    dialogColor.rgbReserved = 0;

                    rgb = (dialogColor.rgbRed << 16)
                        + (dialogColor.rgbGreen << 8)
                        + (dialogColor.rgbBlue);

                    sprintf(buffer,"RGB 0x%08X",rgb);
                    SendMessage(hlb,CB_DELETESTRING,0,0);
                    SendMessage(hlb,CB_INSERTSTRING,0,(LPARAM)buffer);

                    j = 0;
                    rgb = *(DWORD *)&dialogColor;

                    for (i = 1; i < 16; i++)
                    {
                        if (rgb == ((*(DWORD *)&colorTable[i]) & 0x00ffffff))
                        {
                            j = i;
                            dialogColor.rgbReserved = (BYTE)i;
                            break;
                        }
                    }


                    SendMessage(hlb,CB_SETCURSEL,j,0);
                }
                return TRUE;
            }
        }
        break;
    }
    return FALSE;
}

/*-----------------------------------------------------------------------
    Copied from annoex.c

	This function demonstrates the SCCVW_HILITETEXT option of the
	SCCVW_ADDANNOTATION message.  The currently selected text is
	hilighted in one of two color schemes based on the menu choice.
-----------------------------------------------------------------------*/

VOID DoHighlight(HWND hWnd)
{
    SCCVWFINDPOSITION41   sFindPos;
    SCCVWSETSELECTION41   sSetPos;
    SCCVWPOS              locStart, locEnd;
    SCCVWHILITETEXT41     locHilite;
    SCCVWCOMPPOSITIONS41  sCompPos;
    UNUSED(hWnd);

    sFindPos.dwSize = sizeof ( sFindPos );
    sFindPos.dwFindOptions = SCCVW_FINDANCHORSELECTPOS;
    if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM) (PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
        return;

    locStart = sFindPos.sResultPos;
    sFindPos.dwFindOptions = SCCVW_FINDENDSELECTPOS;
    if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM) (PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
        return;

    locEnd = sFindPos.sResultPos;

    sCompPos.pPosA = &locStart;
    sCompPos.pPosB = &locEnd;

    if ( SendMessage ( hViewWnd, SCCVW_COMPPOSITIONS, 0, (LPARAM)(PSCCVWCOMPPOSITIONS41)(&sCompPos) ) != 0 )
    {
        SCCVWMAPPOSITION41	sMapPos;
        sMapPos.dwSize = sizeof(sMapPos);
        sMapPos.dwMapOptions = SCCVW_MAPPOSTOACC;
        sMapPos.sPos = locStart;
        SendMessage(hViewWnd,SCCVW_MAPPOSITION,0,(LPARAM)(PSCCVWMAPPOSITION41)&sMapPos);

        locHilite.dwSize = sizeof ( locHilite );
        locHilite.dwUser = 0;
        locHilite.sStartPos = locStart;
        locHilite.sEndPos = locEnd;
        locHilite.dwData = 0;
        locHilite.dwDisplay = SCCVW_USESTYLE;

        locHilite.dwInteraction = SCCVW_EVENTSINGLECLICK | SCCVW_EVENTDOUBLECLICK | SCCVW_EVENTTRANSITIONINTO | SCCVW_EVENTTRANSITIONOUTOF;
        SendMessage(hViewWnd,SCCVW_ADDANNOTATION,SCCVW_HILITETEXT,(LPARAM)(PSCCVWHILITETEXT40)&locHilite);
    }

    sSetPos.dwSize = sizeof (SCCVWSETSELECTION41);
    sSetPos.sAnchorPos = sSetPos.sEndPos = locStart;
    SendMessage(hViewWnd,SCCVW_SETSELECTION,0,(LPARAM)(PSCCVWSETSELECTION41)&sSetPos);
    bHaveAnnos = TRUE;
}

VOID SetHighlightColors(VOID)
{
    SCCVWHILITESTYLE41 style;

    style.dwSize = sizeof(style);
    style.dwStyleId = 0;
    style.dwOptions = SCCVW_USEFOREGROUND | SCCVW_USEBACKGROUND;
    style.sForeground = SCCVWRGB(fColor.rgbRed,fColor.rgbGreen,fColor.rgbBlue);
    style.sBackground = SCCVWRGB(bColor.rgbRed,bColor.rgbGreen,bColor.rgbBlue);
    style.wCharAttr = 0;

    SendMessage(hViewWnd,SCCVW_HILITESTYLE,0,(LPARAM)&style);
}

/* Copied from edit.c */
/*----------------------------------------------------------------------
    This function demonstrates the SCCVW_HIDETEXT option of the
    SCCVW_ADDANNOTATION message.  The currently selected text is
    hidden.
----------------------------------------------------------------------*/

VOID  DoHideSelection( HWND hWnd )
{
    SCCVWFINDPOSITION41    sFindPos;
    SCCVWSETSELECTION41    sSetPos;
    SCCVWCOMPPOSITIONS41   sCompPos;
    SCCVWPOS               locStart, locEnd;
    SCCVWHIDETEXT80        locHide;
    LONG                   lCompResult = 0;
    UNUSED(hWnd);

    sFindPos.dwSize = sizeof ( SCCVWFINDPOSITION41 );
    sFindPos.dwFindOptions = SCCVW_FINDANCHORSELECTPOS;
    if( SendMessage( hViewWnd, SCCVW_FINDPOSITION, 0, (LPARAM)&sFindPos ) != SCCVWERR_OK )
        return;

    locStart = sFindPos.sResultPos;

    sFindPos.dwFindOptions = SCCVW_FINDENDSELECTPOS;
    if( SendMessage( hViewWnd, SCCVW_FINDPOSITION, 0, (LPARAM)&sFindPos ) != SCCVWERR_OK )
        return;

    locEnd = sFindPos.sResultPos;

    sCompPos.pPosA = &locStart;
    sCompPos.pPosB = &locEnd;
    lCompResult = (LONG)SendMessage( hViewWnd, SCCVW_COMPPOSITIONS, 0, (LPARAM)&sCompPos );

    if( lCompResult != 0 )
    {
        locHide.dwSize    = sizeof ( SCCVWHIDETEXT80 );
        locHide.dwUser    = 0;
        locHide.sStartPos = ( lCompResult < 0 ? locStart : locEnd );
        locHide.sEndPos   = ( lCompResult < 0 ? locEnd   : locStart );
        locHide.dwData    = 0;
        SendMessage( hViewWnd, SCCVW_ADDANNOTATION, SCCVW_HIDETEXT, (LPARAM)&locHide );
    }

    sSetPos.dwSize = sizeof (SCCVWSETSELECTION41);
    sSetPos.sAnchorPos = sSetPos.sEndPos = locStart;
    SendMessage( hViewWnd, SCCVW_SETSELECTION, 0, (LPARAM)&sSetPos );
}

/*
 |  ResetDrawPage
 |  Re-initializes DrawPage. We need to do this if adding or deleting an
 |  annotation might require a re-wrap of the document.
 */

VOID   ResetDrawPage()
{
    SendMessage( hViewWnd, SCCVW_DEINITDRAWPAGE, 0, 0 );
    SendMessage( hViewWnd, SCCVW_INITDRAWPAGE, 0, 0 );
}

/*
 |  DrawPageErrorBox
 |
 |  Report any DrawPage errors via Win32 MessageBox
 */
VOID DrawPageErrorBox (LRESULT lResult, DWORD dwPage)
{
    char  szErrorMessage[128];  /* error string buffer */

    BOOL  bShowError = TRUE;  /* flag if we should show an error */

    /* We assume that the incoming page number is a 0-based index and
     *   update it here
     */
    dwPage++;

    /* Generate the correct error message */
    switch (lResult)
    {
    case SCCVWERR_NONEFOUND:
    case SCCVWERR_NOPAGE:
        wsprintf(szErrorMessage, "Could not get information for page %d.", dwPage);
        break;
    case SCCVWERR_ALLOCFAILED:
        wsprintf(szErrorMessage, "Could not allocate memory.");
        break;
    case SCCVWERR_NOFILE:
        wsprintf(szErrorMessage, "No file open.");
        break;
    case SCCVWERR_NOINIT:
        wsprintf(szErrorMessage, "DrawPage not initialized.");
        break;
    case SCCVWERR_FEATURENOTAVAIL:
        wsprintf(szErrorMessage, "DrawPage feature unavailable.");
        break;
    case SCCVWERR_LASTPAGE:
        wsprintf(szErrorMessage, "The document has %d page(s).", dwPage);
        break;
    case SCCVWERR_OK:
        bShowError = FALSE;
        break;
    default:
        wsprintf(szErrorMessage, "Could not get draw page %d.", dwPage);
        break;
    }

    /* Output the error, if necessary */
    if (bShowError)
        MessageBox(hMainWnd, szErrorMessage, "DrawPage Error", MB_ICONWARNING | MB_OK);
}
