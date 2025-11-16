/**
 *  StdAfx.h
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


// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__08BD59BB_BBEA_4CE3_8A50_A3698B638E5C__INCLUDED_)
#define AFX_STDAFX_H__08BD59BB_BBEA_4CE3_8A50_A3698B638E5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER			0x0A00 // 0x0A00 is Windows 10
#define _WIN32_WINNT	0x0A00

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#ifdef _DEBUG
#define WP_CONSOLE				//for WinPenguins Console
//#define WP_FRAME_WINDOW_REGIONS	//For testing draw a frame around the found window regions (where penguins cant access)
#endif // _DEBUG 

#include "lib/random/random.hpp"

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__08BD59BB_BBEA_4CE3_8A50_A3698B638E5C__INCLUDED_)
