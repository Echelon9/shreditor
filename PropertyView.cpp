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
// PropertyView.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "PropertyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropertyView

IMPLEMENT_DYNCREATE(PropertyView, CView)

PropertyView::PropertyView()
{
	ToolPages = NULL;
}


PropertyView::~PropertyView()
{
	if (ToolPages) delete ToolPages;
}


BEGIN_MESSAGE_MAP(PropertyView, CView)
	//{{AFX_MSG_MAP(PropertyView)
	ON_WM_CREATE()
	ON_WM_CANCELMODE()
	ON_WM_SETFOCUS()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropertyView drawing

void PropertyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	// Pass to ToolPages.
	ToolPages->RedrawWindow();
}

/////////////////////////////////////////////////////////////////////////////
// PropertyView diagnostics

#ifdef _DEBUG
void PropertyView::AssertValid() const
{
	CView::AssertValid();
}

void PropertyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// PropertyView message handlers

void PropertyView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
}


void PropertyView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::CalcWindowRect(lpClientRect, nAdjustType);
}


int PropertyView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	ToolPages = new ToolSheet("Tool Pages", this);
	ToolPages->Create(this, WS_CHILD | WS_VISIBLE, 0);

	// Move the child property sheet so it's in the frame.
	CRect rectClient, rectWindow;
	ToolPages->GetWindowRect(rectClient);
	GetWindowRect(rectWindow);

//	// CMiniFrameWnd::CalcWindowRect adds the extra width and height
//	// needed from the mini frame.
//	CalcWindowRect(rectWindow);

//	SetWindowPos(NULL, rectWindow.left, rectWindow.top,
//		rectWindow.Width(), rectWindow.Height(),
//		SWP_NOZORDER | SWP_NOACTIVATE);


	ToolPages->SetWindowPos(NULL, 0, 0,
		rectClient.Width(), rectClient.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);

	return 0;
}


void PropertyView::OnCancelMode() 
{
	CView::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void PropertyView::OnSetFocus(CWnd* pOldWnd) 
{
//	CView::OnSetFocus(pOldWnd);
//	
//	// TODO: Add your message handler code here

	// Forward focus to the embedded property sheet
	ASSERT_VALID(ToolPages);
	ToolPages->SetFocus();
	
}

void PropertyView::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CView::OnCaptureChanged(pWnd);
}

GridFormView* PropertyView::GetGridFormView()
{
	return &ToolPages->GridPage;
}


GeometryListView* PropertyView::GetGeometryListView()
{
	return &ToolPages->GeoListView;
}


ObjectFormView*	PropertyView::GetObjectFormView()
{
	return &ToolPages->ObjectPage;
}


PolygonFormView*	PropertyView::GetPolygonFormView()
{
	return &ToolPages->PolygonPage;
}


SolidListView*	PropertyView::GetSolidListView()
{
	return &ToolPages->SolidListPage;
}


PaintParametersView*	PropertyView::GetPaintParametersView()
{
	return &ToolPages->PaintPage;
}


void PropertyView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	GetGeometryListView()->Update();
	GetObjectFormView()->Update();
	GetPolygonFormView()->Update();
	GetSolidListView()->Update();
}

BOOL PropertyView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
