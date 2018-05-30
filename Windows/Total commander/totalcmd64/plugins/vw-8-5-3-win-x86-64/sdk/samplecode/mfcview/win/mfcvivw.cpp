// mfcvivw.cpp : implementation of the CMfcviewView class
//

#include "stdafx.h"
#include "mfcview.h"

#include "mfcvidoc.h"
#include "mfcvivw.h"
#include <strsafe.h>
// SCC  Include the viewer technology .H file
// SCC
#include "sccvw.h" 
// SCC
// SCC

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcviewView

IMPLEMENT_DYNCREATE(CMfcviewView, CView)

BEGIN_MESSAGE_MAP(CMfcviewView, CView)
	//{{AFX_MSG_MAP(CMfcviewView)
	ON_WM_PAINT()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcviewView construction/destruction

CMfcviewView::CMfcviewView()
{
	// TODO: add construction code here
}

CMfcviewView::~CMfcviewView()
{
}

// SCC
// SCC

BOOL CMfcviewView::PreCreateWindow(CREATESTRUCT& cs)
{
static TCHAR	locClass[20];
CMfcviewApp * locAppPtr;
	
	CView::PreCreateWindow(cs);
	  
	locAppPtr = (CMfcviewApp *)AfxGetApp();
	StringCbCopy(locClass, 20, _T("SCCVIEWER"));	
	cs.lpszClass = locClass;
		
	return(TRUE);
}

WNDPROC * CMfcviewView::GetSuperWndProcAddr()
{
static WNDPROC staticWndProc;

	return(&staticWndProc);
}

void  CMfcviewView::OnActivateView( BOOL bActivate, CView* /* pActivateView */, CView* /* pDeactiveView */ )
{
	if (bActivate)
		CMfcviewView::DefWindowProc(WM_QUERYNEWPALETTE,0,0);
}

// SCC
// SCC

/////////////////////////////////////////////////////////////////////////////
// CMfcviewView drawing

void CMfcviewView::OnDraw(CDC* /* pDC */ )
{
	CMfcviewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMfcviewView diagnostics

#ifdef _DEBUG
void CMfcviewView::AssertValid() const
{
	CView::AssertValid();
}

void CMfcviewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMfcviewDoc* CMfcviewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcviewDoc)));
	return (CMfcviewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcviewView message handlers

void CMfcviewView::OnPaint()
{
	CMfcviewView::DefWindowProc(WM_PAINT,0,0);
}


void CMfcviewView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	LRESULT locRet;

	locRet = CMfcviewView::DefWindowProc(SCCVW_GETCLIPINFO,0,0);	
	pCmdUI->Enable((BOOL)(locRet && SCCVW_CANCOPYTOCLIP));
}

void CMfcviewView::OnEditCopy()
{
	CMfcviewView::DefWindowProc(SCCVW_COPYTOCLIP,0,0);
}



void CMfcviewView::OnUpdateFilePrint(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
	
}

void CMfcviewView::OnFilePrint()
{
	// TODO: Add your command handler code here
	CMfcviewView::DefWindowProc(SCCVW_PRINT,0,0);
	
}
