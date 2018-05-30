/*
|  Outside In Viewer Technology
|  Sample Application
|  
|
|  WELCOME
|  Viewer Technology sample application
|
|  Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.
|
|  You have a royalty-free right to use, modify, reproduce and
|  distribute the Sample Applications (and/or any modified version)
|  in any way you find useful, provided that you agree that  
|  Oracle has no warranty obligations or liability for any
|  Sample Application files which are modified.
|
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

#include "welcome.h"
#include "welcome.pro"
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

HANDLE    hInst;                 // Program instance handle
HWND      hwndFrame = NULL;      // Handle to main window
HWND      hwndMDIClient = NULL;  // Handle to MDI client
LONG      styleDefault = 0;      // Default style bits for child windows
WNDPROC   MDIClientProc;
HMENU     hMenu;
DWORD     gFlags;

HANDLE    hViewerLibrary;        // Handle to SCxxVW.DLL

char      szFrame[] = "WELCOMEFRAME";  // Class name for "frame" window
char      szChild[40];                 // Class name for MDI window
char      szViewerPath[256];

char      szWelcomeFile[268];  

/* Flags for gFlags */
#define MDIV_AUTOSIZE   0x0001

WIN_ENTRYSC LONG WIN_ENTRYMOD BlandFrameWndProc 
(
 register HWND hwnd,
 UINT msg,
 register WPARAM wParam,
 LPARAM lParam
 )
{
    switch (msg)
    {
    case WM_CREATE:
        {
            CLIENTCREATESTRUCT ccs;

            // Find window menu where children will be listed 

            ccs.hWindowMenu = GetSubMenu(GetMenu(hwnd), WINDOWMENU);
            ccs.idFirstChild = IDM_WINDOWCHILD;

            // Create the MDI client 

            hwndMDIClient = CreateWindow("mdiclient", NULL, WS_CHILD |
                WS_CLIPCHILDREN, 0, 0, 0, 0, hwnd, 0, hInst, (LPSTR)&ccs);

            //Subclass MDI client so SCC messages can be passed to frame
#ifdef WIN64
            MDIClientProc = (WNDPROC)SetWindowLongPtr(hwndMDIClient,GWLP_WNDPROC,(LONG_PTR)MDIClientSubclass);
#else
            MDIClientProc = (WNDPROC)SetWindowLong(hwndMDIClient,GWL_WNDPROC,(DWORD)MakeProcInstance((FARPROC)MDIClientSubclass,hInst));
#endif

            ShowWindow(hwndMDIClient, SW_SHOW);
        }
        break;

    case WM_COMMAND:
        // Direct all menu selection or accelerator commands to 
        // the CommandHandler function
        CommandHandler(hwnd, wParam, lParam);
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case SCCVW_VIEWTHISFILE:
        {
            HWND                    locViewWnd;
            PSCCVWVIEWTHISFILE40    locVTFPtr40;
            PSCCVWVIEWTHISFILE80    locVTFPtr80;
            char                    szTitle[256];

            locVTFPtr40 = (PSCCVWVIEWTHISFILE40)lParam;
            locVTFPtr80 = (PSCCVWVIEWTHISFILE80)lParam;
            if (locVTFPtr80->dwSize == sizeof(SCCVWVIEWTHISFILE80))
            {
                LPSTR  p = szTitle;
                LPWORD q = locVTFPtr80->sViewFile.szDisplayName;

                while (*q)
                    *p++ = (char)*q++;
                *p = 0;
            }
            else
                lstrcpy(szTitle,locVTFPtr40->sViewFile.szDisplayName);


            locViewWnd = CreateMDIView( szTitle, &(locVTFPtr40->sViewFile) );
            if (IsWindow(locViewWnd))
                return(SCCVWERR_MESSAGEHANDLED);
            else
                return(0);
        }
        break;

    case WM_QUERYNEWPALETTE:
        {
            HWND  locWnd;
            WORD  locResult=0;

            locWnd = GetActiveChild( hwndMDIClient );
            if( locWnd )
            {
                // Tell the active document to realize in foreground.
                locResult = (WORD)SendMessage(locWnd, WM_QUERYNEWPALETTE, 0, 0);

                // If mapping is unchanged, other documents could still change,
                // so give them a change to realize.

                if (!locResult)
                    SendMessageToKids(WM_PALETTECHANGED, (WPARAM)locWnd, 0);
            }

            return(locResult);
        }
        break;


        // System palette has changed, so pass it on to the children.
    case WM_PALETTECHANGED:
    case WM_SYSCOLORCHANGE:
    case WM_SETFOCUS:
        SendMessageToKids(WM_PALETTECHANGED, wParam, lParam);
        break;

    default:
        // use DefFrameProc() instead of DefWindowProc(), since there
        // are things that have to be handled differently because of MDI
        return (LONG)DefFrameProc(hwnd, hwndMDIClient, msg, wParam, lParam);
    }

    return 0;
}


