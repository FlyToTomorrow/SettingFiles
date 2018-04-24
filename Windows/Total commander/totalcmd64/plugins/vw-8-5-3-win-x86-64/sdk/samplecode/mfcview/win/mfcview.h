// mfcview.h : main header file for the MFCVIEW application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfcviewApp:
// See mfcview.cpp for the implementation of this class
//

class CMfcviewApp : public CWinApp
{
public:
	CMfcviewApp();

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Data

	HINSTANCE	m_hViewerLibrary;

// Implementation

	//{{AFX_MSG(CMfcviewApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////