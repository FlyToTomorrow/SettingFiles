
/*
|  Outside In Viewer Technology
|  Sample Application
|  
|
|  RFASERV
|  Remote Filter Access 
|  Server Sample Application
|
|  Purpose:
|  To demonstrate the use of the Remote Filter Access technology
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
|  Includes
|
*/

#define  STRICT
#include <windows.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <commdlg.h>
#include <direct.h>
#include <ctype.h>

#define WINDOWS
#include "sccra.h"
#include "sccio.h"

#include "rfaserv.h"


/*
|
|  Global Variables
|
*/

/*
|  RfaClients[] is a global array of structures used to keep track
|  of the multiple instances of the server side of the named
|  pipe.  As a client connects to a given instance, a new
|  server thread is created and added to the array.
*/
WRTHANDLE RfaClients[MAX_PIPE_INSTANCES];
DWORD     dwClientCount = 0;             // Global count of connected clients.

HWND   hWnd;
HANDLE hInst;
HBITMAP hMidLive, hMidDead;
HBITMAP hEndLive, hEndDead;
HBITMAP hServer,  hServerAlone;


/*
|
|  WinMain
|  Program initialization, message loop processing 
|
*/
int APIENTRY WinMain (
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR  lpCmdLine,
	int    nCmdShow)


{
MSG  msg;
WNDCLASS wc;
OSVERSIONINFO Ver;
DAERR err;

	UNREFERENCED_PARAMETER( lpCmdLine );
	UNREFERENCED_PARAMETER( hPrevInstance );

	Ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	Ver.dwPlatformId = 0;
	GetVersionEx( &Ver );
	if( Ver.dwPlatformId != VER_PLATFORM_WIN32_NT )
		{
		MessageBox( NULL, "This application requires Windows NT", "RFAServ Error",
			MB_ICONINFORMATION | MB_OK | MB_TASKMODAL);
		return 0;
		}

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

	hInst = hInstance;

	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon (hInstance, "rfaserv");
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = GetStockObject (WHITE_BRUSH);
	wc.lpszMenuName = "PipeMenu";
	wc.lpszClassName = "PipeWClass";

	RegisterClass(&wc);

	hMidLive = LoadBitmap (hInst, MAKEINTRESOURCE(BMP_MIDLIVE));
	hMidDead = LoadBitmap (hInst, MAKEINTRESOURCE(BMP_MIDDEAD));
	hEndLive = LoadBitmap (hInst, MAKEINTRESOURCE(BMP_ENDLIVE));
	hEndDead = LoadBitmap (hInst, MAKEINTRESOURCE(BMP_ENDDEAD));
	hServer  = LoadBitmap (hInst, MAKEINTRESOURCE(BMP_SERVERLIVE));
	hServerAlone = LoadBitmap (hInst, MAKEINTRESOURCE(BMP_SERVERDEAD));

	hWnd = CreateWindow ("PipeWClass",
							  "SccViewer Remote Data Access Server",
							  WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT,
							  CW_USEDEFAULT,
							  CW_USEDEFAULT,
							  CW_USEDEFAULT,
							  NULL,
							  NULL,
							  hInstance,
							  NULL);


	ShowWindow (hWnd, nCmdShow);

	while (GetMessage (&msg, NULL, 0, 0))
		DispatchMessage (&msg);

	DADeInit();

	if( hMidLive ) DeleteObject(hMidLive);
	if( hMidDead ) DeleteObject(hMidLive);
	if( hEndLive ) DeleteObject(hEndDead);
	if( hEndDead ) DeleteObject(hEndDead);
	if( hServer )  DeleteObject(hServer);
	if( hServerAlone ) DeleteObject(hServerAlone);

	return (int)(msg.wParam);

}

/*
|
|  MainWndProc
|  Message Handling, client thread launching 
|
*/
LONG CALLBACK MainWndProc (
	HWND   hwnd,
	UINT   message,
	WPARAM wParam,
	LPARAM lParam)
{
LONG        lpServerThreadID;
PAINTSTRUCT paintStruct;
HDC         hDC;

	switch (message)
		{
		case WM_PAINT:
			// DrawBranch is used to paint the spools and text to the window.
			hDC = BeginPaint (hwnd, &paintStruct);
			DrawBranch (hDC);
			EndPaint (hwnd, &paintStruct);
			return(0);

		case WM_CREATE :
			// Create the first instance of a server side of the pipe.
			CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
						(DWORD)0,                          // Same stack size.
						(LPTHREAD_START_ROUTINE)ServerProc,// Thread procedure.
						(LPVOID)hwnd,                     // Parameter.
						(DWORD)0,                          // Start immediatly.
						(LPDWORD)&lpServerThreadID);       // Thread ID.
			return (0);

		case WM_DESTROY :
			PostQuitMessage (0);
			return (0);
		}
	return (LONG)DefWindowProc (hwnd, message, wParam, lParam);
}



