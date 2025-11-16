/**
 *  winpenguinsDlg.h
 *
 *  Copyright (C) 2000  Michael Vines
 *  Copyright (c) 2011-2025 Riccardo Marcangelo
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  As a special exception, Michael Vines gives permission to link this program
 *  with the Microsoft Visual C++ Runtime/MFC Environment, and distribute the
 *  resulting executable, without including the source code for the Microsoft 
 *  Visual C++ Runtime/MFC Environment in the source distribution
 */

#if !defined(AFX_WINpenguinsDLG_H__9A375BB6_4AD9_4221_9C06_B83B6BF7DDA8__INCLUDED_)
#define AFX_WINpenguinsDLG_H__9A375BB6_4AD9_4221_9C06_B83B6BF7DDA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UrlStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	virtual BOOL OnInitDialog();

// Implementation
protected:
	CUrlStatic urlStatic;

	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticUrl();
};


/////////////////////////////////////////////////////////////////////////////
// CWinpenguinsDlg dialog

class CWinpenguinsDlg : public CDialog
{
// Construction
public:
	CWinpenguinsDlg(int penguinCount, int delay, int splat, int santa, unsigned char alpha, bool largePenguins, bool soundEnabled, CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWinpenguinsDlg)
	int		m_penguincount;
	int		m_delay;
	int		m_splat;	
	int		m_santa;
	int		m_alpha;
	int		m_largepenguins;
	int		m_soundenabled;	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinpenguinsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWinpenguinsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINpenguinsDLG_H__9A375BB6_4AD9_4221_9C06_B83B6BF7DDA8__INCLUDED_)
