/**
 *  Toon.cpp
 *
 *  Copyright (C) 2000,2001  Michael Vines
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
#include "Toon.h"
#include "MainWnd.h"
#include "Def.h"
#include <mmsystem.h>

ToonData penguin_data[penguin_BITMAP_COUNT] = {
   { IDB_WALKER,							IDB_WALKERMSK,							{}, {}, 8, 2, 32, 32, true },
   { IDB_WALKER_UPSIDE_DOWN,				IDB_WALKER_UPSIDE_DOWNMSK,				{}, {}, 8, 2, 32, 32, true },
   { IDB_FALLER,							IDB_FALLERMSK,							{}, {}, 8, 1, 32, 32, true },
   { IDB_TUMBLER,							IDB_TUMBLERMSK,							{}, {}, 8, 1, 32, 32, true },
   { IDB_FLOATER,							IDB_FLOATERMSK,							{}, {}, 8, 1, 32, 32, true },
   { IDB_CLIMBER,							IDB_CLIMBERMSK,							{}, {}, 8, 2, 32, 32, true },
   { IDB_CLIMBER_DOWN,						IDB_CLIMBER_DOWNMSK,					{}, {}, 8, 2, 32, 32, true },
   { IDB_BOMBER,							IDB_BOMBERMSK,							{}, {}, 17, 1, 32, 32, false },
   { IDB_EXPLOSION,							IDB_EXPLOSIONMSK,						{}, {}, 4, 1, 64, 64, false },
   { IDB_SANTA_WALKER,						IDB_SANTA_WALKERMSK,					{}, {}, 8, 2, 32, 32, true },
   { IDB_SANTA_WALKER_UPSIDE_DOWN,			IDB_SANTA_WALKER_UPSIDE_DOWNMSK,		{}, {}, 8, 2, 32, 32, true },
   { IDB_SANTA_CLIMBER,						IDB_SANTA_CLIMBERMSK,					{}, {}, 8, 2, 32, 32, true },
   { IDB_SANTA_CLIMBER_DOWN,				IDB_SANTA_CLIMBER_DOWNMSK,				{}, {}, 8, 2, 32, 32, true },
   { IDB_READER,							IDB_READERMSK,							{}, {}, 12, 1, 30, 30, true },
   { IDB_BALLOONER,							IDB_BALLOONERMSK,						{}, {},  8, 1, 30, 62, true },//-2 from x-axis brings it to 32, add 30 to y-axis brings it to 32 (equal with faller/walker)
   { IDB_SKATEBOARDER,						IDB_SKATEBOARDERMSK,					{}, {},  1, 2, 30, 30, true },//-2 from x-axis, add 2 to y axis (equal with faller/walker)
   { IDB_SKATEBOARDER_UPSIDE_DOWN,			IDB_SKATEBOARDER_UPSIDE_DOWNMSK,		{}, {},  1, 2, 30, 30, true },//-2 from x-axis, add 2 to y axis (equal with faller/walker)  
   { IDB_SKATEBOARDER_CLIMBER,				IDB_SKATEBOARDER_CLIMBERMSK,			{}, {},	 1, 2, 30, 30, true },
   { IDB_SKATEBOARDER_CLIMBER_DOWN,			IDB_SKATEBOARDER_CLIMBER_DOWNMSK,		{}, {},	 1, 2, 30, 30, true },
   { IDB_SPLAT,								IDB_SPLATMSK,							{}, {}, 12, 1, 32, 32, false },
   { IDB_EXIT,								IDB_EXITMSK,							{}, {},  9, 2, 32, 32, false},
   { IDB_SUPER,								IDB_SUPERMSK,							{}, {},  8, 2, 30, 30, true},
   //Rick 2021: Large Penguins
   { IDB_WALKERLARGE,						IDB_WALKERLARGEMSK,						{}, {},	8, 2, 45, 45, true },
   { IDB_WALKER_UPSIDE_DOWNLARGE,			IDB_WALKER_UPSIDE_DOWNLARGEMSK,			{}, {},	8, 2, 45, 45, true },
   { IDB_FALLERLARGE,						IDB_FALLERLARGEMSK,						{}, {}, 8, 1, 45, 45, true },
   { IDB_TUMBLERLARGE,						IDB_TUMBLERLARGEMSK,					{}, {}, 8, 1, 45, 45, true },
   { IDB_FLOATERLARGE,						IDB_FLOATERLARGEMSK,					{}, {}, 8, 1, 45, 45, true },
   { IDB_CLIMBERLARGE,						IDB_CLIMBERLARGEMSK,					{}, {}, 8, 2, 45, 45, true },
   { IDB_CLIMBER_DOWNLARGE,					IDB_CLIMBER_DOWNLARGEMSK,				{}, {}, 8, 2, 45, 45, true },
   { IDB_BOMBERLARGE,						IDB_BOMBERLARGEMSK,						{}, {}, 16, 1, 48, 48, false },
   { IDB_EXPLOSIONLARGE,					IDB_EXPLOSIONLARGEMSK,					{}, {}, 4, 1, 96, 96, false },
   { IDB_SANTA_WALKERLARGE,					IDB_SANTA_WALKERLARGEMSK,				{}, {}, 8, 2, 45, 45, true },
   { IDB_SANTA_WALKER_UPSIDE_DOWNLARGE,		IDB_SANTA_WALKER_UPSIDE_DOWNLARGEMSK,	{}, {}, 8, 2, 45, 45, true },
   { IDB_SANTA_CLIMBERLARGE,				IDB_SANTA_CLIMBERLARGEMSK,				{}, {}, 8, 2, 45, 45, true },
   { IDB_SANTA_CLIMBER_DOWNLARGE,			IDB_SANTA_CLIMBER_DOWNLARGEMSK,			{}, {}, 8, 2, 45, 45, true },
   { IDB_READERLARGE,						IDB_READERLARGEMSK,						{}, {}, 12, 1, 45, 45, true },
   { IDB_BALLOONERLARGE,					IDB_BALLOONERLARGEMSK,					{}, {},  8, 1, 45, 93, true },
   { IDB_SKATEBOARDERLARGE,					IDB_SKATEBOARDERLARGEMSK,				{}, {},  1, 2, 45, 45, true },
   { IDB_SKATEBOARDER_UPSIDE_DOWNLARGE,		IDB_SKATEBOARDER_UPSIDE_DOWNLARGEMSK,	{}, {},  1, 2, 45, 45, true },
   { IDB_SKATEBOARDER_CLIMBERLARGE,			IDB_SKATEBOARDER_CLIMBERLARGEMSK,		{}, {},	 1, 2, 45, 45, true },
   { IDB_SKATEBOARDER_CLIMBER_DOWNLARGE,	IDB_SKATEBOARDER_CLIMBER_DOWNLARGEMSK,	{}, {},	 1, 2, 45, 45, true },
   { IDB_SPLATLARGE,						IDB_SPLATLARGEMSK,						{}, {}, 12, 1, 48, 48, false },
   { IDB_EXITLARGE,							IDB_EXITLARGEMSK,						{}, {},  9, 2, 48, 48, false },
   { IDB_SUPERLARGE,						IDB_SUPERLARGEMSK,						{}, {},  8, 2, 45, 45, true },
};


/////////////////////////////////////////////////////////////////////////////
// CToon


CToon::CToon(int x, int y, int u, int v, int bmpIndex, int directionIndex, bool isLarge) :
	m_x {x}, m_y {y}, m_u {u}, m_v {v}, m_prefd{-1}, m_tumbleStartY{0}, m_bmpIndex{bmpIndex}, m_frameIndex{0}, m_directionIndex{directionIndex},
	m_active{true}, m_startingUp{true}, m_isLarge{isLarge}, m_tstSubType{ToonSubType::TST_UNSPECIFIED}
{
}


void CToon::DeleteAni()
{
	//if (m_bmpIndex == penguin_EXIT || m_bmpIndex == penguin_EXITLARGE)
	//	return;

	//Rick 2021: Have walker penguins walking off into the distance (sprite exit.bmp). Have other penguins leave with the bomber animation
	if (m_bmpIndex == penguin_WALKER)
	{
		m_frameIndex = 0;
		m_bmpIndex = penguin_EXIT;
	}
	else if (m_bmpIndex == penguin_WALKERLARGE)
	{
		m_frameIndex = 0;
		m_bmpIndex = penguin_EXITLARGE;

		//Rick 2021: Offset as Exit is 48x48 rather than 45x45
		m_x -= 3;
		m_y -= 3;		
	}
	else if (m_bmpIndex != penguin_BOMBER && m_bmpIndex != penguin_BOMBERLARGE 
			&& m_bmpIndex != penguin_EXPLOSION && m_bmpIndex != penguin_EXPLOSIONLARGE
			&& m_bmpIndex != penguin_SPLAT && m_bmpIndex != penguin_SPLATLARGE
			&& m_bmpIndex != penguin_EXIT && m_bmpIndex != penguin_EXITLARGE)
	{
		m_directionIndex = 0;
		m_frameIndex = 0;
		if (!m_isLarge) {
			m_bmpIndex = penguin_BOMBER;
			m_x -= 2;
			m_y -= 2; //Rick: prevent reading and skateboarding penguins exploding as they are 30x30 instead of 32x32 for penguin_BOMBER, penguin_WALKER and others
		}
		else {
			m_bmpIndex = penguin_BOMBERLARGE;
			m_x -= 3;
			m_y -= 3;//Rick 2021: compensate as bomber large is 48x48 and regular large are 45x45			
		}
	}
}

void CToon::ExplodeAni(bool soundOn)
{
	if (penguin_EXPLOSION == m_bmpIndex || penguin_EXPLOSIONLARGE == m_bmpIndex
		|| penguin_SPLAT == m_bmpIndex || penguin_SPLATLARGE == m_bmpIndex) {
		return;
	}

	m_directionIndex = 0;
	m_frameIndex = 0;
	SetType(penguin_EXPLOSION);

	//Rick 2021: offset small
	if (!m_isLarge) {
		m_x -= 16;
		m_y -= 16;
	}
	//Rick 2021: offset large 
	else {
		m_x -= 32;
		m_y -= 32;
	}

	if (soundOn) {
		PlaySound( MAKEINTRESOURCE( IDR_SOUND_BOOM ), theApp.m_hInstance, 
			SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
	}
}

void CToon::SplatAni(bool soundOn)
{
	if (penguin_SPLAT == m_bmpIndex || penguin_SPLATLARGE == m_bmpIndex) {
		return;
	}

	m_frameIndex = 0;
	SetType(penguin_SPLAT);

	//Rick 2021 - offset as large splat bmp is 48x48 rather than 45x45
	if (m_isLarge) {		
		m_x -= 3;
		m_y -= 3;
	}

	if (soundOn) {
		PlaySound(MAKEINTRESOURCE(IDR_SOUND_BOOM), theApp.m_hInstance,
			SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
	}
}

void CToon::PopSound()
{
	PlaySound(MAKEINTRESOURCE( IDR_SOUND_POP ), theApp.m_hInstance,
			SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
}

bool CToon::IsActive()
{
	return m_active;
}

bool CToon::IsExploding()
{
	return (m_bmpIndex == penguin_EXPLOSIONLARGE || m_bmpIndex == penguin_EXPLOSION);
}

//true if penguin is at end of cycle (exploded or splattered)
bool CToon::IsEndOfCycleBitmap()
{
	return (m_bmpIndex == penguin_EXPLOSIONLARGE || m_bmpIndex == penguin_EXPLOSION ||  m_bmpIndex == penguin_SPLATLARGE || m_bmpIndex == penguin_SPLAT);
}

void CToon::GetRect(RECT &rt)
{
	rt.left = m_x;
	rt.top = m_y;
	rt.right = rt.left + penguin_data[m_bmpIndex].width;
	rt.bottom = rt.top + penguin_data[m_bmpIndex].height;
}


void CToon::GetRect(RECT &rt, int newx, int newy)
{
	rt.left = newx;
	rt.top = newy;
	rt.right = rt.left + penguin_data[m_bmpIndex].width;
	rt.bottom = rt.top + penguin_data[m_bmpIndex].height;
}


void CToon::AdvanceFrame()
{
	++m_frameIndex;

	//check for explosion or splat and set inactive if so
	if (m_frameIndex >= penguin_data[m_bmpIndex].nframes) {
		if (penguin_data[m_bmpIndex].cycle) {
			m_frameIndex = 0;
		} else {
			m_active = false;
		}
	}
}


void CToon::PaintSprite(CDC* activeDC, CDC* tmpDc) const
{
	CBitmap* oldBmp = tmpDc->SelectObject(&penguin_data[m_bmpIndex].bmp);

	activeDC->TransparentBlt(m_x, m_y,
		penguin_data[m_bmpIndex].width, penguin_data[m_bmpIndex].height,
		tmpDc, m_frameIndex * penguin_data[m_bmpIndex].width, m_directionIndex * penguin_data[m_bmpIndex].height,
		penguin_data[m_bmpIndex].width, penguin_data[m_bmpIndex].height, BG_COLOUR);

	tmpDc->SelectObject(oldBmp);
}


void CToon::PaintBackground(CDC *bgBitmapDC, CDC *activeDC) const
{
	const int x = m_x - paintPaddingX;
	if (!m_isLarge) {		
		const int ySmall = m_y - smallPaintPaddingY;
		activeDC->BitBlt(x, ySmall,
			penguin_data[m_bmpIndex].width + paintPaddingX * 2,
			penguin_data[m_bmpIndex].height + smallPaintPaddingY * 2,
			bgBitmapDC, x, ySmall, SRCCOPY); // xSrc/ySrc 0, 0 works
	}
	else {
		const int yLarge = m_y - largePaintPaddingY;
		activeDC->BitBlt(x, yLarge,
			penguin_data[m_bmpIndex].width + paintPaddingX * 2,
			penguin_data[m_bmpIndex].height + largePaintPaddingY * 2,
			bgBitmapDC, x, yLarge, SRCCOPY);// xSrc/ySrc 0, 0 works
	}
}


void CToon::PaintWindowOverlay(CDC *dc,  CDC *activeBmpDC) const
{
	const int x = m_x - paintPaddingX;
	if (!m_isLarge) {
		const int ySmall = m_y - smallPaintPaddingY;
		dc->BitBlt(x, ySmall,
			penguin_data[m_bmpIndex].width + paintPaddingX * 2,
			penguin_data[m_bmpIndex].height + smallPaintPaddingY * 2,
			activeBmpDC, x, ySmall, SRCCOPY);
	}
	else {
		const int yLarge = m_y - largePaintPaddingY;
		dc->BitBlt(x, yLarge,
			penguin_data[m_bmpIndex].width + paintPaddingX * 2,
			penguin_data[m_bmpIndex].height + largePaintPaddingY * 2,
			activeBmpDC, x, yLarge, SRCCOPY);
	}
}
	 
void CToon::SetType(int type)
{
	//Rick 2021: Large penguin bitmap offsets are +22 
	if (m_isLarge)
		type += 22;

	m_bmpIndex = type;
}

void CToon::SetType(int type, int direction)
{
	//Rick 2021: Large penguin bitmap offsets are +22 
	if (m_isLarge)
		type += 22;

	m_bmpIndex = type;
	m_directionIndex = direction;
}


void CToon::SetPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CToon::SetVelocity(int u, int v)
{
	m_u = u;
	m_v = v;
}

//Rick 2021: Handle Penguin size change (large to small or small to large)
void CToon::ChangeSize()
{
	if (!m_isLarge)
		MakeLarge();
	else
		MakeSmall();
}

//Rick 2021: Convert Large Penguin to Small Penguin
void CToon::MakeSmall()
{
	m_bmpIndex -= 22; //small penguin bmps are -22
	m_y += 13; //compensate for being 32x32 (some are 30x30) instead of 45x45
	m_x += 13;	

	m_isLarge = false;
}

//Rick 2021: Convert Small Penguin to Large Penguin
void CToon::MakeLarge()
{
	m_bmpIndex += 22; //large penguin bmps are +22
	
	//Rick: compensate for being 45x45 instead of 32x32 but don't want to move off screen
	const int potential_y = m_y - 15;
	const int potential_x = m_x - 15;
	if (potential_y > 0 && potential_x > 0) 
	{ 
		m_y = potential_y;
		m_x = potential_x;
	}

	m_isLarge = true;
}

int CToon::AdvanceToon(/*bool force,*/)
{
	int status = TOON_OK;

	int newx = m_x + m_u;
	int newy = m_y + m_v;	 

	if (!IsExploding()) {
		//Rick: constrain penguins on x-axis
		if (newx < 0) {
			newx = 0;
			status = TOON_PARTIALMOVE;
		}
		else if (newx + penguin_data[m_bmpIndex].width > CMainWnd::winPenguinsWindowRect.right) {
			newx = CMainWnd::winPenguinsWindowRect.right - penguin_data[m_bmpIndex].width;
			status = TOON_PARTIALMOVE;
		}

		//Rick: constrain penguins on y-axis
		//Rick 2021: Added !IsStartingUp so we can have penguins enter from top of screen when spawned	
		if (!IsStartingUp() && newy < 0) {
			newy = 0;
			status = TOON_PARTIALMOVE;
		}
		else if (newy + penguin_data[m_bmpIndex].height > CMainWnd::winPenguinsWindowRect.bottom) {
			//if (!force) {
			newy = CMainWnd::winPenguinsWindowRect.bottom - penguin_data[m_bmpIndex].height;
			status = TOON_PARTIALMOVE;
			//} 
		}
	}

	RECT rt;
	int move_ahead = 1;

	//Rick: Get rect for new location of penguin
	GetRect(rt, newx + CMainWnd::winPenguinsWindowRect.left, newy + CMainWnd::winPenguinsWindowRect.top);

	//Rick 2021: Check if penguin is in window region
	if (CMainWnd::combinedWindowRegion.GetSafeHandle() && CMainWnd::combinedWindowRegion.RectInRegion(&rt) /*&& !force*/) {
		int tryx, tryy, step = 1;

		move_ahead = 0;
		status = TOON_BLOCKED;

		if (abs(m_v) < abs(m_u)) {
			if (newx > m_x)
				step = -1;

			for (tryx = newx + step; tryx != m_x; tryx += step) {
				tryy = m_y + ((tryx - m_x)*m_v)/m_u;

				GetRect(rt, tryx + CMainWnd::winPenguinsWindowRect.left, tryy + CMainWnd::winPenguinsWindowRect.top);
				if (!CMainWnd::combinedWindowRegion.RectInRegion(&rt)) {
					newx = tryx;
					newy = tryy;
					status = TOON_PARTIALMOVE;
					move_ahead = 1;
					break;
				}
			}
		} else {
			if (newy > m_y) 
				step = -1;

			if (m_v != 0){//ricky -error checking. can't divide by 0. velocity shouldn't be 0 anyway unless reading
				for (tryy = newy + step; tryy != m_y; tryy += step) {
					tryx = m_x + ((tryy - m_y)*m_u)/m_v;

					GetRect(rt, tryx + CMainWnd::winPenguinsWindowRect.left, tryy + CMainWnd::winPenguinsWindowRect.top);
					if (!CMainWnd::combinedWindowRegion.RectInRegion(&rt)) {
						newx = tryx;
						newy = tryy;
						status = TOON_PARTIALMOVE;
						move_ahead = 1;
						break;
					}
				}
			}
		}
	}

	if (move_ahead) {
		m_x = newx;
		m_y = newy;
	}

	AdvanceFrame();

	return status;
}

