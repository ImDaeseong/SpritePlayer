#include "stdafx.h"
#include "SpritePlayer.h"
#include "SpritePlayerDlg.h"


#define TotalCount 14

typedef struct {
	int index;
	int x;
	int y;
	int widht;
	int height;
} Point_t;

static const Point_t SpritePoint[TotalCount] = 
{
	{0, 23,  2, 80, 81},
	{1, 128, 4, 86, 78},
	{2, 242, 8, 80, 74},
	{3, 337, 11, 95, 69},
	{4, 440, 34, 104, 45},

	{5,  24, 135, 79, 64 },
	{6,  140, 134, 72,  68},
	{7,  251, 135, 71, 63},
	{8,  359, 135, 72, 58},
	{9,  468, 137, 73, 58},

	{10, 32, 241, 71, 56},
	{11, 133, 242, 80, 55},
	{12, 228, 242, 96, 45},
	{13, 333, 224, 99, 63}
};

CSpritePlayerDlg::CSpritePlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpritePlayerDlg::IDD, pParent)
{
	m_pBackSkin	= NULL;	

	m_pSprite = NULL;
	m_curIndex = 0;
	m_TotalIndex = 0;

	m_bMoveSprite = TRUE; 
	m_nSpeed = 5;
	m_nMoveX = 50;
}

void CSpritePlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LABEL, m_Labelstatic);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_Picturestatic);
	DDX_Control(pDX, IDC_BUTTON_SKIN, m_btnSkin);
}

BEGIN_MESSAGE_MAP(CSpritePlayerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CSpritePlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	if(!LoadSkin())
	{
		CDialog::OnCancel();
		return FALSE;
	}

	InitControl();

	CDC* pDC = GetDC();
	DrawSkin(pDC);
	ReleaseDC(pDC);

	return TRUE;  
}

void CSpritePlayerDlg::OnPaint()
{
	CPaintDC dc(this); 
	DrawSkin(&dc);
}

void CSpritePlayerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_pSprite != NULL)
		delete m_pSprite;
	m_pSprite = NULL;	

	if(m_pBackSkin != NULL)
		delete m_pBackSkin;
	m_pBackSkin = NULL;	
}

void CSpritePlayerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{	
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE,  0,0);
	}
	else
	{
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE,  0,0);
	}	

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CSpritePlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSpritePlayerDlg::DrawSkin(CDC* pDC)
{
	CDC memDC;
	CBitmap bmp;
	CBitmap* bmp_old;

	CRect rect;
	GetWindowRect(&rect);

	int cx = rect.Width();
	int cy = rect.Height();

	memDC.CreateCompatibleDC(pDC);

	BITMAPINFO bmInfo;
	memset(&bmInfo, 0x00, sizeof(BITMAPINFO));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = cx;
	bmInfo.bmiHeader.biHeight = cy;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 32;
	bmInfo.bmiHeader.biCompression = BI_RGB;
	bmInfo.bmiHeader.biSizeImage = 0;
	bmInfo.bmiHeader.biClrUsed = 0;
	bmInfo.bmiHeader.biClrImportant = 0;

	LPBYTE pbmpBits = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), (LPBITMAPINFO)&bmInfo, DIB_RGB_COLORS, (void **)&pbmpBits, NULL, 0);
	if(hBitmap == NULL)
		bmp.CreateCompatibleBitmap(pDC, cx, cy);
	else
		bmp.Attach(hBitmap);

	bmp_old = memDC.SelectObject(&bmp);
	Graphics gps(memDC.GetSafeHdc());

	gps.DrawImage(
		m_pBackSkin->m_pBitmap, 
		Rect(0, 0, m_pBackSkin->m_pBitmap->GetWidth(), m_pBackSkin->m_pBitmap->GetHeight()), 
		0,
		0, 
		m_pBackSkin->m_pBitmap->GetWidth(), 
		m_pBackSkin->m_pBitmap->GetHeight(),
		UnitPixel);

	DrawSprite(gps);

	if(m_Picturestatic.IsAvailableDraw())
		m_Picturestatic.OnDrawLayerdWindow(gps);

	m_Labelstatic.OnDrawLayerdWindow(gps);

	if(m_btnSkin.IsAvailableDraw())
		m_btnSkin.OnDrawLayerdWindow(gps);

	//LabelLocation(gps);
	
	CPoint ptWindowScreenPosition(rect.TopLeft());
	CSize szWindow(cx, cy);

	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	CPoint ptSrc(0,0);

	HDC hdc = gps.GetHDC();

	BOOL bRet= ::UpdateLayeredWindow(GetSafeHwnd(), 
		pDC->GetSafeHdc(),
		&ptWindowScreenPosition,
		&szWindow,
		hdc,
		&ptSrc,
		0, 
		&blendPixelFunction,
		ULW_ALPHA);

	gps.ReleaseHDC(hdc);

	memDC.SelectObject(bmp_old);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

BOOL CSpritePlayerDlg::LoadSkin()
{
	m_pBackSkin = new CGdiPlusBitmapResource;
	if(!m_pBackSkin->LoadPath(""))
	{
		if(!m_pBackSkin->Load(MAKEINTRESOURCE(IDB_PNG_BG5), _T("PNG"), AfxGetApp()->m_hInstance))
		{
			delete m_pBackSkin;
			m_pBackSkin = NULL;
			return FALSE;
		}
	}
	
	MoveLocationDialog();
		
	return TRUE;
}

void CSpritePlayerDlg::MoveLocationDialog()
{	
	int cx = m_pBackSkin->m_pBitmap->GetWidth();
	int cy = m_pBackSkin->m_pBitmap->GetHeight();

	RECT rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	MoveWindow( ((rcWorkArea.right - cx)/2), ((rcWorkArea.bottom - cy)/2), cx, cy);
}

