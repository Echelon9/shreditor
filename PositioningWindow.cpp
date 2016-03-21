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
// PositioningWindow.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "PositioningWindow.h"
#include "TerrainDoc.h"
#include "ChildFrm.h"
#include "ToolPalette.h"


const int	POSITIONING_WINDOW_SIZE = 400;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PositioningWindow

IMPLEMENT_DYNCREATE(PositioningWindow, CScrollView)

PositioningWindow::PositioningWindow()
{
	ViewCenterX = 0;
	ViewCenterZ = 0;
}

PositioningWindow::~PositioningWindow()
{
}


BEGIN_MESSAGE_MAP(PositioningWindow, CScrollView)
	//{{AFX_MSG_MAP(PositioningWindow)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PositioningWindow drawing

void PositioningWindow::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

//	CSize sizeTotal;
//	// TODO: calculate the total size of this view
//	sizeTotal.cx = sizeTotal.cy = 100;
//	SetScrollSizes(MM_TEXT, sizeTotal);

//	OnUpdate(CView* sender, LPARAM lHint, CObject* pHint);
}


void PositioningWindow::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	// Draw the outline of the terrain boundaries.
	DrawBox(pDC, 0, 0, POSITIONING_WINDOW_SIZE, POSITIONING_WINDOW_SIZE, RGB(0, 0, 0));

	// Draw the view center.
	int	x = (ViewCenterX * POSITIONING_WINDOW_SIZE) / GetDocument()->GetXSize();
	int	y = (ViewCenterZ * POSITIONING_WINDOW_SIZE) / GetDocument()->GetZSize();
	pDC->SetPixel(x, y, RGB(0, 0, 200));
	pDC->SetPixel(x+1, y, RGB(0, 0, 200));
	pDC->SetPixel(x, y+1, RGB(0, 0, 200));
	pDC->SetPixel(x-1, y, RGB(0, 0, 200));
	pDC->SetPixel(x, y-1, RGB(0, 0, 200));
}


CTerrainDoc*	PositioningWindow::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTerrainDoc)));
	return (CTerrainDoc*)m_pDocument;
}


CChildFrame*	PositioningWindow::GetFrame()
// Gets our frame.  You can use the frame to get pointers to the various
// views and the tool palette.
{
	return dynamic_cast<CChildFrame*>(GetParent()->GetParent()->GetParent());
}


int	PositioningWindow::GetViewXOrigin()
// Returns the x-location (in world coordinates) of the left edge of
// the main terrain view.
{
	CChildFrame* frame = GetFrame();
	if (frame == NULL) return 0;
	ToolPalette* palette = frame->GetToolPalette();
	if (palette == NULL) return 0;

	int	x = ViewCenterX - (/* half width of view window in w.c.*/ 50 << palette->GetZoomLevel());
	if (x < 0) x = 0;

	return x;
}


int	PositioningWindow::GetViewZOrigin()
// Returns the z-location (in world coordinates) of the left edge of
// the main terrain view.
{
	CChildFrame* frame = GetFrame();
	if (frame == NULL) return 0;
	ToolPalette* palette = frame->GetToolPalette();
	if (palette == NULL) return 0;

	int	z = ViewCenterZ - (/* half height of view window in w.c.*/ 50 << palette->GetZoomLevel());
	if (z < 0) z = 0;

	return z;
}


/////////////////////////////////////////////////////////////////////////////
// PositioningWindow diagnostics

#ifdef _DEBUG
void PositioningWindow::AssertValid() const
{
	CScrollView::AssertValid();
}

void PositioningWindow::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// PositioningWindow message handlers


void PositioningWindow::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
// Called when document changes.
{
//	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();
//	int	XSize = (((GetDocument()->GetXSize() - 1) * BlockPixels) >> ZoomLevel) + 1;
//	int	YSize = (((GetDocument()->GetZSize() - 1) * BlockPixels) >> ZoomLevel) + 1;
	int	XSize = POSITIONING_WINDOW_SIZE;
	int	YSize = POSITIONING_WINDOW_SIZE;

	SetScrollSizes(MM_TEXT, CSize(XSize, YSize));

	// Call the base class.
	CScrollView::OnUpdate(pSender, lHint, pHint);
}


void PositioningWindow::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CScrollView::OnLButtonDown(nFlags, point);

	point += GetDeviceScrollPosition();

	// Convert from window coords to world coords.

	ViewCenterX = point.x * GetDocument()->GetXSize() / POSITIONING_WINDOW_SIZE;
	ViewCenterZ = point.y * GetDocument()->GetZSize() / POSITIONING_WINDOW_SIZE;

	// Tell the view to update.
	GetDocument()->UpdateAllViews(NULL);
}


void PositioningWindow::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CScrollView::OnMouseMove(nFlags, point);

	if (nFlags & MK_LBUTTON) {
		point += GetDeviceScrollPosition();
		
		// Convert from window coords to world coords.
		
		ViewCenterX = point.x * GetDocument()->GetXSize() / POSITIONING_WINDOW_SIZE;
		ViewCenterZ = point.y * GetDocument()->GetZSize() / POSITIONING_WINDOW_SIZE;
		
		// Tell the view to update.
		GetDocument()->UpdateAllViews(NULL);
	}
}


void PositioningWindow::OnCancelMode() 
{
	CScrollView::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}
