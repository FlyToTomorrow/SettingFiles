/*
|  Outside In Viewer Technology
|  Sample Application
|
|
|  REDIRECT
|  Viewer Technology sample application
|
|  Purpose:
|  Shows how to redirect the Viewer Technology's IO system.
|  As a trivial example, it redirects the Viewer's IO routines through
|  the C runtime FILE routines (fopen, fgetc, etc...)
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
#include <stdio.h>

#include "sccvw.h"

#include "redirect.h"
#include "redirect.pro"
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
|  Typedefs
|  Typedefs
|  Typedefs
|
*/

typedef struct MYFILEtag
{
    BASEIO  sBaseIO;
    FILE *  pFile;
    char    *pszFileName;
    HANDLE  hThis;
} MYFILE, * LPMYFILE;

typedef struct SECINFOtag
{
    char *pFileName;
} SECINFO, * LPSECINFO;

/*
|
|  Globals
|  Globals
|  Globals
|
*/

HANDLE    hInst;            /* Handle of the current instance */
HWND      hMainWnd;         /* Handle to top level window */
HWND      hViewWnd;         /* Handle to the view window */
char      szExePath[256];   /* Path to this executable */
HANDLE    hViewerLibrary;   /* Handle to SCCVW.DLL */
char      szFile[256];      /* Global file name used to call fopen */
/*MYFILE    sMyFile; */         /* File structure */
FILE *    pLogFile;         /* File to log IO activity to */
char      szTemp[256];

HANDLE			hsecInfoList;
SECINFO *	secInfoList;
int				secInfoLastIndex = 0;
int				secInfoAllocated = 0;


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
    |  Open the log file
    */

    pLogFile = fopen("redirect.log","wb");

    /*
    | Create main window
    */

    hMainWnd = CreateWindow(
        (LPSTR) "SCC_MAIN",                   /* window class    */
        "OIVT Redirect Sample",                /* window name      */
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

    if (hsecInfoList)
    {
        while (secInfoLastIndex > 0)
        {
            secInfoLastIndex--;
            free((void *)secInfoList[secInfoLastIndex].pFileName);
        }
        GlobalUnlock(hsecInfoList);
        GlobalFree(hsecInfoList);
    }

    if (pLogFile)
        fclose(pLogFile);

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

        case MENU_EDIT_COPY:

            SendMessage(hViewWnd,SCCVW_COPYTOCLIP,0,0L);
            break;

        case MENU_VIEW_INFO:

            DialogBox(  hInst,
                MAKEINTRESOURCE(IDD_SECONDARYINFO),
                hViewWnd,
                (DLGPROC)DisplaySecondaryInfo);

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


    if (IsWindow(hViewWnd))
    {
        SCCVWOPTIONSPEC40  locOption;
        DWORD          locFlags;

        /*
        | Enable Raw Text.  This is required for annotations.
        */

        locFlags = SCCVW_SYSTEM_RAWTEXT; /* | SCCVW_SYSTEM_NOREADAHEAD;		*/

        locFlags = CS_ASCII;
        locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
        locOption.dwId = SCCID_DEFAULTINPUTCHARSET;
        locOption.dwFlags = SCCVWOPTION_CURRENT | SCCVWOPTION_DEFAULT;
        locOption.pData = (VTVOID *)&locFlags;

        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
    }

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

    if (secInfoAllocated)
    {
        GlobalUnlock(hsecInfoList);
        GlobalFree(hsecInfoList);
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

    /* Reset the index cnt on Secondary info so only the data for the current file is displayed */
    secInfoLastIndex=0;

}

/*
|  Handle SCCVW_FILECHANGE message
*/

VOID DoFileChange(HWND hWnd)
{
    SCCVWFILEINFO40  locFileInfo;
    char *defStr = "OIVT Redirect Sample";

    locFileInfo.dwSize = sizeof(locFileInfo);

    if (SendMessage(hViewWnd, SCCVW_GETFILEINFO, 0, (LPARAM)&locFileInfo) == SCCVWERR_OK)
    {
        char *locStr = (char *)malloc(strlen(locFileInfo.szDisplayName)+strlen(locFileInfo.szFileIdName)+24);   /* big enough to hold everything */
        if (locStr)
            wsprintf(locStr,"OIVT Redirect - %s - %s",(LPSTR)locFileInfo.szDisplayName,(LPSTR)locFileInfo.szFileIdName);
        else
            locStr = defStr;

        SetWindowText(hWnd,locStr);

        if (locStr != defStr)
            free(locStr);   
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
    LPMYFILE    locMyFilePtr;
    HANDLE      locMyFileHnd;

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
            if (pLogFile)
            {
                wsprintf(szTemp,"Open file %s\r\n",(LPSTR)locFileName);
                fputs(szTemp,pLogFile);
            }

            locMyFileHnd = GlobalAlloc(GMEM_ZEROINIT,sizeof(MYFILE));
            locMyFilePtr = (LPMYFILE) GlobalLock ( locMyFileHnd );

            locMyFilePtr->hThis = locMyFileHnd;
            locMyFilePtr->pszFileName = (char *)malloc(strlen(locFileName)+1);
            lstrcpy((LPSTR)locMyFilePtr->pszFileName,locFileName);

            lstrcpy((LPSTR)szFile,locFileName);
            locMyFilePtr->pFile = fopen(szFile,"rb");

            if (locMyFilePtr->pFile != NULL)
            {
                SCCVWVIEWFILE40  locViewFile;

                /* Reset the index cnt on Secondary info so only the data for the current file is displayed */
                secInfoLastIndex=0;

                locMyFilePtr->sBaseIO.pClose = MyClose;
                locMyFilePtr->sBaseIO.pRead = MyRead;
                locMyFilePtr->sBaseIO.pWrite = NULL;
                locMyFilePtr->sBaseIO.pSeek = MySeek;
                locMyFilePtr->sBaseIO.pTell = MyTell;
                locMyFilePtr->sBaseIO.pGetInfo = MyGetInfo;
                locMyFilePtr->sBaseIO.pOpen = NULL;
                locMyFilePtr->sBaseIO.pSeek64 = MySeek64;
                locMyFilePtr->sBaseIO.pTell64 = MyTell64;

                locViewFile.dwSize = sizeof(SCCVWVIEWFILE40);
                locViewFile.dwSpecType = IOTYPE_REDIRECT;
                locViewFile.pSpec = (VTVOID *) locMyFilePtr;
                locViewFile.dwViewAs = 0;
                locViewFile.bUseDisplayName = FALSE;
                locViewFile.bDeleteOnClose = FALSE;
                locViewFile.dwFlags = 0;
                locViewFile.dwReserved1 = 0;
                locViewFile.dwReserved2 = 0;

                SendMessage(hViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&locViewFile);

                SetFocus(hViewWnd);
            }
            else
            {
                GlobalUnlock(locMyFileHnd);
                GlobalFree(locMyFileHnd);
            }
        }
    }
}

