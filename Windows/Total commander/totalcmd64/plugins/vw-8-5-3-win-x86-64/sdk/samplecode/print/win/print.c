/*
|  Outside In Viewer Technology
|  Sample Application
|  
|
|  PRINT
|  Viewer Technology sample application
|
|  Purpose:
|  To show the various ways to print from the viewer technology
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

#define DEFAULTOEMID  "99"

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

#include "print.h"
#include "print.pro"
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
char        szExePath[256];   /* Path to this executable */
HANDLE      hViewerLibrary;   /* Handle to SCCVW.DLL */

/*
|
|  Routines
|  Routines
|  Routines
|
*/

#include "scclink.c"

int WINMAIN_ENTRYMOD WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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
        WndClass.hIcon = LoadIcon(hInstance,"SCC_ICON");
        WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
        WndClass.hbrBackground = GetStockObject(LTGRAY_BRUSH);
        WndClass.lpszMenuName = (LPSTR) "SCC_MENU";
        WndClass.lpszClassName = (LPSTR) "SCC_MAIN";

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
        (LPSTR) "SCC_MAIN",                   /* window class    */
        "OIVT Print Sample",                  /* window name      */
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

        case MENU_FILE_PRINTEX:

            DoPrintEx(hWnd);
            break;

        case MENU_FILE_PRINT_SETUP:

            SendMessage(hViewWnd, SCCVW_PRINTSETUP, 0, 0);
            break;

        case MENU_EDIT_COPY:

            SendMessage(hViewWnd,SCCVW_COPYTOCLIP,0,0L);
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
        break;
    }

    return (0L);
}

/*
|  Handle WM_CREATE
*/

VOID DoCreate(HWND hWnd)
{
    char  locViewerPath[256];
    char  locViewerClass[256];

    /*
    |  Load the Outside In Viewer Technology DLL (SCxxVW.DLL)
    |
    |  This code loads SCxxVW.DLL from the same directory
    |  the sample application is in. It uses the two routines
    |  SCCGetExePath and SCCLoadViewerDLL that are in SCCLINK.C.
    |  SCCLINK.C is #included above (just before WinMain).
    */

    SCCGetExePath(hInst,(VTLPSTR)szExePath,256);
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

    lstrcpy((LPSTR)locViewerClass,"SCCVIEWER");

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
    char *defStr = "OIVT Print Sample";

    locFileInfo.dwSize = sizeof(locFileInfo);

    if (SendMessage(hViewWnd, SCCVW_GETFILEINFO, 0, (LPARAM)&locFileInfo) == SCCVWERR_OK)
    {
        char *locStr = (char *)malloc(wcslen(locFileInfo.szDisplayName)+wcslen(locFileInfo.szFileIdName)+24);   /* big enough to hold everything */
        if (locStr)
            wsprintf(locStr,"OIVT Print - %S - %S",locFileInfo.szDisplayName,locFileInfo.szFileIdName);
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

VOID DoOpenFile(HWND hWnd)
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
        }
    }
}

/*
|  Print using our own print dialog and SCCVW_PRINTEX
*/

