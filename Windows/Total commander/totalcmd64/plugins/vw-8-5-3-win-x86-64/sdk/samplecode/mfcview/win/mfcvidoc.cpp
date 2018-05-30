// mfcvidoc.cpp : implementation of the CMfcviewDoc class
//

#include "stdafx.h"
#include "mfcview.h"

#include "mfcvidoc.h"

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
// CMfcviewDoc

IMPLEMENT_DYNCREATE(CMfcviewDoc, CDocument)

BEGIN_MESSAGE_MAP(CMfcviewDoc, CDocument)
	//{{AFX_MSG_MAP(CMfcviewDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcviewDoc construction/destruction

CMfcviewDoc::CMfcviewDoc()
{
	// TODO: add one-time construction code here
}

CMfcviewDoc::~CMfcviewDoc()
{
}

BOOL CMfcviewDoc::OnNewDocument()
{
// SCC Can't create new documents
// SCC
//	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

//	return TRUE;
// SCC
// SCC
}

// SCC  Send SCCVW_VIEWFILE message to open the file
// SCC
BOOL CMfcviewDoc::OnOpenDocument( const TCHAR* pszPathName )
{
POSITION pos;
CView* pFirstView;
SCCVWVIEWFILE40	locViewFile;
	
	pos = CMfcviewDoc::GetFirstViewPosition();
 	pFirstView = CMfcviewDoc::GetNextView( pos );

	locViewFile.dwSize = sizeof(SCCVWVIEWFILE40);
#ifdef UNICODE
	locViewFile.dwSpecType = IOTYPE_UNICODEPATH;
#else
	locViewFile.dwSpecType = IOTYPE_ANSIPATH;
#endif
	locViewFile.pSpec = (VTVOID *)pszPathName;
	locViewFile.dwViewAs = 0;
	locViewFile.bUseDisplayName = FALSE;
	locViewFile.bDeleteOnClose = FALSE;
	locViewFile.dwFlags = 0;
	locViewFile.dwReserved1 = 0;
	locViewFile.dwReserved2 = 0;

	pFirstView->SendMessage(SCCVW_VIEWFILE,0,(LPARAM)(PSCCVWVIEWFILE40)&locViewFile);
	
	return(TRUE);
}
// SCC
// SCC

/////////////////////////////////////////////////////////////////////////////
// CMfcviewDoc serialization

void CMfcviewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMfcviewDoc diagnostics

#ifdef _DEBUG
void CMfcviewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMfcviewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

