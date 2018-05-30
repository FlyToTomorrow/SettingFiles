/*
|  Outside In Viewer Technology
|  Sample Application
|
|  ANNOEX
|  Viewer Technology sample application
|
|  Purpose:
|  Show how to use the Get Raw Text, Annotation, and Positioning
|  abilites of the view window.  Text Access is also demonstrated
|  in this sample with a comparison against Raw Text.
|
|
|  Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.
|
|  You have a royalty-free right to use, modify, reproduce and
|  distribute the Sample Applications (and/or any modified version)
|  in any way you find useful, provided that you agree that Oracle
|  has no warranty obligations or liability for any Sample Application 
|  files which are modified.
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
#include <string.h>
#include "sccta.h"
#include "sccvw.h"
#include "annoex.h"
#include "annoex.pro"
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

HINSTANCE   hInst;             /* Handle of the current instance */
HWND        hMainWnd;          /* Handle to top level window */
HWND        hViewWnd;          /* Handle to the view window */
VTCHAR      szExePath[256];    /* Path to this executable */
HANDLE      hViewerLibrary;    /* Handle to SCCVW.DLL */
VTCHAR      szURL[2048];

#define MAXFILENAME     256
VTCHAR      gFileName[MAXFILENAME];
WORD        gFileId;
VTCHAR      szFileId[100];

WORD        wAnnoCount;
WORD        wBookMarkCount = 0;

#define MAXTABUFSIZE    1024

WORD        gTAOffset = 0;
DWORD       gTABufSize = 0;
DWORD       gTATotal = 0;
DWORD       gCmpTotal = 0;
VTHDOC      ghDoc  = VTHDOC_INVALID;
VTHTEXT     ghText = VTHDOC_INVALID;
DAERR       gErrTA = 0;
VTCHAR      gTABuf[MAXTABUFSIZE];
VTCHAR      gSearchString[80];
SCCVWSEARCHINFO40  gSearchInfo;
HWND               hDlgReplace = (HWND)NULL;

#define MASK_BOOKMARK      0x00010000
#define MASK_SEARCHHITS    0x00020000
#define MASK_HIDDENTEXT    0x00040000

#define ANNOEX_STYLE_SEARCHHIT  1
#define ANNOEX_STYLE_CUSTOM     2

SCCVWHILITESTYLE81  gCustomStyle = {0};

#ifndef SCCVW_USETHINUNDERLINE
#define SCCVW_USETHINUNDERLINE 0x00000010
#endif

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
        WndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
        WndClass.lpszMenuName =  "SCC_MENU";
        WndClass.lpszClassName =  "SCC_MAIN";

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
        "SCC_MAIN",                              /* window class     */
        "Annoex Sample",                         /* window name      */
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,   /* window type      */
        CW_USEDEFAULT,                           /* x position       */
        CW_USEDEFAULT,                           /* y position       */
        CW_USEDEFAULT,                           /* width            */
        CW_USEDEFAULT,                           /* height           */
        NULL,                                    /* parent handle    */
        NULL,                                    /* menu or child ID */
        hInstance,                               /* instance         */
        NULL);                                   /* additional info  */

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
        if( !IsWindow( hDlgReplace ) || 
            !IsDialogMessage( hDlgReplace, &locMsg ) )
        {
            TranslateMessage(&locMsg);
            DispatchMessage(&locMsg);
        }
    }

    return (int)(locMsg.wParam);     /* Returns the value from PostQuitMessage */
}

WIN_ENTRYSC LRESULT WIN_ENTRYMOD SccWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WORD      locId;
    WORD      locEvent;
    HWND      locCtrl;
    LRESULT   locRet;

    locRet = 0;

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

    case ANNOEX_HYPERLINK:
        DoHyperLink ( hWnd );
        break;

    case SCCVW_ANNOTATIONEVENT:
        locRet = DoAnnotationEvent(hWnd, (PSCCVWANNOTATIONEVENT41)lParam);
        break;

    case SCCVW_BAILOUT:
        locRet = DoBailOutMessage(hWnd, (DWORD)lParam );
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

        case MENU_EDIT_CUT:
            SendMessage(hViewWnd,SCCVW_COPYTOCLIP,0,0L);
            DoHideSelection(hViewWnd);
            break;

        case MENU_EDIT_SEARCH:
            DoSearch( TRUE );
            break;

        case MENU_EDIT_SEARCH_NEXT:
            DoSearchNext( TRUE );
            break;

        case MENU_EDIT_REPLACE:
            DoReplace();
            break;

        case MENU_BOOKMARK_DROP:
        case MENU_BOOKMARK_FIRST:
        case MENU_BOOKMARK_LAST:
        case MENU_BOOKMARK_NEXT:
        case MENU_BOOKMARK_PREV:
        case MENU_BOOKMARK_CLEAR:
        case MENU_BOOKMARK_CLEARALL: 
        case MENU_BOOKMARK_DROPBITMAP:
            DoBookMark(hWnd,locId);
            break;

        case MENU_POSITION_FIRST:
        case MENU_POSITION_LAST:
        case MENU_POSITION_PREV:
        case MENU_POSITION_NEXT:
        case MENU_POSITION_STARTDISPLAY:
        case MENU_POSITION_ENDDISPLAY:
        case MENU_POSITION_STARTLINE:
        case MENU_POSITION_ENDLINE:
        case MENU_POSITION_PREVLINE:
        case MENU_POSITION_NEXTLINE:
        case MENU_POSITION_PREVWORD:
        case MENU_POSITION_NEXTWORD:
            DoPosition(hWnd,locId);
            break;

        case MENU_HIGHLIGHT_BY:
        case MENU_HIGHLIGHT_YR:
        case MENU_HIGHLIGHT_CUSTOM:
            DoHighlight(hWnd,locId);
            break;

        case MENU_HIGHLIGHT_SETCUSTOMSTYLE:
            if( DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_CUSTOMHILITE),hWnd,(DLGPROC)CustomHighlightDlg,(LPARAM)&gCustomStyle) )
                SendMessage ( hViewWnd, SCCVW_HILITESTYLE, 0, (LPARAM)(PSCCVWHILITESTYLE81)&gCustomStyle );
            break;

        case MENU_HIGHLIGHT_SEARCHHITS:
            DoSearchHighlight ();
            break;
        case MENU_HIGHLIGHT_NEXTHIT:
            DoSearchNextHighlight ();
            break;
        case MENU_HIGHLIGHT_PREVHIT:
            DoSearchPreviousHighlight ();
            break;
        case MENU_HIGHLIGHT_CLEARHITS:
            DoClearSelectedAnnos ( hViewWnd, MASK_SEARCHHITS );
            break;
        case MENU_HIGHLIGHT_CLEARALLHITS:
            SendMessage(hViewWnd,SCCVW_CLEARANNOTATIONS,SCCVW_MASK,(LPARAM) MASK_SEARCHHITS);
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

    return ( locRet );
}

/*
|  Handle WM_CREATE
*/

