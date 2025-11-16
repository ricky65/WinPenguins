/**
 *  winpenguinsDlg.cpp
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

#include "StdAfx.h"
#include "winpenguins.h"
#include "winpenguinsDlg.h"
#include "MainWnd.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	urlStatic.Create(this, IDC_STATIC_URL);

	SetForegroundWindow();

	return true;
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_STATIC_URL, &CAboutDlg::OnStnClickedStaticUrl)
END_MESSAGE_MAP()





void CAboutDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDblClk(nFlags, point);
}




/////////////////////////////////////////////////////////////////////////////
// CWinpenguinsDlg dialog


CWinpenguinsDlg::CWinpenguinsDlg(int penguinCount, int delay, int splat, int santa, unsigned char alpha, bool largePenguins, bool soundEnabled, CWnd* pParent)
	: CDialog{ IDD_WINpenguins_DIALOG, pParent }, m_penguincount{ penguinCount }, m_delay{ delay }, m_splat{ splat }, m_santa{ santa }, m_alpha{ alpha }, 
		m_largepenguins{ largePenguins }, m_soundenabled{ soundEnabled }, m_hIcon{ AfxGetApp()->LoadIcon(IDR_ICON) }
{
}

void CWinpenguinsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinpenguinsDlg)
	DDX_Slider(pDX, IDC_PENGUINCOUNT, m_penguincount);
	DDX_Slider(pDX, IDC_SPEED, m_delay);
	DDX_Slider(pDX, IDC_SPLAT, m_splat);
	DDX_Slider(pDX, IDC_ALPHA, m_alpha);
	DDX_Slider(pDX, IDC_SANTA, m_santa);
	DDX_Slider(pDX, IDC_PENGUINSIZE, m_largepenguins);
	DDX_Check(pDX, IDC_SOUNDENABLED, m_soundenabled);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinpenguinsDlg, CDialog)
	//{{AFX_MSG_MAP(CWinpenguinsDlg)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWinpenguinsDlg message handlers

BOOL CWinpenguinsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetForegroundWindow();

	CSliderCtrl* p = (CSliderCtrl*)GetDlgItem(IDC_PENGUINSIZE);
	p->SetRange(0, 1);
	p->SetTicFreq(1);
	p->SetPos(m_largepenguins);
	p->UpdateWindow();

	p = (CSliderCtrl*)GetDlgItem(IDC_PENGUINCOUNT);
	p->SetRange(1, CMainWnd::maxToonCount);
	p->SetTicFreq(10);
	p->SetPos(m_penguincount);
	p->UpdateWindow();

	p = (CSliderCtrl*)GetDlgItem(IDC_SPEED);
	p->SetRange(0, CMainWnd::maxMoveDelay);
	p->SetTicFreq(15);
	p->SetPos(m_delay);
	p->UpdateWindow();

	p = (CSliderCtrl*)GetDlgItem(IDC_SPLAT);
	p->SetRange(10, CMainWnd::winPenguinsWindowRect.bottom);
	p->SetTicFreq(100);
	p->SetPos(m_splat);
	p->UpdateWindow();

	p = (CSliderCtrl*)GetDlgItem(IDC_ALPHA);
	p->SetRange(0, 255);
	p->SetTicFreq(32);
	p->SetPos(m_alpha);
	p->UpdateWindow();

	p = (CSliderCtrl*)GetDlgItem(IDC_SANTA);
	p->SetRange(0, 100);
	p->SetTicFreq(10);
	p->SetPos(m_santa);
	p->UpdateWindow();

	CButton* b = (CButton*)GetDlgItem(IDC_SOUNDENABLED);
	b->SetCheck(m_soundenabled);

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CWinpenguinsDlg::OnApply()
{
	UpdateData(TRUE);
	((CMainWnd*)theApp.m_pMainWnd)->ApplyOptions(*this);
}




void CAboutDlg::OnStnClickedStaticUrl()
{
	// TODO: Add your control notification handler code here
}
