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

/*
|
|  Defines
|  Defines
|  Defines
|
*/

#define DEFAULTOEMID  TEXT("99")


#define WIN_ENTRYSC __declspec(dllexport)
#define WIN_ENTRYMOD __stdcall
#define WINMAIN_ENTRYMOD APIENTRY

/*
|
|  Includes
|  Includes
|  Includes
|
*/

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>

#include "sccvw.h"
#include "sccfi.h"
#include "sccda.h"

#include "sccca.h"

#include "options.h"
#include "options.pro"
#include "verbuild.h"

#include "charsets.h"



/*
|
|  Globals
|  Globals
|  Globals
|
*/

#define  MAX_PATH_LENGTH     256
#define  MAX_CHARBUF_LENGTH  256

#define VIEWTHISFILE_LAUNCH_STRING TEXT("LaunCheDf0RSCCVW_VIEWTHISFILE")

#define	NUM_LEGIT_CHARSETS 78
DWORD adwCharsets[NUM_LEGIT_CHARSETS] = {
    CS_SYSTEMDEFAULT,
    CS_ASCII,
    CS_BIDI_E0,
    CS_BIDI_OLDCODE,
    CS_BIDI_PC8,
    CS_CHINESE_EUC_TRAD1,
    CS_CHINESE_EUC_TRAD2,
    CS_CHINESE_SIMPLE,
    CS_CHINESE_TRAD1,
    CS_CHINESE_TRAD2,
    CS_DOS_437,
    CS_DOS_737,
    CS_DOS_850,
    CS_DOS_852,
    CS_DOS_855,
    CS_DOS_857,
    CS_DOS_860,
    CS_DOS_861,
    CS_DOS_863,
    CS_DOS_865,
    CS_DOS_866,
    CS_DOS_869,
    CS_EBCDIC_37,
    CS_EBCDIC_273,
    CS_EBCDIC_274,
    CS_EBCDIC_277,
    CS_EBCDIC_278,
    CS_EBCDIC_280,
    CS_EBCDIC_282,
    CS_EBCDIC_284,
    CS_EBCDIC_285,
    CS_EBCDIC_297,
    CS_EBCDIC_500,
    CS_EBCDIC_1026,
    CS_HPROMAN8,
    CS_ISO8859_1,
    CS_ISO8859_2,
    CS_ISO8859_3,
    CS_ISO8859_4,
    CS_ISO8859_5,
    CS_ISO8859_6,
    CS_ISO8859_7,
    CS_ISO8859_8,
    CS_ISO8859_9,
    CS_JAPANESE,
    CS_JAPANESE_EUC,
    CS_JAPANESE_X0201,
    CS_KOREAN,
    CS_MAC_ARABIC,
    CS_MAC_CE,
    CS_MAC_CROATIAN,
    CS_MAC_CYRILLIC,
    CS_MAC_GREEK,
    CS_MAC_HEBREW,
    CS_MAC_ICELANDIC,
    CS_MAC_JAPANESE,
    CS_MAC_ROMAN,
    CS_MAC_ROMANIAN,
    CS_MAC_TURKISH,
    CS_RUSSIAN_KOI8,
    CS_UNICODE,
    CS_UTF7,
    CS_UTF8,
    CS_BIGENDIAN_UNICODE,
    CS_LITTLEENDIAN_UNICODE,
    CS_WINDOWS_874,
    CS_WINDOWS_932,
    CS_WINDOWS_936,
    CS_WINDOWS_949,
    CS_WINDOWS_950,
    CS_WINDOWS_1250,
    CS_WINDOWS_1251,
    CS_WINDOWS_1252,
    CS_WINDOWS_1253,
    CS_WINDOWS_1254,
    CS_WINDOWS_1255,
    CS_WINDOWS_1256,
    CS_WINDOWS_1257
};

TCHAR aszCharsets[NUM_LEGIT_CHARSETS][32] = {
    TEXT("CS_SYSTEMDEFAULT"),
    TEXT("CS_ASCII"),
    TEXT("CS_BIDI_E0"),
    TEXT("CS_BIDI_OLDCODE"),
    TEXT("CS_BIDI_PC8"),
    TEXT("CS_CHINESE_EUC_TRAD1"),
    TEXT("CS_CHINESE_EUC_TRAD2"),
    TEXT("CS_CHINESE_SIMPLE"),
    TEXT("CS_CHINESE_TRAD1"),
    TEXT("CS_CHINESE_TRAD2"),
    TEXT("CS_DOS_437"),
    TEXT("CS_DOS_737"),
    TEXT("CS_DOS_850"),
    TEXT("CS_DOS_852"),
    TEXT("CS_DOS_855"),
    TEXT("CS_DOS_857"),
    TEXT("CS_DOS_860"),
    TEXT("CS_DOS_861"),
    TEXT("CS_DOS_863"),
    TEXT("CS_DOS_865"),
    TEXT("CS_DOS_866"),
    TEXT("CS_DOS_869"),
    TEXT("CS_EBCDIC_37"),
    TEXT("CS_EBCDIC_273"),
    TEXT("CS_EBCDIC_274"),
    TEXT("CS_EBCDIC_277"),
    TEXT("CS_EBCDIC_278"),
    TEXT("CS_EBCDIC_280"),
    TEXT("CS_EBCDIC_282"),
    TEXT("CS_EBCDIC_284"),
    TEXT("CS_EBCDIC_285"),
    TEXT("CS_EBCDIC_297"),
    TEXT("CS_EBCDIC_500"),
    TEXT("CS_EBCDIC_1026"),
    TEXT("CS_HPROMAN8"),
    TEXT("CS_ISO8859_1"),
    TEXT("CS_ISO8859_2"),
    TEXT("CS_ISO8859_3"),
    TEXT("CS_ISO8859_4"),
    TEXT("CS_ISO8859_5"),
    TEXT("CS_ISO8859_6"),
    TEXT("CS_ISO8859_7"),
    TEXT("CS_ISO8859_8"),
    TEXT("CS_ISO8859_9"),
    TEXT("CS_JAPANESE"),
    TEXT("CS_JAPANESE_EUC"),
    TEXT("CS_JAPANESE_X0201"),
    TEXT("CS_KOREAN"),
    TEXT("CS_MAC_ARABIC"),
    TEXT("CS_MAC_CE"),
    TEXT("CS_MAC_CROATIAN"),
    TEXT("CS_MAC_CYRILLIC"),
    TEXT("CS_MAC_GREEK"),
    TEXT("CS_MAC_HEBREW"),
    TEXT("CS_MAC_ICELANDIC"),
    TEXT("CS_MAC_JAPANESE"),
    TEXT("CS_MAC_ROMAN"),
    TEXT("CS_MAC_ROMANIAN"),
    TEXT("CS_MAC_TURKISH"),
    TEXT("CS_RUSSIAN_KOI8"),
    TEXT("CS_UNICODE"),
    TEXT("CS_UTF7"),
    TEXT("CS_UTF8"),
    TEXT("CS_BIGENDIAN_UNICODE"),
    TEXT("CS_LITTEENDIAN_UNICODE"),
    TEXT("CS_WINDOWS_874"),
    TEXT("CS_WINDOWS_932"),
    TEXT("CS_WINDOWS_936"),
    TEXT("CS_WINDOWS_949"),
    TEXT("CS_WINDOWS_950"),
    TEXT("CS_WINDOWS_1250"),
    TEXT("CS_WINDOWS_1251"),
    TEXT("CS_WINDOWS_1252"),
    TEXT("CS_WINDOWS_1253"),
    TEXT("CS_WINDOWS_1254"),
    TEXT("CS_WINDOWS_1255"),
    TEXT("CS_WINDOWS_1256"),
    TEXT("CS_WINDOWS_1257")
};

VTDWORD dwStrId[SCCCA_MAIL_END - SCCCA_MAIL_BASE] =
{
SCCID_MAIL_TO,
SCCID_MAIL_CC,
SCCID_MAIL_BCC,
SCCID_MAIL_SUBJECT,
SCCID_MAIL_MSGFLAG,
SCCID_MAIL_FLAGSTS,
SCCID_MAIL_EXPIRES,
SCCID_MAIL_CATEGORIES,
SCCID_MAIL_IMPORTANCE,
SCCID_MAIL_SENSITIVITY,
SCCID_VCAL_LOCATION,
SCCID_MAIL_FULLNAME,
SCCID_MAIL_JOBTITLE,
SCCID_MAIL_COMPANY,
SCCID_MAIL_EMAIL,
SCCID_MAIL_WEBPAGE,
SCCID_MAIL_BUSINESSPHONE,
SCCID_MAIL_HOMEPHONE,
SCCID_MAIL_FROM,
SCCID_MAIL_ATTACHMENT,
SCCID_MAIL_RTFBODY,
SCCID_MAILFIELD_MESSAGE_DELIVERY_TIME,
SCCID_MAILFIELD_MESSAGE_SIZE,
SCCID_MAILFIELD_LAST_MODIFICATION_TIME,
SCCID_MAIL_NEWSGROUPS,
SCCID_MAILFIELD_PROVIDER_SUBMIT_TIME,
SCCID_MAILFIELD_MESSAGE_CC_ME,
SCCID_MAILFIELD_ALTERNATE_RECIPIENT_ALLOWED,
SCCID_MAIL_SENT,
SCCID_MAILFIELD_CREATION_TIME,
SCCID_MAILFIELD_CONVERSATION_INDEX,
SCCID_MAILFIELD_CONVERSATION_TOPIC,
SCCID_MAILFIELD_MESSAGE_SUBMISSION_ID,
SCCID_MAILFIELD_MESSAGE_CLASS,
SCCID_MAILFIELD_ORIGINATOR_DELIVERY_REPORT_REQUESTED,
SCCID_MAILFIELD_READ_RECEIPT_REQUESTED,
SCCID_MAILFIELD_RCVD_REPRESENTING_ADDRTYPE,
SCCID_MAILFIELD_RCVD_REPRESENTING_EMAIL_ADDRESS,
SCCID_MAILFIELD_RCVD_REPRESENTING_ENTRYID,
SCCID_MAILFIELD_RCVD_REPRESENTING_NAME,
SCCID_MAILFIELD_RCVD_REPRESENTING_SEARCH_KEY,
SCCID_MAILFIELD_RECEIVED_BY_ADDRTYPE,
SCCID_MAILFIELD_RECEIVED_BY_EMAIL_ADDRESS,
SCCID_MAILFIELD_RECEIVED_BY_ENTRYID,
SCCID_MAILFIELD_RECEIVED_BY_NAME,
SCCID_MAILFIELD_RECEIVED_BY_SEARCH_KEY,
SCCID_MAILFIELD_RTF_IN_SYNC,
SCCID_MAILFIELD_RTF_SYNC_BODY_COUNT,
SCCID_MAILFIELD_RTF_SYNC_BODY_CRC,
SCCID_MAILFIELD_RTF_SYNC_BODY_TAG,
SCCID_MAILFIELD_RTF_SYNC_PREFIX_COUNT,
SCCID_MAILFIELD_RTF_SYNC_TRAILING_COUNT,
SCCID_MAILFIELD_SEARCH_KEY,
SCCID_MAILFIELD_SENDER_ADDRTYPE,
SCCID_MAILFIELD_SENDER_EMAIL_ADDRESS,
SCCID_MAILFIELD_SENDER_ENTRYID,
SCCID_MAILFIELD_SENDER_NAME,
SCCID_MAILFIELD_SENDER_SEARCH_KEY,
SCCID_MAILFIELD_SENT_REPRESENTING_ADDRTYPE,
SCCID_MAILFIELD_SENT_REPRESENTING_EMAIL_ADDRESS,
SCCID_MAILFIELD_SENT_REPRESENTING_ENTRYID,
SCCID_MAILFIELD_SENT_REPRESENTING_SEARCH_KEY,
SCCID_MAILFIELD_TRANSPORT_MESSAGE_HEADERS,
SCCID_MAILFIELD_PRIORITY,
SCCID_MAILFIELD_AUTO_FORWARDED,
SCCID_MAILFIELD_DEFERRED_DELIVERY_TIME,
SCCID_MAILFIELD_EXPIRY_TIME,
SCCID_MAILFIELD_LATEST_DELIVERY_TIME,
SCCID_MAILFIELD_RECIPIENT_REASSIGNMENT_PROHIBITED,
SCCID_MAILFIELD_REPLY_TIME,
SCCID_MAILFIELD_REPORT_TAG,
SCCID_MAILFIELD_RESPONSE_REQUESTED,
SCCID_MAILFIELD_REPLY_REQUESTED,
SCCID_MAILFIELD_DELETE_AFTER_SUBMIT,
SCCID_MAILFIELD_MESSAGE_LOCALE_ID,
SCCID_MAILFIELD_CREATOR_NAME,
SCCID_MAILFIELD_CREATOR_ENTRYID,
SCCID_MAILFIELD_LAST_MODIFIER_NAME,
SCCID_MAILFIELD_LAST_MODIFIER_ENTRYID,
SCCID_MAILFIELD_INTERNET_ARTICLE_NUMBER,
SCCID_MAILFIELD_NT_SECURITY_DESCRIPTOR,
SCCID_MAILFIELD_TRUST_SENDER,
SCCID_MAILFIELD_INTERNET_MESSAGE_ID,
SCCID_MAILFIELD_ATTR_HIDDEN,
SCCID_MAILFIELD_ATTR_SYSTEM,
SCCID_MAILFIELD_ATTR_READONLY,
SCCID_MAILFIELD_INTERNET_CPID,
SCCID_MAILFIELD_MESSAGE_CODEPAGE,
SCCID_MAILFIELD_SENDER_FLAGS,
SCCID_MAILFIELD_SENT_REPRESENTING_FLAGS,
SCCID_MAILFIELD_RCVD_BY_FLAGS,
SCCID_MAILFIELD_RCVD_REPRESENTING_FLAGS,
SCCID_MAILFIELD_INET_MAIL_OVERRIDE_FORMAT,
SCCID_MAILFIELD_MSG_EDITOR_FORMAT,
SCCID_MAILFIELD_PROFILE_CONNECT_FLAGS,
SCCID_MAIL_ON_BEHALF_OF,
SCCID_PST_ENTRYID,
SCCID_MAIL_NORMALIZED_SUBJECT,
SCCID_PST_ATTENDEES,
SCCID_MAIL_REQATTENDEE,
SCCID_MAIL_OPTATTENDEE,
SCCID_MAIL_FILEAS,
SCCID_MAIL_DISPLAYAS,
SCCID_MAIL_TITLE,
SCCID_VCARD_SUFFIX,
SCCID_MAIL_NICKNAME,
SCCID_MAIL_PROFESSION,
SCCID_MAIL_DEPARTMENT,
SCCID_MAIL_OFFICE,
SCCID_MAIL_ANNIVERSARY,
SCCID_VCARD_BIRTHDATE,
SCCID_MAIL_ASSISTANTSNAME,
SCCID_MAIL_SPOUSESNAME,
SCCID_MAIL_MANAGERSNAME,
SCCID_MAIL_BUSINESSADDRESS,
SCCID_MAIL_BUSINESSPHONE,
SCCID_MAIL_HOMEADDRESS,
SCCID_MAIL_OTHERADDRESS,
SCCID_MAIL_MOBILEPHONE,
SCCID_MAIL_BUSINESSFAX,
SCCID_MAIL_IMADDRESS,
SCCID_MAIL_INTERNETFREEBUSYADDR,
SCCID_MAIL_REMINDERTOPIC,
SCCID_PST_CONTACTS,
SCCID_MAIL_CALLBACKPHONE,
SCCID_VCARD_FIRST,
SCCID_VCARD_FAMILY,
SCCID_VCARD_TELENUMBER,
SCCID_MAIL_BUSINESSPHONE2,
SCCID_MAIL_RADIOPHONE,
SCCID_MAIL_CARPHONE,
SCCID_MAIL_OTHERPHONE,
SCCID_MAIL_PAGERPHONE,
SCCID_MAIL_OTHERFAX,
SCCID_MAIL_HOMEFAX,
SCCID_MAIL_TELEXPHONE,
SCCID_MAIL_ISDN,
SCCID_MAIL_ASSISTANTPHONE,
SCCID_MAIL_HOMEPHONE2,
SCCID_VCARD_MIDDLE,
SCCID_MAIL_TTYTTDPHONE,
SCCID_MAIL_GENDER,
SCCID_MAIL_PERSONALHOMEPAGE,
SCCID_MAIL_COMPANYPHONE,
SCCID_MAIL_HOMECITY,
SCCID_MAIL_HOMECOUNTRY,
SCCID_MAIL_HOMEPOSTALCODE,
SCCID_MAIL_HOMESTATE,
SCCID_MAIL_HOMESTREET,
SCCID_MAIL_HOMEPOBOX,
SCCID_MAIL_OTHERCITY,
SCCID_MAIL_OTHERCOUNTRY,
SCCID_MAIL_OTHERPOSTALCODE,
SCCID_MAIL_OTHERSTATE,
SCCID_MAIL_OTHERSTREET,
SCCID_MAIL_OTHERPOBOX,
SCCID_MAIL_BUSINESSSTREET,
SCCID_MAIL_BUSINESSCITY,
SCCID_MAIL_BUSINESSSTATE,
SCCID_MAIL_BUSINESSPOSTALCODE,
SCCID_MAIL_BUSINESSCOUNTRY,
SCCID_MAIL_BUSINESSPOBOX,
SCCID_MAIL_DISPLAYAS2,
SCCID_MAIL_EMAIL2,
SCCID_MAIL_DISPLAYAS3,
SCCID_MAIL_EMAIL3,
SCCID_VCAL_DTSTART,
SCCID_VCAL_DTEND,
SCCID_MSPROJECT_START,
SCCID_MSPROJECT_DURATION,
SCCID_MSPROJECT_ACCRUEATTYPE1,
SCCID_PST_ENTRY_TYPE,
SCCID_PST_STATUS,
SCCID_PST_PERCENT_COMPLETE,
SCCID_PST_START_DATE,
SCCID_PST_DUE_DATE,
SCCID_PST_COMPLETED,
SCCID_MSPROJECT_ACTUALWORK,
SCCID_PST_TOTAL_WORK,
SCCID_PST_OWNER,
SCCID_PST_BILLING,
SCCID_PST_MILEAGE,
SCCID_MAILFIELD_PROOF_OF_DELIVERY_REQUESTED,
SCCID_MAILFIELD_PROOF_OF_SUBMISSION_REQUESTED,
SCCID_MAILFIELD_IN_REPLY_TO_ID,
SCCID_MAIL_TASK_COMPLETE,
SCCID_MAIL_APPOINTMENT_DURATION,
SCCID_MAIL_RECURRENCE_TYPE,
SCCID_MAIL_RECURRENCE_RANGE,
SCCID_MAIL_ORGANIZER,
SCCID_MAIL_CALENDAR_EXCEPTION
};

HANDLE		hInst;             /* Handle of the current instance */
HWND        hMainWnd;          /* Handle to top level window */
HWND        hViewWnd;          /* Handle to the view window */
TCHAR       szExePath[MAX_PATH_LENGTH];    /* Path to this executable */
HANDLE      hViewerLibrary;    /* Handle to SCCVW.DLL */
TCHAR       szArcOutputPath[MAX_PATH_LENGTH];  /* Directory to where the Extracted files are saved */
TCHAR        szTempDir[MAX_PATH_LENGTH];  /* Directory where the temp files are created */
BOOL        bJPGOff, bLZWOff, bEmailOff, bIgnorePassword,bPDF_Process_Bidi;
BOOL        bHandleReadAheadMsg;
BOOL        bAntiAlias;
BOOL        bMaintainZoom;
BOOL        bNewSBO;
int         nOutputPathMode;
LONG        lTimeZone;
TCHAR       szCharBuffer[MAX_CHARBUF_LENGTH];
BOOL        bFileAccessOn = FALSE;
BOOL        bParseXMPOn = FALSE;
BOOL        rad = FALSE;    /* Read Ahead Done */
BOOL        bAbort = FALSE;
TCHAR        szLotusNotesDirectory[MAX_PATH_LENGTH];
DWORD       dwHtmlCondCommentFlags = 0;
BOOL		bDisableOpen = FALSE;

/*
|
|  Routines
|  Routines
|  Routines
|
*/

void ClearBusy(void);
VOID InitMenuChecks(HWND hWnd);

VTDWORD DAStatCallback( VTDWORD dwStatus, VTDWORD dwStatFlags, VTSYSVAL pData, VTSYSVAL pUserData);

VOID UpdateComboBoxWithAliasNames( HWND hAliasComboWnd, VTLPWORD lpwAliasList );

static HINSTANCE SCCLoadViewerDLL(TCHAR* lpViewerPath);
static VTVOID SCCGetExePath(HINSTANCE hInst,TCHAR *lpPath,VTWORD wPathLen);

VTVOID HandleCondCommentOption(HWND hWnd, VTDWORD dwFlagVal, VTDWORD dwGuiValue);
VTVOID HandleInfoMessage(HWND hWnd, VTDWORD infomessage);
VTVOID ClearInfoMessages();
VTVOID UpdateZoom(HWND hWnd);

WIN_ENTRYSC LRESULT WIN_ENTRYMOD SccVwViewWndProc(HWND, UINT, WPARAM, LPARAM);
WIN_ENTRYSC BOOL WIN_ENTRYMOD FileAccessDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
WIN_ENTRYSC BOOL WIN_ENTRYMOD NotesAccessDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


#ifndef UNUSED
#define UNUSED(x) ((x) = (x))
#endif

TCHAR	*szCmdLine = NULL;
TCHAR   szDisplayName[SCCVW_DISPLAYNAMEMAX] = TEXT("");

int WINMAIN_ENTRYMOD WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG  locMsg;

	UNUSED(lpCmdLine);
	UNUSED(nCmdShow);	


	/* Process the command line */
	{
		int argc;
		LPWSTR *argv;
		argv = CommandLineToArgvW(GetCommandLineW(),&argc);	

		if (argc > 1)
		{
			szCmdLine = (TCHAR*)malloc(sizeof(TCHAR) * (lstrlen(argv[1])+1));
			lstrcpy(szCmdLine,argv[1]);
	
			if (argc > 2)
			{				
				if (!lstrcmp(argv[2],VIEWTHISFILE_LAUNCH_STRING))
					bDisableOpen = TRUE;
	
				if (argc > 3)
				{
					lstrcpyn(szDisplayName,argv[3],SCCVW_DISPLAYNAMEMAX);
					szDisplayName[SCCVW_DISPLAYNAMEMAX-1]=0;
				}
			}
		}   
	}

    /*
    | Register window class if necessary
    */

    if (!hPrevInstance)
    {
        WNDCLASSW WndClassw;
        WndClassw.style = 0;
        WndClassw.lpfnWndProc = SccWndProc;
        WndClassw.cbClsExtra = 0;
        WndClassw.cbWndExtra = 0;
        WndClassw.hInstance = hInstance;
        WndClassw.hIcon = LoadIcon(hInstance, TEXT("SCC_ICON"));
        WndClassw.hCursor = LoadCursor(NULL,IDC_ARROW);
        WndClassw.hbrBackground = GetStockObject(LTGRAY_BRUSH);
        WndClassw.lpszMenuName = TEXT ("SCC_MENU");
        WndClassw.lpszClassName = TEXT("SCC_MAIN");

        if (!RegisterClass(&WndClassw)) return(0);
    }

    /*
    | Save instance in global
    */

    hInst = hInstance;


    /*  Initialization of options */
    GetTempPath( (DWORD)MAX_PATH_LENGTH, szArcOutputPath);
    bHandleReadAheadMsg = FALSE;
    bJPGOff = bLZWOff = bEmailOff = bIgnorePassword = FALSE;
    bAntiAlias = FALSE;
	bMaintainZoom = FALSE;
    bNewSBO = FALSE;

    /*
    | Create main window
    */

    hMainWnd = CreateWindow(
        TEXT("SCC_MAIN"),                      /* window class    */
        TEXT("OIVT Options Sample"),                   /* window name      */
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN ,   /* window type     */
        CW_USEDEFAULT,                           /* x position       */
        CW_USEDEFAULT,                           /* y position     */
        CW_USEDEFAULT,                           /* width           */
        CW_USEDEFAULT,                           /* height        */
        NULL,                                    /* parent handle   */
        NULL,                                    /* menu or child ID*/
        hInstance,                               /* instance        */
        NULL);                                   /* additional info */
    /*
    |  Confirm window was created
    */

    if (!hMainWnd)
        return (0);

    ShowWindow(hMainWnd,SW_SHOW);
    UpdateWindow(hMainWnd);

    /*
    |  GetMessage loop
    */

    while (GetMessage(&locMsg,NULL,0,0))
    {
        TranslateMessage(&locMsg);
        DispatchMessage(&locMsg);
    }
	close_child_processes(TRUE);

    return (int)(locMsg.wParam);     /* Returns the value from PostQuitMessage */
}


