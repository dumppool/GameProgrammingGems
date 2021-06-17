// Canvas.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "HLADemo.h"
#include "Canvas.h"
#include ".\canvas.h"

// CCanvas

IMPLEMENT_DYNAMIC(CCanvas, CWnd)
CCanvas::CCanvas(ICanvasDelegate *dg)
{
	m_dg=dg;
}

CCanvas::~CCanvas()
{
}

BEGIN_MESSAGE_MAP(CCanvas, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CCanvas 메시지 처리기입니다.


int CCanvas::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_font.Attach(new Font(L"Arial",9));
	m_backBuffer=new Bitmap(800,800,PixelFormat32bppARGB);

	return 0;
}

void CCanvas::OnDestroy()
{
	CWnd::OnDestroy();

	m_font.Free();
	delete m_backBuffer;
}

void CCanvas::OnPaint()
{
	RenderToBackBuffer();

	CPaintDC dc(this); // device context for painting
	Graphics graphics(dc);
	graphics.DrawImage(m_backBuffer,0,0);
}

void CCanvas::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_dg->OnLButtonDown(nFlags,point);

	CWnd::OnLButtonDown(nFlags, point);
}

void CCanvas::RenderToBackBuffer()
{
	Graphics graphics(m_backBuffer);
	Color bgr(255,255,255,255);
	graphics.Clear(bgr);
	m_graphicsOnRender=&graphics;
	m_dg->OnRender();
}

void CCanvas::Render()
{
	RenderToBackBuffer();

	CPaintDC dc(this); // device context for painting
	Graphics graphics(dc);
	graphics.DrawImage(m_backBuffer,0,0);
}
void CCanvas::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_dg->OnLButtonUp(nFlags,point);

	CWnd::OnLButtonUp(nFlags, point);
}

void CCanvas::OnMouseMove(UINT nFlags, CPoint point)
{
	m_dg->OnMouseMove(nFlags,point);

	CWnd::OnMouseMove(nFlags, point);
}