//Rick: checks if cursor position is inside the rectangle of a penguin
bool CToon::IsMouseOver()
{
	RECT rt;
	POINT pos;

	//get rectangle area of penguin
	GetRect(rt);
	GetCursorPos(&pos);

	//Rick 2021: Only pop balloons when mouse is over balloons area of sprite
	if (m_bmpIndex == penguin_BALLOONER) {	
		rt.bottom -= 30; //offset for small penguins	
	}
	else if (m_bmpIndex == penguin_BALLOONERLARGE) {
		rt.bottom -= 45; //offset for large penguins
	}

	//check whether mouse coordinates lie within penguin rectangle	
	return PtInRect(&rt, pos);
}

bool CToon::IsStartingUp()
{
	return m_startingUp;
}

bool CToon::CheckFullyEnteredScreen()
{
	//Rick 2021: Ensure newly spawned penguin is completely on the screen
	m_startingUp = m_y < CMainWnd::winPenguinsWindowRect.top;
	return m_startingUp;
}

CToon CToon::CreateStarterPenguin(bool largePenguin)
{
	static constexpr int faller_offscreen_offset = 40;
	static constexpr int ballooner_offscreen_offset = 30;

	//Rick 2021 - randomize initial penguins to either fallers or ballooners with 50/50 probability
	if (Random::get<bool>())
	{		
		const int fallerTypeBMPIndex = largePenguin ? penguin_FALLERLARGE : penguin_FALLER;
		const int x = Random::get<int>(CMainWnd::winPenguinsWindowRect.left, CMainWnd::winPenguinsWindowRect.right - penguin_data[fallerTypeBMPIndex].width);
		const int y = CMainWnd::winPenguinsWindowRect.top - penguin_data[fallerTypeBMPIndex].height - faller_offscreen_offset;
		const int u = Random::get(-3, 3); 
		constexpr int v = 3;
		return { x, y, u, v, fallerTypeBMPIndex, penguin_FOREWARD, largePenguin };
	}
	else
	{
		const int balloonerTypeBMPIndex = largePenguin ? penguin_BALLOONERLARGE : penguin_BALLOONER;
		const int x = Random::get<int>(CMainWnd::winPenguinsWindowRect.left, CMainWnd::winPenguinsWindowRect.right - penguin_data[balloonerTypeBMPIndex].width);
		const int y = CMainWnd::winPenguinsWindowRect.top - penguin_data[balloonerTypeBMPIndex].height - ballooner_offscreen_offset;
		const int u = Random::get(-2, 2);
		constexpr int v = 2;
		return  { x, y, u, v, balloonerTypeBMPIndex, penguin_FOREWARD, largePenguin };
	}

}



