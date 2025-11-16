/**
 *  ScreenCapture.cpp
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

#include "StdAfx.h"
#include "ScreenCapture.h"

using namespace Gdiplus;

int GetEncoderClsid(const std::wstring_view format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    const Status GetImageEncodersSizeStatus = GetImageEncodersSize(&num, &size);
    if (GetImageEncodersSizeStatus != Status::Ok || size == 0)
        return -1;  // Failure

    std::vector<std::byte> dataVec { size };

    ImageCodecInfo* pImageCodecInfo = reinterpret_cast<ImageCodecInfo*>(dataVec.data());

    const Status GetImageEncodersStatus = GetImageEncoders(num, size, pImageCodecInfo);
	if (GetImageEncodersStatus != Status::Ok)
		return -1;  // Failure

    for (UINT j = 0; j < num; ++j)
    {
        if (pImageCodecInfo[j].MimeType == format)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            return j;  // Success
        }
    }

    return -1;  // Failure
}

void GetScreenshot(const CString & lpszFilename, const std::wstring_view format)
{
	// Initialize GDI+.
    ULONG_PTR gdiplusToken;
	const GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
	
	HDC hdcScreen  = CreateDC(L"DISPLAY", nullptr, nullptr, nullptr);
	HDC hdcCapture = CreateCompatibleDC(hdcScreen);
	const int nWidth = GetDeviceCaps(hdcScreen, HORZRES);
	const int nHeight = GetDeviceCaps(hdcScreen, VERTRES);	
	
	HBITMAP hbmCapture = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
	if(!hbmCapture){
		DeleteDC(hdcCapture);
		DeleteDC(hdcScreen);
		GdiplusShutdown(gdiplusToken);
		return;
	}
	
	const int nCapture = SaveDC(hdcCapture);
	SelectObject(hdcCapture, hbmCapture);
	BitBlt(hdcCapture, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY);
	RestoreDC(hdcCapture, nCapture);
	DeleteDC(hdcCapture);
	DeleteDC(hdcScreen);

	// Get the CLSID of the encoder.
	CLSID imageCLSID;
	const int EncoderClsidRes = GetEncoderClsid(format, &imageCLSID);
	if (EncoderClsidRes != -1)
	{
		Bitmap bmpScreenShot { hbmCapture, nullptr };
		bmpScreenShot.Save(lpszFilename, &imageCLSID, nullptr);
	}

	DeleteObject(hbmCapture);
	GdiplusShutdown(gdiplusToken);

	return;
}