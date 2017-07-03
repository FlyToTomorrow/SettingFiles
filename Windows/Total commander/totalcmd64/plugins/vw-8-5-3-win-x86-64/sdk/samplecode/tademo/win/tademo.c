/*
|  Outside In Viewer Technology
|  Sample Application
|  
|
|  TADEMO
|  Text Access sample application
|
|  Purpose:
|  Show how to use the Text Access Specification
|
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

#define WIN_ENTRYSC __declspec(dllexport)
#define WIN_ENTRYMOD __stdcall
#define WINMAIN_ENTRYMOD APIENTRY

#define  NULLHANDLE  (HANDLE)NULL
#define  MenuOn(x)   EnableMenuItem(hMenu,x,MF_ENABLED)
#define  MenuOff(x)  EnableMenuItem(hMenu,x,MF_GRAYED)

/*
|
|  Includes
|  Includes
|  Includes
|
*/

#include <windows.h>             // required for all windows programs 
#include <commdlg.h>             // required for common dialogs
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <ctype.h>

#include "scctype.h"// SCC type definitions
#include "sccta.h"  // SCC include file required for use of text access DLL
#include "sccio.h"  // SCC include file required for I/O definitions

#include "tademo.h"              // Text access header
#include "tademo.pro"            // Function prototypes
#include "verbuild.h"
/*
|
|  Command line arguments
|  Command line arguments
|  Command line arguments
|
*/

#define ARGC	__argc
#define ARGV(x)	(__argv[x])

/*
|
|  Globals
|  Globals
|  Globals
|
*/

#define      PATHSIZE        256
#define      MAXBUFFERSIZE   40000
#define      MINBUFFERSIZE   10

HANDLE       hInst;
HMENU        hMenu;
char         szAppName[] = "TADemo";
char         szDisplayClass[] = "TextDispClass";
char         szInputPath[PATHSIZE];
HWND         hDisplayWnd = NULLHANDLE;
HANDLE       hTestData = NULLHANDLE;
LPSTR        pTestData = NULL;
DWORD        dwDataSize = 0;
DWORD        dwBlockNum = 0;
DWORD        dwBufferSize = 4096;
VTHDOC       hDoc = VTHDOC_INVALID;
VTHTEXT      hText = VTHDOC_INVALID;
VTHDOC       hDocOriginal = VTHDOC_INVALID;
VTHTEXT      hTextOriginal = VTHDOC_INVALID;

#ifndef UNUSED
#define UNUSED(x) ((x) = (x))
#endif

int WINMAIN_ENTRYMOD WinMain(
                             HINSTANCE hInstance,  
                             HINSTANCE hPrevInstance, 
                             LPSTR lpszCmdLine, 
                             int nCmdShow)
{
    HWND      hwnd;
    MSG       msg;
    WNDCLASS  wndclass;
    ATOM      wndClassAtom[2];

    UNUSED(lpszCmdLine);
    UNUSED(nCmdShow);

    if( !hPrevInstance )
    {
        wndclass.style          = 0;
        wndclass.lpfnWndProc    = (WNDPROC)MainWndProc;
        wndclass.cbClsExtra     = 0;
        wndclass.cbWndExtra     = 0;
        wndclass.hInstance      = hInstance;
        wndclass.hIcon          = LoadIcon( hInstance, szAppName );
        wndclass.hCursor        = LoadCursor( NULL, IDC_ARROW );
        wndclass.hbrBackground  = GetStockObject(LTGRAY_BRUSH);
        wndclass.lpszMenuName   = szAppName;
        wndclass.lpszClassName  = szAppName;

        wndClassAtom[0] = RegisterClass( &wndclass );
        if (!wndClassAtom[0])
            return 0;

        wndclass.style          = CS_HREDRAW;
        wndclass.lpfnWndProc    = (WNDPROC)DisplayWndProc;
        wndclass.hIcon          = NULLHANDLE;
        wndclass.hbrBackground  = GetStockObject(WHITE_BRUSH);
        wndclass.lpszMenuName   = NULL;
        wndclass.lpszClassName  = szDisplayClass;

        wndClassAtom[1] = RegisterClass( &wndclass );
        if (!wndClassAtom[1])
            return 0;
    }
    else
        return 1;

    hInst = hInstance;

    hMenu = LoadMenu( hInstance, "TADemoMenu" );

    hwnd = CreateWindow(szAppName,
        "OIT Text Access Demo",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        50,
        50,
        500,
        380,
        (HWND)NULL, 
        hMenu,
        hInstance,
        NULL );

    if (!hwnd)
        return 0;

    ShowWindow( hwnd, SW_SHOWNORMAL );
    UpdateWindow( hwnd );

    while( GetMessage( &msg, (HWND)NULL, 0, 0 ))
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }       

    DestroyMenu(hMenu);
    UnregisterClass((LPCTSTR)(LONG_PTR)wndClassAtom[0],hInstance);
    UnregisterClass((LPCTSTR)(LONG_PTR)wndClassAtom[1],hInstance);

    return (int)( msg.wParam );
}