/*
|
|  PROCEDURE: ServerProc (HWND hWnd)
|
|  PURPOSE:
|
|    A thread procedure, which creates an instance of the server side of
|    the named pipe, and then blocks waiting for a client to connect.
|    Once the client connects, a global array is updated with the specific
|    clients information, and this procedure is called again
|    to launch another waiting server thread.  After launching the new
|    thread, this thread begins to loop, reading the named pipe.  When
|    a message comes from it's client, it uses TellAll() to broadcast
|    the message to the other clients in the array.
|
|  CALLED BY:
|
|    ServerProc();
|    WinMain();
|
|  CALLS TO:
|
|    TellAll();
|    ServerProc().
|
|  COMMENTS:
|
|    Clients is a global array which hold information on each client
|    connected to the named pipe.  This procedure recieves a buffer.
|    It then steps through this global array, and for each client it
|    writes the buffer.
|
*/

VOID ServerProc(HWND hWnd)
 {
HANDLE hPipe;                       // Pipe handle.
DWORD  ServerThreadID;              // Used for CreateThread().
CHAR   errorBuf[LINE_LEN] = "";     // Used for error messages.
DWORD  bytesRead;                   // Used in ReadFile().
DWORD  bytesWritten;
DWORD  retCode;                     // Used to trap return codes.
DWORD  clientIndex;                 // Index into global array, for this
	                                 // instances client.
DWORD  lastError;                   // Traps returns from GetLastError().
BOOL   ExitLoop = FALSE;            // Boolean Flag to exit loop.
MYMESSAGEHDR inMessage, outMessage;
VTHREMOTE  hTopRemote = VTHDOC_INVALID;
SECURITY_ATTRIBUTES sa;
PSECURITY_DESCRIPTOR pSD;

	// Create a local named pipe with
	// the name '\\.\PIPE\sccviewer'.  The
	// '.' signifies local pipe.
	// Allocate memory for the security descriptor.
	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	// Initialize the new security descriptor.
	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
	// Add a NULL descriptor ACL to the security descriptor.
	SetSecurityDescriptorDacl(pSD, TRUE, (PACL) NULL, FALSE);
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = TRUE;
	hPipe = CreateNamedPipe ("\\\\.\\PIPE\\sccviewer",  // Pipe name = 'sccviewer'.
		PIPE_ACCESS_DUPLEX | FILE_FLAG_WRITE_THROUGH,    // 2 way pipe.
		PIPE_TYPE_MESSAGE | PIPE_WAIT,    // Wait on messages.
		MAX_PIPE_INSTANCES,               // Maximum instance limit.
		OUT_BUF_SIZE,                     // Buffer sizes.
		IN_BUF_SIZE,
		TIME_OUT,                         // Specify time out.
		&sa);                             // No securities specified.

	retCode = GetLastError();         // Report any error, it should always succeed

   // Check Errors.
	if( hPipe == (HANDLE)(LONG_PTR)ERROR_INVALID_PARAMETER || 
		 hPipe == INVALID_HANDLE_VALUE)
		{
		wsprintf (errorBuf,  
			"Error return code from CreateNamedPipe = %li.",
			retCode);
		MessageBox (hWnd, errorBuf, "Debug Window",
			MB_ICONINFORMATION | MB_OK | MB_APPLMODAL);
		LocalFree(pSD);
		ExitThread(0);                      // Clean up and die.
		}

                                       // Block until a client connects.
	ConnectNamedPipe(hPipe, NULL);

	// Set the clientIndex, then increment
	// the count.  Fill in the structure
	// for this client in the array.
	clientIndex = dwClientCount++;
	RfaClients[clientIndex].hPipe   = hPipe;
	RfaClients[clientIndex].live    = TRUE;
	RfaClients[clientIndex].dwTotalBytes = 0;
	RfaClients[clientIndex].dwLastRequest = 0;
	RfaClients[clientIndex].dwLastBytes = 0;
	RfaClients[clientIndex].dwTotalRequest = 0;

	// Read from the client, the first
	// first message should always be
	// OPENPIPE
	ReadFile (hPipe, &inMessage, sizeof(inMessage), &bytesRead, NULL);
	if (inMessage.dwType == MESSAGE_OPENPIPE)
		{
		CHAR inBuf[256];
		ReadFile (hPipe, inBuf, inMessage.dwMoreDataSize, &bytesRead, NULL);
		strcpy (RfaClients[clientIndex].szName, inBuf);
		outMessage.dwType = MESSAGE_OPENPIPERET;
		outMessage.dwFlags = 0;
		outMessage.dwParam1 = 0;
		outMessage.dwParam2 = 0;
		outMessage.dwMoreDataSize = 0;
		outMessage.dwRet = MESSAGE_RETOK;
		WriteFile (hPipe, &outMessage, sizeof(outMessage), &bytesWritten, NULL);
		}
	else
		{
		RfaClients[clientIndex].live = FALSE;  
		CloseHandle (hPipe);                   // Close handles.
		DisconnectNamedPipe (hPipe);           // Close pipe instance.
		ExitThread(0);                         // Clean up and die.
		}

	// Create a thread which will make
	// another server instance of the
	// named pipe.
	CreateThread ((LPSECURITY_ATTRIBUTES)NULL,        // No security attributes.
		(DWORD)0,                           // Use same stack size.
		(LPTHREAD_START_ROUTINE)ServerProc, // Thread procedure.
		(LPVOID)hWnd,                       // Parameter to pass.
		(DWORD)0,                           // Run immediately.
		(LPDWORD)&ServerThreadID);          // Thread identifier.

	InvalidateRect(hWnd, NULL, TRUE);
	do
		{
		retCode = ReadFile (hPipe, &inMessage, sizeof(inMessage), &bytesRead, NULL);

		if (retCode)
			{
			hTopRemote = ProcessClientMessage(hWnd, &inMessage, hPipe, clientIndex);
			}
		else
			{
			lastError = GetLastError();
			switch (lastError)
				{
				case ERROR_BROKEN_PIPE:
					ExitLoop = TRUE;
					break;
				default:
					wsprintf (errorBuf, "ReadFile Error in ServerProc()= %d",
					lastError);
					MessageBox (hWnd, errorBuf, "Debug Information", MB_OK);
					ExitLoop = TRUE;
					break;
				}
			}
		}while(!ExitLoop);

	if (hTopRemote)
		{
		VTHDOC hDoc;
		DARetrieveDocHandle( hTopRemote, &hDoc );
		RACloseRemote( hTopRemote );
		DACloseDocument( hDoc );
		hTopRemote = VTHDOC_INVALID;
		}
	RfaClients[clientIndex].live = FALSE;  // Turns spool gray.
	InvalidateRect(hWnd, NULL, TRUE);
	CloseHandle (hPipe);                // Close handles.
	DisconnectNamedPipe (hPipe);        // Close pipe instance.
	LocalFree(pSD);
	ExitThread(0);                      // Clean up and die.
}