VOID DoCreate(HWND hWnd)
{
    char  locViewerPath[256];
    char  locTxPath[256];
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
        DWORD WinErr = GetLastError();
        char buf[256];
        sprintf(buf,"Failed to load viewer DLL.  Error %d",WinErr);
        MessageBox(NULL,buf,"Error",MB_OK | MB_ICONERROR | MB_APPLMODAL);
        DestroyWindow(hWnd);
        return;
    }

    /*
    | Initialize the Text Access DLL
    */

    lstrcpy(locTxPath,szExePath);

    DAInitEx(SCCOPT_INIT_NOTHREADS, OI_INIT_DEFAULT);

    /*
    |  Create the view window here
    */

    lstrcpy(locViewerClass,"SCCVIEWER");

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
        SCCVWHILITESTYLE81 sStyle;
        VTDWORD            locFlags;

        /*
        | Enable Raw Text.  This is required for annotations.
        */

        locFlags = SCCVW_SYSTEM_RAWTEXT;
        locOption.dwSize  = sizeof(SCCVWOPTIONSPEC40);
        locOption.dwId    = SCCID_SYSTEMFLAGS;
        locOption.dwFlags = SCCVWOPTION_CURRENT;
        locOption.pData   = (VTVOID *)&locFlags;

        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);


        locFlags = CS_ASCII;
        locOption.dwSize  = sizeof(SCCVWOPTIONSPEC40);
        locOption.dwId    = SCCID_DEFAULTINPUTCHARSET;
        locOption.dwFlags = SCCVWOPTION_CURRENT | SCCVWOPTION_DEFAULT;
        locOption.pData   = (VTVOID *)&locFlags;

        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);

        /*
        | Define a hilite style for use with user defined searching.
        */
        sStyle.dwSize      = sizeof ( SCCVWHILITESTYLE81 );
        sStyle.dwStyleId   = ANNOEX_STYLE_SEARCHHIT;
        sStyle.dwOptions   = SCCVW_USECHARATTR | SCCVW_USEFOREGROUND | SCCVW_USEBACKGROUND;
        sStyle.sForeground = SCCVWRGB (0,0x80,0);
        sStyle.sBackground = SCCVWRGB (0xFF,0xFF,0x80);
        sStyle.wCharAttr   = SCCVW_CHARATTR_UNDERLINE;
        sStyle.wCharAttrMask = sStyle.wCharAttr;
        SendMessage ( hViewWnd, SCCVW_HILITESTYLE, 0, (LPARAM)(PSCCVWHILITESTYLE81)&sStyle );

        /* 
        Define a custom highlight style.
        */
        gCustomStyle.dwSize = sizeof ( SCCVWHILITESTYLE81 );
        gCustomStyle.dwStyleId = ANNOEX_STYLE_CUSTOM;
        gCustomStyle.dwOptions = SCCVW_USECHARATTR | SCCVW_USEUNDERLINECOLOR | SCCVW_USEBACKGROUND;
        gCustomStyle.sBackground = SCCVWRGB(0xd0,0xd0,0xd0);
        gCustomStyle.sUnderline = SCCVWRGB(0xff,0,0);
        gCustomStyle.wCharAttr = SCCVW_CHARATTR_DASHUNDERLINE;
        gCustomStyle.wCharAttrMask = gCustomStyle.wCharAttr;
        SendMessage ( hViewWnd, SCCVW_HILITESTYLE, 0, (LPARAM)(PSCCVWHILITESTYLE81)&gCustomStyle );



        /*
        | Define a hilite style for use when internal search tagging is applied.
        | This is only relevent to a proprietary format of one OEM.
        */
        sStyle.dwSize      = sizeof ( SCCVWHILITESTYLE81 );
        sStyle.dwStyleId   = SCCVW_STYLESEARCHTAG;
        sStyle.dwOptions   = SCCVW_USECHARATTR | SCCVW_USEFOREGROUND;
        sStyle.sForeground = SCCVWRGB (0,0x80,0);
        sStyle.sBackground = 0;
        sStyle.wCharAttr   = SCCVW_CHARATTR_UNDERLINE;
        sStyle.wCharAttrMask = sStyle.wCharAttr;
        SendMessage ( hViewWnd, SCCVW_HILITESTYLE, 0, (LPARAM)(PSCCVWHILITESTYLE81)&sStyle );

        /*
        | Define a hilite style for use when internal object tagging is applied.
        */
        sStyle.dwSize      = sizeof ( SCCVWHILITESTYLE81 );
        sStyle.dwStyleId   = SCCVW_STYLEOBJECTTAG;
        sStyle.dwOptions   = SCCVW_USECHARATTR | SCCVW_USEFOREGROUND;
        sStyle.sForeground = SCCVWRGB (0,0x80,0);
        sStyle.sBackground = 0;
        sStyle.wCharAttr   = SCCVW_CHARATTR_UNDERLINE;
        sStyle.wCharAttrMask = sStyle.wCharAttr;
        SendMessage ( hViewWnd, SCCVW_HILITESTYLE, 0, (LPARAM)(PSCCVWHILITESTYLE81)&sStyle );

        /* Initialize the search dialog structure */
        gSearchInfo.dwSize = sizeof ( SCCVWSEARCHINFO40 );
        gSearchInfo.siText[0] = 0;
        gSearchInfo.siType = SCCVW_SEARCHNOCASE;
        gSearchInfo.siFrom = SCCVW_SEARCHCURRENT;
        gSearchInfo.siDirection = SCCVW_SEARCHFORWARD;
        gSearchInfo.siTextLen = 0;

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

    DADeInit();
    if (IsWindow(hViewWnd))
    {
        SendMessage(hViewWnd,SCCVW_CLOSEFILE,0,0L);
        DestroyWindow(hViewWnd);
    }

    if (hViewerLibrary != NULL)
    {
        FreeLibrary((HMODULE)hViewerLibrary);
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
    char *defStr = "Annoex Sample";

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
    OPENFILENAME  locOFN;
    char          locFilterSpec[128] = "All files\0*.*\0";
    char          locFileTitle[MAXFILENAME];

    InitTAVars();
    gFileName[0] = 0x00;
    locFileTitle[0] = 0x00;

    locOFN.lStructSize       = sizeof(OPENFILENAME);
    locOFN.hwndOwner         = hWnd;
    locOFN.hInstance         = hInst;
    locOFN.lpstrFilter       = locFilterSpec;
    locOFN.lpstrCustomFilter = NULL;
    locOFN.nMaxCustFilter    = 0L;
    locOFN.nFilterIndex      = 0L;
    locOFN.lpstrFile         = gFileName;
    locOFN.nMaxFile          = MAXFILENAME;
    locOFN.lpstrFileTitle    = locFileTitle;
    locOFN.nMaxFileTitle     = MAXFILENAME;
    locOFN.lpstrInitialDir   = NULL;
    locOFN.lpstrTitle        = "Open...";
    locOFN.Flags             = OFN_FILEMUSTEXIST;
    locOFN.lpstrDefExt       = NULL;
    locOFN.lCustData         = 0;
    locOFN.lpfnHook          = NULL;
    locOFN.lpTemplateName    = NULL;

    if (GetOpenFileName(&locOFN) == TRUE)
    {
        if (IsWindow(hViewWnd))
        {
            SCCVWVIEWFILE40  locViewFile;

            InitTAVars();
            locViewFile.dwSize     = sizeof(SCCVWVIEWFILE40);
            locViewFile.dwSpecType = IOTYPE_ANSIPATH;
            locViewFile.pSpec      = (VTVOID *)gFileName;
            locViewFile.dwViewAs   = 0;
            locViewFile.bUseDisplayName = FALSE;
            locViewFile.bDeleteOnClose  = FALSE;
            locViewFile.dwFlags     = 0;
            locViewFile.dwReserved1 = 0;
            locViewFile.dwReserved2 = 0;

            /* The code below is a test of the FI API */
            {
                HMODULE hLib;
                WORD   (*lpfnFIIdFile)(VTDWORD,VTLPSTR,VTDWORD,VTWORD *);
                if ((hLib = SCCLoadFIDLL(szExePath)) != NULL)
                {
                    WORD  wErr;
                    lpfnFIIdFile = (VTWORD (*)(VTDWORD,VTLPSTR,VTDWORD,VTWORD *))GetProcAddress(hLib,"_FIIDFILE");
                    if ( lpfnFIIdFile != NULL )
                        wErr = (*lpfnFIIdFile)(IOTYPE_ANSIPATH, (VTLPSTR)gFileName, IOOPEN_READ, (VTLPWORD)(&gFileId));

                    FreeLibrary(hLib);
                }
            }

            SendMessage(hViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&locViewFile);

            SetFocus(hViewWnd);
        }
    }
}


/*-------------------------------------------------------------------
This function just outputs a debug string to indicate an event
generated by interaction with an annotation.

It demonstrates the receipt of an SCCVW_ANNOTATIONEVENT message.

It also demonstrates how to support hyperlinking to another 
document by replacing the currently viewed document with the
one hyperlinked.  Note, for most applications, it is not necessary
to trap hyperlink annotations since the default behavior is sufficient
so long as the application supports the SCCVW_VIEWTHISFILE message.

-------------------------------------------------------------------*/

DWORD DoAnnotationEvent(HWND hWnd, PSCCVWANNOTATIONEVENT41	pEvent)
{
    DWORD  locRet;

    switch (pEvent->dwEvent)
    {
    case SCCVW_EVENTSINGLECLICK:
        OutputInfo ( "Left Single Click\r\n" );
        if ( ( pEvent->dwUser & SCCVW_USERHYPERTAG ) == SCCVW_USERHYPERTAG )
        {
            SCCVWGETANNOTATIONDATA sGetAnnoData;
            sGetAnnoData.dwSize = sizeof ( SCCVWGETANNOTATIONDATA );
            sGetAnnoData.dwUser = pEvent->dwUser;
            sGetAnnoData.dwData = (DWORD)pEvent->dwData;
            sGetAnnoData.dwResultDataSize = sizeof(szURL);
            sGetAnnoData.pData = (VTLPVOID) szURL;
            locRet = (DWORD)SendMessage(hViewWnd,SCCVW_GETANNOTATIONDATA,0,(LPARAM)(PSCCVWGETANNOTATIONDATA)&sGetAnnoData);
            if ( locRet == SCCVWERR_OK && sGetAnnoData.dwDataType == ADTYPE_URL &&
                sGetAnnoData.dwResultDataSize && szURL[0] != '#' )
            {
                PostMessage ( hWnd, ANNOEX_HYPERLINK, 0, 0L );
                return(1); /* return non-zero took avoid default hyperlink support */
            }
            /* else let the default hyperlink support handle the link */
        }
        else
        {
             if (DialogBox(hInst, MAKEINTRESOURCE(IDD_LEFTSINGLECLICK), hWnd, (DLGPROC)AnnoLeftSingleClick))
                 return 1;
             else
                 return 0;
        }
        break;
    case SCCVW_EVENTDOUBLECLICK:
         if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DOUBLECLICK), hWnd, (DLGPROC)AnnoDoubleClick))
             return 1;
         else
             return 0;
         break;
        break;
    case SCCVW_EVENTTRANSITIONINTO:
        break;
    case SCCVW_EVENTTRANSITIONOUTOF:
        break;
    case SCCVW_EVENTSINGLERIGHTCLICK:
         if (DialogBox(hInst, MAKEINTRESOURCE(IDD_RIGHTSINGLECLICK), hWnd, (DLGPROC)AnnoRightSingleClick))
             return 1;
         else
             return 0;
         break;
    default:
        OutputInfo ( "Unknown Event\r\n" );
        break;
    } 
    return (0);
}

/*-------------------------------------------------------------------
This function attempts to replace the currently viewed document
by the document which results from resolving the URL stored in
the global string szURL.  This string was retrieved due to a
a single click event on a hyperlink. If the URL includes a pound
sign then the string following the pound sign is treated as a bookmark
and the function attempts to "goto" the bookmark by enumerating all
bookmarks in the document and finding the one that matches.

For this example, the resolution of the URL is limited to locating
a LOCAL file in the same directory as the currently viewed document 
which matches the URL reference.

-------------------------------------------------------------------*/

