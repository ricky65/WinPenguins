/**
 *  UrlStatic.cpp
 *
 *  Copyright (C) 2001  Michael Vines
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
#include "UrlStatic.h"


/////////////////////////////////////////////////////////////////////////////
// CUrlStatic

CUrlStatic::CUrlStatic()
{
}

CUrlStatic::~CUrlStatic()
{
}


BEGIN_MESSAGE_MAP(CUrlStatic, CStatic)
	//{{AFX_MSG_MAP(CUrlStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUrlStatic message handlers

void CUrlStatic::OnPaint() 
{
  CPaintDC dc(this); // device context for painting
	
  // Do not call CStatic::OnPaint() for painting messages

  CString text;
  GetWindowText(text);

  CPen pen(PS_SOLID, 1, RGB(0,0,255)), *oldPen;
  CGdiObject *oldFont;

  oldFont = dc.SelectStockObject(SYSTEM_FONT);
  oldPen = dc.SelectObject(&pen);

  dc.SetTextColor(RGB(0, 0, 255));
  dc.SetBkMode(TRANSPARENT);

  dc.TextOut(0, 0, text);

  const CSize size = dc.GetTextExtent(text);
  dc.MoveTo(0, size.cy);
  dc.LineTo(size.cx, size.cy);

  dc.SelectObject(oldPen);
  dc.SelectObject(oldFont);
}


BOOL CUrlStatic::Create(CWnd *parent, UINT staticId) 
{
  CString text;
  RECT rt;

  CWnd *base = parent->GetDlgItem(staticId);

  base->GetWindowRect(&rt);
  parent->ScreenToClient(&rt);

  base->GetWindowText(text);
  base->ShowWindow(SW_HIDE);

  return CStatic::Create(text, WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY, 
                         rt, parent, staticId);
}

 
void CUrlStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDown(nFlags, point);

	CString url;

	GetWindowText(url);

	const INT_PTR ShellExecuteResult = reinterpret_cast<const INT_PTR>(ShellExecute(nullptr, nullptr, url, nullptr, nullptr, SW_SHOWNORMAL));
	if (!(ShellExecuteResult > 32)) {
		MessageBox(L"Unable to open URL", L"Unable to launch web browser", MB_ICONWARNING);
	}
}
