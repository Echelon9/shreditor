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
#if !defined(AFX_POSITIONINGWINDOW_H__960D4060_1CA8_11D2_83C5_444553540000__INCLUDED_)
#define AFX_POSITIONINGWINDOW_H__960D4060_1CA8_11D2_83C5_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PositioningWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PositioningWindow view


class CTerrainDoc;
class CChildFrame;


class PositioningWindow : public CScrollView
{
protected:
	PositioningWindow();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(PositioningWindow)

// Attributes
public:

// Operations
public:
	int	GetViewXOrigin();
	int	GetViewZOrigin();

	int	GetViewXCenter() { return ViewCenterX; }
	int	GetViewZCenter() { return ViewCenterZ; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PositioningWindow)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

	CTerrainDoc*	GetDocument();
	CChildFrame*	GetFrame();

// Implementation
protected:
	virtual ~PositioningWindow();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(PositioningWindow)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int	ViewCenterX, ViewCenterZ;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSITIONINGWINDOW_H__960D4060_1CA8_11D2_83C5_444553540000__INCLUDED_)
