#pragma once

class CLabelStatic : public CStatic
{
	DECLARE_DYNAMIC(CLabelStatic)

public:
	CLabelStatic();
	virtual ~CLabelStatic();

	void SetFontName(CString strFont);
	void SetFontSize(int nFontSize);
	void SetTextColor(COLORREF crText);
	void SetFontBold(BOOL bBold);
	void SetStringAlign(int nAlign);
	void SetText(CString strMsg);   
	void SetLabelPoint(int xPoint, int yPoint, int nWidth = 0, int nHeight = 0);
	void OnDrawLayerdWindow(Graphics& Gps);

	CLabelStatic& SetLabelStaticClick(BOOL bClick = FALSE);
	void SetLabelStaticCursor(BOOL bCursor = FALSE);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

private:
	CString    m_strMsg;
	CString	   m_strFont;
	int		   m_nFontSize;	
	COLORREF   m_crText;
	FontStyle  m_FontStyle;
	int        m_nAlign;
	
	BOOL       m_bClick;
	BOOL       m_bCursor;

	int        m_xPoint;
	int        m_yPoint;
	int        m_nWidth;
	int        m_nHeight; 
};