int g_vScrollMin = 0;
int g_vScrollMax = 0;

WIN_ENTRYSC LRESULT WIN_ENTRYMOD SccWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    VTWORD locId;
    VTWORD locEvent;
    HWND  locCtrl;


    switch (message)
    {
	case WM_TIMER:
		/*
			Periodically check any other windows that we've opened as a result of a SCCVW_VIEWTHISFILE message, 
			so that if they requested deleting the file on close, we can accomodate that.
		*/
		if (CHILD_PROCESS_TIMER_EVENT_ID == wParam)
			check_active_child_processes(hWnd);
		break;

    case WM_DESTROY:
        DoDestroy(hWnd);		
        PostQuitMessage(0);
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

	case WM_PAINT:
		{
		LRESULT ret;

		ret = (DefWindowProc(hWnd, message, wParam, lParam));
		UpdateZoom(hWnd);
		return ret;
		}
		break;


    case WM_CREATE:

        DoCreate(hWnd);
        InitMenuChecks(hWnd);

		if (bDisableOpen)
		{
			/* 
				this instance of Options was created in response to a SCCVW_VIEWTHISFILE message, so
				do not permit opening different files.  We'll also close the window if the menu close_file is used.
			*/
			EnableMenuItem(GetMenu(hWnd),MENU_FILE_OPEN,MF_DISABLED|MF_GRAYED);
		}


        break;

		case SCCVW_VIEWTHISFILE:
        {         
            TCHAR szCmd[1024];
			TCHAR szExe[1024];
			TCHAR szDisplay[256]=TEXT("");
            PROCESS_INFORMATION pi;
            STARTUPINFO si;            
            SCCVWVIEWTHISFILE80 *f80 = (SCCVWVIEWTHISFILE80 *)lParam; 
			
			GetModuleFileName(hInst, szExe, 1024);

			if (f80->sViewFile.bUseDisplayName)
				wsprintf(szDisplay,TEXT(" \"%s\""),f80->sViewFile.szDisplayName);

			if (f80->sViewFile.dwSpecType == IOTYPE_UNICODEPATH)			
				wsprintf(szCmd,TEXT("\"%s\" \"%s\" %s%s"),szExe,(LPSTR)f80->sViewFile.pSpec,VIEWTHISFILE_LAUNCH_STRING,szDisplay);
			else
				wsprintf(szCmd,TEXT("\"%s\" \"%S\" %s%s"),szExe,(LPSTR)f80->sViewFile.pSpec,VIEWTHISFILE_LAUNCH_STRING,szDisplay);

            memset(&si,0,sizeof(STARTUPINFO));
            si.cb = sizeof(STARTUPINFO);
            if (CreateProcess(szExe,
                szCmd,
                NULL, NULL, FALSE,
                0,
                NULL, NULL, 
                &si,
                &pi))
            {		
				/* 
					Need to keep track of this, so that a) if this window is destroyed, it can clean up its children, and
					b) when the children are done, we can delete any 'delete on close' files that were in use.
				*/

				add_process_to_active_list(&pi,&(f80->sViewFile),hWnd);               
								
				return SCCVWERR_MESSAGEHANDLED;
            }

            break;
        }


    case WM_SIZE:

        if (wParam != SIZEICONIC)
            DoSize(hWnd,LOWORD(lParam),HIWORD(lParam));
        break;

    case WM_VSCROLL:

        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            SendMessage(hViewWnd,SCCVW_VSCROLL,SCCSB_LINEUP,0);
            break;
        case SB_LINEDOWN:
            SendMessage(hViewWnd,SCCVW_VSCROLL,SCCSB_LINEDOWN,0);
            break;
        case SB_PAGEUP:
            SendMessage(hViewWnd,SCCVW_VSCROLL,SCCSB_PAGEUP,0);
            break;
        case SB_PAGEDOWN:
            SendMessage(hViewWnd,SCCVW_VSCROLL,SCCSB_PAGEDOWN,0);
            break;
        case SB_THUMBPOSITION:
            SendMessage(hViewWnd,SCCVW_VSCROLL,SCCSB_POSITION,HIWORD(wParam));
            break;
        }

        break;

    case WM_HSCROLL:

        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            SendMessage(hViewWnd,SCCVW_HSCROLL,SCCSB_LINEUP,0);
            break;
        case SB_LINEDOWN:
            SendMessage(hViewWnd,SCCVW_HSCROLL,SCCSB_LINEDOWN,0);
            break;
        case SB_PAGEUP:
            SendMessage(hViewWnd,SCCVW_HSCROLL,SCCSB_PAGEUP,0);
            break;
        case SB_PAGEDOWN:
            SendMessage(hViewWnd,SCCVW_HSCROLL,SCCSB_PAGEDOWN,0);
            break;
        case SB_THUMBPOSITION:
            SendMessage(hViewWnd,SCCVW_HSCROLL,SCCSB_POSITION,HIWORD(wParam));
            break;
        }

        break;


    case WM_PALETTECHANGED:
    case WM_SYSCOLORCHANGE:
    case WM_QUERYNEWPALETTE:
    case WM_SETFOCUS:

        if (IsWindow(hViewWnd))
            return(SendMessage(hViewWnd,message,wParam,lParam));
        break;

    case OPT_SHOWSCROLLBAR:
        ShowScrollBar(hWnd,(int)wParam,(lParam == SCCSB_ENABLED));
        break;

    case OPT_SETSCROLLRANGE:
		if (wParam == SB_VERT)
			SetScrollRange(hWnd,(int)wParam,g_vScrollMin,g_vScrollMax,TRUE);
		else
			SetScrollRange(hWnd,(int)wParam,(int)LOWORD(lParam),(int)HIWORD(lParam),TRUE);		
        break;

    case SCCVW_FILECHANGE:

        DoFileChange(hWnd);
        break;

    case SCCVW_SETVSCROLLPOSITION:
        SetScrollPos(hWnd,SB_VERT,(int)lParam,TRUE);
        break;

    case SCCVW_SETVSCROLLPAGESIZE:
		{
			SCROLLINFO ScrollInfo;

			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_PAGE;
			ScrollInfo.nPage = (int)lParam;

			ScrollInfo.nMin = 0;
			ScrollInfo.nMax = 0;
			ScrollInfo.nPos = 0;
			ScrollInfo.nTrackPos = 0;

			SetScrollInfo(hWnd,SB_VERT,&ScrollInfo,TRUE);
		}
        break;

    /*
     | Post yourself a message to set the scrollbar range instead of
     | calling SetScrollRange directly to prevent any inadvertent messages
     | from being sent to the Viewer window, such as WM_SIZE.
     */

    case SCCVW_SETVSCROLLRANGEMIN:
		g_vScrollMin = (int)lParam;
        PostMessage(hWnd,OPT_SETSCROLLRANGE,SB_VERT,0);
        break;

	case SCCVW_SETVSCROLLRANGEMAX:
		g_vScrollMax = (int)lParam;
        PostMessage(hWnd,OPT_SETSCROLLRANGE,SB_VERT,0);
        break;

    case SCCVW_SETVSCROLLSTATE:

        /*
        | Disable and Enable the scroll bars as the scroll state changes
        */

        if (lParam == SCCSB_DISABLED)
            EnableScrollBar(hWnd,SB_VERT,ESB_DISABLE_BOTH);
        else
            EnableScrollBar(hWnd,SB_VERT,ESB_ENABLE_BOTH);
        break;

    case SCCVW_SETHSCROLLPOSITION:
        SetScrollPos(hWnd,SB_HORZ,(int)lParam,TRUE);
        break;

    case SCCVW_SETHSCROLLPAGESIZE:
		{
			SCROLLINFO ScrollInfo;

			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_PAGE;
			ScrollInfo.nPage = (int)lParam;

			ScrollInfo.nMin = 0;
			ScrollInfo.nMax = 0;
			ScrollInfo.nPos = 0;
			ScrollInfo.nTrackPos = 0;

			SetScrollInfo(hWnd,SB_HORZ,&ScrollInfo,TRUE);
		}
        break;

    case SCCVW_SETHSCROLLRANGE:

        /*
        | Post yourself a message to set the scrollbar range instead of
        | calling SetScrollRange directly to prevent any inadvertent messages
        | from being sent to the Viewer window, such as WM_SIZE.
        */

        PostMessage(hWnd,OPT_SETSCROLLRANGE,SB_HORZ,lParam);
        break;

    case SCCVW_SETHSCROLLSTATE:

        /*
        | Disable and Enable the scroll bars as the scroll state changes.
        */

        if (lParam == SCCSB_DISABLED)
            EnableScrollBar(hWnd,SB_HORZ,ESB_DISABLE_BOTH);
        else
            EnableScrollBar(hWnd,SB_HORZ,ESB_ENABLE_BOTH);
        break;

    case SCCVW_DISPLAYCHANGE:
        UpdateZoom(hWnd);
		break;

	case SCCVW_OPTIONCHANGE:
			/*
			|  An example of how to track and react to changing options.			 
			*/
		switch (lParam)
		{
		case SCCID_BMPZOOM:
		case SCCID_VECZOOM:
			UpdateZoom(hWnd);
			break;
		}
		break;

	case SCCVW_INFOMESSAGE:
		HandleInfoMessage(hWnd,(VTDWORD)lParam);
		//MessageBox(hWnd,TEXT("Info Message"),TEXT("Options"),MB_OK);
		break;


    case SCCVW_READAHEADDONE:
        ClearBusy();
        if (bHandleReadAheadMsg)
        {
            MessageBox(hWnd,TEXT("Read Ahead complete"), TEXT("OPTIONS"),MB_OK);
        }
        break;

    case SCCVW_DOHELP:

        switch (lParam)
        {
        case SCCID_PRINTOPTIONSDIALOG:
        case SCCID_PRINTDIALOG:
        case SCCID_SELECTFONTDIALOG:
        case SCCID_DISPLAYOPTIONSDIALOG:
        case SCCID_DISPLAYMOREDIALOG:
        case SCCID_PRINTMOREDIALOG:
        case SCCID_PRINTSETUPDIALOG:
        case SCCID_CLIPBOARDOPTIONSDIALOG:
        case SCCID_CLIPBOARDMOREDIALOG:
            MessageBox(hViewWnd,TEXT("Extra information is not available"),TEXT("Help"),MB_OK);
        default:
            break;
        }

        break;

    case SCCVW_GETFILEACCESSDATA:
        if (bFileAccessOn && lParam)
        {
            VTDWORD  i;  /* generic counter */

            PSCCVWFILEACCESSDATA  pFileAccessData = (PSCCVWFILEACCESSDATA)lParam;  /* pointer to SCCVWFILEACCESSDATA structure */

            VTWORD   wDialogRet = 0;         /* catch dialog return value */
            VTDWORD  dwRet = SCCERR_CANCEL;  /* return value              */

            memset(szCharBuffer, 0, MAX_CHARBUF_LENGTH);

            switch (pFileAccessData->dwRequestId)
            {
            case OIT_FILEACCESS_PASSWORD:
                /* Request a password from the user */
                wDialogRet = (VTWORD)DialogBox(hInst, MAKEINTRESOURCE(SCC_FILEACCESSDIALOG), hViewWnd, (DLGPROC)FileAccessDialogProc);
                break;
            case OIT_FILEACCESS_NOTESID:
                /* Request a path to a Lotus Notes ID file from the user */
                wDialogRet = (VTWORD)DialogBox(hInst, MAKEINTRESOURCE(SCC_NOTESACCESSDIALOG), hViewWnd, (DLGPROC)NotesAccessDialogProc);
                break;
            default:
                break;
            }

            if (wDialogRet)
            {
                VTLPWORD  lpwszBuf = (VTLPWORD)pFileAccessData->pReturnData;
                DWORD     dwReturnDataSize = pFileAccessData->dwReturnDataSize;

                /* Copy the password into the request data buffer
                *
                * NOTE: The return data buffer for the OIT_FILEACCESS_PASSWORD
                *       id is a wide-character buffer, so make sure to copy the
                *       characters correctly.
                */
                for (i = 0; (i < wDialogRet) && (i < dwReturnDataSize / 2); i++)
                    lpwszBuf[i] = (VTWORD)szCharBuffer[i];

                /* Clear the character buffer */
                memset(szCharBuffer, 0, MAX_CHARBUF_LENGTH);

                dwRet = SCCERR_OK;
            }

            /* Flag that we have processed this request */
            pFileAccessData->bProcessed = TRUE;

            return (dwRet);
        }
        else
            return (SCCERR_NONEFOUND);
        break;

    case WM_COMMAND:
        locId    = LOWORD(wParam);
        locEvent = HIWORD(wParam);
        locCtrl  = (HWND) lParam;

        if (locCtrl == 0) //Menu
        {
            switch(locId)
            {
            case MENU_FILE_OPEN:
                DoOpenFile(hWnd);
                break;

            case MENU_FILE_CLOSE:
				/* 
				   If this instance of Options is running as a result of a previous SCCVW_VIEWTHISFILE message, 
				   we'll close it on the MENU_FILE_CLOSE as well.
				*/
				if (bDisableOpen)
				{ 
					DestroyWindow(hWnd);
					break;
				}
                DoCloseFile(hWnd);
                break;

            case MENU_FILE_ABORT:
                if (!rad)
                    bAbort = TRUE;
                break;

            case MENU_FILE_PRINT:
                SendMessage(hViewWnd,SCCVW_PRINT,0,0);
                break;

            case MENU_FILE_PRINTSETUP:
                SendMessage(hViewWnd,SCCVW_DODIALOG,0,SCCID_PRINTSETUPDIALOG);
                break;

			case MENU_FILE_INFOMESSAGES:
				DialogBoxParam(hInst, MAKEINTRESOURCE(SCC_INFOMSGDIALOG), hViewWnd, (DLGPROC)InfoMsgDlgProc, 0);
				break;

            case MENU_FILE_PROPERTIES:
                DialogBoxParam(hInst, MAKEINTRESOURCE(SCC_PROPERTYDIALOG), hViewWnd, (DLGPROC)PropertyDlgProc, 0);
                break;

            case MENU_FILE_PROPERTIESBYID:
                DialogBoxParam(hInst, MAKEINTRESOURCE(SCC_PROPERTYDIALOGBYID), hViewWnd, (DLGPROC)PropertyDlgByIDProc, 0);
                break;

            case MENU_FILE_PROPERTIESBYINDEX:
                DialogBoxParam(hInst, MAKEINTRESOURCE(SCC_PROPERTYDIALOGBYINDEX), hViewWnd, (DLGPROC)PropertyDlgByIndexProc, 0);
                break;

            case MENU_FILE_GETPAGENUMBER:
                {
                DWORD pageNumber = (DWORD)SendMessage(hViewWnd,SCCVW_GETCURRENTPAGENUMBER,0,0L);
                if( pageNumber == 0 )
                    MessageBox( hWnd, TEXT("Unable to determine the current page number"), TEXT("SCCVW_GETCURRENTPAGENUMBER"), MB_OK|MB_ICONERROR|MB_APPLMODAL );
                else
                    {
                    TCHAR buf[128];
                    wsprintf( buf, TEXT("Currently viewing page %d"), pageNumber );
                    MessageBox( hWnd, buf, TEXT("SCCVW_GETCURRENTPAGENUMBER"), MB_OK|MB_APPLMODAL );
                    }
                }
                break;

            case MENU_EDIT_COPY:
                SendMessage(hViewWnd,SCCVW_COPYTOCLIP,0,0L);
                break;

            case MENU_OPTIONS_WPDRAFT:
            case MENU_OPTIONS_WPNORMAL:
            case MENU_OPTIONS_WPPREVIEW:
                {
                    SCCVWOPTIONSPEC40  locOptionSpec;
                    VTDWORD          locMode;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_WPDISPLAYMODE;

                    switch (locId)
                    {
                    case MENU_OPTIONS_WPDRAFT:
                        locMode = SCCVW_WPMODE_DRAFT;
                        break;

                    case MENU_OPTIONS_WPNORMAL:
                        locMode = SCCVW_WPMODE_NORMAL;
                        break;

                    case MENU_OPTIONS_WPPREVIEW:
                        locMode = SCCVW_WPMODE_PREVIEW;
                        break;
                    }

                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = (VTVOID *)&locMode;

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_OPTIONS_WPWRAPON:
            case MENU_OPTIONS_WPWRAPOFF:
                {
                    SCCVWOPTIONSPEC40  locOptionSpec;
                    VTDWORD          locMode;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_WPWRAPTOWINDOW;

                    if (locId == MENU_OPTIONS_WPWRAPON)
                    {
                        locMode = TRUE;
                        CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_WPWRAPON,  MF_CHECKED);
                        CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_WPWRAPOFF, MF_UNCHECKED);
                    }
                    else
                    {
                        locMode = FALSE;
                        CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_WPWRAPON,  MF_UNCHECKED);
                        CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_WPWRAPOFF, MF_CHECKED);
                    }

                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = (VTVOID *)&locMode;

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_OPTIONS_REORDEROFF:				
            case MENU_OPTIONS_REORDERUNICODELTOR:
            case MENU_OPTIONS_REORDERUNICODERTOL:
                {
                    SCCVWOPTIONSPEC40  locOptionSpec;
                    VTDWORD          locMode;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);

                    CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_REORDEROFF,		   MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_REORDERUNICODELTOR, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_REORDERUNICODERTOL, MF_UNCHECKED);					

                    locOptionSpec.dwId = SCCID_REORDERMETHOD;

                    switch (locId)
                    {
                    default:
                    case MENU_OPTIONS_REORDEROFF:		
                        CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_REORDEROFF,    MF_CHECKED);
                        locMode = SCCUT_REORDER_UNICODE_OFF;
                        break;

                    case MENU_OPTIONS_REORDERUNICODELTOR:
                        CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_REORDERUNICODELTOR, MF_CHECKED);
                        locMode = SCCUT_REORDER_UNICODE_LTOR;
                        break;

                    case MENU_OPTIONS_REORDERUNICODERTOL:
                        CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_REORDERUNICODERTOL, MF_CHECKED);
                        locMode = SCCUT_REORDER_UNICODE_RTOL;
                        break;
                    }

                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = (VTVOID *)&locMode;


                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_OPTIONS_PRINTMARGINS10:
            case MENU_OPTIONS_PRINTMARGINS15:
            case MENU_OPTIONS_PRINTMARGINS20:
                {
                    SCCVWOPTIONSPEC40  locOptionSpec;
                    SCCVWPRINTMARGINS  locMargins;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_DEFAULTPRINTMARGINS;

                    switch (locId)
                    {
                    case MENU_OPTIONS_PRINTMARGINS10:
                        locMargins.dwTop = 1440;
                        locMargins.dwBottom = 1440;
                        locMargins.dwLeft = 1440;
                        locMargins.dwRight = 1440;
                        break;

                    case MENU_OPTIONS_PRINTMARGINS15:
                        locMargins.dwTop = 2160;
                        locMargins.dwBottom = 2160;
                        locMargins.dwLeft = 2160;
                        locMargins.dwRight = 2160;
                        break;

                    case MENU_OPTIONS_PRINTMARGINS20:
                        locMargins.dwTop = 2880;
                        locMargins.dwBottom = 2880;
                        locMargins.dwLeft = 2880;
                        locMargins.dwRight = 2880;
                        break;
                    }

                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = (VTVOID *)&locMargins;

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_OPTIONS_HVSCROLL:
            case MENU_OPTIONS_HSCROLL:
            case MENU_OPTIONS_VSCROLL:
            case MENU_OPTIONS_NOSCROLL:
                {
                    SCCVWOPTIONSPEC40	locOptionSpec;
                    VTDWORD				locScrollFlags;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_SCROLLFLAGS;

                    switch (locId)
                    {
                    case MENU_OPTIONS_HVSCROLL:
                        locScrollFlags = SCCVW_HSCROLL_ALWAYS | SCCVW_VSCROLL_ALWAYS;
                        break;

                    case MENU_OPTIONS_HSCROLL:
                        locScrollFlags = SCCVW_HSCROLL_ALWAYS | SCCVW_VSCROLL_NEVER;
                        break;

                    case MENU_OPTIONS_VSCROLL:
                        locScrollFlags = SCCVW_HSCROLL_NEVER | SCCVW_VSCROLL_ALWAYS;
                        break;

                    case MENU_OPTIONS_NOSCROLL:
                        locScrollFlags = SCCVW_HSCROLL_NEVER | SCCVW_VSCROLL_NEVER;
                        break;
                    }

                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = (VTVOID *)&locScrollFlags;

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_OPTIONS_RESOURCEID:
                {
                    SCCVWOPTIONSPEC40	locOptionSpec;
                    SCCVWRESOURCEID		SCCResID;
                    VTLONG				lRet;

                    SCCResID.dwSize = sizeof (SCCVWRESOURCEID);
                    SCCResID.szResourceID[0] = 0;

                    /* get current ID for the resource library */
                    /* the default is "" and OIVT uses SCCLO.DLL */
                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_RESOURCELIBRARYID;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &SCCResID;
                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

                    /* prompt user for a resource library ID */
                    lRet = (VTLONG)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_RESOURCEIDDIALOG),
                        hViewWnd,
                        (DLGPROC)ResourceIDProc,
                        (LPARAM)(VOID *)&SCCResID);

                    /*  call set option to set the new value, if value has changed */
                    if (lRet == 1)
                    {
                        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                    }
                }
                break;

            case MENU_FIOPTIONS_FILEID:
                DialogBox(hInst, MAKEINTRESOURCE(SCC_FILEIDDIALOG), hViewWnd, (DLGPROC)FileIDProc);
                break;

            case MENU_FIOPTIONS_FINORMAL:
            case MENU_FIOPTIONS_FIEXTENDED:
                {
                    SCCVWOPTIONSPEC40  locOption;
                    VTDWORD          locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    if (locId == MENU_FIOPTIONS_FINORMAL)
                    {
                        locFlags = SCCVW_FI_NORMAL;
                        CheckMenuItem( GetMenu(hWnd), MENU_FIOPTIONS_FINORMAL,  MF_CHECKED);
                        CheckMenuItem( GetMenu(hWnd), MENU_FIOPTIONS_FIEXTENDED, MF_UNCHECKED);
                    }
                    else
                    {
                        locFlags = SCCVW_FI_EXTENDEDTEST;
                        CheckMenuItem( GetMenu(hWnd), MENU_FIOPTIONS_FINORMAL,   MF_UNCHECKED);
                        CheckMenuItem( GetMenu(hWnd), MENU_FIOPTIONS_FIEXTENDED, MF_CHECKED);
                    }
                    locOption.dwId = SCCID_FIFLAGS;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

            case MENU_SYSTEM_OPTIONS_READAHEADON:
            case MENU_SYSTEM_OPTIONS_READAHEADOFF:
                {
                    SCCVWOPTIONSPEC40  locOption;
                    VTDWORD          locFlags;

                    locFlags = 0;
                    locOption.dwId = SCCID_SYSTEMFLAGS;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;
                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);

                    if (locId == MENU_SYSTEM_OPTIONS_READAHEADOFF)
                    {
                        locFlags &= (~SCCVW_SYSTEM_NOREADAHEAD);
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_READAHEADOFF,  MF_CHECKED);
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_READAHEADON, MF_UNCHECKED);
                    }
                    else
                    {
                        locFlags |= SCCVW_SYSTEM_NOREADAHEAD;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_READAHEADOFF,  MF_UNCHECKED);
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_READAHEADON,	 MF_CHECKED);
                    }

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

            case MENU_SYSTEM_OPTIONS_GRAPHICDRIVERBUG:
            case MENU_SYSTEM_OPTIONS_PRINTERDRIVERBUG:
                {
                    SCCVWOPTIONSPEC40  locOption;
                    VTDWORD          locFlags;
                    MENUITEMINFO      MenuInfo;

                    MenuInfo.cbSize = sizeof(MENUITEMINFO);
                    MenuInfo.fMask = MIIM_STATE;

                    locFlags = 0;
                    locOption.dwId = SCCID_SYSTEMFLAGS;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;
                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);

                    if (locId == MENU_SYSTEM_OPTIONS_GRAPHICDRIVERBUG)
                    {
                        if (locFlags & SCCVW_SYSTEM_TEXTOUTDISPLAYDRIVERBUG)
                        {
                            locFlags &= (~SCCVW_SYSTEM_TEXTOUTDISPLAYDRIVERBUG);
                            CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_GRAPHICDRIVERBUG,  MF_UNCHECKED);						
                        }
                        else
                        {
                            locFlags |= SCCVW_SYSTEM_TEXTOUTDISPLAYDRIVERBUG;
                            CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_GRAPHICDRIVERBUG,  MF_CHECKED);						
                        }

                        MenuInfo.fState = ((locFlags & SCCVW_SYSTEM_TEXTOUTDISPLAYDRIVERBUG)==0)?MFS_UNCHECKED:MFS_CHECKED;
                        SetMenuItemInfo(GetMenu(hMainWnd),locId,FALSE,&MenuInfo);
                    }
                    else
                    {
                        if (locFlags & SCCVW_SYSTEM_TEXTOUTPRINTERDRIVERBUG)
                        {
                            locFlags &= (~SCCVW_SYSTEM_TEXTOUTPRINTERDRIVERBUG);
                            CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_PRINTERDRIVERBUG,  MF_UNCHECKED);
                        }
                        else
                        {
                            locFlags |= SCCVW_SYSTEM_TEXTOUTPRINTERDRIVERBUG;
                            CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_PRINTERDRIVERBUG,  MF_CHECKED);						
                        }