/*
|  This is the main window procedure.
*/
WIN_ENTRYSC LRESULT WIN_ENTRYMOD MainWndProc ( 
    HWND hwnd, 
    UINT message, 
    WPARAM wParam, 
    LPARAM lParam )
{
    PAINTSTRUCT  ps;
    HDC          hdc;
    char         OutputBuf[120];
    DLGPROC      lpDlgProc;
    TEXTMETRIC   tm;
    RECT         rcSize;
    LPSTR        strTemp;

    static WORD	wFontHeight;
    static WORD	wFontWidth;
    DAERR err;

#define STATUSBARHEIGHT  (2*wFontHeight)

    switch( message )
    {
    case WM_CREATE:

        // Initialize the Data Access DLL
        err = DAInitEx(SCCOPT_INIT_NOTHREADS, OI_INIT_DEFAULT);
        if (err != DAERR_OK)
        {
            char ebuf[80];
            char buf[256];
            DAGetErrorString(err,ebuf,79);
            sprintf(buf,"SCCDA initialization failed.  Error 0x%x\n\n\"%s\"",err,ebuf);
            MessageBox(NULL,buf,"Error",MB_OK | MB_ICONERROR | MB_APPLMODAL);
            exit(err);
        }

        // Get font dimensions.

        hdc = GetDC( hwnd );
        SelectObject( hdc, GetStockObject( SYSTEM_FONT ) );
        GetTextMetrics( hdc, &tm );
        ReleaseDC( hwnd, hdc );

        wFontHeight = (WORD)tm.tmHeight;
        wFontWidth  = (WORD)tm.tmAveCharWidth;

        // Create window that displays document text.

        hDisplayWnd = CreateWindow( szDisplayClass, NULL,
            WS_CHILD | WS_BORDER | WS_VSCROLL | WS_VISIBLE,
            0, 0, 0, 0,
            hwnd, 0, hInst, NULL );

        UpdateWindow(hwnd);
        break;

    case WM_SIZE:
        GetClientRect(hwnd,&rcSize);
        MoveWindow( hDisplayWnd,
            -1, 
            STATUSBARHEIGHT+1,
            rcSize.right - rcSize.left + 2, 
            rcSize.bottom - rcSize.top - STATUSBARHEIGHT, 
            1 );
        ShowWindow( hDisplayWnd, SW_SHOW );
        return 0;

    case WM_PAINT:
        hdc = BeginPaint( hwnd, &ps );

        SelectObject( hdc, GetStockObject( SYSTEM_FONT ) );
        SetTextAlign( hdc, TA_LEFT | TA_TOP );
        SetBkColor( hdc, 0x00C0C0C0 );
        SetBkMode( hdc, OPAQUE );

        // Show file name.
        if( hDoc )
        {
            strTemp = lstrrchr((LPSTR)szInputPath,'\\');
            strTemp++;
            lstrcpy( OutputBuf, strTemp );
            AnsiUpper( OutputBuf );
        }
        else
            wsprintf( OutputBuf, "No file open." );

        TextOut( hdc, wFontWidth, 0, OutputBuf, lstrlen(OutputBuf) );
        if( dwDataSize || dwBlockNum )
            wsprintf( OutputBuf, "Block %lu:    buffer size = %lu    data size = %lu", dwBlockNum, dwBufferSize, dwDataSize );
        else
            wsprintf( OutputBuf, "Buffer size = %lu", dwBufferSize );

        TextOut( hdc, wFontWidth, wFontHeight, OutputBuf, lstrlen(OutputBuf) );
        ShowWindow( hDisplayWnd, SW_SHOW );

        EndPaint( hwnd, &ps );

        return 0;

    case	WM_COMMAND:

        switch( wParam )
        {
        case IDM_OPEN:

            if( DoOpenFileDlg( szInputPath, hwnd, "Open Input File" ) )
                return 0;

            DoTextClose();
            dwBlockNum = 0;

            if( DoTextOpen(hwnd) == DAERR_OK )
            {
                MenuOn( IDM_CLOSE );
                MenuOn( IDM_SAVETEXTAS );
                MenuOn( IDM_FIRSTBLOCK );
                MenuOff(IDM_NEXTBLOCK);
            }
            InvalidateRect( hwnd, NULL, 1 );
            UpdateWindow( hwnd );
            return 0;

        case IDM_CLOSE:
            DoTextClose();
            InvalidateRect( hwnd, NULL, 1 );

            MenuOff(IDM_CLOSE);
            MenuOff(IDM_SAVETEXTAS);
            MenuOff(IDM_FIRSTBLOCK);
            MenuOff(IDM_NEXTBLOCK);
            return 0;

        case IDM_SAVETEXTAS:

            DoSaveTextAs(hwnd);
            break;

        case IDM_NEXTBLOCK:
            dwBlockNum++;
            if( hDoc )
            {
                // Get the text...
                DoTextBlockAccess( hwnd, (WORD)wParam );

                // Update display.
                GetClientRect(hwnd,&rcSize);
                rcSize.bottom = rcSize.top + STATUSBARHEIGHT;
                InvalidateRect( hwnd, &rcSize, 1 );

                SendMessage( hDisplayWnd, TXM_NEWDATA, 0, 0L );
            }
            return 0;

        case IDM_FIRSTBLOCK:
            dwBlockNum = 0;
            if( hDoc )
            {
                // Get the text...
                DoTextBlockAccess( hwnd, (WORD)wParam );

                // Update display.
                GetClientRect(hwnd,&rcSize);
                rcSize.bottom = rcSize.top + STATUSBARHEIGHT;
                InvalidateRect( hwnd, &rcSize, 1 );

                SendMessage( hDisplayWnd, TXM_NEWDATA, 0, 0L );
            }
            return 0;

        case IDM_BUFSIZE:
            lpDlgProc = (DLGPROC)MakeProcInstance((FARPROC) SizeDlg, hInst);

            if( IDOK == DialogBox (hInst, MAKEINTRESOURCE(SIZEDLG), hwnd, lpDlgProc) )
            {
                // Update display.
                GetClientRect(hwnd,&rcSize);
                rcSize.bottom = rcSize.top + STATUSBARHEIGHT;
                InvalidateRect( hwnd, &rcSize, 1 );
            }
            FreeProcInstance ((FARPROC)lpDlgProc) ;
            return 0;

        case IDM_ABOUT:
            lpDlgProc = (DLGPROC)MakeProcInstance((FARPROC)AboutDlgProc,hInst);
            DialogBox(hInst, MAKEINTRESOURCE(ABOUTDLG), hwnd, lpDlgProc);
            FreeProcInstance((FARPROC)lpDlgProc);
            return 0;

        case IDM_RECORDCOUNT:
            if (hDoc)
            {
                VTLONG  lRecordCount;
                VTCHAR	szCountText[256];

                if (DAERR_OK == DAGetTreeCount(hDoc,(VTLPDWORD)&lRecordCount))
                {
                    wsprintf(szCountText,"%d Records.",lRecordCount);
                    MessageBox(hwnd,szCountText,"RecordCount",MB_OK);
                }
                else
                {
                    MessageBox(hwnd,"Not an archive.","RecordCount",MB_OK);
                }
            }
            return 0;
        case IDM_RECORDINFO:
            if (hDoc)
            {
                VTLONG  lRecordCount;
                VTCHAR	szCountText[256];
                VTLONG	i;
                SCCDATREENODE	NodeInfo;

                if (DAERR_OK == DAGetTreeCount(hDoc,(VTLPDWORD)&lRecordCount))
                {
                    NodeInfo.dwSize = sizeof(NodeInfo);

                    for (i=0;i<lRecordCount;i++)
                    {
                        NodeInfo.dwNode = i;
                        wsprintf(szCountText,"Record %d.",i);
                        if (DAERR_OK == DAGetTreeRecord(hDoc,&NodeInfo))
                        {
                            MessageBox(hwnd,(LPSTR)NodeInfo.szName,szCountText,MB_OK);
                        }
                        else
                        {
                            MessageBox(hwnd,"Error",szCountText,MB_OK);
                        }
                    }
                }
                else
                    MessageBox(hwnd,"Not an archive.","RecordCount",MB_OK);						
            }
            return 0;
        case IDM_RECORDOPEN:
            if (hDoc && !hDocOriginal)
            {
                VTLONG  lRecordCount;						
                SCCDATREENODE	NodeInfo;

                if (DAERR_OK == DAGetTreeCount(hDoc,(VTLPDWORD)&lRecordCount))
                {
                    NodeInfo.dwSize = sizeof(NodeInfo);							
                    NodeInfo.dwNode = 0;

                    if (DAERR_OK == DAGetTreeRecord(hDoc,&NodeInfo))
                    {
                        hDocOriginal = hDoc;
                        hTextOriginal = hText;								
                        if(DAOpenTreeRecord(hDocOriginal,&hDoc,0) == DAERR_OK)
                        {
                            if (TAOpenText(hDoc,&hText) != DAERR_OK)
                            {
                                DACloseTreeRecord(hDoc);
                                hDoc  = hDocOriginal;
                                hText = hTextOriginal;
                                hDocOriginal  = VTHDOC_INVALID;
                                hTextOriginal = VTHDOC_INVALID;
                                MessageBox(hwnd,"Error","TextOpen",MB_OK);

                            }
                            else
                            {
                                MenuOn( IDM_RECORDCLOSE );
                                MenuOff(IDM_CLOSE);
                                MenuOff(IDM_OPEN);
                                MenuOff(IDM_RECORDOPEN);
                                dwBlockNum = 0;					
                                DoTextBlockAccess(hwnd,IDM_FIRSTBLOCK);

                                // Update display.
                                GetClientRect(hwnd,&rcSize);
                                rcSize.bottom = rcSize.top + STATUSBARHEIGHT;
                                InvalidateRect( hwnd, &rcSize, 1 );

                                SendMessage(hDisplayWnd,TXM_NEWDATA,0,0L);
                            }
                        }
                        else
                        {
                            hDoc  = hDocOriginal;
                            hText = hTextOriginal;
                            hDocOriginal  = VTHDOC_INVALID;
                            hTextOriginal = VTHDOC_INVALID;
                            MessageBox(hwnd,"Error","RecordOpen",MB_OK);														
                        }
                    }
                    else
                        MessageBox(hwnd,"Error","RecordInfo",MB_OK);														
                }
                else
                    MessageBox(hwnd,"Not an archive.","RecordCount",MB_OK);						
            }
            else 
            {
                MessageBox(hwnd,"Close open node please.","RecordOpen",MB_OK);
            }
            return 0;
        case IDM_RECORDCLOSE:
            if (hDoc && hDocOriginal)
            {
                TACloseText(hText);
                DACloseTreeRecord(hDoc);
                hDoc			= hDocOriginal;
                hText			= hTextOriginal;
                hDocOriginal	= VTHDOC_INVALID;
                hTextOriginal	= VTHDOC_INVALID;
                MenuOff(IDM_RECORDCLOSE);
                MenuOn(IDM_CLOSE);
                MenuOn(IDM_RECORDOPEN);
                MenuOn(IDM_OPEN);
                dwBlockNum = 0;					
                DoTextBlockAccess(hwnd,IDM_FIRSTBLOCK);

                // Update display.
                GetClientRect(hwnd,&rcSize);
                rcSize.bottom = rcSize.top + STATUSBARHEIGHT;
                InvalidateRect( hwnd, &rcSize, 1 );

                SendMessage(hDisplayWnd,TXM_NEWDATA,0,0L);
            }
            return 0;

        }
        break;


    case WM_DESTROY:
        DoTextClose();

        // De-initialize the Data Access DLL
        DADeInit();

        PostQuitMessage(0);
        return 0;
    }

    return( DefWindowProc( hwnd, message, wParam, lParam ));
}



