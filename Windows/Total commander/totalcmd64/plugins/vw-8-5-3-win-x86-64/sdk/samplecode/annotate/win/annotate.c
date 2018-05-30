/*
|  Outside In Viewer Technology
|  Sample Application
|
|  ANNOTATE
|  Viewer Technology sample application
|
|  Purpose:
|  Show how to use the Raw Text & Annotation abilites of the view window.
|
|
|  Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.
|
|  You have a royalty-free right to use, modify, reproduce and
|  distribute the Sample Applications (and/or any modified version)
|  in any way you find useful, provided that you agree that Oracle
|  Corporation has no warranty obligations or liability for any
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

#define DEFAULTOEMID  TEXT("99")

#define WIN_ENTRYSC __declspec(dllexport)
#define WIN_ENTRYMOD __stdcall
#define WINMAIN_ENTRYMOD APIENTRY

#define ANNO_MASK_YB   ( 0x08000000 | 0x10000000 )
#define ANNO_MASK_BG   ( 0x08000000 | 0x20000000 )
#define ANNO_MASK_WG   ( 0x08000000 | 0x40000000 )
#define ANNO_MASK_ANY  0x08000000


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

#include "sccvw.h"

#include "annotate.h"
#include "annotate.pro"
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

HANDLE      hInst;             /* Handle of the current instance */
HWND        hMainWnd;          /* Handle to top level window */
HWND        hViewWnd;          /* Handle to the view window */
TCHAR       szExePath[256];    /* Path to this executable */
HANDLE      hViewerLibrary;    /* Handle to SCCVW.DLL */

DWORD       dwAnnoCount;        /* Used to create unique ids for annotations */

