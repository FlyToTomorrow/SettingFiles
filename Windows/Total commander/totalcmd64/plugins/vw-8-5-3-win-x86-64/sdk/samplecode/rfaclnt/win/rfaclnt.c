/*
|  Outside In Viewer Technology
|  Sample Application
|  
|
|  RFACLNT -- Remote Filter Access Client
|  Viewer Technology sample application
|
|  Purpose:
|  To show the client side implementation of the Remote Filter Access Feature of 
|  Outside In Viewer Technology
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
#include <string.h>

#define WINDOWS
#include "sccvw.h"
#include "sccra.h"

#include "rfaclnt.h"
#include "rfaclnt.pro"
#include "resource.h"
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
BOOL        bReadAhead;

char	ShrName[256];
char	ClntName[256];
char	RemoteFileName[256];
HANDLE	ghPipe;

/*
|
|  Routines
|  Routines
|  Routines
|
*/

#include "scclink.c"


int WINMAIN_ENTRYMOD WinMain(
                             HINSTANCE hInstance,  
                             HINSTANCE hPrevInstance, 
                             LPSTR lpCmdLine, 
                             int nCmdShow)
{
    MSG  locMsg;
    char fileName[256];
    DWORD retCode;
    MYMESSAGEHDR myMessage, inMessage;

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

    /* Init Pipe */
    DialogBox(hInst, (LPCSTR)"InitDialog", NULL, (DLGPROC)InitDlgProc);

    strcpy( fileName, "\\\\" );
    strcat( fileName, ShrName );
    strcat( fileName, "\\PIPE\\sccviewer" );

    if( 0 != RFAOpenPipe( &ghPipe, fileName ) )
        return 0;

    myMessage.dwType = MESSAGE_OPENPIPE;
    myMessage.dwFlags = 0;
    myMessage.dwParm1 = 0;
    myMessage.dwParm2 = 0;
    myMessage.dwMoreDataSize = (DWORD)strlen(ClntName)+1;
    myMessage.dwRet=0;
    retCode = RFASendMessage( ghPipe, &myMessage, sizeof(MYMESSAGEHDR), NULL );
    retCode = RFASendMessage( ghPipe, ClntName,   myMessage.dwMoreDataSize, NULL );
    inMessage.dwType = 0;
    retCode = RFAGetMessage( ghPipe, &inMessage, sizeof(MYMESSAGEHDR), NULL );
    if (inMessage.dwType != MESSAGE_OPENPIPERET)
    {
        MessageBox (NULL,
            "Viewer Server did not acknoledge my response, so quit!",
            "",
            MB_OK);
        RFAClosePipe( ghPipe );
        ghPipe = NULL;
        return 0;
    }



    /*
    | Create main window
    */

    hMainWnd = CreateWindow(
        (LPSTR) "SCC_MAIN",                   /* window class    */
        "RFA Client Sample",                  /* window name      */
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

    bReadAhead = TRUE;

    /*
    |  GetMessage loop
    */

    while (GetMessage(&locMsg,NULL,0,0))
    {
        TranslateMessage(&locMsg);
        DispatchMessage(&locMsg);
    }

    if (ghPipe)
    {
        RFAClosePipe( ghPipe );
        ghPipe = NULL;
    }

    return (int)(locMsg.wParam);     /* Returns the value from PostQuitMessage */
}


/*************************************************************************\
*
*  PROCEDURE: InitDlgProc (HWND hDlg, WORD wMsg, LONG wParam, LONG lParam)
*
*  PURPOSE:  This dialog box prompts the user for a net share name and
*            a client or user name.  These values are placed into global
*            strings ShrName and ClntName.
*
*  CALLED BY:
*
*    ClientDlgProc();
*
\*************************************************************************/

LONG CALLBACK InitDlgProc (
                           HWND hDlg, 
                           UINT wMsg, 
                           WPARAM wParam, 
                           LPARAM lParam)
{

    UNREFERENCED_PARAMETER(lParam);

    switch (wMsg)
    {
    case WM_INITDIALOG:
        {
            char szName[128];

            SendMessage (GetDlgItem (hDlg, IDD_SVREDIT),
                EM_LIMITTEXT, 256, 0);

            SendMessage (GetDlgItem (hDlg, IDD_CLNTEDIT),
                EM_LIMITTEXT, 256, 0);
            SetDlgItemText( hDlg, IDD_SVREDIT, "." );
            {
                DWORD dwMaxLen;
                dwMaxLen = 127; 
                GetComputerName( (LPSTR)szName, &dwMaxLen );
            }
            SetDlgItemText( hDlg, IDD_CLNTEDIT, szName );
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {                            // When the user clicks okay, get the
        case IDB_INITOK:             // share name and user name from the
            // edit fields.
            GetWindowText (GetDlgItem (hDlg, IDD_SVREDIT), ShrName, 256);
            GetWindowText (GetDlgItem (hDlg, IDD_CLNTEDIT), ClntName, 256);
            EndDialog(hDlg, 0);
            break;

        default:
            break;
        }
    default:
        break;
    }
    return (0);
}

LONG CALLBACK OpenRFADlgProc (
                              HWND hDlg, 
                              UINT wMsg, 
                              WPARAM wParam, 
                              LPARAM lParam)
{

    UNREFERENCED_PARAMETER(lParam);

    switch (wMsg)
    {
    case WM_INITDIALOG:
        SendMessage (GetDlgItem (hDlg, IDD_FILENAME),
            EM_LIMITTEXT, 256, 0);
        SetFocus( GetDlgItem( hDlg, IDD_FILENAME ) );
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {                            // When the user clicks okay, get the
        case IDB_OPENRFAOK:             // share name and user name from the
            GetWindowText (GetDlgItem (hDlg, IDD_FILENAME), RemoteFileName, 256);
            EndDialog(hDlg, 0);
            break;
        case IDB_OPENRFACANCEL:
            EndDialog(hDlg, 0);
            RemoteFileName[0] ='\0';
            break;
        default:
            break;
        }
    default:
        break;
    }
    return (0);
}


WIN_ENTRYSC LRESULT WIN_ENTRYMOD SccWndProc(
    HWND hWnd, 
    UINT message, 
    WPARAM wParam, 
    LPARAM lParam)
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

        if (IsWindow(hViewWnd))
            return(SendMessage(hViewWnd,message,wParam,lParam));
        break;

    case SCCVW_FILECHANGE:

        DoFileChange(hWnd);
        break;

    case SCCVW_VIEWTHISFILE:
        /*
        |  Support for embedded objects and hyperlinks could be added here
        */
        return SCCVWERR_MESSAGEHANDLED;
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

        case MENU_FILE_READAHEAD:
            {
                DWORD dwState;
                HMENU hMenu;
                DWORD dwOptions;
                SCCVWOPTIONSPEC40 vwOption;

                hMenu = GetMenu(hWnd);

                bReadAhead = !bReadAhead;
                dwState = (bReadAhead)? (MF_CHECKED|MF_BYCOMMAND) : (MF_UNCHECKED|MF_BYCOMMAND);
                CheckMenuItem(hMenu, MENU_FILE_READAHEAD, (WORD)dwState);
                DrawMenuBar(hWnd);

                if( bReadAhead )
                    dwOptions = 0;
                else
                    dwOptions = SCCVW_SYSTEM_NOREADAHEAD|SCCVW_SYSTEM_NOOPTIONSSAVE;

                vwOption.dwSize  = sizeof(SCCVWOPTIONSPEC40);
                vwOption.dwId    = SCCID_SYSTEMFLAGS;
                vwOption.dwFlags = SCCVWOPTION_CURRENT;
                vwOption.pData   = (LPDWORD)&dwOptions;

                SendMessage(hViewWnd, SCCVW_SETOPTION, 0, (LPARAM)(PSCCVWOPTIONSPEC40)&vwOption);
            }
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

VOID DoCreate(
              HWND  hWnd )
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

VOID DoSize(
            HWND  hWnd,
            WORD  wWidth,
            WORD  wHeight )
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

VOID DoDestroy(
               HWND  hWnd )
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

VOID DoCloseFile(
                 HWND  hWnd )
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

VOID DoFileChange( HWND  hWnd )
{
    SCCVWFILEINFO84  locFileInfo;
    char *defStr = "RFA Client Sample";

    locFileInfo.dwSize = sizeof(locFileInfo);

    if (SendMessage(hViewWnd, SCCVW_GETFILEINFO, 0, (LPARAM)&locFileInfo) == SCCVWERR_OK)
    {
        char *locStr = (char *)malloc(wcslen(locFileInfo.szDisplayName)+wcslen(locFileInfo.szFileIdName)+24);   /* big enough to hold everything */
        if (locStr)
            wsprintf(locStr,"%s - %S - %S",defStr,locFileInfo.szDisplayName,locFileInfo.szFileIdName);
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

VOID DoOpenFile(
                HWND  hWnd )
{
    SCCVWVIEWFILE40  locViewFile;
    DWORD         retCode;
    MYMESSAGEHDR  outMessage, inMessage;
    PMYFILE       pmyFile;
    HANDLE        hmyFile;

    if( !IsWindow(hViewWnd) )
        return;

    DialogBox(hInst,
        MAKEINTRESOURCE(ID_OPENRFADIAG),
        hWnd,
        (DLGPROC)OpenRFADlgProc);
    if( strlen(RemoteFileName) == 0 )
        return;

    DoCloseFile(hWnd);

    /*
    |   Send the OPENFILE message to the RFA server
    */
    outMessage.dwType  = MESSAGE_OPENFILE;
    outMessage.dwFlags = 0;
    outMessage.dwParm1 = 0;
    outMessage.dwParm2 = 0;
    outMessage.dwMoreDataSize = (DWORD)strlen(RemoteFileName)+1;
    outMessage.dwRet=0;
    retCode = RFASendMessage( ghPipe, &outMessage,    sizeof(MYMESSAGEHDR),      NULL );
    retCode = RFASendMessage( ghPipe, RemoteFileName, outMessage.dwMoreDataSize, NULL );

    /*
    |   Retrieve the server's reply to the OPENFILE message
    */
    inMessage.dwType = 0;
    retCode = RFAGetMessage( ghPipe, &inMessage, sizeof(MYMESSAGEHDR), NULL );
    if ( (inMessage.dwType != MESSAGE_OPENFILERET) ||
        (inMessage.dwRet != MESSAGE_RETOK))
    {
        MessageBox (hWnd,
            "Open Remote File Failed.",
            "RFAClnt Error",
            MB_OK);
        return;
    }

    /*
    |   View the remote file through the RFA server
    */
    hmyFile = GlobalAlloc(GMEM_MOVEABLE, sizeof(*pmyFile));
    pmyFile = (PMYFILE)GlobalLock(hmyFile);
    if (!pmyFile)
        return;
    pmyFile->hThis = hmyFile;
    pmyFile->baseRFA.pGetRemoteData       = myGetRemoteData;
    pmyFile->baseRFA.pCloseRemoteData     = myCloseRemoteData;
    pmyFile->baseRFA.pOpenRemoteSubObject = myOpenRemoteSubObject;
    pmyFile->hPipe = ghPipe;
    pmyFile->hRemoteDataBuffer = NULL;
    pmyFile->dwFlag = 0;
    pmyFile->hDoc   = (VTHDOC)(inMessage.dwParm1);

    locViewFile.dwSize      = sizeof(SCCVWVIEWFILE40);
    locViewFile.dwSpecType  = IOTYPE_REMOTEFILTERACCESS;
    locViewFile.pSpec       = (VTVOID *)pmyFile;
    locViewFile.dwViewAs    = 0;
    locViewFile.bUseDisplayName = FALSE;
    locViewFile.bDeleteOnClose = FALSE;
    locViewFile.dwFlags     = 0;
    locViewFile.dwReserved1 = 0;
    locViewFile.dwReserved2 = 0;
    SendMessage(hViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&locViewFile);
}


IO_ENTRYSC IOERR IO_ENTRYMOD myGetRemoteData (
    HIOFILE        hFile, 
    PIORFAREQUEST  pRequest,
    LPVOID *   ppData, 
    LPDWORD        pdwDataSize)
{
    DWORD    retCode;
    DWORD    bytesRead;
    LPBYTE   pData;
    PMYFILE  pmyFile;
    HANDLE   hPipe;
    MYMESSAGEHDR  outMessage;
    MYMESSAGEHDR  inMessage;

    pmyFile = (PMYFILE)hFile;
    hPipe   = pmyFile->hPipe;

    outMessage.dwType = MESSAGE_REQUESTDATA;
    outMessage.dwFlags = 0;
#ifdef WIN64
    outMessage.dwParm1 = (DWORD_PTR)pmyFile->hDoc;
#else
    outMessage.dwParm1 = (DWORD)pmyFile->hDoc;
#endif
    outMessage.dwParm2 = 0;
    outMessage.dwMoreDataSize = sizeof(IORFAREQUEST);
    outMessage.dwRet = 0;
    retCode = RFASendMessage( ghPipe, &outMessage, sizeof(MYMESSAGEHDR), NULL );
    retCode = RFASendMessage( ghPipe, pRequest, outMessage.dwMoreDataSize, NULL );

    inMessage.dwType = 0;
    retCode = RFAGetMessage( ghPipe, &inMessage, sizeof(MYMESSAGEHDR), NULL );
    if ((inMessage.dwType != MESSAGE_REQUESTDATARET) ||
        (inMessage.dwRet != MESSAGE_RETOK))
        return IOERR_UNKNOWN;

    if (pmyFile->hRemoteDataBuffer)
    {
        GlobalUnlock(pmyFile->hRemoteDataBuffer);
        if (pmyFile->dwRemoteDataSize < inMessage.dwMoreDataSize)
        {
            pmyFile->dwRemoteDataSize = inMessage.dwMoreDataSize;
            pmyFile->hRemoteDataBuffer = (HANDLE)GlobalReAlloc(pmyFile->hRemoteDataBuffer, inMessage.dwMoreDataSize, GMEM_MOVEABLE);
        }
    }
    else
    {
        pmyFile->dwRemoteDataSize = inMessage.dwMoreDataSize;
        pmyFile->hRemoteDataBuffer = (HANDLE)GlobalAlloc(GMEM_MOVEABLE, inMessage.dwMoreDataSize);
    }
    pData = (LPBYTE) GlobalLock(pmyFile->hRemoteDataBuffer);
    retCode = RFAGetMessage( ghPipe, pData, inMessage.dwMoreDataSize, &bytesRead );
    if (bytesRead != inMessage.dwMoreDataSize)
    {
        char errorBuf[256];
        wsprintf (errorBuf, "mygetData from remote read failed");
        MessageBox (NULL, errorBuf, "Debug Window",
            MB_ICONINFORMATION | MB_OK | MB_APPLMODAL);
        return IOERR_UNKNOWN;
    }
    *ppData = pData;
    *pdwDataSize = inMessage.dwMoreDataSize;
    return IOERR_OK;
}

IO_ENTRYSC IOERR IO_ENTRYMOD myCloseRemoteData (
    HIOFILE hFile)
{
    DWORD    retCode;
    PMYFILE  pmyFile;
    HANDLE   hPipe;
    MYMESSAGEHDR outMessage;
    MYMESSAGEHDR inMessage;

    pmyFile = (PMYFILE)hFile;
    hPipe = pmyFile->hPipe;

    if (pmyFile->hRemoteDataBuffer)
    {
        GlobalUnlock(pmyFile->hRemoteDataBuffer);
        GlobalFree(pmyFile->hRemoteDataBuffer);
        pmyFile->hRemoteDataBuffer = NULL;
    }
    outMessage.dwType = MESSAGE_CLOSEFILE;
    outMessage.dwFlags = 0;
#ifdef WIN64
    outMessage.dwParm1=(DWORD_PTR)(pmyFile->hDoc);
#else
    outMessage.dwParm1=(DWORD)(pmyFile->hDoc);
#endif
    outMessage.dwParm2 = 0;
    outMessage.dwMoreDataSize = 0;
    outMessage.dwRet = 0;
    retCode = RFASendMessage( hPipe, &outMessage, sizeof(MYMESSAGEHDR), NULL );
    GlobalUnlock(pmyFile->hThis);
    GlobalFree(pmyFile->hThis);
    inMessage.dwType = 0;
    retCode = RFAGetMessage( hPipe, &inMessage, sizeof(MYMESSAGEHDR), NULL );
    if ((inMessage.dwType != MESSAGE_CLOSEFILERET) ||
        (inMessage.dwRet != MESSAGE_RETOK))
        return IOERR_UNKNOWN;
    else
        return IOERR_OK;
}


IO_ENTRYSC IOERR IO_ENTRYMOD myOpenRemoteSubObject (
    HIOFILE   hFile,
    VTDWORD   dwObjectId, 
    VTDWORD   dwStreamId,
    VTDWORD   dwReserved1,
    VTDWORD   dwReserved2,
    DWORD * pdwSpecType,
    LPVOID * ppSpec)
{
    DWORD    retCode;
    MYMESSAGEHDR outMessage, inMessage;
    PMYFILE  pmyFile;
    HANDLE   hPipe;
    HANDLE   hObjectFile;
    PMYFILE  pObjectFile;
    IOSPECSUBOBJECT ObjectSpec;

    pmyFile = (PMYFILE)hFile;
    hPipe   = pmyFile->hPipe;

    ObjectSpec.dwStructSize = sizeof(IOSPECSUBOBJECT);
    ObjectSpec.hDoc         = pmyFile->hDoc;
    ObjectSpec.dwObjectId   = dwObjectId;
    ObjectSpec.dwStreamId   = dwStreamId;
    ObjectSpec.dwReserved1  = dwReserved1;
    ObjectSpec.dwReserved2  = dwReserved2;

    outMessage.dwType  = MESSAGE_OPENSUBOBJECT;
    outMessage.dwFlags = 0;
#ifdef WIN64
    outMessage.dwParm1 = (DWORD_PTR)(pmyFile->hDoc);
#else
    outMessage.dwParm1 = (DWORD)(pmyFile->hDoc);
#endif
    outMessage.dwParm2 = dwObjectId;
    outMessage.dwMoreDataSize = sizeof(IOSPECSUBOBJECT);
    outMessage.dwRet = MESSAGE_RETOK;
    retCode = RFASendMessage( ghPipe, &outMessage, sizeof(MYMESSAGEHDR), NULL );
    retCode = RFASendMessage( ghPipe, &ObjectSpec, outMessage.dwMoreDataSize, NULL );

    inMessage.dwType = 0;
    retCode = RFAGetMessage( hPipe, &inMessage, sizeof(MYMESSAGEHDR), NULL );
    if ( (inMessage.dwType != MESSAGE_OPENSUBOBJECTRET) || (inMessage.dwRet != MESSAGE_RETOK))
    {
        return IOERR_UNKNOWN;
    }

    hObjectFile = GlobalAlloc(GMEM_MOVEABLE, sizeof(*pObjectFile));
    pObjectFile = (PMYFILE)GlobalLock(hObjectFile);
    if (!pObjectFile)
        return IOERR_UNKNOWN;
    pObjectFile->hThis = hObjectFile;
    pObjectFile->baseRFA.pGetRemoteData = myGetRemoteData;
    pObjectFile->baseRFA.pCloseRemoteData = myCloseRemoteData;
    pObjectFile->baseRFA.pOpenRemoteSubObject = myOpenRemoteSubObject;
    pObjectFile->hPipe = hPipe;
    pObjectFile->dwFlag = 0;
    pObjectFile->hDoc = (VTHDOC)(inMessage.dwParm1);
    pObjectFile->hRemoteDataBuffer = NULL;

    *pdwSpecType = IOTYPE_REMOTEFILTERACCESS;
    *ppSpec = (LPVOID)pObjectFile;
    return IOERR_OK;
}


DWORD RFAOpenPipe(
                  HANDLE * phPipe,
                  LPSTR szFileName )
{
    DWORD  dwRetVal = 0;

    // Do CreateFile() to connect to the
    // named pipe.
    *phPipe = CreateFile (
        szFileName,                   // Pipe name.
        GENERIC_WRITE|GENERIC_READ, // Generic access, read/write.
        FILE_SHARE_READ| FILE_SHARE_WRITE, // Share both read and write.
        NULL,                       // No security.
        OPEN_EXISTING,              // Fail if not existing.
        0,
        NULL);                      // No template.

    if (*phPipe == INVALID_HANDLE_VALUE)
    {
        dwRetVal = GetLastError();
        // This error means pipe wasn't found.
        if ((dwRetVal == ERROR_SEEK_ON_DEVICE) ||
            (dwRetVal == ERROR_FILE_NOT_FOUND))
        {
            MessageBox (NULL,
                "CANNOT FIND PIPE: Assure RFASERV is started, check share name.",
                "RFAClnt Error",
                MB_OK);
        }
        else
        {                           // Flagging unknown errors.
            char errorBuf[256];

            wsprintf (errorBuf,
                "CreateFile() on pipe failed, see winerror.h error #%d.",
                dwRetVal);
            MessageBox (NULL, errorBuf, "RFAClnt Error",
                MB_ICONINFORMATION | MB_OK | MB_APPLMODAL);
        }
        dwRetVal = (DWORD)-1;
    }

    return dwRetVal;
}

DWORD RFAClosePipe(
                   HANDLE hPipe )
{
    CloseHandle(hPipe);
    return 0;
}

DWORD RFASendMessage( 
                     HANDLE hPipe,
                     VOID * lpData,
                     DWORD      dwDataSize,
                     LPDWORD    lpBytesWritten )
{
    DWORD dwResult;
    DWORD dwBytesWritten;


    dwResult = WriteFile( hPipe, lpData, dwDataSize, &dwBytesWritten, NULL );

    if( lpBytesWritten )
        *lpBytesWritten = dwBytesWritten;
    return dwResult;
}

DWORD RFAGetMessage(
                    HANDLE hPipe,
                    VOID * lpData,
                    DWORD      dwDataSize,
                    LPDWORD    lpBytesRead )
{
    DWORD dwResult;
    DWORD dwBytesRead;

    dwResult = ReadFile( hPipe, lpData, dwDataSize, &dwBytesRead, NULL );

    if( lpBytesRead )
        *lpBytesRead = dwBytesRead;
    return dwResult;
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
            lstrcpy(buffer, "RFAClient Sample Application\n");
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