/*-----------------  CommandHandler  -------------------------------*/
/*
*
*  FUNCTION   : CommandHandler ()
*
*  PURPOSE    : Processes all "frame" WM_COMMAND messages.
*
*/


VOID CommandHandler (HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    WORD  locId;
    WORD  locEvent;
    HWND  locCtrl;

    locId    = LOWORD(wParam);
    locEvent = HIWORD(wParam);
    locCtrl  = (HWND) lParam;

    switch (locId)
    {
    case IDM_FILEOPEN:
        // Make an empty MDI child window 
        DoOpenFile();
        break;

    case IDM_FILERETURN:
        CreateMDIView( szWelcomeFile, NULL );
        break;

    case IDM_FILECLOSE:
        DoCloseFile(hwnd);
        break;

    case IDM_FILEEXIT:
        // Close application 
        SendMessage(hwnd, WM_CLOSE, 0, 0L);
        break;

    case IDM_FILEPRINT:
        {
            HWND  locWnd;

            locWnd = GetActiveChild( hwndMDIClient );
            if( locWnd )
                SendMessage(locWnd,SCCVW_PRINT,0,0L);
        }
        break;

    case IDM_EDITCOPY:
        {
            HWND  locWnd;

            locWnd = GetActiveChild( hwndMDIClient );
            if( locWnd )
                SendMessage(locWnd,SCCVW_COPYTOCLIP,0,0L);
        }
        break;

    case IDM_WINDOWTILE:
        // Tile MDI windows 
        SendMessage(hwndMDIClient, WM_MDITILE, 0, 0L);
        break;

    case IDM_WINDOWCASCADE:
        // Cascade MDI windows 
        SendMessage(hwndMDIClient, WM_MDICASCADE, 0, 0L);
        break;

    case IDM_WINDOWICONS:
        {
            register HWND hwndT;

            hwndT = GetWindow (hwndMDIClient, GW_CHILD);
            InvalidateRect(hwndT,NULL,TRUE);

        }
        // Auto - arrange MDI icons 
        SendMessage(hwndMDIClient, WM_MDIICONARRANGE, 0, 0L);
        break;

    case IDM_WINDOWCLOSEALL:
        CloseAllChildren();
        break;

    case IDM_WINDOWAUTOSIZE:
        {
            DWORD dwState;

            if( gFlags & MDIV_AUTOSIZE )
                gFlags &= ~MDIV_AUTOSIZE;
            else
                gFlags |= MDIV_AUTOSIZE;
            dwState = (gFlags & MDIV_AUTOSIZE)? (MF_CHECKED|MF_BYCOMMAND) : (MF_UNCHECKED|MF_BYCOMMAND);
            CheckMenuItem(hMenu, IDM_WINDOWAUTOSIZE, (WORD)dwState);
            DrawMenuBar(hwnd);
        }
        break;

    case MENU_HELP_ABOUT:

        DialogBox(hInst, "HELP_ABOUTBOX", hwndFrame, (DLGPROC)HelpAbout);
        break;

    default:
        // This is essential, since there are frame WM_COMMANDS generated
        // by the MDI system for activating child windows via the
        // window menu.
        DefFrameProc(hwnd, hwndMDIClient, WM_COMMAND, wParam, lParam);
    }
}

/*-------------------  CloseAllChildren  -----------------------------*/
/*
*
*  FUNCTION   : CloseAllChildren ()
*
*  PURPOSE    : Destroys all MDI child windows.
*
*/


VOID CloseAllChildren (VOID)
{
    register HWND hwndT;

    // As long as the MDI client has a child, destroy it 

    while ((hwndT = GetWindow(hwndMDIClient, GW_CHILD)) != 0)
    {
        // Skip the icon title windows 
        while (hwndT && GetWindow(hwndT, GW_OWNER))
            hwndT = GetWindow(hwndT, GW_HWNDNEXT);
        if (hwndT)
            SendMessage(hwndMDIClient, WM_MDIDESTROY, (WPARAM)hwndT, 0L);
        else
            break;
    }
}


#include "scclink.c"

