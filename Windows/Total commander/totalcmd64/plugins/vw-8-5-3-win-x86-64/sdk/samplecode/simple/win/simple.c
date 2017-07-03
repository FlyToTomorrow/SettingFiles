
/*
|  Outside In Viewer Technology
|  Sample Application
|  
|
|  SIMPLE
|  Viewer Technology sample application
|
|  Purpose:
|  To show the simplest possible implementation of the viewer technology
|
|
|  Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.
|
|  You have a royalty-free right to use, modify, reproduce and
|  distribute the Sample Applications (and/or any modified version)
|  in any way you find useful, provided that you agree that Oracle 
|  has no warranty obligations or liability for any
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

#ifdef _DEBUG
#define WIN32DEBUG
#endif
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
#include <tchar.h>

#include "sccvw.h"

#include "simple.h"
#include "simple.pro"
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
|  Globals
|  Globals
|  Globals
|
*/

HANDLE      hInst;            /* Handle of the current instance */
HWND        hMainWnd;         /* Handle to top level window */
HWND        hViewWnd;         /* Handle to the view window */
TCHAR       szExePath[256];   /* Path to this executable */
HANDLE      hViewerLibrary;   /* Handle to SCCVW.DLL */

TCHAR		*cmdLine = NULL;



/*
|
|  Routines
|  Routines
|  Routines
|
*/

#include "scclink.c"

void ViewFile(LPTSTR filename);

LPTSTR DBtoSB(LPTSTR dest, LPWORD src)
{
    LPTSTR d = dest;
    LPWORD s = src;
    while (*s)
        *d++ = (TCHAR)*s++;
    *d = 0;
    return dest;
}


VTVOID QVShowNum(LPTSTR Message, int DisplayNum) 
{
    TCHAR   NumString[30];
    TCHAR   OutString[60];

    _itot(DisplayNum, NumString, 10);
    lstrcpy(OutString, Message);
    lstrcat(OutString, NumString);

    MessageBox(NULL, OutString, TEXT("Message"), MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON1 | MB_APPLMODAL);
}

int WINMAIN_ENTRYMOD WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    MSG  locMsg;

    UNUSED(lpCmdLine);
    UNUSED(nCmdShow);

    /*
    | Register window class if necessary
    */

    if (ARGC > 1)
        cmdLine = ARGV(1);

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


        if (!RegisterClass(&WndClass)) {
            // lasterr = GetLastError();
            return(0);
        }
    }

    /*
    | Save instance in global
    */

    hInst = hInstance;

    /*
    | Create main window
    */
    hMainWnd = CreateWindow(
        (LPTSTR) TEXT("SCC_MAIN"),                   /* window class    */
        TEXT("OIVT Simple Sample"),                  /* window name      */
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,/* window type     */
        CW_USEDEFAULT,                        /* x position       */
        CW_USEDEFAULT,                        /* y position     */
        CW_USEDEFAULT,                        /* width           */
        CW_USEDEFAULT,                        /* height        */
        NULL,                                 /* parent handle   */
        NULL,                                 /* menu or child ID*/
        hInstance,                            /* instance        */
        NULL);                                /* additional info */



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

    return ((int)locMsg.wParam);     /* Returns the value from PostQuitMessage */
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

    case WM_HELP:		  
        DialogBox(hInst, TEXT("HELP_ABOUTBOX"), hViewWnd, (DLGPROC)HelpAbout);
        break;

    case WM_CREATE:

        if (!DoCreate(hWnd))
            DestroyWindow(hWnd);
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

        case MENU_FILE_PRINT:

            SendMessage(hViewWnd,SCCVW_PRINT,0,0);
            break;

        case MENU_EDIT_COPY:

            SendMessage(hViewWnd,SCCVW_COPYTOCLIP,0,0L);
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

    return (0L);
}

/*
|  Handle WM_CREATE
*/

BOOL DoCreate(HWND hWnd)
{
    TCHAR  locViewerPath[256];
    TCHAR  locViewerClass[256];

    VTLONG temp = 0;

    LRESULT result;

    /*
    |  Load the Outside In Viewer Technology DLL (SCxxVW.DLL)
    |
    |  This code loads SCxxVW.DLL from the same directory
    |  the sample application is in. It uses the two routines
    |  SCCGetExePath and SCCLoadViewerDLL that are in SCCLINK.C.
    |  SCCLINK.C is #included above (just before WinMain).
    */

    SCCGetExePath(hInst,(LPTSTR)szExePath,256);
    lstrcpy(locViewerPath,szExePath);
    hViewerLibrary = SCCLoadViewerDLL(locViewerPath);

    /*
    |  If the Viewer Technology can not be loaded, bail out.
    */

    if (hViewerLibrary == NULL)
    {

        DWORD	WinErr;
        WinErr = GetLastError();
        QVShowNum(TEXT("Failed to load viewer DLL - "),WinErr);

        WinErr = 0;
        return FALSE;
    }

    /*
    |  Create the view window here
    */

    lstrcpy((LPTSTR)locViewerClass,TEXT("SCCVIEWER") );

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

    if(hViewWnd == NULL)
    {
        return FALSE;
    }

    result = SendMessage(hViewWnd, SCCVW_SETIDLEBITMAP, (WPARAM) hInst, temp | IDLEBITMAP);


    if (cmdLine)
        ViewFile(cmdLine);

    return TRUE;
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
    char *defStr = "OIVT Simple Sample";

    locFileInfo.dwSize = sizeof(locFileInfo);

    if (SendMessage(hViewWnd, SCCVW_GETFILEINFO, 0, (LPARAM)&locFileInfo) == SCCVWERR_OK)
    {
        char *locStr = (char *)malloc(wcslen(locFileInfo.szDisplayName)+wcslen(locFileInfo.szFileIdName)+24);   /* big enough to hold everything */
        if (locStr)
            wsprintf(locStr,"OIVT Simple - %S - %S",locFileInfo.szDisplayName,locFileInfo.szFileIdName);
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

void ViewFile(LPTSTR filename)
{
    if (IsWindow(hViewWnd))
    {

        SCCVWVIEWFILE40  locViewFile;

        locViewFile.dwSize = sizeof(SCCVWVIEWFILE40);
        locViewFile.dwSpecType = IOTYPE_ANSIPATH;
        locViewFile.pSpec = (VTVOID *)filename;
        locViewFile.dwViewAs = 0;
        locViewFile.bUseDisplayName = FALSE;
        locViewFile.bDeleteOnClose = FALSE;
        locViewFile.dwFlags = 0;
        locViewFile.dwReserved1 = 0;
        locViewFile.dwReserved2 = 0;

        SendMessage(hViewWnd,SCCVW_VIEWFILE,0,(LPARAM)&locViewFile);

        SetFocus(hViewWnd);
    }
}

/*
|  Handle Open menu item
*/

VOID DoOpenFile(HWND hWnd)
{
#define MAXFILENAME 256

    OPENFILENAME  locOFN;
    TCHAR        locFilterSpec[128] = TEXT("All files\0*.*\0");
    TCHAR        locFileName[MAXFILENAME];
    TCHAR        locFileTitle[MAXFILENAME];

    locFileName[0] = 0;
    locFileTitle[0] = 0;


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
        ViewFile(locFileName);
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
            lstrcpy(buffer, TEXT("Simple Sample Application\n"));
            lstrcat(lstrcat(buffer, TEXT(S_PRODUCT)), TEXT("\n"));

            lstrcat(lstrcat(buffer, TEXT("Build:\t")), TEXT(S_FILEVERSION));
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