VOID DoPrintEx(HWND hWnd)
{
    PRINTDLG         locPrintDlg;
    SCCVWPRINTEX40   locPrintEx;

    locPrintDlg.lStructSize = sizeof(PRINTDLG);
    locPrintDlg.hwndOwner = hWnd;
    locPrintDlg.hDevMode = NULL;  /* Assume defaults - not saving options from job to job.*/
    locPrintDlg.hDevNames = NULL;
    locPrintDlg.hDC = NULL;
    locPrintDlg.nFromPage = locPrintDlg.nToPage = locPrintDlg.nMinPage = 1;
    locPrintDlg.nMaxPage = 10000;	//We must initialize our minimum and maximum page, or dialog box will disable pages button.  AHS 5/20/99
    locPrintDlg.Flags = PD_ALLPAGES | PD_RETURNDC |
        PD_USEDEVMODECOPIES;
    locPrintDlg.nCopies = 1;

    if (PrintDlg(&locPrintDlg))
    {
        LPDEVNAMES locDevNamesPtr = NULL;

        if (locPrintDlg.hDC != NULL)
        {
            DOCINFO	locDocInfo;
            char     locDocName[] = "Test SCCVW_PRINTEX";

            locDocInfo.cbSize = sizeof(DOCINFO);
            locDocInfo.lpszDocName = (LPSTR)locDocName;
            locDocInfo.lpszOutput = NULL;
            locDocInfo.lpszDatatype = NULL;
            locDocInfo.fwType = 0;

            StartDoc(locPrintDlg.hDC, &locDocInfo);

            locPrintEx.dwSize = sizeof(SCCVWPRINTEX40);
            locPrintEx.dwFlags = SCCVW_USEPRINTERDC | SCCVW_USECOPIES | SCCVW_USEDEVMODE;
            if (locPrintDlg.Flags & PD_SELECTION)
            {
                SCCVWOPTIONSPEC40 pOptionSpec;
                LONG TempVar=SCCVW_PRINT_SELECTION;
                pOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                pOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                pOptionSpec.pData = &TempVar;
                pOptionSpec.dwId = SCCID_WHATTOPRINT;
                SendMessage(hViewWnd, SCCVW_SETOPTION, 0, (LPARAM)(PSCCVWOPTIONSPEC40)&pOptionSpec);
            }
            if (locPrintDlg.Flags & PD_PAGENUMS)
            {
                SCCVWOPTIONSPEC40 pOptionSpec;

                LONG TempVar=SCCVW_PRINT_PAGERANGE;
                pOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                pOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                pOptionSpec.pData = &TempVar;
                pOptionSpec.dwId = SCCID_WHATTOPRINT;
                SendMessage(hViewWnd, SCCVW_SETOPTION, 0, (LPARAM)(PSCCVWOPTIONSPEC40)&pOptionSpec);
                pOptionSpec.dwId = SCCID_PRINTSTARTPAGE;
                TempVar = locPrintDlg.nFromPage;
                SendMessage(hViewWnd, SCCVW_SETOPTION, 0, (LPARAM)(PSCCVWOPTIONSPEC40)&pOptionSpec);
                pOptionSpec.dwId = SCCID_PRINTENDPAGE;
                TempVar = locPrintDlg.nToPage;
                SendMessage(hViewWnd, SCCVW_SETOPTION, 0, (LPARAM)(PSCCVWOPTIONSPEC40)&pOptionSpec);
            }

            /*if (locPrintDlg.Flags & PD_ALLPAGES)
            locPrintEx.dwFlags |= SCCVW_PRINT_ALLPAGES;
            else if (locPrintDlg.Flags & PD_SELECTION)
            locPrintEx.dwFlags |= SCCVW_PRINT_SELECTION;
            else if (locPrintDlg.Flags & PD_PAGENUMS)
            locPrintEx.dwFlags |= SCCVW_PRINT_PAGERANGE;*/
            locPrintEx.hParentWnd = hWnd;
            locPrintEx.hPrinterDC = locPrintDlg.hDC;
            locPrintEx.bDoSetupDialog = FALSE;
            locPrintEx.bDoAbortDialog = TRUE;
            locPrintEx.bStartDocAlreadyDone = TRUE;
            locPrintEx.dwCopies = 1;
            locPrintEx.hDevMode = locPrintDlg.hDevMode;
            if (locPrintDlg.hDevNames)
            {
                locDevNamesPtr = (LPDEVNAMES)GlobalLock(locPrintDlg.hDevNames);

            }

            if (locDevNamesPtr)
            {
                lstrcpy(locPrintEx.szDriver, ((char *)(locDevNamesPtr))+locDevNamesPtr->wDriverOffset);
                lstrcpy(locPrintEx.szPrinter, ((char *)(locDevNamesPtr))+locDevNamesPtr->wDeviceOffset);
                lstrcpy(locPrintEx.szPort, ((char *)(locDevNamesPtr))+locDevNamesPtr->wOutputOffset);
                locPrintEx.dwFlags |= SCCVW_USEPRINTERNAME;
            }
            else
            {
                lstrcpy(locPrintEx.szPrinter,"PrinterPJB");
                lstrcpy(locPrintEx.szPort,"PortPJB");
                lstrcpy(locPrintEx.szDriver,"DriverPJB");
            }

            SendMessage(hViewWnd,SCCVW_PRINTEX,0,(LPARAM)(SCCVWPRINTEX40 *)&locPrintEx);

            EndDoc(locPrintDlg.hDC);
            DeleteDC(locPrintDlg.hDC);
        }
    }

    if (locPrintDlg.hDevMode)
    {
        GlobalFree(locPrintDlg.hDevMode);
    }

    if (locPrintDlg.hDevNames)
    {
        GlobalFree(locPrintDlg.hDevNames);
    }

}


WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {

            char	buffer[512];

            // Display sample app. version
            lstrcpy(buffer, "Print Sample Application\n");
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
