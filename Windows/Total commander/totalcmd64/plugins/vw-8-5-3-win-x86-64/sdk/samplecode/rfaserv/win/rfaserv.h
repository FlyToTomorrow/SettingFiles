
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



#define MESSAGE_OPENPIPE           1
#define MESSAGE_OPENFILE           2
#define MESSAGE_CLOSEFILE          3
#define MESSAGE_OPENSUBOBJECT      4
#define MESSAGE_REQUESTDATA        5

#define MESSAGE_OPENPIPERET       10
#define MESSAGE_OPENFILERET       20
#define MESSAGE_CLOSEFILERET      30
#define MESSAGE_OPENSUBOBJECTRET  40
#define MESSAGE_REQUESTDATARET    50

#define MESSAGE_RETOK     0
#define MESSAGE_RETERROR  1

typedef struct myMessageHdrTag
	{
	DWORD	dwType;
	DWORD	dwFlags;
#ifdef WIN64
	DWORD_PTR	dwParam1;
	DWORD_PTR	dwParam2;
#else
	DWORD   dwParam1;
	DWORD   dwParam2;
#endif
	DWORD	dwRet;
	DWORD	dwMoreDataSize;
	} MYMESSAGEHDR, * PMYMESSAGEHDR;



#define MAX_PIPE_INSTANCES 100

#define LINE_LEN   80
#define NAME_SIZE  25

// Bitmaps and coordinates.
#define BMP_ENDDEAD                     101
#define BMP_ENDLIVE                     102
#define BMP_MIDDEAD                     103
#define BMP_MIDLIVE                     104
#define BMP_SERVERDEAD                  105
#define BMP_SERVERLIVE                  106

#define BITMAP_X     20
#define BITMAP_Y     32
#define NAME_X       60
#define NAME_Y       2
#define REQ_X        250
#define BYTES_X      430
#define NAME_DELTAY  18

// Buffers and pipe symbolic constants.
#define IN_BUF_SIZE        1024
#define OUT_BUF_SIZE       10240
#define TIME_OUT           0

typedef struct 
	{ 
	HANDLE hPipe;
	BOOL   live;
	CHAR   szName[NAME_SIZE];
	DWORD	 dwTotalBytes;
	DWORD	 dwLastRequest;
	DWORD	 dwLastBytes;
	DWORD	 dwTotalRequest;
	} WRTHANDLE;


LONG CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);
VOID          ServerProc (HWND);
VOID          DrawBranch(HDC);
VTHDOC ProcessClientMessage(HWND hWnd, PMYMESSAGEHDR pinMessage, HANDLE hPipe, DWORD ClientIndex);
VOID DrawBitmap(HDC     hDC,HBITMAP hBitmap,SHORT   X,SHORT   Y );