/*
|  This window procedure displays the text of each 
|  buffer in a scrolling window.
*/	   
long PASCAL DisplayWndProc ( 
                            HWND hwnd, 
                            UINT message, 
                            WPARAM wParam, 
                            LPARAM lParam )
{
    static WORD		wWidth;
    static WORD		wHeight;
    static DWORD	dwLineWidth;
    static DWORD	dwNumLines;
    static DWORD	dwTopLine;
    static WORD		wLinesInWindow;
    static WORD		wDispFontHeight;
    static WORD		wDispFontWidth;
    static DWORD	dwCurDataOffset = 0;

#define OFFSETSTRINGSIZE	8

    HDC         hdc;
    TEXTMETRIC  tm;
    DWORD       dwCurLine;
    RECT        rcSize;
    LPSTR       pData;
    char        OffsetBuf[10];
    int         DataY, DataX;
    DWORD       dwOffset;
    DWORD       dwLineLimit;
    DWORD       dwNewTopLine;
    PAINTSTRUCT ps;

    switch( message )
    {
    case WM_CREATE:
        hdc = GetDC( hwnd );
        SelectObject( hdc, GetStockObject( ANSI_FIXED_FONT ) );
        GetTextMetrics( hdc, &tm );
        wDispFontHeight = (WORD)tm.tmHeight;
        wDispFontWidth = (WORD)tm.tmAveCharWidth;
        ReleaseDC( hwnd, hdc );
        return 0;

    case WM_SIZE:
        GetClientRect(hwnd,&rcSize);
        wHeight = (WORD)(rcSize.bottom-rcSize.top);
        wWidth = (WORD)(rcSize.right-rcSize.left);

        dwLineWidth = wWidth / wDispFontWidth - OFFSETSTRINGSIZE;
        dwLineWidth = max( 10, dwLineWidth - dwLineWidth % 10 );
        wLinesInWindow = wHeight / wDispFontHeight;

        if( dwDataSize )
        {
            dwNumLines = (WORD) (dwDataSize/dwLineWidth);
            if( dwDataSize % dwLineWidth )
                dwNumLines++;

            dwTopLine = dwCurDataOffset / dwLineWidth;
            dwCurDataOffset = dwTopLine * dwLineWidth;

            if( wLinesInWindow < dwNumLines )
                SetScrollRange( hwnd, SB_VERT, 0, (int)(dwNumLines - wLinesInWindow), TRUE );
            else
                SetScrollRange( hwnd, SB_VERT, 0, 0, TRUE );

            SetScrollPos( hwnd, SB_VERT, (int)dwTopLine, TRUE );
        }
        return 0;

    case TXM_NEWDATA:
        InvalidateRect(hwnd,NULL,1);
        dwCurDataOffset = 0;
        dwTopLine = 0;
        SetScrollPos( hwnd, SB_VERT, 0, TRUE );

        if( hDoc && dwDataSize )
        {
            dwNumLines = (WORD) (dwDataSize/dwLineWidth);
            if( dwDataSize % dwLineWidth )
                dwNumLines++;

            if( wLinesInWindow < dwNumLines )
                SetScrollRange( hwnd, SB_VERT, 0, (int)(dwNumLines - wLinesInWindow), FALSE );
            else
                SetScrollRange( hwnd, SB_VERT, 0, 0, FALSE );
        }
        else
        {
            dwNumLines = 0;
            dwTopLine = 0;
        }
        return 0;

    case WM_VSCROLL:
        if( hDoc && dwDataSize )
        {
            int nScrollCode;
            short int nPos;

            nScrollCode = LOWORD(wParam);
            nPos = HIWORD(wParam);

            dwNewTopLine = dwTopLine;

            switch( nScrollCode )
            {
            case SB_TOP:
                dwNewTopLine = 0;
                break;
            case SB_BOTTOM:
                if( dwNumLines > wLinesInWindow )
                    dwNewTopLine = dwNumLines - wLinesInWindow;
                break;
            case SB_LINEUP:
                if( dwTopLine > 0 )
                    dwNewTopLine = dwTopLine-1;
                break;
            case SB_LINEDOWN:
                dwNewTopLine = min( dwNumLines-wLinesInWindow, dwTopLine+1);
                break;
            case SB_PAGEUP:
                dwNewTopLine = max((int)dwTopLine - (int)wLinesInWindow, 0 );
                break;
            case SB_PAGEDOWN:
                dwNewTopLine = min( dwNumLines - wLinesInWindow, dwTopLine+wLinesInWindow );
                break;
            case SB_THUMBTRACK:
            case SB_THUMBPOSITION:
                dwNewTopLine = nPos;
                break;
            }

            if( (WORD) dwNewTopLine != dwTopLine && dwNewTopLine >= 0 )
            {
                ScrollWindow( hwnd, 0, (int)((dwTopLine - dwNewTopLine)*wDispFontHeight), NULL, NULL );

                dwTopLine = (WORD)dwNewTopLine;
                dwCurDataOffset = dwTopLine * dwLineWidth;

                UpdateWindow( hwnd );
                SetScrollPos( hwnd, SB_VERT, (int)dwNewTopLine, TRUE );
            }
        }

        return 0;


    case WM_PAINT:
        hdc = BeginPaint( hwnd, &ps );

        if( hDoc && dwDataSize )
        {
            pData = pTestData + dwCurDataOffset;
            SelectObject( hdc, GetStockObject( ANSI_FIXED_FONT ) );

            DataY = 0;
            DataX = OFFSETSTRINGSIZE * wDispFontWidth;
            dwOffset = dwCurDataOffset;
            dwCurLine = dwTopLine;

            dwLineLimit = dwTopLine + wLinesInWindow;
            if( wHeight % wDispFontHeight )
                dwLineLimit++;	// Display fractional lines at bottom, too.
            dwLineLimit = min( dwLineLimit, dwNumLines );

            while( dwCurLine < dwLineLimit )
            {
                wsprintf( OffsetBuf, "%lu:", dwOffset );
                TextOut( hdc, wDispFontWidth, DataY, OffsetBuf, lstrlen(OffsetBuf) );

                if( dwCurLine == dwNumLines-1 && (dwDataSize % dwLineWidth) )
                    TextOut( hdc, DataX, DataY, pData, (int)(dwDataSize % dwLineWidth) );
                else
                    TextOut( hdc, DataX, DataY, pData, (int)dwLineWidth );

                pData += dwLineWidth;
                DataY += wDispFontHeight;
                dwOffset += dwLineWidth;

                dwCurLine++;
            }
        }

        EndPaint( hwnd, &ps );

        return 0;
    }
    return (long)( DefWindowProc( hwnd, message, wParam, lParam ));
}