CString CSpritePlayerDlg::GetModulePath()
{
	char cTemp[MAX_PATH];
	char *spzRealDirEnd;
	CString strModulePath;

	GetModuleFileName(NULL, cTemp, MAX_PATH);  
	spzRealDirEnd = strrchr(cTemp, '\\');
	*spzRealDirEnd = '\0';
	strModulePath = (CString)cTemp;
	return strModulePath;
}

void CSpritePlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		m_curIndex = (m_curIndex + 1) % m_TotalIndex;	

		if(m_bMoveSprite)
		{
			if( m_nMoveX > (m_pBackSkin->m_pBitmap->GetWidth()))
				m_nMoveX = 50;
			else
				m_nMoveX += m_nSpeed;
		}

		CString strMessage;
		strMessage.Format("%d", m_nMoveX);
		m_Labelstatic.SetText(strMessage);


		CString strImgPath;
		if(m_curIndex == 10)
			strImgPath.Format("%s\\bg1.png", GetModulePath());				
		else if(m_curIndex == 1)
			strImgPath.Format("%s\\bg2.png", GetModulePath());				
		m_Picturestatic.SetImagePath(strImgPath);
		
		InvalidateRect(NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

void CSpritePlayerDlg::InitControl()
{	

	m_Labelstatic.SetLabelPoint(60, 30, 520, 40);
	m_Labelstatic.SetFontName("굴림체");
	m_Labelstatic.SetTextColor(RGB(155, 155, 155));
	m_Labelstatic.SetFontSize(40);
	m_Labelstatic.SetFontBold(TRUE);
	m_Labelstatic.SetStringAlign(1);
	m_Labelstatic.SetText("스프라이트 이미지 테스트");
	m_Labelstatic.SetLabelStaticClick(TRUE);
	m_Labelstatic.SetLabelStaticCursor(TRUE);


	CString strImgPath;
	strImgPath.Format("%s\\bg1.png", GetModulePath());	
	m_Picturestatic.SetImagePath(strImgPath);
	m_Picturestatic.MoveWindow(450, 130, 148, 89);
	m_Picturestatic.SetPictureStaticClick(TRUE);
	m_Picturestatic.SetPictureStaticCursor(TRUE);


	strImgPath.Format("%s\\bear.png", GetModulePath());	
	m_btnSkin.SetImagePath(strImgPath, -70, 50, 1);
	m_btnSkin.SetToolTipText("툴팁 테스트");

	
	LoadSpriteImage();


	Start();
}

void CSpritePlayerDlg::Start()
{
	Stop();
	
	if (m_TotalIndex > 0)
		m_curIndex = 0;
	
	SetTimer(1, 40, NULL);
}

void CSpritePlayerDlg::Stop()
{	
	KillTimer(1);
	m_curIndex = 0;
	Invalidate(FALSE);
}

void CSpritePlayerDlg::LoadSpriteImage() 
{
	CString strImgPath;
	strImgPath.Format("%s\\sprite.png", GetModulePath());	
	m_pSprite = mutilBitmap.SetLoadBitmap(strImgPath);
	m_TotalIndex = TotalCount;
}

void CSpritePlayerDlg::LabelLocation(Graphics& gps)
{
	CRect rc;
	m_Labelstatic.GetWindowRect(&rc);
	ScreenToClient(rc);	
	RectF rectF(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));
	SolidBrush bgbrush(Color(112, 0, 0));
	gps.FillRectangle(&bgbrush, rectF);	
}

void CSpritePlayerDlg::DrawSprite(Graphics& gps)
{		
	if(m_bMoveSprite)
	{
		if(m_pSprite)
		{		
			gps.SetSmoothingMode(SmoothingModeHighQuality);
			gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);

			int nImgWidth = SpritePoint[m_curIndex].widht;
			int nImgHeight = SpritePoint[m_curIndex].height;
			int x = SpritePoint[m_curIndex].x;
			int y = SpritePoint[m_curIndex].y;

			//window size
			CRect rc;
			GetClientRect(&rc);
			Rect wndRect(m_nMoveX, ((rc.bottom - nImgHeight)/2),	nImgWidth, nImgHeight);

			////image size
			Rect ImgRect( x, y, nImgWidth, nImgWidth);
			gps.DrawImage(m_pSprite, wndRect, ImgRect.X, ImgRect.Y, ImgRect.Width, ImgRect.Height, UnitPixel);

			/*
			Gdiplus::Matrix matrix;
            Gdiplus::PointF P( m_nMoveX, nImgHeight );    
              
			float m_iAngle = (float)0;
			matrix.RotateAt( - m_iAngle, P); 
			gps.SetTransform(&matrix);

			gps.SetSmoothingMode(SmoothingModeHighQuality);
			gps.DrawImage( m_pSprite, ImgRect.X, ImgRect.Y, ImgRect.Width, ImgRect.Height);
			*/
		}
	}
	else
	{
		if(m_pSprite)
		{		
			gps.SetSmoothingMode(SmoothingModeHighQuality);
			gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);

			int nImgWidth = SpritePoint[m_curIndex].widht;
			int nImgHeight = SpritePoint[m_curIndex].height;
			int x = SpritePoint[m_curIndex].x;
			int y = SpritePoint[m_curIndex].y;

			//window size
			CRect rc;
			GetClientRect(&rc);
			Rect wndRect(((rc.right - nImgWidth)/2), ((rc.bottom - nImgHeight)/2),	nImgWidth, nImgHeight);

			//image size
			Rect ImgRect( x, y, nImgWidth, nImgWidth);
			gps.DrawImage(m_pSprite, wndRect, ImgRect.X, ImgRect.Y, ImgRect.Width, ImgRect.Height, UnitPixel);
		}
	}

}