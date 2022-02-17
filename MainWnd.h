/**
 *  MainWnd.h
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

#if !defined(AFX_MAINWND_H__A5A421E7_D255_40F8_8E29_9C2DD2A83624__INCLUDED_)
#define AFX_MAINWND_H__A5A421E7_D255_40F8_8E29_9C2DD2A83624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

#define UWM_SYSTRAY (WM_USER + 1) // Sent to us by the systray

#include "toon.h"
#include "Afxtempl.h"
#include "winpenguinsdlg.h"
#include "def.h"

#include <vector>

//RNG
using Random = effolkronium::random_static;

enum TimerEvent : UINT_PTR {
	InitializeScreenBitmaps = 1,
	MainLoop = 2
};


#ifdef WP_CONSOLE
static HANDLE Console = nullptr;

void NewLineToConsole();

void ToConsole(const wchar_t* str);
void ToConsole(const std::wstring& str);

//Rick: Write formatted string to console (wrapper around std::format and WriteConsoleW)
template <typename... Args >
void ToConsole(const std::wstring_view formatStr, const Args&... args)
{
	const std::wstring strBuffer = std::vformat(formatStr, std::make_wformat_args(args...));

	DWORD numCharsWritten = 0;
	WriteConsoleW(Console, strBuffer.c_str(), static_cast<DWORD>(strBuffer.length()), &numCharsWritten, nullptr);

	NewLineToConsole();
}

void LoadConsole();
#endif // WP_CONSOLE

// Hook events
static HWINEVENTHOOK LocationChangeHook;
static HWINEVENTHOOK ShowHideHook;
static HWINEVENTHOOK CloakedUncloakedHook;
static bool windowChanged = true;

void CALLBACK HandleWindowEvents(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
void InitializeHooks();
void ShutdownHooks();

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CMainWnd : public CFrameWnd
{
// Construction
public:
	CMainWnd();

// Attributes
public:

  // slowest allowed time between frames in ms
  static constexpr int maxMoveDelay = 150;

  // max number of toons
  static constexpr int maxToonCount = 50;
	
  //Rick 2021: Registry entry strings
  static constexpr wchar_t optionsRegStr[] = L"Options";

  static constexpr wchar_t penguinCountStr[] = L"PenguinCount";
  static constexpr wchar_t moveDelayStr[] = L"MoveDelay";
  static constexpr wchar_t splatDistanceStr[] = L"SplatDistance";  
  static constexpr wchar_t santaPercentStr[] = L"SantaPercent";
  static constexpr wchar_t blendLevelStr[] = L"BlendLevel";
  static constexpr wchar_t largePenguinsEnabledStr[] = L"LargePenguinsEnabled";
  static constexpr wchar_t soundEnabledStr[] = L"SoundEnabled";

  //Rick 2021: Default values for first run
  static constexpr int defaultNumPenguins = maxToonCount;
  static constexpr int defaultMoveDelay = maxMoveDelay / 2 ;
  static constexpr int defaultSantaPercent = 0 ;
  static constexpr unsigned char defaultBlendLevel = 255;
  static constexpr bool defaultSoundEnabled = true;

  // The region that covers all top level windows
  inline static CRgn combinedWindowRegion;

  // Window rectangle for WinPenguins overlay (same as Desktop window rectangle)
  inline static RECT winPenguinsWindowRect;

private:
	int m_numpenguins;
	int m_moveDelay;
	int m_splatDist;	
	int m_santaPercent;// number between 0-100: 0 = no santa, 100 = always santa	
	unsigned char m_blendLevel;// alpha blending level (0-255)	
	bool m_largePenguinsEnabled;// large penguins toggle: 1 = large penguins, 0 = small penguins
	bool m_soundEnabled;

	std::vector<CToon> m_toonVec;

	CBitmap m_bgBitmap;
	CBitmap m_activeBmp;

	HICON m_trayIcon;
	HANDLE m_hInstanceMutex;

	CDialog* m_activeDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	//}}AFX_VIRTUAL

// Implementation
public:

	UINT getDPIScale(HWND hwnd);
	RECT getWindowRectDWM(HWND hwnd);

	BOOL IsWindowCloaked(HWND hwnd);
	BOOL IsWindowVisibleOnScreen(HWND hwnd);

	bool CheckSubType(CToon& pToon);
	void UpdateWindowRegion(const RECT* WindowRect);
	void FindVisibleWindows();
	void CheckForWindowChanges();
	void InitializeScreenBitmaps();
	void SetToonCountTo(int count);
	BOOL SetWindowAlphaValue(unsigned char alphaValue);
	void ApplyOptions(const CWinpenguinsDlg & dlg);	

	virtual ~CMainWnd() override;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSysTray(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAbout();
	afx_msg void OnExit();
	afx_msg void OnOptions();
	afx_msg void OnScreenCapture();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__A5A421E7_D255_40F8_8E29_9C2DD2A83624__INCLUDED_)



