/**
 *  MainWnd.cpp
 *
 *  Copyright (C) 2000  Michael Vines
 *  Copyright (c) 2011-2022 Riccardo Marcangelo
 *
 *  This file contains code from Xpenguins 1.1
 *		Copyright (C) 1999,2000  Robin Hogan
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
#include "Def.h"
#include "ScreenCapture.h"

#pragma comment (lib, "dwmapi.lib")


#ifdef WP_CONSOLE

void NewLineToConsole()
{
	const wchar_t newLine[] = L"\r\n";
	const int newLineLen = lstrlenW(newLine);
	DWORD numCharsWritten;
	WriteConsoleW(Console, newLine, newLineLen, &numCharsWritten, nullptr);
}

//Rick: Write string to console (WriteConsoleW wrapper)
void ToConsole(const wchar_t* str)
{
	DWORD numCharsWritten = 0;
	const int strLen = lstrlenW(str);
	WriteConsoleW(Console, str, strLen, &numCharsWritten, nullptr);
	NewLineToConsole();
}

//Rick: Write string to console (WriteConsoleW wrapper)
void ToConsole(const std::wstring& str)
{
	DWORD numCharsWritten = 0;
	WriteConsoleW(Console, str.c_str(), static_cast<DWORD>(str.length()), &numCharsWritten, nullptr);
	NewLineToConsole();
}

void LoadConsole()
{	
	const BOOL ACRes = AllocConsole();
	if (!ACRes) {
		const std::wstring ACErrorStr = std::format(L"AllocConsole Error: {}", GetLastError());
		MessageBoxW(nullptr, ACErrorStr.c_str(), L"Internal Error", MB_ICONERROR);
		return;
	}

	Console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (Console == INVALID_HANDLE_VALUE) {
		const std::wstring GetStdHandleErrorStr = std::format(L"GetStdHandle Error: {}", GetLastError());
		MessageBoxW(nullptr, GetStdHandleErrorStr.c_str(), L"Internal Error", MB_ICONERROR);
		return;
	}

	ToConsole(L"WinPenguins Debug Console");
}
#endif // WP_CONSOLE

/////////////////////////////////////////////////////////////////////////////



// Rick 2021: Callback function that handles events so we get notified when a window changes and can update the found windows region
void CALLBACK HandleWindowEvents(HWINEVENTHOOK hook, DWORD event, HWND hwnd,
	LONG idObject, LONG idChild,
	DWORD dwEventThread, DWORD dwmsEventTime)
{
	if (hwnd &&
		idObject == OBJID_WINDOW &&
		idChild == CHILDID_SELF)
	{
		windowChanged = true;

#ifdef WP_CONSOLE
		switch (event)
		{
		case EVENT_OBJECT_LOCATIONCHANGE:
			ToConsole(L"EVENT_OBJECT_LOCATIONCHANGE: {}", static_cast<const void*>(hwnd));
			break;
		case EVENT_OBJECT_SHOW:
			ToConsole(L"EVENT_OBJECT_SHOW: {}", static_cast<const void*>(hwnd));
			break;
		case EVENT_OBJECT_HIDE:
			ToConsole(L"EVENT_OBJECT_HIDE: {}", static_cast<const void*>(hwnd));
			break;
		case EVENT_OBJECT_CLOAKED:
			ToConsole(L"EVENT_OBJECT_CLOAKED: {}", static_cast<const void*>(hwnd));
			break;
		case EVENT_OBJECT_UNCLOAKED:
			ToConsole(L"EVENT_OBJECT_UNCLOAKED: {}", static_cast<const void*>(hwnd));
			break;
		default:
			break;
		}
#endif			
	}
}

//Rick 2021: Initialize event hooks to monitor windows changes
void InitializeHooks()
{
	LocationChangeHook = SetWinEventHook(
		EVENT_OBJECT_LOCATIONCHANGE, 
		EVENT_OBJECT_LOCATIONCHANGE,  // Range of events
		nullptr,                                          // Handle to DLL.
		HandleWindowEvents,                                // The callback.
		0, 0,              // Process and thread IDs of interest (0 = all)
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS); // Flags.

	ShowHideHook = SetWinEventHook(
		EVENT_OBJECT_SHOW,
		EVENT_OBJECT_HIDE,
		nullptr,
		HandleWindowEvents,
		0, 0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

	CloakedUncloakedHook = SetWinEventHook(
		EVENT_OBJECT_CLOAKED,
		EVENT_OBJECT_UNCLOAKED,
		nullptr,
		HandleWindowEvents,
		0, 0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
}

//Rick 2021: Unhook event hooks
void ShutdownHooks()
{
	if (LocationChangeHook)
		UnhookWinEvent(LocationChangeHook);

	if (ShowHideHook)
		UnhookWinEvent(ShowHideHook);

	if (CloakedUncloakedHook)
		UnhookWinEvent(CloakedUncloakedHook);
}

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

//Rick 2021: DPI scaling support for found windows
UINT CMainWnd::getDPIScale(HWND hwnd)
{
	// Determine the DPI to use, according to the DPI awareness mode
	const DPI_AWARENESS dpiAwareness = GetAwarenessFromDpiAwarenessContext(GetThreadDpiAwarenessContext());
	switch (dpiAwareness)
	{
		// No DPI awareness/invalid DPI awareness. Use 96 DPI default scale factor of 100%
	case DPI_AWARENESS::DPI_AWARENESS_INVALID:
	case DPI_AWARENESS::DPI_AWARENESS_UNAWARE:
		break;

		// Scale the window to the system DPI
	case DPI_AWARENESS::DPI_AWARENESS_SYSTEM_AWARE:
		return GetDpiForSystem();

		// Scale the window to the monitor DPI
	case DPI_AWARENESS::DPI_AWARENESS_PER_MONITOR_AWARE:
		return GetDpiForWindow(hwnd);

	default:
		break;
	}

	return 96;
}

RECT CMainWnd::getWindowRectDWM(HWND hwnd)
{
	RECT WindowRect;
	const HRESULT DWMGWA_Result = DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &WindowRect, sizeof(WindowRect));


	if (!(DWMGWA_Result == S_OK)) {
		const std::wstring DWMGWAErrorStr = std::format(L"Error obtaining WindowRect: DwmGetWindowAttribute failed with error code: {:#010x}", DWMGWA_Result);
#ifdef WP_CONSOLE		
		ToConsole(DWMGWAErrorStr);
#endif // WP_CONSOLE
		MessageBoxW(DWMGWAErrorStr.c_str(), L"Internal Error", MB_ICONERROR);
	}

	//Rick 2021: Check for DPI scaling
	const UINT DPI = getDPIScale(hwnd);

	if (DPI == 96) // Rick 2021: 96 DPI is default scale factor of 100%, no adjustment needed
		return WindowRect;

	//Rick 2021: adjust window rect for DPI
	const BOOL adjustRectForDPI_Res = AdjustWindowRectExForDpi(&WindowRect, 0, FALSE, 0, DPI);

	if (!adjustRectForDPI_Res) {
		const std::wstring adjustRectForDPIErrorStr = std::format(L"Error: AdjustWindowRectExForDpi failed with error code: {}", GetLastError());
#ifdef WP_CONSOLE
		ToConsole(adjustRectForDPIErrorStr);
#endif // WP_CONSOLE
		MessageBoxW(adjustRectForDPIErrorStr.c_str(), L"Internal Error", MB_ICONERROR);
	}

	return WindowRect;
}

BOOL CMainWnd::IsWindowCloaked(HWND hwnd)
{
	//DwmGetWindowAttribute introduced in Windows Vista
	BOOL isCloaked = FALSE;
	const HRESULT DWMGWA_Result = DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &isCloaked, sizeof(isCloaked));

	return ((DWMGWA_Result == S_OK) && isCloaked);
}

BOOL CMainWnd::IsWindowVisibleOnScreen(HWND hwnd)
{
	return ::IsWindowVisible(hwnd) && !IsWindowCloaked(hwnd);
}

void CMainWnd::UpdateWindowRegion(const RECT * WindowRect)
{
	// All good, add this window to the region	
	if (CMainWnd::combinedWindowRegion.GetSafeHandle()) {
		CRgn currentWindowRegion;
		currentWindowRegion.CreateRectRgnIndirect(WindowRect);

		CMainWnd::combinedWindowRegion.CombineRgn(&CMainWnd::combinedWindowRegion, &currentWindowRegion, RGN_OR);

		currentWindowRegion.DeleteObject();//cleanup
	}
	else //create rectangle region if empty
	{
		CMainWnd::combinedWindowRegion.CreateRectRgnIndirect(WindowRect);
	}
}

//Rick 2021: Find all windows that are child windows of the desktop and add to region 
void CMainWnd::FindVisibleWindows()
{	
	static const wchar_t taskbarClassName[] = L"Shell_TrayWnd";//Taskbar
	static const wchar_t programManagerTitle[] = L"Program Manager";
	
	HWND prevWindow = nullptr;
	for (;;) 
	{
		//Rick 2021: Iterate through all windows that are child windows of the desktop.
		//Using FindWindowEx as EnumWindows fails to retrieve some system windows such as Search and Action Center 
		const HWND currentWindow = ::FindWindowEx(nullptr, prevWindow, nullptr, nullptr);

		//Rick 2021: break out of loop when we have iterated through all child windows of the desktop. 
		if (!currentWindow)
			break;		

		prevWindow = currentWindow;

		// Ignore windows in our process
		DWORD pid;
		GetWindowThreadProcessId(currentWindow, &pid);
		if (pid == GetCurrentProcessId()) {
			continue;
		}				

		wchar_t windowTitleBuf[32];
		::GetWindowText(currentWindow, windowTitleBuf, static_cast<int>(std::size(windowTitleBuf)));

		// Ignore the "Program Manager" window
		if (!_wcsicmp(windowTitleBuf, programManagerTitle)) {
			continue;
		}
		
		wchar_t classNameBuf[32];
		GetClassName(currentWindow, classNameBuf, static_cast<int>(std::size(classNameBuf)));

		// Rick 2021: Check if hwnd is the taskbar (it does not have a window title)
		const bool taskbarFound = _wcsicmp(classNameBuf, taskbarClassName) == 0;
#ifdef WP_CONSOLE
		if (taskbarFound) {
			ToConsole(L"Taskbar found: (HWND: {})", static_cast<const void*>(currentWindow));
		}
#endif	// WP_CONSOLE	

		const int winTextLength = ::GetWindowTextLength(currentWindow);

		//Rick 2021: Skip windows that are not visible and do not have title bar text (exclude the taskbar)
		if (!(IsWindowVisibleOnScreen(currentWindow) && !::IsIconic(currentWindow) && winTextLength != 0) && !taskbarFound) {			
//#ifdef WP_CONSOLE
//			ToConsole(L"Discarded window (HWND: {}| WindowTitle: {}| WindowClass: {})", static_cast<const void*>(currentWindow), windowTitleBuf, classNameBuf);
//#endif // WP_CONSOLE
			continue;
		}

#ifdef WP_CONSOLE
		ToConsole(L"Window found (HWND: {}| WindowTitle: {}| WindowClass: {})", static_cast<const void*>(currentWindow), windowTitleBuf, classNameBuf);
#endif // WP_CONSOLE
	
		// Rick 2021: Get bounding rectangle of found window, taking into account DPI Scaling
		// 
		// Rick 2021: Note regarding The Start window (The window that appears when Start button is pressed (Title: "Start" Classname:"Windows.UI.Core.CoreWindow") 
		// My Windows 10 [Version 10.0.19042.867] reports what appears to be the wrong dimensions of the window bounding rectangle 
		// It appears to always return the RECT as if Start is full screen, even if it is not fullscreen!
		const RECT currentWindowRect = getWindowRectDWM(currentWindow);

		//Rick 2021: Add found visible window to region
		UpdateWindowRegion(&currentWindowRect);
	}

}


CMainWnd::CMainWnd()
{

#ifdef WP_CONSOLE
	LoadConsole();
#endif	
	
    // Check if another instance of WinPenguins is already running
	m_hInstanceMutex = ::CreateMutex(nullptr, FALSE, L"WinPenguinsInstanceMutex");
	if (!m_hInstanceMutex) {
		const std::wstring mutexStr = std::format(L"Unable to create WinPenguins Instance Mutex. CreateMutex error: {}", GetLastError());
#ifdef WP_CONSOLE		
		ToConsole(mutexStr);
#endif WP_CONSOLE //WP_CONSOLE
		MessageBox(mutexStr.c_str(), L"Internal Error", MB_ICONERROR);
		ExitProcess(1);
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		ExitProcess(0);  	// Exit quitely if the mutex already exists
	}

	// Load the penguin bitmap resources
	for (int i = 0; i < penguin_BITMAP_COUNT; ++i) {
		penguin_data[i].bmp.LoadBitmap(MAKEINTRESOURCE(penguin_data[i].resId));
		penguin_data[i].mskBmp.LoadBitmap(MAKEINTRESOURCE(penguin_data[i].mskResId));
	}

	// Find the desktop window 
	const HWND desktopHWND = ::GetDesktopWindow();
	if (desktopHWND) {
		//Rick 2021: Get rectangle of Desktop window - we want WinPenguins overlay window to be same size
		::GetWindowRect(desktopHWND, &winPenguinsWindowRect);
	}
	else {
		const wchar_t desktopHWNDStr[] = L"GetDesktopWindow Error";
#ifdef WP_CONSOLE
		ToConsole(desktopHWNDStr);
#endif WP_CONSOLE //WP_CONSOLE
		MessageBoxW(desktopHWNDStr, L"Internal Error", MB_ICONERROR);
		ExitProcess(1);		
	}

	// Rick: Load config options from registry 
	m_numpenguins = theApp.GetProfileInt(optionsRegStr, penguinCountStr, defaultNumPenguins);
	m_moveDelay = theApp.GetProfileInt(optionsRegStr, moveDelayStr, defaultMoveDelay);
	m_splatDist = theApp.GetProfileInt(optionsRegStr, splatDistanceStr, winPenguinsWindowRect.bottom);	
	SYSTEMTIME st;
	GetSystemTime(&st);
	//Rick: all penguins are santa penguins during December 
	m_santaPercent = st.wMonth == 12 ? 100 : theApp.GetProfileInt(optionsRegStr, santaPercentStr, defaultSantaPercent);
	m_blendLevel = theApp.GetProfileInt(optionsRegStr, blendLevelStr, defaultBlendLevel);
	//Rick 2021: Let's have large penguins default on displays 1080p and larger
	m_largePenguinsEnabled = theApp.GetProfileInt(optionsRegStr, largePenguinsEnabledStr, winPenguinsWindowRect.right >= 1920 && winPenguinsWindowRect.bottom >= 1080);
	m_soundEnabled = theApp.GetProfileInt(optionsRegStr, soundEnabledStr, defaultSoundEnabled);
	
	//Rick 2021: Create transparent layered window which we can use to render penguins
	const BOOL createWindowResult = Create(nullptr, L"WinPenguins", WS_POPUP | WS_VISIBLE,
		winPenguinsWindowRect,
		nullptr,
		nullptr,
		WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, //WS_EX_TOOLWINDOW so it does not appear in taskbar
		nullptr);

	if (!createWindowResult) {
		const wchar_t createWindowResultStr [] = L"CFrameWnd::Create Error: Unable to create WinPenguins overlay window";		
#ifdef WP_CONSOLE		
		ToConsole(createWindowResultStr);
#endif //WP_CONSOLE
		MessageBoxW(createWindowResultStr, L"Internal Error", MB_ICONERROR);
		ExitProcess(1);
	}

	SetWindowAlphaValue(m_blendLevel);	

	m_toonVec.reserve(m_numpenguins);

	SetToonCountTo(m_numpenguins);	
	
	m_trayIcon = theApp.LoadIcon(IDR_ICON);
	
	// Create the systray icon
	NOTIFYICONDATA ni { .cbSize = sizeof(NOTIFYICONDATA), .hWnd = m_hWnd, .uID = 1, .uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP,
		.uCallbackMessage = UWM_SYSTRAY, .hIcon = m_trayIcon, .szTip = L"WinPenguins" };

	BOOL sniResult = Shell_NotifyIcon(NIM_ADD, &ni);
	if (!sniResult) {
		const wchar_t sniErrorStr[] = L"Shell_NotifyIcon NIM_ADD Error";
#ifdef WP_CONSOLE
		ToConsole(sniErrorStr);
#endif //WP_CONSOLE
		MessageBoxW(sniErrorStr, L"Internal Error", MB_ICONERROR);
	}

	//Rick 2021: Initialize hooks for system wide window changes
	InitializeHooks();

	// Rick: Initialize screen bitmaps timer
	SetTimer(TimerEvent::InitializeScreenBitmaps, USER_TIMER_MINIMUM, nullptr);
	// Movement timer 
	SetTimer(TimerEvent::MainLoop, m_moveDelay, nullptr);

	m_activeDlg = nullptr;	
}


CMainWnd::~CMainWnd() 
{
	::CloseHandle(m_hInstanceMutex);	

	ShutdownHooks();

	m_bgBitmap.DeleteObject();
	m_activeBmp.DeleteObject();	

#ifdef WP_CONSOLE
	FreeConsole();
#endif
}

BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_MESSAGE(UWM_SYSTRAY, OnSysTray)
	ON_WM_CLOSE()
	ON_WM_TIMER() 
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_OPTIONS, OnOptions)
	ON_COMMAND(ID_SCREENCAP, OnScreenCapture)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

//Rick 2021: Override CMainFrame::PreCreateWindow to remove WS_EX_CLIENTEDGE causing 1 pixel border
BOOL CMainWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// Call the base-class version
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;	

	return TRUE;
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	return 0;
}


LRESULT CMainWnd::OnSysTray(WPARAM wParam, LPARAM lParam) 
{
	switch (lParam) {
	case WM_LBUTTONUP:
		if (m_activeDlg) {
			m_activeDlg->SetForegroundWindow();
			break;
		}

		OnOptions();
		break;

	case WM_RBUTTONDOWN:
		if (m_activeDlg) {
			m_activeDlg->SetForegroundWindow();
			break;
		}

		{
			POINT pt;
			::GetCursorPos(&pt);

			CMenu menu;
			menu.LoadMenu(MAKEINTRESOURCE(IDR_SYSTRAY));

			CMenu* sysMenu = menu.GetSubMenu(0);
			sysMenu->SetDefaultItem(1, TRUE);
			sysMenu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON,
				pt.x, pt.y, this);
		}
		break;

	case WM_LBUTTONDBLCLK:
		if (m_activeDlg) {
			m_activeDlg->SetForegroundWindow();
			break;
		}

		PostMessage(WM_COMMAND, ID_OPTIONS, 0);
		break;

	default:
		break;
	}
	return 0;
}


void CMainWnd::OnClose() 
{
	if (m_numpenguins > 0 || !m_toonVec.empty()) 
	{
		m_numpenguins = 0;
		SetToonCountTo(m_numpenguins);
		return;
	}  
	
	CWnd::OnClose();
}


void CMainWnd::OnTimer(UINT_PTR nIDEvent) //rick - updated so now compiles to x64
{
	switch (nIDEvent) 
	{
	case TimerEvent::InitializeScreenBitmaps:
		// Rick 2021: only want to call InitializeScreenBitmaps() once when overlay window is fully initialized
		InitializeScreenBitmaps();
		KillTimer(TimerEvent::InitializeScreenBitmaps);
		break;
	case TimerEvent::MainLoop:
	{
		CheckForWindowChanges();

		for (CToon& toon : m_toonVec)
		{
			// Rick 2021: Explode penguin if inside window, or on mouse over (excluding ballooners we pop later)
			if (toon.IsBlocked(TOON_HERE) || (toon.IsMouseOver() && (toon.m_bmpIndex != penguin_BALLOONER && toon.m_bmpIndex != penguin_BALLOONERLARGE))) {
				toon.ExplodeAni(m_soundEnabled);
			}			
			
			const int status = toon.AdvanceToon();

			//Rick 2021: Check if the newly spawned penguin has fully entered from top of screen
			toon.CheckFullyEnteredScreen();

			switch (toon.m_bmpIndex)
			{
			case penguin_FALLER:
			case penguin_FALLERLARGE:
				//Rick 2021: if blocked left or right - climb
				if (toon.IsBlocked(TOON_LEFT) || toon.IsBlocked(TOON_RIGHT))
				{
					//Rick 2021 : Don't want to transform into climber if starting and above top of screen
					if (toon.IsStartingUp()) {
						break;
					}

					CheckSubType(toon);

					if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
						toon.SetType(penguin_CLIMBER, toon.m_u > 0);
					}
					else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
						toon.SetType(penguin_SANTA_CLIMBER, toon.m_u > 0);
					}

					toon.SetVelocity(0, -4);
				}
				else if (status != TOON_OK)
				{
					if (toon.IsBlocked(TOON_DOWN))
					{
						const int direction = (toon.m_prefd > -1) ? toon.m_prefd : Random::get(0, 1);

						CheckSubType(toon);

						//Rick 2021: when penguin lands on the top of a window transform to either skateboarder or walker with 50/50 probability 
						if (Random::get<bool>())
						{//walker
							if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
								toon.SetType(penguin_WALKER, direction);
							}
							else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
								toon.SetType(penguin_SANTA_WALKER, direction);
							}

							toon.SetVelocity(4 * ((2 * direction) - 1), 0);
							toon.m_prefd = -1;
						}
						else//skateboarder
						{
							toon.SetType(penguin_SKATEBOARDER, direction);
							toon.SetVelocity(7 /*<< change to adjust speed of skateboarder*/ * ((2 * direction) - 1), 0);
							toon.m_prefd = -1;
							if (!m_largePenguinsEnabled)
								toon.m_y += 2;//Rick: compensate for bmp being 30x30 instead of 32x32- not necessary with large penguin																
						}
					}
				}
				break;
			case penguin_TUMBLER:
			case penguin_TUMBLERLARGE:
				if (status != TOON_OK) {
					//Rick: splat if splatdist is more than tumble distance
					if (toon.m_y - toon.m_tumbleStartY > m_splatDist) {
						toon.SplatAni(m_soundEnabled);
						break;
					}

					//no splat so walk left or right
					const int direction = Random::get(0, 1); // 0 - left, 1 right

					CheckSubType(toon);

					//Rick 2021: when penguin lands on the top of a window transform to either skateboarder or walker with 50/50 probability 
					if (Random::get<bool>())
					{//walker
						if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
							toon.SetType(penguin_WALKER, direction);
						}
						else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
							toon.SetType(penguin_SANTA_WALKER, direction);
						}

						toon.SetVelocity(4 * ((2 * direction) - 1), 0);
						toon.m_prefd = -1;
					}
					else//skateboarder
					{
						toon.SetType(penguin_SKATEBOARDER, direction);
						toon.SetVelocity(7 /*<< change to adjust speed of skateboarder*/ * ((2 * direction) - 1), 0);
						toon.m_prefd = -1;
						if (!m_largePenguinsEnabled)
							toon.m_y += 2;//Rick: compensate for bmp being 30x30 instead of 32x32- not necessary with large penguin																	
					}
				}
				break;
			case penguin_WALKER:
			case penguin_SANTA_WALKER:
			case penguin_WALKERLARGE:
			case penguin_SANTA_WALKERLARGE:
				//Rick 2021: 1 in 100 chance of reading
				if (Random::get<bool>(0.01)) {
					//start reading
					toon.m_prefd = toon.m_directionIndex;
					toon.SetType(penguin_READER, penguin_FOREWARD);
					toon.SetVelocity(0, 0);
					//Rick 2021: compensate for width and height of reader being 2px less than walker - not necessary with large penguin
					if (!m_largePenguinsEnabled)
						toon.SetPosition(toon.m_x + 1, toon.m_y + 2);

					break;
				}
				if (!toon.IsBlocked(TOON_DOWN)) {
					toon.m_prefd = toon.m_directionIndex;
					toon.SetType(penguin_TUMBLER, penguin_FOREWARD);
					toon.SetVelocity(0, 6);
					toon.m_tumbleStartY = toon.m_y;
				}
				else if (status != TOON_OK) {
					/* Blocked!  We can turn around, fly or climb */

					//Rick 2021 : Don't want to transform into climber, floater, or super if starting and above top of screen
					//Odds - 1/4 Floater, 1/4 Climber, 1/8 Super, 3/8 walk back the other way
					const int transition = toon.IsStartingUp() ? 6 : Random::get(1, 8);

					switch (transition) {
					case 1:
					case 2:
						toon.SetType(penguin_FLOATER, penguin_FOREWARD);
						toon.SetVelocity(Random::get(0, 4 * -toon.m_u / 4), -3);
						break;
					case 3:
					case 4:
						CheckSubType(toon);

						if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
							toon.SetType(penguin_CLIMBER);
						}
						else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
							toon.SetType(penguin_SANTA_CLIMBER);
						}

						toon.SetVelocity(0, -4);
						break;
					case 5:
						toon.SetType(penguin_SUPER, !toon.m_directionIndex);
						toon.SetVelocity(Random::get(0, 9 * -toon.m_u / 4), -8);
						break;
					case 6:
					case 7:
					case 8:
						//walk back the other way
						CheckSubType(toon);

						if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
							toon.SetType(penguin_WALKER, !toon.m_directionIndex);
						}
						else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
							toon.SetType(penguin_SANTA_WALKER, !toon.m_directionIndex);
						}
						toon.SetVelocity(-toon.m_u, 0);
						break;
					}
				}
				break;
			case penguin_WALKER_UPSIDE_DOWN:
			case penguin_SANTA_WALKER_UPSIDE_DOWN:
			case penguin_WALKER_UPSIDE_DOWNLARGE:
			case penguin_SANTA_WALKER_UPSIDE_DOWNLARGE:
				//Rick 2021: If above isn't blocked, climb up
				if (!toon.IsBlocked(TOON_UP)) {
					//Rick 2021: Make sure we haven't overshot the window
					//m_directionIndex = 1 when moving left
					//m_directionIndex = 0 when moving right
					for (int max = 1; !toon.IsBlocked(TOON_UP) && max < 10; ++max) {
						if (toon.m_directionIndex)
							++toon.m_x;
						else
							--toon.m_x;
					}
					if (toon.m_directionIndex)
						--toon.m_x;
					else
						++toon.m_x;

					CheckSubType(toon);
					if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
						toon.SetType(penguin_CLIMBER);
					}
					else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
						toon.SetType(penguin_SANTA_CLIMBER);
					}
					toon.SetVelocity(0, -4);
					toon.m_y -= 20;
				}
				//Rick 2021: Blocked - Turn around
				else if (toon.IsBlocked(!toon.m_directionIndex)) {
					//Rick 2021: 50/50 between changing direction and climbing down
					if (Random::get<bool>()) {
						toon.m_directionIndex = !toon.m_directionIndex;
						toon.SetVelocity(-toon.m_u, 0);
					}
					else {
						//Rick 2021: Climb Down
						CheckSubType(toon);
						if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
							toon.SetType(penguin_CLIMBER_DOWN);
						}
						else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
							toon.SetType(penguin_SANTA_CLIMBER_DOWN);
						}
						toon.SetVelocity(0, toon.m_directionIndex ? -toon.m_u : toon.m_u);
					}
				}
				break;
			case penguin_CLIMBER:
			case penguin_SANTA_CLIMBER:
			case penguin_CLIMBERLARGE:
			case penguin_SANTA_CLIMBERLARGE:
				if (!toon.IsBlocked(toon.m_directionIndex)) {//walk across window

					//Rick 2021: Make sure we haven't overshot the window
					for (int max = 1; !toon.IsBlocked(toon.m_directionIndex) && max < 10; ++max) {
						++toon.m_y;
					}
					--toon.m_y;

					CheckSubType(toon);

					if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
						toon.SetType(penguin_WALKER);
					}
					else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
						toon.SetType(penguin_SANTA_WALKER);
					}

					//Rick: shift x-coordinate +3/-3 depending on whether penguin is moving left or right
					toon.m_x += 3 * ((2 * toon.m_directionIndex) - 1);
					toon.SetVelocity(7 * ((2 * toon.m_directionIndex) - 1), 0);
				}
				//Rick 2021: Hit the top of a window or top of the screen
				else if (toon.IsBlocked(TOON_UP)) {

					//Rick 2021: 33/33/33 between faller, climb down, walk upside down
					//Rick 2021: Don't want to walk across top of screen
					const int nextType = Random::get(1, toon.m_y != CMainWnd::winPenguinsWindowRect.top ? 3 : 2);

					switch (nextType)
					{
					case 1:
						//Rick 2021: Faller					
						toon.SetVelocity(1 - toon.m_directionIndex * 2, 3);//Rick: m_u is +1/-1
						toon.SetType(penguin_FALLER, penguin_FOREWARD);
						break;

					case 2:
						//Rick 2021: Climb back down
						CheckSubType(toon);
						if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
							toon.SetType(penguin_CLIMBER_DOWN, !toon.m_directionIndex);
						}
						else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
							toon.SetType(penguin_SANTA_CLIMBER_DOWN, !toon.m_directionIndex);
						}
						toon.SetVelocity(0, -toon.m_v);//reverse direction
						break;
					case 3:
						//Rick 2021: Walk upside down
						CheckSubType(toon);
						if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
							toon.SetType(penguin_WALKER_UPSIDE_DOWN);
						}
						else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
							toon.SetType(penguin_SANTA_WALKER_UPSIDE_DOWN);
						}
						toon.SetVelocity(toon.m_directionIndex ? toon.m_v : -toon.m_v, 0);
						break;
					}
				}
				break;
				//Rick 2021: TOOO
			case penguin_CLIMBER_DOWN:
			case penguin_SANTA_CLIMBER_DOWN:
			case penguin_CLIMBER_DOWNLARGE:
			case penguin_SANTA_CLIMBER_DOWNLARGE:
				if (toon.IsBlocked(TOON_DOWN)) {//Rick 2021: run across window

					CheckSubType(toon);

					if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
						toon.SetType(penguin_WALKER);
					}
					else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
						toon.SetType(penguin_SANTA_WALKER);
					}

					//Rick: shift x-coordinate +3/-3 depending on whether penguin is moving left or right
					toon.m_x += 3 * ((2 * toon.m_directionIndex) - 1);
					toon.SetVelocity(7 * ((2 * toon.m_directionIndex) - 1), 0);
				}
				//Rick 2021: Walk upside down!
				else if (!toon.IsBlocked(!toon.m_directionIndex)/*&& !toon.IsBlocked(TOON_UP)*/) {

					//Rick 2021: Make sure we haven't overshot the window
					//m_directionIndex = 1 when moving down right side of window
					//m_directionIndex = 0 when moving down left side of window
					for (int max = 1; !toon.IsBlocked(!toon.m_directionIndex) && max < 10; ++max) {
						--toon.m_y;
					}
					++toon.m_y;

					CheckSubType(toon);

					if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
						toon.SetType(penguin_WALKER_UPSIDE_DOWN);
					}
					else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
						toon.SetType(penguin_SANTA_WALKER_UPSIDE_DOWN);
					}

					//Rick: shift x-coordinate +3/-3 depending on whether penguin is moving left or right
					toon.m_x += 3 * ((2 * toon.m_directionIndex) - 1);
					toon.SetVelocity(toon.m_directionIndex ? -toon.m_v : toon.m_v, 0);
				}
				break;
			case penguin_FLOATER:
			case penguin_FLOATERLARGE:
				if (status != TOON_OK) {
					if (toon.IsBlocked(TOON_UP)) {
						toon.SetType(penguin_FALLER, penguin_FOREWARD);
						toon.SetVelocity(((toon.m_u > 0) * 2 - 1), 3);
					}
					else {
						//Rick 2021: Bounce off left or right							
						toon.SetVelocity(-toon.m_u, -3);
					}
				}
				break;
				//extensions
			case penguin_READER:
			case penguin_READERLARGE:
				//Rick: check if penguin is reading in mid-air (window has been closed, moved or resized)
				//although I find it amusing when a penguin is reading in mid-air and tumbles after it closes the book :D
				if (!toon.IsBlocked(TOON_DOWN)) {
					toon.m_prefd = toon.m_directionIndex;
					toon.SetType(penguin_TUMBLER, penguin_FOREWARD);
					toon.SetVelocity(0, 6);
					toon.m_tumbleStartY = toon.m_y;
				}
				//Rick 2021: 1 in 50 chance of transforming into walker penguin
				else if (Random::get<bool>(0.02)) {
					const int direction = (toon.m_prefd > -1) ? toon.m_prefd : Random::get(0, 1);

					CheckSubType(toon);

					if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
						toon.SetType(penguin_WALKER, direction);
					}
					else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
						toon.SetType(penguin_SANTA_WALKER, direction);
					}
					//Rick 2021: compensate for width and height of reader being 2px less than walker - not necessary with large penguin	
					if (!m_largePenguinsEnabled) {
						toon.SetPosition(toon.m_x - 1, toon.m_y - 2);
					}
					toon.SetVelocity(4 * ((2 * direction) - 1), 0);
					toon.m_prefd = -1;
				}
				break;
			case penguin_BALLOONER:
			case penguin_BALLOONERLARGE:
				//"pop" balloons.
				if (toon.IsMouseOver())
				{
					if (m_soundEnabled) {
						toon.PopSound();
					}
					toon.SetType(penguin_TUMBLER, penguin_FOREWARD);					
					const int balloonerPopNewY = (!m_largePenguinsEnabled) ? toon.m_y + 45 : toon.m_y + 70;
					//const int balloonerPopNewY = (!m_largePenguinsEnabled) ? toon.m_y + 60 : toon.m_y + 90;//Rick 2021 Old offset when popped
					toon.SetPosition(toon.m_x, balloonerPopNewY);
					toon.SetVelocity(0, 6);
					toon.m_tumbleStartY = toon.m_y;//prevents splat
					break;
				}
				if (status != TOON_OK) {
					if (toon.IsBlocked(TOON_DOWN)) {
						//Rick: compensate for height of balllooner bmp being taller							
						if (!m_largePenguinsEnabled) {
							toon.m_y += 30;//small penguins (62 - 32 = 30)
						}
						else {
							toon.m_y += 48;//large penguins (93 - 62 = 48)
						}
						const int direction = (toon.m_prefd > -1) ? toon.m_prefd : Random::get(0, 1);

						CheckSubType(toon);

						if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
							toon.SetType(penguin_WALKER, direction);
						}
						else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
							toon.SetType(penguin_SANTA_WALKER, direction);
						}
						toon.SetVelocity(4 * ((2 * direction) - 1), 0);
						toon.m_prefd = -1;
					}
					else
					{//if ballooner hits side of window, start climbing 
						//Rick 2021 : Don't want to transform into climber if starting and above top of screen
						if (toon.IsStartingUp()) {
							break;
						}

						if (!m_largePenguinsEnabled) {
							if (toon.IsBlocked(TOON_RIGHT))
								toon.m_x -= 2;//Rick 2021: compensate on right side for different width of ballooner bmp - unnecessary for large penguins
						}
						CheckSubType(toon);

						if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
							toon.SetType(penguin_CLIMBER, toon.m_u > 0);
						}
						else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
							toon.SetType(penguin_SANTA_CLIMBER, toon.m_u > 0);
						}
						toon.SetVelocity(0, -4);
					}
				}
				break;
			case penguin_SKATEBOARDER:
			case penguin_SKATEBOARDERLARGE:
				//Rick 2021: Change direction if we hit an obstacle to the right or left
				if (toon.IsBlocked(toon.m_directionIndex))
				{
					/* Blocked!  We can turn around, fly or climb */

					//Rick 2021 : Don't want to transform into skateboarder climber, floater, or super if starting and above top of screen
					//Odds 1/4 Skateboarder Climber, 1/8 Change direction and run, 1/8 Floater, 1/8 Super, 3/8 Stay on the skateboard and turn around	
					const int transition = toon.IsStartingUp() ? 6 : Random::get(1, 8);

					switch (transition) {
					case 1:
					case 2:
						//climb using skateboard
						toon.SetType(penguin_SKATEBOARDER_CLIMBER);
						toon.SetVelocity(0, -6);
						break;
					case 3://change direction and run
						if (toon.IsBlocked(TOON_DOWN))
						{
							//Rick: compensate for small penguins as skateboarder is 30x30 whereas walker is 32x32
							if (!m_largePenguinsEnabled) {
								if (toon.m_directionIndex == TOON_LEFT)
									toon.m_x += 2;
								else if (toon.m_directionIndex == TOON_RIGHT)
									toon.m_x -= 2;

								toon.m_y -= 2;
							}

							CheckSubType(toon);

							if (toon.GetSubType() == ToonSubType::TST_NORMAL_PENGUIN) {
								toon.SetType(penguin_WALKER, !toon.m_directionIndex);
							}
							else if (toon.GetSubType() == ToonSubType::TST_SANTA_PENGUIN) {
								toon.SetType(penguin_SANTA_WALKER, !toon.m_directionIndex);
							}
							toon.SetVelocity(-toon.m_u, 0);
						}
						break;
					case 4://float off								
						//Rick: compensate for small penguins as skateboarder is 30x30 whereas floater is 32x32
						if (!m_largePenguinsEnabled) {
							if (toon.m_directionIndex == TOON_LEFT)
								toon.m_x += 2;
							else if (toon.m_directionIndex == TOON_RIGHT)
								toon.m_x -= 2;

							toon.m_y -= 2;
						}

						toon.SetType(penguin_FLOATER, penguin_FOREWARD);
						toon.SetVelocity(Random::get(0, 4 * -toon.m_u / 7), -3);
						break;
					case 5:
						toon.SetType(penguin_SUPER, !toon.m_directionIndex);
						toon.SetVelocity(Random::get(0, 9 * -toon.m_u / 7), -8);
						break;
					case 6:
					case 7:
					case 8:
						//stay on the skateboard and turn around									
						toon.m_directionIndex = !toon.m_directionIndex;//change direction
						toon.SetVelocity(-toon.m_u, 0);//switch to the oppositve left or right speed
						break;
					}
				}
				//allow for skateboard to glide down off windows
				else if (!toon.IsBlocked(TOON_DOWN)) {
					toon.SetVelocity(7 /*<< change to adjust speed of skateboarder*/ * ((2 * toon.m_directionIndex) - 1), 5);
				}
				//landed using the skateboard, carry on in the direction we were gliding down with
				else {
					toon.SetVelocity(toon.m_u, 0);
				}
				//change skateboarders direction with keyboard
				//left
				if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) {
#ifdef WP_CONSOLE
					ToConsole(L"LEFT or \"A\" key down");
#endif						
					toon.m_directionIndex = TOON_LEFT;
					toon.SetVelocity(-6, 0);
				}
				//right
				if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) {
#ifdef WP_CONSOLE
					ToConsole(L"RIGHT or \"D\" key down");
#endif	
					toon.m_directionIndex = TOON_RIGHT;
					toon.SetVelocity(6, 0);
				}
				break;
			case penguin_SKATEBOARDER_UPSIDE_DOWN:
			case penguin_SKATEBOARDER_UPSIDE_DOWNLARGE:
			{
				//Rick 2021: If obstacle, stay on the skateboard and turn around
				if (toon.IsBlocked(!toon.m_directionIndex)) {
					//Rick 2021: 50/50 between changing direction and climbing down
					if (Random::get<bool>()) {
						toon.m_directionIndex = !toon.m_directionIndex;//change direction
						toon.SetVelocity(-toon.m_u, 0);//switch to the oppositve left or right speed
					}
					else {
						//Rick 2021: Skateboard down
						toon.SetType(penguin_SKATEBOARDER_CLIMBER_DOWN);
						toon.SetVelocity(0, toon.m_directionIndex ? -toon.m_u : toon.m_u);
					}
				}
				else if (!toon.IsBlocked(TOON_UP)) {
					//Rick 2021: Make sure we haven't overshot the window
					//m_directionIndex = 1 when moving left
					//m_directionIndex = 0 when moving right
					for (int max = 1; !toon.IsBlocked(TOON_UP) && max < 10; ++max) {
						if (toon.m_directionIndex)
							++toon.m_x;
						else
							--toon.m_x;
					}
					if (toon.m_directionIndex)
						--toon.m_x;
					else
						++toon.m_x;

					//climb using skateboard
					//toon.m_y -= 20;
					toon.SetType(penguin_SKATEBOARDER_CLIMBER);
					toon.SetVelocity(0, -6);
				}

				//change skateboarders direction with keyboard
				//left
				if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) {
#ifdef WP_CONSOLE
					ToConsole(L"LEFT or \"A\" key down");
#endif						
					toon.m_directionIndex = TOON_RIGHT;
					toon.SetVelocity(-6, 0);
				}
				//right
				if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) {
#ifdef WP_CONSOLE
					ToConsole(L"RIGHT or \"D\" key down");
#endif	
					toon.m_directionIndex = TOON_LEFT;
					toon.SetVelocity(6, 0);
				}
			}
			break;
			case penguin_SKATEBOARDER_CLIMBER:
			case penguin_SKATEBOARDER_CLIMBERLARGE:
				//skateboard across window	
				if (!toon.IsBlocked(toon.m_directionIndex)) {

					//Rick 2021: Make sure we haven't overshot the window
					for (int max = 1; !toon.IsBlocked(toon.m_directionIndex) && max < 10; ++max) {
						++toon.m_y;
					}
					--toon.m_y;

					toon.SetType(penguin_SKATEBOARDER);
					toon.SetVelocity(7 /*<< change to adjust speed of skateboarder*/ * ((2 * toon.m_directionIndex) - 1), 0);
					toon.SetPosition(toon.m_x + (2 * toon.m_directionIndex), toon.m_y);
					toon.m_prefd = toon.m_directionIndex;
				}
				else if (toon.IsBlocked(TOON_UP)) {
					//Rick 2021: 33/33/33 between faller, skateboard back down, skateboard upside down
					//Rick 2021: Don't want to skateboard across top of screen
					const int nextType = Random::get(1, toon.m_y != CMainWnd::winPenguinsWindowRect.top ? 3 : 2);

					switch (nextType)
					{
					case 1:
						//Rick 2021: Faller					
						toon.SetVelocity(2 - toon.m_directionIndex * 4, 3);//Rick: m_u is +2/-2
						toon.SetType(penguin_FALLER, penguin_FOREWARD);
						break;
					case 2:
						//Rick 2021: Skateboard back down
						toon.SetType(penguin_SKATEBOARDER_CLIMBER_DOWN, !toon.m_directionIndex);
						toon.SetVelocity(0, -toon.m_v);//reverse direction
						break;
					case 3:
						//Rick 2021: Skateboard upside down
						toon.SetType(penguin_SKATEBOARDER_UPSIDE_DOWN);
						toon.SetVelocity(toon.m_directionIndex ? toon.m_v : -toon.m_v, 0);
						break;
					}
				}
				//Rick 2021: change skateboarder direction with keyboard
				//down
				if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) {
#ifdef WP_CONSOLE
					ToConsole(L"DOWN or \"S\" key down");
#endif						
					//Rick 2021: Skateboard back down
					toon.SetType(penguin_SKATEBOARDER_CLIMBER_DOWN, !toon.m_directionIndex);
					toon.SetVelocity(0, -toon.m_v);//reverse direction
				}
				break;
			case penguin_SKATEBOARDER_CLIMBER_DOWN:
			case penguin_SKATEBOARDER_CLIMBER_DOWNLARGE:
				//Rick 2021: Skateboard upside down!
				if (!toon.IsBlocked(!toon.m_directionIndex)/*&& !toon.IsBlocked(TOON_UP)*/) {
					//toon.SetVelocity(7 /*<< change to adjust speed of skateboarder*/ * ((2 * !toon.m_directionIndex) - 1), 0);
					// toon.m_y -= 3;
					//toon.m_x += 8 - toon.m_directionIndex * 16;// m_x +/- 9
					//toon.SetPosition(toon.m_x + (2 * toon.m_directionIndex), toon.m_y);

					//Rick 2021: Make sure we haven't overshot the window
					for (int max = 1; !toon.IsBlocked(!toon.m_directionIndex) && max < 10; ++max) {
						--toon.m_y;
					}
					++toon.m_y;

					toon.SetType(penguin_SKATEBOARDER_UPSIDE_DOWN);

					//Rick: shift x-coordinate +3/-3 depending on whether penguin is moving left or right
					toon.m_x += 3 * ((2 * toon.m_directionIndex) - 1);
					toon.SetVelocity(toon.m_directionIndex ? -toon.m_v : toon.m_v, 0);
					break;
				}
				//across window	
				else if (toon.IsBlocked(TOON_DOWN)) {
					toon.SetVelocity(7 /*<< change to adjust speed of skateboarder*/ * ((2 * toon.m_directionIndex) - 1), 0);
					toon.SetPosition(toon.m_x + (2 * toon.m_directionIndex), toon.m_y);
					toon.SetType(penguin_SKATEBOARDER);
					break;
				}
				//Rick 2021: change skateboarder direction with keyboard
				//up
				if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) {
#ifdef WP_CONSOLE
					ToConsole(L"UP or \"W\" key down");
#endif						
					//Rick 2021: Skateboard back down
					toon.SetType(penguin_SKATEBOARDER_CLIMBER, !toon.m_directionIndex);
					toon.SetVelocity(0, -toon.m_v);//reverse direction
				}

				break;
			case penguin_SUPER:
			case penguin_SUPERLARGE:
				if (status != TOON_OK) {
					if (toon.IsBlocked(TOON_UP)) {
						toon.SetType(penguin_FALLER, penguin_FOREWARD);
						toon.SetVelocity(((toon.m_u > 0) * 2 - 1), 3);
					}
					else {
						//Rick 2021: Bounce off side of window							
						toon.m_u = -toon.m_u;
						toon.m_directionIndex = !toon.m_directionIndex;
					}
				}
				break;
			default:
				break;
			}
		}

		CDC* dc = GetDC();		

		CDC bgBitmapDC, activeBmpDC, tmpDC;		

		bgBitmapDC.CreateCompatibleDC(dc);
		activeBmpDC.CreateCompatibleDC(dc);
		tmpDC.CreateCompatibleDC(dc);	

		CBitmap* oldBgBmp = bgBitmapDC.SelectObject(&m_bgBitmap);
		CBitmap* oldActiveBmp = activeBmpDC.SelectObject(&m_activeBmp);

		for (const CToon& toon : m_toonVec) {
			toon.PaintBackground(&bgBitmapDC, &activeBmpDC); //transparent bg onto blank activeBmpDC
		}
		
		for (const CToon& toon : m_toonVec) {
			toon.PaintSprite(&activeBmpDC, &tmpDC);//sprite onto activeBmpDC	
		}

		for (CToon& toon : m_toonVec) {
			toon.PaintWindowOverlay(dc, &activeBmpDC); //activeBmpDC onto WinPenguins DC

			//Rick 2021: Reset penguins that are at end of cycle (exploded or splattered)
			if (!toon.IsActive() &&
				toon.IsEndOfCycleBitmap()
				&& m_numpenguins > 0) {
				toon = CToon::CreateStarterPenguin(m_largePenguinsEnabled);
			}
		}

		bgBitmapDC.SelectObject(oldBgBmp);
		activeBmpDC.SelectObject(oldActiveBmp);
		ReleaseDC(dc);

		//Rick 2021: Number of penguins reduced, erase them from back of vector
		//m_toonVec.size() always at least 1 unless app is shutting down
		while (!m_toonVec.back().m_active) {
			m_toonVec.pop_back(); 

			//Rick 2021: 0 penguins left. Close app
			if (m_toonVec.empty()) {
				KillTimer(TimerEvent::MainLoop);
				PostMessage(WM_CLOSE); 
				return;
			}
		}
	}

	break;

	default:
