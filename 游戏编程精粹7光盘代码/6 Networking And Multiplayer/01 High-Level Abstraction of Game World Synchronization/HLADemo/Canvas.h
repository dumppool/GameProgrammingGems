#pragma once

class CHLADemoDlg;

class ICanvasDelegate
{
public:
	virtual ~ICanvasDelegate() {}
	virtual void OnRender()=0;
	virtual void OnLButtonDown(UINT nFlags, CPoint point)=0;
	virtual void OnLButtonUp(UINT nFlags, CPoint point)=0;
	virtual void OnMouseMove(UINT nFlags, CPoint point)=0;
};

// CCanvas

class CCanvas : public CWnd
{
	DECLARE_DYNAMIC(CCanvas)

	ICanvasDelegate *m_dg;
public:
	Bitmap* m_backBuffer;
	CHLADemoDlg* m_owner;
	CAutoPtr<Font> m_font;
	Graphics *m_graphicsOnRender; // valid only while RenderBackBuffer() is working

	CCanvas(ICanvasDelegate *dg);
	virtual ~CCanvas();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();

	void RenderToBackBuffer();
	void Render();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


