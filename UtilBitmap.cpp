#include "stdafx.h"
#include "UtilBitmap.h"

CUtilBitmap::CUtilBitmap()
{

}

CUtilBitmap::~CUtilBitmap()
{
}


Bitmap* CUtilBitmap::SetLoadBitmap(CString strImgPath)
{
	WCHAR wFile[MAX_PATH];   
#ifndef   UNICODE   
	MultiByteToWideChar(CP_ACP,   0,   LPCTSTR(strImgPath),   -1,   wFile,   MAX_PATH);   
#else   
	lstrcpy(wFile,   strImgPath);   
#endif   

	return Gdiplus::Bitmap::FromFile(wFile, false);
}

Bitmap* CUtilBitmap::CloneBitmap(Gdiplus::Bitmap* pBitmap)
{
	Bitmap* pNewBitmap = new Bitmap(pBitmap->GetWidth(), pBitmap->GetHeight(), PixelFormat32bppARGB);
	//Bitmap* pNewBitmap = new Bitmap(pBitmap->GetWidth(), pBitmap->GetHeight(), PixelFormat24bppRGB);
	//Bitmap* pNewBitmap = new Bitmap(pBitmap->GetWidth(), pBitmap->GetHeight(), pBitmap->GetPixelFormat());

	Graphics gps(pNewBitmap);
	gps.DrawImage(pBitmap, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
	return pNewBitmap;
}

Bitmap* CUtilBitmap::GetFromHBITMAP(HBITMAP hBitmap)
{
	return Bitmap::FromHBITMAP(hBitmap, NULL);
}

HBITMAP CUtilBitmap::GetFromBitmap(Gdiplus::Bitmap* pBitmap)
{	
	HBITMAP hBitmap = NULL;
    pBitmap->GetHBITMAP(NULL, &hBitmap);
    return hBitmap;
}

Bitmap* CUtilBitmap::ResizeBitmap(Gdiplus::Bitmap* pBitmap, int nWidth, int nHeight)
{
	Bitmap* pNewBitmap = new Bitmap(nWidth, nHeight);
	Graphics gps(pNewBitmap);
	//gps.Clear(Color(0, 0, 0, 0));
    gps.DrawImage(pBitmap, 0, 0, nWidth, nHeight);
    delete pBitmap; 
	return pNewBitmap;
}

Bitmap* CUtilBitmap::CropBitmap(Gdiplus::Bitmap* pBitmap, int x, int y, int nWidth, int nHeight)
{
	Bitmap* pNewBitmap = new Bitmap(nWidth, nHeight);
	Graphics gps(pNewBitmap);
	//gps.Clear(Color(0, 0, 0, 0));
	gps.DrawImage(pBitmap, RectF(0, 0, (REAL)nWidth, (REAL)nHeight), (REAL)x, (REAL)y, (REAL)nWidth, (REAL)nHeight, Gdiplus::Unit::UnitPixel);
	delete pBitmap;
	return pNewBitmap;
}

void CUtilBitmap::FlipBitmap(Gdiplus::Bitmap* pBitmap, bool bXRotate, bool bYRotate)
{
	RotateFlipType rftFlip;

	if (!bXRotate && !bYRotate)
		rftFlip = RotateFlipType::RotateNoneFlipNone;
	else if (bXRotate && !bYRotate)
		rftFlip = RotateFlipType::RotateNoneFlipX;
	else if (!bXRotate && bYRotate)
		rftFlip = RotateFlipType::RotateNoneFlipY;
	else if (bXRotate && bYRotate)
		rftFlip = RotateFlipType::RotateNoneFlipXY;

	pBitmap->RotateFlip(rftFlip);
}

Bitmap* CUtilBitmap::RotateBitmap(Gdiplus::Bitmap* pBitmap, int nAngle)
{
	int nX = pBitmap->GetWidth() / 2;
	int nY = pBitmap->GetHeight() / 2;
	
	Bitmap* pNewBitmap = new Bitmap( pBitmap->GetWidth(), pBitmap->GetHeight());
	Graphics gps(pNewBitmap);

	gps.TranslateTransform((REAL)nX, (REAL)nY);
	gps.RotateTransform((REAL)nAngle);
	gps.TranslateTransform((REAL)-nX, (REAL)-nY);
	gps.Clear(Color(0, 0, 0, 0));
	gps.DrawImage(pBitmap, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());

	delete pBitmap;
	return pNewBitmap;
}

BOOL CUtilBitmap::IsAlpha(Gdiplus::Bitmap* pBitmap)
{
	return pBitmap->GetPixelFormat() == PixelFormat32bppARGB;
}

Bitmap* CUtilBitmap::SetChangeColor(Gdiplus::Bitmap* pBitmap, Color from, Color to)
{
	int nWidth  = pBitmap->GetWidth();
	int nHeight = pBitmap->GetHeight();

	BitmapData bmpData;
	Rect rect(0, 0, nWidth, nHeight);
	pBitmap->LockBits(&rect, ImageLockModeRead|ImageLockModeWrite, PixelFormat24bppRGB, &bmpData);
	BYTE *pByte = (BYTE*)bmpData.Scan0;
	int iOffset = bmpData.Stride - nWidth * 3;
	
	for (int y = 0; y < nHeight; y++)
	{
		for (int x = 0; x < nWidth; x++)
		{
			if (pByte[2] == from.GetRed() && pByte[1] == from.GetGreen() &&	pByte[0] == from.GetBlue())
			{
				pByte[2] = to.GetRed();
				pByte[1] = to.GetGreen();
				pByte[0] = to.GetGreen();
			}
			pByte += 3;
		}
		pByte += iOffset;
	}
	pBitmap->UnlockBits(&bmpData);

	return pBitmap;
}
