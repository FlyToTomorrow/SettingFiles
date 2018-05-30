  /*
  |  Outside In Viewer Technology
  |  Sample Application
  |
  |  MDIVIEW
  |  Viewer Technology sample application
  |
  |  Purpose:
  |  To show the simplest possible MDI implementation
  |  of the viewer technology.
  |
  |
  |  Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.
  |
  |  You have a royalty-free right to use, modify, reproduce and
  |  distribute the Sample Applications (and/or any modified version)
  |  in any way you find useful, provided that you agree that Oracle 
  |  has no warranty obligations or liability for any
  |  Sample Application files which are modified.
  */

#define OPTIONSMENU 2  /* position of options menu    */
#define WINDOWMENU  3  /* position of window menu     */
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
#define IDM_FILECLOSE      1002
#define IDM_FILEEXIT       1003
#define IDM_EDITCOPY       1004
#define IDM_EDITSELECTALL  1005
#define IDM_EDITSEARCH     1006
#define IDM_EDITSEARCHNEXT     1007
#define IDM_EDITSEARCHPREV     1008
#define IDM_OPTIONSAUTOSIZE 1009
#define IDM_OPTIONSENCLATIN1        1010
#define IDM_OPTIONSENCLATIN2        1011
#define IDM_OPTIONSENCCENTRALEU1250 1012
#define IDM_OPTIONSENCJAPANESESJIS  1013
#define IDM_OPTIONSENCJAPANESEEUC   1014
#define IDM_OPTIONSENCCHINESEBIG5   1015
#define IDM_OPTIONSENCCHINESEEUC    1016
#define IDM_OPTIONSENCJAPANESEJIS   1017
#define IDM_OPTIONSENCCHINESEGB     1018
#define IDM_OPTIONSENCKOREAN        1019
#define IDM_OPTIONSENCCYRILLIC1251  1020
#define IDM_OPTIONSENCCYRILLICKOI8  1021
#define IDM_OPTIONSARCHIVESINGLE    1022
#define IDM_OPTIONSSETSELECT        1023
#define IDM_OPTIONSFILEACCESSDIALOG 1024
#define IDM_WINDOWTILE     4001
#define IDM_WINDOWCASCADE  4002
#define IDM_WINDOWCLOSEALL 4003
#define IDM_WINDOWICONS    4004
#define IDM_WINDOWCHILD    4100
#define IDM_HELPABOUT      5000
#define HELP_ABOUTTOPBOX   5001
#define HELP_ABOUTBOXCOL1  5002
#define HELP_ABOUTBOXCOL2  5003
#define ID_OK              5004

#define IDD_SETSELECT      6000
#define IDC_EDTSELECT1     6001
#define IDC_EDTSELECT2     6002
#define IDD_FILEACCESSDIALOG 6003
#define IDD_NOTESACCESSDIALOG 6004

/* string constants */
#define IDS_CLIENTTITLE    16
#define IDS_UNTITLED       17
#define IDS_APPNAME        18
#define IDS_ARCSELERR      19
#define IDS_NOTARCHIVE     20
#define IDS_INPUTPASSWORD  21
#define IDS_PASSWORD       22
#define IDS_INPUTNOTESID   23
#define IDS_NOTESID        24
