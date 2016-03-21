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
// PaintParametersView.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "PaintParametersView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PaintParametersView property page

IMPLEMENT_DYNCREATE(PaintParametersView, CPropertyPage)

PaintParametersView::PaintParametersView() : CPropertyPage(PaintParametersView::IDD)
{
	//{{AFX_DATA_INIT(PaintParametersView)
	m_PaintRadius = 0.0f;
	//}}AFX_DATA_INIT
}

PaintParametersView::~PaintParametersView()
{
}

void PaintParametersView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PaintParametersView)
	DDX_Control(pDX, IDC_PALETTE, m_Palette);
	DDX_Text(pDX, IDC_PAINT_RADIUS, m_PaintRadius);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PaintParametersView, CPropertyPage)
	//{{AFX_MSG_MAP(PaintParametersView)
	ON_WM_CANCELMODE()
	ON_WM_PAINT()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_EN_CHANGE(IDC_PAINT_RADIUS, OnChangePaintRadius)
	ON_BN_CLICKED(IDC_SETRADIUS, OnSetradius)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PaintParametersView message handlers

BOOL PaintParametersView::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	Update();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void PaintParametersView::OnCancelMode() 
{
	CPropertyPage::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}


void PaintParametersView::Update()
// Make sure all controls reflect the values of our params.
{
	m_PaintRadius = Params.Radius;

	UpdateData(FALSE);	// Send data to controls.
}


void	PaintParametersView::GetPaletteRect(RECT* r)
// Returns the client coordinates of the palette rectangle.
{
	RECT	p;
	this->GetWindowRect(&p);
	m_Palette.GetWindowRect(r);
	r->left -= p.left;
	r->top -= p.top;
	r->right -= p.left;
	r->bottom -= p.top;
}


void PaintParametersView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	// Paint the type palette.
	RECT	r;
	GetPaletteRect(&r);
	int	BlockWidth = (r.right - r.left) / 4;
	int	BlockHeight = (r.bottom - r.top) / 4;
	int	x, z;
	for (z = 0; z < 4; z++) {
		for (x = 0; x < 4; x++) {
			unsigned char	t = (x + z * 4);
			dc.FillSolidRect(r.left + x * BlockWidth, r.top + z * BlockHeight, BlockWidth, BlockHeight, TypeColor(t));
			// Label the swatch.
			RECT	tr;
			tr.left = r.left + x * BlockWidth + 2;
			tr.top = r.top + z * BlockHeight + 6;
			tr.right = tr.left + BlockWidth - 4;
			tr.bottom = tr.top + BlockWidth - 12;
			dc.DrawText(TypeName(t), -1, &tr, DT_SINGLELINE | DT_TOP | DT_CENTER);

			if (Params.LeftButtonType == t) {
				// Draw an 'L' in the block to show it's the left mouse button type.
//				RECT	tr;
				tr.left = r.left + x * BlockWidth + 8;
//				tr.top = r.top + z * BlockHeight + 6;
				tr.right = tr.left + BlockWidth - 16;
//				tr.bottom = tr.top + BlockWidth - 12;
				dc.DrawText("L", -1, &tr, DT_SINGLELINE | DT_BOTTOM | DT_LEFT);
			}
			if (Params.RightButtonType == t) {
				// Draw an 'R' to show that this block represents the right mouse button type.
///				RECT	tr;
				tr.left = r.left + x * BlockWidth + 8;
//				tr.top = r.top + z * BlockHeight + 6;
				tr.right = tr.left + BlockWidth - 16;
//				tr.bottom = tr.top + BlockWidth - 12;
				dc.DrawText("R", -1, &tr, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
			}
		}
	}
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}


void PaintParametersView::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CPropertyPage::OnCaptureChanged(pWnd);
}


void PaintParametersView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int	t = ClassifyPalettePoint(point);
	if (t >= 0) {
		Params.LeftButtonType = t;
		// Repaint the palette.
		RECT	r;
		GetPaletteRect(&r);
		InvalidateRect(&r, FALSE);
	}
	
	CPropertyPage::OnLButtonDown(nFlags, point);
}


void PaintParametersView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int	t = ClassifyPalettePoint(point);
	if (t >= 0) {
		Params.RightButtonType = t;
		// Repaint the palette.
		RECT	r;
		GetPaletteRect(&r);
		InvalidateRect(&r, FALSE);
	}
	
	
	CPropertyPage::OnRButtonDown(nFlags, point);
}


int	PaintParametersView::ClassifyPalettePoint(CPoint p)
// Given a point in our window, return the terrain type of the part of the
// palette that the point is in.  If the point is outside the palette,
// return -1.
{
	// Paint the type palette.
	RECT	r;
	GetPaletteRect(&r);
	int	BlockWidth = (r.right - r.left) / 4;
	int	BlockHeight = (r.bottom - r.top) / 4;
	int	x = (p.x - r.left) / BlockWidth;
	int	y = (p.y - r.top) / BlockHeight;

	if (x >= 0 && x < 4 && y >= 0 && y < 4) {
		return (x + y * 4);
	} else {
		return -1;
	}
}


void PaintParametersView::OnChangePaintRadius()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
//xxx	UpdateData(TRUE);	//???
//	Params.Radius = m_PaintRadius;
}

void PaintParametersView::OnSetradius() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	// Get data from control.
	Params.Radius = m_PaintRadius;
}
