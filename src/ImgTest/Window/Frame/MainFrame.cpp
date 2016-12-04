#include "stdafx.h"
#include "Main.h"

#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE( CMainFrame, CFrameWndEx )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWndEx )
	ON_WM_CREATE( )
	ON_COMMAND( ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize )
	ON_REGISTERED_MESSAGE( AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew )
	ON_WM_SETTINGCHANGE( )
END_MESSAGE_MAP( )


CMainFrame::CMainFrame( )
{
}

CMainFrame::~CMainFrame( )
{ }

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CFrameWndEx::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	BOOL bNameValid;

	if( !m_wndMenuBar.Create( this ) )
	{
		TRACE0( "Failed to create menubar\n" );
		return -1;    
	}

	m_wndMenuBar.SetPaneStyle( m_wndMenuBar.GetPaneStyle( ) | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY );


	CMFCPopupMenu::SetForceMenuFocus( FALSE );

	if( !m_wndToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_wndToolBar.LoadToolBar( IDR_MAINFRAME_256 ) )
	{
		TRACE0( "Failed to create toolbar\n" );
		return -1;      
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString( IDS_TOOLBAR_STANDARD );
	ASSERT( bNameValid );
	m_wndToolBar.SetWindowText( strToolBarName );

	CString strCustomize;
	bNameValid = strCustomize.LoadString( IDS_TOOLBAR_CUSTOMIZE );
	ASSERT( bNameValid );
	m_wndToolBar.EnableCustomizeButton( TRUE, ID_VIEW_CUSTOMIZE, strCustomize );

	m_wndMenuBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndMenuBar );
	DockPane( &m_wndToolBar );

	CDockingManager::SetDockingMode( DT_SMART );
	EnableAutoHidePanes( CBRS_ALIGN_ANY );

	if( !CreateDockingWindows( ) )
	{
		TRACE0( "Failed to create docking windows\n" );
		return -1;
	}

	m_wndOutput.EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndOutput );


	CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerOffice2007 ) );

	CMFCVisualManagerOffice2007::SetStyle( CMFCVisualManagerOffice2007::Office2007_ObsidianBlack );

	EnablePaneMenu( TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR );

	CMFCToolBar::EnableQuickCustomization( );

	return 0;
}

BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext )
{
	return m_wndSplitter.Create( this,
		2, 2,              
		CSize( 10, 10 ),     
		pContext 
	);
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CFrameWndEx::PreCreateWindow( cs ) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME | 
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows( )
{
	BOOL bNameValid;

	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString( IDS_OUTPUT_WND );
	ASSERT( bNameValid );
	if( !m_wndOutput.Create( strOutputWnd, this, CRect( 0, 0, 100, 100 ), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "Failed to create Output window\n" );
		return FALSE; 
	}

	SetDockingWindowIcons( );
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons( )
{
	HICON hOutputBarIcon = ( HICON ) ::LoadImage( ::AfxGetResourceHandle( ), MAKEINTRESOURCE( IDI_OUTPUT_WND_HC ), IMAGE_ICON, ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ), 0 );
	m_wndOutput.SetIcon( hOutputBarIcon, FALSE );

}



#ifdef _DEBUG
void CMainFrame::AssertValid( ) const
{
	CFrameWndEx::AssertValid( );
}

void CMainFrame::Dump( CDumpContext& dc ) const
{
	CFrameWndEx::Dump( dc );
}
#endif //_DEBUG


void CMainFrame::OnViewCustomize( )
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog( this, TRUE );
	pDlgCust->Create( );
}

LRESULT CMainFrame::OnToolbarCreateNew( WPARAM wp, LPARAM lp )
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew( wp, lp );
	if( lres == 0 )
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = ( CMFCToolBar* )lres;
	ASSERT_VALID( pUserToolbar );

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString( IDS_TOOLBAR_CUSTOMIZE );
	ASSERT( bNameValid );

	pUserToolbar->EnableCustomizeButton( TRUE, ID_VIEW_CUSTOMIZE, strCustomize );
	return lres;
}


void CMainFrame::OnSettingChange( UINT uFlags, LPCTSTR lpszSection )
{
	CFrameWndEx::OnSettingChange( uFlags, lpszSection );
	m_wndOutput.UpdateFonts( );
}