#ifdef WIN32
                        MenuInfo.fState = ((locFlags & SCCVW_SYSTEM_TEXTOUTPRINTERDRIVERBUG)==0)?MFS_UNCHECKED:MFS_CHECKED;
                        SetMenuItemInfo(GetMenu(hMainWnd),locId,FALSE,&MenuInfo);
#endif
                    }
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

            case MENU_SYSTEM_OPTIONS_JPGOFF:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCID_FILTERJPG;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;
                    if (bJPGOff)
                    {
                        bJPGOff = FALSE;
                        locFlags = SCCVW_FILTER_JPG_ENABLED;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_JPGOFF, MF_UNCHECKED);
                    }
                    else
                    {
                        bJPGOff = TRUE;
                        locFlags = SCCVW_FILTER_JPG_DISABLED;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_JPGOFF, MF_CHECKED);
                    }

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

            case MENU_SYSTEM_OPTIONS_LZWOFF:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCID_FILTERLZW;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;
                    if (bLZWOff)
                    {
                        bLZWOff = FALSE;
                        locFlags = SCCVW_FILTER_LZW_ENABLED;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_LZWOFF, MF_UNCHECKED);
                    }
                    else
                    {
                        bLZWOff = TRUE;
                        locFlags = SCCVW_FILTER_LZW_DISABLED;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_LZWOFF, MF_CHECKED);
                    }

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

            case MENU_SYSTEM_OPTIONS_IGNOREPASSWORD:
                {
                    SCCVWOPTIONSPEC40   locOption;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCID_IGNORE_PASSWORD;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&bIgnorePassword;

                    if (bIgnorePassword)
                    {
                        bIgnorePassword = FALSE;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_IGNOREPASSWORD, MF_UNCHECKED);
                    }
                    else
                    {
                        bIgnorePassword = TRUE;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_IGNOREPASSWORD, MF_CHECKED);
                    }

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

            case MENU_SYSTEM_OPTIONS_PDF_FILTER_BIDI:
                {
                    SCCVWOPTIONSPEC40   locOption;

                    locOption.dwSize  = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId	  = SCCID_PDF_FILTER_REORDER_BIDI;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData   = (VTVOID *)&bPDF_Process_Bidi;

                    if (bPDF_Process_Bidi)
                    {
                        bPDF_Process_Bidi = FALSE;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_PDF_FILTER_BIDI, MF_UNCHECKED);
                    }
                    else
                    {
                        bPDF_Process_Bidi = TRUE;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_PDF_FILTER_BIDI, MF_CHECKED);
                    }

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

            case MENU_SYSTEM_OPTIONS_STANDARD_EMAIL_HEADER:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCOPT_WPEMAILHEADEROUTPUT;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;                 
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_NO_EMAIL_HEADER,			MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_CUSTOM_EMAIL_HEADER,		MF_UNCHECKED);
					CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_ALL_EMAIL_HEADER,			MF_UNCHECKED);
					CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_STANDARD_EMAIL_HEADER,	MF_CHECKED);
                    locFlags = SCCUT_WP_EMAILHEADERSTANDARD;
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;
            case MENU_SYSTEM_OPTIONS_NO_EMAIL_HEADER:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCOPT_WPEMAILHEADEROUTPUT;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;    
                    locFlags = SCCUT_WP_EMAILHEADERNONE;
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_STANDARD_EMAIL_HEADER,	MF_UNCHECKED);                    
					CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_CUSTOM_EMAIL_HEADER,		MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_ALL_EMAIL_HEADER,			MF_UNCHECKED);
					CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_NO_EMAIL_HEADER,			MF_CHECKED);
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;
            case MENU_SYSTEM_OPTIONS_ALL_EMAIL_HEADER:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCOPT_WPEMAILHEADEROUTPUT;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;    
                    locFlags = SCCUT_WP_EMAILHEADERALL;
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_STANDARD_EMAIL_HEADER,	MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_NO_EMAIL_HEADER,			MF_UNCHECKED);
					CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_CUSTOM_EMAIL_HEADER,		MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_ALL_EMAIL_HEADER,			MF_CHECKED);
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;


			case MENU_SYSTEM_OPTIONS_CUSTOM_EMAIL_HEADER:
                {                                        
					if (DialogBoxParam(hInst,
									   MAKEINTRESOURCE(SCC_CUSTOMEMAILHEADERSDIALOG),
									   hViewWnd,
									   (DLGPROC)CustomEmailHeaderProc,
									   (LPARAM)NULL) )
					{
						CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_STANDARD_EMAIL_HEADER,	MF_UNCHECKED);
		                CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_NO_EMAIL_HEADER,			MF_UNCHECKED);
			            CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_ALL_EMAIL_HEADER,			MF_UNCHECKED);
						CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_CUSTOM_EMAIL_HEADER,		MF_CHECKED);
					}
				}
                break;



            case MENU_SYSTEM_OPTIONS_DISABLE_EMAIL:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCOPT_WPDISABLEEMAILHEADER;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;
                    if (bEmailOff)
                    {
                        bEmailOff = FALSE;
                        locFlags = SCCUT_EMAILVIEW_ENABLED;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_DISABLE_EMAIL, MF_UNCHECKED);
                    }
                    else
                    {
                        bEmailOff = TRUE;
                        locFlags = SCCUT_EMAILVIEW_DISABLED;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_DISABLE_EMAIL, MF_CHECKED);
                    }

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

           case MENU_SYSTEM_OPTIONS_COND_COMMENT_NONE:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCOPT_HTML_COND_COMMENT_MODE;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;

                    locFlags = HTML_COND_COMMENT_NONE;
                    dwHtmlCondCommentFlags = HTML_COND_COMMENT_NONE;

                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE5, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE6, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE7, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE8, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE9, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_ALL, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_NONE, MF_CHECKED);

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

                case MENU_SYSTEM_OPTIONS_COND_COMMENT_ALL:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCOPT_HTML_COND_COMMENT_MODE;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;

                    locFlags = HTML_COND_COMMENT_ALL;
                    dwHtmlCondCommentFlags = HTML_COND_COMMENT_ALL;

                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE5, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE6, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE7, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE8, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE9, MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_ALL, MF_CHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_NONE, MF_UNCHECKED);

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

                case MENU_SYSTEM_OPTIONS_COND_COMMENT_IE5:
                {
                    /*this function handles all the CheckMenuItem() and SendMessage() calls.*/
                    HandleCondCommentOption(hWnd, HTML_COND_COMMENT_IE5, MENU_SYSTEM_OPTIONS_COND_COMMENT_IE5);
                }
                break;

                case MENU_SYSTEM_OPTIONS_COND_COMMENT_IE6:
                {
                    /*this function handles all the CheckMenuItem() and SendMessage() calls.*/
                    HandleCondCommentOption(hWnd, HTML_COND_COMMENT_IE6, MENU_SYSTEM_OPTIONS_COND_COMMENT_IE6);
                }
                break;

                case MENU_SYSTEM_OPTIONS_COND_COMMENT_IE7:
                {
                    /*this function handles all the CheckMenuItem() and SendMessage() calls.*/
                    HandleCondCommentOption(hWnd, HTML_COND_COMMENT_IE7, MENU_SYSTEM_OPTIONS_COND_COMMENT_IE7);
                }
                break;

                case MENU_SYSTEM_OPTIONS_COND_COMMENT_IE8:
                {
                    /*this function handles all the CheckMenuItem() and SendMessage() calls.*/
                    HandleCondCommentOption(hWnd, HTML_COND_COMMENT_IE8, MENU_SYSTEM_OPTIONS_COND_COMMENT_IE8);
                }
                break;

                case MENU_SYSTEM_OPTIONS_COND_COMMENT_IE9:
                {
                    /*this function handles all the CheckMenuItem() and SendMessage() calls.*/
                    HandleCondCommentOption(hWnd, HTML_COND_COMMENT_IE9, MENU_SYSTEM_OPTIONS_COND_COMMENT_IE9);
                }
                break;

            case MENU_SYSTEM_OPTIONS_ANTIALIAS:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCID_ANTIALIAS;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;
                    if (bAntiAlias)
                    {
                        bAntiAlias = FALSE;
                        locFlags = SCCVW_ANTIALIAS_OFF;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_ANTIALIAS, MF_UNCHECKED);
                    }
                    else
                    {
                        bAntiAlias = TRUE;
                        locFlags = SCCVW_ANTIALIAS_ALL;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_ANTIALIAS, MF_CHECKED);
                    }

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

			case MENU_SYSTEM_OPTIONS_MAINTAINZOOM:
                {
                    SCCVWOPTIONSPEC40   locOption;                   

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCID_MAINTAINZOOM;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&bMaintainZoom;
                    if (bMaintainZoom)
                    {
                        bMaintainZoom = FALSE;                        
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MAINTAINZOOM, MF_UNCHECKED);
                    }
                    else
                    {
                        bMaintainZoom = TRUE;                        
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MAINTAINZOOM, MF_CHECKED);
                    }

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;



            case MENU_SYSTEM_OPTIONS_SBO:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCOPT_USENEWSBOHANDLER;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;
                    if (bNewSBO)
                    {
                        bNewSBO = FALSE;
                        locFlags = FALSE;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_SBO, MF_UNCHECKED);
                    }
                    else
                    {
                        bNewSBO = TRUE;
                        locFlags = TRUE;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_SBO, MF_CHECKED);
                    }

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)&locOption);
                }
                break;

            case MENU_SYSTEM_OPTIONS_TIMEZONE:
                {
                    SCCVWOPTIONSPEC40     locOptionSpec;
                    VTWORD                wReturn;

                    /* get current output path from user options */
                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_TIMEZONE;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &lTimeZone;
                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

                    wReturn = (VTWORD)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_TIMEZONEDIALOG),
                        hViewWnd,
                        (DLGPROC)GetTimezoneProc,
                        (LPARAM)&lTimeZone);

                    if (wReturn)
                        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_SYSTEM_OPTIONS_IOBUFSIZE:
                {
                    SCCBUFFEROPTIONS bo;
                    SCCVWOPTIONSPEC40 os;
                    os.dwSize = sizeof(os);
                    os.dwId = SCCOPT_IO_BUFFERSIZE;
                    os.dwFlags = SCCVWOPTION_CURRENT;
                    os.pData = &bo;

                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&os);

                    if (DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_IOBUFSIZEDIALOG),
                        hViewWnd,
                        (DLGPROC)GetIOBufSizeProc,
                        (LPARAM)&bo))
                    {
                        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)&os);
                    }
                }
                break;
            case MENU_SYSTEM_OPTIONS_MEMSMALLEST:
            case MENU_SYSTEM_OPTIONS_MEMSMALL:
            case MENU_SYSTEM_OPTIONS_MEMMED:
            case MENU_SYSTEM_OPTIONS_MEMLARGE:
            case MENU_SYSTEM_OPTIONS_MEMMAX:
                {
                    SCCVWOPTIONSPEC40  locOptionSpec;
                    VTDWORD          locMode;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_DOCUMENTMEMORYMODE;

                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMSMALLEST,  MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMSMALL,  MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMMED,	 MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMLARGE,  MF_UNCHECKED);
                    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMMAX,	 MF_UNCHECKED);

                    switch (locId)
                    {
                    case MENU_SYSTEM_OPTIONS_MEMSMALLEST:
                        locMode = SCCVW_DOCUMENTMEMORYMODE_SMALLEST;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMSMALLEST,  MF_CHECKED);
                        break;
                    case MENU_SYSTEM_OPTIONS_MEMSMALL:
                        locMode = SCCVW_DOCUMENTMEMORYMODE_SMALL;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMSMALL,  MF_CHECKED);
                        break;
                    case MENU_SYSTEM_OPTIONS_MEMMED:
                        locMode = SCCVW_DOCUMENTMEMORYMODE_MEDIUM;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMMED,	 MF_CHECKED);
                        break;
                    case MENU_SYSTEM_OPTIONS_MEMLARGE:
                        locMode = SCCVW_DOCUMENTMEMORYMODE_LARGE;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMLARGE,  MF_CHECKED);
                        break;
                    case MENU_SYSTEM_OPTIONS_MEMMAX:
                        locMode = SCCVW_DOCUMENTMEMORYMODE_LARGEST;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMMAX,	 MF_CHECKED);
                        break;
                    default:
                        locMode = SCCVW_DOCUMENTMEMORYMODE_MEDIUM;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMMED,  MF_CHECKED);
                        break;
                    }

                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = (VTVOID *)&locMode;

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }

                break;

            case MENU_SYSTEM_OPTIONS_ISODATES:
            case MENU_SYSTEM_OPTIONS_STANDARDDATES:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             dwFlags = 0;
                    VTDWORD				dwMask;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCID_FORMATFLAGS;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&dwFlags;

                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);

                    switch (locId)
                    {
                    case MENU_SYSTEM_OPTIONS_ISODATES:
                        dwMask = SCCOPT_FLAGS_ISODATETIMES;
                        CheckMenuItem(GetMenu(hWnd), MENU_SYSTEM_OPTIONS_ISODATES, MF_CHECKED);
                        CheckMenuItem(GetMenu(hWnd), MENU_SYSTEM_OPTIONS_STANDARDDATES, MF_UNCHECKED);
                        break;

                    case MENU_SYSTEM_OPTIONS_STANDARDDATES:
                        dwMask = 0;
                        CheckMenuItem(GetMenu(hWnd), MENU_SYSTEM_OPTIONS_ISODATES, MF_UNCHECKED);
                        CheckMenuItem(GetMenu(hWnd), MENU_SYSTEM_OPTIONS_STANDARDDATES, MF_CHECKED);
                        break;
                    default:
                        dwMask = 0;
                        break;
                    }
                    dwFlags &= ~(SCCOPT_FLAGS_ISODATETIMES);	// turn off all flags
                    dwFlags |= dwMask;

                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)&locOption);
                }
                break;

            case MENU_SYSTEM_OPTIONS_FILEACCESSDIALOG:
                if (bFileAccessOn)
                {
                    CheckMenuItem(GetMenu(hWnd), MENU_SYSTEM_OPTIONS_FILEACCESSDIALOG, MF_UNCHECKED);
                    bFileAccessOn = FALSE;
                }
                else
                {
                    CheckMenuItem(GetMenu(hWnd), MENU_SYSTEM_OPTIONS_FILEACCESSDIALOG, MF_CHECKED);
                    bFileAccessOn = TRUE;
                }
                break;

            case MENU_OPTIONS_NODECOUNT:
                {
                    VTDWORD dwCount=0;
                    TCHAR  szNodes[100];

                    if (SCCVWERR_OK == SendMessage(hViewWnd,SCCVW_GETTREECOUNT,0,(LPARAM)(VTLPDWORD)&dwCount))
                    {
                        wsprintf(szNodes,TEXT("%d Nodes."),dwCount);
                        MessageBox(hWnd,szNodes,TEXT("NODE COUNT"),MB_OK);
                    }
                    else
                        MessageBox(hWnd,TEXT("Not a tree file"),TEXT("NODE COUNT"),MB_OK);
                }
                break;
            case MENU_OPTIONS_SAVENODES:
                {
                    /*** This isn't really the most practical way to do this, but it does demonstrate both
                    getting node info, saving individual nodes, and saving everything within a folder.
                    ***/

                    VTDWORD dwCount=0;
                    TCHAR  szNodes[100];

                    if (SCCVWERR_OK == SendMessage(hViewWnd,SCCVW_GETTREECOUNT,0,(LPARAM)(VTLPDWORD)&dwCount))
                    {
                        if (dwCount > 0)
                        {
                            SCCDATREENODE TreeNode;
                            SCCVWSAVENODE SaveNode;
                            TreeNode.dwSize = sizeof(TreeNode);
                            TreeNode.dwNode = 0;
                            SaveNode.dwSize = sizeof(SaveNode);
                            SaveNode.dwNode = 0;

                            /* By setting SaveNode.szPath to an empty string we use the ArcOutputPath. */
                            /*     ArcOutputPath is set using the  SCCID_ARCOUTPUTPATH SetOption */
                            wsprintf((LPWSTR)SaveNode.szPath, TEXT(""));
                            wsprintf((LPWSTR)SaveNode.szName,TEXT(""));
                            SaveNode.dwFlags = SCCVW_SAVENODEFLAG_RESTORE_PATH;
                            /*
                            SaveNode.dwFlags = 0;
                            */

                            /*** Individual nodes. - This goes by each node, and if it isn't a folder, extracts that file.***/
                            while ((TreeNode.dwNode < dwCount) &&
                                (SCCVWERR_OK == SendMessage(hViewWnd,SCCVW_GETTREENODE,0,(LPARAM)(PSCCDATREENODE)&TreeNode)))
                            {
                                if (!(TreeNode.dwFlags & SCCDA_TREENODEFLAG_FOLDER))
                                {
                                    wsprintf(szNodes,TEXT("Node %d."),TreeNode.dwNode);
                                    //MessageBox(hWnd,TreeNode.szName,szNodes,MB_OK);
                                    SendMessage(hViewWnd,SCCVW_SAVETREENODE,0,(LPARAM)(SCCVWSAVENODE *)&SaveNode);
                                }
                                TreeNode.dwNode++;
                                SaveNode.dwNode++;
                            }

                            /* This does the same, but without path information, everything should be in the temp\flat directory. */
                            SaveNode.dwFlags = 0;
                            TreeNode.dwSize = sizeof(TreeNode);
                            TreeNode.dwNode = 0;
                            SaveNode.dwSize = sizeof(SaveNode);
                            SaveNode.dwNode = 0;
                            wsprintf((LPWSTR)SaveNode.szPath,TEXT("%s\\flat"), szArcOutputPath);

                            while ((TreeNode.dwNode < dwCount) &&
                                (SCCVWERR_OK == SendMessage(hViewWnd,SCCVW_GETTREENODE,0,(LPARAM)(PSCCDATREENODE)&TreeNode)))
                            {
                                if (!(TreeNode.dwFlags & SCCDA_TREENODEFLAG_FOLDER))
                                {
                                    wsprintf(szNodes,TEXT("Node %d."),TreeNode.dwNode);
                                    //MessageBox(hWnd,TreeNode.szName,szNodes,MB_OK);
                                    SendMessage(hViewWnd,SCCVW_SAVETREENODE,0,(LPARAM)(SCCVWSAVENODE *)&SaveNode);
                                }
                                TreeNode.dwNode++;
                                SaveNode.dwNode++;
                            }


                            /* this tests saving a folder node, (and therefore, everything in that folder). The 0th node
                            is selected, which will therefore save everything in the file, this time to the folders directory,
                            with path information.
                            */
                            TreeNode.dwSize = sizeof(TreeNode);
                            TreeNode.dwNode = 0;
                            SaveNode.dwSize = sizeof(SaveNode);
                            SaveNode.dwNode = 0;
                            SaveNode.dwFlags = SCCVW_SAVENODEFLAG_RESTORE_PATH;
                            wsprintf((LPWSTR)SaveNode.szPath,TEXT("%s\\folders"),szArcOutputPath);
                            /*** The root node, and consequently everything in the archive. ***/
                            if ((SCCVWERR_OK == SendMessage(hViewWnd,SCCVW_GETTREENODE,0,(LPARAM)(PSCCDATREENODE)&TreeNode)))
                            {
                                if ((TreeNode.dwFlags & SCCDA_TREENODEFLAG_FOLDER))
                                {
                                    wsprintf(szNodes,TEXT("Node %d."),TreeNode.dwNode);
                                    //MessageBox(hWnd,TreeNode.szName,szNodes,MB_OK);
                                    SendMessage(hViewWnd,SCCVW_SAVETREENODE,0,(LPARAM)(SCCVWSAVENODE *)&SaveNode);
                                }
                            }

                        }
                    }
                    else
                        MessageBox(hWnd,TEXT("Not a tree file"),TEXT("NODE COUNT"),MB_OK);
                }
                break;

            case MENU_OPTIONS_NOTIFYREADAHEAD :
                if (bHandleReadAheadMsg)
                {
                    CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_NOTIFYREADAHEAD, MF_UNCHECKED);
                    bHandleReadAheadMsg = FALSE;
                }
                else
                {
                    CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_NOTIFYREADAHEAD, MF_CHECKED);
                    bHandleReadAheadMsg = TRUE;
                }



                break;

            case MENU_OPTIONS_SETARCOUTPUTPATH:
                {
                    SCCVWOPTIONSPEC40     locOptionSpec;
                    VTWORD                    wReturn;

                    /* get current output path from user options */
                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_ARCOUTPUTPATH;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = szArcOutputPath;
                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

                    /* prompt user for an output path */

                    nOutputPathMode = MENU_OPTIONS_SETARCOUTPUTPATH;

                    wReturn = (VTWORD)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_ARCOUTPUTPATH),
                        hViewWnd,
                        (DLGPROC)GetOutputPathProc,
                        (LPARAM)szArcOutputPath);

                    /*  call set option to set the new value, if value has changed */
                    if (wReturn)
                    {
                        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                    }
                }
                break;

            case MENU_SYSTEM_TEMPDIR:
                {
                    SCCVWOPTIONSPEC40     locOptionSpec;
                    VTWORD                        wReturn;
                    SCCUTTEMPDIRSPEC    sUTTmpDir;

                    /* get current output path from user options */
                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_TEMPDIR;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &sUTTmpDir;
                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                    CopyByteStringToWordString(szTempDir, (BYTE *)sUTTmpDir.szTempDirName);
                    sUTTmpDir.dwSpecType = IOTYPE_ANSIPATH;
                    sUTTmpDir.dwSize     = sizeof(SCCUTTEMPDIRSPEC);

                    /* prompt user for an output path */

                    nOutputPathMode = MENU_SYSTEM_TEMPDIR;

                    wReturn = (VTWORD)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_ARCOUTPUTPATH),
                        hViewWnd,
                        (DLGPROC)GetOutputPathProc,
                        (LPARAM)szTempDir);

                    /*  call set option to set the new value, if value has changed */
                    CopyWordStringToByteString((BYTE *)sUTTmpDir.szTempDirName, szTempDir);

                    if (wReturn)
                    {
                        if (SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec) != SCCERR_OK)
                        {
                            MessageBox(hWnd, TEXT("Error Setting Directory Path"), TEXT("Options"), MB_OK);
                        }
                    }
                }
                break;

            case MENU_SYSTEM_OPTIONS_LOTUSNOTESDIRECTORY:
                {
                    SCCVWOPTIONSPEC40     locOptionSpec;
                    VTWORD                        wReturn;
                    VTBYTE       sUTLotusNotesDirectory[MAX_PATH_LENGTH];    
                    DWORD retval;
                    /* get current output path from user options */
                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCOPT_LOTUSNOTESDIRECTORY;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &sUTLotusNotesDirectory;
                    retval = (DWORD)SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                    if (retval == SCCERR_OK)
                    {
                        CopyByteStringToWordString(szLotusNotesDirectory,sUTLotusNotesDirectory);
                    }
                    else
                    {
                        szLotusNotesDirectory[0]=0;
                    }

                    /* this variable affects the title bar and some behavior
                    of the subsequent dialog box. */
                    nOutputPathMode = MENU_SYSTEM_OPTIONS_LOTUSNOTESDIRECTORY;

                    /* prompt user for the directory */
                    wReturn = (VTWORD)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_ARCOUTPUTPATH),
                        hViewWnd,
                        (DLGPROC)GetOutputPathProc,
                        (LPARAM)szLotusNotesDirectory);

                    /*  call set option to set the new value, if value has changed */
                    CopyWordStringToByteString(sUTLotusNotesDirectory, szTempDir);
                    if (wReturn)
                    {
                        if (SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec) != SCCERR_OK)
                        {
                            MessageBox(hWnd, TEXT("Error setting Lotus Notes Directory"), TEXT("Options"), MB_OK);
                        }
                    }
                }
                break;



            case MENU_OPTIONS_UNMAPPABLECHAR:
                {
                    SCCVWOPTIONSPEC40       locOptionSpec;
                    VTWORD                  wUnmappableChar;
                    VTWORD                  wReturn;

                    /* get current unmappable char from user options */
                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_UNMAPPABLECHAR;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &wUnmappableChar;
                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

                    /* prompt user for an unmappable char */
                    wReturn = (VTWORD)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_UNMAPPABLECHARDIALOG),
                        hViewWnd,
                        (DLGPROC)UnmappableCharProc,
                        (LPARAM)wUnmappableChar);

                    /*  call set option to set the new value, if value has changed */
                    if (wReturn != wUnmappableChar)
                    {
                        (*(VTWORD *)locOptionSpec.pData) = wReturn;
                        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                    }
                }
                break;

            case MENU_FONTOPTIONS_FONTSCALINGFACTOR:
                {
                    SCCVWOPTIONSPEC40       locOptionSpec;
                    VTDWORD                 dwScaleFactor;
                    VTDWORD                 dwCurrentValue;

                    /* get current value of font scaling factor */
                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_FONTSCALINGFACTOR;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &dwCurrentValue;
                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

                    /* prompt user for a new value, using current value as a default */
                    dwScaleFactor = (VTDWORD)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_FONTSCALINGDIALOG),
                        hViewWnd,
                        (DLGPROC)FontScalingProc,
                        (LPARAM)dwCurrentValue);

                    /* call set option to set the new value, if value has changed */
                    if (dwScaleFactor != dwCurrentValue)
                    {
                        (*(VTDWORD *)locOptionSpec.pData) = dwScaleFactor;
                        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                    }
                }
                break;

            case MENU_FONTOPTIONS_ADDDISPLAYFONTALIAS:
                {
                    SCCVWOPTIONSPEC40     locOptionSpec;
                    SCCVWFONTALIAS        locFontAlias;
                    VTLONG                lRet;

                    locFontAlias.dwSize = sizeof(SCCVWFONTALIAS);
                    locFontAlias.dwAliasID = 0;
                    locFontAlias.dwFlags = SCCVW_FONTALIAS_ALIASNAME;
                    locFontAlias.szwOriginal[0] = (VTWORD)0;
                    locFontAlias.szwAlias[0] = (VTWORD)0;

                    /* prompt user for a new display font alias */
                    lRet = (VTLONG)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_ADDFONTALIASDIALOG),
                        hViewWnd,
                        (DLGPROC)AddFontAliasProc,
                        (LPARAM)(VOID *)&locFontAlias);

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_DISPLAYFONTALIAS;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &locFontAlias;
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_FONTOPTIONS_REMOVEDISPLAYFONTALIAS:
                {
                    VTLONG                lRet;
                    REMOVEALIASMAP        RemoveAliasMap;

                    /* build the display font alias map */
                    BuildFontAliasMap(hViewWnd, &RemoveAliasMap.sccAliasMap, TRUE);
                    RemoveAliasMap.bIsDisplay = TRUE;

                    /* call dialag to remove display font aliases */
                    lRet = (VTLONG)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_REMOVEFONTALIASDIALOG),
                        hViewWnd,
                        (DLGPROC)RemoveFontAliasProc,
                        (LPARAM)(VOID *)&RemoveAliasMap);

                    DestroyFontAliasMap(&RemoveAliasMap.sccAliasMap);
                }
                break;

            case MENU_FONTOPTIONS_CLEARDISPLAYFONTALIASMAP:
                {
                    SCCVWOPTIONSPEC40     locOptionSpec;
                    SCCVWFONTALIAS        locFontAlias;

                    locFontAlias.dwSize = sizeof(SCCVWFONTALIAS);
                    locFontAlias.dwAliasID = 0;
                    locFontAlias.dwFlags = SCCVW_FONTALIAS_REMOVEALL;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_DISPLAYFONTALIAS;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &locFontAlias;
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_FONTOPTIONS_SETDISPLAYDEFAULTALIASMAP:
                {
                    SCCVWOPTIONSPEC40     locOptionSpec;
                    SCCVWFONTALIAS        locFontAlias;

                    locFontAlias.dwSize = sizeof(SCCVWFONTALIAS);
                    locFontAlias.dwAliasID = 0;
                    locFontAlias.dwFlags = SCCVW_FONTALIAS_USEDEFAULTS;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_DISPLAYFONTALIAS;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &locFontAlias;
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_FONTOPTIONS_ADDPRINTFONTALIAS:
                {
                    SCCVWOPTIONSPEC40     locOptionSpec;
                    SCCVWFONTALIAS        locFontAlias;
                    VTLONG                lRet;

                    locFontAlias.dwSize = sizeof(SCCVWFONTALIAS);
                    locFontAlias.dwAliasID = 0;
                    locFontAlias.dwFlags = SCCVW_FONTALIAS_ALIASNAME;
                    locFontAlias.szwOriginal[0] = (VTWORD)0;
                    locFontAlias.szwAlias[0] = (VTWORD)0;

                    /* prompt user for a new print font alias */
                    lRet = (VTLONG)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_ADDFONTALIASDIALOG),
                        hViewWnd,
                        (DLGPROC)AddFontAliasProc,
                        (LPARAM)(VOID *)&locFontAlias);

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_PRINTFONTALIAS;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &locFontAlias;
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_FONTOPTIONS_REMOVEPRINTFONTALIAS:
                {
                    VTLONG                lRet;
                    REMOVEALIASMAP        RemoveAliasMap;

                    /* build the print font alias map */
                    BuildFontAliasMap(hViewWnd, &RemoveAliasMap.sccAliasMap, FALSE);
                    RemoveAliasMap.bIsDisplay = FALSE;

                    /* call dialag to remove print font aliases */
                    lRet = (VTLONG)DialogBoxParam(hInst,
                        MAKEINTRESOURCE(SCC_REMOVEFONTALIASDIALOG),
                        hViewWnd,
                        (DLGPROC)RemoveFontAliasProc,
                        (LPARAM)(VOID *)&RemoveAliasMap);

                    DestroyFontAliasMap(&RemoveAliasMap.sccAliasMap);
                }
                break;

            case MENU_FONTOPTIONS_CLEARPRINTFONTALIASMAP:
                {
                    SCCVWOPTIONSPEC40     locOptionSpec;
                    SCCVWFONTALIAS        locFontAlias;

                    locFontAlias.dwSize = sizeof(SCCVWFONTALIAS);
                    locFontAlias.dwAliasID = 0;
                    locFontAlias.dwFlags = SCCVW_FONTALIAS_REMOVEALL;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_PRINTFONTALIAS;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &locFontAlias;
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_FONTOPTIONS_SETPRINTDEFAULTALIASMAP:
                {
                    SCCVWOPTIONSPEC40     locOptionSpec;
                    SCCVWFONTALIAS        locFontAlias;

                    locFontAlias.dwSize = sizeof(SCCVWFONTALIAS);
                    locFontAlias.dwAliasID = 0;
                    locFontAlias.dwFlags = SCCVW_FONTALIAS_USEDEFAULTS;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOptionSpec.dwId = SCCID_PRINTFONTALIAS;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &locFontAlias;
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                }
                break;

            case MENU_FONTOPTIONS_DEFAULTINPUTCHARACTERSET:
                DialogBox(hInst, MAKEINTRESOURCE(SCC_DEFAULTCHARSETDIALOG), hViewWnd, (DLGPROC)DefaultCharsetProc);
                break;

            case MENU_SYSTEM_OPTIONS_PARSEXMPMETADATA:
                {
                    SCCVWOPTIONSPEC40   locOption;
                    VTDWORD             locFlags;

                    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    locOption.dwId = SCCID_PARSEXMPMETADATA;
                    locOption.dwFlags = SCCVWOPTION_CURRENT;
                    locOption.pData = (VTVOID *)&locFlags;
                    if (bParseXMPOn)
                    {
                        locFlags = 0;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_PARSEXMPMETADATA, MF_UNCHECKED);
                        bParseXMPOn = FALSE;
                    }
                    else
                    {
                        locFlags = 1;
                        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_PARSEXMPMETADATA, MF_CHECKED);
                        bParseXMPOn = TRUE;
                    }
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
                }
                break;

            case MENU_HELP_ABOUT:
                DialogBox(hInst, TEXT("HELP_ABOUTBOX"), hViewWnd, (DLGPROC)HelpAbout);
                break;

            default:
                return (DefWindowProc(hWnd, message, wParam, lParam));
            }
        }
        break;

    default:
        return (DefWindowProc(hWnd, message, wParam, lParam));
        break;
    }

    return (0L);
}

