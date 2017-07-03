// mfcview.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "mfcview.h"

#include "mainfrm.h"
#include "mfcvidoc.h"
#include "mfcvivw.h"
#include "verbuild.h"
#include "MfcviewMDI.h"

// SCC  Include the viewer technoloies .H file
// SCC
#include "sccvw.h"
// SCC
// SCC

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcviewApp

BEGIN_MESSAGE_MAP(CMfcviewApp, CWinApp)
	//{{AFX_MSG_MAP(CMfcviewApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcviewApp construction

CMfcviewApp::CMfcviewApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMfcviewApp object

CMfcviewApp theApp;

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Include definition of SCC C function to load the Viewer DLL
// SCC
// SCC
#include "scclink.c"
// SCC
// SCC
/////////////////////////////////////////////////////////////////////////////
// CMfcviewApp initialization

BOOL CMfcviewApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	SetDialogBkColor();        // Set dialog background color to gray
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_MFCVIETYPE,
		RUNTIME_CLASS(CMfcviewDoc),
		RUNTIME_CLASS(CMfcviewMDI),
		RUNTIME_CLASS(CMfcviewView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// create a new (empty) document

// SCC  Don't start with an empty document
// SCC
//	OnFileNew();
// SCC
// SCC

// SCC  Load the viewer DLL from the same directory the application is in,
// SCC  parsing the command line for the OEM ID.
	{
	TCHAR		szExePath[256];
	
	SCCGetExePath(m_hInstance,szExePath,256);
	m_hViewerLibrary = SCCLoadViewerDLL(szExePath);
	}
// SCC
// SCC

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

// SCC
// SCC
int CMfcviewApp::ExitInstance()
{
int	locRet;

		/*
		|	Call default
		*/
		
	locRet = CWinApp::ExitInstance();
	
		/*
		|	Unload viewer DLL
		*/
		
	FreeLibrary(m_hViewerLibrary);
	
	
	return(locRet);
}
// SCC
// SCC

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMfcviewApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog()
{

	TCHAR	buffer[200];
			
	// Display sample app. version
	StringCbCopy(buffer, 200, _T("MFCView Sample Application\n"));
	StringCbCat(buffer, 200, _T("S_PRODUCT"));
	StringCbCat(buffer, 200, _T("\n"));	
	StringCbCat(buffer, 200, _T("Build:\t"));
	StringCbCat(buffer, 200, _T("S_FILEVERSION"));
	SetDlgItemText(HELP_ABOUTBOXVERINFO, (LPCTSTR)buffer);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMfcviewApp commands