#ifdef WP_CONSOLE
		ToConsole(L"Unknown timer event: {}", nIDEvent);
#endif
		break;
	}

	CWnd::OnTimer(nIDEvent);
}


void CMainWnd::OnAbout() 
{
	CAboutDlg dlg;
	m_activeDlg = &dlg;
	dlg.DoModal();	
	m_activeDlg = nullptr;
}

// <TL> Thought some people might find this nice...

void CMainWnd::OnScreenCapture()
{
	static const wchar_t szFilter[] = L"BMP (*.bmp)|*.bmp|PNG (*.png)|*.png|JPEG (*.jpg)|*.jpg||";
	CFileDialog dlg(FALSE, L"bmp", L"WinPenguins", OFN_OVERWRITEPROMPT | OFN_EXTENSIONDIFFERENT, szFilter, this);
	m_activeDlg = &dlg;

	if (dlg.DoModal() == IDOK) {
		Sleep(500);//Don't want the save file dialog in the screenshot

		const CString FileFormat = dlg.GetFileExt();
		const CString FilePath = dlg.GetPathName();
		
		if (FileFormat == L"bmp") {		
			GetScreenshot(FilePath, L"image/bmp");
		}
		else if (FileFormat == L"png") {
			GetScreenshot(FilePath, L"image/png");
		}
		else if (FileFormat == L"jpg") {
			GetScreenshot(FilePath, L"image/jpeg");
		}
	}

	m_activeDlg = nullptr;
}