IO_ENTRYSC IOERR IO_ENTRYMOD MyClose(HIOFILE hFile)
{
    LPMYFILE  pMyFile = (LPMYFILE)hFile;
    HANDLE    locThis;
    IOERR     locRet;

    if (pLogFile)
    {
        wsprintf(szTemp,"Close\r\n");
        fputs(szTemp,pLogFile);
    }

    if (pMyFile->pFile != NULL)
    {
        fclose(pMyFile->pFile);
        if (pMyFile->sBaseIO.pClose) FreeProcInstance((FARPROC)pMyFile->sBaseIO.pClose);
        if (pMyFile->sBaseIO.pRead) FreeProcInstance((FARPROC)pMyFile->sBaseIO.pRead);
        if (pMyFile->sBaseIO.pWrite) FreeProcInstance((FARPROC)pMyFile->sBaseIO.pWrite);
        if (pMyFile->sBaseIO.pSeek) FreeProcInstance((FARPROC)pMyFile->sBaseIO.pSeek);
        if (pMyFile->sBaseIO.pTell) FreeProcInstance((FARPROC)pMyFile->sBaseIO.pTell);
        if (pMyFile->sBaseIO.pGetInfo) FreeProcInstance((FARPROC)pMyFile->sBaseIO.pGetInfo);
        if (pMyFile->sBaseIO.pOpen) FreeProcInstance((FARPROC)pMyFile->sBaseIO.pOpen);
        if (pMyFile->sBaseIO.pSeek64) FreeProcInstance((FARPROC)pMyFile->sBaseIO.pSeek64);
        if (pMyFile->sBaseIO.pTell64) FreeProcInstance((FARPROC)pMyFile->sBaseIO.pTell64);

        locRet = IOERR_OK;
    }
    else
    {
        locRet = IOERR_BADPARAM;
    }

    free(pMyFile->pszFileName);
    locThis =  pMyFile->hThis;
    GlobalUnlock(locThis);
    GlobalFree(locThis);

    return(locRet);
}

