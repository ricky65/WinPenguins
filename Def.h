/**
 *  Def.h
 *
 *  Copyright (C) 2000  Michael Vines
 *  Copyright (c) 2011-2025 Riccardo Marcangelo
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

#ifndef _DEF_H_INCLUDED_
#define _DEF_H_INCLUDED_

#define penguin_FOREWARD	0
#define penguin_LEFTRIGHT	1
#define penguin_LEFT		0
#define penguin_RIGHT		1

#define penguin_WALKER							0
#define penguin_WALKER_UPSIDE_DOWN				1
#define penguin_FALLER							2
#define penguin_TUMBLER							3
#define penguin_FLOATER							4
#define penguin_CLIMBER							5
#define penguin_CLIMBER_DOWN					6
#define penguin_BOMBER							7
#define penguin_EXPLOSION						8
#define penguin_SANTA_WALKER					9
#define penguin_SANTA_WALKER_UPSIDE_DOWN		10
#define penguin_SANTA_CLIMBER					11
#define penguin_SANTA_CLIMBER_DOWN				12
#define penguin_READER							13
#define penguin_BALLOONER						14
#define penguin_SKATEBOARDER					15
#define penguin_SKATEBOARDER_UPSIDE_DOWN		16
#define penguin_SKATEBOARDER_CLIMBER			17	
#define penguin_SKATEBOARDER_CLIMBER_DOWN		18	
#define penguin_SPLAT							19
#define penguin_EXIT							20
#define penguin_SUPER							21
//Rick 2021: Large Penguins
#define penguin_WALKERLARGE						22
#define penguin_WALKER_UPSIDE_DOWNLARGE			23
#define penguin_FALLERLARGE						24
#define penguin_TUMBLERLARGE					25	
#define penguin_FLOATERLARGE					26		
#define penguin_CLIMBERLARGE					27
#define penguin_CLIMBER_DOWNLARGE				28
#define penguin_BOMBERLARGE						29
#define penguin_EXPLOSIONLARGE					30
#define penguin_SANTA_WALKERLARGE				31
#define penguin_SANTA_WALKER_UPSIDE_DOWNLARGE	32
#define penguin_SANTA_CLIMBERLARGE				33
#define penguin_SANTA_CLIMBER_DOWNLARGE			34
#define penguin_READERLARGE						35				
#define penguin_BALLOONERLARGE					36
#define penguin_SKATEBOARDERLARGE				37
#define penguin_SKATEBOARDER_UPSIDE_DOWNLARGE	38	
#define penguin_SKATEBOARDER_CLIMBERLARGE		39
#define penguin_SKATEBOARDER_CLIMBER_DOWNLARGE	40
#define penguin_SPLATLARGE						41
#define penguin_EXITLARGE						42
#define penguin_SUPERLARGE						43

//number of bitmaps
#define penguin_BITMAP_COUNT					44 //Rick 2021: 22 small bitmaps. 22 large bitmaps.

struct ToonData {
   DWORD resId;  // resource id
   DWORD mskResId;  // resource id of the bitmask
   CBitmap bmp;
   CBitmap mskBmp;

   int nframes,ndirections; /* number of frames and directions in image */
   int width,height; /* width and height of an individual frame/direction */

   bool cycle;  // does the animation cycle
};

extern ToonData penguin_data[penguin_BITMAP_COUNT];

#endif