bool CToon::IsBlocked(int direction)
{	
	RECT rt;

	switch (direction) {
	case TOON_LEFT:
		if (m_x <= 0)
			return true;
		GetRect(rt, CMainWnd::winPenguinsWindowRect.left + m_x - 1, CMainWnd::winPenguinsWindowRect.top + m_y);
		break;
	case TOON_RIGHT:
		if (m_x + penguin_data[m_bmpIndex].width >= CMainWnd::winPenguinsWindowRect.right)
			return true;
		GetRect(rt, CMainWnd::winPenguinsWindowRect.left + m_x + 1, CMainWnd::winPenguinsWindowRect.top + m_y);
		break;
	case TOON_UP:
		if (m_y <= 0) 
			return true;
		GetRect(rt, CMainWnd::winPenguinsWindowRect.left + m_x, CMainWnd::winPenguinsWindowRect.top + m_y - 1);
		break;
	case TOON_DOWN: 
		if (m_y + penguin_data[m_bmpIndex].height >= CMainWnd::winPenguinsWindowRect.bottom)
			return true;
		GetRect(rt, CMainWnd::winPenguinsWindowRect.left + m_x, CMainWnd::winPenguinsWindowRect.top + m_y + 1);
		break;
	case TOON_HERE:
		GetRect(rt, CMainWnd::winPenguinsWindowRect.left + m_x, CMainWnd::winPenguinsWindowRect.top + m_y);
		break;
	default:
		ASSERT(0);
	}

	if (CMainWnd::combinedWindowRegion.GetSafeHandle() && CMainWnd::combinedWindowRegion.RectInRegion(&rt)) {
		return true;
	}

	return false;
}

void CToon::SetSubType(ToonSubType tst) 
{ 
	m_tstSubType = tst; 
}

ToonSubType CToon::GetSubType() 
{ 
	return m_tstSubType; 
}