#define NO_MORE_ANNOS 0xffffffff

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
        TEXT("SCC_MAIN"),                      /* window class      */
        TEXT("Annotate Sample"),                  /* window name       */
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,   /* window type       */
        CW_USEDEFAULT,                           /* x position        */
        CW_USEDEFAULT,                           /* y position        */
        CW_USEDEFAULT,                           /* width             */
        CW_USEDEFAULT,                           /* height            */
        NULL,                                    /* parent handle     */
        NULL,                                    /* menu or child ID  */
        hInstance,                               /* instance          */
        NULL);                                   /* additional info   */

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
    HWND locCtrl;

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

    case SCCVW_RAWTEXTEVENT:

        if (dwAnnoCount != NO_MORE_ANNOS)
            DoRawText(hWnd, (DWORD)lParam);
        break;

    case SCCVW_ANNOTATIONEVENT:

        DoAnnotationEvent(hWnd, (PSCCVWANNOTATIONEVENT40)lParam);
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

        case MENU_FILE_RETURN:

            DoOpenAnnotate(hWnd);
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

        case MENU_GOTO_FIRST_YB:
        case MENU_GOTO_NEXT_YB:
        case MENU_GOTO_PREV_YB:
        case MENU_GOTO_LAST_YB:
        case MENU_GOTO_FIRST_BG:
        case MENU_GOTO_NEXT_BG:
        case MENU_GOTO_PREV_BG:
        case MENU_GOTO_LAST_BG:
        case MENU_GOTO_FIRST_WG:
        case MENU_GOTO_NEXT_WG:
        case MENU_GOTO_PREV_WG:
        case MENU_GOTO_LAST_WG:
        case MENU_GOTO_FIRST:
        case MENU_GOTO_NEXT:
        case MENU_GOTO_PREV:
        case MENU_GOTO_LAST:

            DoGoto(hWnd,locId);
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

    /*
    |  Use the SCCID_SYSTEMFLAGS options to turn RAWTEXT on
    */

    if (IsWindow(hViewWnd))
    {
        SCCVWOPTIONSPEC40  locOption;
        DWORD          locFlags;

        locFlags = SCCVW_SYSTEM_RAWTEXT;

        locOption.dwSize  = sizeof(SCCVWOPTIONSPEC40);
        locOption.dwId    = SCCID_SYSTEMFLAGS;
        locOption.dwFlags = SCCVWOPTION_CURRENT;
        locOption.pData   = &locFlags;

        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
    }

    /*
    |
    |  View the startup document ANNOTATE.DOC
    |
    */

    if (IsWindow(hViewWnd))
    {
        DoOpenAnnotate(hWnd);
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
    char *defStr = "Annotate Sample";

    locFileInfo.dwSize = sizeof(locFileInfo);

    if (SendMessage(hViewWnd, SCCVW_GETFILEINFO, 0, (LPARAM)&locFileInfo) == SCCVWERR_OK)
    {
        char *locStr = (char *)malloc(wcslen(locFileInfo.szDisplayName)+wcslen(locFileInfo.szFileIdName)+24);   /* big enough to hold everything */
        if (locStr)
            wsprintf(locStr,"%s %S %S",defStr,locFileInfo.szDisplayName,locFileInfo.szFileIdName);
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
    TCHAR        locFilterSpec[128] = TEXT("All files\0*.*\0");
    TCHAR        locFileName[MAXFILENAME];
    TCHAR        locFileTitle[MAXFILENAME];

    dwAnnoCount = 0;

    locFileName[0] = 0x00;
    locFileTitle[0] = 0x00;

    locOFN.lStructSize = sizeof(OPENFILENAME);
    locOFN.hwndOwner   = hWnd;
    locOFN.hInstance   = hInst;
    locOFN.lpstrFilter = locFilterSpec;
    locOFN.lpstrCustomFilter = NULL;
    locOFN.nMaxCustFilter = 0L;
    locOFN.nFilterIndex   = 0L;
    locOFN.lpstrFile      = locFileName;
    locOFN.nMaxFile       = MAXFILENAME;
    locOFN.lpstrFileTitle = locFileTitle;
    locOFN.nMaxFileTitle  = MAXFILENAME;
    locOFN.lpstrInitialDir = NULL;
    locOFN.lpstrTitle     = TEXT("Open...");
    locOFN.Flags          = OFN_FILEMUSTEXIST;
    locOFN.lpstrDefExt    = NULL;
    locOFN.lCustData      = 0;
    locOFN.lpfnHook       = NULL;
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


#define STRSTR strstr

VOID DoRawText(HWND hWnd, DWORD dwCharCount)
{
    BYTE *          locTextStart;
    BYTE *          locText;
    SCCVWHILITETEXT40   locHilite;
    SCCVWMAPPOSITION41  locMapPos;
    SCCVWGETRAWTEXT50	sRaw;
    DWORD               dwTextCount, dwStartTextCount;
    DWORD               dwMatchLen;
    BYTE                bThe[4];
    UNUSED(hWnd);


    bThe[0] = 0x74;	// t
    bThe[1] = 0x68;	// h
    bThe[2] = 0x65;	// e
    bThe[3] = 0x00;	//	null terminator

    if (dwCharCount != (DWORD)-1)
    {
        /*
        |  Demonstrates RawText and Highlighting
        |  The first occurance of the text "the" is highlighted
        |  with Yellow on Blue, the next is highlighted
        |  with Black on Green, the next is highlighted
        |  with White on Gray and then the pattern repeats.
        */
        sRaw.dwSize    = sizeof ( SCCVWGETRAWTEXT50 );
        sRaw.dwOptions = 0;                         
        sRaw.dwACC     = dwCharCount;
        if ( SendMessage(hViewWnd,SCCVW_GETRAWTEXT,0,(LPARAM)(PSCCVWGETRAWTEXT50)&sRaw) != SCCVWERR_OK )
            return;

        dwStartTextCount = dwTextCount = sRaw.dwCharCount;

        locTextStart = locText = (BYTE *)GlobalLock(sRaw.hText);

        locMapPos.dwSize       = sizeof(SCCVWMAPPOSITION41);
        locMapPos.dwMapOptions = SCCVW_MAPACCTOPOS;

        dwMatchLen = 0;
        while ((locText = DoMatchIt(sRaw.dwCharSet, locText, bThe, &dwTextCount, &dwMatchLen)) != NULL)
        {
            locHilite.dwSize = sizeof(SCCVWHILITETEXT40);

            locMapPos.dwACC = sRaw.dwTextOffset + (DWORD)(dwStartTextCount - dwTextCount);
            SendMessage(hViewWnd, SCCVW_MAPPOSITION, 0, (LPARAM)(SCCVWMAPPOSITION41 *)&locMapPos);
            locHilite.dwStartPos = locMapPos.sPos;

            locMapPos.dwACC = locMapPos.dwACC + dwMatchLen;
            SendMessage(hViewWnd, SCCVW_MAPPOSITION, 0, (LPARAM)(SCCVWMAPPOSITION41 *)&locMapPos);
            locHilite.dwEndPos   = locMapPos.sPos;

            locHilite.dwInteraction = SCCVW_EVENTSINGLECLICK | SCCVW_EVENTDOUBLECLICK;

            switch (dwAnnoCount % 3)
            {
            case 0:
                locHilite.dwUser = dwAnnoCount | ANNO_MASK_YB;
                locHilite.dwDisplay  = SCCVW_BBLUE | SCCVW_FYELLOW;
                break;
            case 1:
                locHilite.dwUser = dwAnnoCount | ANNO_MASK_BG;
                locHilite.dwDisplay = SCCVW_BGREEN | SCCVW_FBLACK;
                break;
            case 2:
                locHilite.dwUser = dwAnnoCount | ANNO_MASK_WG;
                locHilite.dwDisplay = SCCVW_BLIGHTGRAY | SCCVW_FWHITE;
                break;
            }

            if (SendMessage(hViewWnd,SCCVW_ADDANNOTATION,SCCVW_HILITETEXT,(LPARAM)(PSCCVWHILITETEXT40)&locHilite) != SCCVWERR_OK)
            {
                dwAnnoCount = NO_MORE_ANNOS;
                break;
            }

            dwAnnoCount++;
            dwTextCount--;
            locText++;
        }


        GlobalUnlock(sRaw.hText);
        GlobalFree(sRaw.hText);
    }
}

/* Return TRUE if bChar is the lead byte for a multi-byte character set. */
BOOL   isLeadByte(DWORD dwCharSet, BYTE bChar)
{
	switch (dwCharSet)
	{
		case SO_ANSI932:
			if ((bChar >= 0x81 && bChar <= 0x9f) ||
				(bChar >= 0xe0 && bChar <= 0xfc))
				return TRUE;
			break;

		case SO_ANSI949:
		/* and according to http://www.microsoft.com/globaldev/reference/dbcs/949.mspx (or http://msdn.microsoft.com/en-us/goglobal/cc305154.aspx)
		 * 0x81 - 0xFE is also the correct range for 949, so fall through to that.
		 */

		case SO_ANSI936:
		/*USER100155908 Elektra  According to http://www.microsoft.com/globaldev/reference/dbcs/936.mspx, this is the correct range
		 |for 936.
		 */
			if (bChar >= 0x81 && bChar <= 0xfe)
				return TRUE;
			break;

		case SO_ANSI950:
			if ((bChar >= 0xa1 && bChar <= 0xc6) ||
				(bChar >= 0xc9 && bChar <= 0xf9))
				return TRUE;
			break;
	}
	return FALSE;
}

/* Multi-byte aware character iterator.
   Return the character at p and increment p to point to the next character.
 */
WORD   nextChar(DWORD dwCharSet, LPBYTE *p)
{
    LPBYTE   pc = *p;
    WORD     c;

    if (isLeadByte(dwCharSet, *pc))
    {
        c = (WORD)(((*pc) << 8) | *(pc+1));
        pc += 2;
    }
    else
    {
        c = (WORD)(*pc);
        ++pc;
    }
    *p = pc;
    return c;
}

/*------------------------------------------------------------------------
Simple non case sensitive matching algorithm.
We assume the input string pText is in lower case.

Parameters:

dwCharSet       Character set used in the text buffer
pBuffer         Text buffer to search
pText           Search string
pCharCount      Count of characters (not bytes) in the text buffer
pMatchLen       Returns the number of characters in the matched string

If the search string is found, the return value will be a pointer to the
matched text in the buffer and *pCharCount will be updated to be the count
of characters (not bytes) in the buffer starting at the search hit.
If the search string is not found, the return value will be NULL.
-------------------------------------------------------------------------*/
LPBYTE	DoMatchIt (DWORD dwCharSet, LPBYTE pBuffer, LPBYTE pText ,LPDWORD pCharCount, LPDWORD pMatchLen)
{
    LPBYTE	p1;
    LPBYTE	p2;
    DWORD	locCount;

    *pMatchLen = 0;
    while ( *pCharCount != 0 )
    {
        WORD   c1, c2;

        p1 = pBuffer;
        p2 = pText;
        locCount = *pCharCount;
        for (;;)
        {
            if ( *p2 == '\0' ) // done
            {
                *pMatchLen = (*pCharCount) - locCount;
                return ( pBuffer );
            }
            while ((*p1 == '\0') && (locCount)) // Skip nulls in buffer
            {
                p1++;
                locCount--;
            }

            c1 = nextChar(dwCharSet, &p1);
            c2 = nextChar(dwCharSet, &p2);
            if (c1 < 128) c1 = (WORD)toupper(c1);
            if (c2 < 128) c2 = (WORD)toupper(c2);

            if (c1 != c2)
                break;
            --locCount;
        }
        nextChar(dwCharSet, &pBuffer);
        --(*pCharCount);
    }
    return ( NULL );
}

VOID DoAnnotationEvent(HWND hWnd, PSCCVWANNOTATIONEVENT40 pEvent)
{
    UNUSED(hWnd);
    switch (pEvent->dwEvent)
    {
    case SCCVW_EVENTSINGLECLICK:
        break;
    case SCCVW_EVENTDOUBLECLICK:
        break;
    default:
        break;
    }
}

VOID DoGoto(HWND hWnd, WORD wId)
{
    UNUSED(hWnd);
    switch (wId)
    {
    case MENU_GOTO_FIRST_YB:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOFIRST | SCCVW_MASK, (LPARAM)ANNO_MASK_YB);
        break;
    case MENU_GOTO_NEXT_YB:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTONEXT | SCCVW_MASK, (LPARAM)ANNO_MASK_YB);
        break;
    case MENU_GOTO_PREV_YB:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOPREV | SCCVW_MASK, (LPARAM)ANNO_MASK_YB);
        break;
    case MENU_GOTO_LAST_YB:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOLAST | SCCVW_MASK, (LPARAM)ANNO_MASK_YB);
        break;
    case MENU_GOTO_FIRST_BG:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOFIRST | SCCVW_MASK, (LPARAM)ANNO_MASK_BG);
        break;
    case MENU_GOTO_NEXT_BG:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTONEXT | SCCVW_MASK, (LPARAM)ANNO_MASK_BG);
        break;
    case MENU_GOTO_PREV_BG:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOPREV | SCCVW_MASK, (LPARAM)ANNO_MASK_BG);
        break;
    case MENU_GOTO_LAST_BG:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOLAST | SCCVW_MASK, (LPARAM)ANNO_MASK_BG);
        break;
    case MENU_GOTO_FIRST_WG:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOFIRST | SCCVW_MASK, (LPARAM)ANNO_MASK_WG);
        break;
    case MENU_GOTO_NEXT_WG:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTONEXT | SCCVW_MASK, (LPARAM)ANNO_MASK_WG);
        break;
    case MENU_GOTO_PREV_WG:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOPREV | SCCVW_MASK, (LPARAM)ANNO_MASK_WG);
        break;
    case MENU_GOTO_LAST_WG:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOLAST | SCCVW_MASK, (LPARAM)ANNO_MASK_WG);
        break;
    case MENU_GOTO_FIRST:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOFIRST | SCCVW_MASK, (LPARAM)ANNO_MASK_ANY);
        break;
    case MENU_GOTO_NEXT:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTONEXT | SCCVW_MASK, (LPARAM)ANNO_MASK_ANY);
        break;
    case MENU_GOTO_PREV:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOPREV | SCCVW_MASK, (LPARAM)ANNO_MASK_ANY);
        break;
    case MENU_GOTO_LAST:
        SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOLAST | SCCVW_MASK, (LPARAM)ANNO_MASK_ANY);
        break;
    }
}

