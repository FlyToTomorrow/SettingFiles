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

#include <windows.h>

//-----------------For the menu...
#define	IDM_OPEN			1
#define	IDM_BUFSIZE		2
#define	IDM_FIRSTBLOCK	3
#define	IDM_NEXTBLOCK	4
#define	IDM_CLOSE		5
#define	IDM_ABOUT		6
#define HELP_ABOUTBOXVERINFO    4201
#define	IDM_SAVETEXTAS 7
#define IDM_RECORDCOUNT	8
#define IDM_RECORDINFO	9
#define IDM_RECORDOPEN	10
#define IDM_RECORDCLOSE 11

#define 	SIZEDLG        100
#define 	DLG_NUMEDIT    101
#define	ABOUTDLG			200

#define	TXM_NEWDATA		WM_USER+500
