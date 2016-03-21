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
// ToolPalette.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "ToolPalette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MAX_ZOOM	10


/////////////////////////////////////////////////////////////////////////////
// ToolPalette

IMPLEMENT_DYNCREATE(ToolPalette, CFormView)

ToolPalette::ToolPalette()
	: CFormView(ToolPalette::IDD), PlantParams()
{
	//{{AFX_DATA_INIT(ToolPalette)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

ToolPalette::~ToolPalette()
{
}

void ToolPalette::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ToolPalette)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ToolPalette, CFormView)
	//{{AFX_MSG_MAP(ToolPalette)
	ON_BN_CLICKED(IDC_MOVEGRID, OnToolSelect)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_DRAW_ALL_GRIDS, OnDrawAllGrids)
	ON_BN_CLICKED(IDC_DRAW_OBJECTS, OnDrawObjects)
	ON_BN_CLICKED(IDC_DRAW_SELECTED_GRID, OnDrawSelectedGrid)
	ON_BN_CLICKED(IDC_DRAW_SURFACE, OnDrawSurface)
	ON_BN_CLICKED(IDC_MOVEOBJECT, OnToolSelect)
	ON_BN_CLICKED(IDC_PLANTOBJECT, OnToolSelect)
	ON_BN_CLICKED(IDC_CLEANUP_QUADTREE, OnCleanupQuadTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ToolPalette diagnostics

#ifdef _DEBUG
void ToolPalette::AssertValid() const
{
	CFormView::AssertValid();
}

void ToolPalette::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ToolPalette message handlers

void ToolPalette::OnToolSelect() 
{
	// TODO: Add your control notification handler code here
}


ToolPalette::ToolType	ToolPalette::GetCurrentTool()
// Returns the currently selected tool.
{
	ToolType	tool = GRIDTOOL;

	// Query the dialog buttons...

	CButton* b = (CButton*) GetDlgItem(IDC_GRIDTOOL);
	if (b->GetState() & 1) {
		// "Grid Tool" is selected.
		tool = GRIDTOOL;
	}

	b = (CButton*) GetDlgItem(IDC_OBJECTTOOL);
	if (b->GetState() & 1) {
		// "Object Tool" is selected.
		tool = OBJECTTOOL;
	}

	b = (CButton*) GetDlgItem(IDC_EXCAVATE);
	if (b->GetState() & 1) {
		// "Excavate" is selected.
		tool = EXCAVATE;
	}

	b = (CButton*) GetDlgItem(IDC_PAINTSURFACE);
	if (b->GetState() & 1) {
		tool = PAINTSURFACE;
	}

	b = (CButton*) GetDlgItem(IDC_PLANTOBJECTS);
	if (b->GetState() & 1) {
		// "Plant Objects" tool is selected.
		tool = PLANTOBJECTS;
	}

	b = (CButton*) GetDlgItem(IDC_VERTEX);
	if (b->GetState() & 1) {
		// "Vertex Tool" is selected.
		tool = VERTEXTOOL;
	}

	b = (CButton*) GetDlgItem(IDC_HAND);
	if (b->GetState() & 1) {
		// "Hand" is selected.
		tool = HAND;
	}

	return tool;
}


float	ToolPalette::GetContourInterval()
// Returns the height interval, in meters, at which to draw contour lines.
// This value is controlled by a slider.
{
	CSliderCtrl* s = (CSliderCtrl*) /*dynamic_cast<CSliderCtrl*>*/(GetDlgItem(IDC_CONTOURINTERVAL));
	if (s) {
		int ContourIndex = s->GetPos();

		// Don't allow small intervals if we're zoomed way out.
		int Limit = (GetZoomLevel() - 2) * 2.5;
		if (ContourIndex < Limit) ContourIndex = Limit;

		float	c = (ContourIndex + 4) / (40.0 + 4.0);
		return 40 * c * c;
	}

	return 2.0;
}


int	ToolPalette::GetZoomLevel()
// Returns the zoom level, from 0 to ...
// The zoom level is the log2 of the scale factor.
// This value is controlled by a slider.
{
	CSliderCtrl* s = (CSliderCtrl*) (GetDlgItem(IDC_ZOOM));
	if (s) {
		return s->GetPos();
	}

	return 2.0;
}


void	ToolPalette::SetZoomLevel(int NewLevel)
// Sets the zoom level to the given value (clamped within limits).
{
	CSliderCtrl* s = (CSliderCtrl*) (GetDlgItem(IDC_ZOOM));
	if (s) {
		s->SetPos(NewLevel);
	}
}


bool	ToolPalette::GetDrawSelectedGrid()
// Returns true if the "Draw Selected Grids" option is checked.
{
	CButton*	c = (CButton*) (GetDlgItem(IDC_DRAW_SELECTED_GRID));
	if (c) {
		return c->GetCheck();
	}

	return true;
}


bool	ToolPalette::GetDrawAllGrids()
// Returns true if the "Draw All Grids" option is checked.
{
	CButton*	c = (CButton*) (GetDlgItem(IDC_DRAW_ALL_GRIDS));
	if (c) {
		return c->GetCheck();
	}

	return true;
}


bool	ToolPalette::GetDrawObjects()
// Returns true if the "Draw Objects" option is checked.
{
	CButton*	c = (CButton*) (GetDlgItem(IDC_DRAW_OBJECTS));
	if (c) {
		return c->GetCheck();
	}

	return true;
}


bool	ToolPalette::GetDrawSurface()
// Returns true if the "Draw Surface" option is checked.
{
	CButton*	c = (CButton*) (GetDlgItem(IDC_DRAW_SURFACE));
	if (c) {
		return c->GetCheck();
	}

	return true;
}


bool	ToolPalette::GetShowCursor()
// Returns true if the "Show Tool Cursor" option is checked.
{
	CButton*	c = (CButton*) (GetDlgItem(IDC_SHOW_TOOL_CURSOR));
	if (c) {
		return c->GetCheck();
	}

	return true;
}


void ToolPalette::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	// Set up the sliders.
	CSliderCtrl*	s = (CSliderCtrl*) (GetDlgItem(IDC_ZOOM));
	if (s) {
		s->SetRange(0, MAX_ZOOM, TRUE);
		s->SetPos(MAX_ZOOM - 1);
	}
	

	s = (CSliderCtrl*) (GetDlgItem(IDC_CONTOURINTERVAL));
	if (s) {
		s->SetRange(1, 40, TRUE);
		s->SetPos(40);
	}

	// Set the "Hand" as the current tool.
	CButton*	b = (CButton*) GetDlgItem(IDC_HAND);
	if (b) b->SetCheck(1);
}