void CMainWnd::OnExit() 
{
	// Remove the systray icon
	NOTIFYICONDATA ni { .cbSize = sizeof(NOTIFYICONDATA), .hWnd = m_hWnd, .uID = 1 };

	BOOL sniResult = Shell_NotifyIcon(NIM_DELETE, &ni);
	if (!sniResult) {
		const wchar_t sniErrorStr[] = L"Shell_NotifyIcon NIM_DELETE Error";
#ifdef WP_CONSOLE
		ToConsole(sniErrorStr);
#endif //WP_CONSOLE
		MessageBoxW(sniErrorStr, L"Internal Error", MB_ICONERROR);
	}

	PostMessage(WM_CLOSE);
}

void CMainWnd::OnOptions() 
{
	CWinpenguinsDlg optiondlg {m_numpenguins, maxMoveDelay - m_moveDelay, m_splatDist, m_santaPercent, m_blendLevel, 
		m_largePenguinsEnabled, m_soundEnabled};

	m_activeDlg = &optiondlg;

	if (optiondlg.DoModal() == IDOK) {
		ApplyOptions(optiondlg);

		theApp.WriteProfileInt(optionsRegStr, penguinCountStr, m_numpenguins);
		theApp.WriteProfileInt(optionsRegStr, moveDelayStr, m_moveDelay);
		theApp.WriteProfileInt(optionsRegStr, splatDistanceStr, m_splatDist);
		theApp.WriteProfileInt(optionsRegStr, santaPercentStr, m_santaPercent);
		theApp.WriteProfileInt(optionsRegStr, blendLevelStr, m_blendLevel);		
		theApp.WriteProfileInt(optionsRegStr, largePenguinsEnabledStr, m_largePenguinsEnabled);
		theApp.WriteProfileInt(optionsRegStr, soundEnabledStr, m_soundEnabled);
	}
	else {
		/* restore original values  */
		m_numpenguins = theApp.GetProfileInt(optionsRegStr, penguinCountStr, defaultNumPenguins);
		m_moveDelay = theApp.GetProfileInt(optionsRegStr, moveDelayStr, defaultMoveDelay);
		m_splatDist = theApp.GetProfileInt(optionsRegStr, splatDistanceStr, winPenguinsWindowRect.bottom);
		m_santaPercent = theApp.GetProfileInt(optionsRegStr, santaPercentStr, defaultSantaPercent);
		m_blendLevel = theApp.GetProfileInt(optionsRegStr, blendLevelStr, defaultBlendLevel);		
		m_largePenguinsEnabled = theApp.GetProfileInt(optionsRegStr, largePenguinsEnabledStr, winPenguinsWindowRect.right >= 1920 && winPenguinsWindowRect.bottom >= 1080);
		m_soundEnabled = theApp.GetProfileInt(optionsRegStr, soundEnabledStr, defaultSoundEnabled);

		optiondlg.m_penguincount = m_numpenguins;
		optiondlg.m_delay = maxMoveDelay - m_moveDelay;
		optiondlg.m_splat = m_splatDist;
		optiondlg.m_santa = m_santaPercent;
		optiondlg.m_alpha = m_blendLevel;		
		optiondlg.m_largepenguins = m_largePenguinsEnabled;
		optiondlg.m_soundenabled = m_soundEnabled;

		ApplyOptions(optiondlg);
	}

	m_activeDlg = nullptr;
}

