#pragma once

class CUtilBitmap 
{
public:
	CUtilBitmap();
	virtual ~CUtilBitmap();

	Bitmap* SetLoadBitmap(CString strImgPath);
	Bitmap* CloneBitmap(Gdiplus::Bitmap* pBitmap);
	Bitmap* GetFromHBITMAP(HBITMAP hBitmap);
	HBITMAP GetFromBitmap(Gdiplus::Bitmap* pBitmap);
    Bitmap* ResizeBitmap(Gdiplus::Bitmap* pBitmap, int nWidth, int nHeight);
	Bitmap* CropBitmap(Gdiplus::Bitmap* pBitmap, int x, int y, int nWidth, int nHeight);
	void FlipBitmap(Gdiplus::Bitmap* pBitmap, bool bXRotate, bool bYRotate);
	Bitmap* RotateBitmap(Gdiplus::Bitmap* pBitmap, int nAngle);
	BOOL IsAlpha(Gdiplus::Bitmap* pBitmap);
	Bitmap* SetChangeColor(Gdiplus::Bitmap* pBitmap, Color from, Color to);
};


