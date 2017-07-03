/*
|  Outside In Viewer Technology
|  Sample Application
|
|  MDIVIEW
|  Viewer Technology sample application
|
|  Purpose:
|  To show the simplest possible MDI implementation
|  of the viewer technology.
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
#include <io.h>
#include <dos.h>
#include <errno.h>

#include "sccda.h"
#include "sccvw.h"

#include "mdiview.h"
#include "mdiview.pro"
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

/* SendMessageToActiveChild macro - sends a message to the MDI child window that has focus */
#define SendMessageToActiveChild(hwndMDIClient,Msg,wParam,lParam) \
    SendMessage( (HWND)SendMessage((hwndMDIClient),WM_MDIGETACTIVE,0,(LPARAM)NULL), (Msg), (wParam), (lParam) )


/*
|
|  Globals
|  Globals
|  Globals
|
*/
#define  MAX_CHARBUF_LENGTH  256

HANDLE  hInst;                 // Program instance handle
HWND    hwndFrame = NULL;      // Handle to main window
HWND    hwndMDIClient = NULL;  // Handle to MDI client
LONG    styleDefault = 0;      // Default style bits for child windows
WNDPROC MDIClientProc;
HMENU	  hMenu;
DWORD   gFlags;

HANDLE  hViewerLibrary;        // Handle to SCxxVW.DLL

char szFrame[] = "MDIVIEWFRAME";  // Class name for "frame" window
char szChild[40];                 // Class name for MDI window
BOOL bArchiveSingleClickView = FALSE;  // TRUE if single-click views archived files.
BOOL bFileAccessOn = FALSE;
TCHAR szCharBuffer[MAX_CHARBUF_LENGTH];

BOOL CALLBACK SetSelectDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
WIN_ENTRYSC BOOL WIN_ENTRYMOD FileAccessDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD NotesAccessDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

/* Flags for gFlags */
#define MDIV_AUTOSIZE   0x0001

