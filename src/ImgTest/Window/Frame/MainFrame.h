#pragma once
#include "Window/Pane/OutputWnd.h"

class CMainFrame : public CFrameWndEx
{
protected:
	CMainFrame( );
	DECLARE_DYNCREATE( CMainFrame )

public:
	virtual ~CMainFrame( );

public:
	virtual BOOL OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext );
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

#ifdef _DEBUG
	virtual void AssertValid( ) const;
	virtual void Dump( CDumpContext& dc ) const;
#endif


protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnViewCustomize( );
	afx_msg LRESULT OnToolbarCreateNew( WPARAM wp, LPARAM lp );
	afx_msg void OnSettingChange( UINT uFlags, LPCTSTR lpszSection );
	DECLARE_MESSAGE_MAP( )

	BOOL CreateDockingWindows( );
	void SetDockingWindowIcons( );



protected:
	CMFCMenuBar			m_wndMenuBar;
	CMFCToolBar			m_wndToolBar;
	COutputWnd			m_wndOutput;
	CSplitterWnd		m_wndSplitter;

};


