/*
|  Outside In Viewer Technology
|  Sample Application
|  
|
|  SEARCH
|  Viewer Technology sample application
|
|  Purpose:
|  Show how to use the viewer technology's search messages.
|
|
|  Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.
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
|  Defines
|  Defines
|
*/

#define DEFAULTOEMID  TEXT("99")

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

#include "sccvw.h"

#include "search.h"
#include "search.pro"
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

/*
|
| Defines
| Defines
| Defines
|
*/

HANDLE      hInst;            /* Handle of the current instance */
HWND        hMainWnd;         /* Handle to top level window */
HWND        hViewWnd;         /* Handle to the view window */
TCHAR		szExePath[256];   /* Path to this executable */
HANDLE      hViewerLibrary;   /* Handle to SCCVW.DLL */

/*
|
|  Routines
|  Routines
|  Routines
|
*/

#include "scclink.c"

int WINMAIN_ENTRYMOD WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    MSG  locMsg;

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
        WndClass.hIcon = LoadIcon(hInstance,TEXT("SCC_ICON"));
        WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
        WndClass.hbrBackground = GetStockObject(LTGRAY_BRUSH);
        WndClass.lpszMenuName = TEXT("SCC_MENU");
        WndClass.lpszClassName = TEXT("SCC_MAIN");

        if (!RegisterClass(&WndClass)) return(0);
    }

    /*
    | Save instance in global
    */

    hInst = hInstance;


    /*
    | Create main window
    */

    hMainWnd = CreateWindow(
        TEXT("SCC_MAIN"),                      /* window class    */
        TEXT("OIVT Search Sample"),                     /* window name      */
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

    while (GetMessage(&locMsg,NULL,0,0))
    {
        TranslateMessage(&locMsg);
        DispatchMessage(&locMsg);
    }

    return (int)(locMsg.wParam);     /* Returns the value from PostQuitMessage */
}


WIN_ENTRYSC LRESULT WIN_ENTRYMOD SccWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WORD   locId;
    WORD   locEvent;
    HWND    locCtrl;
    LRESULT  locSearchRet = 0;

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

    case WM_COMMAND:

        locId    = LOWORD(wParam);
        locEvent = HIWORD(wParam);
        locCtrl  = (HWND) lParam;

        switch(locId)
        {
        case MENU_FILE_OPEN:

            DoOpenFile(hWnd);
            break;

        case MENU_FILE_CLOSE:

            DoCloseFile(hWnd);
            break;

        case MENU_FILE_SEARCH:

            locSearchRet = SendMessage(hViewWnd,SCCVW_SEARCHDIALOG,0,0L);
            break;

        case MENU_FILE_SEARCHTHE:

            {
                SCCVWSEARCHINFO40  locSearchInfo;

                locSearchInfo.dwSize = sizeof(SCCVWSEARCHINFO40);
                lstrcpy(locSearchInfo.siText,TEXT("The"));
                locSearchInfo.siTextLen = 3;
                locSearchInfo.siType = SCCVW_SEARCHNOCASE;
                locSearchInfo.siFrom = SCCVW_SEARCHCURRENT;
                locSearchInfo.siDirection = SCCVW_SEARCHFORWARD;

                locSearchRet = SendMessage(hViewWnd,SCCVW_SEARCH,0,(LPARAM)(PSCCVWSEARCHINFO40)&locSearchInfo);
            }
            break;

        case MENU_FILE_SEARCHNEXT:

            locSearchRet = SendMessage(hViewWnd,SCCVW_SEARCHNEXT,SCCVW_SEARCHFORWARD,0L);

            break;

        case MENU_FILE_SEARCHPREV:

            locSearchRet = SendMessage(hViewWnd,SCCVW_SEARCHNEXT,SCCVW_SEARCHBACK,0L);

            break;

        case MENU_HELP_ABOUT:

            DialogBox(hInst, TEXT("HELP_ABOUTBOX"), hViewWnd, (DLGPROC)HelpAbout);

            break;

        default:
            return (DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;

    default:
        return (DefWindowProc(hWnd, message, wParam, lParam));
        break;
    }

	if (locSearchRet != 0)
	{
		/* We used a search function, and didn't get an SCCERR_OK result. */
		if (locSearchRet == 1)
			MessageBox(hWnd,TEXT("No search hit found."),TEXT("Search Result"),MB_OK);
		else
			MessageBox(hWnd,TEXT("Search has encounted an error."),TEXT("Search Result"),MB_OK);
	}


    return (0L);
}

/*
|  Handle WM_CREATE
*/