WIN_ENTRYSC LONG WIN_ENTRYMOD BlandFrameWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
                WS_CLIPCHILDREN, 0, 0, 0, 0, hwnd, 0, hInst,
                (LPSTR)&ccs);

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
        hwndMDIClient = NULL;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case SCCVW_SELCHANGE:
        if( bArchiveSingleClickView )
        {
            SCCVWDISPLAYINFO40   info;

            info.dwSize = sizeof( SCCVWDISPLAYINFO40 );
            SendMessageToActiveChild( hwndMDIClient, SCCVW_GETDISPLAYINFO, 0, (LPARAM)&info );
            if( info.dwType == SCCVWTYPE_ARCHIVE )
            {
                // Verify that the node with the focus is selected and not a folder.
                SCCVWFINDPOSITION41   sFind;

                sFind.dwSize        = sizeof( SCCVWFINDPOSITION41 );
                sFind.dwFindOptions = SCCVW_FINDANCHORSELECTPOS;
                // switched to if instead of while. This is single click, only one thing *can* be selected.
                // And after first pass, active child is a file view not an archive, so we went into an infinite loop
                // trying to find the next "node" of the file view...
                if ( SendMessageToActiveChild( hwndMDIClient, SCCVW_FINDPOSITION, 0, (LPARAM)&sFind ) == SCCVWERR_OK )
                {	
                    // Get information for this node.
                    SCCVWMAPTREEPOSITION60   sMap;
                    ZeroMemory( &sMap, sizeof( SCCVWMAPTREEPOSITION60 ) );
                    sMap.dwSize       = sizeof( SCCVWMAPTREEPOSITION60 );
                    sMap.dwMapOptions = SCCVW_MAPPOSTONODE;
                    sMap.sPos         = sFind.sResultPos;
                    if( SendMessageToActiveChild( hwndMDIClient, SCCVW_MAPTREEPOSITION, 0, (LPARAM)&sMap ) == SCCVWERR_OK )
                    {
                        SCCDATREENODE   sNode;
                        ZeroMemory( &sNode, sizeof( SCCDATREENODE ) );
                        sNode.dwSize = sizeof( SCCDATREENODE );
                        sNode.dwNode = sMap.dwNode;
                        SendMessageToActiveChild( hwndMDIClient, SCCVW_GETTREENODE, 0, (LPARAM)&sNode );
                        if( ( sNode.dwFlags & SCCDA_TREENODEFLAG_FOCUS ) && ( sNode.dwFlags & SCCDA_TREENODEFLAG_FOLDER ) == 0 )
                        {
                            // The node has focus and is not a folder.
                            SCCVWOPTIONSPEC40   opt;
                            DWORD               arcview = SCCVW_ARCHIVE_VIEWTHIS;

                            opt.dwSize  = sizeof( SCCVWOPTIONSPEC40 );
                            opt.dwId    = SCCID_ARCSAVEEVENT;
                            opt.dwFlags = SCCVWOPTION_CURRENT;
                            opt.pData   = &arcview;
                            SendMessageToActiveChild( hwndMDIClient, SCCVW_SETOPTION, 0, (LPARAM)&opt );
                        }
                    }
                }
            }
        }
        break;

    case SCCVW_VIEWTHISFILE:

        {
            PSCCVWVIEWTHISFILE40    locVTFPtr40;
            PSCCVWVIEWTHISFILE80    locVTFPtr80;
            MDICREATESTRUCT         locMcs;
            HWND                    locViewWnd;
            BOOL                    bVersion80;
            char                    szTitle[MAX_PATH];

            locVTFPtr40 = (PSCCVWVIEWTHISFILE40)lParam;
            locVTFPtr80 = (PSCCVWVIEWTHISFILE80)lParam;
            bVersion80 = (locVTFPtr40->dwSize == sizeof(SCCVWVIEWTHISFILE80));
            if (bVersion80)
            {
                LPTSTR p = szTitle;
                LPWORD q = locVTFPtr80->sViewFile.szDisplayName;

                while (*q)
                    *p++ = (char)*q++;
                *p = 0;

                locMcs.szTitle = (LPTSTR)szTitle;
            }
            else
                locMcs.szTitle = (LPTSTR)locVTFPtr40->sViewFile.szDisplayName;

            locMcs.szClass = szChild;
            locMcs.hOwner = hInst;
            locMcs.x = locMcs.cx = CW_USEDEFAULT;  // Use the default size for the window
            locMcs.y = locMcs.cy = CW_USEDEFAULT;
            locMcs.style = styleDefault;   // Set the style DWORD of the window

            locViewWnd = (HWND)SendMessage(hwndMDIClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&locMcs);

            if (IsWindow(locViewWnd))
            {
                ShowWindow(locViewWnd, SW_SHOW);
                /* Test CJK Mode for System Flags  rmc 1-8-97 */
                /*
                {
                SCCVWOPTIONSPEC40 sOptVal;
                LONG err;
                DWORD dwSysFlag;
                dwSysFlag = SCCVW_SYSTEM_CJKADDON;
                sOptVal.dwSize = sizeof(SCCVWOPTIONSPEC40);
                sOptVal.dwId = SCCID_SYSTEMFLAGS;
                sOptVal.dwFlags = SCCVWOPTION_CURRENT;
                sOptVal.pData = &dwSysFlag;
                err = SendMessage(locViewWnd, SCCVW_SETOPTION, 0, (LPARAM)(PSCCVWOPTIONSPEC40)&sOptVal);
                }
                */
                if (bVersion80)
                    SendMessage(locViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE80)&(locVTFPtr80->sViewFile));
                else
                    SendMessage(locViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&(locVTFPtr40->sViewFile));

                return(SCCVWERR_MESSAGEHANDLED);
            }
            else
            {
                return(0);
            }
        }

        break;

    case SCCVW_GETFILEACCESSDATA:
        if (bFileAccessOn && lParam)
        {
            HWND  locWnd;
            VTDWORD  i;  /* generic counter */

            PSCCVWFILEACCESSDATA  pFileAccessData = (PSCCVWFILEACCESSDATA)lParam;  /* pointer to SCCVWFILEACCESSDATA structure */

            VTWORD   wDialogRet = 0;         /* catch dialog return value */
            VTDWORD  dwRet = SCCERR_CANCEL;  /* return value              */

            /* Get the currently active document. */
            locWnd = (HWND)SendMessage(hwndMDIClient, WM_MDIGETACTIVE, 0, 0);

            if (IsWindow(locWnd))
            {
                memset(szCharBuffer, 0, MAX_CHARBUF_LENGTH);

                switch (pFileAccessData->dwRequestId)
                {
                case OIT_FILEACCESS_PASSWORD:
                    /* Request a password from the user */
                    wDialogRet = (VTWORD)DialogBox(hInst, MAKEINTRESOURCE(IDD_FILEACCESSDIALOG), locWnd, (DLGPROC)FileAccessDialogProc);
                    break;
                case OIT_FILEACCESS_NOTESID:
                    /* Request a path to a Lotus Notes ID file from the user */
                    wDialogRet = (VTWORD)DialogBox(hInst, MAKEINTRESOURCE(IDD_NOTESACCESSDIALOG), locWnd, (DLGPROC)NotesAccessDialogProc);
                    break;
                default:
                    break;
                }

                if (wDialogRet)
                {
                    VTLPWORD  lpwszBuf = (VTLPWORD)pFileAccessData->pReturnData;
                    DWORD     dwReturnDataSize = pFileAccessData->dwReturnDataSize;

                    /* Copy the password into the request data buffer
                    *
                    * NOTE: The return data buffer for the OIT_FILEACCESS_PASSWORD
                    *       id is a wide-character buffer, so make sure to copy the
                    *       characters correctly.
                    */
                    for (i = 0; (i < wDialogRet) && (i < dwReturnDataSize / 2); i++)
                        lpwszBuf[i] = (VTWORD)szCharBuffer[i];

                    /* Clear the character buffer */
                    memset(szCharBuffer, 0, MAX_CHARBUF_LENGTH);

                    dwRet = SCCERR_OK;
                }
            }

            /* Flag that we have processed this request */
            pFileAccessData->bProcessed = TRUE;

            return (dwRet);
        }
        else
            return (SCCERR_NONEFOUND);
        break;

    case WM_QUERYNEWPALETTE:

        {
            HWND  locWnd;
            WORD  locResult = 0;

            // Get the currently active document.
            locWnd = (HWND)SendMessage(hwndMDIClient,WM_MDIGETACTIVE, 0, 0);

            if (IsWindow(locWnd))
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

        // System palette or system color has changed, so pass it on to the children.
    case WM_PALETTECHANGED:
    case WM_SYSCOLORCHANGE:
        SendMessageToKids(msg, wParam, lParam);
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
        MakeNewChild();
        break;

    case IDM_FILECLOSE:
        // Close active MDI child window 
        if (IsWindow(hwndMDIClient))
        {
            SendMessageToActiveChild( hwndMDIClient, WM_CLOSE, 0, 0L );
        }
        break;

    case IDM_FILEEXIT:
        // Close application 
        SendMessage(hwnd, WM_CLOSE, 0, 0L);
        break;

    case IDM_EDITCOPY:
        SendMessageToActiveChild( hwndMDIClient, SCCVW_COPYTOCLIP, 0, 0L );
        break;

    case IDM_EDITSELECTALL:
        SendMessageToActiveChild( hwndMDIClient, SCCVW_SELECTALL, 0, 0L );
        break;

    case IDM_EDITSEARCH:
        SendMessageToActiveChild( hwndMDIClient, SCCVW_SEARCHDIALOG, 0, 0L );
        break;

    case IDM_EDITSEARCHNEXT:
        SendMessageToActiveChild( hwndMDIClient, SCCVW_SEARCHNEXT, SCCVW_SEARCHFORWARD, 0L );
        break;

    case IDM_EDITSEARCHPREV:
        SendMessageToActiveChild( hwndMDIClient, SCCVW_SEARCHNEXT, SCCVW_SEARCHBACK, 0L );
        break;

    case IDM_OPTIONSAUTOSIZE:
        {
            DWORD dwState;

            if( gFlags & MDIV_AUTOSIZE )
                gFlags &= ~MDIV_AUTOSIZE;
            else
                gFlags |= MDIV_AUTOSIZE;
            dwState = (gFlags & MDIV_AUTOSIZE)? (MF_CHECKED|MF_BYCOMMAND) : (MF_UNCHECKED|MF_BYCOMMAND);
            CheckMenuItem(hMenu, IDM_OPTIONSAUTOSIZE, (WORD)dwState);
            DrawMenuBar(hwnd);
        }
        break;

    case IDM_OPTIONSENCLATIN1       :
    case IDM_OPTIONSENCLATIN2       :
    case IDM_OPTIONSENCCENTRALEU1250:
    case IDM_OPTIONSENCJAPANESESJIS :
    case IDM_OPTIONSENCJAPANESEEUC  :
    case IDM_OPTIONSENCJAPANESEJIS :
    case IDM_OPTIONSENCCHINESEBIG5  :
    case IDM_OPTIONSENCCHINESEEUC   :
    case IDM_OPTIONSENCCHINESEGB    :
    case IDM_OPTIONSENCKOREAN       :
    case IDM_OPTIONSENCCYRILLIC1251 :
    case IDM_OPTIONSENCCYRILLICKOI8 :
        MDIViewAs( locId );
        break;

    case IDM_OPTIONSARCHIVESINGLE:
        {
            if( bArchiveSingleClickView )
            {
                bArchiveSingleClickView = FALSE;
                CheckMenuItem( GetMenu( hwnd ), IDM_OPTIONSARCHIVESINGLE, MF_BYCOMMAND | MF_UNCHECKED );
            }
            else
            {
                bArchiveSingleClickView = TRUE;
                CheckMenuItem( GetMenu( hwnd ), IDM_OPTIONSARCHIVESINGLE, MF_BYCOMMAND | MF_CHECKED );
            }
            break;
        }
    case IDM_OPTIONSSETSELECT:
        {
            SCCVWDISPLAYINFO40   info;

            info.dwSize = sizeof( SCCVWDISPLAYINFO40 );
            SendMessageToActiveChild( hwndMDIClient, SCCVW_GETDISPLAYINFO, 0, (LPARAM)&info );
            if( info.dwType == SCCVWTYPE_ARCHIVE )
            {
                DWORD   dwSelect[2] = { 0, 0 };

                if( DialogBoxParam( hInst, MAKEINTRESOURCE( IDD_SETSELECT ), hwnd, (DLGPROC)SetSelectDlgProc, (LPARAM)dwSelect ) )
                {
                    SCCVWMAPTREEPOSITION60   sMap;
                    SCCVWSETSELECTION41     sSelect;

                    sSelect.dwSize = sizeof( SCCVWSETSELECTION41 );

                    // Map the node indexes to SCCVWPOS structures.
                    sMap.dwSize       = sizeof( SCCVWMAPTREEPOSITION60 );
                    sMap.dwMapOptions = SCCVW_MAPNODETOPOS;
                    sMap.dwNode       = dwSelect[0];
                    SendMessageToActiveChild( hwndMDIClient, SCCVW_MAPTREEPOSITION, 0, (LPARAM)&sMap );
                    sSelect.sAnchorPos = sMap.sPos;

                    sMap.dwNode = dwSelect[1];
                    SendMessageToActiveChild( hwndMDIClient, SCCVW_MAPTREEPOSITION, 0, (LPARAM)&sMap );
                    sSelect.sEndPos = sMap.sPos;

                    SendMessageToActiveChild( hwndMDIClient, SCCVW_SETSELECTION, 0, (LPARAM)&sSelect );
                }
            }
            else
            {
                char szErr[255];

                LoadString(hInst, IDS_NOTARCHIVE, szErr, 255);
                MessageBox(hwnd, szErr, NULL, MB_OK | MB_ICONSTOP);
            }
            break;
        }

    case IDM_OPTIONSFILEACCESSDIALOG:
        if (bFileAccessOn)
        {
            CheckMenuItem(GetMenu(hwnd), IDM_OPTIONSFILEACCESSDIALOG, MF_UNCHECKED);
            bFileAccessOn = FALSE;
        }
        else
        {
            CheckMenuItem(GetMenu(hwnd), IDM_OPTIONSFILEACCESSDIALOG, MF_CHECKED);
            bFileAccessOn = TRUE;
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
        // Auto - arrange MDI icons 
        SendMessage(hwndMDIClient, WM_MDIICONARRANGE, 0, 0L);
        break;

    case IDM_WINDOWCLOSEALL:
        CloseAllChildren();
        break;

    case IDM_HELPABOUT:
        {
            DialogBox(hInst, "HELP_ABOUTBOX", hwndFrame, (DLGPROC)HelpAbout);
        }
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
    char locViewerPath[256];
    MSG  msg;

    hInst = hInstance;
    gFlags = 0;

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

    SCCGetExePath(hInst,(VTLPSTR)locViewerPath,256);
    hViewerLibrary = SCCLoadViewerDLL(locViewerPath);

    /*
    |  Generate the child window class name
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
    CheckMenuItem(hMenu, IDM_OPTIONSAUTOSIZE, ((gFlags & MDIV_AUTOSIZE)? (MF_CHECKED|MF_BYCOMMAND) : (MF_UNCHECKED|MF_BYCOMMAND)));

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

VOID SendMessageToKids (UINT msg, WPARAM wParam, LPARAM lParam)
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

HWND MakeNewChild ()
{
#define MAXFILENAME 256

    HWND          locViewWnd = NULL;
    MDICREATESTRUCT  locMcs;
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
    {
        locMcs.szTitle = (LPSTR)locFileName; /* Fully qualified pathname*/
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
            locMcs.x = locMcs.cx = CW_USEDEFAULT;  // Use the default size for the window
            locMcs.y = locMcs.cy = CW_USEDEFAULT;
        }
        locMcs.style = styleDefault;   // Set the style DWORD of the window

        locViewWnd = (HWND)SendMessage(hwndMDIClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&locMcs);

        if (IsWindow(locViewWnd))
        {
            SCCVWVIEWFILE40  locViewFile;
            SCCVWOPTIONSPEC40 locOptionSpec;
            DWORD             dwDialogFlags;

            locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
            locOptionSpec.dwId   = SCCID_DIALOGFLAGS;
            locOptionSpec.dwFlags = SCCVWOPTION_CURRENT|SCCVWOPTION_DEFAULT;
            locOptionSpec.pData   = &dwDialogFlags;
            dwDialogFlags = 0;
            SendMessage(locViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

            /* Test CJK Mode for System Flags  rmc 1-8-97 */
            /*
            {
            SCCVWOPTIONSPEC40 sOptVal;
            LONG err;
            DWORD dwSysFlag;
            dwSysFlag = SCCVW_SYSTEM_CJKADDON;
            sOptVal.dwSize = sizeof(SCCVWOPTIONSPEC40);
            sOptVal.dwId = SCCID_SYSTEMFLAGS;
            sOptVal.dwFlags = SCCVWOPTION_CURRENT;
            sOptVal.pData = &dwSysFlag;
            err = SendMessage(locViewWnd, SCCVW_SETOPTION, 0, (LPARAM)(PSCCVWOPTIONSPEC40)&sOptVal);
            }
            */
            locViewFile.dwSize = sizeof(SCCVWVIEWFILE40);
            locViewFile.dwSpecType = IOTYPE_ANSIPATH;
            locViewFile.pSpec = (VTVOID *)locFileName;
            locViewFile.dwViewAs = 0;
            locViewFile.bUseDisplayName = FALSE;
            locViewFile.bDeleteOnClose = FALSE;
            locViewFile.dwFlags = 0;
            locViewFile.dwReserved1 = 0;
            locViewFile.dwReserved2 = 0;
            SendMessage(locViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&locViewFile);

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

            ShowWindow(locViewWnd, SW_SHOW);
            SetFocus(locViewWnd);
        }
    }

    return locViewWnd;
}

