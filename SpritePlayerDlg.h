#pragma once
#include "afxwin.h"

class CSpritePlayerDlg : public CDialog
{
public:
	CSpritePlayerDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_SPRITEPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void DrawSkin(CDC* pDC);
	BOOL LoadSkin();
	void MoveLocationDialog();
	CString GetModulePath();
	CGdiPlusBitmapResource* m_pBackSkin;	

	void InitControl();
	void LoadSpriteImage();
	void LabelLocation(Graphics& gps);
	void DrawSprite(Graphics& gps);
	void Start();
	void Stop();
	int m_curIndex;
	int m_TotalIndex;

	Gdiplus::Bitmap* m_pSprite;
	CUtilBitmap mutilBitmap;
	BOOL m_bMoveSprite; 
	int m_nSpeed;
	int m_nMoveX;

public:
	CLabelStatic m_Labelstatic;
	CPictureStatic m_Picturestatic;
	CSkinButton m_btnSkin;
};