DWORD  DoHyperLink ( HWND hWnd )
{
    BYTE  szBookMarkName [ 128 ];
    SCCVWVIEWFILE40  locViewFile;
    WORD  i, j, k;

    /* If this is a web address, let's invoke a browser. */
    if( 0 == _strnicmp("http://",szURL,7) )
    {
        ShellExecute(hWnd, "open" ,szURL, NULL, NULL, SW_SHOW);
    }
    else
    {
        /* We're going to try to view a file on disk.
        First, close the file being viewed */
        DoCloseFile(hWnd);


        /* Locate filename proper to replace with new filename */
        for ( i = (WORD)strlen ( gFileName ); (i > 0) && 
            (gFileName[i] != ':' && gFileName[i] != '\\' && gFileName[i] != '/'); i-- )
            ;
        if ( gFileName[i] == ':' || gFileName[i] == '\\' || gFileName[i] == '/' )
            i++;

        /* Extract bookmark name if one exists */
        for ( j = (WORD)strlen ( szURL ); (j>0) && (szURL[j] != '#'); j-- )
            ;
        szBookMarkName[0] = '\0';
        if ( szURL[j] == '#' )
        {
            for ( k=0; szURL[j+k+1] != '\0'; k++ )
                szBookMarkName[k] = szURL[j+k+1];
            szBookMarkName[k] = '\0';
        }
        else
            j = (WORD)strlen ( szURL );

        /* Locate filename proper in URL */
        for ( ; (j > 0) && 
            (szURL[j] != ':' && szURL[j] != '\\' && szURL[j] != '/'); j-- )
            ;
        if ( szURL[j] == ':' || szURL[j] == '\\' || szURL[j] == '/' )
            j++;

        /* Append new file name to gFileName */
        for ( ; szURL [j] != '\0' && szURL [j] != '#'; )
            gFileName [i++] = szURL [j++];
        gFileName [i] = '\0';

        InitTAVars();
        locViewFile.dwSize     = sizeof(SCCVWVIEWFILE40);
        locViewFile.dwSpecType = IOTYPE_ANSIPATH;
        locViewFile.pSpec      = (VTVOID *)gFileName;
        locViewFile.dwViewAs   = 0;
        locViewFile.bUseDisplayName = FALSE;
        locViewFile.bDeleteOnClose  = FALSE;
        locViewFile.dwFlags     = 0;
        locViewFile.dwReserved1 = 0;
        locViewFile.dwReserved2 = 0;

        if ( szBookMarkName[0] != '\0' )
        {
            locViewFile.dwFlags |= SCCVW_GOTOBOOKMARK;
            locViewFile.dwReserved1 = (DWORD_PTR)szBookMarkName;
        }

        if ( SendMessage(hViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&locViewFile) != SCCVWERR_OK )
            MessageBox ( hMainWnd, "Could not view document referenced by hyperlink!", "Hyperlink", MB_ICONINFORMATION );

    }

    return(0);

}


/*-------------------------------------------------------------------------
This function will handle the BailOut message byu putting a simple
string to the screen.  We just want to demonstrate that we are sending
the message and we don't display the error message when the message is handled.
-------------------------------------------------------------------------*/


WORD DoBailOutMessage(HWND hWnd, DWORD dwId)
{
    char	sBuf[100];
    LPSTR   p;

    UNUSED(hWnd);

    switch (dwId)
    {
    case SCCVW_BAILOUT_MEMORY:
        p = "Memory Failure!";
        break;
    case SCCVW_BAILOUT_STREAMBAIL:
        p = "Filter Bailed Out!";
        break;
    case SCCVW_BAILOUT_FILEOPENFAILED:
        p = "File Open Failed!";
        break;
    case SCCVW_BAILOUT_MISSINGELEMENT:
        p = "Couldn't Find Needed Element!";
        break;
    case SCCVW_BAILOUT_BADFILE:
        p = "Bad File Error!";
        break;
    case SCCVW_BAILOUT_PROTECTEDFILE:
        p = "This is Protected File!";
        break;
    case SCCVW_BAILOUT_SUPFILEOPENFAILS:
        p = "Needed Files Not Found!";
        break;
    case SCCVW_BAILOUT_EMPTYFILE:
        p = "This is an Empty File!";
        break;
    case SCCVW_BAILOUT_EMPTYSECTION:
        p = "This section is empty!";
        break;
    case SCCVW_BAILOUT_NOFILTER:
        p = "No Filter Available for this type!";
        break;
    case SCCVW_BAILOUT_WRITEERROR:
        p = "There was a write Error!";
        break;
    case SCCVW_BAILOUT_FILECHANGED:
        p = "The file has changed!";
        break;
    case SCCVW_BAILOUT_GPFAULT:
        p = "General Protection Fault!";
        break;
    case SCCVW_BAILOUT_DIVIDEBYZERO:
        p = "Divide by Zero Error!";
        break;
    case SCCVW_BAILOUT_NOSUPPORTEDFILE:
        p = "This file type is not supported!";
        break;
    case SCCVW_BAILOUT_OTHEREXCEPTION:
        p = "General Exception!";
        break;
    case SCCVW_BAILOUT_NOENGINE:
        p = "Display Engine Not Available!";
        break;
    case SCCVW_BAILOUT_UNKNOWNNOTVIEWED:
        p = "Unknown File Type - Not Viewed.";
        break;
    case SCCVW_BAILOUT_FILTERTIMEOUT:
        p = "The Filter Timed Out!";
        break;
    case SCCVW_BAILOUT_UNKNOWN:
    default:
        p = "Unknown Error!";
        break;
    }

    wsprintf ( sBuf, "Annoex Handled Bail Out!\r\n%s",p );
    MessageBox ( hMainWnd, sBuf, "Bail Out", MB_ICONINFORMATION );


    return(TRUE);
}

/*-------------------------------------------------------------------------
This function demonstrates the use of creating an icon annotation and
subsequently jumping from one to another.  This functionality is
only available on a Windows platform.
-------------------------------------------------------------------------*/


VOID DoBookMark(HWND hWnd, WORD wId)
{
    switch (wId)
    {
    case MENU_BOOKMARK_DROP:
        {
            SCCVWFINDPOSITION41	sFindPos;
            SCCVWINSERTICON41		sInsertIcon;

            /* First get the anchorposition */
            sFindPos.dwSize = sizeof(SCCVWFINDPOSITION41);
            sFindPos.dwFindOptions = SCCVW_FINDANCHORSELECTPOS;
            if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM)(PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
                return;

            /* Insert an icon at the anchor */
            sInsertIcon.dwSize = sizeof ( SCCVWINSERTICON41 );
            sInsertIcon.dwUser = (VTDWORD)(wBookMarkCount | MASK_BOOKMARK);
            sInsertIcon.dwData = 0;
            sInsertIcon.sIconPos = sFindPos.sResultPos;
            sInsertIcon.hIcon    = (VTHANDLE)LoadIcon ( hInst, "SCC_ICONBOOKMARK" );
            sInsertIcon.dwInteraction = SCCVW_EVENTSINGLECLICK | SCCVW_EVENTDOUBLECLICK |
                SCCVW_EVENTTRANSITIONINTO | SCCVW_EVENTTRANSITIONOUTOF | SCCVW_EVENTSINGLERIGHTCLICK;
            SendMessage(hViewWnd,SCCVW_ADDANNOTATION,SCCVW_INSERTICON,(LPARAM)(PSCCVWINSERTICON41)&sInsertIcon);

            wBookMarkCount++;

            InvalidateRect(hWnd,NULL,0);
        }
        break;
    case MENU_BOOKMARK_DROPBITMAP:
        {
            SCCVWFINDPOSITION41	sFindPos;
            SCCVWINSERTBITMAP42		sInsertBitmap;

            /* First get the anchorposition */
            sFindPos.dwSize = sizeof(SCCVWFINDPOSITION41);
            sFindPos.dwFindOptions = SCCVW_FINDANCHORSELECTPOS;
            if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM)(PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
                return;

            /* Insert an Bitmap at the anchor */
            sInsertBitmap.dwSize = sizeof ( SCCVWINSERTBITMAP42 );
            sInsertBitmap.dwUser = (VTDWORD)(wBookMarkCount | MASK_BOOKMARK);
            sInsertBitmap.dwData = 0;
            sInsertBitmap.sBitmapPos = sFindPos.sResultPos;
            sInsertBitmap.hBitmap = (VTHANDLE)LoadBitmap ( hInst, "SCC_BITMAPBOOKMARK" );
            sInsertBitmap.dwInteraction = SCCVW_EVENTSINGLECLICK | SCCVW_EVENTDOUBLECLICK | SCCVW_EVENTSINGLERIGHTCLICK |
                SCCVW_EVENTTRANSITIONINTO | SCCVW_EVENTTRANSITIONOUTOF;
            SendMessage(hViewWnd,SCCVW_ADDANNOTATION,SCCVW_INSERTBITMAP,(LPARAM)(PSCCVWINSERTBITMAP42)&sInsertBitmap);

            wBookMarkCount++;

            InvalidateRect(hWnd,NULL,0);
        }
        break;
    case MENU_BOOKMARK_FIRST:
        if ( SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOFIRST | SCCVW_MASK,(LPARAM) MASK_BOOKMARK) == SCCVW_FOUNDNONE)
            ;
        break;
    case MENU_BOOKMARK_LAST:
        if ( SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOLAST | SCCVW_MASK,(LPARAM) MASK_BOOKMARK) == SCCVW_FOUNDNONE)
            ;
        break;
    case MENU_BOOKMARK_NEXT:
        if ( SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTONEXT | SCCVW_MASK,(LPARAM) MASK_BOOKMARK) == SCCVW_FOUNDNONE)
            ;
        break;
    case MENU_BOOKMARK_PREV:
        if ( SendMessage(hViewWnd,SCCVW_GOTOANNOTATION,SCCVW_GOTOPREV | SCCVW_MASK,(LPARAM) MASK_BOOKMARK) == SCCVW_FOUNDNONE)
            ;
        break;
    case MENU_BOOKMARK_CLEAR:
        DoClearSelectedAnnos ( hViewWnd, MASK_BOOKMARK );
        break;
    case MENU_BOOKMARK_CLEARALL:
        SendMessage(hViewWnd,SCCVW_CLEARANNOTATIONS,SCCVW_MASK,(LPARAM) MASK_BOOKMARK);
        break;
    }
}


