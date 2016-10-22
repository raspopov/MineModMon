// MineModMon.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MineModMon.h"
#include "MineModMonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMineModMonApp

BEGIN_MESSAGE_MAP(CMineModMonApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CMineModMonApp construction

CMineModMonApp::CMineModMonApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

// The one and only CMineModMonApp object

CMineModMonApp theApp;

// CMineModMonApp initialization

BOOL CMineModMonApp::InitInstance()
{
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey( AFX_IDS_COMPANY_NAME );

	// Log version information and warnings
	DWORD handle = NULL;
	if ( DWORD size = GetFileVersionInfoSize( sModuleFileName, &handle ) )
	{
		if ( char* ver = (char*)GlobalAlloc( GPTR, size ) )
		{
			if ( GetFileVersionInfo( sModuleFileName, handle, size, ver ) )
			{
				UINT len = 0;
				VS_FIXEDFILEINFO* fix = NULL;
				if ( VerQueryValue( ver, _T( "\\" ), (void**)&fix, &len ) && len )
				{
					sVersion.Format( _T( "%u.%u.%u.%u" ),
						HIWORD( fix->dwFileVersionMS ), LOWORD( fix->dwFileVersionMS ),
						HIWORD( fix->dwFileVersionLS ), LOWORD( fix->dwFileVersionLS ) );

					LPCTSTR szLegalCopyright;
					if ( VerQueryValue( ver, _T( "\\StringFileInfo\\000004b0\\LegalCopyright" ), (void**)&szLegalCopyright, &len ) && len )
					{
						sCopyright = szLegalCopyright;
					}
				}
			}
			GlobalFree( ver );
		}
	}

	CMineModMonDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
