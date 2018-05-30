// mfcvidoc.h : interface of the CMfcviewDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CMfcviewDoc : public CDocument
{
protected: // create from serialization only
	CMfcviewDoc();
	DECLARE_DYNCREATE(CMfcviewDoc)

// Attributes
public:
// Operations
public:

// Implementation
public:
	virtual ~CMfcviewDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument( const TCHAR* pszPathName );

// Generated message map functions
protected:
	//{{AFX_MSG(CMfcviewDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
