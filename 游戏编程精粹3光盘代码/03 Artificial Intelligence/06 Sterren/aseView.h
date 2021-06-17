/* Copyright (C) William van der Sterren, 2002. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) William van der Sterren, 2002"
 */

/* Copyright (C) James Matthews, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) James Matthews, 2001"
 */

#ifndef _AFX_ASEVIEW_H_
#define _AFX_ASEVIEW_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "pathfinder.h"

class CAseView : CView
{
protected:
	CAseView();
	DECLARE_DYNCREATE(CAseView)

public:
//	bool	DebugSet() { return (m_cDebug.x != -1); }
	void	HighlightNode(_asNode *, bool);
	void	RemoveHighlight();

	CAseDoc *GetDocument();

	// Virtual and inline functions

	virtual ~CAseView();
	inline void SetBrushType(UINT b) { m_uBrushType = b; }

  void DisplayWaitCursor();
  void DisplayDefaultCursor();

	//{{AFX_VIRTUAL(CAseView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool		m_bDragging;

	UINT		m_uBrushType;

	CPoint		m_cStart;
	CPoint		m_cEnd;
	CPoint		m_cHilight;

	_asNode *	m_cHilightNode;

	void		DrawRoute(CDC *);
	void		MouseToPoint(CPoint, UINT);

	inline void		DrawRectangle(int, int, CDC *, COLORREF);
	inline void		DrawRectangleNB(int, int, CDC *, COLORREF);
	inline void		DrawCircle(int, int, CDC *, COLORREF);

	//{{AFX_MSG(CAseView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline CAseDoc* CAseView::GetDocument()
   { return (CAseDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}

#endif
