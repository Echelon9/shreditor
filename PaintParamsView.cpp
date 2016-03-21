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
// PaintParamsView.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "PaintParamsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PaintParamsView property page

IMPLEMENT_DYNCREATE(PaintParamsView, CPropertyPage)

PaintParamsView::PaintParamsView() : CPropertyPage(PaintParamsView::IDD)
{
	//{{AFX_DATA_INIT(PaintParamsView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

PaintParamsView::~PaintParamsView()
{
}

void PaintParamsView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PaintParamsView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PaintParamsView, CPropertyPage)
	//{{AFX_MSG_MAP(PaintParamsView)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PaintParamsView message handlers
/////////////////////////////////////////////////////////////////////////////
// PaintParamsView property page

IMPLEMENT_DYNCREATE(PaintParamsView, CPropertyPage)

PaintParamsView::PaintParamsView() : CPropertyPage(PaintParamsView::IDD)
{
	//{{AFX_DATA_INIT(PaintParamsView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

PaintParamsView::~PaintParamsView()
{
}

void PaintParamsView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PaintParamsView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PaintParamsView, CPropertyPage)
	//{{AFX_MSG_MAP(PaintParamsView)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PaintParamsView message handlers