/*
|  Handle WM_CREATE
*/

VOID DoCreate(HWND hWnd)
{
    TCHAR  locViewerPath[MAX_PATH_LENGTH];
    TCHAR  locViewerClass[MAX_PATH_LENGTH];

    /*
    |  Load the Outside In Viewer Technology DLL (SCxxVW.DLL)
    |
    |  This code loads SCxxVW.DLL from the same directory
    |  the sample application is in.
    */

    SCCGetExePath(hInst,szExePath,MAX_PATH_LENGTH);
    lstrcpy(locViewerPath,szExePath);
    hViewerLibrary = SCCLoadViewerDLL(locViewerPath);

    /*
    |  If the Viewer Technology can not be loaded, bail out.
    */

    if (hViewerLibrary == NULL)
    {
        return;
    }

    /*
    |  Create the view window here
    */

    lstrcpy(locViewerClass,TEXT("SCCVIEWER"));

    hViewWnd = CreateWindow(locViewerClass,
        NULL,
        WS_CHILD | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE,
        0,
        0,
        0,
        0,
        hWnd,
        0,
        hInst,
        NULL);

	if (hViewWnd == NULL)
		return;

	if (szCmdLine && (*szCmdLine != '\0'))
	{      
		SCCVWVIEWFILE80  locViewFile;

		locViewFile.dwSize = sizeof(SCCVWVIEWFILE80);
		locViewFile.dwSpecType = IOTYPE_UNICODEPATH;
		locViewFile.pSpec = (VTVOID *)szCmdLine;
		locViewFile.dwViewAs = 0;
		if (szDisplayName[0] != 0)
		{
			locViewFile.bUseDisplayName = TRUE;
			lstrcpy(locViewFile.szDisplayName,szDisplayName);
		}
		else
			locViewFile.bUseDisplayName = FALSE;

		locViewFile.bDeleteOnClose = FALSE;
		locViewFile.dwFlags = 0;
		locViewFile.dwReserved1 = 0;
		locViewFile.dwReserved2 = 0;

		SendMessage(hViewWnd,SCCVW_VIEWFILE,0,(LPARAM)&locViewFile);       
		free(szCmdLine);
		szCmdLine = NULL;
	}
	SetFocus(hViewWnd);
}


/***  Initialize all checkboxes in menus ***/
VOID InitMenuChecks(HWND hWnd)
{
    SCCVWOPTIONSPEC40   locOption;
    VTDWORD             locDword;

    if (bHandleReadAheadMsg)
        CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_NOTIFYREADAHEAD, MF_CHECKED);
    else
        CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_NOTIFYREADAHEAD, MF_UNCHECKED);

    /* Same for all options. */
    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
    locOption.dwFlags = SCCVWOPTION_CURRENT;
    locOption.pData = (VTVOID *)&locDword;


    locOption.dwId = SCCOPT_USENEWSBOHANDLER;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    bNewSBO = (locDword != 0);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_SBO, bNewSBO ? MF_CHECKED : MF_UNCHECKED);


    locOption.dwId = SCCID_WPWRAPTOWINDOW;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_WPWRAPON, locDword? MF_CHECKED : MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_WPWRAPOFF, locDword?MF_UNCHECKED :MF_CHECKED);


    locOption.dwId = SCCID_REORDERMETHOD;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);

    CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_REORDEROFF,			(locDword == SCCUT_REORDER_UNICODE_OFF) ?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_REORDERUNICODELTOR,	(locDword == SCCUT_REORDER_UNICODE_LTOR)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_OPTIONS_REORDERUNICODERTOL,	(locDword == SCCUT_REORDER_UNICODE_RTOL)?MF_CHECKED:MF_UNCHECKED);



    locOption.dwId = SCCID_FIFLAGS;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    CheckMenuItem( GetMenu(hWnd), MENU_FIOPTIONS_FINORMAL,  (locDword == SCCVW_FI_NORMAL)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_FIOPTIONS_FIEXTENDED,(locDword == SCCVW_FI_NORMAL)?MF_UNCHECKED:MF_CHECKED);

    locOption.dwId = SCCID_SYSTEMFLAGS;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_READAHEADOFF,		(locDword&SCCVW_SYSTEM_NOREADAHEAD)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_READAHEADON,      (locDword&SCCVW_SYSTEM_NOREADAHEAD)?MF_UNCHECKED:MF_CHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_GRAPHICDRIVERBUG, (locDword&SCCVW_SYSTEM_TEXTOUTDISPLAYDRIVERBUG)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_PRINTERDRIVERBUG, (locDword&SCCVW_SYSTEM_TEXTOUTPRINTERDRIVERBUG)?MF_CHECKED:MF_UNCHECKED);

    locOption.dwId = SCCID_FILTERJPG;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    bJPGOff = !(locDword==SCCVW_FILTER_JPG_ENABLED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_JPGOFF, (bJPGOff)?MF_CHECKED:MF_UNCHECKED);

    locOption.dwId = SCCID_FILTERLZW;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    bLZWOff = !(locDword == SCCVW_FILTER_LZW_ENABLED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_LZWOFF, (bLZWOff)?MF_CHECKED:MF_UNCHECKED);

    locOption.dwId = SCCID_IGNORE_PASSWORD;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    bIgnorePassword = locDword;
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_IGNOREPASSWORD, (bIgnorePassword)?MF_CHECKED:MF_UNCHECKED);

    locOption.dwId = SCCID_PDF_FILTER_REORDER_BIDI;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    bPDF_Process_Bidi = locDword;
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_PDF_FILTER_BIDI, (bPDF_Process_Bidi)?MF_CHECKED:MF_UNCHECKED);

    locOption.dwId = SCCOPT_WPDISABLEEMAILHEADER;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    bEmailOff = (locDword == SCCUT_EMAILVIEW_DISABLED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_DISABLE_EMAIL, (locDword == SCCUT_EMAILVIEW_DISABLED)?MF_CHECKED:MF_UNCHECKED);

    locOption.dwId = SCCOPT_WPEMAILHEADEROUTPUT;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_STANDARD_EMAIL_HEADER, (locDword == SCCUT_WP_EMAILHEADERSTANDARD)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_NO_EMAIL_HEADER, (locDword == SCCUT_WP_EMAILHEADERNONE)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_ALL_EMAIL_HEADER, (locDword == SCCUT_WP_EMAILHEADERALL)?MF_CHECKED:MF_UNCHECKED);
	CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_CUSTOM_EMAIL_HEADER, (locDword == SCCUT_WP_EMAILHEADERCUSTOM)?MF_CHECKED:MF_UNCHECKED);


    locOption.dwId = SCCID_ANTIALIAS;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    bAntiAlias = (locDword==SCCVW_ANTIALIAS_ALL);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_ANTIALIAS, bAntiAlias?MF_CHECKED:MF_UNCHECKED);

	locOption.dwId = SCCID_MAINTAINZOOM;
	locOption.pData = (VTVOID *)&bMaintainZoom;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);	
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MAINTAINZOOM, bMaintainZoom?MF_CHECKED:MF_UNCHECKED);
	locOption.pData = (VTVOID *)&locDword;


    locOption.dwId = SCCID_DOCUMENTMEMORYMODE;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);            
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMSMALLEST,(locDword == SCCVW_DOCUMENTMEMORYMODE_SMALLEST)	?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMSMALL, (locDword == SCCVW_DOCUMENTMEMORYMODE_SMALL)	?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMMED,	(locDword == SCCVW_DOCUMENTMEMORYMODE_MEDIUM)	?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMLARGE, (locDword == SCCVW_DOCUMENTMEMORYMODE_LARGE)	?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_MEMMAX,	(locDword == SCCVW_DOCUMENTMEMORYMODE_LARGEST)	?MF_CHECKED:MF_UNCHECKED);

    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_FILEACCESSDIALOG,	MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_PARSEXMPMETADATA, MF_UNCHECKED);

    locOption.dwId = SCCOPT_HTML_COND_COMMENT_MODE;
    locDword = HTML_COND_COMMENT_NONE;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&locOption);
    
    dwHtmlCondCommentFlags = locDword;
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE5,	
        (dwHtmlCondCommentFlags == HTML_COND_COMMENT_IE5)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE6,	
        (dwHtmlCondCommentFlags == HTML_COND_COMMENT_IE6)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE7,	
        (dwHtmlCondCommentFlags == HTML_COND_COMMENT_IE7)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE8,	
        (dwHtmlCondCommentFlags == HTML_COND_COMMENT_IE8)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_IE9,	
        (dwHtmlCondCommentFlags == HTML_COND_COMMENT_IE9)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_NONE,	
        (dwHtmlCondCommentFlags == HTML_COND_COMMENT_NONE)?MF_CHECKED:MF_UNCHECKED);
    CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_ALL,	
        (dwHtmlCondCommentFlags == HTML_COND_COMMENT_ALL)?MF_CHECKED:MF_UNCHECKED);


}




/*
|  Handle WM_SIZE
*/

VOID DoSize(HWND hWnd,VTWORD wWidth,VTWORD wHeight)
{
    if (IsWindow(hViewWnd))
    {
        InvalidateRect(hWnd,NULL,0);
        MoveWindow(hViewWnd,20,20,wWidth-40,wHeight-40,TRUE);
        ShowWindow(hViewWnd,SW_SHOW);
    }
}

/*
|  Handle WM_DESTROY
*/

VOID DoDestroy(HWND hWnd)
{
    UNUSED(hWnd);
    if (IsWindow(hViewWnd))
    {
        SendMessage(hViewWnd,SCCVW_CLOSEFILE,0,0L);
        DestroyWindow(hViewWnd);
    }

    if (hViewerLibrary != NULL)
    {
        FreeLibrary(hViewerLibrary);
    }
}

/*
|  Handle Close menu item
*/

VOID DoCloseFile(HWND hWnd)
{
    UNUSED(hWnd);

	close_child_processes(FALSE);
	ClearInfoMessages();

    if (IsWindow(hViewWnd))
    {
        VWSetStatCallback((VWSTATCALLBACKFN)0,hViewWnd, 0);    
        SendMessage(hViewWnd,SCCVW_CLOSEFILE,0,0L);
        InvalidateRect(hViewWnd,NULL,TRUE);
    }
}

/*
|  Handle SCCVW_FILECHANGE message
*/

VOID DoFileChange(HWND hWnd)
{
    SCCVWFILEINFO84  locFileInfo;
    TCHAR *defStr = TEXT("OIVT Options Sample");

    locFileInfo.dwSize = sizeof(locFileInfo);

    if (SendMessage(hViewWnd, SCCVW_GETFILEINFO, 0, (LPARAM)&locFileInfo) == SCCVWERR_OK)
    {
        TCHAR *locStr = (TCHAR *)malloc((wcslen(locFileInfo.szDisplayName)+wcslen(locFileInfo.szFileIdName)+24)*sizeof(TCHAR));   /* big enough to hold everything */
        if (locStr)
            wsprintf(locStr,TEXT("OIVT Options - %s - %s"),locFileInfo.szDisplayName,locFileInfo.szFileIdName);
        else
            locStr = defStr;

        SetWindowText(hWnd,locStr);

        if (locStr != defStr)
            free(locStr);   

        SendMessage(hViewWnd,SCCVW_FREEFILEINFO,0,(LPARAM)&locFileInfo);
    }
    else
    {
        SetWindowText(hWnd,defStr);
    }

	UpdateZoom(hWnd);
}


/*
|  Handle Open menu item
*/

VOID DoOpenFile(HWND hWnd)
{
#define MAXFILENAME 256

    OPENFILENAME  locOFN;
    TCHAR           locFilterSpec[128] = TEXT("All files\0*.*\0");
    TCHAR           locFileName[MAXFILENAME];
    TCHAR           locFileTitle[MAXFILENAME];

    locFileName[0] = 0x00;
    locFileTitle[0] = 0x00;

    locOFN.lStructSize = sizeof(OPENFILENAME);
    locOFN.hwndOwner = hWnd;
    locOFN.hInstance = hInst;
    locOFN.lpstrFilter = locFilterSpec;
    locOFN.lpstrCustomFilter = NULL;
    locOFN.nMaxCustFilter = 0L;
    locOFN.nFilterIndex = 0L;
    locOFN.lpstrFile = locFileName;
    locOFN.nMaxFile = MAXFILENAME;
    locOFN.lpstrFileTitle = locFileTitle;
    locOFN.nMaxFileTitle = MAXFILENAME;
    locOFN.lpstrInitialDir = NULL;
    locOFN.lpstrTitle = TEXT("Open...");
    locOFN.Flags = OFN_FILEMUSTEXIST;
    locOFN.lpstrDefExt = NULL;
    locOFN.lCustData = 0;
    locOFN.lpfnHook = NULL;
    locOFN.lpTemplateName = NULL;

    if (GetOpenFileName(&locOFN) == TRUE)
    {
        if (IsWindow(hViewWnd))
        {
            SCCVWVIEWFILE40  locViewFile;

            locViewFile.dwSize = sizeof(SCCVWVIEWFILE40);
            locViewFile.dwSpecType = IOTYPE_UNICODEPATH;
            locViewFile.pSpec = (VTVOID *)locFileName;
            locViewFile.dwViewAs = 0;
            locViewFile.bUseDisplayName = FALSE;
            locViewFile.bDeleteOnClose = FALSE;
            locViewFile.dwFlags = 0;
            locViewFile.dwReserved1 = 0;
            locViewFile.dwReserved2 = 0;

			close_child_processes(TRUE);
			ClearInfoMessages();

            SendMessage(hViewWnd,SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&locViewFile);

            rad = FALSE;
            bAbort = FALSE;
            VWSetStatCallback((VWSTATCALLBACKFN)DAStatCallback, hViewWnd, 0);
            SetFocus(hViewWnd);
        }
    }
}


VOID UpdateZoom(HWND hWnd)
{
	SCCVWIMAGEZOOM Zoom;
	TCHAR buf[128];	
	SCCVWDISPLAYINFO80 DisplayInfo;
	SCCVWOPTIONSPEC40  OptionInfo;
	HDC hdc;
	RECT textRect;
	RECT dcRect;
	static RECT oldRect={0,0,0,0};
	HBRUSH brush;
	int w,h;
	int oldBkMode;
	int zx, zy;
	
	DisplayInfo.dwSize = sizeof(SCCVWDISPLAYINFO80);
	SendMessage(hViewWnd,SCCVW_GETDISPLAYINFO,0,(LPARAM)&DisplayInfo);

	OptionInfo.dwSize  = sizeof(SCCVWOPTIONSPEC40);	
	OptionInfo.dwFlags = SCCVWOPTION_CURRENT;
	OptionInfo.pData   = &Zoom;

	if (DisplayInfo.dwType == SCCVWTYPE_IMAGE || DisplayInfo.dwType == SCCVWTYPE_VECTOR)
	{
		OptionInfo.dwId    = (DisplayInfo.dwType == SCCVWTYPE_IMAGE)?SCCID_BMPZOOM:SCCID_VECZOOM;
		SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)&OptionInfo);
		zx = Zoom.dwXNu*100/Zoom.dwXDe;
		zy = Zoom.dwYNu*100/Zoom.dwYDe;

		if (zx != zy)
			wsprintf( buf, TEXT("Zoom: X: %d%%  Y: %d%%"),zx,zy);
		else
			wsprintf( buf, TEXT("Zoom: %d%%"),zx);
	}
	else
	{		
		wsprintf( buf, TEXT(""));
	}

	hdc = GetDC(hWnd);

	/* Clear any old writing */
	brush = GetStockObject(LTGRAY_BRUSH);
	FillRect(hdc,&oldRect,brush);

	if (buf[0] != 0)
	{
		/* Determine size needed for new writing */
		textRect.left = 0;
		textRect.top = 0;
		textRect.bottom = 10;
		textRect.right = 10;	
	
		DrawText(hdc,buf,-1,&textRect,DT_CALCRECT|DT_CENTER);

		w = textRect.right - textRect.left;
		h = textRect.bottom- textRect.top;

		/* Get location to put text, bottom right */
		GetClientRect(hWnd,&dcRect);

		textRect.top    = dcRect.bottom-2-h;
		textRect.bottom = textRect.top+h;
	
		textRect.right = dcRect.right-20;
		textRect.left = textRect.right-w;

		/* Write text out, transparent background */
		oldBkMode = SetBkMode(hdc,TRANSPARENT);
		DrawText(hdc,buf,-1,&textRect,DT_CENTER);
		SetBkMode(hdc,oldBkMode);

		/* Save this rectangle to erase it later */
		oldRect = textRect;
	}
	ReleaseDC(hWnd,hdc);	
}



