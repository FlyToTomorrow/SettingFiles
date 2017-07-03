#if !defined(AFX_MFCVIEWMDI_H__C8BC59FB_9502_408B_B0A4_B53199B48079__INCLUDED_)
#define AFX_MFCVIEWMDI_H__C8BC59FB_9502_408B_B0A4_B53199B48079__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MfcviewMDI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMfcviewMDI frame

class CMfcviewMDI : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMfcviewMDI)
protected:
	CMfcviewMDI();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcviewMDI)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMfcviewMDI();

	// Generated message map functions
	//{{AFX_MSG(CMfcviewMDI)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
// SCC
// SCC
	LRESULT OnViewThisFile(WPARAM wParam,LPARAM lParam);
// SCC
// SCC
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCVIEWMDI_H__C8BC59FB_9502_408B_B0A4_B53199B48079__INCLUDED_)
