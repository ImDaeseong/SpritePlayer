#include "stdafx.h"
#include "LabelStatic.h"

IMPLEMENT_DYNAMIC(CLabelStatic, CStatic)

CLabelStatic::CLabelStatic()
{
	m_strMsg = "";
	m_strFont = "굴림";
	m_nFontSize = 12;
	m_crText = RGB(0, 0, 0);
	m_FontStyle = FontStyle::FontStyleRegular;
    m_nAlign = 0;

	m_bClick = FALSE;
	m_bCursor = FALSE;

	m_xPoint = 0;
	m_yPoint = 0;
	m_nWidth = 0;
	m_nHeight = 0; 
}

CLabelStatic::~CLabelStatic()
{
}

BEGIN_MESSAGE_MAP(CLabelStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

BOOL CLabelStatic::OnEraseBkgnd(CDC* pDC)
{	
	return TRUE;
}

void CLabelStatic::SetFontName(CString strFont)
{	
	m_strFont = strFont;
}

void CLabelStatic::SetFontSize(int nFontSize)
{
	m_nFontSize = nFontSize;
}

void CLabelStatic::SetTextColor(COLORREF crText)
{
	m_crText = crText;
}

void CLabelStatic::SetFontBold(BOOL bBold)
{	
	if(bBold)
		m_FontStyle = FontStyle::FontStyleBold;
	else
		m_FontStyle = FontStyle::FontStyleRegular;
}

void CLabelStatic::SetStringAlign(int nAlign)
{
	m_nAlign = nAlign;
}

void CLabelStatic::OnPaint()
{
	CPaintDC dc(this); 	
}

CLabelStatic& CLabelStatic::SetLabelStaticClick(BOOL bClick)
{
	m_bClick = bClick;

	if (bClick)
		ModifyStyle(0,SS_NOTIFY);
	else
		ModifyStyle(SS_NOTIFY,0);
	return *this;
}

void CLabelStatic::SetLabelStaticCursor(BOOL bCursor)
{
	m_bCursor = bCursor;	
}

void CLabelStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bClick)
	{
		::ShellExecute(NULL, "open", "", "", NULL, SW_SHOWNORMAL);
	}	
}

BOOL CLabelStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(m_bCursor)
	{		
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return TRUE;
	}
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CLabelStatic::SetText(CString strMsg)
{	
	m_strMsg = strMsg;
	/*
	CRect rect;
	GetWindowRect(&rect);
	GetParent()->ScreenToClient(&rect);
	GetParent()->InvalidateRect(rect, TRUE);
	Invalidate(FALSE);
	*/
}

void CLabelStatic::SetLabelPoint(int xPoint, int yPoint, int nWidth, int nHeight)
{
	m_xPoint = xPoint;
	m_yPoint = yPoint;
	m_nWidth = nWidth;
	m_nHeight = nHeight; 
}

void CLabelStatic::OnDrawLayerdWindow(Graphics& Gps)
{
	USES_CONVERSION;
	
	if(m_nWidth > 0 || m_nHeight > 0)
	{
		Gps.SetSmoothingMode(SmoothingModeHighQuality);
		Gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		
		FontFamily fontFamily(A2W(m_strFont));
		Gdiplus::Font font(&fontFamily, m_nFontSize, m_FontStyle, UnitPixel);

		Gdiplus::SolidBrush brush(Gdiplus::Color(255, GetRValue(m_crText), GetGValue(m_crText), GetBValue(m_crText)));
		
		StringFormat stringFormat(StringFormatFlagsDisplayFormatControl);
		if(m_nAlign == 1)
		{
			stringFormat.SetAlignment(StringAlignmentCenter);
			stringFormat.SetLineAlignment(StringAlignmentCenter);
		}
		else if(m_nAlign == 2)
		{
			stringFormat.SetAlignment(StringAlignmentFar);
			stringFormat.SetLineAlignment(StringAlignmentFar);
		}
		else
		{
			stringFormat.SetAlignment(StringAlignmentNear);
			stringFormat.SetLineAlignment(StringAlignmentNear);
		}

		//FontStyleRegular 타입일때 텍스트가 보이지 않는다면 이부분 제거
		Gps.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);	

		Gdiplus::RectF rectF(m_xPoint, m_yPoint, m_nWidth, m_nHeight);
		Gps.DrawString(A2W(m_strMsg), -1, &font, rectF, &stringFormat, &brush);

		//SolidBrush bgbrush(Color(112, 0, 0));
		//Gps.FillRectangle(&bgbrush, rectF);		
	}
	else
	{
		Gps.SetSmoothingMode(SmoothingModeHighQuality);
		Gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		
		FontFamily fontFamily(A2W(m_strFont));
		Gdiplus::Font font(&fontFamily, m_nFontSize, m_FontStyle, UnitPixel);

		Gdiplus::SolidBrush brush(Gdiplus::Color(255, GetRValue(m_crText), GetGValue(m_crText), GetBValue(m_crText)));
		
		//FontStyleRegular 타입일때 텍스트가 보이지 않는다면 이부분 제거
		Gps.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

		PointF PointF((float)(m_xPoint), (float)(m_yPoint));
		Gps.DrawString(A2W(m_strMsg), -1, &font, PointF, &brush);
	}
}