/*
|  This function calls TAOpenText() to open the input 
|  document, and handles error conditions if they occur.
*/

DAERR DoTextOpen( 
                 HWND   hwnd )
{
    DAERR    err;
    LPSTR  pFileName;


    err = DAOpenDocument( &hDoc, (VTDWORD)IOTYPE_ANSIPATH, (VTLPVOID)szInputPath, 0 );
    if( err == DAERR_OK )
    {
        err = TAOpenText( hDoc, &hText );
        if( err != DAERR_OK )
            DACloseDocument( hDoc );
    }

    if( err != DAERR_OK )
    {
        char TextBuf[1024];

        pFileName = lstrrchr( szInputPath, '\\' );
        pFileName++;

        DAGetErrorString(err,TextBuf,1024);
        MessageBox(hwnd,TextBuf,pFileName,MB_OK | MB_ICONSTOP);
        return err;
    }

    return 0;
}




/*
|  This function closes the input document with a call 
|  to SccCloseFileForText(), and frees the allocated buffer.
*/

void DoTextClose()
{
    if( hDoc )
    {
        TACloseText( hText );
        DACloseDocument( hDoc );
        hDoc = VTHDOC_INVALID;
    }

    dwDataSize = 0L;

    if( hTestData )
    {
        GlobalUnlock(hTestData);
        GlobalFree(hTestData);
        hTestData = NULLHANDLE;
    }
}