IO_ENTRYSC IOERR IO_ENTRYMOD MyRead(HIOFILE hFile, VTBYTE *pData, DWORD dwSize, DWORD * pCount)
{
    LPMYFILE  pMyFile = (LPMYFILE)hFile;
    int      fChar;

    if (pLogFile)
    {
        wsprintf(szTemp,"Read %li bytes\r\n",dwSize);
        fputs(szTemp,pLogFile);
    }

    if (pMyFile->pFile != NULL)
    {
        *pCount = 0;

        while (*pCount != dwSize)
        {
            if ((fChar = fgetc(pMyFile->pFile)) != EOF)
            {
                pData[*pCount] = (char)fChar;
                (*pCount)++;
            }
            else
            {
                if (*pCount == 0)
                    return(IOERR_EOF);
                else
                    return(IOERR_OK);

                break;
            }
        }

        return(IOERR_OK);
    }
    else
    {
        return(IOERR_BADPARAM);
    }
}

IO_ENTRYSC IOERR IO_ENTRYMOD MySeek(HIOFILE hFile, WORD wFrom, LONG dwOffset)
{
    LPMYFILE  pMyFile = (LPMYFILE)hFile;
    int      iOrigin;

    if (pLogFile)
    {
        wsprintf(szTemp,"Seek by %li from %hi\r\n",dwOffset,wFrom);
        fputs(szTemp,pLogFile);
    }

    if (pMyFile->pFile != NULL)
    {
        switch (wFrom)
        {
        case IOSEEK_CURRENT:
            iOrigin = SEEK_CUR;
            break;
        case IOSEEK_BOTTOM:
            iOrigin = SEEK_END;
            break;
        case IOSEEK_TOP:
        default:
            iOrigin = SEEK_SET;
            break;
        }

        fseek(pMyFile->pFile, (long)dwOffset, iOrigin);

        return(IOERR_OK);
    }
    else
    {
        return(IOERR_BADPARAM);
    }
}

IO_ENTRYSC IOERR IO_ENTRYMOD MyTell(HIOFILE hFile, DWORD * pOffset)
{
    LPMYFILE  pMyFile = (LPMYFILE)hFile;

    if (pLogFile)
    {
        wsprintf(szTemp,"Tell\r\n");
        fputs(szTemp,pLogFile);
    }

    if (pMyFile->pFile != NULL)
    {
        *pOffset = (DWORD)ftell(pMyFile->pFile);
        return(IOERR_OK);
    }
    else
    {
        return(IOERR_BADPARAM);
    }
}