DWORD MDIViewAs(
                WORD wMenuId )
{
    HWND hwndActiveChild;
    SCCVWFILEINFO40 locFileInfo;
    WORD locFileId = 0;

    hwndActiveChild = (HWND)SendMessage(hwndMDIClient,WM_MDIGETACTIVE,0,(LPARAM)NULL);

    locFileInfo.dwSize = sizeof(SCCVWFILEINFO40);
    SendMessage(hwndActiveChild,SCCVW_GETFILEINFO,0,(LPARAM)(PSCCVWFILEINFO40)&locFileInfo);

    locFileId = (WORD)locFileInfo.dwFileId;
    switch(locFileId)
    {
    case FI_HTML :
    case FI_HTML_LATIN2 :
    case FI_HTML_JAPANESESJIS :
    case FI_HTML_JAPANESEEUC :
    case FI_HTML_JAPANESEJIS :
    case FI_HTML_CHINESEBIG5 :
    case FI_HTML_CHINESEEUC :
    case FI_HTML_CHINESEGB :
    case FI_HTML_KOREANHANGUL :
    case FI_HTML_CYRILLIC1251 :
    case FI_HTML_CYRILLICKOI8 :
        switch(wMenuId)
        {
        case IDM_OPTIONSENCLATIN1       : 
            locFileId = FI_HTML; 
            break;
        case IDM_OPTIONSENCCENTRALEU1250: 
            locFileId = FI_HTML_LATIN2;   
            break;
        case IDM_OPTIONSENCJAPANESESJIS : 
            locFileId = FI_HTML_JAPANESESJIS;  
            break;
        case IDM_OPTIONSENCJAPANESEEUC  : 
            locFileId = FI_HTML_JAPANESEEUC; 
            break;
        case IDM_OPTIONSENCJAPANESEJIS : 
            locFileId = FI_HTML_JAPANESEJIS;  
            break;
        case IDM_OPTIONSENCCHINESEBIG5  :
            locFileId = FI_HTML_CHINESEBIG5;
            break;
        case IDM_OPTIONSENCCHINESEEUC   :
            locFileId = FI_HTML_CHINESEEUC;
            break;
        case IDM_OPTIONSENCCHINESEGB    :
            locFileId = FI_HTML_CHINESEGB;
            break;
        case IDM_OPTIONSENCKOREAN       :
            locFileId = FI_HTML_KOREANHANGUL;
            break;
        case IDM_OPTIONSENCCYRILLIC1251 :
            locFileId = FI_HTML_CYRILLIC1251;
            break;
        case IDM_OPTIONSENCCYRILLICKOI8 :
            locFileId = FI_HTML_CYRILLICKOI8;
            break;
        }
        break;
    case FI_SHIFTJIS:
    case FI_JAPANESE_EUC:
    case FI_JAPANESE_JIS:
    case FI_CHINESEGB:
    case FI_HANGEUL:
    case FI_CHINESEBIG5:
    case FI_CYRILLIC1251:
    case FI_CYRILLICKOI8:
    case FI_CENTRALEU_1250:
    case FI_LATIN2:
    case FI_ASCII:
    case FI_ASCII8:
    case FI_MAC:
    case FI_MAC8:
    case FI_ANSI:
    case FI_ANSI8:
    case FI_UNICODE:
    case FI_UNKNOWN:
        switch(wMenuId)
        {
        case IDM_OPTIONSENCLATIN1       : 
            locFileId = FI_ASCII8; 
            break;
        case IDM_OPTIONSENCLATIN2       : 
            locFileId = FI_LATIN2; 
            break;
        case IDM_OPTIONSENCCENTRALEU1250: 
            locFileId = FI_CENTRALEU_1250; 
            break;
        case IDM_OPTIONSENCJAPANESESJIS : 
            locFileId = FI_SHIFTJIS;
            break;
        case IDM_OPTIONSENCJAPANESEEUC  : 
            locFileId = FI_JAPANESE_EUC;
            break;
        case IDM_OPTIONSENCJAPANESEJIS  : 
            locFileId = FI_JAPANESE_JIS;
            break;
        case IDM_OPTIONSENCCHINESEBIG5  :
            locFileId = FI_CHINESEBIG5;
            break;
        case IDM_OPTIONSENCCHINESEGB    :
            locFileId = FI_CHINESEGB;
            break;
        case IDM_OPTIONSENCKOREAN       :
            locFileId = FI_HANGEUL;
            break;
        case IDM_OPTIONSENCCYRILLIC1251 :
            locFileId = FI_CYRILLIC1251;
            break;
        case IDM_OPTIONSENCCYRILLICKOI8 :
            locFileId = FI_CYRILLICKOI8;
            break;
        }
        break;
    }


    if( locFileId != 0 )
        return (DWORD)SendMessage(hwndActiveChild,SCCVW_VIEWAS,locFileId, 0L);
    else
        return SCCVWERR_INVALIDID;
}


