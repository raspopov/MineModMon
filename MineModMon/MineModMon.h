// MineModMon.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMineModMonApp:
// See MineModMon.cpp for the implementation of this class
//

class CMineModMonApp : public CWinApp
{
public:
	CMineModMonApp();

	mutable CCriticalSection m_pSection;

	CString	sVersion;				// Application version ("X.X.X.X")
	CString sCopyright;				// Copyright information
	CString sModuleFileName;		// Full path of application exe-file

protected:

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CMineModMonApp theApp;
