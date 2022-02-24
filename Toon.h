/**
 *  Toon.h
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

#if !defined(AFX_TOON_H__7339F5A5_53B0_4448_8C93_420E8E3B5FEC__INCLUDED_)
#define AFX_TOON_H__7339F5A5_53B0_4448_8C93_420E8E3B5FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Toon.h : header file
//

// return values from AdvanceToon
#define TOON_OK 0
#define TOON_PARTIALMOVE 1
#define TOON_BLOCKED -1

#define TOON_LEFT 0
#define TOON_RIGHT 1
#define TOON_UP 2
#define TOON_DOWN 3
#define TOON_HERE -1


enum class ToonSubType
{
	TST_UNSPECIFIED,
	TST_NORMAL_PENGUIN,
	TST_SANTA_PENGUIN
};

/* Bitmap colour that will be transparent */
#define BG_COLOUR RGB(0,0,0)
 

/////////////////////////////////////////////////////////////////////////////
// CToon window

class CToon 
{

// Construction
public:	
	CToon() = delete;
	CToon(int x, int y, int u, int v, int bmpIndex, int directionIndex, bool isLarge);
	
	// Attributes
public:
	int m_x;//x-axis position of penguin
	int m_y;//y-axis position of penguin
	int m_u;//left-right direction. simulates wind for fallers. (negative numbers send penguins left, positive numbers send penguins right)
	int m_v;//velocity - up/down speed (negative numbers up, positive numbers down)
	int m_prefd;  // prefered direction -- -1 = none
	int m_tumbleStartY;

	int m_bmpIndex;//type of penguin. e.g floater, walker
	int m_frameIndex;
	int m_directionIndex;

	bool m_active;
	bool m_startingUp; //Rick 2021: true unless penguin has fully entered screen
	bool m_isLarge;//Rick 2021

	bool m_respawn;//Rick 2022

	ToonSubType m_tstSubType;	

	//Rick: Padding so we don't show remnants of bitmaps on screen
	//Width padding: 20 seems enough for both Small and Large Penguins
	//Small penguin height padding: 62 (height of Small Ballooner bmp)
	//Large penguin height padding: 93 (height of Large Ballooner bmp)	
	static constexpr int paintPaddingX = 20;
	static constexpr int smallPaintPaddingY = 62;
	static constexpr int largePaintPaddingY = 93;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToon)
	//}}AFX_VIRTUAL

// Implementation
public:
	bool IsBlocked(int direction);
	bool IsMouseOver();//Rick
	bool IsStartingUp(); //Rick 2022
	bool CheckFullyEnteredScreen();//Rick 2021
	int AdvanceToon(/*bool force,*/);
	void SetVelocity(int u, int v);	
	void SetPosition(int x, int y);
	void SetType(int type);
	void SetType(int type, int direction);

	void ChangeSize();
	void MakeSmall();
	void MakeLarge();
	
	void DeleteAni();
	void ExplodeAni(bool soundOn);
	void SplatAni(bool soundOn);//Rick
	
	void PopSound();//Rick

	bool IsActive();//Rick 2022	
	bool IsDeleteAni();//Rick 2022
	bool IsExploding();//Rick 2022
	bool IsSplat();//Rick 2022
	bool IsEndOfCycleBitmap();//Rick 2022	

	bool IsRespawnable();//Rick 2022
	void SetRespawnable(bool respawn);//Rick 2022

	void GetRect(RECT &rt);
	void GetRect(RECT &rt, int newx, int newy);

	void PaintSprite(CDC *activeDC, CDC *tmpDc) const;
	void PaintBackground(CDC *bgBitmapDC, CDC *activeDC) const;
	void PaintWindowOverlay(CDC *dc,  CDC *activeBmpDC) const;	
		
	void SetSubType(ToonSubType tst);
	ToonSubType GetSubType();

	static CToon CreateStarterPenguin(bool largePenguin);
private:
	void AdvanceFrame();

	

	// Generated message map functions
protected:


	//{{AFX_MSG(CToon)
	//}}AFX_MSG
//	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOON_H__7339F5A5_53B0_4448_8C93_420E8E3B5FEC__INCLUDED_)
