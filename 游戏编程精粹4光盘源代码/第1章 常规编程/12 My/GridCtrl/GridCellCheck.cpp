// GridCellCheck.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for a combobox cell type of the
// grid control.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// Parts of the code contained in this file are based on the original
// CInPlaceList from http://www.codeguru.com/listview
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.22+
//
// History:
// 23 Jul 2001 - Complete rewrite
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCell.h"
#include "GridCtrl.h"

#include "GridCellCheck.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGridCellCheck, CGridCell)

CGridCellCheck::CGridCellCheck() : CGridCell()
{
    m_bChecked = FALSE;
    //m_Rect.IsRectNull();
}

CSize CGridCellCheck::GetCellExtent(CDC* pDC)
{
    // Using SM_CXHSCROLL as a guide to the size of the checkbox
	int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();
	return CGridCell::GetCellExtent(pDC) + CSize(nWidth, nWidth);
}

// i/o:  i=dims of cell rect; o=dims of text rect
BOOL CGridCellCheck::GetTextRect( LPRECT pRect)
{
    BOOL bResult = CGridCell::GetTextRect(pRect);
    if (bResult)
    {
        int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();
        pRect->left += nWidth;
        if (pRect->left > pRect->right)
            pRect->left = pRect->right;
    }
    return bResult;
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellCheck::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

#ifndef _WIN32_WCE
    // Store the cell's dimensions for later
    m_Rect = rect;

    CRect CheckRect = GetCheckPlacement();
    rect.left = CheckRect.right;

    // enough room to draw?
    // if (CheckRect.Width() < rect.Width() && CheckRect.Height() < rect.Height()) {

    // Do the draw 
    pDC->DrawFrameControl(GetCheckPlacement(), DFC_BUTTON, 
	    (m_bChecked)? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK);

    // }
#endif
    return bResult;
}

void CGridCellCheck::OnClick(CPoint PointCellRelative)
{
	// PointCellRelative is relative to the topleft of the cell. Convert to client coords
	PointCellRelative += m_Rect.TopLeft();

	// GetCheckPlacement returns the checkbox dimensions in client coords. Only check/
	// uncheck if the user clicked in the box
	if (GetCheckPlacement().PtInRect(PointCellRelative))
	{
		m_bChecked = !m_bChecked;
		GetGrid()->InvalidateRect(m_Rect);
	}
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

BOOL CGridCellCheck::SetCheck(BOOL bChecked /*=TRUE*/)
{
	BOOL bTemp = m_bChecked;
	m_bChecked = bChecked;
	if (!m_Rect.IsRectEmpty())
		GetGrid()->InvalidateRect(m_Rect);

	return bTemp;
}

BOOL CGridCellCheck::GetCheck()
{
	return m_bChecked;
}

//////////////////////////////////////////////////////////////////////
// Protected implementation
//////////////////////////////////////////////////////////////////////

// Returns the dimensions and placement of the checkbox in client coords.
CRect CGridCellCheck::GetCheckPlacement()
{
	int nWidth = GetSystemMetrics(SM_CXHSCROLL);
	CRect place = m_Rect + CSize(GetMargin(), GetMargin());
    place.right = place.left + nWidth;
    place.bottom = place.top + nWidth;

	/* for centering
	int nDiff = (place.Width() - nWidth)/2;
	if (nDiff > 0)
	{
		place.left += nDiff;
		place.right = place.left + nWidth;
	}
	nDiff = (place.Height() - nWidth)/2;
	if (nDiff > 0)
	{
		place.top += nDiff;
		place.bottom = place.top + nWidth;
	}
    */

	return place;
}