/*--------------------------------------------------------------------------
This function demonstrates the use numerous messages used to find and
display particular positions using SCCVW_FINDPOSITION 
and SCCVW_SETSELECTION.
------------------------------------------------------------------------*/

VOID DoPosition(HWND hWnd, WORD wId)
{
    SCCVWFINDPOSITION41     sFindPos = {0};
    SCCVWDISPLAYPOSITION41  sDisplayPos;
    SCCVWSETSELECTION41     sSetPos;
    DWORD   dwFindOptions;
    BOOL    bFromCursor;

    UNUSED(hWnd);

    bFromCursor = FALSE;
    dwFindOptions = 0;
    sDisplayPos.dwDisplayOptions = 0;

    switch (wId)
    {
    case MENU_POSITION_FIRST:
        dwFindOptions = SCCVW_FINDFIRSTPOS;
        break;

    case MENU_POSITION_LAST:
        dwFindOptions = SCCVW_FINDLASTPOS;
        break;

    case MENU_POSITION_PREV:
        bFromCursor = TRUE;
        dwFindOptions = SCCVW_FINDPREVPOS;
        break;

    case MENU_POSITION_NEXT:
        bFromCursor = TRUE;
        dwFindOptions = SCCVW_FINDNEXTPOS;
        break;

    case MENU_POSITION_STARTDISPLAY:
        dwFindOptions = SCCVW_FINDSTARTDISPLAYPOS;
        break;

    case MENU_POSITION_ENDDISPLAY:
        dwFindOptions = SCCVW_FINDENDDISPLAYPOS;
        break;

    case MENU_POSITION_STARTLINE:
        bFromCursor = TRUE;
        dwFindOptions = SCCVW_FINDSTARTLINEPOS;
        break;

    case MENU_POSITION_ENDLINE:
        bFromCursor = TRUE;
        dwFindOptions = SCCVW_FINDENDLINEPOS;
        break;

    case MENU_POSITION_PREVLINE:
        bFromCursor = TRUE;
        dwFindOptions = SCCVW_FINDPREVLINEPOS;
        break;

    case MENU_POSITION_NEXTLINE:
        bFromCursor = TRUE;
        dwFindOptions = SCCVW_FINDNEXTLINEPOS;
        break;

    case MENU_POSITION_PREVWORD:
        bFromCursor = TRUE;
        dwFindOptions = SCCVW_FINDPREVWORDPOS;
        break;

    case MENU_POSITION_NEXTWORD:
        bFromCursor = TRUE;
        dwFindOptions = SCCVW_FINDNEXTWORDPOS;
        break;

    }

    sFindPos.dwSize = sizeof ( SCCVWFINDPOSITION41 );
    if ( bFromCursor )
    {
        sFindPos.dwFindOptions = SCCVW_FINDANCHORSELECTPOS;
        if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM) (PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
            return;
        sFindPos.sSearchPos = sFindPos.sResultPos;
    }

    if ( dwFindOptions )
    {
        sFindPos.dwFindOptions = dwFindOptions;
        if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM) (PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
            return;
    }

    sDisplayPos.dwSize = sizeof ( SCCVWDISPLAYPOSITION41 );
    sDisplayPos.dwDisplayOptions = SCCVW_DISPLAYNEARMIDDLE;
    sDisplayPos.sPos = sFindPos.sResultPos;
    SendMessage(hViewWnd,SCCVW_DISPLAYPOSITION,0,(LPARAM) (PSCCVWDISPLAYPOSITION41)&sDisplayPos);

    sSetPos.dwSize = sizeof (SCCVWSETSELECTION41);
    sSetPos.sAnchorPos = sSetPos.sEndPos = sDisplayPos.sPos;
    SendMessage(hViewWnd,SCCVW_SETSELECTION,0,(LPARAM)(PSCCVWSETSELECTION41)&sSetPos);
}

/*----------------------------------------------------------------------
This function demonstrates the SCCVW_HIDETEXT option of the
SCCVW_ADDANNOTATION message.  The currently selected text is
hidden.
----------------------------------------------------------------------*/

VOID  DoHideSelection(HWND hWnd)
{
    SCCVWFINDPOSITION41  sFindPos;
    SCCVWSETSELECTION41  sSetPos;
    SCCVWCOMPPOSITIONS41 sCompPos;
    SCCVWPOS             locStart, locEnd;
    SCCVWHIDETEXT41      locHide;

    UNUSED(hWnd);

    sFindPos.dwSize = sizeof ( SCCVWFINDPOSITION41 );
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
        locHide.dwSize = sizeof ( SCCVWHIDETEXT41 );
        locHide.dwUser = MASK_HIDDENTEXT;
        locHide.sStartPos = locStart;
        locHide.sEndPos   = locEnd;
        locHide.dwData    = 0;
        SendMessage(hViewWnd,SCCVW_ADDANNOTATION,SCCVW_HIDETEXT,(LPARAM)(PSCCVWHIDETEXT41)&locHide);
    }

    sSetPos.dwSize = sizeof (SCCVWSETSELECTION41);
    sSetPos.sAnchorPos = sSetPos.sEndPos = locStart;
    SendMessage(hViewWnd,SCCVW_SETSELECTION,0,(LPARAM)(PSCCVWSETSELECTION41)&sSetPos);
}

VOID  DoClearSelectedAnnos (HWND hWnd, DWORD dwMask)
{
    SCCVWFINDPOSITION41   sFindPos;
    SCCVWPOS              locStart, locEnd;
    SCCVWFINDANNOTATION41 sFindAnno;
    SCCVWCOMPPOSITIONS41  sCompPos;

    UNUSED(hWnd);

    /*
    | Find the start and end of selection
    */
    sFindPos.dwSize = sizeof ( SCCVWFINDPOSITION41 );
    sFindPos.dwFindOptions = SCCVW_FINDANCHORSELECTPOS;
    if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM) (PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
        return;
    locStart = sFindPos.sResultPos;
    sFindPos.dwFindOptions = SCCVW_FINDENDSELECTPOS;
    if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM) (PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
        return;
    locEnd = sFindPos.sResultPos;

    /*
    | Use find annotation to locate the first annotation in the selected area
    | with the provided mask.
    */
    sFindAnno.dwSize = sizeof ( SCCVWFINDANNOTATION41 );
    sFindAnno.dwFindOptions = SCCVW_FINDFIRSTFROMPOS | SCCVW_MASK;
    sFindAnno.dwUserTest = (VTDWORD)dwMask;
    sFindAnno.sSearchPos = locStart;
    if ( SendMessage(hViewWnd,SCCVW_FINDANNOTATION,0,(LPARAM) (PSCCVWFINDANNOTATION41)&sFindAnno) != SCCVWERR_OK )
        return; /* No annotations of this type within the selected area */

    sFindAnno.dwFindOptions = SCCVW_FINDNEXT | SCCVW_MASK;
    sCompPos.pPosA = &sFindAnno.sStartPos;
    sCompPos.pPosB = &locEnd;
    while ( SendMessage ( hViewWnd, SCCVW_COMPPOSITIONS, 0, (LPARAM)(PSCCVWCOMPPOSITIONS41)(&sCompPos) ) != 1 )
    {
        SendMessage(hViewWnd,SCCVW_CLEARANNOTATIONS,SCCVW_ABSOLUTE,sFindAnno.dwUserResult);
        sFindAnno.sSearchPos = sFindAnno.sStartPos;
        if ( SendMessage(hViewWnd,SCCVW_FINDANNOTATION,0,(LPARAM) (PSCCVWFINDANNOTATION41)&sFindAnno) != SCCVWERR_OK )
            break; /* No more annotations with this mask */
    }
}

/*-----------------------------------------------------------------------
This function demonstrates the SCCVW_HILITETEXT option of the
SCCVW_ADDANNOTATION message.  The currently selected text is
hilighted in one of two color schemes based on the menu choice.
-----------------------------------------------------------------------*/