void CMainWnd::SetToonCountTo(int count)
{	
	for (INT_PTR i = count - m_toonVec.size(); i > 0; --i) {
		m_toonVec.emplace_back(CToon::CreateStarterPenguin(m_largePenguinsEnabled));
	}

	//display penguin vanish animation on app close
	if (count < m_toonVec.size()) {
		for (int i = count; i < m_toonVec.size(); ++i) {
			m_toonVec[i].DeleteAni();
		}
	}	

	//if (m_toonVec.empty()) {
	//	PostMessage(WM_CLOSE);
	//}
}

//Rick 2021: Set window to have transparent white background with specified alpha value
BOOL CMainWnd::SetWindowAlphaValue(unsigned char alphaValue)
{
	const bool alpha_enabled = alphaValue < 255;
	const DWORD flag = alpha_enabled ? LWA_ALPHA | LWA_COLORKEY : LWA_COLORKEY;
	return SetLayeredWindowAttributes(RGB(255, 255, 255), alphaValue, flag);
}

void CMainWnd::ApplyOptions(const CWinpenguinsDlg & dlg)
{
	m_numpenguins = dlg.m_penguincount;
	m_moveDelay = maxMoveDelay - dlg.m_delay;
	m_splatDist = dlg.m_splat;
	m_santaPercent = dlg.m_santa;
	m_blendLevel = dlg.m_alpha;	
	m_largePenguinsEnabled = dlg.m_largepenguins;
	m_soundEnabled = dlg.m_soundenabled;	

	// Movement timer 
	KillTimer(TimerEvent::MainLoop);
	SetTimer(TimerEvent::MainLoop, m_moveDelay, nullptr);

	if (m_numpenguins > m_toonVec.capacity())
		m_toonVec.reserve(m_numpenguins);

	SetToonCountTo(m_numpenguins);
	
	SetWindowAlphaValue(m_blendLevel);

	// Rick 2021: Handle Penguin size change	
	for (CToon& toon : m_toonVec) {
		if (toon.m_isLarge != m_largePenguinsEnabled) {
			toon.ChangeSize();
		}
	}
}

