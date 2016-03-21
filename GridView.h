/* Copyright (c) 1998 Slingshot Game Technology

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#if !defined(AFX_GRIDVIEW_H__57AE2504_CF32_11D1_83C5_98E1A892656D__INCLUDED_)
#define AFX_GRIDVIEW_H__57AE2504_CF32_11D1_83C5_98E1A892656D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GridView.h : header file
//


#include "Grid.h"


class CTerrainDoc;


const int	GRIDVIEW_MAX_CAPTURE_POINTS = 200;


/////////////////////////////////////////////////////////////////////////////
// GridView view

class GridView : public CScrollView
{
protected:
	GridView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(GridView)

// Attributes
public:

// Operations
public:
	CTerrainDoc* GetDocument();
	void ChangeHeight(int x, int z, float Delta);
	Grid*	GetSelectedGrid() { return SelectedGrid; }
	void	SetSelectedGrid(Grid* Selected);

	void	OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void	CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GridView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~GridView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(GridView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnExcavate();
	afx_msg void OnGridDoubleHeight();
	afx_msg void OnGridHalveHeight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	float ExcavateWidth;
	float ExcavateDepth;
	float ScratchData[MAX_GRID_SIZE][MAX_GRID_SIZE];
	void ApplyStroke();
	Grid*	SelectedGrid;
	
	enum { NONE = 0, LEFT_BUTTON, RIGHT_BUTTON }	CaptureMode;
	int	CapturePointCount;
	POINT	CapturePoint[GRIDVIEW_MAX_CAPTURE_POINTS];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDVIEW_H__57AE2504_CF32_11D1_83C5_98E1A892656D__INCLUDED_)