VOID DoHighlight(HWND hWnd, WORD wId)
{
    SCCVWFINDPOSITION41   sFindPos;
    SCCVWSETSELECTION41   sSetPos;
    SCCVWPOS              locStart, locEnd;
    SCCVWHILITETEXT41     locHilite;
    SCCVWCOMPPOSITIONS41  sCompPos;

    UNUSED(hWnd);

    sFindPos.dwSize = sizeof ( SCCVWFINDPOSITION41 );
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
        sMapPos.dwSize = sizeof(SCCVWMAPPOSITION41);
        sMapPos.dwMapOptions = SCCVW_MAPPOSTOACC;
        sMapPos.sPos = locStart;
        SendMessage(hViewWnd,SCCVW_MAPPOSITION,0,(LPARAM)(PSCCVWMAPPOSITION41)&sMapPos);

        locHilite.dwSize = sizeof ( SCCVWHILITETEXT41 );
        locHilite.dwUser = MASK_SEARCHHITS | sMapPos.dwACC;
        locHilite.sStartPos = locStart;
        locHilite.sEndPos = locEnd;
        locHilite.dwData = 0;
        if (wId == MENU_HIGHLIGHT_YR )
            locHilite.dwDisplay = SCCVW_BRED | SCCVW_FYELLOW;
        else if (wId == MENU_HIGHLIGHT_BY )
            locHilite.dwDisplay = SCCVW_BYELLOW | SCCVW_FBLACK;
        else if( wId == MENU_HIGHLIGHT_CUSTOM )
            locHilite.dwDisplay = SCCVW_USESTYLE | ANNOEX_STYLE_CUSTOM;

        locHilite.dwInteraction = SCCVW_EVENTSINGLECLICK | SCCVW_EVENTDOUBLECLICK | SCCVW_EVENTTRANSITIONINTO | SCCVW_EVENTTRANSITIONOUTOF | SCCVW_EVENTSINGLERIGHTCLICK;
        SendMessage(hViewWnd,SCCVW_ADDANNOTATION,SCCVW_HILITETEXT,(LPARAM)(PSCCVWHILITETEXT40)&locHilite);
    }

    sSetPos.dwSize = sizeof (SCCVWSETSELECTION41);
    sSetPos.sAnchorPos = sSetPos.sEndPos = locStart;
    SendMessage(hViewWnd,SCCVW_SETSELECTION,0,(LPARAM)(PSCCVWSETSELECTION41)&sSetPos);

}

/*----------------------------------------------------------------------
This function outputs an information string related to Text Access
comparisons to Raw Text.
----------------------------------------------------------------------*/

VOID  TAMessage ( LPSTR pStr, DWORD dwCmpOK )
{
    char  sBuf[100];
    wsprintf ( sBuf, "\r\n%ld Bytes compared OK\r\n", dwCmpOK );
    lstrcat ( sBuf, pStr );
    OutputInfo ( sBuf );
}

VOID InitTAVars (void)
{
    gErrTA     = SCCERR_OK;
    gTAOffset  = 0;
    gTABufSize = 0;
    gTATotal   = 0;
    gCmpTotal  = 0;
    ghDoc      = VTHDOC_INVALID;
    ghText     = VTHDOC_INVALID;
}

VOID	OutputInfo ( LPSTR pStr )
{
#ifdef _DEBUG
    OutputDebugString ( pStr );
#else
    UNUSED(pStr);
#endif
}


HINSTANCE SCCLoadFIDLL(LPSTR lpViewerPath)
{
    HINSTANCE  locRet;
    LPSTR      locDirPtr;
    char       locPath[144];
    char	   locDir[256];

    /*
    |
    |  The code below loads the SCCFI DLL from lpViewerPath
    |
    */


    /*
    |	Make copy of load path
    */

    lstrcpy(locDir,lpViewerPath);

    /*
    |	Strip off trailing "\"
    */

    locDirPtr = locDir;
    while (*locDirPtr != 0x00)
        locDirPtr++;
    locDirPtr--;
    if (*locDirPtr == '\\')
        *locDirPtr = 0x00;

    /*
    |	Load the library
    */

    lstrcpy(locPath,locDir);
    lstrcat(locPath,"\\");

    lstrcat(locPath,"SCCFI.DLL");

    locRet = LoadLibraryEx(locPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);


    return(locRet);
}

LONG	DoSearch ( BOOL bNotify )
{
    LONG   locRet;

    locRet = (LONG)SendMessage( hViewWnd, SCCVW_SEARCHDIALOG, 0, (LPARAM)(PSCCVWSEARCHINFO40)&gSearchInfo );
    if( bNotify )
        DoSearchMessage ( locRet );
    return locRet;
}

LONG	DoSearchNext ( BOOL bNotify )
{
    LONG   locRet;

    locRet = (LONG)SendMessage( hViewWnd, SCCVW_SEARCHNEXT, SCCVW_SEARCHSAME, 0 );
    if( bNotify )
        DoSearchMessage ( locRet );
    return locRet;
}

VOID	DoSearchMessage ( LONG locRet )
{
    if ( locRet == 1 )
        MessageBox ( hMainWnd, "None found!", "Search", MB_ICONINFORMATION );
}

VOID   DoReplace(void)
{
    hDlgReplace = CreateDialog( hInst, MAKEINTRESOURCE(IDD_REPLACEDIALOG), hMainWnd, (DLGPROC)ReplaceDialog );
    if( hDlgReplace )
    {
        ShowWindow( hDlgReplace, SW_SHOW );
    }
}

/*
|  GetSearchHitPos()
|  Return the start and end position of text found in the last search.
*/

BOOL   GetSearchHitPos( PSCCVWPOS pStartPos, PSCCVWPOS pEndPos )
{
    /*
    |  Find the search highlight annotation.
    */

    SCCVWFINDANNOTATION41   sFind;
    BOOL                    result = FALSE;

    ZeroMemory( &sFind, sizeof( SCCVWFINDANNOTATION41 ) );
    sFind.dwSize        = sizeof( SCCVWFINDANNOTATION41 );
    sFind.dwFindOptions = SCCVW_FINDFIRST | SCCVW_MASK;
    sFind.dwUserTest    = SCCVW_USERVWSEARCHTAG;

    if( SendMessage( hViewWnd, SCCVW_FINDANNOTATION, 0, (LPARAM)(PSCCVWFINDANNOTATION41)&sFind ) == SCCVWERR_OK )
    {
        if( pStartPos )
        {
            *pStartPos = sFind.sStartPos;
        }

        if( pEndPos )
        {
            *pEndPos   = sFind.sEndPos;
        }

        result = TRUE;
    }

    return result;
}

/*------------------------------------------------------------------------
DoHighlightSearchHits
This function uses Get Raw Text to scan the currently viewed document,
- searches the text for the string passed;
- uses SCCVW_MAPPOSITION to map actual character counts to SCCVWPOS's;
- highlights all matches using the mapped positions
- places the cursor at the first hit


NOTE: In the interest of simplicity.
- The string used is the one provided by a previous "Edit"-"Search"
- This function does not take into account matches which overlap 
raw text buffers.  
- Also a more powerful implementation of highlighting all search 
hits would use some form of background processing to return 
control to the user after the first hits are highlighted.
-------------------------------------------------------------------------*/

DWORD	DoHighlightSearchHits ( LPSTR pStr )
{
    SCCVWGETRAWTEXT50	    sRaw = {0};
    SCCVWDISPLAYPOSITION41	sDisplayPos;
    SCCVWSETSELECTION41		sSetPos;
    SCCVWHILITETEXT41       sHilite; 
    SCCVWMAPPOSITION41	    sMap = {0};
    DWORD		            dwHitCount;
    DWORD	                dwTextCount, dwStartTextCount;
    VTDWORD	                dwMatchLen;
    LPSTR	                locText, locStartText;

    /*
    | First clear all annotations
    */
    SendMessage ( hViewWnd, SCCVW_CLEARANNOTATIONS, 0, 0L );

    sRaw.dwSize    = sizeof ( SCCVWGETRAWTEXT50 );
    sRaw.dwOptions = 0;                         
    sRaw.dwACC     = 0;
    sHilite.dwSize = sizeof ( SCCVWHILITETEXT41 );
    sMap.dwSize    = sizeof ( SCCVWMAPPOSITION41 );
    sMap.dwMapOptions = SCCVW_MAPACCTOPOS;
    sSetPos.dwSize = sizeof (SCCVWSETSELECTION41);
    sDisplayPos.dwSize = sizeof ( SCCVWDISPLAYPOSITION41 );

    dwHitCount = 0;

    while ( SendMessage(hViewWnd,SCCVW_GETRAWTEXT,0,(LPARAM)(PSCCVWGETRAWTEXT50)&sRaw) == SCCVWERR_OK )
    {
        locStartText = locText = GlobalLock ( sRaw.hText );
        dwStartTextCount = dwTextCount = sRaw.dwCharCount;

        dwMatchLen = 0;
        while ((locText = DoMatchIt(sRaw.dwCharSet, locText, pStr, &dwTextCount, &dwMatchLen)) != NULL)
        {
            /* Map the resulting positions */
            sMap.dwACC = (VTDWORD)(sRaw.dwTextOffset + (dwStartTextCount - dwTextCount));
            SendMessage ( hViewWnd, SCCVW_MAPPOSITION, 0, (LPARAM)(PSCCVWMAPPOSITION41)&sMap );
            sHilite.sStartPos = sMap.sPos;
            sHilite.dwUser = (VTDWORD)(MASK_SEARCHHITS | sMap.dwACC);

            sMap.dwACC += dwMatchLen;
            SendMessage ( hViewWnd, SCCVW_MAPPOSITION, 0, (LPARAM)(PSCCVWMAPPOSITION41)&sMap );
            sHilite.sEndPos = sMap.sPos;

            sHilite.dwDisplay = (VTDWORD)(SCCVW_USESTYLE | ANNOEX_STYLE_SEARCHHIT);
            sHilite.dwInteraction = SCCVW_EVENTSINGLECLICK | SCCVW_EVENTDOUBLECLICK | SCCVW_EVENTSINGLERIGHTCLICK;
            sHilite.dwData = 0;
            SendMessage(hViewWnd,SCCVW_ADDANNOTATION,SCCVW_HILITETEXT,(LPARAM)(PSCCVWHILITETEXT40)&sHilite);
            if ( dwHitCount == 0 )
            {
                sDisplayPos.sPos = sHilite.sStartPos;
                sDisplayPos.dwDisplayOptions = 0;

                SendMessage(hViewWnd,SCCVW_DISPLAYPOSITION,0,(LPARAM) (PSCCVWDISPLAYPOSITION41)&sDisplayPos);

                sSetPos.sAnchorPos = sSetPos.sEndPos = sHilite.sStartPos;
                SendMessage(hViewWnd,SCCVW_SETSELECTION,0,(LPARAM)(PSCCVWSETSELECTION41)&sSetPos);
            }
            dwHitCount++;
            locText++;
            dwTextCount--;
        }

        GlobalUnlock ( sRaw.hText );

        GlobalFree ( sRaw.hText );
        sRaw.dwACC += sRaw.dwCharCount;
    }

    return ( dwHitCount );
}

