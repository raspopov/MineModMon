// MineModMonDlg.h : header file
//

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
};

enum { COL_ID = 0, COL_NAME };
