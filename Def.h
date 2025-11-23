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

enum class PenguinType {
	WALKER = 0,
	WALKER_UPSIDE_DOWN = 1,
	FALLER = 2,
	TUMBLER = 3,
	FLOATER = 4,
	CLIMBER = 5,
	CLIMBER_DOWN = 6,
	BOMBER = 7,
	EXPLOSION = 8,
	SANTA_WALKER = 9,
	SANTA_WALKER_UPSIDE_DOWN = 10,
	SANTA_CLIMBER = 11,
	SANTA_CLIMBER_DOWN = 12,
	READER = 13,
	BALLOONER = 14,
	SKATEBOARDER = 15,
	SKATEBOARDER_UPSIDE_DOWN = 16,
	SKATEBOARDER_CLIMBER = 17,
	SKATEBOARDER_CLIMBER_DOWN = 18,
	SPLAT = 19,
	EXIT = 20,
	SUPER = 21,
	//Rick 2021: Large Penguins
	WALKER_LARGE = 22,
	WALKER_UPSIDE_DOWN_LARGE = 23,
	FALLER_LARGE = 24,
	TUMBLER_LARGE = 25,
	FLOATER_LARGE = 26,
	CLIMBER_LARGE = 27,
	CLIMBER_DOWN_LARGE = 28,
	BOMBER_LARGE = 29,
	EXPLOSION_LARGE = 30,
	SANTA_WALKER_LARGE = 31,
	SANTA_WALKER_UPSIDE_DOWN_LARGE = 32,
	SANTA_CLIMBER_LARGE = 33,
	SANTA_CLIMBER_DOWN_LARGE = 34,
	READER_LARGE = 35,
	BALLOONER_LARGE = 36,
	SKATEBOARDER_LARGE = 37,
	SKATEBOARDER_UPSIDE_DOWN_LARGE = 38,
	SKATEBOARDER_CLIMBER_LARGE = 39,
	SKATEBOARDER_CLIMBER_DOWN_LARGE = 40,
	SPLAT_LARGE = 41,
	EXIT_LARGE = 42,
	SUPER_LARGE = 43
};

//number of bitmaps
constexpr int PenguinBitmapCount = 44; //Rick 2021: 22 small bitmaps. 22 large bitmaps.

struct ToonData {
   DWORD resId;  // resource id
   DWORD mskResId;  // resource id of the bitmask
   CBitmap bmp;
   CBitmap mskBmp;

   int nframes,ndirections; /* number of frames and directions in image */
   int width,height; /* width and height of an individual frame/direction */

   bool cycle;  // does the animation cycle
};

extern ToonData penguin_data[PenguinBitmapCount];

#endif