VOID DoOpenAnnotate(HWND hWnd)
{
    UNUSED(hWnd);
    if (IsWindow(hViewWnd))
    {
        SCCVWVIEWFILE40   locViewFile;
        TCHAR             locFile[256];

        lstrcpy(locFile,szExePath);
        lstrcat(locFile,TEXT("..\\files\\annotate.doc"));

        if ( GetFileAttributes( (LPCTSTR) locFile ) == 0xffffffff )
        {
            lstrcpy(locFile,szExePath);
            lstrcat(locFile,TEXT("files\\annotate.doc"));
            if ( GetFileAttributes( (LPCTSTR) locFile ) == 0xffffffff )
                return;
        }

        locViewFile.dwSize = sizeof(SCCVWVIEWFILE40);
        locViewFile.dwSpecType  = IOTYPE_ANSIPATH;
        locViewFile.pSpec       = (VTVOID *)locFile;
        locViewFile.dwViewAs    = 0;
        locViewFile.bUseDisplayName = FALSE;
        locViewFile.bDeleteOnClose = FALSE;
        locViewFile.dwFlags     = 0;
        locViewFile.dwReserved1 = 0;
        locViewFile.dwReserved2 = 0;

        SendMessage(hViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&locViewFile);
    }
}

WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {

            TCHAR buffer[512];

            // Display sample app. version
            lstrcpy(buffer, TEXT("Annotate Sample Application\n"));
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
