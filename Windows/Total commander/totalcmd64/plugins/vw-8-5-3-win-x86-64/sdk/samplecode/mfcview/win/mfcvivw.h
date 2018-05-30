// mfcvivw.h : interface of the CMfcviewView class
//
/////////////////////////////////////////////////////////////////////////////

class CMfcviewView : public CView
{
protected: // create from serialization only
	CMfcviewView();
	DECLARE_DYNCREATE(CMfcviewView)

// Attributes
public:
	CMfcviewDoc* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CMfcviewView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual WNDPROC * GetSuperWndProcAddr();
	virtual void OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView );
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMfcviewView)
	afx_msg void OnPaint();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in mfcvivw.cpp
inline CMfcviewDoc* CMfcviewView::GetDocument()
   { return (CMfcviewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
