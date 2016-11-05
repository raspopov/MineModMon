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