VOID	DoSearchNextHighlight (void)
{
    SCCVWSETSELECTION41    sSetPos;
    SCCVWDISPLAYPOSITION41 sDisplayPos;
    SCCVWFINDPOSITION41    sFindPos, sSavePos;
    SCCVWFINDANNOTATION41  sFindAnno;
    SCCVWCOMPPOSITIONS41   sCompPos;
    BOOL  bDone;

    bDone = FALSE;

    sFindPos.dwSize = sizeof ( SCCVWFINDPOSITION41 );
    sFindPos.dwFindOptions = SCCVW_FINDANCHORSELECTPOS;
    if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM) (PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
    {
        DoSearchMessage ( 1 );
        return;
    }
    sSavePos = sFindPos;

    while ( !bDone )
    {
        sFindAnno.dwSize = sizeof ( SCCVWFINDANNOTATION41 );
        sFindAnno.dwFindOptions = SCCVW_FINDNEXT | SCCVW_MASK;
        sFindAnno.dwUserTest = MASK_SEARCHHITS;
        sFindAnno.sSearchPos = sFindPos.sResultPos;
        if ( SendMessage(hViewWnd,SCCVW_FINDANNOTATION,0,(LPARAM) (PSCCVWFINDANNOTATION41)&sFindAnno) != SCCVWERR_OK )
        {
            /* No more matches */
            DoSearchMessage ( 1 );
            return;
        }

        sDisplayPos.dwSize = sizeof ( SCCVWDISPLAYPOSITION41 );
        sDisplayPos.sPos = sFindAnno.sStartPos;
        sDisplayPos.dwDisplayOptions = 0;
        SendMessage(hViewWnd,SCCVW_DISPLAYPOSITION,0,(LPARAM) (PSCCVWDISPLAYPOSITION41)&sDisplayPos);

        sSetPos.dwSize = sizeof (SCCVWSETSELECTION41);
        sSetPos.sAnchorPos = sSetPos.sEndPos = sDisplayPos.sPos;
        SendMessage(hViewWnd,SCCVW_SETSELECTION,0,(LPARAM)(PSCCVWSETSELECTION41)&sSetPos);

        /*
        | Now get the current anchor position back to verify that the cursor
        | has actually advanced.  This is necessary since some text elements
        | can't be cursored to (table, spreadsheets, vector) so we end up
        | stuck in one spot, always jumping to text which forward but puts 
        | the cursor back where it was.  If the cursor has not moved then 
        | move the sFindPos to the sFindAnnoPos and search again.
        */
        if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM) (PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
            return;
        sCompPos.pPosA = &sFindPos.sResultPos;
        sCompPos.pPosB = &sSavePos.sResultPos;
        if ( SendMessage ( hViewWnd, SCCVW_COMPPOSITIONS, 0, (LPARAM)(PSCCVWCOMPPOSITIONS41)(&sCompPos) ) != 0 )
            bDone = TRUE;
        else
            sFindPos.sResultPos = sFindAnno.sStartPos;

    }

}

