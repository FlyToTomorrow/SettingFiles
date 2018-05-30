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

#define MENU_FILE_OPEN      100
#define MENU_FILE_CLOSE     101
#define MENU_FILE_READAHEAD 102
#define MENU_FILE_PRINT     103
#define MENU_EDIT_COPY      104
#define MENU_HELP_ABOUT         4200
#define HELP_ABOUTBOXVERINFO    4201

typedef struct myfilestructtag
{
    IOBASERFA   baseRFA;
    HANDLE      hThis;
    HANDLE      hPipe;
    VTHDOC      hDoc;
    HANDLE      hRemoteDataBuffer;
    DWORD       dwRemoteDataSize;   /* keep track of the buffer size rather than realloc every time */
    DWORD       dwFlag;
} MYFILE, * PMYFILE;
                   
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

#define MESSAGE_RETOK              0
#define MESSAGE_RETERROR           1

typedef struct myMessageHdrTag
	{
	DWORD  dwType;
	DWORD  dwFlags;
#ifdef WIN64
	DWORD_PTR  dwParm1;
	DWORD_PTR  dwParm2;
#else
	DWORD   dwParm1;
	DWORD   dwParm2;
#endif
	DWORD  dwRet;
	DWORD  dwMoreDataSize;
	} MYMESSAGEHDR, * PMYMESSAGEHDR;

