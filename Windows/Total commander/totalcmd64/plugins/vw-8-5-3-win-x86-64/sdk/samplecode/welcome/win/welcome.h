/*
|  Outside In Viewer Technology
|  Sample Application
|  
|
|  WELCOME
|  Viewer Technology sample application
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

#define WINDOWMENU  2  /* position of window menu     */
#define CBWNDEXTRA  0

#ifdef RC_INVOKED
#define ID(id) id
#else
#define ID(id) MAKEINTRESOURCE(id)
#endif

/* resource ID's */
#define IDBLANDFRAME  ID(1)
#define IDBLANDCHILD  ID(2)
#define IDBLANDMENU   ID(3)

/* menu ID's */
#define IDM_FILEOPEN       1001
#define IDM_FILERETURN     1002
#define IDM_FILECLOSE      1003
#define IDM_FILEPRINT      1004
#define IDM_FILEEXIT       1006
#define IDM_EDITCOPY       1010
#define IDM_WINDOWTILE     4001
#define IDM_WINDOWCASCADE  4002
#define IDM_WINDOWCLOSEALL 4003
#define IDM_WINDOWICONS    4004
#define IDM_WINDOWAUTOSIZE 4005
#define IDM_WINDOWCHILD    4100
#define MENU_HELP_ABOUT         4200
#define HELP_ABOUTBOXVERINFO    4201

/* string constants */
#define IDS_CLIENTTITLE    16
#define IDS_UNTITLED       17
#define IDS_APPNAME        18

