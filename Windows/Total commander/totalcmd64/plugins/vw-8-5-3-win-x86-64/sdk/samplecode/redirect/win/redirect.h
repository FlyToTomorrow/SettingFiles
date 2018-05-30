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

#define MENU_FILE_OPEN		100
#define MENU_FILE_CLOSE		101
#define MENU_FILE_PRINT		102
#define MENU_EDIT_COPY		103
#define MENU_VIEW_INFO		104
#define MENU_HELP_ABOUT         4200
#define HELP_ABOUTBOXVERINFO    4201

#define IDD_SECONDARYINFO	105
#define IDC_SECINFO_LIST	106

#define SECINFOALLOCUNIT	8
 
WIN_ENTRYSC BOOL WIN_ENTRYMOD DisplaySecondaryInfo(HWND, UINT, WPARAM, LPARAM);
