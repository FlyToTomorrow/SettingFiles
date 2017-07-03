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

#define ANNOEX_HYPERLINK		500

#define IDD_SEARCHDIALOG                102
#define IDC_EDITSEARCHTEXT			    1000

#define IDD_REPLACEDIALOG               103
#define IDD_CUSTOMHILITE					 104
#define IDD_RIGHTSINGLECLICK            105
#define IDD_DOUBLECLICK                 106
#define IDD_LEFTSINGLECLICK             107

#define IDC_STATICSEARCH                2000
#define IDC_REPLACESEARCHTEXT           2001
#define IDC_STATICREPLACE               2002
#define IDC_REPLACETEXT                 2003
#define IDC_MATCHCASE                   2004
#define IDC_FINDNEXT                    2005
#define IDC_REPLACE                     2006
#define IDC_REPLACEALL                  2007

#define IDC_FGCOLOR							2008
#define IDC_BGCOLOR							2009
#define IDC_ULCOLOR							2010
#define IDC_USETHINUNDERLINE				2011
#define IDC_BOLD								2012
#define IDC_ITALIC							2013
#define IDC_UNDERLINE						2014
#define IDC_STRIKEOUT						2015
#define IDC_DBLUNDERLINE					2016
#define IDC_WORDUNDERLINE					2017
#define IDC_DASHUNDERLINE					2018
#define IDC_DOTUNDERLINE					2019
#define IDC_STATIC							2020

#define IDC_BOLD_ON							2021
#define IDC_BOLD_OFF							2022
#define IDC_BOLD_UNCHANGED					2023
#define IDC_ITALIC_ON						2024
#define IDC_ITALIC_OFF						2025
#define IDC_ITALIC_UNCHANGED				2026
#define IDC_UNDERLINE_ON					2027
#define IDC_UNDERLINE_OFF					2028
#define IDC_UNDERLINE_UNCHANGED			2029
#define IDC_STRIKEOUT_ON					2030
#define IDC_STRIKEOUT_OFF					2031
#define IDC_STRIKEOUT_UNCHANGED			2032
#define IDC_DUNDERLINE_ON					2033
#define IDC_DUNDERLINE_OFF					2034
#define IDC_DUNDERLINE_UNCHANGED			2035
#define IDC_WUNDERLINE_ON					2036
#define IDC_WUNDERLINE_OFF					2037
#define IDC_WUNDERLINE_UNCHANGED			2038
#define IDC_DASHUNDERLINE_ON				2039
#define IDC_DASHUNDERLINE_OFF				2040
#define IDC_DASHUNDERLINE_UNCHANGED		2041
#define IDC_DOTUNDERLINE_ON				2042
#define IDC_DOTUNDERLINE_OFF				2043
#define IDC_DOTUNDERLINE_UNCHANGED		2044


#define MENU_FILE_OPEN			100
#define MENU_FILE_CLOSE		101
#define MENU_FILE_PRINT		102
#define MENU_EDIT_COPY			103
#define MENU_FILE_RETURN		104
#define MENU_EDIT_CUT			105
#define MENU_EDIT_SEARCH		106
#define MENU_EDIT_SEARCH_NEXT	107
#define MENU_EDIT_REPLACE       108

 
#define MENU_GOTO_FIRST_YB	200
#define MENU_GOTO_NEXT_YB		201
#define MENU_GOTO_PREV_YB		202
#define MENU_GOTO_LAST_YB		203

#define MENU_GOTO_FIRST_BG	210
#define MENU_GOTO_NEXT_BG		211
#define MENU_GOTO_PREV_BG		212
#define MENU_GOTO_LAST_BG		213

#define MENU_GOTO_FIRST_WG	220
#define MENU_GOTO_NEXT_WG 	221
#define MENU_GOTO_PREV_WG	   222
#define MENU_GOTO_LAST_WG	   223

#define MENU_GOTO_FIRST   	230
#define MENU_GOTO_NEXT 	   231
#define MENU_GOTO_PREV	      232
#define MENU_GOTO_LAST	      233

#define MENU_BOOKMARK_DROP		300
#define MENU_BOOKMARK_FIRST	301
#define MENU_BOOKMARK_LAST		302
#define MENU_BOOKMARK_NEXT		303
#define MENU_BOOKMARK_PREV		304
#define MENU_BOOKMARK_CLEAR		305
#define MENU_BOOKMARK_CLEARALL	306
#define MENU_BOOKMARK_DROPBITMAP		307

#define MENU_POSITION_FIRST				400
#define MENU_POSITION_LAST					401
#define MENU_POSITION_PREV					402
#define MENU_POSITION_NEXT					403
#define MENU_POSITION_STARTDISPLAY		404
#define MENU_POSITION_ENDDISPLAY			405
#define MENU_POSITION_STARTLINE			406
#define MENU_POSITION_ENDLINE				407
#define MENU_POSITION_PREVLINE			408
#define MENU_POSITION_NEXTLINE			409
#define MENU_POSITION_PREVWORD			410
#define MENU_POSITION_NEXTWORD			411
#define MENU_POSITION_NEARTOP				412
#define MENU_POSITION_NEARBOTTOM			413
#define MENU_POSITION_NEARMIDDLE			414

#define MENU_HIGHLIGHT_BY					440
#define MENU_HIGHLIGHT_YR					441
#define MENU_HIGHLIGHT_SEARCHHITS		442
#define MENU_HIGHLIGHT_NEXTHIT			443
#define MENU_HIGHLIGHT_PREVHIT			444
#define MENU_HIGHLIGHT_CLEARHITS			445
#define MENU_HIGHLIGHT_CLEARALLHITS		446
#define MENU_HIGHLIGHT_CUSTOM				447
#define MENU_HIGHLIGHT_SETCUSTOMSTYLE	448


#define MENU_HELP_ABOUT         4200
#define HELP_ABOUTBOXVERINFO    4201

#define IDC_STATICRIGHTSINGLECLICK   600
#define IDC_STATICDOUBLECLICK        601
#define IDC_STATICLEFTSINGLECLICK    602