/*
|  This function calls SccGetFirstBlock() or SccGetNextBlock()  
|  to retrieve text from the input document, and handles error 
|  conditions if they occur.  
|  The text is read into a globally allocated buffer of the current
|  buffer size.  
*/

DAERR DoTextBlockAccess(
                        HWND  hwnd,
                        WORD  wType )
{
    HCURSOR  hCursor;
    DAERR    err;

    if( hTestData )		// Free any existing buffer.
    {
        GlobalUnlock(hTestData);
        GlobalFree(hTestData);
    } 

    // Allocate the buffer for the current block of text.
    hTestData = GlobalAlloc( GMEM_MOVEABLE, (DWORD)dwBufferSize );
    if( hTestData == NULLHANDLE )
    {
        MessageBox( hwnd, "Memory for buffer couldn't be allocated.", NULL, MB_OK|MB_ICONSTOP );
        return (DAERR)-1;
    }

    pTestData = GlobalLock(hTestData);

    hCursor = SetCursor( LoadCursor((HINSTANCE)NULL, IDC_WAIT) );
    ShowCursor(TRUE);


    if( wType == IDM_FIRSTBLOCK )
        err = TAReadFirst( hText, (VTLPBYTE)pTestData, (VTDWORD)dwBufferSize, (VTLPDWORD)&dwDataSize );
    else
        err = TAReadNext( hText, (VTLPBYTE)pTestData, (VTDWORD)dwBufferSize, (VTLPDWORD)&dwDataSize );


    ShowCursor( FALSE );
    SetCursor( hCursor );

    switch( err )
    {
    case SCCERR_OK:
        MenuOn(IDM_NEXTBLOCK);
        break;

    case SCCERR_EOF:
        MenuOff(IDM_NEXTBLOCK);

        if( wType == IDM_FIRSTBLOCK && dwDataSize == 0 )
        {
            MessageBeep(0);
            MessageBox( hwnd, "No text found in file." , NULL, MB_OK | MB_ICONSTOP ) ;
        }
        break;

    default:{
        char TextBuf[1024];

        LPSTR pFileName = lstrrchr( szInputPath, '\\' );
        pFileName++;

        DAGetErrorString(err,TextBuf,1024);
        MessageBeep(0);
        MessageBox(hwnd,TextBuf,pFileName,MB_OK | MB_ICONSTOP);

        hDoc = VTHDOC_INVALID;
        dwDataSize = 0L;

        MenuOff(IDM_CLOSE);
        MenuOff(IDM_SAVETEXTAS);
        MenuOff(IDM_FIRSTBLOCK);
        MenuOff(IDM_NEXTBLOCK);
            }
    }

    return err;
}