void CMainWnd::CheckForWindowChanges()
{
	//Rick 2021: Only update window region if we have a window changed event from the hook
	if (windowChanged) {
		//Rick 2021: if window region is active, free the resource for next iteration
		if (combinedWindowRegion.GetSafeHandle()) {
			combinedWindowRegion.DeleteObject();
		}

		//Rick 2021: Find all windows that are child windows of the desktop and add to region 
		FindVisibleWindows();
		windowChanged = false;

		//Rick 2021: For testing draw a frame around the found window regions (where penguins cant access)
#ifdef _DEBUG
#ifdef WP_FRAME_WINDOW_REGIONS
		if (combinedWindowRegion.GetSafeHandle())
		{
			CDC* frame_dc = GetDC();
			CBrush redBrush;
			redBrush.CreateSolidBrush(RGB(255, 0, 0));  //Red
			BOOL FrameRgnRes = frame_dc->FrameRgn(&combinedWindowRegion, &redBrush, 2, 2);
			if (!FrameRgnRes) {
				ToConsole(L"FrameRgn Error");
			}
			if (redBrush.GetSafeHandle()) {
				redBrush.DeleteObject();
			}
			if (frame_dc) {
				ReleaseDC(frame_dc);
			}
		}
#endif // WP_FRAME_WINDOW_REGIONS
#endif // _DEBUG 
	}
}