int WINMAIN_ENTRYMOD WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    MSG      msg;
    char     locFile[256];
    OFSTRUCT locOf;
    BOOL     bWelcomeFileFound;

    hInst = hInstance;
    gFlags = MDIV_AUTOSIZE;

    // If this is the first instance of the app. register window classes 
    if (!hPrevInstance)
        if (!InitializeApplication())
            return 0;

    // Create the frame and do other initialization
    if (!InitializeInstance(lpszCmdLine, nCmdShow))
        return 0;

    /*
    |  Load the Outside In Viewer Technology DLL (SCxxVW.DLL)
    */

    SCCGetExePath(hInst,(VTLPSTR)szViewerPath,256);
    hViewerLibrary = SCCLoadViewerDLL(szViewerPath);

    /*
    |  Generate the child window class name
    */

    /*
    |  In Win32, the view window's CLASS name does not have 
    |  to be different for each OEM since global class names
    |  are process specific.
    */

    lstrcpy((LPSTR)szChild,"SCCVIEWER");

    /*
    |  Increase file handle count so more files can be open at one time
    */

    SetHandleCount(100);

    /*
    |  Initialize state of autosize menu option to unchecked
    */
    hMenu = GetMenu(hwndFrame);
    CheckMenuItem(hMenu, IDM_WINDOWAUTOSIZE, ((gFlags & MDIV_AUTOSIZE)? (MF_CHECKED|MF_BYCOMMAND) : (MF_UNCHECKED|MF_BYCOMMAND)));

    /*
    |  View the startup document WELCOME.DOC
    */
    bWelcomeFileFound = TRUE;

    lstrcpy(locFile,szViewerPath);
    lstrcat(locFile,"..\\files\\welcome.doc");

    if (OpenFile(locFile, &locOf,OF_EXIST) != -1)
    {
        lstrcpy(szWelcomeFile, locFile);
    }
    else
    {
        lstrcpy(locFile,szViewerPath);
        lstrcat(locFile,".\\files\\welcome.doc");

        if (OpenFile(locFile, &locOf,OF_EXIST) != -1)
        {
            lstrcpy(szWelcomeFile, locFile);
        }
        else
        {
            EnableMenuItem(hMenu, IDM_FILERETURN, MF_DISABLED|MF_GRAYED);
            bWelcomeFileFound = FALSE;
        }
    }

    if (bWelcomeFileFound)
    {
        CreateMDIView( szWelcomeFile, NULL );
    }
    /*
    |  Message loop
    */

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateMDISysAccel(hwndMDIClient, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    FreeLibrary(hViewerLibrary);

    return(0);
}

VOID SendMessageToKids (WORD msg, WPARAM wParam, LPARAM lParam)
{
    register HWND hwndT;

    hwndT = GetWindow (hwndMDIClient, GW_CHILD);

    while (hwndT)
    {
        SendMessage (hwndT, msg, wParam, lParam);
        hwndT = GetWindow(hwndT, GW_HWNDNEXT);
    }
}

WIN_ENTRYSC LRESULT WIN_ENTRYMOD MDIClientSubclass(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg >= SCCVW_FIRSTFROMMESSAGE  && msg <= SCCVW_LASTFROMMESSAGE)
    {
        return(SendMessage(GetParent(hwnd),msg,wParam,lParam));
    }
    else
    {
        return(CallWindowProc((WNDPROC)MDIClientProc,hwnd,msg,wParam,lParam));
    }
}



BOOL InitializeApplication ()
{
    WNDCLASS wc;

    // Register the frame class 

    wc.style = 0;
    wc.lpfnWndProc = (WNDPROC)BlandFrameWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(hInst, IDBLANDFRAME);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(LONG_PTR)(COLOR_APPWORKSPACE + 1);
    wc.lpszMenuName = IDBLANDMENU;
    wc.lpszClassName = szFrame;

    if (RegisterClass(&wc))
        return TRUE;
    else
        return FALSE;
}



BOOL InitializeInstance (LPSTR lpCmdLine, int nCmdShow)
{
    extern HWND hwndMDIClient;
    char sz[80];

    UNUSED(lpCmdLine);

    // Get the base window title

    LoadString(hInst, IDS_APPNAME, sz, sizeof(sz));

    // Create the frame 
    // MDI Client window is created in frame's WM_CREATE case
    hwndFrame = CreateWindow(szFrame, sz, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL,
        hInst, NULL);
    if (hwndFrame && hwndMDIClient)
    {

        // Display the frame window 
        ShowWindow(hwndFrame, nCmdShow);
        UpdateWindow(hwndFrame);

        return TRUE;
    }
    return FALSE;
}

