#pragma once

#include "trackball.h"
#include "Math\math.h"

#include "TestModule.h"

// XenoTestbedWindow

class Point
{
public:
	Point() {}
	Point(int32 _x, int32 _y) : x(_x), y(_y) {}

	int32 x;
	int32 y;
};

class XenoTestbedWindow
{

public:

	XenoTestbedWindow(HINSTANCE hInstance);
	virtual ~XenoTestbedWindow();

public:

	bool Init(void);
	void SetSize(int width, int height);
	void Simulate(float32 dt);
	void Solve( float32* v1, float32* v2, float32 h1, float32 h2, float32 target );
	void DrawScene(void);

	void LoadView();
	void SaveView();

	void WindowPointToWorldRay(Vector* rayOrigin, Vector* rayDirection, const Point& p);

	HWND GetHWND() { return m_hWnd; }

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static XenoTestbedWindow* s_this;

protected:

	HGLRC m_glContext;

	TrackBall* m_rotation;
	Vector* m_translation;

	int32 m_moduleIndex;
	TestModule* m_module;

	bool m_leftButtonDown;
	bool m_middleButtonDown;
	bool m_rightButtonDown;

	Point m_lastMousePoint;

	HINSTANCE m_hInstance;
	HWND m_hWnd;

	int32 m_captureCount;

public:

	void OnPaint();
	void OnLButtonDown(UINT nFlags, Point point);
	void OnLButtonUp(UINT nFlags, Point point);
	void OnMButtonDown(UINT nFlags, Point point);
	void OnMButtonUp(UINT nFlags, Point point);
	void OnRButtonDown(UINT nFlags, Point point);
	void OnRButtonUp(UINT nFlags, Point point);
	void OnMouseMove(UINT nFlags, Point point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnSize(UINT nType, int cx, int cy);
};

