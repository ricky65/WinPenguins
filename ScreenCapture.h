/**
 *  ScreenCapture.h
 *
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

 //Rick 2021: Module to save a screenshot in BMP/PNG/JPG formats using GDI+ library

#ifndef SCREEN_CAPTURE_H_INCLUDED_
#define SCREEN_CAPTURE_H_INCLUDED_

#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

#include <string_view>
#include <vector>

int GetEncoderClsid(const std::wstring_view format, CLSID* pClsid);
void GetScreenshot(const CString & lpszFilename, const std::wstring_view format);

#endif