WIN_ENTRYSC BOOL WIN_ENTRYMOD FontScalingProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT                locRect;
    int                 locX;
    int                 locY;

    static int  iFontScaleFactor;
    TCHAR sFontScaleFactor[32];

    switch (message)
    {
    case WM_INITDIALOG:
        {
            /*
            |   Center dialog
            */

            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            /*
            |   Load scale factor and convert to a string
            */

            iFontScaleFactor = (int)lParam;
            myitoa(iFontScaleFactor, sFontScaleFactor);

            SetDlgItemText(hDlg,EDIT_FONTSCALINGFACTOR,sFontScaleFactor);
        }

        return (TRUE);

    case WM_COMMAND:
        {
            switch(wParam)
            {
            case IDOK:

                GetDlgItemText(hDlg,EDIT_FONTSCALINGFACTOR,sFontScaleFactor,10);

                if (sFontScaleFactor)
                    iFontScaleFactor = _wtoi(sFontScaleFactor);

                if (iFontScaleFactor < 40)
                {
                    iFontScaleFactor = 40;
                    myitoa(iFontScaleFactor, sFontScaleFactor);
                    SetDlgItemText(hDlg,EDIT_FONTSCALINGFACTOR,sFontScaleFactor);
                    SendDlgItemMessage(hDlg,EDIT_FONTSCALINGFACTOR,EM_SETSEL,(WPARAM)0,(LPARAM)-1);
                }
                else if (iFontScaleFactor > 300)
                {
                    iFontScaleFactor = 300;
                    myitoa(iFontScaleFactor, sFontScaleFactor);
                    SetDlgItemText(hDlg,EDIT_FONTSCALINGFACTOR,sFontScaleFactor);
                    SendDlgItemMessage(hDlg,EDIT_FONTSCALINGFACTOR,EM_SETSEL,(WPARAM)0,(LPARAM)-1);
                }
                else
                {
                    EndDialog(hDlg, iFontScaleFactor);
                }

                return (TRUE);

            case IDCANCEL:

                EndDialog(hDlg, iFontScaleFactor);
                return (TRUE);

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return(FALSE);
}

#define LINE_EXTRAS 4
#define NAME_SIZE 128
#define DATA_GRAIN 512
WIN_ENTRYSC BOOL WIN_ENTRYMOD PropertyDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            RECT	locRect;
            int		locX;
            int		locY;
            HWND	hTextWnd;
            TCHAR*	pCurText;
            TCHAR*	pTextBuffer;
            DWORD	dwTextSize = DATA_GRAIN;
            DWORD	dwTextUsed = 0;
            LRESULT res = SCCERR_OK;
            DWORD	dwNameSize = NAME_SIZE;
            DWORD	dwDataSize = DATA_GRAIN;
            DWORD	dwPropertyCount = 0;

            SCCVWGETPROP props = { 0 };

            hTextWnd = GetDlgItem(hDlg, EDIT_PROPERTY);
            pCurText = pTextBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwTextSize * sizeof(WCHAR));
            if (!pTextBuffer)
            {
                res = SCCERR_ALLOCFAILED;
                goto PropsError;
            }

            props.dwSize = sizeof(SCCVWGETPROP);
            props.dwNameSize = dwNameSize;
            props.pName = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.dwNameSize);
            props.dwDataSize = dwDataSize;
            props.pData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.dwDataSize);

            /*
            |   Center dialog
            */
            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            while (SCCERR_OK == res)
            {
                if (0 == dwPropertyCount)
                {
                    res = SendMessage(hViewWnd, SCCVW_GETPROPERTY, SCCVW_GETPROP_FIRST, (LPARAM) &props);
                }
                else
                {
                    res = SendMessage(hViewWnd, SCCVW_GETPROPERTY, SCCVW_GETPROP_NEXT, (LPARAM) &props);
                }
                if (SCCERR_INSUFFICIENTBUFFER == res)
                {
                    void* pNew;
                    if (props.dwNameSize > dwNameSize)
                    {
                        dwNameSize = props.dwNameSize;
                        pNew = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.pData, dwNameSize);
                        if (pNew)
                        {
                            props.pName = pNew;
                        }
                        else
                        {
                            res = SCCERR_ALLOCFAILED;
                            goto PropsError;
                        }
                    }
                    if (props.dwDataSize > dwDataSize)
                    {
                        dwDataSize = props.dwDataSize;
                        pNew = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.pData, dwDataSize);
                        if (pNew)
                        {
                            props.pData = pNew;
                        }
                        else
                        {
                            res = SCCERR_ALLOCFAILED;
                            goto PropsError;
                        }
                    }
                    props.dwNameSize = dwNameSize;
                    props.dwDataSize = dwDataSize;
                    props.dwPropId = dwPropertyCount;
                    res = SendMessage(hViewWnd, SCCVW_GETPROPERTY, SCCVW_GETPROP_BYINDEX, (LPARAM) &props);
                }
                if (SCCERR_OK == res)
                {
                    DWORD dwNameSizeWords, dwDataSizeWords;

                    dwNameSizeWords = props.dwNameSize / sizeof(WORD);
                    dwDataSizeWords = props.dwDataSize / sizeof(WORD);
                    dwPropertyCount++;

                    if (dwTextSize < dwTextUsed + props.dwNameSize + props.dwDataSize + LINE_EXTRAS)
                    {
                        VOID* pNew;
                        while (dwTextSize < dwTextUsed + props.dwNameSize + props.dwDataSize)
                        {
                            dwTextSize += 256;
                        }
                        pNew = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, pTextBuffer, dwTextSize * sizeof(WCHAR));
                        if (pNew)
                        {
                            pTextBuffer = pNew;
                            pCurText = pTextBuffer + dwTextUsed;
                        }
                        else
                        {
                            res = SCCERR_ALLOCFAILED;
                            goto PropsError;
                        }
                    }
                    lstrcpy(pCurText, props.pName);
                    pCurText += dwNameSizeWords - 1; // -1, don't copy terminator!
                    *pCurText = ':';
                    pCurText++;
                    *pCurText = ' ';
                    pCurText++;
                    dwTextUsed += dwNameSizeWords + 1;
                    lstrcpy(pCurText, props.pData);
                    pCurText += dwDataSizeWords - 1; // -1, don't copy terminator!
                    *pCurText = '\r';
                    pCurText++;
                    *pCurText = '\n';
                    pCurText++;
                    dwTextUsed += dwDataSizeWords + 1;

                    // reset sizes
                    props.dwNameSize = dwNameSize;
                    props.dwDataSize = dwDataSize;
                }
            }
            // expected result at end of list
            if (SCCERR_INDEXOUTOFBOUNDS == res)
            {
                // is the list empty?
                if (0 == dwPropertyCount)
                {
                    SetWindowText(hTextWnd, TEXT("No property information."));
                }
                else
                {
                    SetWindowText(hTextWnd, pTextBuffer);
                }
                res = SCCERR_OK;
            }

PropsError:
            if (pTextBuffer)
            {
                HeapFree(GetProcessHeap(), 0, pTextBuffer);
                pTextBuffer = NULL;
            }
            if (props.pName);
            {
                HeapFree(GetProcessHeap(), 0, props.pName);
                props.pName = NULL;
            }
            if (props.pData);
            {
                HeapFree(GetProcessHeap(), 0, props.pData);
                props.pData = NULL;
            }
            if (res != SCCERR_OK)
            {
                SetWindowText(hTextWnd, TEXT("ERROR"));
            }
            return TRUE;
        }
        break;

    case WM_COMMAND:
        if (IDOK == wParam)
        {
            EndDialog(hDlg, 1);
        }
        return (TRUE);
        break;
    }
    return FALSE;
}

WIN_ENTRYSC BOOL WIN_ENTRYMOD PropertyDlgByIDProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            RECT	locRect;
            int		locX;
            int		locY;

            /*
            |   Center dialog
            */
            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            return TRUE;
        }
        break;

    case WM_COMMAND:
        if (IDOK == wParam)
        {
            HWND	hIdWnd;
            HWND	hNameWnd;
            HWND	hValueWnd;
            TCHAR*	pNameBuffer = NULL;
            TCHAR*	pValueBuffer = NULL;
            LRESULT res = SCCERR_OK;
            TCHAR	szID[NAME_SIZE];

            SCCVWGETPROP props;

            hIdWnd = GetDlgItem(hDlg, EDIT_PROPERTYID);
            hNameWnd = GetDlgItem(hDlg, EDIT_PROPERTYNAME);
            hValueWnd = GetDlgItem(hDlg, EDIT_PROPERTYVALUE);
            GetWindowText(hIdWnd, szID, NAME_SIZE);

            memset(&props, 0, sizeof(SCCVWGETPROP));
            props.dwSize = sizeof(SCCVWGETPROP);
            props.dwPropId = _wtoi(szID);
            res = SendMessage(hViewWnd, SCCVW_GETPROPERTY, SCCVW_GETPROP_BYID, (LPARAM) &props);
            if (SCCERR_INSUFFICIENTBUFFER == res) /* expecting this result, as buffer size was 0! */
            {
                props.pName = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.dwNameSize);
                props.pData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.dwDataSize);
                pNameBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.dwNameSize);
                pValueBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.dwDataSize);
                if (!props.pName || !props.pData || !pNameBuffer || !pValueBuffer)
                {
                    res = SCCERR_ALLOCFAILED;
                }
                else
                {
                    res = SendMessage(hViewWnd, SCCVW_GETPROPERTY, SCCVW_GETPROP_BYID, (LPARAM) &props);
                }
            }
            if (SCCERR_OK == res)
            {
                lstrcpy(pNameBuffer, props.pName);
                lstrcpy(pValueBuffer, props.pData);
                SetWindowText(hNameWnd, pNameBuffer);
                SetWindowText(hValueWnd, pValueBuffer);
            }
            else if (SCCERR_INDEXOUTOFBOUNDS == res)
            {
                SetWindowText(hNameWnd, TEXT("Not Found"));
                SetWindowText(hValueWnd, TEXT(""));
            }
            else
            {
                SetWindowText(hNameWnd, TEXT("Error!"));
                SetWindowText(hValueWnd, TEXT(""));
            }
            if (props.pName)
            {
                HeapFree(GetProcessHeap(), 0, props.pName);
            }
            if (props.pData)
            {
                HeapFree(GetProcessHeap(), 0, props.pData);
            }
            if (pNameBuffer)
            {
                HeapFree(GetProcessHeap(), 0, pNameBuffer);
            }
            if (pValueBuffer)
            {
                HeapFree(GetProcessHeap(), 0, pValueBuffer);
            }
        }
        else if (IDCANCEL == wParam)
        {
            EndDialog(hDlg, 1);
        }
        return (TRUE);
        break;
    }
    return FALSE;
}

WIN_ENTRYSC BOOL WIN_ENTRYMOD PropertyDlgByIndexProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            RECT	locRect;
            int		locX;
            int		locY;

            /*
            |   Center dialog
            */
            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            return TRUE;
        }
        break;

    case WM_COMMAND:
        if (IDOK == wParam)
        {
            HWND	hIdWnd;
            HWND	hNameWnd;
            HWND	hValueWnd;
            TCHAR*	pNameBuffer = NULL;
            TCHAR*	pValueBuffer = NULL;
            LRESULT res = SCCERR_OK;
            TCHAR	szID[NAME_SIZE];

            SCCVWGETPROP props;

            hIdWnd = GetDlgItem(hDlg, EDIT_PROPERTYID);
            hNameWnd = GetDlgItem(hDlg, EDIT_PROPERTYNAME);
            hValueWnd = GetDlgItem(hDlg, EDIT_PROPERTYVALUE);
            GetWindowText(hIdWnd, szID, NAME_SIZE);

            memset(&props, 0, sizeof(SCCVWGETPROP));
            props.dwSize = sizeof(SCCVWGETPROP);
            props.dwPropId = _wtoi(szID);
            res = SendMessage(hViewWnd, SCCVW_GETPROPERTY, SCCVW_GETPROP_BYINDEX, (LPARAM) &props);
            if (SCCERR_INSUFFICIENTBUFFER == res) /* expecting this result, as buffer size was 0! */
            {
                props.pName = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.dwNameSize);
                props.pData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.dwDataSize);
                pNameBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.dwNameSize);
                pValueBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, props.dwDataSize);
                if (!props.pName || !props.pData || !pNameBuffer || !pValueBuffer)
                {
                    res = SCCERR_ALLOCFAILED;
                }
                else
                {
                    /* we set dwPropId to the index, but it gets filled with the property id
                    * when we sent the message the first time. Reset it here.
                    */
                    props.dwPropId = _wtoi(szID);
                    res = SendMessage(hViewWnd, SCCVW_GETPROPERTY, SCCVW_GETPROP_BYINDEX, (LPARAM) &props);
                }
            }
            if (SCCERR_OK == res)
            {
                lstrcpy(pNameBuffer, props.pName);
                lstrcpy(pValueBuffer, props.pData);
                SetWindowText(hNameWnd, pNameBuffer);
                SetWindowText(hValueWnd, pValueBuffer);
            }
            else if (SCCERR_INDEXOUTOFBOUNDS == res)
            {
                SetWindowText(hNameWnd, TEXT("Not Found"));
                SetWindowText(hValueWnd, TEXT(""));
            }
            else
            {
                SetWindowText(hNameWnd, TEXT("Error!"));
                SetWindowText(hValueWnd, TEXT(""));
            }
            if (props.pName)
            {
                HeapFree(GetProcessHeap(), 0, props.pName);
            }
            if (props.pData)
            {
                HeapFree(GetProcessHeap(), 0, props.pData);
            }
            if (pNameBuffer)
            {
                HeapFree(GetProcessHeap(), 0, pNameBuffer);
            }
            if (pValueBuffer)
            {
                HeapFree(GetProcessHeap(), 0, pValueBuffer);
            }
        }
        else if (IDCANCEL == wParam)
        {
            EndDialog(hDlg, 1);
        }
        return (TRUE);
        break;
    }
    return FALSE;
}

WIN_ENTRYSC BOOL WIN_ENTRYMOD ResourceIDProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT                locRect;
    int                 locX;
    int                 locY;

    static PSCCVWRESOURCEID pSCCResID;
    TCHAR                           sDlgResID[11];
    VTWORD                          x;

    switch (message)
    {
    case WM_INITDIALOG:
        {

            /*
            |   Center dialog
            */

            pSCCResID = (PSCCVWRESOURCEID)lParam;

            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            for (x=0; x<6 && pSCCResID->szResourceID[x] != 0; x++)
                sDlgResID[x] = pSCCResID->szResourceID[x];
            sDlgResID[x] = 0;

            SetDlgItemText(hDlg,EDIT_RESOURCEID,sDlgResID);
        }

        return (TRUE);

    case WM_COMMAND:
        {

            switch(wParam)
            {
            case IDOK:
                /* The user hit OK so let's get the new resource ID */

                GetDlgItemText(hDlg,EDIT_RESOURCEID,sDlgResID,6); /* 6 = 5 + 1 for terminator */

                /* The limit is 5 Characters */
                for (x=0; x<6 && sDlgResID[x] != 0; x++)
                    pSCCResID->szResourceID[x] = (VTBYTE)sDlgResID[x];
                pSCCResID->szResourceID[x] = 0;

                EndDialog(hDlg, 1);

                return (TRUE);

            case IDCANCEL:

                EndDialog(hDlg, 0);
                return (TRUE);

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return(FALSE);
}

/*
* FileIDProc
* Dialog proc for the file type ID option.
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD FileIDProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT                locRect;
    int                 locX;
    int                 locY;

    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {

            /*
            |   Center dialog
            */

            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            /* Initially blank out the file type string. */
            SetDlgItemText(hDlg, STATIC_FILEIDSTRING, TEXT(""));
        }

        return (TRUE);

    case WM_COMMAND:
        {

            switch(wParam)
            {
            case IDOK:
                {
                    BYTE     pTypeNameByte[64];
                    TCHAR     pTypeName[64];
                    VTWORD    wType;
                    BOOL      bTranslated;

                    wType = (VTWORD)GetDlgItemInt(hDlg, EDIT_FILEID, &bTranslated, FALSE);
                    FIGetIDString(wType, (VTLPSTR)pTypeNameByte, sizeof(pTypeNameByte));
                    CopyByteStringToWordString(pTypeName, pTypeNameByte);
                    SetDlgItemText(hDlg, STATIC_FILEIDSTRING, pTypeName);
                    return TRUE;
                }
            case IDCANCEL:

                EndDialog(hDlg, 0);
                return (TRUE);

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return(FALSE);
}

/*
* UnmappableCharProc
* Dialog proc for the file type ID option.
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD UnmappableCharProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT              locRect;
    int                   locX;
    int                   locY;
    static VTWORD     wUnicodeChar;

    switch (message)
    {
    case WM_INITDIALOG:
        {
            TCHAR szStringForm[5];

            /*
            |   Center dialog
            */

            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            /*
            |   Initialize with character stored in options
            */
            wUnicodeChar = (VTWORD)lParam;
            wsprintf(szStringForm, TEXT("%x"), wUnicodeChar);
            SetDlgItemText(hDlg, EDIT_UNMAPPABLECHAR, szStringForm);
        }

        return (TRUE);

    case WM_COMMAND:
        {

            switch(wParam)
            {
            case IDOK:
                {
                    TCHAR   szStringForm[5];
                    VTWORD  wChar;

                    GetDlgItemText(hDlg, EDIT_UNMAPPABLECHAR, szStringForm, 5);
                    szStringForm[4] = '\0';

                    wChar = TextToHex(szStringForm);

                    /*
                    | We could be more stringent here - i.e. 0xfffe is not a legal unicode character -
                    | but for simplicity's sake we'll just use 0xffff as an error code and allow all
                    | entries between 0x0000 and 0xfffe
                    */
                    if (wChar == 0xffff)
                    {
                        MessageBox(hDlg, TEXT("Invalid Unicode character"), TEXT("Illegal entry"), MB_OK | MB_ICONSTOP);

                        wsprintf(szStringForm, TEXT("%x"), wUnicodeChar);
                        SetDlgItemText(hDlg, EDIT_UNMAPPABLECHAR, szStringForm);

                        return(FALSE);
                    }
                    else
                    {
                        wUnicodeChar = wChar;

                        EndDialog(hDlg, wUnicodeChar);
                        return (TRUE);
                    }
                }
            case IDCANCEL:

                EndDialog(hDlg, wUnicodeChar);
                return (TRUE);

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return(FALSE);
}


/*
* AddFontAliasProc
* Dialog proc for adding a font alias to the font alias map
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD AddFontAliasProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT                locRect;
    int                 locX;
    int                 locY;

    static PSCCVWFONTALIAS      pFontAlias;

    switch (message)
    {
    case WM_INITDIALOG:
        {
            pFontAlias = (PSCCVWFONTALIAS)lParam;

            pFontAlias->szwOriginal[0] = (VTWORD)0;
            pFontAlias->szwAlias[0] = (VTWORD)0;

            /*
            |   Center dialog
            */

            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);
        }

        return(TRUE);

    case WM_COMMAND:
        {

            switch(wParam)
            {
            case IDOK:
                {
                    /*
                    | NOTE:  We are only handling single-byte character strings here; to handle double-byte
                    |        character strings, more intelligent processing must be done with the edit controls.
                    */
                    TCHAR szOriginalName[SCCVW_FONTNAMEMAX], szAliasName[SCCVW_FONTNAMEMAX];
                    GetDlgItemText(hDlg, EDIT_ORIGINALNAME, (LPWSTR)szOriginalName, SCCVW_FONTNAMEMAX);
                    GetDlgItemText(hDlg, EDIT_ALIASNAME, (LPWSTR)szAliasName, SCCVW_FONTNAMEMAX);
                    lstrcpy(pFontAlias->szwOriginal,szOriginalName);
                    lstrcpy(pFontAlias->szwAlias, szAliasName);
                    EndDialog(hDlg, 1);
                    return(TRUE);
                }
            case IDCANCEL:

                EndDialog(hDlg, 0);
                return(TRUE);

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return(FALSE);
}




/*
* RemoveFontAliasProc
* Dialog proc for removing a font alias from the font alias map
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD RemoveFontAliasProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{ 
    RECT                locRect;
    int                 locX;
    int                 locY;

    static PREMOVEALIASMAP      RemoveFontAlias;

    switch (message)
    {
    case WM_INITDIALOG:
        {
            TCHAR    szFontName[ SCCVW_FONTNAMEMAX + 1 ];
            HWND    hOrigComboWnd;
            VTDWORD nDx;

            RemoveFontAlias = (PREMOVEALIASMAP)lParam;

            /* Center the dialog */
            GetWindowRect(hDlg,&locRect);
            locX = ( GetSystemMetrics( SM_CXSCREEN ) - ( locRect.right - locRect.left ) ) / 2;
            locY = ( GetSystemMetrics( SM_CYSCREEN ) - ( locRect.bottom - locRect.top ) ) / 2;
            SetWindowPos( hDlg, NULL, locX, locY, 0, 0, SWP_NOSIZE | SWP_NOZORDER );

            /* Set up the combo box for the original names */
            hOrigComboWnd = GetDlgItem( hDlg, COMBO_ORIGINALNAME );
            for( nDx = 0; nDx < RemoveFontAlias->sccAliasMap.dwCount; nDx++ )
            {
                lstrcpy(szFontName, RemoveFontAlias->sccAliasMap.szwOriginal[ nDx ]);
                ComboBox_AddString( hOrigComboWnd, szFontName );
            }
            ComboBox_SetCurSel( hOrigComboWnd, -1 );

            return(TRUE);
        }

    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case IDCANCEL:
            case IDOK:
                {
                    EndDialog(hDlg, 0);
                    return(TRUE);
                }

            case COMBO_ORIGINALNAME:
                {
                    VTDWORD             dwSelection;
                    HWND				hOrigComboWnd;
                    HWND				hAliasComboWnd;
                    SCCVWFONTALIAS      locFontAlias;
                    SCCVWOPTIONSPEC40   locOptionSpec;
                    TCHAR                szFontName[ SCCVW_FONTNAMEMAX + 1 ];

                    if (HIWORD(wParam) != CBN_SELCHANGE)
                        break;

                    /* first, get the selection */
                    hOrigComboWnd = GetDlgItem(hDlg, COMBO_ORIGINALNAME);
                    dwSelection = ComboBox_GetCurSel(hOrigComboWnd);
                    ComboBox_GetLBText(hOrigComboWnd, dwSelection, szFontName);

                    /* then, get the alias using the original name */
                    lstrcpy(locFontAlias.szwOriginal, szFontName);
                    locFontAlias.dwSize = sizeof(SCCVWFONTALIAS);
                    locFontAlias.dwAliasID = 0;
                    locFontAlias.dwFlags = SCCVW_FONTALIAS_ALIASNAME;

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);

                    if (RemoveFontAlias->bIsDisplay)
                        locOptionSpec.dwId = SCCID_DISPLAYFONTALIAS;
                    else
                        locOptionSpec.dwId = SCCID_PRINTFONTALIAS;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &locFontAlias;
                    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

                    /* Set up the combo box for the alias names */
                    hAliasComboWnd = GetDlgItem( hDlg, COMBO_ALIASNAME );
                    ComboBox_ResetContent( hAliasComboWnd );
                    UpdateComboBoxWithAliasNames( hAliasComboWnd, locFontAlias.szwAlias );

                    return(TRUE);
                }

            case BUTTON_DELETEALIAS:
                {
                    SCCVWFONTALIAS      locFontAlias;
                    SCCVWFONTALIASMAP   locFontAliasMap;
                    SCCVWOPTIONSPEC40   locOptionSpec;
                    HWND                hOrigComboWnd;
                    HWND                hAliasComboWnd;
                    VTDWORD             nDx;

                    /*
                    | NOTE:  We are only handling single-byte character strings here; to handle double-byte
                    |        character strings, more intelligent processing must be done with the edit controls.
                    */

                    TCHAR szOriginalName[SCCVW_FONTNAMEMAX + 1], szAliasName[SCCVW_FONTNAMEMAX + 1];

                    szOriginalName[0] = '\0';
                    szAliasName[0] = '\0';

                    GetDlgItemText(hDlg, COMBO_ORIGINALNAME, szOriginalName, SCCVW_FONTNAMEMAX);
                    GetDlgItemText(hDlg, COMBO_ALIASNAME, szAliasName, SCCVW_FONTNAMEMAX);

                    locFontAlias.dwSize = sizeof(SCCVWFONTALIAS);
                    locFontAlias.dwAliasID = 0;
                    locFontAlias.dwFlags = SCCVW_FONTALIAS_REMOVEALIASBYNAME;
                    lstrcpy(locFontAlias.szwOriginal, szOriginalName);
                    if( lstrcmp( szAliasName, TEXT("ALL") ) == 0 )
                    {
                        memset( locFontAlias.szwAlias, 0, sizeof( WORD ) * ( SCCVW_FONTNAMEMAX + 1 ) );
                    }
                    else
                    {
                        lstrcpy(locFontAlias.szwAlias, szAliasName);
                    }

                    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                    if (RemoveFontAlias->bIsDisplay)
                        locOptionSpec.dwId = SCCID_DISPLAYFONTALIAS;
                    else
                        locOptionSpec.dwId = SCCID_PRINTFONTALIAS;
                    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                    locOptionSpec.pData = &locFontAlias;
                    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

                    /* update the combo boxes */
                    hOrigComboWnd = GetDlgItem(hDlg, COMBO_ORIGINALNAME);
                    hAliasComboWnd = GetDlgItem(hDlg, COMBO_ALIASNAME);

                    SendMessage(hOrigComboWnd, CB_RESETCONTENT, 0, 0L);
                    SendMessage(hAliasComboWnd, CB_RESETCONTENT, 0, 0L);

                    /* build the font alias map */
                    BuildFontAliasMap(hViewWnd, &locFontAliasMap, RemoveFontAlias->bIsDisplay);

                    for (nDx = 0; nDx < locFontAliasMap.dwCount; nDx++)
                    {
                        lstrcpy(szOriginalName, locFontAliasMap.szwOriginal[nDx]);
                        ComboBox_AddString(hOrigComboWnd, szOriginalName);
                    }
                    ComboBox_SetCurSel(hOrigComboWnd, -1);
                    ComboBox_SetCurSel(hAliasComboWnd, -1);

                    DestroyFontAliasMap(&locFontAliasMap);
                    return(TRUE);
                }
            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return(FALSE);
}