IO_ENTRYSC IOERR IO_ENTRYMOD MyGetInfo(HIOFILE hFile, DWORD dwInfoId, VOID * pInfo)
{
    LPMYFILE        pMyFile = (LPMYFILE)hFile;
    IOERR           locRet;
    PIOGENSECONDARY locGenSec;
    PIOGENSECONDARYDP locGenSecDP;
    char            locPath[256];
    LPSTR           locStartPtr;
    LPSTR           locScanPtr;
    LPMYFILE        locMyFilePtr;
    HANDLE          locMyFileHnd;
    VTDWORD         dwLen;

    if (pLogFile)
    {
        wsprintf(szTemp,"GetInfo\r\n");
        fputs(szTemp,pLogFile);
    }

    locRet = IOERR_OK;

    switch (dwInfoId)
    {
    case IOGETINFO_OSHANDLE:
        locRet = IOERR_BADINFOID;
        break;
    case IOGETINFO_HSPEC:
        locRet = IOERR_BADINFOID;
        break;

    case IOGETINFO_PATHNAME:
        /* return the path that was previously stored in pMyFile->pszFileName */
        if (strlen(pMyFile->pszFileName) >= MAX_PATH)
            locRet = IOERR_INSUFFICIENTBUFFER;
        else
        {
            strcpy((char *)pInfo, pMyFile->pszFileName);
            locRet = IOERR_OK;
        }
        break;

    case IOGETINFO_DPATHNAME:
        {
            PDPATHNAME pvlp = (PDPATHNAME)pInfo;
            VTDWORD dwLen = (VTDWORD)strlen(pMyFile->pszFileName);
            if (dwLen >= pvlp->dwPathLen)
            {
                pvlp->dwPathLen = dwLen + 1;
                locRet = IOERR_INSUFFICIENTBUFFER;
            }
            else
            {
                strcpy((char *)pvlp->pPath, pMyFile->pszFileName);
                locRet = IOERR_OK;
            }
        }
        break;

    case IOGETINFO_FILENAME:

        /*
        | Find just the file name part of the path.
        */

        lstrcpy(locPath,pMyFile->pszFileName);

        locStartPtr = locScanPtr = locPath;

        while (*locScanPtr != 0x00)
            locScanPtr++;
        while (*locScanPtr != '\\' && *locScanPtr != '/' && *locScanPtr != ':' && locScanPtr != locStartPtr)
            locScanPtr--;
        if (locScanPtr != locStartPtr)
            locScanPtr++;

        if (strlen(locScanPtr) >= MAX_PATH)
            locRet = IOERR_INSUFFICIENTBUFFER;
        else
            lstrcpy((LPSTR)pInfo,locScanPtr);

        break;
    case IOGETINFO_ISOLE2STORAGE:
        locRet = IOERR_FALSE;
        break;
    case IOGETINFO_GENSECONDARY:

        locGenSec = (PIOGENSECONDARY)pInfo;

        /*
        | Build a new path using the path of the original file and the file name requested
        | by the viewer.
        */

        lstrcpy(locPath,pMyFile->pszFileName);

        locStartPtr = locScanPtr = locPath;

        while (*locScanPtr != 0x00)
            locScanPtr++;
        while (*locScanPtr != '\\' && *locScanPtr != '/' && *locScanPtr != ':' && locScanPtr != locStartPtr)
            locScanPtr--;
        if (locScanPtr != locStartPtr)
            locScanPtr++;

        lstrcpy(locScanPtr,locGenSec->pFileName);

        /*
        | Generate a new redirected IO
        */

        locMyFileHnd = GlobalAlloc(GMEM_ZEROINIT,sizeof(MYFILE));
        locMyFilePtr = (LPMYFILE) GlobalLock (locMyFileHnd);

        locMyFilePtr->hThis = locMyFileHnd;
        locMyFilePtr->pszFileName = (char *)malloc(strlen(locPath)+1);
        lstrcpy((LPSTR)locMyFilePtr->pszFileName,locPath);
        lstrcpy((LPSTR)szFile,locPath);
        locMyFilePtr->pFile = fopen(szFile,"rb");

        if (locMyFilePtr->pFile != NULL)
        {
            locMyFilePtr->sBaseIO.pClose = MyClose;
            locMyFilePtr->sBaseIO.pRead = MyRead;
            locMyFilePtr->sBaseIO.pWrite = NULL;
            locMyFilePtr->sBaseIO.pSeek = MySeek;
            locMyFilePtr->sBaseIO.pTell = MyTell;
            locMyFilePtr->sBaseIO.pGetInfo = MyGetInfo;
            locMyFilePtr->sBaseIO.pOpen = NULL;

            locGenSec->pSpec = locMyFilePtr;
            locGenSec->dwSpecType = IOTYPE_REDIRECT;
        }
        else
        {
            GlobalUnlock (locMyFileHnd);
            GlobalFree(locMyFileHnd);
            locRet = IOERR_BADINFOID;
        }

        /* Add the GenInfo data to the list so it can be displayed by the sample application */

        if ( secInfoAllocated == 0 )
        {
            secInfoAllocated = SECINFOALLOCUNIT;
            hsecInfoList = GlobalAlloc(GMEM_ZEROINIT,sizeof(SECINFO) * secInfoAllocated);
        }
        if ( secInfoLastIndex >= secInfoAllocated )
        {
            GlobalUnlock(hsecInfoList);
            secInfoAllocated += SECINFOALLOCUNIT;
            hsecInfoList = GlobalReAlloc(hsecInfoList, sizeof(SECINFO) * secInfoAllocated, GMEM_MOVEABLE);
        }

        secInfoList = (SECINFO *) GlobalLock ( hsecInfoList );

        lstrcpy(secInfoList[secInfoLastIndex].pFileName, locGenSec->pFileName);
        secInfoLastIndex ++;

        break;
    case IOGETINFO_GENSECONDARYDP:

        locGenSecDP = (PIOGENSECONDARYDP)pInfo;

        /*
        | Build a new path using the path of the original file and the file name requested
        | by the viewer.
        */
        if (sizeof(VTVOID *) > locGenSecDP->dwSpecLen)
        {
            locGenSecDP->dwSpecLen = sizeof(VTVOID *);
            return IOERR_INSUFFICIENTBUFFER;
        }


        locStartPtr = locScanPtr = pMyFile->pszFileName;
        while (*locScanPtr != 0x00)
            locScanPtr++;
        while (*locScanPtr != '\\' && *locScanPtr != '/' && *locScanPtr != ':'  && locScanPtr != locStartPtr)
            locScanPtr--;
        if (locScanPtr != locStartPtr)
            locScanPtr++;
        dwLen = (VTDWORD)(locScanPtr - locStartPtr);

        /*
        | Generate a new redirected IO
        */

        locMyFileHnd = GlobalAlloc(GMEM_ZEROINIT,sizeof(MYFILE));
        locMyFilePtr = (LPMYFILE) GlobalLock (locMyFileHnd);

        locMyFilePtr->hThis = locMyFileHnd;

        locMyFilePtr->pszFileName = (char *)malloc(dwLen + strlen(locGenSecDP->pFileName) + 1);
        memcpy(locMyFilePtr->pszFileName,locStartPtr,dwLen);
        lstrcpy((LPSTR)&locMyFilePtr->pszFileName[dwLen],(LPSTR)locGenSecDP->pFileName);

        locMyFilePtr->pFile = fopen(locMyFilePtr->pszFileName,"rb");

        if (locMyFilePtr->pFile != NULL)
        {
            locMyFilePtr->sBaseIO.pClose = MyClose,hInst;
            locMyFilePtr->sBaseIO.pRead = MyRead,hInst;
            locMyFilePtr->sBaseIO.pWrite = NULL;
            locMyFilePtr->sBaseIO.pSeek = MySeek,hInst;
            locMyFilePtr->sBaseIO.pTell = MyTell,hInst;
            locMyFilePtr->sBaseIO.pGetInfo = MyGetInfo,hInst;
            locMyFilePtr->sBaseIO.pOpen = NULL;

            locGenSecDP->pSpec = locMyFilePtr;
            locGenSecDP->dwSpecType = IOTYPE_REDIRECT;
        }
        else
        {
            free(locMyFilePtr->pszFileName);
            GlobalUnlock (locMyFileHnd);
            GlobalFree(locMyFileHnd);
            locRet = IOERR_BADINFOID;
        } 


        /* Add the GenInfo data to the list so it can be displayed by the sample application */

        if ( secInfoAllocated == 0 )
        {
            secInfoAllocated = SECINFOALLOCUNIT;
            hsecInfoList = GlobalAlloc(GMEM_ZEROINIT,sizeof(SECINFO) * secInfoAllocated);
        }
        if ( secInfoLastIndex >= secInfoAllocated )
        {
            GlobalUnlock(hsecInfoList);
            secInfoAllocated += SECINFOALLOCUNIT;
            hsecInfoList = GlobalReAlloc(hsecInfoList, sizeof(SECINFO) * secInfoAllocated, GMEM_MOVEABLE);
        }

        secInfoList = (SECINFO *) GlobalLock ( hsecInfoList );

        secInfoList[secInfoLastIndex].pFileName = (char *)malloc(strlen(locGenSecDP->pFileName)+1);
        lstrcpy((LPSTR)secInfoList[secInfoLastIndex].pFileName, (LPSTR)locGenSecDP->pFileName);
        secInfoLastIndex ++;

    case IOGETINFO_GENSECONDARYFULL:
        locGenSec = (PIOGENSECONDARY)pInfo;

        lstrcpy(locGenSec->pSpec, locGenSec->pFileName);
        locGenSec->dwSpecType = dwInfoId;
        locRet = IOERR_BADINFOID;

        if ( secInfoAllocated == 0 )
        {
            secInfoAllocated = SECINFOALLOCUNIT;
            hsecInfoList = GlobalAlloc(GMEM_ZEROINIT,sizeof(SECINFO) * secInfoAllocated);
        }
        if ( secInfoLastIndex >= secInfoAllocated )
        {
            GlobalUnlock(hsecInfoList);
            secInfoAllocated += SECINFOALLOCUNIT;
            hsecInfoList = GlobalReAlloc(hsecInfoList, sizeof(SECINFO) * secInfoAllocated, GMEM_MOVEABLE);
        }

        secInfoList = (SECINFO *) GlobalLock ( hsecInfoList );

        lstrcpy(secInfoList[secInfoLastIndex].pFileName, locGenSec->pFileName);
        secInfoLastIndex ++;

        break;
    case IOGETINFO_GENSECONDARYFULLDP:
        locGenSecDP = (PIOGENSECONDARYDP)pInfo;

        dwLen = (VTDWORD)(strlen(locGenSecDP->pFileName) + 1);
        if (dwLen > locGenSecDP->dwSpecLen)
        {
            locGenSecDP->dwSpecLen = dwLen;
            return IOERR_INSUFFICIENTBUFFER;
        }
        strcpy((char *)locGenSecDP->pSpec, (char *)locGenSecDP->pFileName);
        locGenSecDP->dwSpecType = dwInfoId;
        locRet = IOERR_BADINFOID;

        if ( secInfoAllocated == 0 )
        {
            secInfoAllocated = SECINFOALLOCUNIT;
            hsecInfoList = GlobalAlloc(GMEM_ZEROINIT,sizeof(SECINFO) * secInfoAllocated);
        }
        if ( secInfoLastIndex >= secInfoAllocated )
        {
            GlobalUnlock(hsecInfoList);
            secInfoAllocated += SECINFOALLOCUNIT;
            hsecInfoList = GlobalReAlloc(hsecInfoList, sizeof(SECINFO) * secInfoAllocated, GMEM_MOVEABLE);
        }

        secInfoList = (SECINFO *) GlobalLock ( hsecInfoList );

        secInfoList[secInfoLastIndex].pFileName = (char *)malloc(strlen(locGenSecDP->pFileName)+1);
        lstrcpy(secInfoList[secInfoLastIndex].pFileName, locGenSecDP->pFileName);
        secInfoLastIndex ++;

        break;


    default:
        locRet = IOERR_BADINFOID;
        break;
    }

    return(locRet);
}

