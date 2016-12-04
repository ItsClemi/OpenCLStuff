#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Main.h"

#include "Window/Frame/MainFrame.h"
#include "Window/Frame/AppDoc.h"
#include "Window/Frame/AppView.h"

#include "Window/Dialog/AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP( CApp, CWinAppEx )
	ON_COMMAND( ID_APP_ABOUT, &CApp::OnAppAbout )

	ON_COMMAND( ID_FILE_NEW, &CWinAppEx::OnFileNew )
	ON_COMMAND( ID_FILE_OPEN, &CWinAppEx::OnFileOpen )
END_MESSAGE_MAP( )

CApp theApp;




CApp::CApp( )
{
	SetAppID( _T( "ImgTest.AppID.NoVersion" ) );
}

BOOL CApp::InitInstance( )
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &InitCtrls );

	CWinAppEx::InitInstance( );


	EnableTaskbarInteraction( FALSE );

	AfxInitRichEdit2( );

	SetRegistryKey( _T( "Local AppWizard-Generated Applications" ) );
	LoadStdProfileSettings( 0 );  
	
	InitContextMenuManager( );
	InitKeyboardManager( );
	InitTooltipManager( );

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager( )->SetTooltipParams( AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS( CMFCToolTipCtrl ), &ttParams );

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate( IDR_MAINFRAME, RUNTIME_CLASS( CAppDoc ), RUNTIME_CLASS( CMainFrame ), RUNTIME_CLASS( CAppView ) );
	if( !pDocTemplate )
	{
		return FALSE;
	}

	AddDocTemplate( pDocTemplate );


	CCommandLineInfo cmdInfo;
	ParseCommandLine( cmdInfo );


	if( !ProcessShellCommand( cmdInfo ) )
	{
		return FALSE;
	}

	m_pMainWnd->ShowWindow( SW_SHOWMAXIMIZED );
	m_pMainWnd->UpdateWindow( );
	return TRUE;
}

void CApp::OnAppAbout( )
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal( );
}