VOID DoCreate(HWND hWnd)
{
    TCHAR locViewerPath[256];
    TCHAR locViewerClass[256];

    /*
    |  Load the Outside In Viewer Technology DLL (SCxxVW.DLL)
    |
    |  This code loads SCxxVW.DLL from the same directory
    |  the sample application is in. It uses the two routines
    |  SCCGetExePath and SCCLoadViewerDLL that are in SCCLINK.C.
    |  SCCLINK.C is #included above (just before WinMain).
    */

    SCCGetExePath(hInst,szExePath,256);
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

    lstrcpy(locViewerClass,TEXT("SCCVIEWER"));

    hViewWnd = CreateWindow(locViewerClass,
        NULL,
        WS_CHILD | WS_BORDER | WS_VISIBLE | WS_CLIPCHILDREN,
        0, 
        0, 
        0,
        0,
        hWnd,
        0, 
        hInst, 
        NULL);


}

/*
|  Handle WM_SIZE
*/

VOID DoSize(HWND hWnd,WORD wWidth,WORD wHeight)
{
    if (IsWindow(hViewWnd))
    {
        InvalidateRect(hWnd,NULL,0);
        MoveWindow(hViewWnd,20,20,wWidth-40,wHeight-40,TRUE);
        ShowWindow(hViewWnd,SW_SHOW);
    }
}

/*
|  Handle WM_DESTROY
*/

VOID DoDestroy(HWND hWnd)
{
    UNUSED(hWnd);
    if (IsWindow(hViewWnd))
    {
        SendMessage(hViewWnd,SCCVW_CLOSEFILE,0,0L);
        DestroyWindow(hViewWnd);
    }

    if (hViewerLibrary != NULL)
    {
        FreeLibrary(hViewerLibrary);
    }
}

/*
|  Handle Close menu item
*/

VOID DoCloseFile(HWND hWnd)
{
    UNUSED(hWnd);
    if (IsWindow(hViewWnd))
    {
        SendMessage(hViewWnd,SCCVW_CLOSEFILE,0,0L);
        InvalidateRect(hViewWnd,NULL,TRUE);
    }
}

/*
|  Handle SCCVW_FILECHANGE message
*/

VOID DoFileChange(HWND hWnd)
{
    SCCVWFILEINFO84  locFileInfo;
    char *defStr = "OIVT Search Sample";

    locFileInfo.dwSize = sizeof(locFileInfo);

    if (SendMessage(hViewWnd, SCCVW_GETFILEINFO, 0, (LPARAM)&locFileInfo) == SCCVWERR_OK)
    {
        char *locStr = (char *)malloc(wcslen(locFileInfo.szDisplayName)+wcslen(locFileInfo.szFileIdName)+24);   /* big enough to hold everything */
        if (locStr)
            wsprintf(locStr,"OIVT Search - %S - %S",locFileInfo.szDisplayName,locFileInfo.szFileIdName);
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

VOID DoOpenFile(HWND hWnd)
{
#define MAXFILENAME 256

    OPENFILENAME  locOFN;
    TCHAR		locFilterSpec[128] = TEXT("All files\0*.*\0");
    TCHAR		locFileName[MAXFILENAME];
    TCHAR		locFileTitle[MAXFILENAME];
    SCCVWOPTIONSPEC40  locOption;
    VTDWORD            locFlags;

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
    locOFN.lpstrTitle = TEXT("Open...");
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

            locViewFile.dwSize = sizeof(SCCVWVIEWFILE40);
            locViewFile.dwSpecType = IOTYPE_ANSIPATH;
            locViewFile.pSpec = (VTVOID *)locFileName;
            locViewFile.dwViewAs = 0;
            locViewFile.bUseDisplayName = FALSE;
            locViewFile.bDeleteOnClose = FALSE;
            locViewFile.dwFlags = 0;
            locViewFile.dwReserved1 = 0;
            locViewFile.dwReserved2 = 0;
            locFlags = CS_ASCII;
            locOption.dwSize  = sizeof(SCCVWOPTIONSPEC40);
            locOption.dwId    = SCCID_DEFAULTINPUTCHARSET;
            locOption.dwFlags = SCCVWOPTION_CURRENT | SCCVWOPTION_DEFAULT;
            locOption.pData   = (VTVOID *)&locFlags;

            SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);

            SendMessage(hViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&locViewFile);

            SetFocus(hViewWnd);
        }
    }
}


WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {

            TCHAR	buffer[512];

            // Display sample app. version
            lstrcpy(buffer, TEXT("Search Sample Application\n"));
            lstrcat(lstrcat(buffer, TEXT(S_PRODUCT)), TEXT("\n"));

            lstrcat(lstrcat(buffer, TEXT("Build:\t")), TEXT(S_FILEVERSION));
            SetDlgItemText(hDlg,HELP_ABOUTBOXVERINFO,buffer);
        }
        return (TRUE);
    case WM_COMMAND:
        {

            switch(wParam)
            {
            case IDCANCEL:
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