void ToolPalette::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::CalcWindowRect(lpClientRect, nAdjustType);
}


CTerrainDoc* ToolPalette::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTerrainDoc)));
	return (CTerrainDoc*) m_pDocument;
}


BOOL ToolPalette::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void ToolPalette::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::OnPrepareDC(pDC, pInfo);
}


void ToolPalette::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);

	CSliderCtrl*	s = (CSliderCtrl*) pScrollBar;
	if (
		((CSliderCtrl*) (GetDlgItem(IDC_ZOOM)) == s ||
		(CSliderCtrl*) (GetDlgItem(IDC_CONTOURINTERVAL)) == s))
	{
		// User moved one of the sliders.
		if (nSBCode == SB_ENDSCROLL) {
			// User is done moving, so we can redraw all the views.
			GetDocument()->UpdateAllViews(NULL);
		} else {
			// User is busy moving.  Redraw the slider numbers only.
			UpdateSliderNumbers();
		}
	}
}


void ToolPalette::OnDrawAllGrids() 
{
	// Be sure to update view.
	GetDocument()->UpdateAllViews(NULL);
}

void ToolPalette::OnDrawObjects() 
{
	// Be sure to update view.
	GetDocument()->UpdateAllViews(NULL);
}

void ToolPalette::OnDrawSelectedGrid() 
{
	// Be sure to update view.
	GetDocument()->UpdateAllViews(NULL);
}

void ToolPalette::OnDrawSurface() 
{
	// Be sure to update view.
	GetDocument()->UpdateAllViews(NULL);
}


void ToolPalette::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UpdateSliderNumbers();
}


void ToolPalette::UpdateSliderNumbers()
// Update the numbers beneath the sliders, so that they reflect the
// slider positions.
{
	char	buf[80];

	// Zoom number.
	CWnd*	c = (CWnd*) GetDlgItem(IDC_ZOOM_NUMBER);
	sprintf(buf, "%d", GetZoomLevel());
	c->SetWindowText(buf);

	// Contour number.
	c = (CWnd*) GetDlgItem(IDC_CONTOUR_NUMBER);
	sprintf(buf, "%g", GetContourInterval());
	c->SetWindowText(buf);

	// Node count.
	c = (CWnd*) GetDlgItem(IDC_NODE_COUNT);
	sprintf(buf, "nodes: %d", GetDocument()->GetQuadTreeNodeCount());
	if (c) c->SetWindowText(buf);
}


void ToolPalette::OnCleanupQuadTree()
// Eliminate useless nodes, and recount the number of nodes in the tree.
{
	GetDocument()->CleanupQuadTree(50);

	// Be sure to update view.
	GetDocument()->UpdateAllViews(NULL);
}