VOID UpdateComboBoxWithAliasNames( HWND hAliasComboWnd, VTLPWORD lpwAliasList )
{
    TCHAR  szAliasList[ ( SCCVW_FONTNAMEMAX + 1 ) * SCCVW_MAXALIASES ];
    TCHAR  szDelim[ 2 ];
    TCHAR* szAlias;
    DWORD i = 0;

    lstrcpy( szDelim, TEXT(",\0") );

    /* Shove the word string into a char string... */
    while( lpwAliasList[ i ] != 0 )
    {
        szAliasList[ i ] = (TCHAR)( lpwAliasList[ i ] );
        ++i;
    }
    szAliasList[ i ] = 0;

    /* ...and then populate the combo box */
    szAlias = wcstok( szAliasList, szDelim );
    while( szAlias )
    {
        ComboBox_AddString( hAliasComboWnd, szAlias );
        szAlias = wcstok( NULL, szDelim );
    }						
    ComboBox_AddString( hAliasComboWnd, TEXT("ALL\0"));
    ComboBox_SetCurSel( hAliasComboWnd, 0 );
}

VOID BuildFontAliasMap(HANDLE hWnd, PSCCVWFONTALIASMAP pFontAliasMap, BOOL bIsDisplay)
{
    SCCVWFONTALIAS      locFontAlias;
    SCCVWOPTIONSPEC40   locOptionSpec;
    VTDWORD             nDx, dwCount = 0;
    UNUSED(hWnd);

    /* these options values never change */
    locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
    if (bIsDisplay)
        locOptionSpec.dwId = SCCID_DISPLAYFONTALIAS;
    else
        locOptionSpec.dwId = SCCID_PRINTFONTALIAS;
    locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;

    /* first, get the count */
    locFontAlias.dwSize = sizeof(SCCVWFONTALIAS);
    locFontAlias.dwAliasID = 0;
    locFontAlias.dwFlags = SCCVW_FONTALIAS_COUNT;
    locOptionSpec.pData = &locFontAlias;
    SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
    dwCount = locFontAlias.dwAliasID;

    /* now, get the aliases by ID */
    locFontAlias.dwFlags = SCCVW_FONTALIAS_GETALIASBYID;
    for (nDx = 0; nDx < dwCount; nDx++)
    {
        locFontAlias.dwAliasID = nDx;
        SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

        pFontAliasMap->hOriginal[nDx] = GlobalAlloc(GMEM_MOVEABLE, sizeof(VTWORD)*SCCVW_FONTNAMEMAX);
        pFontAliasMap->szwOriginal[nDx] = (LPWORD)GlobalLock(pFontAliasMap->hOriginal[nDx]);
        pFontAliasMap->hAlias[nDx] = GlobalAlloc(GMEM_MOVEABLE, sizeof(VTWORD)*SCCVW_FONTNAMEMAX);
        pFontAliasMap->szwAlias[nDx] = (LPWORD)GlobalLock(pFontAliasMap->hAlias[nDx]);
        CopyWordStringToWordString(pFontAliasMap->szwOriginal[nDx], locFontAlias.szwOriginal);
        CopyWordStringToWordString(pFontAliasMap->szwAlias[nDx], locFontAlias.szwAlias);
    }
    pFontAliasMap->dwCount = dwCount;
}


VOID DestroyFontAliasMap(PSCCVWFONTALIASMAP pFontAliasMap)
{
    VTDWORD nDx;

    for (nDx = 0; nDx < pFontAliasMap->dwCount; nDx++)
    {
        GlobalFree(pFontAliasMap->hOriginal[nDx]);
        GlobalFree(pFontAliasMap->hAlias[nDx]);
    }
}

VOID CopyByteStringToWordString(VTWORD * pStr1, BYTE * pStr2)
{
    while (*pStr2!=0)
    {
        *pStr1++ = *pStr2++;
    }

    *pStr1++ = *pStr2++;
}

VOID CopyWordStringToByteString(BYTE * pStr1, VTWORD * pStr2)
{
    while (*pStr2!=0)
    {
        *pStr1++ = (BYTE)*pStr2++;
    }

    *pStr1++ = (BYTE)*pStr2++;
}

VOID CopyWordStringToWordString(VTWORD * pStr1, VTWORD * pStr2)
{
    while (*pStr2!=0)
    {
        *pStr1++ = *pStr2++;
    }

    *pStr1++ = *pStr2++;
}

void myitoa(int iInteger, TCHAR *sString)
{
    int i,j,n;
    TCHAR c;

    /* lets get the digits to characters (in reverse order) */
    n = 0;
    do
    {
        sString[n++] = (TCHAR)((iInteger % 10) + '0');
    } while ((iInteger /= 10) > 0);

    /* now reverse the string */
    sString[n] = '\0';
    for (i =0, j = n-1; i < j; i++, j--)
    {
        c = sString[i];
        sString[i] = sString[j];
        sString[j] = c;
    }
}


WORD TextToHex(TCHAR *szString)
{
    TCHAR * szTemp;
    WORD        nDx, nDx2, wCount;
    WORD        wReturn = 0, wTemp;

    szTemp = szString;
    for (wCount = 0; *szTemp; wCount++)
        szTemp++;

    szTemp = szString;
    for (nDx = wCount; nDx > 0; nDx--, szTemp++)
    {
        wTemp = 1;
        for (nDx2 = nDx-1; nDx2 > 0; nDx2--)
            wTemp *= 16;

        if ((*szTemp >= 0x30) && (*szTemp <= 0x39))
            wReturn += wTemp * (*szTemp - 0x30);
        else if ((*szTemp >= 0x41) && (*szTemp <= 0x46))
            wReturn += wTemp * (*szTemp - 0x37);
        else if ((*szTemp >= 0x61) && (*szTemp <= 0x66))
            wReturn += wTemp * (*szTemp - 0x57);
        else
        {
            wReturn = 0xffff;
            break;
        }
    }

    return (wReturn);
}

WIN_ENTRYSC BOOL WIN_ENTRYMOD HelpAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {

            TCHAR   buffer[512];

            // Display sample app. version
            lstrcpy(buffer, TEXT("Options Sample Application\n"));
            lstrcat(lstrcat(buffer, TEXT(S_PRODUCT)), TEXT("\n"));

            lstrcat(lstrcat(buffer, TEXT("Build:\t")), TEXT(S_FILEVERSION));
            SetDlgItemText(hDlg,HELP_ABOUTBOXVERINFO,buffer);
        }
        return (TRUE);
    case WM_COMMAND:
        {

            switch(wParam)
            {
            case IDOK:
            case IDCANCEL:

                EndDialog(hDlg, 0);
                return (TRUE);

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return(FALSE);
}


/*
* GetOutputPathProc
* Dialog proc for requesting and changing a directory.
* Used by several options which deal with directories.
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD GetOutputPathProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT              locRect;
    int                   locX;
    int                   locY;

    DWORD dwAttrib;
    TCHAR szOutputPath[MAX_PATH_LENGTH];

    switch (message)
    {
    case WM_INITDIALOG:
        {
            lstrcpy(szOutputPath, (TCHAR *)lParam);

            /*
            | Center dialog
            */

            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            /*
            |   Initialize with character stored in options
            */
            if (nOutputPathMode == MENU_SYSTEM_OPTIONS_LOTUSNOTESDIRECTORY)
            {
                SetDlgItemText(hDlg, EDIT_SETPATH, szLotusNotesDirectory);
                SetWindowText(hDlg, TEXT("Lotus Notes Directory"));
            }
            else if (nOutputPathMode == MENU_SYSTEM_TEMPDIR)
            {
                SetDlgItemText(hDlg, EDIT_SETPATH, szTempDir);
                SetWindowText(hDlg, TEXT("Temporary File Output Path"));
            }
            else
            {
                SetDlgItemText(hDlg, EDIT_SETPATH, szOutputPath);
                SetWindowText(hDlg, TEXT("Archive File Output Path"));
            }
            return TRUE;
        }
        break;

    case WM_COMMAND:
        {
            switch(wParam)
            {
            case IDOK:
                {
                    GetDlgItemText(hDlg, EDIT_SETPATH, szOutputPath, MAX_PATH_LENGTH);

                    if (nOutputPathMode == MENU_OPTIONS_SETARCOUTPUTPATH)
                    {
                        if ( (dwAttrib = GetFileAttributes( szOutputPath ) ) !=  0xFFFFFFFF )
                        {
                            if ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY  )
                            {
                                lstrcpy(szArcOutputPath, szOutputPath);
                                EndDialog(hDlg, 1);
                                return(TRUE);
                            }
                            else
                            {
                                MessageBox(hDlg, TEXT("Path was not a directory"), TEXT("Options"), MB_OK);
                            }
                        }
                        else
                        {
                            MessageBox(hDlg, TEXT("Invalid Path"), TEXT("Options"), MB_OK);
                        }
                    }
                    else
                    {
                        lstrcpy(szTempDir, szOutputPath);
                        EndDialog(hDlg, 1);
                        return(TRUE);
                    }
                    return(FALSE);
                }
                break;

            case IDCANCEL:
                EndDialog(hDlg, 0);
                return(TRUE);

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return(FALSE);
}

/*
* GetTimzoneProc
* Dialog proc for changing the timezone setting
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD GetTimezoneProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT  locRect;
    int   locX;
    int   locY;

    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            /*
            | Center dialog
            */

            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            /*
            |   Initialize with value stored in options
            */
            if (SCC_TIMEZONE_USENATIVE == lTimeZone)
            {
                TIME_ZONE_INFORMATION   tz;
                LONG                    lBias = 0;

                GetTimeZoneInformation(&tz);
                lBias = tz.Bias + tz.StandardBias;
                // Gotta love Microsoft. Bias is local to UTC, not UTC to local...
                lBias = -lBias;
                lTimeZone = lBias / 15;
                CheckDlgButton(hDlg, BUTTON_USE_SYS_TZ, BST_CHECKED);
                EnableWindow(GetDlgItem(hDlg, EDIT_TIMEZONE), FALSE);
            }
            else
            {
                CheckDlgButton(hDlg, BUTTON_USE_SYS_TZ, BST_UNCHECKED);
                EnableWindow(GetDlgItem(hDlg, EDIT_TIMEZONE), TRUE);
            }
            SetDlgItemInt(hDlg, EDIT_TIMEZONE, lTimeZone/4, TRUE);
            return TRUE;
        }
        break;

    case WM_COMMAND:
        {
            switch(wParam)
            {
            case IDOK:
                {
                    BOOL bOk = TRUE;
                    int nUseSysZone = IsDlgButtonChecked(hDlg, BUTTON_USE_SYS_TZ);

                    if (BST_CHECKED == nUseSysZone)
                    {
                        lTimeZone = SCC_TIMEZONE_USENATIVE;
                    }
                    else
                    {
                        BOOL bWorked;
                        UINT nTimeZone = GetDlgItemInt(hDlg, EDIT_TIMEZONE, &bWorked, TRUE);
                        if (bWorked)
                        {
                            lTimeZone = (LONG) nTimeZone;
                        }
                        else
                        {
                            lTimeZone = 0;
                        }
                        if (lTimeZone > 24 || lTimeZone < -24)
                        {
                            MessageBox(hDlg, TEXT("Timezone values must be between -24 and 24"), TEXT("Options"), MB_OK);
                            bOk = FALSE;
                        }
                        else
                        {
                            lTimeZone *= 4;
                        }
                    }
                    if (bOk)
                    {
                        EndDialog(hDlg, 1);
                    }
                    return bOk;
                }
                break;

            case BUTTON_USE_SYS_TZ:
                {
                    int nUseSysZone = IsDlgButtonChecked(hDlg, BUTTON_USE_SYS_TZ);

                    if (BST_CHECKED == nUseSysZone)
                    {
                        TIME_ZONE_INFORMATION   tz;
                        LONG                    lBias = 0;

                        GetTimeZoneInformation(&tz);
                        lBias = tz.Bias + tz.StandardBias;
                        // Gotta love Microsoft. Bias is local to UTC, not UTC to local...
                        lBias = -lBias;
                        lTimeZone = lBias / 15;
                        SetDlgItemInt(hDlg, EDIT_TIMEZONE, lTimeZone/4, TRUE);

                        EnableWindow(GetDlgItem(hDlg, EDIT_TIMEZONE), FALSE);
                    }
                    else
                    {
                        EnableWindow(GetDlgItem(hDlg, EDIT_TIMEZONE), TRUE);
                    }
                }
                break;

            case IDCANCEL:
                EndDialog(hDlg, 0);
                return(TRUE);
                break;

            default:
                break;
            }
            break;
        }
        break;

    default:
        break;
    }

    return(FALSE);
}




/*
* GetIOBufSizeProc
* Dialog proc for changing the I/O Buffer Size settings
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD GetIOBufSizeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT  locRect;
    int   locX;
    int   locY;
    static PSCCBUFFEROPTIONS pbo;

    switch (message)
    {
    case WM_INITDIALOG:
        {
            /*
            | Center dialog
            */

            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

            /* Init fields */
            pbo = (PSCCBUFFEROPTIONS)lParam;

            SetDlgItemInt(hDlg, EDIT_READBUFSIZE, pbo->dwReadBufferSize, FALSE);
            SetDlgItemInt(hDlg, EDIT_MMAPBUFSIZE, pbo->dwMMapBufferSize, FALSE);
            SetDlgItemInt(hDlg, EDIT_TEMPBUFSIZE, pbo->dwTempBufferSize, FALSE);


            return TRUE;
        }
        break;

    case WM_COMMAND:
        {
            switch(wParam)
            {
            case IDOK:
                pbo->dwReadBufferSize = GetDlgItemInt(hDlg, EDIT_READBUFSIZE,   NULL, FALSE);
                pbo->dwMMapBufferSize = GetDlgItemInt(hDlg, EDIT_MMAPBUFSIZE,   NULL, FALSE);
                pbo->dwTempBufferSize = GetDlgItemInt(hDlg, EDIT_TEMPBUFSIZE, NULL, FALSE);
                EndDialog(hDlg, 1);
                return TRUE;
                break;

            case IDCANCEL:
                EndDialog(hDlg, 0);
                return TRUE;
                break;

            case BUT_DEFAULT:
                pbo->dwReadBufferSize = SCCBUFOPT_DEFAULT_READBUFSIZE;
                pbo->dwMMapBufferSize = SCCBUFOPT_DEFAULT_MMAPBUFSIZE;
                pbo->dwTempBufferSize = SCCBUFOPT_DEFAULT_TEMPBUFSIZE;
                SetDlgItemInt(hDlg, EDIT_READBUFSIZE, pbo->dwReadBufferSize, FALSE);
                SetDlgItemInt(hDlg, EDIT_MMAPBUFSIZE, pbo->dwMMapBufferSize, FALSE);
                SetDlgItemInt(hDlg, EDIT_TEMPBUFSIZE, pbo->dwTempBufferSize, FALSE);
                break;

            default:
                break;
            }
            break;
        }
        break;

    default:
        break;
    }

    return FALSE;
}