HWND DoOpenFile ()
{
#define MAXFILENAME 256
    HWND          locViewWnd = NULL;
    OPENFILENAME    locOFN;
    char          locFilterSpec[128] = "All files\0*.*\0";
    char          locFileName[MAXFILENAME];
    char          locFileTitle[MAXFILENAME];

    locFileName[0] = 0x00;
    locFileTitle[0] = 0x00;

    locOFN.lStructSize = sizeof(OPENFILENAME);
    locOFN.hwndOwner = hwndFrame;
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
        locViewWnd = CreateMDIView( locFileName, NULL );

    return locViewWnd;
}


HWND CreateMDIView(
                   LPSTR szFilename,
                   PSCCVWVIEWFILE40 lpViewFile )
{
    MDICREATESTRUCT  locMcs;
    HWND locViewWnd = NULL;

    locMcs.szTitle = (LPSTR)szFilename; /* Fully qualified pathname*/
    locMcs.szClass = szChild;
    locMcs.hOwner = hInst;

    if( gFlags & MDIV_AUTOSIZE )
    {
        RECT locClientRect;

        GetClientRect( hwndFrame, (LPRECT)&locClientRect );

        locMcs.x = locClientRect.left;
        locMcs.y = locClientRect.top;
        locMcs.cx = (locClientRect.right - locClientRect.left);
        locMcs.cy = (locClientRect.bottom - locClientRect.top);
    }
    else
    {
        locMcs.x = locMcs.cx = CW_USEDEFAULT;
        locMcs.y = locMcs.cy = CW_USEDEFAULT;
    }
    locMcs.style = styleDefault;

    locViewWnd = (HWND)SendMessage(hwndMDIClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&locMcs);
    ShowWindow(locViewWnd, SW_SHOW);

    if (IsWindow(locViewWnd))
    {
        SCCVWVIEWFILE40  locViewFile;

        if( lpViewFile == NULL )
        {
            locViewFile.dwSize = sizeof(SCCVWVIEWFILE40);
            locViewFile.dwSpecType = IOTYPE_ANSIPATH;
            locViewFile.pSpec = (VTVOID *)szFilename;
            locViewFile.dwViewAs = 0;
            locViewFile.bUseDisplayName = FALSE;
            locViewFile.bDeleteOnClose = FALSE;
            locViewFile.dwFlags = 0;
            locViewFile.dwReserved1 = 0;
            locViewFile.dwReserved2 = 0;

            lpViewFile = &locViewFile;
        }

        SendMessage(locViewWnd,SCCVW_VIEWFILE,0,(LPARAM)lpViewFile);
        if( gFlags & MDIV_AUTOSIZE )
        {
            SCCVWIDEALSIZE50 locIdealSize;
            RECT locMaxRect;

            GetClientRect( locViewWnd, (LPRECT)&locMaxRect );

            locIdealSize.dwSize = sizeof(SCCVWIDEALSIZE50);
            locIdealSize.dwFlags = 0;
            locIdealSize.dwMaxWidth  = locMaxRect.right - locMaxRect.left;
            locIdealSize.dwMaxHeight = locMaxRect.bottom - locMaxRect.top;

            SendMessage(locViewWnd,SCCVW_GETIDEALWINDOWSIZE,0,(LPARAM)(PSCCVWIDEALSIZE50)&locIdealSize);

            locMaxRect.left = locMaxRect.top = 0;
            locMaxRect.right = (WORD)locIdealSize.dwResultWidth;
            locMaxRect.bottom = (WORD)locIdealSize.dwResultHeight;
            AdjustWindowRect( &locMaxRect, (WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_SYSMENU|WS_CAPTION|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|styleDefault), FALSE );

            MoveWindow(locViewWnd, 0,0, (locMaxRect.right - locMaxRect.left), (locMaxRect.bottom - locMaxRect.top), TRUE );
        }

        SetFocus(locViewWnd);
    }

    return(locViewWnd);
}

VOID DoCloseFile(HWND hWnd)
{
    HWND locActiveWnd;
    UNUSED(hWnd);

    locActiveWnd = GetActiveChild( hwndMDIClient );
    if( locActiveWnd )
    {
        SendMessage(locActiveWnd,SCCVW_CLOSEFILE,0,0L);
        DestroyWindow(locActiveWnd);
    }
}


HWND GetActiveChild(
                    HWND hMDIClient )
{
    HWND  locWnd;

    // Get the currently active document.
    locWnd = (HWND)SendMessage(hMDIClient,WM_MDIGETACTIVE, 0, 0);

    if(!IsWindow(locWnd))
        locWnd = NULL;

    return locWnd;
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
            lstrcpy(buffer, "Welcome Sample Application\n");
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
