/**
 *  winpenguins.h
 *
 *  Copyright (C) 2000  Michael Vines
 *  Copyright (c) 2011-2022 Riccardo Marcangelo
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

#if !defined(AFX_WINpenguins_H__9EE34D44_6D41_4245_B2B2_08F6FAFBCCCA__INCLUDED_)
#define AFX_WINpenguins_H__9EE34D44_6D41_4245_B2B2_08F6FAFBCCCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinpenguinsApp:
// See winpenguins.cpp for the implementation of this class
//

class CWinpenguinsApp : public CWinApp
{
public:
	CWinpenguinsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinpenguinsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinpenguinsApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CWinpenguinsApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINpenguins_H__9EE34D44_6D41_4245_B2B2_08F6FAFBCCCA__INCLUDED_)