/*
* DefaultCharsetProc
* Dialog proc for the Default Character Set option.
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD DefaultCharsetProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT                locRect;
    int                 locX;
    int                 locY;

    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            /*
            |   Center dialog
            */

            HWND hComboWnd;
            LRESULT lRes;
            int	i, iSave = 0;
            SCCVWOPTIONSPEC40	locOptionSpec;
            DWORD				dwCurCharSet = 0xffffffff;

            GetWindowRect(hDlg,&locRect);
            locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
            locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);
            hComboWnd = GetDlgItem(hDlg, COMBO_CHARSETID);

            locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
            locOptionSpec.dwId = SCCOPT_DEFAULTINPUTCHARSET;
            locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
            locOptionSpec.pData = &dwCurCharSet;
            SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
            lRes = CB_ERR;
            for (i=0; i<NUM_LEGIT_CHARSETS; i++)
            {
                lRes = SendMessage(hComboWnd, CB_ADDSTRING, 0, (LPARAM) aszCharsets[i]);
                lRes = SendMessage(hComboWnd, CB_SETITEMDATA, i /* which item */, (LPARAM)adwCharsets[i]);
                if (adwCharsets[i] == dwCurCharSet)
                    iSave = i;
            }
            lRes = SendMessage(hComboWnd, CB_SETCURSEL, iSave, iSave);
        }

        return (TRUE);

    case WM_COMMAND:
        {

            switch(wParam)
            {
            case IDOK:
                {
                    HWND	hComboWnd;
                    LRESULT lRes;

                    hComboWnd = GetDlgItem(hDlg, COMBO_CHARSETID);
                    lRes = SendMessage(hComboWnd, CB_GETCURSEL, 0, 0);
                    if (lRes != CB_ERR)
                    {
                        lRes = SendMessage(hComboWnd, CB_GETITEMDATA, lRes, 0);
                    }

                    if (lRes != CB_ERR)
                    {
                        SCCVWOPTIONSPEC40	locOptionSpec;
                        DWORD				dwCharSet = (DWORD) lRes;

                        locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
                        locOptionSpec.dwId = SCCOPT_DEFAULTINPUTCHARSET;
                        locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
                        locOptionSpec.pData = &dwCharSet;
                        SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
                    }

                    EndDialog(hDlg, 1);
                    return TRUE;
                }
            case IDCANCEL:

                EndDialog(hDlg, 0);
                return (TRUE);

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return(FALSE);
}

/*
* FileAccessDialogProc
* Dialog proc for the file access dialog option.
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD FileAccessDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    VTWORD  wCount;
    RECT    locRect;
    int     locX;
    int     locY;

    BOOL  bRet = FALSE;  /* return value */

    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        /*
        |   Center dialog
        */
        GetWindowRect(hDlg, &locRect);
        locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
        locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
        SetWindowPos(hDlg, NULL, locX, locY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        /* Clear the password string */
        SetDlgItemText(hDlg, EDIT_INPUTPASSWORD, TEXT(""));

        /* Set a character limit on the input text */
        (VOID)SendMessage(hDlg, EM_LIMITTEXT, 256, 0);

        bRet = TRUE;
        break;
    case WM_COMMAND:
        switch(wParam)
        {
        case IDOK:
            wCount = (VTWORD)GetDlgItemText(hDlg, EDIT_INPUTPASSWORD, szCharBuffer, MAX_CHARBUF_LENGTH);
            EndDialog(hDlg, wCount);
            bRet = TRUE;
            break;
        case IDCANCEL:
            EndDialog(hDlg, 0);
            bRet = TRUE;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return (bRet);
}

/*
* NotesAccessDialogProc
* Dialog proc for the Notes ID access dialog option.
*/

WIN_ENTRYSC BOOL WIN_ENTRYMOD NotesAccessDialogProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    VTWORD  wCount;
    RECT    locRect;
    int     locX;
    int     locY;

    BOOL  bRet = FALSE;  /* return value */

    UNUSED(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        /*
        |   Center dialog
        */
        GetWindowRect(hDlg, &locRect);
        locX = (GetSystemMetrics(SM_CXSCREEN) - (locRect.right - locRect.left)) / 2;
        locY = (GetSystemMetrics(SM_CYSCREEN) - (locRect.bottom - locRect.top)) / 2;
        SetWindowPos(hDlg, NULL, locX, locY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        /* Clear the password string */
        SetDlgItemText(hDlg, EDIT_INPUTNOTESID, TEXT(""));

        /* Set a character limit on the input text */
        (VOID)SendMessage(hDlg, EM_LIMITTEXT, 256, 0);

        bRet = TRUE;
        break;
    case WM_COMMAND:
        switch(wParam)
        {
        case IDOK:
            wCount = (VTWORD)GetDlgItemText(hDlg, EDIT_INPUTNOTESID, szCharBuffer, MAX_CHARBUF_LENGTH);
            EndDialog(hDlg, wCount);
            bRet = TRUE;
            break;
        case IDCANCEL:
            EndDialog(hDlg, 0);
            bRet = TRUE;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return (bRet);
}

/* Sign-of-Life Indicator */
void ClearBusy(void)
{
    HDC hdest;
    POINT p;
    RECT r;

    hdest = GetWindowDC(hMainWnd);
    GetWindowRect(hMainWnd,&r);

    p.x = 4;
    p.y = 2;
    ClientToScreen(hMainWnd,&p);
    p.x -= r.left;
    p.y -= r.top;

    r.left = p.x;
    r.top = p.y;
    r.right = r.left + 16;
    r.bottom = r.top + 16;

    FillRect(hdest,&r,GetStockObject(LTGRAY_BRUSH));

    ReleaseDC(hMainWnd,hdest);

    rad = TRUE;
    bAbort = FALSE;
}


VTDWORD DAStatCallback( VTDWORD dwStatus, VTDWORD dwStatFlags, VTSYSVAL pData, VTSYSVAL pUserData)
{
    static int state = 0;
    static HBITMAP hBullet[2] = {0,0};
    HDC hdest;
    HDC hsrc;
    POINT p;
    RECT r;
    HBITMAP holdbmp;

    UNUSED(dwStatus);
    UNUSED(dwStatFlags);
    UNUSED(pData);
    UNUSED(pUserData);

    if (bAbort)
    {
        bAbort = FALSE;
        return SCCERR_CANCEL;
    }

    if (rad)
        return SCCERR_OK;

    if (hBullet[0] == 0)
    {
        hBullet[0] = LoadBitmap(hInst,MAKEINTRESOURCE(BULLET0));
        hBullet[1] = LoadBitmap(hInst,MAKEINTRESOURCE(BULLET1));
    }
    hdest = GetWindowDC(hMainWnd);
    hsrc = CreateCompatibleDC(hdest);
    holdbmp = SelectObject(hsrc,hBullet[state]);

    GetWindowRect(hMainWnd,&r);

    p.x = 4;
    p.y = 2;
    ClientToScreen(hMainWnd,&p);
    p.x -= r.left;
    p.y -= r.top;

    BitBlt(hdest,p.x,p.y,16,16,hsrc,0,0,SRCCOPY);
    SelectObject(hsrc,holdbmp);

    DeleteDC(hsrc);
    ReleaseDC(hMainWnd,hdest);

    state ^= 1;

    return SCCERR_OK;
}

HINSTANCE g_hLOModule = NULL;

BOOL OPGetLOString(DWORD dwID, LPTSTR pString, DWORD dwLen, HWND hWnd)
{		
    UNUSED(hWnd);

	pString[0] = 0;

	if (g_hLOModule == NULL)
	{
		SCCVWOPTIONSPEC40	locOptionSpec;
        SCCVWRESOURCEID		SCCResID;        
		WCHAR				buf[128];

		SCCResID.dwSize = sizeof (SCCVWRESOURCEID);
        SCCResID.szResourceID[0] = 0;

        locOptionSpec.dwSize = sizeof(SCCVWOPTIONSPEC40);
        locOptionSpec.dwId = SCCID_RESOURCELIBRARYID;
        locOptionSpec.dwFlags = SCCVWOPTION_CURRENT;
        locOptionSpec.pData = &SCCResID;
        SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

		wsprintf( buf, TEXT("scclo%s.dll"),SCCResID.szResourceID);					
		g_hLOModule = LoadLibrary(buf);
	}

	if (g_hLOModule && LoadString(g_hLOModule, dwID, pString, dwLen) > 0)
			return TRUE;
	
	return FALSE;
}


BOOL EmailDialogGetActiveWindowsBySubtype(HWND hDlg, DWORD SubType, HWND *hAvailableWnd, HWND *hSelectedWnd)
{
	*hSelectedWnd  = NULL;
	*hAvailableWnd = NULL;
	
	switch (SubType)
	{
	case SCCUT_MAILTYPE_EMAIL:
		*hSelectedWnd  = GetDlgItem(hDlg,SCC_EMAIL_SELECTED);
		*hAvailableWnd = GetDlgItem(hDlg,SCC_EMAIL_AVAILABLE);							
		break;

	case SCCUT_MAILTYPE_JOURNAL:					
		*hSelectedWnd  = GetDlgItem(hDlg,SCC_JOURNAL_SELECTED);
		*hAvailableWnd = GetDlgItem(hDlg,SCC_JOURNAL_AVAILABLE);							
		break;

	case SCCUT_MAILTYPE_CONTACT:
		*hSelectedWnd  = GetDlgItem(hDlg,SCC_CONTACT_SELECTED);
		*hAvailableWnd = GetDlgItem(hDlg,SCC_CONTACT_AVAILABLE);
		break;

	case SCCUT_MAILTYPE_NOTE:
		*hSelectedWnd  = GetDlgItem(hDlg,SCC_NOTE_SELECTED);
		*hAvailableWnd = GetDlgItem(hDlg,SCC_NOTE_AVAILABLE);							
		break;

	case SCCUT_MAILTYPE_APPOINTMENT:
		*hSelectedWnd  = GetDlgItem(hDlg,SCC_APPOINTMENT_SELECTED);
		*hAvailableWnd = GetDlgItem(hDlg,SCC_APPOINTMENT_AVAILABLE);							
		break;

	case SCCUT_MAILTYPE_TASK:
		*hSelectedWnd  = GetDlgItem(hDlg,SCC_TASK_SELECTED);
		*hAvailableWnd = GetDlgItem(hDlg,SCC_TASK_AVAILABLE);							
		break;

	case SCCUT_MAILTYPE_DISTROLIST:
		*hSelectedWnd  = GetDlgItem(hDlg,SCC_DISTROLIST_SELECTED);
		*hAvailableWnd = GetDlgItem(hDlg,SCC_DISTROLIST_AVAILABLE);							
		break;

	case SCCUT_MAILTYPE_POST:
		*hSelectedWnd  = GetDlgItem(hDlg,SCC_POST_SELECTED);
		*hAvailableWnd = GetDlgItem(hDlg,SCC_POST_AVAILABLE);
		break;
	default:		
		return FALSE;
	}
	return TRUE;
}

BOOL EmailDialogGetActiveWindowsFromDropdown(HWND hDlg, HWND *hAvailableWnd, HWND *hSelectedWnd)
{
	HWND    hDropdown = GetDlgItem(hDlg, SCC_EMAIL_SUBTYPE);
	int lRes;

	*hSelectedWnd  = NULL;
	*hAvailableWnd = NULL;
	
	if ((lRes = (int)SendMessage(hDropdown, CB_GETCURSEL, 0, 0)) == CB_ERR)
		return FALSE;

	return EmailDialogGetActiveWindowsBySubtype(hDlg,(DWORD)SendMessage(hDropdown, CB_GETITEMDATA, lRes, 0),hAvailableWnd,hSelectedWnd);
}


/*
 *   CustomEmailHeaderProc
 *   Dialog proc for setting up custom email headers
 */
typedef struct emlheaderchoice
{
	VTDWORD dwId;
	VTBOOL  bSet;
} HEADERCHOICE, *PHEADERCHOICE;

DWORD mailtypes[8] = {SCCUT_MAILTYPE_EMAIL, SCCUT_MAILTYPE_JOURNAL, SCCUT_MAILTYPE_CONTACT, 
                      SCCUT_MAILTYPE_NOTE, SCCUT_MAILTYPE_APPOINTMENT, SCCUT_MAILTYPE_TASK,
					  SCCUT_MAILTYPE_POST, SCCUT_MAILTYPE_DISTROLIST };

WIN_ENTRYSC BOOL WIN_ENTRYMOD  CustomEmailHeaderProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	
	HWND	hSelectedWnd;
	HWND	hAvailableWnd;
	static HEADERCHOICE added_items[SCCCA_MAIL_END - SCCCA_MAIL_BASE];
	static HEADERCHOICE removed_items[SCCCA_MAIL_END - SCCCA_MAIL_BASE];

    UNUSED(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        {            
            /* Center the dialog */
			RECT                locRect;
			int                 locX;
			int                 locY;

            GetWindowRect(hDlg,&locRect);
            locX = ( GetSystemMetrics( SM_CXSCREEN ) - ( locRect.right - locRect.left ) ) / 2;
            locY = ( GetSystemMetrics( SM_CYSCREEN ) - ( locRect.bottom - locRect.top ) ) / 2;
            SetWindowPos( hDlg, NULL, locX, locY, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
		}


		{
			/* Load the selection boxes */			
			int header;			
			HWND    hDropdown;
			DWORD	dwSubType;
			int		pos = 0;
			int i;
			

            SCCUTEMAILHEADERINFO HeaderInfo;

			/* Initialize our list of changed items */
			for(i = 0; i < (SCCCA_MAIL_END - SCCCA_MAIL_BASE); ++i)
			{
				added_items[i].bSet = FALSE;
				added_items[i].dwId = 0;
				removed_items[i].bSet = FALSE;
				removed_items[i].dwId = 0;
			}

			hDropdown = GetDlgItem(hDlg,SCC_EMAIL_SUBTYPE);		
			SendMessage(hDropdown, CB_RESETCONTENT, 0, 0);

			HeaderInfo.wsMimeHeaderName[0] = 0;
			HeaderInfo.wsMimeHeaderLabel[0] = 0;			

			for (dwSubType = SCCUT_MAILTYPE_EMAIL; dwSubType <= SCCUT_MAILTYPE_DISTROLIST; dwSubType<<=1)
			{				                
				switch (dwSubType)
				{
				case SCCUT_MAILTYPE_EMAIL:			pos = (int)SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM) TEXT("Email"));			break;
				case SCCUT_MAILTYPE_JOURNAL:		pos = (int)SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM) TEXT("Journal"));		break;
				case SCCUT_MAILTYPE_CONTACT:		pos = (int)SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM) TEXT("Contact"));		break;
				case SCCUT_MAILTYPE_NOTE:			pos = (int)SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM) TEXT("Note"));			break;
				case SCCUT_MAILTYPE_APPOINTMENT:	pos = (int)SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)TEXT("Appointment"));		break;
				case SCCUT_MAILTYPE_TASK:			pos = (int)SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM) TEXT("Task"));			break;
				case SCCUT_MAILTYPE_DISTROLIST:		pos = (int)SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM) TEXT("Distrolist"));		break;
				case SCCUT_MAILTYPE_POST:			pos = (int)SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM) TEXT("Post"));			break;
				}

				EmailDialogGetActiveWindowsBySubtype(hDlg,dwSubType, &hAvailableWnd,&hSelectedWnd);

				/* Enable the horizontal scrollbars in the listboxes, because some headers have long names. */
				ListBox_SetHorizontalExtent(hSelectedWnd,300);
				ListBox_SetHorizontalExtent(hAvailableWnd,300);


				ShowWindow(hSelectedWnd,SW_HIDE);
				ShowWindow(hAvailableWnd,SW_HIDE);

				SendMessage(hDropdown, CB_SETITEMDATA, pos, (LPARAM)dwSubType);

				

				for (header = SCCCA_MAIL_BASE ; header <= SCCCA_MAIL_END ; header++)
				{	
					SCCVWOPTIONSPEC40       locOptionSpec;
					TCHAR					szPropertyString[100];
					
					if (OPGetLOString(dwStrId[header - SCCCA_MAIL_BASE],(LPTSTR)szPropertyString,100,hDlg))
					{
						HeaderInfo.dwHeaderID = header;
						
						locOptionSpec.dwSize	= sizeof(SCCVWOPTIONSPEC40);
						locOptionSpec.dwId		= SCCOPT_MAILHEADERVISIBLE;
						locOptionSpec.dwFlags	= SCCVWOPTION_CURRENT;
						locOptionSpec.pData		= &HeaderInfo;
						SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

						if (HeaderInfo.dwSubtypeID & dwSubType)
						{
							int pos = (int)SendMessage(hSelectedWnd, LB_ADDSTRING, 0, (LPARAM)szPropertyString); 
							SendMessage(hSelectedWnd, LB_SETITEMDATA, pos, (LPARAM) header); 
						}
						else
						{
							int pos = (int)SendMessage(hAvailableWnd, LB_ADDSTRING, 0, (LPARAM)szPropertyString); 
							SendMessage(hAvailableWnd, LB_SETITEMDATA, pos, (LPARAM) header); 
						}
					}
				}
			}

			SendMessage(hDropdown, CB_SETCURSEL, 0, (LPARAM)dwSubType);
			SetFocus(hDropdown);
			EmailDialogGetActiveWindowsFromDropdown(hDlg,&hAvailableWnd,&hSelectedWnd);
			ShowWindow(hSelectedWnd,SW_SHOW);
			ShowWindow(hAvailableWnd,SW_SHOW);
		}
		

		/* This is only used during initilization */
		if (g_hLOModule)
		{
			FreeLibrary(g_hLOModule);
			g_hLOModule = NULL;
		}


        return(TRUE);
        

    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case IDCANCEL:
				EndDialog(hDlg, 0);
                return(FALSE);

            case IDOK:
			{				
				int i;				
				SCCVWOPTIONSPEC40       locOptionSpec;
				SCCUTEMAILHEADERINFO HeaderInfo;

				HeaderInfo.wsMimeHeaderName[0] = 0;
				HeaderInfo.wsMimeHeaderLabel[0] = 0;

				locOptionSpec.dwSize	= sizeof(SCCVWOPTIONSPEC40);
				locOptionSpec.dwFlags	= SCCVWOPTION_CURRENT;
				locOptionSpec.pData		= &HeaderInfo;

				for (i=SCCCA_MAIL_BASE;i<SCCCA_MAIL_END;i++)
				{
					HeaderInfo.dwHeaderID	= i;

					if(added_items[i - SCCCA_MAIL_BASE].bSet)
					{
						locOptionSpec.dwId		= SCCOPT_MAILHEADERVISIBLE;
						HeaderInfo.dwSubtypeID  = added_items[i - SCCCA_MAIL_BASE].dwId & SCCUT_MAILTYPE_AVAILABLE_MASK;
						SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
					}
					if(removed_items[i - SCCCA_MAIL_BASE].bSet)
					{
						locOptionSpec.dwId		= SCCOPT_MAILHEADERHIDDEN;
						HeaderInfo.dwSubtypeID  = removed_items[i - SCCCA_MAIL_BASE].dwId & SCCUT_MAILTYPE_AVAILABLE_MASK;
						SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
					}
				}

                EndDialog(hDlg, 1);
                return(TRUE);
			}
            
			case SCC_ADD_HEADER:
			case SCC_REMOVE_HEADER:
				{
					HWND	to, from, hDrop;
					TCHAR	szPropertyString[100];
					int items[SCCCA_MAIL_END - SCCCA_MAIL_BASE+1];
					int count;
					int pos;
					int i;
					VTDWORD header;
					VTBOOL bShow;
					int mailType;

					hDrop = GetDlgItem(hDlg,SCC_EMAIL_SUBTYPE);

					if(!hDrop)
						return FALSE;
					mailType = (int)SendMessage(hDrop, CB_GETCURSEL, 0, 0);
					if((mailType == CB_ERR) || (mailType < 0) || (mailType > (sizeof(mailtypes) / sizeof(DWORD))))
						return FALSE;
					/* Convert from pos to SCCUT mailtype... */
					mailType = mailtypes[mailType];
					if (EmailDialogGetActiveWindowsFromDropdown(hDlg, &hAvailableWnd, &hSelectedWnd))
					{
						if (LOWORD(wParam) == SCC_ADD_HEADER)
						{
							from = hSelectedWnd;
							to   = hAvailableWnd;
							bShow = FALSE;
						}
						else
						{
							to   = hSelectedWnd;
							from = hAvailableWnd;
							bShow = TRUE;
						}
						if ((count = (int)SendMessage(from, LB_GETSELCOUNT, 0, 0)) > 0)
						{
							count = (int)SendMessage(from,LB_GETSELITEMS,SCCCA_MAIL_END - SCCCA_MAIL_BASE,(LPARAM)items);
							/* Loop backwards, so earlier removals don't disturb the count of later items */
							for (i = count-1;i>=0;i--)
							{
								SendMessage(from,LB_GETTEXT,items[i],(LPARAM)szPropertyString);
								header = (DWORD)SendMessage(from,LB_GETITEMDATA,items[i],0);
								if(bShow)
								{
									/* Check to see if we previously turned it off. If so, 
									   just unset the "turn off" */
									if((removed_items[header - SCCCA_MAIL_BASE].bSet) &&
									   (removed_items[header - SCCCA_MAIL_BASE].dwId & mailType))
									{
										removed_items[header - SCCCA_MAIL_BASE].dwId &= ~mailType;
										if(removed_items[header - SCCCA_MAIL_BASE].dwId == 0)
											removed_items[header - SCCCA_MAIL_BASE].bSet = FALSE;
									}
									else
									{
										added_items[header - SCCCA_MAIL_BASE].bSet = TRUE;
										added_items[header - SCCCA_MAIL_BASE].dwId |= mailType;
									}
									
								}
								else
								{
									/* Check to see if we previously turned it on. If so, 
									   just unset the "turn on" */
									if((added_items[header - SCCCA_MAIL_BASE].bSet) &&
									   (added_items[header - SCCCA_MAIL_BASE].dwId & mailType))
									{
										added_items[header - SCCCA_MAIL_BASE].dwId &= ~mailType;
										if(added_items[header - SCCCA_MAIL_BASE].dwId == 0)
											added_items[header - SCCCA_MAIL_BASE].bSet = FALSE;
									}
									else
									{
										removed_items[header - SCCCA_MAIL_BASE].bSet = TRUE;
										removed_items[header - SCCCA_MAIL_BASE].dwId |= mailType;
									}
								}
								pos = (int)SendMessage(to, LB_ADDSTRING, 0, (LPARAM)szPropertyString); 
								SendMessage(to, LB_SETITEMDATA, pos, (LPARAM) header); 

								SendMessage(from,LB_DELETESTRING,items[i],0);								
							}
						}						
					}				
				}
				break;


			case SCC_EMAIL_SUBTYPE:
				{
					WINDOWINFO WindowInfo;

					EmailDialogGetActiveWindowsFromDropdown(hDlg, &hAvailableWnd, &hSelectedWnd);						
					GetWindowInfo(hSelectedWnd,&WindowInfo);

					if (!(WindowInfo.dwStyle&WS_VISIBLE))
					{
						ShowWindow(GetDlgItem(hDlg,SCC_EMAIL_SELECTED),			SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_EMAIL_AVAILABLE),		SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_JOURNAL_SELECTED),		SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_JOURNAL_AVAILABLE),		SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_CONTACT_SELECTED),		SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_CONTACT_AVAILABLE),		SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_NOTE_SELECTED),			SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_NOTE_AVAILABLE),			SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_APPOINTMENT_SELECTED),	SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_APPOINTMENT_AVAILABLE),	SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_TASK_SELECTED),			SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_TASK_AVAILABLE),			SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_DISTROLIST_SELECTED),	SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_DISTROLIST_AVAILABLE),	SW_HIDE);											
						ShowWindow(GetDlgItem(hDlg,SCC_POST_SELECTED),			SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,SCC_POST_AVAILABLE),			SW_HIDE);
				
						ShowWindow(hSelectedWnd,								SW_SHOW);
						ShowWindow(hAvailableWnd,								SW_SHOW);
					}            
				}
				break;

			case SCC_ADD_CUSTOM:
				DialogBoxParam(hInst,
						MAKEINTRESOURCE(SCC_NONSTANDARDHEADERSDIALOG),
						hDlg,
						(DLGPROC)NonStandardHeaderProc,
						(LPARAM)NULL);
				break;
            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return(FALSE);
}


SCCUTEMAILHEADERINFO g_NonStandardMailHeaders[MAX_NONSTANDARD_HEADERS];