VTHDOC ProcessClientMessage(
	HWND hWnd, 
	PMYMESSAGEHDR pinMessage, 
	HANDLE hPipe, 
	DWORD ClientIndex)
{
DWORD   bytesWritten;
DWORD   bytesRead;
MYMESSAGEHDR outMessage;
VTHDOC  hDoc = VTHDOC_INVALID;
VTHREMOTE hRemote = VTHDOC_INVALID;

	switch(pinMessage->dwType)
		{
		case MESSAGE_OPENFILE:
			{
			CHAR  szFileName[256];
			DAERR daResult;

			ReadFile (hPipe, szFileName, pinMessage->dwMoreDataSize, &bytesRead, NULL);

			daResult = DAOpenDocument( &hDoc, IOTYPE_ANSIPATH, szFileName, 0 );
			if( daResult == DAERR_OK )
				{
				daResult = RAOpenRemote( hDoc, &hRemote );
				if( daResult != DAERR_OK )
					DACloseDocument( hDoc );
				}

			if (daResult == DAERR_OK)
				{
				outMessage.dwType = MESSAGE_OPENFILERET;
				outMessage.dwFlags = 0;
#ifdef WIN64
				outMessage.dwParam1 = (DWORD_PTR)hRemote;
#else
				outMessage.dwParam1 = (DWORD)hRemote;
#endif
				outMessage.dwParam2 = 0;
				outMessage.dwMoreDataSize = 0;
				outMessage.dwRet = MESSAGE_RETOK;
				}
			else
				{
				outMessage.dwType = MESSAGE_OPENFILERET;
				outMessage.dwFlags = 0;
				outMessage.dwParam1 = 0;
				outMessage.dwParam2 = 0;
				outMessage.dwMoreDataSize = 0;
				outMessage.dwRet = MESSAGE_RETERROR;
				}
			WriteFile (hPipe, &outMessage, sizeof(outMessage), &bytesWritten, NULL);
			}
			break;
	
		case MESSAGE_CLOSEFILE:
			hRemote = (VTHREMOTE)pinMessage->dwParam1;
			DARetrieveDocHandle( hRemote, &hDoc );

			RACloseRemote( hRemote );
			DACloseDocument( hDoc );

			outMessage.dwType = MESSAGE_CLOSEFILERET;
			outMessage.dwFlags = 0;
			outMessage.dwParam1 = 0;
			outMessage.dwParam2 = 0;
			outMessage.dwMoreDataSize = 0;
			outMessage.dwRet = MESSAGE_RETOK;
			WriteFile (hPipe, &outMessage, sizeof(outMessage), &bytesWritten, NULL);

			hDoc = VTHDOC_INVALID;
			break;

		case MESSAGE_OPENSUBOBJECT:
			{
			VTHDOC	hSubDoc;
			VTHREMOTE hSubRemote = 0;
			IOSPECSUBOBJECT ObjectSpec;
			DAERR daResult;

			hRemote = (VTHREMOTE)pinMessage->dwParam1;
			ReadFile (hPipe, &ObjectSpec, pinMessage->dwMoreDataSize, &bytesRead, NULL);
			daResult = DAOpenDocument( &hSubDoc, IOTYPE_SUBOBJECT, &ObjectSpec, 0 );

			if( daResult == DAERR_OK )
				{
				daResult = RAOpenRemote( hSubDoc, &hSubRemote );
				if( daResult != DAERR_OK )
					DACloseDocument( hSubDoc );
				}

			if (daResult == DAERR_OK)
				{
				outMessage.dwType = MESSAGE_OPENSUBOBJECTRET;
				outMessage.dwFlags = 0;
				outMessage.dwParam1 = (DWORD_PTR) hSubRemote;
				outMessage.dwParam2 = 0;
				outMessage.dwMoreDataSize = 0;
				outMessage.dwRet = MESSAGE_RETOK;
				}
			else
				{
				outMessage.dwType = MESSAGE_OPENSUBOBJECTRET;
				outMessage.dwFlags = 0;
				outMessage.dwParam1 = 0;
				outMessage.dwParam2 = 0;
				outMessage.dwMoreDataSize = 0;
				outMessage.dwRet = MESSAGE_RETERROR;
				}

			WriteFile (hPipe, &outMessage, sizeof(outMessage), &bytesWritten, NULL);
			}
			break;

		case MESSAGE_REQUESTDATA:
			{
			DWORD	dwRequest=0;
			LPBYTE	pData;
			DWORD	dwDataSize;
			RECT		rect;
			DAERR    daResult;
			IORFAREQUEST rfaRequest;

			hRemote = (VTHREMOTE)pinMessage->dwParam1;
			ReadFile (hPipe, &rfaRequest, pinMessage->dwMoreDataSize, &bytesRead, NULL);
			daResult = RAGetRemoteData( hRemote, &rfaRequest, &pData, &dwDataSize);
			if (daResult == DAERR_OK)
				{
				outMessage.dwType = MESSAGE_REQUESTDATARET;
				outMessage.dwFlags = 0;
#ifdef WIN64
				outMessage.dwParam1 = (DWORD_PTR)hRemote;
#else
				outMessage.dwParam1 = (DWORD)hRemote;
#endif
				outMessage.dwParam2 = 0;
				outMessage.dwMoreDataSize = dwDataSize;
				outMessage.dwRet = MESSAGE_RETOK;
				WriteFile (hPipe, &outMessage, sizeof(outMessage), &bytesWritten, NULL);
				WriteFile (hPipe, pData, dwDataSize, &bytesWritten, NULL);
				RfaClients[ClientIndex].dwTotalBytes += dwDataSize;
				RfaClients[ClientIndex].dwLastRequest = dwRequest;
				RfaClients[ClientIndex].dwLastBytes = dwDataSize;
				RfaClients[ClientIndex].dwTotalRequest++;
				}
			else
				{
				outMessage.dwType = MESSAGE_REQUESTDATARET;
				outMessage.dwFlags = 0;
				outMessage.dwParam1 = 0;
				outMessage.dwParam2 = 0;
				outMessage.dwMoreDataSize = 0;
				outMessage.dwRet = MESSAGE_RETERROR;
				WriteFile (hPipe, &outMessage, sizeof(outMessage), &bytesWritten, NULL);
				}
			rect.top = ClientIndex * BITMAP_Y;            // Y is based on index in the array.
			rect.bottom = rect.top +  BITMAP_Y;                    // X position is constant.
			rect.left = 0;
			rect.right = 1024;
			InvalidateRect(hWnd, &rect, TRUE);
			}
			break;

		default:
			break;
		}
	return hRemote;
}




