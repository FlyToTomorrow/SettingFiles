  /*
  |  Outside In Viewer Technology
  |  Sample Application
  |  
  |
  |  OPTIONS
  |  Viewer Technology sample application
  |
  |  Purpose:
  |  Shows how to set the viewers options and call dialogs programatically
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

#define MENU_FILE_OPEN			100
#define MENU_FILE_CLOSE			101
#define MENU_FILE_ABORT         108
#define MENU_FILE_PRINT			102
#define MENU_FILE_PROPERTIES	109
#define MENU_FILE_PROPERTIESBYID 110
#define MENU_FILE_PROPERTIESBYINDEX 111
#define MENU_FILE_GETPAGENUMBER 112
#define MENU_FILE_INFOMESSAGES	113
#define MENU_EDIT_COPY			103
#define MENU_FILE_PRINTSETUP	104
#define BULLET0                 106
#define BULLET1                 107
#define MENU_HELP_ABOUT         4200
#define HELP_ABOUTBOXVERINFO    4201

#define MENU_OPTIONS_WPDRAFT   201
#define MENU_OPTIONS_WPNORMAL  202
#define MENU_OPTIONS_WPPREVIEW 203
#define MENU_OPTIONS_WPWRAPON  204
#define MENU_OPTIONS_WPWRAPOFF 205
#define MENU_OPTIONS_REORDEROFF		206
#define MENU_OPTIONS_REORDERUNICODELTOR 207
#define MENU_OPTIONS_REORDERUNICODERTOL 208

#define MENU_OPTIONS_PRINTMARGINS10  210
#define MENU_OPTIONS_PRINTMARGINS15  211
#define MENU_OPTIONS_PRINTMARGINS20  212
#define MENU_OPTIONS_HVSCROLL		223
#define MENU_OPTIONS_HSCROLL 224
#define MENU_OPTIONS_VSCROLL 225
#define MENU_OPTIONS_NOSCROLL 226
#define MENU_OPTIONS_VECSHOWBACKGROUND 227
#define MENU_OPTIONS_VECPRINTBACKGROUND 228
#define MENU_OPTIONS_RESOURCEID  229
#define MENU_OPTIONS_UNMAPPABLECHAR  230
#define MENU_OPTIONS_NODECOUNT	231
#define MENU_OPTIONS_SAVENODES  232
#define MENU_OPTIONS_SETARCOUTPUTPATH 234
#define MENU_OPTIONS_NOTIFYREADAHEAD 235

#define MENU_FONTOPTIONS_FONTSCALINGFACTOR 240
#define MENU_FONTOPTIONS_ADDDISPLAYFONTALIAS 241
#define MENU_FONTOPTIONS_REMOVEDISPLAYFONTALIAS 242
#define MENU_FONTOPTIONS_CLEARDISPLAYFONTALIASMAP 243
#define MENU_FONTOPTIONS_SETDISPLAYDEFAULTALIASMAP 244
#define MENU_FONTOPTIONS_ADDPRINTFONTALIAS 245
#define MENU_FONTOPTIONS_REMOVEPRINTFONTALIAS 246
#define MENU_FONTOPTIONS_CLEARPRINTFONTALIASMAP 247
#define MENU_FONTOPTIONS_SETPRINTDEFAULTALIASMAP 248
#define MENU_FONTOPTIONS_DEFAULTINPUTCHARACTERSET 249

#define MENU_FIOPTIONS_FILEID     260
#define MENU_FIOPTIONS_FINORMAL   261
#define MENU_FIOPTIONS_FIEXTENDED 262

#define MENU_SYSTEM_OPTIONS_GRAPHICDRIVERBUG	270
#define MENU_SYSTEM_OPTIONS_PRINTERDRIVERBUG	271
#define MENU_SYSTEM_OPTIONS_READAHEADON			272
#define MENU_SYSTEM_OPTIONS_READAHEADOFF		273
#define MENU_SYSTEM_OPTIONS_JPGOFF				274
#define MENU_SYSTEM_OPTIONS_LZWOFF				275
#define MENU_SYSTEM_OPTIONS_ANTIALIAS           276
#define MENU_SYSTEM_OPTIONS_SBO                 277
#define MENU_SYSTEM_OPTIONS_TIMEZONE            278
#define MENU_SYSTEM_OPTIONS_IOBUFSIZE           279
#define MENU_SYSTEM_OPTIONS_DISABLE_EMAIL		280
#define MENU_SYSTEM_OPTIONS_MEMSMALLEST			281
#define MENU_SYSTEM_OPTIONS_MEMSMALL			282
#define MENU_SYSTEM_OPTIONS_MEMMED				283
#define MENU_SYSTEM_OPTIONS_MEMLARGE			284
#define MENU_SYSTEM_OPTIONS_MEMMAX				285
#define MENU_SYSTEM_OPTIONS_STANDARD_EMAIL_HEADER 286
#define MENU_SYSTEM_OPTIONS_FILEACCESSDIALOG    287
#define MENU_SYSTEM_OPTIONS_PARSEXMPMETADATA    288
#define MENU_SYSTEM_OPTIONS_LOTUSNOTESDIRECTORY 289
#define MENU_SYSTEM_OPTIONS_ISODATES			290
#define MENU_SYSTEM_OPTIONS_STANDARDDATES		291
#define MENU_SYSTEM_OPTIONS_IGNOREPASSWORD		292
#define MENU_SYSTEM_OPTIONS_PDF_FILTER_BIDI		293
#define MENU_SYSTEM_OPTIONS_ALL_EMAIL_HEADER	294
#define MENU_SYSTEM_OPTIONS_NO_EMAIL_HEADER     295
#define MENU_SYSTEM_OPTIONS_CUSTOM_EMAIL_HEADER 296
#define MENU_SYSTEM_OPTIONS_MAINTAINZOOM		297

#define MENU_SYSTEM_OPTIONS_COND_COMMENT_NONE   600
#define MENU_SYSTEM_OPTIONS_COND_COMMENT_IE5    601
#define MENU_SYSTEM_OPTIONS_COND_COMMENT_IE6    602
#define MENU_SYSTEM_OPTIONS_COND_COMMENT_IE7    603
#define MENU_SYSTEM_OPTIONS_COND_COMMENT_IE8    604
#define MENU_SYSTEM_OPTIONS_COND_COMMENT_IE9    605
#define MENU_SYSTEM_OPTIONS_COND_COMMENT_ALL    606

#define STATIC_RESIDHELP        301

#define EDIT_FONTSCALINGFACTOR 401
#define EDIT_RESOURCEID        402

#define STATIC_FILEID          403
#define EDIT_FILEID            404
#define STATIC_FILEIDSTRING    405

#define STATIC_UNMAPPABLECHAR  406
#define EDIT_UNMAPPABLECHAR	   407
#define EDIT_SETPATH           408

#define EDIT_ALIASNAME         501
#define EDIT_ORIGINALNAME      502
#define BUTTON_DELETEALIAS     503
#define COMBO_ORIGINALNAME     504
#define COMBO_ALIASNAME        505
#define SCC_FONTSCALINGDIALOG     506
#define SCC_RESOURCEIDDIALOG      507
#define SCC_FILEIDDIALOG          508
#define SCC_ADDFONTALIASDIALOG    509
#define SCC_REMOVEFONTALIASDIALOG 510
#define SCC_UNMAPPABLECHARDIALOG  511
#define SCC_ARCOUTPUTPATH         512
#define MENU_SYSTEM_TEMPDIR       513

#define SCC_TIMEZONEDIALOG        514
#define BUTTON_USE_SYS_TZ         515
#define EDIT_TIMEZONE             516
#define STATIC_TIMEZONE           517

#define STATIC_CHARSETID          518
#define COMBO_CHARSETID			  519
#define SCC_DEFAULTCHARSETDIALOG  520

#define SCC_IOBUFSIZEDIALOG       521
#define EDIT_READBUFSIZE          522
#define EDIT_MMAPBUFSIZE          523
#define EDIT_TMIBLOCKSIZE         524
#define EDIT_TEMPBUFSIZE          525
#define TEXT_READBUFSIZE          526
#define TEXT_MMAPBUFSIZE          527
#define TEXT_TEMPBUFSIZE          528
#define TEXT_KB1                  529
#define TEXT_KB2                  530
#define TEXT_KB4                  531
#define BUT_DEFAULT               532

#define SCC_PROPERTYDIALOG		550
#define EDIT_PROPERTY			551
#define SCC_PROPERTYDIALOGBYID	552
#define EDIT_PROPERTYID			553
#define	EDIT_PROPERTYNAME		554
#define	EDIT_PROPERTYVALUE		555
#define SCC_PROPERTYDIALOGBYINDEX	556
#define STATIC_PROPID			557
#define STATIC_PROPNAME			558
#define STATIC_PROPVALUE		559
#define SCC_FILEACCESSDIALOG	560
#define STATIC_INPUTPASSWORD    561
#define EDIT_INPUTPASSWORD      562
#define SCC_NOTESACCESSDIALOG	563
#define STATIC_INPUTNOTESID     564
#define EDIT_INPUTNOTESID       565

#define SCC_INFOMSGDIALOG       570
#define EDIT_INFOMSG            571


#define SCC_CUSTOMEMAILHEADERSDIALOG	580
#define SCC_EMAIL_AVAILABLE				581
#define SCC_EMAIL_SELECTED				582
#define SCC_JOURNAL_AVAILABLE			583
#define SCC_JOURNAL_SELECTED			584
#define SCC_CONTACT_AVAILABLE			585
#define SCC_CONTACT_SELECTED			586
#define SCC_TASK_AVAILABLE				587
#define SCC_TASK_SELECTED				588
#define SCC_APPOINTMENT_AVAILABLE		589
#define SCC_APPOINTMENT_SELECTED		590
#define SCC_NOTE_AVAILABLE				591
#define SCC_NOTE_SELECTED				592
#define SCC_DISTROLIST_AVAILABLE		593
#define SCC_DISTROLIST_SELECTED			594
#define SCC_POST_AVAILABLE				595
#define SCC_POST_SELECTED				596


#define SCC_EMAIL_SUBTYPE				600
#define SCC_ADD_HEADER					601
#define SCC_REMOVE_HEADER				602
#define SCC_ADD_CUSTOM					603
#define STATIC_EMAIL_TYPE				604
#define STATIC_EMAIL_SELECTED			605
#define STATIC_EMAIL_AVAILABLE			606
#define SCC_CUSTOM_NAME					607

#define SCC_NONSTANDARDHEADERSDIALOG		610
#define STATIC_NONSTANDARD_HEADER			611
#define STATIC_NONSTANDARD_LABEL			612
#define SCC_NONSTANDARD_HEADER				613
#define SCC_NONSTANDARD_LABEL				614
#define SCC_NONSTANDARD_EMAIL_CHECKBOX		615
#define SCC_NONSTANDARD_JOURNAL_CHECKBOX	616
#define SCC_NONSTANDARD_CONTACT_CHECKBOX	617
#define SCC_NONSTANDARD_TASK_CHECKBOX		618
#define SCC_NONSTANDARD_APPOINTMENT_CHECKBOX		619
#define SCC_NONSTANDARD_NOTE_CHECKBOX		620
#define SCC_NONSTANDARD_DISTROLIST_CHECKBOX		621
#define SCC_NONSTANDARD_POST_CHECKBOX		622
#define SCC_ADD_NONSTANDARD					623
#define SCC_REMOVE_NONSTANDARD				624
#define SCC_NONSTANDARD_LIST				625
#define STATIC_VISIBLE_IN					626
#define SCC_REMOVEALL_NONSTANDARD			627

#define OPT_SHOWSCROLLBAR	WM_USER+1
#define OPT_SETSCROLLRANGE	WM_USER+2

typedef struct REMOVEALIASMAPtag
{
	BOOL bIsDisplay;
	SCCVWFONTALIASMAP sccAliasMap;
} REMOVEALIASMAP, * PREMOVEALIASMAP;

#define CHILD_PROCESS_TIMER_EVENT_ID	34

typedef struct CHILD_PROCESS_DATA_tag
{
	struct CHILD_PROCESS_DATA_tag	*next;
	PROCESS_INFORMATION			pi;
	TCHAR						szDeleteFileName[1024];
} CHILD_PROCESS_DATA, *PCHILD_PROCESS_DATA;