#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"      


class CApp : public CWinAppEx
{
public:
	CApp();

public:
	virtual BOOL InitInstance();


	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CApp theApp;
