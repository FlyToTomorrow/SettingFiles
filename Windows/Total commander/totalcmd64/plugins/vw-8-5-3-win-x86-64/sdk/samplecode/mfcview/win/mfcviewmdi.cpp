// MfcviewMDI.cpp : implementation file
//

#include "stdafx.h"
#include "mfcview.h"
#include "MfcviewMDI.h"
#include <strsafe.h>


// SCC  Include the viewer technology .H file
// SCC
#include "sccvw.h" 
// SCC
// SCC


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcviewMDI

IMPLEMENT_DYNCREATE(CMfcviewMDI, CMDIChildWnd)

CMfcviewMDI::CMfcviewMDI()
{
}

CMfcviewMDI::~CMfcviewMDI()
{
}


BEGIN_MESSAGE_MAP(CMfcviewMDI, CMDIChildWnd)
	//{{AFX_MSG_MAP(CMfcviewMDI)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(SCCVW_VIEWTHISFILE,OnViewThisFile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcviewMDI message handlers

// SCC
// SCC
LRESULT CMfcviewMDI::OnViewThisFile(WPARAM /* wParam */,LPARAM lParam)
{
	SCCVWVIEWTHISFILE80 *f80 = (SCCVWVIEWTHISFILE80 *)lParam;
	TCHAR szCmd[1024];

    CMfcviewApp * locAppPtr = (CMfcviewApp *)AfxGetApp();


	if (f80->sViewFile.dwSpecType == IOTYPE_UNICODEPATH)
		StringCbPrintf(szCmd, 1024, TEXT("%s"), (LPSTR)f80->sViewFile.pSpec);
	else
		StringCbPrintf(szCmd, 1024, TEXT("%S"), (LPSTR)f80->sViewFile.pSpec);

	locAppPtr->OpenDocumentFile(szCmd);

    return(SCCVWERR_MESSAGEHANDLED);
}
// SCC
// SCC