/*************************************************************************\
*
*  PROCEDURE: DrawBranch (HDC hDC)
*
*  PURPOSE:
*
*    To draw one of four bitmaps for each client, depending upon the clients
*    status (alive = red spool, dead or disconnected = gray), and location in
*    the array.  It also draws the clients user name beside the spool.
*    This procedure is executed when the WM_PAINT message is trapped.
*
*  CALLED BY:
*
*    WinMain();
*
*  COMMENTS:
*
\*************************************************************************/
VOID DrawBranch(HDC hDC)
{
SHORT X, Y;
DWORD index;
CHAR ComputerName[32];
CHAR buf[256];
DWORD dwMaxLen;

	if( dwClientCount == 0 )
		DrawBitmap( hDC, hServerAlone, BITMAP_X, 0 );
	else
		DrawBitmap( hDC, hServer, BITMAP_X, 0 );

	dwMaxLen = 32;
	GetComputerName( (LPTSTR)ComputerName, &dwMaxLen );
	wsprintf(buf, "sccviewer Server on %-10s", ComputerName );
	TextOut (hDC, NAME_X, NAME_Y, buf, (int)strlen(buf));


	X = BITMAP_X;     // X position is constant.
	Y = 0;            // Y is based on index in the array.

	// For each client, determine if
	// if alive or not, and position;
	// then blt appropriate map and
	// clients name.
	for (index = 0; index < dwClientCount; index++)
		{
		X = BITMAP_X;                      // X position is constant.
		Y = (SHORT)((index+1) * BITMAP_Y); // Y is based on index in the array.

		if (index < dwClientCount - 1)     // ClientCount - 1 = last (end) client.
			{
			if(RfaClients[index].live)        // If live = red, else = gray.
				DrawBitmap( hDC, hMidLive, X, Y );
			else
				DrawBitmap( hDC, hMidDead, X, Y );
			}
		else
			{
			if(RfaClients[index].live)        // If live = red, else = gray.
				DrawBitmap( hDC, hEndLive, X, Y );
			else
				DrawBitmap( hDC, hEndDead, X, Y );
			}

		X =  NAME_X;
		Y += NAME_Y;
		wsprintf(buf, "%-10s", RfaClients[index].szName );
		TextOut (hDC, X, Y, buf, (int)strlen(buf));

		X = REQ_X;
		wsprintf(buf, "Total Req: %04d", RfaClients[index].dwTotalRequest );
		TextOut(hDC, X, Y, buf, (int)strlen(buf));

		X = BYTES_X;
		wsprintf(buf, "Total Bytes: %06d", RfaClients[index].dwTotalBytes );
		TextOut(hDC, X, Y, buf, (int)strlen(buf));

		X = NAME_X;
		Y += NAME_DELTAY;
		if(!RfaClients[index].live)        // If live = red, else = gray.
			{
			strcpy( buf, "(Disconnected)" );
			TextOut(hDC, X, Y, buf, (int)strlen(buf));
			}

		X = REQ_X;
		wsprintf(buf, "Last Req: 0x%008x", RfaClients[index].dwLastRequest );
		TextOut(hDC, X, Y, buf, (int)strlen(buf));

		X = BYTES_X;
		wsprintf(buf, "Last Bytes: %06d", RfaClients[index].dwLastBytes );
		TextOut(hDC, X, Y, buf, (int)strlen(buf));
		}
}

VOID DrawBitmap(
	HDC     hDC,
	HBITMAP hBitmap,
	SHORT   X,
	SHORT   Y )
{
HDC hDCMem;
BITMAP bm;
POINT ptSize, ptOrg;

	if( hBitmap )
		{
		// Blt the chosen map.
		hDCMem = CreateCompatibleDC(hDC);

		SelectObject(hDCMem, hBitmap);
		SetMapMode(hDCMem, GetMapMode(hDC));
		GetObject(hBitmap, sizeof(BITMAP), &bm);

		ptSize.x = bm.bmWidth;
		ptSize.y = bm.bmHeight;
		DPtoLP (hDC, &ptSize, 1);

		ptOrg.x = 0;
		ptOrg.y = 0;
		DPtoLP (hDCMem, &ptOrg, 1);

		BitBlt(hDC, X, Y, ptSize.x, ptSize.y,
			hDCMem, ptOrg.x, ptOrg.y, SRCCOPY);

		DeleteDC(hDCMem);
		}
}