VOID	DoSearchPreviousHighlight (void)
{
    SCCVWSETSELECTION41    sSetPos;
    SCCVWDISPLAYPOSITION41 sDisplayPos;
    SCCVWFINDPOSITION41    sFindPos, sSavePos;
    SCCVWFINDANNOTATION41  sFindAnno;
    SCCVWCOMPPOSITIONS41   sCompPos;
    BOOL		bDone;

    sFindPos.dwSize = sizeof ( SCCVWFINDPOSITION41 );
    sFindPos.dwFindOptions = SCCVW_FINDANCHORSELECTPOS;
    if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM) (PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
    {
        DoSearchMessage ( 1 );
        return;
    }

    sSavePos = sFindPos;
    bDone = FALSE;

    while ( !bDone )
    {
        sFindAnno.dwSize = sizeof ( SCCVWFINDANNOTATION41 );
        sFindAnno.dwFindOptions = SCCVW_FINDPREV | SCCVW_MASK;
        sFindAnno.dwUserTest = MASK_SEARCHHITS;
        sFindAnno.sSearchPos = sFindPos.sResultPos;
        if ( SendMessage(hViewWnd,SCCVW_FINDANNOTATION,0,(LPARAM) (PSCCVWFINDANNOTATION41)&sFindAnno) != SCCVWERR_OK )
        {
            /* No more matches */
            DoSearchMessage ( 1 );
            return;
        }

        sDisplayPos.dwSize = sizeof ( SCCVWDISPLAYPOSITION41 );
        sDisplayPos.sPos = sFindAnno.sStartPos;
        sDisplayPos.dwDisplayOptions = 0;
        SendMessage(hViewWnd,SCCVW_DISPLAYPOSITION,0,(LPARAM) (PSCCVWDISPLAYPOSITION41)&sDisplayPos);

        sSetPos.dwSize = sizeof (SCCVWSETSELECTION41);
        sSetPos.sAnchorPos = sSetPos.sEndPos = sDisplayPos.sPos;
        SendMessage(hViewWnd,SCCVW_SETSELECTION,0,(LPARAM)(PSCCVWSETSELECTION41)&sSetPos);
        /*
        | Now get the current anchor position back to verify that the cursor
        | has actually advanced.  This is necessary since some text elements
        | can't be cursored to (table, spreadsheets, vector, hidden) so we end 
        | up stuck in one spot, always jumping to text which is previous but
        | puts the cursor back where it was.  If the cursor has not moved then 
        | move the sFindPos to the sFindAnnoPos and search again.
        */
        if ( SendMessage(hViewWnd,SCCVW_FINDPOSITION,0,(LPARAM) (PSCCVWFINDPOSITION41)&sFindPos) != SCCVWERR_OK )
            return;
        sCompPos.pPosA = &sFindPos.sResultPos;
        sCompPos.pPosB = &sSavePos.sResultPos;
        if ( SendMessage ( hViewWnd, SCCVW_COMPPOSITIONS, 0, (LPARAM)(PSCCVWCOMPPOSITIONS41)(&sCompPos) ) != 0 )
            bDone = TRUE;
        else
            sFindPos.sResultPos = sFindAnno.sStartPos;
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
WORD   nextChar(DWORD dwCharSet, LPSTR *p)
{
    LPSTR   pc = *p;
    WORD    c;

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
Simple case IN-sensitive matching algorithm

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
LPSTR	DoMatchIt (DWORD dwCharSet, LPSTR pBuffer, LPSTR pText ,LPDWORD pCharCount, LPDWORD pMatchLen)
{
    LPSTR	p1;
    LPSTR	p2;
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

VOID	DoSearchHighlight (void)
{
    FARPROC	pSearchDlg;
    pSearchDlg = (FARPROC)MakeProcInstance ( SearchDialog, hInst );

    if ( DialogBox ( hInst, MAKEINTRESOURCE(IDD_SEARCHDIALOG), hMainWnd, (DLGPROC)pSearchDlg ) )
    {
        if ( gSearchString[0] )
        {
            if ( DoHighlightSearchHits(gSearchString) == 0 )
            {
                MessageBox ( hMainWnd, "Match not found!", "Search", MB_ICONINFORMATION );
            }
        }
    }

    FreeProcInstance(pSearchDlg);

}


WIN_ENTRYSC BOOL WIN_ENTRYMOD SearchDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT  locRect;
    int   locX;
    int   locY;

    UNUSED(lParam);

    switch (message)
    {
    case WM_INITDIALOG:

        /*
        |	Center dialog
        */

        GetWindowRect(hDlg,&locRect);
        locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
        locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
        SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

        gSearchString[0] = '\0';
        SetDlgItemText(hDlg,IDC_EDITSEARCHTEXT,gSearchString);
        return (TRUE);

    case WM_COMMAND:

        switch(wParam)
        {
        case IDOK:

            GetDlgItemText(hDlg,IDC_EDITSEARCHTEXT,gSearchString,80);
            if (gSearchString[0])
                EndDialog(hDlg,TRUE);
            else
                EndDialog(hDlg,FALSE);
            return (TRUE);

        case IDCANCEL:

            EndDialog(hDlg, FALSE);
            return (TRUE);

        default:
            break;
        }

        break;

    default:
        break;
    }

    return(FALSE);
}



DWORD getComboData( HWND hList )
{
    long index = (long)SendMessage(hList,CB_GETCURSEL,0,0);
    if( index == CB_ERR )
        index = 0;

    return (DWORD) SendMessage(hList, CB_GETITEMDATA, (WPARAM) index, 0 );
}

void addItemToCombo(HWND hList, LPARAM data, LPCSTR label)
{
    long index = (long)SendMessage( hList, CB_ADDSTRING,0, (LPARAM)label );
    if( index >= 0 )
        SendMessage(hList,CB_SETITEMDATA,(WPARAM)index, data);
}

#define UNSPECIFIED_COLOR 0x01010101
void initColorList(HWND hList)
{
    addItemToCombo(hList, (LPARAM) UNSPECIFIED_COLOR, "Unchanged" );
    addItemToCombo(hList, (LPARAM) SCCVWRGB(0xff,0,0), "Red");
    addItemToCombo(hList, (LPARAM) SCCVWRGB(0xff,0xFF,0), "Yellow");
    addItemToCombo(hList, (LPARAM) SCCVWRGB(0,0xc0,0), "Green");
    addItemToCombo(hList, (LPARAM) SCCVWRGB(0,0,0xc0), "Blue");
    addItemToCombo(hList, (LPARAM) SCCVWRGB(0xc0,0,0xc0), "Purple");
    addItemToCombo(hList, (LPARAM) SCCVWRGB(0xff,0xFF,0xFF), "White");
    addItemToCombo(hList, (LPARAM) SCCVWRGB(0xd0,0xd0,0xd0), "Gray");
    addItemToCombo(hList, (LPARAM) SCCVWRGB(0,0,0), "Black");
}


void selectComboItemByData(HWND hCombo, DWORD dwData )
{
    long index = 0;
    DWORD itemData;
    do
    {
        itemData = (DWORD) SendMessage(hCombo, CB_GETITEMDATA, (WPARAM)index,0 );

        if( dwData == itemData )
        {
            SendMessage( hCombo, CB_SETCURSEL, index, 0 );
            return;
        }
        else
            index++;

    }  while( itemData != (DWORD)CB_ERR );
}


typedef struct ANNOEX_CHARATTR_CONTROLS
{
    DWORD idOn;
    DWORD idOff;
    DWORD idUnchanged;
    WORD  wFlag;
} ANNOEX_CHARATTR_CONTROLS;

ANNOEX_CHARATTR_CONTROLS gAttrRadioButtons[] = {
    { IDC_BOLD_ON, IDC_BOLD_OFF, IDC_BOLD_UNCHANGED, SCCVW_CHARATTR_BOLD },
    { IDC_ITALIC_ON, IDC_ITALIC_OFF, IDC_ITALIC_UNCHANGED, SCCVW_CHARATTR_ITALIC },
    { IDC_STRIKEOUT_ON, IDC_STRIKEOUT_OFF, IDC_STRIKEOUT_UNCHANGED, SCCVW_CHARATTR_STRIKEOUT },
    { IDC_UNDERLINE_ON, IDC_UNDERLINE_OFF, IDC_UNDERLINE_UNCHANGED, SCCVW_CHARATTR_UNDERLINE },
    { IDC_DUNDERLINE_ON, IDC_DUNDERLINE_OFF, IDC_DUNDERLINE_UNCHANGED, SCCVW_CHARATTR_DUNDERLINE },
    { IDC_WUNDERLINE_ON, IDC_WUNDERLINE_OFF, IDC_WUNDERLINE_UNCHANGED, SCCVW_CHARATTR_WORDUNDERLINE },
    { IDC_DASHUNDERLINE_ON, IDC_DASHUNDERLINE_OFF, IDC_DASHUNDERLINE_UNCHANGED, SCCVW_CHARATTR_DASHUNDERLINE },
    { IDC_DOTUNDERLINE_ON, IDC_DOTUNDERLINE_OFF, IDC_DOTUNDERLINE_UNCHANGED, SCCVW_CHARATTR_DOTUNDERLINE }
};

#define NUM_CHAR_ATTR 8

WIN_ENTRYSC BOOL WIN_ENTRYMOD CustomHighlightDlg( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    PSCCVWHILITESTYLE81 pStyle;
    int i=0;

    switch( message )
    {
    case WM_INITDIALOG:

        /* Save the pointer to the highlight structure */
        SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam );

        /* Initialize combo boxes. */
        initColorList( GetDlgItem(hDlg,IDC_FGCOLOR) );
        initColorList( GetDlgItem(hDlg,IDC_BGCOLOR) );
        initColorList( GetDlgItem(hDlg,IDC_ULCOLOR) );

        pStyle = (PSCCVWHILITESTYLE81) lParam;

        if( pStyle->dwOptions & SCCVW_USEFOREGROUND )
            selectComboItemByData( GetDlgItem(hDlg,IDC_FGCOLOR), (DWORD)pStyle->sForeground);
        else
            selectComboItemByData( GetDlgItem(hDlg,IDC_FGCOLOR), UNSPECIFIED_COLOR);

        if( pStyle->dwOptions & SCCVW_USEBACKGROUND )
            selectComboItemByData( GetDlgItem(hDlg,IDC_BGCOLOR), (DWORD)pStyle->sBackground);
        else
            selectComboItemByData( GetDlgItem(hDlg,IDC_BGCOLOR), UNSPECIFIED_COLOR);

        if( pStyle->dwOptions & SCCVW_USEUNDERLINECOLOR )
            selectComboItemByData( GetDlgItem(hDlg,IDC_ULCOLOR), (DWORD)pStyle->sUnderline);
        else
            selectComboItemByData( GetDlgItem(hDlg,IDC_ULCOLOR), UNSPECIFIED_COLOR);


        /* Initialize checkboxes for attributes. */
        if( pStyle->dwOptions & SCCVW_USECHARATTR )
        {
            DWORD dwId;
            for( i=0; i<NUM_CHAR_ATTR; i++ )
            {
                if( pStyle->wCharAttrMask & gAttrRadioButtons[i].wFlag )
                    dwId = (pStyle->wCharAttr & gAttrRadioButtons[i].wFlag) ? gAttrRadioButtons[i].idOn : gAttrRadioButtons[i].idOff ;
                else
                    dwId = gAttrRadioButtons[i].idUnchanged;

                CheckDlgButton( hDlg, dwId,TRUE );
            }
        }
        else
        {
            for( i=0; i<NUM_CHAR_ATTR; i++ )
                CheckDlgButton( hDlg, gAttrRadioButtons[i].idUnchanged,TRUE );
        }

        CheckDlgButton(hDlg, IDC_USETHINUNDERLINE, (pStyle->dwOptions & SCCVW_USETHINUNDERLINE) ? TRUE : FALSE );

        return TRUE;

    case WM_COMMAND:

        switch( LOWORD( wParam ) )
        {
        case IDOK:
            {
                DWORD dwColor;
                WORD  wAttr = 0;
                WORD  wAttrMask = 0;

                pStyle = (PSCCVWHILITESTYLE81) GetWindowLongPtr(hDlg,GWLP_USERDATA);

                dwColor = getComboData( GetDlgItem(hDlg,IDC_FGCOLOR) );
                if( dwColor == UNSPECIFIED_COLOR ) 
                    pStyle->dwOptions &= ~SCCVW_USEFOREGROUND;
                else
                {
                    pStyle->dwOptions |= SCCVW_USEFOREGROUND;
                    pStyle->sForeground = dwColor;
                }

                dwColor = getComboData( GetDlgItem(hDlg,IDC_BGCOLOR) );
                if( dwColor == UNSPECIFIED_COLOR ) 
                    pStyle->dwOptions &= ~SCCVW_USEBACKGROUND;
                else
                {
                    pStyle->dwOptions |= SCCVW_USEBACKGROUND;
                    pStyle->sBackground = dwColor;
                }

                dwColor = getComboData( GetDlgItem(hDlg,IDC_ULCOLOR) );
                if( dwColor == UNSPECIFIED_COLOR ) 
                    pStyle->dwOptions &= ~SCCVW_USEUNDERLINECOLOR;
                else
                {
                    pStyle->dwOptions |= SCCVW_USEUNDERLINECOLOR;
                    pStyle->sUnderline = dwColor;
                }

                if( IsDlgButtonChecked(hDlg,IDC_USETHINUNDERLINE) )
                    pStyle->dwOptions |= SCCVW_USETHINUNDERLINE;
                else
                    pStyle->dwOptions &= ~SCCVW_USETHINUNDERLINE;

                for( i=0; i<NUM_CHAR_ATTR; i++ )
                {
                    if( IsDlgButtonChecked(hDlg,gAttrRadioButtons[i].idOn) )
                    {
                        wAttr |= gAttrRadioButtons[i].wFlag;
                        wAttrMask |= gAttrRadioButtons[i].wFlag;
                    }
                    else if( IsDlgButtonChecked(hDlg,gAttrRadioButtons[i].idOff) )
                    {
                        wAttrMask |= gAttrRadioButtons[i].wFlag;
                    }
                }

                pStyle->wCharAttr = wAttr;
                pStyle->wCharAttrMask = wAttrMask;

                if( wAttrMask )
                    pStyle->dwOptions |= SCCVW_USECHARATTR;
                else
                    pStyle->dwOptions &= ~SCCVW_USECHARATTR;

                EndDialog(hDlg, 1);
            }
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, 0);
            return TRUE;
        }
    }
    return FALSE;
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
            lstrcpy(buffer, "Annoex Sample Application\n");
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

#define MAX_SEARCH_STRING   80
#define MAX_REPLACE_STRING  80

WIN_ENTRYSC BOOL WIN_ENTRYMOD ReplaceDialog( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    static BOOL    bMatchCase = FALSE;
    static BOOL    bGotMatch  = FALSE;
    TCHAR          szSearch[ MAX_SEARCH_STRING ], szReplace[ MAX_REPLACE_STRING ];
    RECT           rcDlg, rcViewWnd;
    LONG           lDlgX, lDlgY;

    UNUSED(lParam);

    switch( message )
    {
    case WM_INITDIALOG:

        /* Center dialog */
        GetWindowRect( hDlg, &rcDlg );
        GetClientRect( hViewWnd, &rcViewWnd );
        lDlgX = ( ( rcViewWnd.right  - rcViewWnd.left ) - ( rcDlg.right  - rcDlg.left ) ) / 2;
        lDlgY = ( ( rcViewWnd.bottom - rcViewWnd.top  ) - ( rcDlg.bottom - rcDlg.top  ) ) / 2;
        SetWindowPos( hDlg, NULL, lDlgX, lDlgY, 0, 0, SWP_NOSIZE | SWP_NOZORDER );

        /* Limit edit fields. */
        SendDlgItemMessage( hDlg, IDC_REPLACESEARCHTEXT, EM_LIMITTEXT, MAX_SEARCH_STRING,  0 );
        SendDlgItemMessage( hDlg, IDC_REPLACETEXT,       EM_LIMITTEXT, MAX_REPLACE_STRING, 0 );

        /* Initialize "match case" checkbox. */
        CheckDlgButton( hDlg, IDC_MATCHCASE, 
            ( bMatchCase ? BST_CHECKED : BST_UNCHECKED ) );

        bGotMatch = FALSE;
        return TRUE;

    case WM_COMMAND:

        switch( LOWORD( wParam ) )
        {
        case IDC_MATCHCASE:

            bMatchCase = IsDlgButtonChecked( hDlg, IDC_MATCHCASE );
            break;

        case IDC_REPLACE:

            if( bGotMatch )
            {
                SCCVWPOS   sStartPos, sEndPos;

                GetDlgItemText( hDlg, IDC_REPLACETEXT, szReplace, MAX_REPLACE_STRING );

                if( GetSearchHitPos( &sStartPos, &sEndPos ) )
                {
                    ReplaceOne( szReplace, &sStartPos, &sEndPos );
                }
            }

            /* Drop through to find next... */

        case IDC_FINDNEXT:

            if( bGotMatch )
            {
                if( DoSearchNext( TRUE ) != 0 )
                {
                    bGotMatch = FALSE;
                }
            }
            else
            {
                LONG   lSearchResult;

                GetDlgItemText( hDlg, IDC_REPLACESEARCHTEXT, szSearch, MAX_SEARCH_STRING );

                /* Set up the search info struct. */
                gSearchInfo.siTextLen = (VTWORD)lstrlen( szSearch );
                if( gSearchInfo.siTextLen > 80 )
                {
                    gSearchInfo.siTextLen = 80;
                }

                CopyMemory( gSearchInfo.siText, szSearch, gSearchInfo.siTextLen * sizeof(TCHAR) );

                gSearchInfo.siType      = ( bMatchCase ? SCCVW_SEARCHCASE : SCCVW_SEARCHNOCASE );
                gSearchInfo.siFrom      = SCCVW_SEARCHCURRENT;
                gSearchInfo.siDirection = SCCVW_SEARCHFORWARD;

                lSearchResult = (LONG)SendMessage( hViewWnd, SCCVW_SEARCH, 0, (LPARAM)&gSearchInfo );
                DoSearchMessage( lSearchResult );

                bGotMatch = ( lSearchResult == 0 );
            }
            break;

        case IDC_REPLACEALL:

            GetDlgItemText( hDlg, IDC_REPLACESEARCHTEXT, szSearch,  MAX_SEARCH_STRING );
            GetDlgItemText( hDlg, IDC_REPLACETEXT,       szReplace, MAX_REPLACE_STRING );
            ReplaceAll( szSearch, szReplace, bMatchCase );
            /* Fall through to close the dialog... */

        case IDCANCEL:

            if( hDlgReplace )
            {
                DestroyWindow( hDlg );
                hDlgReplace = NULL;
            }
            return TRUE;
        }
    }
    return FALSE;
}

/*
|  ReplaceAll()
|  Replace all occurrences of szSearch with szReplace.
*/

VOID   ReplaceAll( LPTSTR szSearch, LPTSTR szReplace, BOOL bMatchCase )
{
    SCCVWPOS   sStartPos, sEndPos;

    gSearchInfo.siTextLen = (VTWORD)lstrlen( szSearch );
    if( gSearchInfo.siTextLen > 80 )
    {
        gSearchInfo.siTextLen = 80;
    }
    CopyMemory( gSearchInfo.siText, szSearch, gSearchInfo.siTextLen * sizeof(TCHAR) );

    gSearchInfo.siType      = ( bMatchCase ? SCCVW_SEARCHCASE : SCCVW_SEARCHNOCASE );
    gSearchInfo.siFrom      = SCCVW_SEARCHTOP;
    gSearchInfo.siDirection = SCCVW_SEARCHFORWARD;

    if( SendMessage( hViewWnd, SCCVW_SEARCH, 0, (LPARAM)&gSearchInfo ) == 0 )
    {
        do
        {
            /*
            |  PeekMessage loop (so the user can cancel the replace).
            */
            MSG   msg;

            while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
            {
                if( !IsDialogMessage( hDlgReplace, &msg ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
            }

            if( !hDlgReplace )
            {
                /* Cancelled by user. */
                break;
            }

            if( GetSearchHitPos( &sStartPos, &sEndPos ) )
            {
                ReplaceOne( szReplace, &sStartPos, &sEndPos );
            }
        } while( DoSearchNext( FALSE ) == 0 );
    }
}

/*
|  ReplaceOne()
|  Replace the text between sStartPos and sEndPos with szReplace.
*/

VOID   ReplaceOne( LPTSTR szReplace, PSCCVWPOS pStartPos, PSCCVWPOS pEndPos )
{
    SCCVWCOMPPOSITIONS41   sCompPos;

    if( !pStartPos || !pEndPos )
    {
        return;
    }

    /* Hide the existing text. */
    sCompPos.pPosA = pStartPos;
    sCompPos.pPosB = pEndPos;
    if( SendMessage( hViewWnd, SCCVW_COMPPOSITIONS, 0, (LPARAM)&sCompPos ) != 0 )
    {
        SCCVWHIDETEXT80   sHide;

        ZeroMemory( &sHide, sizeof( SCCVWHIDETEXT80 ) );
        sHide.dwSize    = sizeof( SCCVWHIDETEXT80 );
        sHide.sStartPos = *pStartPos;
        sHide.sEndPos   = *pEndPos;
        SendMessage( hViewWnd, SCCVW_ADDANNOTATION, SCCVW_HIDETEXT, (LPARAM)&sHide );
    }

    /* Add the new text. */
    if( szReplace && *szReplace )
    {
        SCCVWINSERTTEXT80   sInsert;
        LONG                lTextLen = lstrlen( szReplace );
        HANDLE              hText    = GlobalAlloc( GHND, lTextLen * sizeof( WORD ) );

        if( hText )
        {
            LPWORD   pwText = GlobalLock( hText );

            if( pwText )
            {
                LONG   lTextIndex;

                for( lTextIndex = 0; lTextIndex < lTextLen; ++lTextIndex )
                {
                    pwText[ lTextIndex ] = (WORD)szReplace[ lTextIndex ];
                }

                ZeroMemory( &sInsert, sizeof( SCCVWINSERTTEXT80 ) );
                sInsert.dwSize   = sizeof( SCCVWINSERTTEXT80 );
                sInsert.sTextPos = *pStartPos;
                sInsert.pText    = pwText;
                sInsert.dwLength = lTextLen;

                SendMessage( hViewWnd, SCCVW_ADDANNOTATION, (WPARAM)SCCVW_INSERTTEXT, (LPARAM)&sInsert );

                GlobalUnlock( hText );
            }

            GlobalFree( hText );
        }
    }
}



WIN_ENTRYSC BOOL WIN_ENTRYMOD AnnoRightSingleClick(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT  locRect;
    int   locX;
    int   locY;

    UNUSED(lParam);

    switch (message)
    {
    case WM_INITDIALOG:

        /*
        |	Center dialog
        */

        GetWindowRect(hDlg,&locRect);
        locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
        locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
        SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

        SetDlgItemText(hDlg,IDC_STATICRIGHTSINGLECLICK,"Annotation Right Single Click Event");
        return (TRUE);

    case WM_COMMAND:

        switch(wParam)
        {
        case IDOK:
             EndDialog(hDlg,TRUE);
            return (TRUE);

        case IDCANCEL:
            EndDialog(hDlg, FALSE);
            return (TRUE);

        default:
            break;
        }

        break;

    default:
        break;
    }

    return(FALSE);
}

WIN_ENTRYSC BOOL WIN_ENTRYMOD AnnoDoubleClick(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT  locRect;
    int   locX;
    int   locY;

    UNUSED(lParam);

    switch (message)
    {
    case WM_INITDIALOG:

        /*
        |	Center dialog
        */

        GetWindowRect(hDlg,&locRect);
        locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
        locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
        SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

        SetDlgItemText(hDlg,IDC_STATICDOUBLECLICK,"Annotation Double Click Event");
        return (TRUE);

    case WM_COMMAND:

        switch(wParam)
        {
        case IDOK:
             EndDialog(hDlg,TRUE);
            return (TRUE);

        case IDCANCEL:
            EndDialog(hDlg, FALSE);
            return (TRUE);

        default:
            break;
        }

        break;

    default:
        break;
    }

    return(FALSE);
}

WIN_ENTRYSC BOOL WIN_ENTRYMOD AnnoLeftSingleClick(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT  locRect;
    int   locX;
    int   locY;

    UNUSED(lParam);

    switch (message)
    {
    case WM_INITDIALOG:

        /*
        |	Center dialog
        */

        GetWindowRect(hDlg,&locRect);
        locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
        locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
        SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

        SetDlgItemText(hDlg,IDC_STATICLEFTSINGLECLICK,"Annotation Left Single Click Event");
        return (TRUE);

    case WM_COMMAND:

        switch(wParam)
        {
        case IDOK:
            EndDialog(hDlg,TRUE);
            return (TRUE);

        case IDCANCEL:
            EndDialog(hDlg, FALSE);
            return (TRUE);

        default:
            break;
        }

        break;

    default:
        break;
    }

    return(FALSE);
}
