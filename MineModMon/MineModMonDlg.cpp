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


// CMineModMonDlg dialog

CMineModMonDlg::CMineModMonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx	( CMineModMonDlg::IDD, pParent )
	, m_hIcon	( AfxGetApp()->LoadIcon( IDR_MAINFRAME ) )
{
}

void CMineModMonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_PROJECT_LIST, m_wndProjectList );
}

BEGIN_MESSAGE_MAP(CMineModMonDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMineModMonDlg message handlers

BOOL CMineModMonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText( AfxGetAppName() );

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_wndProjectList.SetView( LV_VIEW_TILE );
	LVTILEVIEWINFO lvtvwi = { sizeof( LVTILEVIEWINFO ), LVTVIM_COLUMNS, 0, { }, 4 };
	m_wndProjectList.SetTileViewInfo( &lvtvwi );
	m_wndProjectList.InsertColumn( COL_ID, _T("id") );
	m_wndProjectList.InsertColumn( COL_NAME, _T("name") );

	UpdateSizes();

	Start();

	SetTimer( TIMER, 250, NULL );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMineModMonDlg::OnTimer( UINT_PTR nIDEvent )
{
	CDialogEx::OnTimer( nIDEvent );

	if ( nIDEvent == TIMER )
	{
		Update();
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMineModMonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMineModMonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMineModMonDlg::OnDestroy()
{
	KillTimer( TIMER );

	Stop();

	CDialogEx::OnDestroy();
}

void CMineModMonDlg::Update()
{
	CSingleLock pLock( &theApp.m_pSection, TRUE );

	for ( auto it = m_oProjects.begin(); it != m_oProjects.end(); ++it )
	{
		CProject& prj = (*it).second;

		if ( prj.dirty )
		{
			prj.dirty = false;

			const auto index = m_wndProjectList.InsertItem( m_wndProjectList.GetCountPerPage(), CA2T( prj.id.c_str() ) );
			m_wndProjectList.SetItemText( index, COL_NAME, CA2T( prj.image.c_str() ) );
			
			UINT cols[ 1 ] = { COL_NAME };
			int fmts[ 1 ] = { LVCFMT_LEFT };
			LVTILEINFO lvti = { sizeof( LVTILEINFO ), index, 1, cols, fmts };
			m_wndProjectList.SetTileInfo( &lvti );
		}
	}
}

void CMineModMonDlg::UpdateSizes()
{
	if ( IsWindow( m_wndProjectList.m_hWnd ) )
	{
		CRect rc;
		m_wndProjectList.GetClientRect( &rc );
		LVTILEVIEWINFO lvtvwi = { sizeof( LVTILEVIEWINFO ), LVTVIM_TILESIZE, LVTVIF_FIXEDWIDTH, { rc.Width() - GetSystemMetrics( SM_CXVSCROLL ) - 4, 0 } };
		m_wndProjectList.SetTileViewInfo( &lvtvwi );
		m_wndProjectList.RedrawWindow();
	}
}

/*
Просмотр новых модов ->
	https://minecraft.curseforge.com/mc-mods?filter-sort=updated&page=N
	Выбор: /projects/*
Обзор ->
	/projects/wynncraftenhanced
Список файлов ->
	/projects/wynncraftenhanced/files
Выбор конкретного файла ->
	/projects/wynncraftenhanced/files/2336593
Загрузка ->
	/projects/wynncraftenhanced/files/2336593/download
*/

void CMineModMonDlg::DownloadIndex(CInternetSession* pSession)
{
	for ( int page = 1; page < 5; ++page )
	{
		CString sURL;
		sURL.Format( _T("https://minecraft.curseforge.com/mc-mods?filter-sort=updated&page=%d"), page );
		try
		{
			TRACE( "URL -> %s\n", CT2A( sURL ) );

			CAutoPtr< CStdioFile > stream( pSession->OpenURL( sURL ) );
			if ( stream )
			{
				// Read HTML content
				string html;
				char buf[ 1024 ];
				for (;;)
				{
					const auto read = stream->Read( buf, sizeof( buf ) );
					if ( ! read )
						break;
					html.append( buf, read );
				}
				stream->Close();

				TRACE( "Downloaded -> %u bytes\n", html.size() );

				HTML::ParserDom parser;
				const auto tr = parser.parseTree( html );
				for ( auto it = tr.begin(); it != tr.end(); ++it )
				{
					// Looking for project URL
					if ( it->isTag() && it->tagName() == "a" )
					{
						it->parseAttributes();
						const auto href = it->attribute( "href" );
						if ( href.first && href.second.substr( 0, 10 ) == "/projects/" )
						{
							// Project found
							const string project = href.second.substr( 10 );

							CSingleLock pLock( &theApp.m_pSection, TRUE );

							if ( m_oProjects.find( project ) == m_oProjects.end() )
							{
								TRACE( "Project -> %s\n", project.c_str() );
								m_oProjects.insert( make_pair( project, CProject( project ) ) );
							}

							// Looking for project properties
							for( auto inner = tr.child( it, 0 ); inner != it.end(); ++ inner )
							{
								if ( inner->isTag() && inner->tagName() == "img" )
								{
									inner->parseAttributes();
									const auto src = inner->attribute( "src" );
									if ( src.first )
									{
										TRACE( "Image -> %s\n", src.second.c_str() );
										m_oProjects[ project ].image = src.second;
										break;
									}
								}
							}
						}
					}
				}
			}
		}
		catch (...) {}
	}
}

void CMineModMonDlg::Start()
{
	m_eventThreadStop.ResetEvent();

	ASSERT( m_pThread == NULL );
	m_pThread = AfxBeginThread( ThreadFn, this );
}

void CMineModMonDlg::Stop()
{
	// Ask thread termination
	m_eventThreadStop.SetEvent();

	// Wait for termination
	while ( m_pThread && WaitForSingleObject( m_pThread->m_hThread, 100 ) == WAIT_TIMEOUT )
	{
		// Message loop
		MSG msg;
		while ( PeekMessage( &msg, NULL, NULL, NULL, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		if ( GetWindowThreadProcessId( AfxGetMainWnd()->GetSafeHwnd(), NULL ) == GetCurrentThreadId() )
		{
			LONG lIdle = 0;
			while ( AfxGetApp()->OnIdle( lIdle++ ) );
		}
	}

	// Clean-up
	delete m_pThread;
	m_pThread = NULL;
}

BOOL CMineModMonDlg::IsWorkEnabled() const
{
	return ( WaitForSingleObject( m_eventThreadStop, 0 ) == WAIT_TIMEOUT );
}

UINT __cdecl CMineModMonDlg::ThreadFn( LPVOID pParam )
{
	( (CMineModMonDlg*)pParam )->Thread();
	return 0;
}

void CMineModMonDlg::Thread()
{
	m_pThread->m_bAutoDelete = FALSE;

	if ( SUCCEEDED( CoInitialize( NULL ) ) )
	{
		// Initialize WinInet
		CAutoPtr< CInternetSession > pInternet;
		try
		{
			pInternet.Attach( new CInternetSession( CString( theApp.m_pszAppName ) + _T("/") + theApp.sVersion ) );
		}
		catch (...) {}

		if ( pInternet && *pInternet )
		{
			while ( IsWorkEnabled() )
			{
				// Relaxing
				SwitchToThread();
				Sleep( 250 );

				DownloadIndex( pInternet );

				break;
			}

			pInternet.Free();
		}
	}

	CoUninitialize();
}

void CMineModMonDlg::OnSize( UINT nType, int cx, int cy )
{
	CDialogEx::OnSize( nType, cx, cy );

	UpdateSizes();
}