/*
|  Buffer size dialog function.
*/
int PASCAL SizeDlg(
                   HWND hDlg,
                   WORD wMsg,
                   WORD wParam,
                   LONG lParam )
{
    BOOL  bTrans;
    UNUSED(lParam);

    switch (wMsg) 
    {
    case WM_COMMAND:  
        {
            DWORD dwNewBufferSize;
            switch (wParam) 
            {
            case IDOK:
                dwNewBufferSize = GetDlgItemInt( hDlg, DLG_NUMEDIT, &bTrans, 0 );
                if( bTrans )
                {
                    if ((dwNewBufferSize > MAXBUFFERSIZE) || (dwNewBufferSize < MINBUFFERSIZE))
                    {
                        char Msg[80];
                        wsprintf(Msg, "Buffer size should be between %d and %d", MINBUFFERSIZE, MAXBUFFERSIZE);
                        MessageBox(hDlg, Msg, "Text Access Demo", MB_OK);
                        return (FALSE);
                    }
                    else
                    {
                        dwBufferSize = dwNewBufferSize;
                    }
                }

            case IDCANCEL:
                EndDialog (hDlg, wParam);
                return (FALSE);
            }
        }
        break;

    case WM_INITDIALOG:
        SetFocus (GetDlgItem (hDlg, DLG_NUMEDIT));
        SetDlgItemInt(hDlg, DLG_NUMEDIT, (UINT)dwBufferSize, 0 );
        SendDlgItemMessage( hDlg, DLG_NUMEDIT, EM_SETSEL, 0, MAKELONG(0,0x7fff) );
        return (FALSE);
    }
    return (FALSE);
}



