#pragma once


class CAppView : public CView
{
protected:
	CAppView( );
	DECLARE_DYNCREATE( CAppView )
public:
	virtual ~CAppView( );

public:
	CAppDoc* GetDocument( ) const;

public:

public:
	virtual void OnDraw( CDC* pDC );
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
protected:


#ifdef _DEBUG
	virtual void AssertValid( ) const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

protected:

protected:
	afx_msg void OnFilePrintPreview( );
	afx_msg void OnRButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint point );
	DECLARE_MESSAGE_MAP( )
};

#ifndef _DEBUG
inline CAppDoc* CAppView::GetDocument( ) const
{
	return reinterpret_cast< CAppDoc* >( m_pDocument );
}
#endif

