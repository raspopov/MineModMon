/*

Minecraft Mods Monitor (MineModMon)
Copyright (C) 2016 Nikolay Raspopov <raspopov@cherubicsoft.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

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