/*
|	About dialog function
*/
BOOL PASCAL AboutDlgProc(
                         HWND      hDlg,
                         unsigned  message,
                         WORD      wParam,
                         LONG      lParam )
{
    RECT  locRect;
    int   locX;
    int   locY;
    char  buffer[100];
    UNUSED(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        lstrcat(lstrcpy(buffer, "Build:\t"), S_FILEVERSION);
        SetDlgItemText(hDlg,HELP_ABOUTBOXVERINFO,buffer);
        GetWindowRect(hDlg,&locRect);
        locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
        locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
        SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);
        return (TRUE);

    case WM_COMMAND:
        if (wParam == IDOK)
        {
            EndDialog(hDlg, 0);
            return (TRUE);
        }
        break;
    }

    return (FALSE);
}


/*
|  This function calls the common dialog DLL to 
|  get the input file name.
*/
WORD  DoOpenFileDlg(
                    LPSTR  szFileName,
                    HWND   hWnd,
                    LPSTR  szCaption )
{
    OPENFILENAME  locOFN;
    TCHAR        locFilterSpec[128] = TEXT("All files\0*.*\0");
    TCHAR        locFileName[PATHSIZE];
    TCHAR        locFileTitle[PATHSIZE];


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
    locOFN.nMaxFile = PATHSIZE;
    locOFN.lpstrFileTitle = locFileTitle;
    locOFN.nMaxFileTitle = PATHSIZE;
    locOFN.lpstrInitialDir = NULL;
    locOFN.lpstrTitle = szCaption;
    locOFN.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    locOFN.lpstrDefExt = NULL;
    locOFN.lCustData = 0;
    locOFN.lpfnHook = NULL;
    locOFN.lpTemplateName = NULL;

    *szFileName = 0;
    if (GetOpenFileName(&locOFN) == TRUE)
    {
        strcpy(szFileName,locFileName);
        return 0;
    }

    return 1;
}