void CMainWnd::InitializeScreenBitmaps()
{
	CDC *dc = GetDC();

	const long width = CMainWnd::winPenguinsWindowRect.right;
	const long height = CMainWnd::winPenguinsWindowRect.bottom;

	m_bgBitmap.CreateCompatibleBitmap(dc, width, height);
	
	m_activeBmp.CreateCompatibleBitmap(dc, width, height);	

	CDC bmpDc;
	bmpDc.CreateCompatibleDC(dc);
	CBitmap *oldBmp = bmpDc.SelectObject(&m_bgBitmap);
	
	bmpDc.BitBlt(0, 0, width, height, dc, 0, 0, SRCCOPY); //copy the transparent dc to m_bgBitmap

	bmpDc.SelectObject(oldBmp);	
	
	ReleaseDC(dc);
}

bool CMainWnd::CheckSubType(CToon &pToon)
{
	if (pToon.GetSubType() == ToonSubType::TST_UNSPECIFIED) {
		if (m_santaPercent == 0)
			pToon.SetSubType(ToonSubType::TST_NORMAL_PENGUIN);
		else if (m_santaPercent == 100)
			pToon.SetSubType(ToonSubType::TST_SANTA_PENGUIN);
		else {
			const double santaProbability = m_santaPercent / 100.00;
			if (Random::get<bool>(santaProbability)) {
				pToon.SetSubType(ToonSubType::TST_SANTA_PENGUIN);
			}
			else {
				pToon.SetSubType(ToonSubType::TST_NORMAL_PENGUIN);
			}
		}

		return true;
	}
	
  return false;
}