#define numberofdll 18
LPSTR	dlllist[numberofdll];

WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        {
            // Display sample app. version
            char	header[200];

            lstrcpy(header, "MDI Sample Application\n");
            lstrcat(lstrcat(header, S_PRODUCT), "\n");

            lstrcat(lstrcat(header, "Build:\t"), S_FILEVERSION);
            SetDlgItemText(hDlg, HELP_ABOUTTOPBOX, header);
            {
                DWORD	verinfosize, number = 0;
                int		i, j, rowpercolumnWIN32 = 9;
                char	buffer[600];	// This storage is only up to 24 dlls

                /* Should there be a need for adding more dlls to the list, do the following
                1. change number of dll above and add that dll to the list below.
                2. Change size of buffer.
                3. change the rowpercolumn (if neccessary)
                4. Make sure the first 2 steps completed correctly.  You should 
                have at least 24 dlls on the list b4 modifying the resource file
                so you can add more column to the list.  Then add another case
                statement at the end to make display correctly.

                */

                // Display the dlls version.
                dlllist[0]  = "DEBMP.DLL";
                dlllist[1]  = "DEHEX.DLL";
                dlllist[2]  = "DEMET.DLL";
                dlllist[3]  = "DESS.DLL";
                dlllist[4]  = "DETREE.DLL";
                dlllist[5]  = "DEWP.DLL";
                dlllist[6]  = "SCCCA.DLL";
                dlllist[7]  = "SCCCH.DLL";
                dlllist[8]  = "SCCDA.DLL";
                dlllist[9]  = "SCCDU.DLL";
                dlllist[10] = "SCCFA.DLL";
                dlllist[11] = "SCCFI.DLL";
                dlllist[12] = "SCCLO.DLL";
                dlllist[13] = "SCCOLE.DLL";
                dlllist[14] = "SCCRA.DLL";
                dlllist[15] = "SCCTA.DLL";
                dlllist[16] = "SCCUT.DLL";
                dlllist[17] = "SCCVW.DLL";

                for (j = 0; j < numberofdll/rowpercolumnWIN32 +
                    (numberofdll % rowpercolumnWIN32?1:0); j++)
                {	//This loop used for the control of the column when displayed.
                    lstrcpy(buffer, "Module\t\tVersion\n\n");
                    dlllist[9]=dlllist[10];
                    for (i = rowpercolumnWIN32*j;
                        i < (numberofdll - rowpercolumnWIN32*j > rowpercolumnWIN32?rowpercolumnWIN32*(j+1):numberofdll);
                        i++)
                    {
                        verinfosize = GetFileVersionInfoSize(dlllist[i], &number);
                        if (verinfosize)
                        {	// Only the dlls w/ version included will be displayed

                            HANDLE  hglobalmem;
                            VOID*	versioninfoptr;

                            UINT	datasize = 0;
                            char*	data = NULL;

                            hglobalmem = GlobalAlloc(GMEM_MOVEABLE, verinfosize);
                            versioninfoptr = GlobalLock(hglobalmem);
                            GetFileVersionInfo(dlllist[i], number, verinfosize, versioninfoptr);
                            VerQueryValue(versioninfoptr, "\\StringFileInfo\\040904e4\\FileVersion", &data, &datasize);
                            lstrcat(lstrcat(lstrcat(lstrcat(buffer, dlllist[i]), "\t"), data), "\n");
                            GlobalUnlock(hglobalmem);
                            GlobalFree(hglobalmem);
                        }

                    }

                    switch(j)
                    {
                    case 0:						//Column 1
                        SetDlgItemText(hDlg,HELP_ABOUTBOXCOL1,buffer);
                        break;
                    case 1:						//Column 2
                        SetDlgItemText(hDlg,HELP_ABOUTBOXCOL2,buffer);
                        break;
                    }
                }
            }
        }
        return (TRUE);
    case WM_COMMAND:
        {

            switch(wParam)
            {
            case ID_OK:

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


BOOL CALLBACK   SetSelectDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    static DWORD   *pdwSelect;
    char            szTemp[256];

    switch( uMsg )
    {
    case WM_INITDIALOG:

        pdwSelect = (DWORD *)lParam;
        return TRUE;

    case WM_COMMAND:

        switch( LOWORD( wParam ) )
        {
        case IDOK:

            GetDlgItemText( hDlg, IDC_EDTSELECT1, szTemp, sizeof( szTemp ) );
            pdwSelect[0] = atol( szTemp );
            GetDlgItemText( hDlg, IDC_EDTSELECT2, szTemp, sizeof( szTemp ) );
            pdwSelect[1] = atol( szTemp );
            if ((long)pdwSelect[0] <= 0 || (long)pdwSelect[1] <= 0)
            {
                char szErr[255];

                LoadString(hInst, IDS_ARCSELERR, szErr, 255);
                MessageBox(hDlg, szErr, NULL, MB_OK | MB_ICONSTOP);

                if ((long)pdwSelect[0] < 0)
                {
                    SetFocus(GetDlgItem(hDlg, IDC_EDTSELECT1));
                    SendMessage(GetDlgItem(hDlg, IDC_EDTSELECT1), EM_SETSEL, 0, -1);
                }
                else
                {
                    SetFocus(GetDlgItem(hDlg, IDC_EDTSELECT2));
                    SendMessage(GetDlgItem(hDlg, IDC_EDTSELECT2), EM_SETSEL, 0, -1);
                }

            }
            else
                EndDialog( hDlg, TRUE );
            return TRUE;

        case IDCANCEL:

            EndDialog( hDlg, FALSE );
            return TRUE;
        }
        break;
    }
    return FALSE;
}

WIN_ENTRYSC BOOL WIN_ENTRYMOD FileAccessDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    VTWORD  wCount;
    RECT    locRect;
    int     locX;
    int     locY;

    BOOL  bRet = FALSE;  /* return value */
    UNUSED(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        /*
        |   Center dialog
        */
        GetWindowRect(hDlg, &locRect);
        locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
        locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
        SetWindowPos(hDlg, NULL, locX, locY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        /* Clear the password string */
        SetDlgItemText(hDlg, IDS_PASSWORD, TEXT(""));

        /* Set a character limit on the input text */
        (VOID)SendMessage(hDlg, EM_LIMITTEXT, 256, 0);

        bRet = TRUE;
        break;
    case WM_COMMAND:
        switch(wParam)
        {
        case IDOK:
            wCount = (VTWORD)GetDlgItemText(hDlg, IDS_PASSWORD, szCharBuffer, MAX_CHARBUF_LENGTH);
            EndDialog(hDlg, wCount);
            bRet = TRUE;
            break;
        case IDCANCEL:
            EndDialog(hDlg, 0);
            bRet = TRUE;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return (bRet);
}

/*
* NotesAccessDialogProc
* Dialog proc for the Notes ID access dialog option.
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD NotesAccessDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    VTWORD  wCount;
    RECT    locRect;
    int     locX;
    int     locY;

    BOOL  bRet = FALSE;  /* return value */
    UNUSED(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        /*
        |   Center dialog
        */
        GetWindowRect(hDlg, &locRect);
        locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
        locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
        SetWindowPos(hDlg, NULL, locX, locY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        /* Clear the password string */
        SetDlgItemText(hDlg, IDS_NOTESID, TEXT(""));

        /* Set a character limit on the input text */
        (VOID)SendMessage(hDlg, EM_LIMITTEXT, 256, 0);

        bRet = TRUE;
        break;
    case WM_COMMAND:
        switch(wParam)
        {
        case IDOK:
            wCount = (VTWORD)GetDlgItemText(hDlg, IDS_NOTESID, szCharBuffer, MAX_CHARBUF_LENGTH);
            EndDialog(hDlg, wCount);
            bRet = TRUE;
            break;
        case IDCANCEL:
            EndDialog(hDlg, 0);
            bRet = TRUE;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return (bRet);
}

