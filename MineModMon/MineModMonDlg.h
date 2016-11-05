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

#pragma once

#define TIMER			1

// Project class

class CProject
{
public:
	inline CProject()
		: dirty( true )
	{
	}

	inline CProject(const string& _id)
		: dirty( true )
		, id   ( _id ) 
	{
	}

	inline CProject(const CProject& prj)
		: dirty( prj.dirty )
		, id   ( prj.id )
		, image( prj.image )
	{
	}

	inline CProject& operator=(const CProject& prj)
	{
		dirty = prj.dirty;
		id    = prj.id;
		image = prj.image;
	}

	bool dirty;
	string id;
	string image;
};

typedef map< string, CProject > CProjects;


// CMineModMonDlg dialog

class CMineModMonDlg : public CDialogEx
{
public:
	CMineModMonDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_MINEMODMON_DIALOG };

protected:
	HICON					m_hIcon;
	CListCtrl				m_wndProjectList;

	CProjects				m_oProjects;

	volatile CWinThread*	m_pThread;
	CEvent					m_eventThreadStop;

	void Update();
	void UpdateSizes();

	static UINT __cdecl ThreadFn(LPVOID pParam);
	void Thread();
	void Start();
	void Stop();
	BOOL IsWorkEnabled() const;

	void DownloadIndex(CInternetSession* pSession);

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize( UINT nType, int cx, int cy );
};

enum { COL_ID = 0, COL_NAME };