VOID NonStandardSelectHeaderByIndex(HWND hDlg, BOOL bSetText)
{
	HWND				hListBox;
	HWND				hLabelEdit;
	HWND				hHeaderEdit;
	int					sel;
	int					current;
	static int			last_sel = LB_ERR;

	hListBox = GetDlgItem(hDlg,SCC_NONSTANDARD_LIST);
	
	sel = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);

	if (sel != last_sel)
	{
		hHeaderEdit = GetDlgItem(hDlg,SCC_NONSTANDARD_HEADER);
		hLabelEdit = GetDlgItem(hDlg,SCC_NONSTANDARD_LABEL);
		if (sel != LB_ERR)
		{				
			current =  (int)SendMessage(hListBox, LB_GETITEMDATA, sel, 0);

			if (g_NonStandardMailHeaders[current].wsMimeHeaderName[0] != 0)
			{
				if (bSetText)
					Edit_SetText(hHeaderEdit,g_NonStandardMailHeaders[current].wsMimeHeaderName);
				Edit_SetText(hLabelEdit, g_NonStandardMailHeaders[current].wsMimeHeaderLabel);

	#define UPDATE_CHECKBOX(type)															\
		if (g_NonStandardMailHeaders[current].dwSubtypeID & SCCUT_MAILTYPE_##type##)		\
				CheckDlgButton(hDlg,SCC_NONSTANDARD_##type##_CHECKBOX,BST_CHECKED);			\
				else																		\
				CheckDlgButton(hDlg,SCC_NONSTANDARD_##type##_CHECKBOX,BST_UNCHECKED)

				UPDATE_CHECKBOX(EMAIL);
				UPDATE_CHECKBOX(JOURNAL);
				UPDATE_CHECKBOX(CONTACT);
				UPDATE_CHECKBOX(TASK);
				UPDATE_CHECKBOX(APPOINTMENT);
				UPDATE_CHECKBOX(DISTROLIST);
				UPDATE_CHECKBOX(POST);
				UPDATE_CHECKBOX(NOTE);
	#undef UPDATE_CHECKBOX			

				Button_SetText(GetDlgItem(hDlg,SCC_ADD_NONSTANDARD),TEXT("Edit Header"));
				Button_Enable(GetDlgItem(hDlg,SCC_ADD_NONSTANDARD),TRUE);
				Button_Enable(GetDlgItem(hDlg,SCC_REMOVE_NONSTANDARD),TRUE);
			}			
		}
		else
		{
			int locCount;
	#define EMPTY_CHECKBOX(type)														\
		CheckDlgButton(hDlg,SCC_NONSTANDARD_##type##_CHECKBOX,BST_UNCHECKED);			
				EMPTY_CHECKBOX(EMAIL);
				EMPTY_CHECKBOX(JOURNAL);
				EMPTY_CHECKBOX(CONTACT);
				EMPTY_CHECKBOX(TASK);
				EMPTY_CHECKBOX(APPOINTMENT);
				EMPTY_CHECKBOX(DISTROLIST);
				EMPTY_CHECKBOX(POST);
				EMPTY_CHECKBOX(NOTE);
	#undef EMPTY_CHECKBOX			

			Edit_SetText(hLabelEdit,TEXT(""));

			Button_SetText(GetDlgItem(hDlg,SCC_ADD_NONSTANDARD),TEXT("Add Header"));
			locCount = (int)SendMessage(hListBox, LB_GETCOUNT, 0,0);
			if ((locCount >= MAX_NONSTANDARD_HEADERS) || (locCount == 0))
				Button_Enable(GetDlgItem(hDlg,SCC_ADD_NONSTANDARD),FALSE);
			else
				Button_Enable(GetDlgItem(hDlg,SCC_ADD_NONSTANDARD),TRUE);
			Button_Enable(GetDlgItem(hDlg,SCC_REMOVE_NONSTANDARD),FALSE);			
		}
		last_sel = sel;
	}
}


WIN_ENTRYSC BOOL WIN_ENTRYMOD NonStandardHeaderProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND				 hListBox;

    UNUSED(lParam);


	switch (message)
    {
	case WM_INITDIALOG:
        {            
            /* Center the dialog */
			RECT                locRect;
			int                 locX;
			int                 locY;

            GetWindowRect(hDlg,&locRect);
            locX = ( GetSystemMetrics( SM_CXSCREEN ) - ( locRect.right - locRect.left ) ) / 2;
            locY = ( GetSystemMetrics( SM_CYSCREEN ) - ( locRect.bottom - locRect.top ) ) / 2;
            SetWindowPos( hDlg, NULL, locX, locY, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
		}

		
		{
			int i;
			int pos;
			SCCVWOPTIONSPEC40    locOptionSpec;			
			

			hListBox = GetDlgItem(hDlg,SCC_NONSTANDARD_LIST);		
			SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
					
			
			/* Populate List Box */	
			locOptionSpec.dwSize	= sizeof(SCCVWOPTIONSPEC40);
			locOptionSpec.dwId		= SCCOPT_MAILHEADERVISIBLE;
			locOptionSpec.dwFlags	= SCCVWOPTION_CURRENT;			
			
			for (i=0;i<MAX_NONSTANDARD_HEADERS;i++)
			{
				locOptionSpec.pData		= &(g_NonStandardMailHeaders[i]);

				g_NonStandardMailHeaders[i].dwHeaderID = NONSTANDARD_HEADER_ID_BASE + i;

				g_NonStandardMailHeaders[i].wsMimeHeaderName[0] = 0;
				SendMessage(hViewWnd,SCCVW_GETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);

				if (g_NonStandardMailHeaders[i].wsMimeHeaderName[0] != 0)
				{
					pos = (int)SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)g_NonStandardMailHeaders[i].wsMimeHeaderName); 
					SendMessage(hListBox, LB_SETITEMDATA, pos, (LPARAM)i); 
				}
			}
			
			if (SendMessage(hListBox, LB_GETCOUNT, 0,0)>= MAX_NONSTANDARD_HEADERS)
				EnableWindow(GetDlgItem(hDlg,SCC_ADD_HEADER),FALSE);
			EnableWindow(GetDlgItem(hDlg,SCC_REMOVE_HEADER),FALSE);
			Edit_LimitText(GetDlgItem(hDlg,SCC_NONSTANDARD_HEADER),255);
			Edit_LimitText(GetDlgItem(hDlg,SCC_NONSTANDARD_LABEL),255);
			Button_Enable(GetDlgItem(hDlg,SCC_REMOVE_NONSTANDARD),FALSE);
			Button_Enable(GetDlgItem(hDlg,SCC_ADD_NONSTANDARD),FALSE);
		}
		break;



	case WM_COMMAND:
		{
			hListBox = GetDlgItem(hDlg,SCC_NONSTANDARD_LIST);

            switch(LOWORD(wParam))
            {
			case SCC_NONSTANDARD_LIST:
				NonStandardSelectHeaderByIndex(hDlg,TRUE);
				break;

			case SCC_ADD_NONSTANDARD:
				{
					int sel,current,pos;
					DWORD subtype = 0;
					HWND hHeaderEdit, hLabelEdit;
					TCHAR HeaderText[260];
					TCHAR LabelText[260];

					sel = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);
					hHeaderEdit = GetDlgItem(hDlg,SCC_NONSTANDARD_HEADER);
					hLabelEdit = GetDlgItem(hDlg,SCC_NONSTANDARD_LABEL);

					Edit_GetText(hHeaderEdit,HeaderText,260);
					Edit_GetText(hLabelEdit,LabelText,260);

#define GET_CHECKBOX(type)															\
	if (IsDlgButtonChecked(hDlg,SCC_NONSTANDARD_##type##_CHECKBOX) == BST_CHECKED)	\
					subtype |= SCCUT_MAILTYPE_##type
			
					GET_CHECKBOX(EMAIL);
					GET_CHECKBOX(JOURNAL);
					GET_CHECKBOX(CONTACT);
					GET_CHECKBOX(TASK);
					GET_CHECKBOX(APPOINTMENT);
					GET_CHECKBOX(DISTROLIST);
					GET_CHECKBOX(POST);
					GET_CHECKBOX(NOTE);
#undef GET_CHECKBOX			

					if (sel != LB_ERR)
					{
						/* Edit existing */			
						current = (int)SendMessage(hListBox, LB_GETITEMDATA, sel,0); 
						SendMessage(hListBox, LB_DELETESTRING, sel,0);					
					}
					else
					{
						/* Add New */
						for (current = 0;current<MAX_NONSTANDARD_HEADERS;current++)
							if (g_NonStandardMailHeaders[current].wsMimeHeaderName[0] == 0)
								break;				
					}

					if (current >= 0 && current < MAX_NONSTANDARD_HEADERS)
					{
						lstrcpy(g_NonStandardMailHeaders[current].wsMimeHeaderName,HeaderText);
						lstrcpy(g_NonStandardMailHeaders[current].wsMimeHeaderLabel,LabelText);
						g_NonStandardMailHeaders[current].dwSubtypeID = subtype;
					
						pos = (int)SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)g_NonStandardMailHeaders[current].wsMimeHeaderName); 
						SendMessage(hListBox, LB_SETITEMDATA, pos, (LPARAM)current); 
						SendMessage(hListBox, LB_SETCURSEL,pos,0);
						NonStandardSelectHeaderByIndex(hDlg,TRUE);
					}
					Edit_SetText(hHeaderEdit,TEXT(""));
					Edit_SetText(hLabelEdit,TEXT(""));
					Button_Enable(GetDlgItem(hDlg,SCC_ADD_NONSTANDARD),FALSE);
				}				
				SendMessage(hListBox, LB_SETCURSEL, (WPARAM)-1,0);
				NonStandardSelectHeaderByIndex(hDlg,TRUE);
				break;


			case SCC_REMOVE_NONSTANDARD:
				{
					int current,sel;

					sel = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);				
					current = (int)SendMessage(hListBox, LB_GETITEMDATA, sel,0); 

					g_NonStandardMailHeaders[current].wsMimeHeaderName[0] = 0;
					g_NonStandardMailHeaders[current].wsMimeHeaderLabel[0] = 0;
					g_NonStandardMailHeaders[current].dwSubtypeID = 0;

					SendMessage(hListBox, LB_DELETESTRING, sel,0);					
				}
				Edit_SetText(GetDlgItem(hDlg,SCC_NONSTANDARD_HEADER),TEXT(""));
				Edit_SetText(GetDlgItem(hDlg,SCC_NONSTANDARD_LABEL),TEXT(""));
                SendMessage(hListBox, LB_SETCURSEL, (WPARAM)-1, 0);
				NonStandardSelectHeaderByIndex(hDlg,TRUE);
				break;
			
			case SCC_REMOVEALL_NONSTANDARD:
				{
					int cur, tot, sel;
					HWND hHeaderEdit, hLabelEdit;

					hHeaderEdit = GetDlgItem(hDlg,SCC_NONSTANDARD_HEADER);
					hLabelEdit = GetDlgItem(hDlg,SCC_NONSTANDARD_LABEL);
				
					tot = (int)SendMessage(hListBox, LB_GETCOUNT, 0,0);
					for(sel = 0; sel < tot; ++sel)
					{
						cur = (int)SendMessage(hListBox, LB_GETITEMDATA, 0,0);
						g_NonStandardMailHeaders[cur].wsMimeHeaderName[0] = 0;
						g_NonStandardMailHeaders[cur].wsMimeHeaderLabel[0] = 0;
						g_NonStandardMailHeaders[cur].dwSubtypeID = 0;
						SendMessage(hListBox, LB_DELETESTRING, 0,0);
					}
					Edit_SetText(hHeaderEdit,TEXT(""));
					Edit_SetText(hLabelEdit,TEXT(""));
                    SendMessage(hListBox, LB_SETCURSEL, (WPARAM)-1, 0);
					NonStandardSelectHeaderByIndex(hDlg,TRUE);
				}
				break;

			case SCC_NONSTANDARD_HEADER:
				{
					int sel;					
					HWND hHeaderEdit;
					TCHAR HeaderText[260];					

					hHeaderEdit = GetDlgItem(hDlg,SCC_NONSTANDARD_HEADER);
					Edit_GetText(hHeaderEdit,HeaderText,260);

					if(HeaderText[0])
						Button_Enable(GetDlgItem(hDlg,SCC_ADD_NONSTANDARD),TRUE);
					else
						Button_Enable(GetDlgItem(hDlg,SCC_ADD_NONSTANDARD),FALSE);
					if ((sel = (int)SendMessage(hListBox,LB_FINDSTRINGEXACT,0,(LPARAM)HeaderText)) != LB_ERR)
						SendMessage(hListBox, LB_SETCURSEL, sel,0);
					else
                        SendMessage(hListBox, LB_SETCURSEL, (WPARAM)-1, 0);
				}
				NonStandardSelectHeaderByIndex(hDlg,FALSE);
				break;

			case SCC_NONSTANDARD_LABEL:
				break;

			case SCC_NONSTANDARD_EMAIL_CHECKBOX:
			case SCC_NONSTANDARD_JOURNAL_CHECKBOX:
			case SCC_NONSTANDARD_CONTACT_CHECKBOX:
			case SCC_NONSTANDARD_TASK_CHECKBOX:
			case SCC_NONSTANDARD_APPOINTMENT_CHECKBOX:
			case SCC_NONSTANDARD_NOTE_CHECKBOX:
			case SCC_NONSTANDARD_DISTROLIST_CHECKBOX:
			case SCC_NONSTANDARD_POST_CHECKBOX:
				{
					if (IsDlgButtonChecked(hDlg,LOWORD(wParam)))
						CheckDlgButton(hDlg,LOWORD(wParam),BST_UNCHECKED);
					else
						CheckDlgButton(hDlg,LOWORD(wParam),BST_CHECKED);
				}
				break;

            case IDCANCEL:
				EndDialog(hDlg, 0);
                return(FALSE);

			case IDOK:
				{
					int i;
					SCCVWOPTIONSPEC40    locOptionSpec;			
					TCHAR HeaderText[260];
					hListBox = GetDlgItem(hDlg,SCC_NONSTANDARD_LIST);		
					
					/* First let's handle any text in the edit boxes... */
					Edit_GetText(GetDlgItem(hDlg,SCC_NONSTANDARD_HEADER),HeaderText,260);
					if(HeaderText[0])
					{
						int place = 0;
						int sel = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);

						if (sel != LB_ERR)
						{
							/* Edit existing */			
							place = (int)SendMessage(hListBox, LB_GETITEMDATA, sel,0); 					
						}
						else
						{
							/* Add New */
							for (place = 0;place<MAX_NONSTANDARD_HEADERS;place++)
								if (g_NonStandardMailHeaders[place].wsMimeHeaderName[0] == 0)
									break;				
						}
						if(place < MAX_NONSTANDARD_HEADERS)
						{
							TCHAR LabelText[260];
							TCHAR *tcTmp;
							int subtype = 0;

#define GET_CHECKBOX(type)															\
	if (IsDlgButtonChecked(hDlg,SCC_NONSTANDARD_##type##_CHECKBOX) == BST_CHECKED)	\
					subtype |= SCCUT_MAILTYPE_##type
			
							GET_CHECKBOX(EMAIL);
							GET_CHECKBOX(JOURNAL);
							GET_CHECKBOX(CONTACT);
							GET_CHECKBOX(TASK);
							GET_CHECKBOX(APPOINTMENT);
							GET_CHECKBOX(DISTROLIST);
							GET_CHECKBOX(POST);
							GET_CHECKBOX(NOTE);
#undef GET_CHECKBOX			
							Edit_GetText(GetDlgItem(hDlg,SCC_NONSTANDARD_LABEL),LabelText,260);
							tcTmp = HeaderText;
							i = 0;
							while(*tcTmp)
							{
								g_NonStandardMailHeaders[place].wsMimeHeaderName[i++] = *tcTmp;
								++tcTmp;
							}
							g_NonStandardMailHeaders[place].wsMimeHeaderName[i] = 0;
							tcTmp = LabelText;
							i = 0;
							while(*tcTmp)
							{
								g_NonStandardMailHeaders[place].wsMimeHeaderLabel[i++] = *tcTmp;
								++tcTmp;
							}
							g_NonStandardMailHeaders[place].wsMimeHeaderLabel[i] = 0;
							g_NonStandardMailHeaders[place].dwHeaderID = place;
							g_NonStandardMailHeaders[place].dwSubtypeID = subtype;
						}
					}
					locOptionSpec.dwSize	= sizeof(SCCVWOPTIONSPEC40);
					locOptionSpec.dwId		= SCCOPT_MAILHEADERVISIBLE;
					locOptionSpec.dwFlags	= SCCVWOPTION_CURRENT;			
			
					for (i=0;i<MAX_NONSTANDARD_HEADERS;i++)
					{
						locOptionSpec.pData		= &(g_NonStandardMailHeaders[i]);

						g_NonStandardMailHeaders[i].dwHeaderID = NONSTANDARD_HEADER_ID_BASE + i;

						/* We might have done a "remove header" and since this list was populated
						   by getting the userdefined headers, go ahead and just set all...
						if (g_NonStandardMailHeaders[i].wsMimeHeaderName[0] != 0) */
							SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOptionSpec);
					}
				}
				EndDialog(hDlg, 1);
                return(TRUE);
			default:
				break;
			}
		}
	default:
		break;
	}
	return FALSE;
}










static HINSTANCE SCCLoadViewerDLL(TCHAR *lpViewerPath)
{
    HINSTANCE   locRet = 0;
    TCHAR      *locDirPtr;
    TCHAR     *locPath = NULL;
    TCHAR     *locDir = NULL;
    VTWORD      wPathLen; 

    wPathLen = (VTWORD)lstrlen(lpViewerPath);

    locDir = (TCHAR *)malloc((wPathLen + 1) * sizeof(TCHAR));
    if(locDir)
    {
        /*
        |   Make copy of load path
        */

        lstrcpy(locDir,lpViewerPath);

        /*
        |   Strip off trailing "\"
        */
        locDirPtr = locDir;
        while (*locDirPtr != 0x00)  locDirPtr++;
        locDirPtr--;
        if (*locDirPtr == '\\') *locDirPtr = 0x00;

        /* 11 comes from adding "\SCCVW.DLL" plus the null terminator
        |  to locDir
        */
        locPath = (TCHAR *)malloc((wPathLen + 11) * sizeof(TCHAR));
        if(locPath)
        {
            /*
            |   Load the library
            */
            lstrcpy(locPath,locDir);
            lstrcat(locPath,TEXT("\\"));
            lstrcat(locPath,TEXT("SCCVW.DLL"));
            locRet = LoadLibraryEx(locPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
            free(locPath);
        }
        free(locDir);
    }
    return(locRet);
}


static VTVOID SCCGetExePath(HINSTANCE hInst,TCHAR *lpPath,VTWORD wPathLen)
{
    GetModuleFileName(hInst, lpPath, wPathLen);

    /*
    |   Strip the file name
    */

    while (*lpPath != 0x00)
        lpPath++;
    while (*lpPath != '\\' && *lpPath != ':')
        lpPath--;
    lpPath++;
    *lpPath = 0x00;
}

VTVOID HandleCondCommentOption(HWND hWnd, VTDWORD dwFlagVal, VTDWORD dwGuiValue)
{

    SCCVWOPTIONSPEC40   locOption;
    VTDWORD             locFlags;

    locOption.dwSize = sizeof(SCCVWOPTIONSPEC40);
    locOption.dwId = SCCOPT_HTML_COND_COMMENT_MODE;
    locOption.dwFlags = SCCVWOPTION_CURRENT;
    locOption.pData = (VTVOID *)&locFlags;


    if(dwHtmlCondCommentFlags == HTML_COND_COMMENT_ALL)
    {
        /*if ALL is set, unset it and proceed as if NONE is set.  
        ALL is treated as a separate
        stand-alone option, not as an aggregate of all other specific options.
        In other words, setting all the IE flags is still a subset of ALL.*/
        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_ALL, 
                MF_UNCHECKED);
        dwHtmlCondCommentFlags = HTML_COND_COMMENT_NONE;
    }

    /*if the flag bit IS set....*/
    if(dwHtmlCondCommentFlags & dwFlagVal)
    {
        /*Turn off the GUI check for this option value and the "all" check.*/
        CheckMenuItem( GetMenu(hWnd), dwGuiValue, MF_UNCHECKED);

        /*Turn off the bit for this option value.*/
        dwHtmlCondCommentFlags &= (~dwFlagVal);

        if(dwHtmlCondCommentFlags == HTML_COND_COMMENT_NONE)
        {
            /*if nothing's checked, turn on the check for "None".*/
            CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_NONE, 
                MF_CHECKED);
        }
    }
    else /*the specific flag is NOT set.*/
    {
        /*turn on the GUI check for this option value, and turn off the "none" and "all" check.*/
        CheckMenuItem( GetMenu(hWnd), dwGuiValue, MF_CHECKED);
        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_NONE, MF_UNCHECKED);
        CheckMenuItem( GetMenu(hWnd), MENU_SYSTEM_OPTIONS_COND_COMMENT_ALL, MF_UNCHECKED);

        /*Turn on the bit corresponding to the option value.*/
        dwHtmlCondCommentFlags |= dwFlagVal;
    }

    /*dwHtmlCondCommentFlags should be the correct value of the option.
      Let's use it directly.*/
    locFlags = dwHtmlCondCommentFlags;

    SendMessage(hViewWnd,SCCVW_SETOPTION,0,(LPARAM)(PSCCVWOPTIONSPEC40)&locOption);
}




TCHAR*	pInfoMsgTextBuffer = NULL;
DWORD   dwInfoMsgTextBufferSize = 0;

WIN_ENTRYSC BOOL WIN_ENTRYMOD InfoMsgDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
	

    switch (message)
    {
    case WM_INITDIALOG:
        {
            RECT	locRect;
            int		locX;
            int		locY;
            HWND	hTextWnd;			
			RECT	locParentRect;

            /*
            |   Center dialog
            */
            GetWindowRect(hDlg,&locRect);
			GetWindowRect(GetParent(hDlg),&locParentRect);
            locX = locParentRect.left + ((locParentRect.right  - locParentRect.left) - (locRect.right - locRect.left)) / 2;
            locY = locParentRect.top  + ((locParentRect.bottom - locParentRect.top)  - (locRect.bottom - locRect.top)) / 2;
            SetWindowPos(hDlg,NULL,locX,locY,0,0,SWP_NOSIZE | SWP_NOZORDER);

			hTextWnd = GetDlgItem(hDlg, EDIT_INFOMSG);
			if (!pInfoMsgTextBuffer)
				SetWindowText(hTextWnd, TEXT("No Info Messages for this file."));
            else
                SetWindowText(hTextWnd, pInfoMsgTextBuffer);

			break;
		}
    case WM_COMMAND:
        if (IDOK == wParam)
        {
            EndDialog(hDlg, 1);
        }
        return (TRUE);
        break;
    }
    return FALSE;
}


int MessageCounts[SCCVW_INFOMESSAGE_TOP+1];

VTVOID ClearInfoMessages()
{
	int i;

	if (pInfoMsgTextBuffer)
		HeapFree(GetProcessHeap(), 0, pInfoMsgTextBuffer);
	pInfoMsgTextBuffer = NULL;
	dwInfoMsgTextBufferSize = 0;

	for (i=0;i <= SCCVW_INFOMESSAGE_TOP;i++)
		MessageCounts[i] = 0;
}


VTVOID HandleInfoMessage(HWND hWnd, VTDWORD infomessage)
{
	TCHAR*	pNewBuffer;

    UNUSED(hWnd);

	/* 
	   We receive a message each time the problem is encountered during viewing.  Scrolling or even screen refreshes may trigger a repeated message,
	   so rather than fill the info box with spam, we'll only add the message to the box the first time we encounter a given message in each file.
	*/
	if (MessageCounts[infomessage]++)
		return;

	if (!pInfoMsgTextBuffer)
		pInfoMsgTextBuffer = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,1);


	if ((pNewBuffer = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,pInfoMsgTextBuffer,  dwInfoMsgTextBufferSize+100)) != NULL)
	{		
		pInfoMsgTextBuffer = pNewBuffer;
		dwInfoMsgTextBufferSize += 100;

		switch (infomessage)
		{
		
		case SCCVW_INFO_BADCOMPRESSION:	lstrcat(pInfoMsgTextBuffer, TEXT("File: Graphic uses an unsupported compression method\r\n"));	break;
		case SCCVW_INFO_BADCOLORSPACE:	lstrcat(pInfoMsgTextBuffer, TEXT("File: Graphic uses an unsupported colorspace\r\n"));			break;		
		case SCCVW_INFO_MISSINGMAP:		lstrcat(pInfoMsgTextBuffer, TEXT("File: PDF file missing toUnicode table\r\n"));				break;
		case SCCVW_INFO_EQUATION:		lstrcat(pInfoMsgTextBuffer, TEXT("Unsupported Feature: Equations\r\n"));						break;
		case SCCVW_INFO_FORMS:			lstrcat(pInfoMsgTextBuffer, TEXT("Unsupported Feature: Forms\r\n"));							break;
		case SCCVW_INFO_VERTICALTEXT:	lstrcat(pInfoMsgTextBuffer, TEXT("Unsupported Feature: Vertical Text\r\n"));					break;
		case SCCVW_INFO_TEXTEFFECTS:	lstrcat(pInfoMsgTextBuffer, TEXT("Unsupported Feature: Text Effects\r\n"));						break;
		case SCCVW_INFO_RTTOLFTTABLES:	lstrcat(pInfoMsgTextBuffer, TEXT("Unsupported Feature: Right to Left Tables\r\n"));				break;
		case SCCVW_INFO_ALIASEDFONT:	lstrcat(pInfoMsgTextBuffer, TEXT("Info: Font Alias used\r\n"));									break;
		case SCCVW_INFO_MISSINGFONT:	lstrcat(pInfoMsgTextBuffer, TEXT("Info: Requested Font unavailable\r\n"));						break;
		case SCCVW_INFO_SUBDOCFAILED:	lstrcat(pInfoMsgTextBuffer, TEXT("Info: A subdocument was not processesed\r\n"));				break;
		case SCCVW_INFO_TYPETHREEFONT:	lstrcat(pInfoMsgTextBuffer, TEXT("File: PDF file uses embedded Type 3 fonts\r\n"));		        break;
		case SCCVW_INFO_BADSHADING:	    lstrcat(pInfoMsgTextBuffer, TEXT("File: PDF file uses an unsupported shading type\r\n"));		break;
		case SCCVW_INFO_BADHTML:	    lstrcat(pInfoMsgTextBuffer, TEXT("File: File uses invalid HTML\r\n"));		                    break;
		default:						lstrcat(pInfoMsgTextBuffer, TEXT("Info: Unknown information\r\n"));								break;							
		}		
	}
	else
	{
		/* No memory to track this info message. */
		if (dwInfoMsgTextBufferSize)
			lstrcpy(pInfoMsgTextBuffer,TEXT("Info Message memory failure. Sorry.\n"));
	}
}



/* 
	The following code tracks any child processes spawned as a result of SCCVW_VIEWTHISFILE messages.

	We need to keep track of the process information for the process we created, so we can monitor it, and clean
	up any delete-on-close files that need to be cleaned up when it exits.  We also need to be able to post messages
	to that process if we close the file being viewed, so that the children windows will slso shut down.

	In order to do this, we'll create a timer, and run the list of open children each time it triggers, performing
	cleanup on each as it terminates.  We'll also run this list when we close the currently viewed file or the window
	and notify the children processes that it's time to go.

	And, this works in a nicely recursive fashion, so if you open an attachment from an email that was stored in a zip
	file, closing the zip file will ensure that all children and grandchildren (and so on...) will be cleaned up correctly.
*/


PCHILD_PROCESS_DATA child_process_data_list = NULL;

VTVOID add_process_to_active_list(PPROCESS_INFORMATION pi, PSCCVWVIEWFILE80 vf, HWND hWnd)
{
	PCHILD_PROCESS_DATA cd;
	
	if ((cd = (PCHILD_PROCESS_DATA)malloc(sizeof(CHILD_PROCESS_DATA)))!=NULL)
	{
		cd->pi = *pi;

		if (vf->bDeleteOnClose)
		{
			if (vf->dwSpecType == IOTYPE_UNICODEPATH)
				wsprintf(cd->szDeleteFileName,TEXT("%s"),(LPWSTR)vf->pSpec);						
			else
				wsprintf(cd->szDeleteFileName,TEXT("%S"),(LPSTR)vf->pSpec);
		}
		else
			cd->szDeleteFileName[0] = 0;
		
		cd->next = child_process_data_list;
		child_process_data_list= cd;

		SetTimer(hWnd,CHILD_PROCESS_TIMER_EVENT_ID,500,NULL);
	}                      
}

VTVOID remove_active_process_from_list(PCHILD_PROCESS_DATA cd)
{
	PCHILD_PROCESS_DATA prev;

	if (cd == child_process_data_list)
	{
		child_process_data_list = cd->next;		
	}
	else
	{
		for (prev = child_process_data_list;prev;prev = prev->next)
		{
			if (prev->next == cd)
			{
				prev->next = cd->next;
				break;
			}
		}
	}	
	free(cd);	
}



VTVOID check_active_child_processes(HWND hWnd)
{
	PCHILD_PROCESS_DATA cd, cd_next;
	DWORD dw;	

	for (cd = child_process_data_list;cd;cd=cd_next)
	{
		cd_next = cd->next;

		GetExitCodeProcess(cd->pi.hProcess,&dw);
		if (dw != STILL_ACTIVE)
		{
			 CloseHandle(cd->pi.hProcess);
             CloseHandle(cd->pi.hThread);

			if (cd->szDeleteFileName[0])
				DeleteFile(cd->szDeleteFileName);			

			remove_active_process_from_list(cd);
		}
	}
	if (!child_process_data_list)
		KillTimer(hWnd,CHILD_PROCESS_TIMER_EVENT_ID);
}

VTVOID close_child_processes(BOOL bForce)
{
	PCHILD_PROCESS_DATA cd, cd_next;		

	for (cd = child_process_data_list;cd;cd=cd_next)
	{
		cd_next = cd->next;

		if (PostThreadMessage(cd->pi.dwThreadId,WM_QUIT,0,0) ||
			(bForce && TerminateProcess(cd->pi.hProcess,1)))
		{
			WaitForSingleObject(cd->pi.hProcess,1000);

			CloseHandle(cd->pi.hProcess);
			CloseHandle(cd->pi.hThread);			
		}

		/*	
			This goes outside the prior check, because it's possible that the user closed the child window and then 
			closed this process before the timer went off and cleaned this file up.  In this case, the above calls
			would fail, as the target thread no longer exists, but the temp file would still be there, which is why
			this child's data is still in our list.
		*/
		if (cd->szDeleteFileName[0])
			DeleteFile(cd->szDeleteFileName);			

		remove_active_process_from_list(cd);
	}
}