IO_ENTRYSC IOERR IO_ENTRYMOD MySeek64(HIOFILE hFile, VTWORD wFrom, VTOFF_T Offset)
{
    /* Defer to the 32-bit MySeek function
    (64-bit seeking will not be supported by this sample application) */
    return MySeek( hFile, wFrom, (VTLONG)Offset );
}

IO_ENTRYSC IOERR IO_ENTRYMOD MyTell64(HIOFILE hFile, VTOFF_T * pOffset)
{
    /* Defer to the 32-bit MyTell function
    (64-bit seeking will not be supported by this sample application) */
    VTDWORD dwOffset;
    IOERR ieResult = MyTell( hFile, &dwOffset );
    *pOffset = (VTOFF_T)dwOffset;
    return ieResult;
}

WIN_ENTRYSC BOOL WIN_ENTRYMOD DisplaySecondaryInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT				locRect;
    int					locX;
    int					locY;
    int					index;

    UNUSED(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        {
            HWND hListWnd;

            /*
            |	Center dialog
            */

            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            hListWnd = GetDlgItem(hDlg, IDC_SECINFO_LIST);
            for ( index = 0; index < secInfoLastIndex; index ++ )
            {
                SendMessage(hListWnd, LB_ADDSTRING, 0, (LPARAM) secInfoList[index].pFileName);

                SendMessage(hListWnd, LB_SETITEMDATA, index, (LPARAM) index);
            }

            return (TRUE);
        }
    case WM_COMMAND:
        {

            switch(wParam)
            {
            case IDOK:
                EndDialog(hDlg, 1);

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


WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {

            char	buffer[512];

            // Display sample app. version
            lstrcpy(buffer, "Redirect Sample Application\n");
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
