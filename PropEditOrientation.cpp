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
// PropEditOrientation.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "PropEditOrientation.h"

#include <math.h>

#define PI 3.1415926535


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropEditOrientation dialog


PropEditOrientation::PropEditOrientation(CWnd* pParent /*=NULL*/)
	: CDialog(PropEditOrientation::IDD, pParent)
{
	//{{AFX_DATA_INIT(PropEditOrientation)
	m_Yaw = 0.0f;
	//}}AFX_DATA_INIT
}


void PropEditOrientation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropEditOrientation)
	DDX_Text(pDX, IDC_YAW, m_Yaw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropEditOrientation, CDialog)
	//{{AFX_MSG_MAP(PropEditOrientation)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropEditOrientation message handlers


void PropEditOrientation::DrawTickMark(CDC * dc)
{
	// Erase the area that should contain the tick mark.
	RECT	r;
	r.left = 20;
	r.top = 20;
	r.right = r.left + 50;
	r.bottom = r.top + 50;
//	dc->FillRect(&r, (CBrush*) GetStockObject(GRAY_BRUSH));

	CPen*	OldPen = dc->SelectObject((CPen*) GetStockObject(BLACK_PEN));

	// Draw a circle.
	dc->Ellipse(&r);

	// Draw a tick mark.
	float	cosine = cos(m_Yaw * PI / 180);
	float	sine = sin(m_Yaw * PI / 180);
	int	cx = r.left + (r.right + r.left) / 2;
	int	cy = r.top + (r.bottom + r.top) / 2;
	int	x0 = cx + cosine * 45;
	int	y0 = cy + sine * 45;
	int	x1 = cx + cosine * 50;
	int	y1 = cy + sine * 50;
	dc->MoveTo(x0, y0);
	dc->LineTo(x1, y1);

	dc->SelectObject(OldPen);
}


void PropEditOrientation::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	DrawTickMark(&dc);
	
	// Do not call CDialog::OnPaint() for painting messages
}