VOID DoSaveTextAs(
                  HWND hwnd)
{
    int         hFile;
    HANDLE      hBuffer;
    LPSTR       pBuffer;
    DAERR       locErr;
    DWORD       locSize;
    OFSTRUCT    locOf;

    if (hDoc)
    {
        if ((hFile = OpenFile("C:\\tademo.txt",&locOf,OF_WRITE | OF_CREATE)) != -1)
        {
            hBuffer = GlobalAlloc(GMEM_MOVEABLE, 1024);

            if (hBuffer != NULL)
            {
                pBuffer = GlobalLock(hBuffer);

                locErr = TAReadFirst( hText, (VTLPBYTE)pBuffer, 1024, &locSize );

                while (locErr == SCCERR_OK)
                {
                    _lwrite(hFile, pBuffer, (WORD)locSize);
                    locErr = TAReadNext( hText, (VTLPBYTE)pBuffer, 1024, &locSize );
                }

                if (locErr == SCCERR_EOF)
                {
                    _lwrite(hFile, pBuffer, (WORD)locSize);
                    MenuOff(IDM_NEXTBLOCK);
                }
                else 
                {
                    char TextBuf[1024];
                    LPSTR pFileName;

                    pFileName = lstrrchr( szInputPath, '\\' );
                    pFileName++;

                    DAGetErrorString(locErr,TextBuf,1024);
                    MessageBeep(0);
                    MessageBox(hwnd,TextBuf,pFileName,MB_OK | MB_ICONSTOP);

                    hDoc = VTHDOC_INVALID;
                    dwDataSize = 0L;

                    MenuOff(IDM_CLOSE);
                    MenuOff(IDM_SAVETEXTAS);
                    MenuOff(IDM_FIRSTBLOCK);
                    MenuOff(IDM_NEXTBLOCK);
                }

                GlobalUnlock(hBuffer);
                GlobalFree(hBuffer);
            }

            _lclose(hFile);
        }
    }
}


LPSTR  lstrrchr( 
                LPSTR str, 
                char ch)
{
    LPSTR  strl;

    strl = str + lstrlen(str);

    do
    {
        if( ch == *strl )
            return strl;

        strl = AnsiPrev( str, strl );

    } while( strl > str );

    return NULL;
}

