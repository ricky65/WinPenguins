/**
 *  UrlStatic.h
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

#if !defined(AFX_URLSTATIC_H__7E240B89_C265_4298_87CF_4FB6D8FB612B__INCLUDED_)
#define AFX_URLSTATIC_H__7E240B89_C265_4298_87CF_4FB6D8FB612B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UrlStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUrlStatic window

class CUrlStatic : public CStatic
{
// Construction
public:
	CUrlStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrlStatic)
	public:
	virtual BOOL Create(CWnd *parent, UINT staticId);

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUrlStatic();

	// Generated message map functions
protected:
	//BOOL m_mouseDown;
	//{{AFX_MSG(CUrlStatic)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLSTATIC_H__7E240B89_C265_4298_87CF_4FB6D8FB612B__INCLUDED_)
