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
// GridFormView.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "GridFormView.h"
#include "ChildFrm.h"
//#include "GridView.h"
#include "Grid.h"
#include "TerrainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GridFormView

//xxxx IMPLEMENT_DYNCREATE(GridFormView, CPropertyPage)

GridFormView::GridFormView()
	: CPropertyPage(GridFormView::IDD)
{
	//{{AFX_DATA_INIT(GridFormView)
	m_GridXSize = 1;
	m_GridXOrigin = 0;
	m_GridZOrigin = 0;
	m_GridZSize = 1;
	//}}AFX_DATA_INIT

	Frame = NULL;
	Document = NULL;
//	m_GridScale.Create(TBS_HORZ | TBS_AUTOTICKS, CRect(0, 0, 100, 10), (CWnd*) this, IDD_FORMVIEW);
//	m_GridScale.SetRange(1, 10);	// The scale is in powers of two.
}

GridFormView::~GridFormView()
{
}

void GridFormView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(GridFormView)
	DDX_Control(pDX, IDC_GRIDSCALE, m_GridScale);
	DDX_Text(pDX, IDC_GRIDXSIZE, m_GridXSize);
	DDV_MinMaxInt(pDX, m_GridXSize, 1, 1024);
	DDX_Text(pDX, IDC_GRIDXORIGIN, m_GridXOrigin);
	DDV_MinMaxInt(pDX, m_GridXOrigin, 0, 65535);
	DDX_Text(pDX, IDC_GRIDZORIGIN, m_GridZOrigin);
	DDV_MinMaxInt(pDX, m_GridZOrigin, 0, 65535);
	DDX_Text(pDX, IDC_GRIDZSIZE, m_GridZSize);
	DDV_MinMaxInt(pDX, m_GridZSize, 1, 1024);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GridFormView, CPropertyPage)
	//{{AFX_MSG_MAP(GridFormView)
	ON_EN_CHANGE(IDC_GRIDXORIGIN, OnDataChange)
	ON_BN_CLICKED(IDC_APPLYGRIDFORM, OnApplyButton)
	ON_BN_CLICKED(IDC_READGRIDFORM, OnReadButton)
	ON_EN_CHANGE(IDC_GRIDZORIGIN, OnDataChange)
	ON_EN_CHANGE(IDC_GRIDXSIZE, OnDataChange)
	ON_EN_CHANGE(IDC_GRIDZSIZE, OnDataChange)
	ON_BN_CLICKED(IDC_PSD_EXPORT, OnPsdExport)
	ON_BN_CLICKED(IDC_PSD_IMPORT, OnPsdImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GridFormView diagnostics

#ifdef _DEBUG
void GridFormView::AssertValid() const
{
	CPropertyPage::AssertValid();
}

void GridFormView::Dump(CDumpContext& dc) const
{
	CPropertyPage::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// GridFormView message handlers

void GridFormView::OnDataChange() 
{
}


void GridFormView::OnApplyButton() 
{
	// TODO: Add your control notification handler code here
	
	// Get the data from the controls into our member variables.
	if (!UpdateData()) {
		return;
	}

	CTerrainDoc* Doc = GetDocument();
	Grid*	g = GetSelectedGrid();

	if (g == NULL) return;

	// Apply the new values, if they're different.
	BOOL Changed = FALSE;

	int	Scale = m_GridScale.GetPos();	// xxxx
	if (Scale != g->GetScale()) {
		g->SetScale(Scale);
		Changed = TRUE;
	}

	if ((m_GridXOrigin >> Scale) != g->GetXOrigin()) {
		g->SetXOrigin(m_GridXOrigin >> Scale);
		Changed = TRUE;
	}

	if ((m_GridZOrigin >> Scale) != g->GetZOrigin()) {
		g->SetZOrigin(m_GridZOrigin >> Scale);
		Changed = TRUE;
	}

	if (m_GridXSize != g->GetXSize() || m_GridZSize != g->GetZSize()) {
		g->Resize(m_GridXSize, m_GridZSize);
		Changed = TRUE;
	}

	if (Changed && Doc) {
		// Notify the document that this grid may need to be re-sorted.
		Doc->ReSortGrid(g);

		// Mark the document as modified, and refresh the view.
		Doc->SetModifiedFlag();
		Doc->UpdateAllViews(NULL);
	}
}

void GridFormView::OnReadButton() 
{
	ReadGridValues();
}


Grid* GridFormView::GetSelectedGrid()
// Returns the currently active grid, if there is one; otherwise returns
// NULL.
{
//	Grid*	g = NULL;
//
//	CChildFrame* parent = static_cast<CChildFrame*>(GetParent()->GetParent()->GetParent());	//xxx yikes.  Could use some assertions, dynamic_cast<>, something safer.
//	if (parent) {
//		GridView* v = parent->GetGridView();
//		if (v) {
//			g = v->GetSelectedGrid();
//		}
//	}
//
//	return g;

	return GetFrame()->GetTerrainView()->GetSelectedGrid();
}

void GridFormView::ReadGridValues()
// If there is a selected grid, then reads its values into the form controls.
{
	// Copy the values from the selected grid to our member variables.
	Grid*	g = GetSelectedGrid();

	if (g == NULL) return;

	m_GridXOrigin = g->GetXOrigin() << g->GetScale();
	m_GridZOrigin = g->GetZOrigin() << g->GetScale();
	m_GridXSize = g->GetXSize();
	m_GridZSize = g->GetZSize();
	m_GridScale.SetPos(g->GetScale());

	// Put the values from our member variables into the form controls.
	UpdateData(FALSE);
}

void GridFormView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::CalcWindowRect(lpClientRect, nAdjustType);
}


BOOL GridFormView::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add your specialized code here and/or call the base class
	m_GridScale.SetRange(0, 9);	// The scale is in powers of two.

	return TRUE;
}


void GridFormView::OnPsdExport() 
// Write grid data to "tmpgrid.psd".
{
	Grid*	g = GetSelectedGrid();
	if (g == NULL) return;

	Bitmap::WriteImage32FromTypeMap("tmpgrid.psd", g->GetTypeData(), g->GetXSize(), g->GetZSize());
}

void GridFormView::OnPsdImport() 
// Read grid data from "tmpgrid.psd"
{
	Grid*	g = GetSelectedGrid();
	if (g == NULL) return;

	Bitmap::ReadTypeMapFromImage32("tmpgrid.psd", g->GetTypeData(), g->GetXSize(), g->GetZSize());

	// Mark the document as modified, and refresh the view.
	CTerrainDoc* Doc = GetDocument();
	Doc->SetModifiedFlag();
	Doc->UpdateAllViews(NULL);
}

