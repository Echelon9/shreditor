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
// GeometryListView.cpp : implementation file
//


#include "stdafx.h"
#include "terrain.h"
#include "terraindoc.h"
#include "GeometryListView.h"
#include "geometry.h"
#include "GeometryPropertiesDialog.h"
#include "ChildFrm.h"
#include "onfirmDeleteDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GeometryListView

//IMPLEMENT_DYNCREATE(GeometryListView, CFormView)

GeometryListView::GeometryListView()
	: CPropertyPage(GeometryListView::IDD)
{
	//{{AFX_DATA_INIT(GeometryListView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	Frame = NULL;
}

GeometryListView::~GeometryListView()
{
}

void GeometryListView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GeometryListView)
	DDX_Control(pDX, IDC_ADD_NEW_GEOMETRY, m_AddNewGeometry);
	DDX_Control(pDX, IDC_GEOMETRYLIST, m_ListControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GeometryListView, CPropertyPage)
	//{{AFX_MSG_MAP(GeometryListView)
	ON_BN_CLICKED(IDC_ADD_NEW_GEOMETRY, OnAddNewGeometry)
	ON_BN_CLICKED(IDC_EDIT_GEOMETRY, OnEditGeometry)
	ON_BN_CLICKED(IDC_DELETE_GEOMETRY, OnDeleteGeometry)
	ON_WM_CANCELMODE()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GeometryListView diagnostics

#ifdef _DEBUG
void GeometryListView::AssertValid() const
{
	CPropertyPage::AssertValid();
}

void GeometryListView::Dump(CDumpContext& dc) const
{
	CPropertyPage::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// GeometryListView message handlers

void GeometryListView::Update() 
// Rebuild the list of geometry resources.
{
//	CPropertyPage::OnUpdate(pSender, lHint, pHint);

	// TODO: Add extra initialization here
	CTerrainDoc*	doc = GetDocument();
	Geometry*	g = doc->GetGeometryList();
	int	GeometryCount = doc->GetGeometryCount();
	CListCtrl*	list = &m_ListControl /* (CListCtrl*) GetDlgItem(IDC_GEOMETRYLIST) */;
	if (list->m_hWnd == NULL) return;

	// Setup the column headings.
	while (list->DeleteColumn(0)) ;

	LV_COLUMN	col;
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;

	// Name.
	col.pszText = "Name";
	col.iSubItem = 0;
	col.cx = 75;
	list->InsertColumn(0, &col);

	// Filename.
	col.iSubItem = 1;
	col.pszText = "Filename";
	col.cx = 100;
	list->InsertColumn(1, &col);

	// Comment.
	col.iSubItem = 2;
	col.pszText = "Comment";
	col.cx = 170;
	list->InsertColumn(2, &col);

	// Set the list items.
	list->DeleteAllItems();
	list->SetItemCount(GeometryCount);
	for (int i = 0; i < GeometryCount; i++, g = g->Next) {
		if (g == NULL) break;

		int index = list->InsertItem(i, g->GetName());
		list->SetItemText(index, 1, g->GetFilename());
		list->SetItemText(index, 2, g->GetComment());
		list->SetItemData(index, i);
	}
}


void GeometryListView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::CalcWindowRect(lpClientRect, nAdjustType);
}


LRESULT GeometryListView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::DefWindowProc(message, wParam, lParam);
}


void GeometryListView::OnAddNewGeometry() 
// Called when user clicks "New Geometry" button.
{
	// Call the properties dialog box with some default initial values.
	GeometryPropertiesDialog d;
	d.SetName("Untitled");
	d.SetFilename("Unknown");
	d.SetComment("");

	if (d.DoModal() != IDCANCEL) {
		// Create a new geometry item with the new values, and add it to our list.
		Geometry*	g = new Geometry;

		g->SetName(d.GetName());
		g->SetFilename(d.GetFilename());
		g->SetComment(d.GetComment());

		GetDocument()->AddGeometry(g);
		GetDocument()->SetModifiedFlag();

		GetDocument()->UpdateAllViews(NULL);
	}
}


void GeometryListView::OnEditGeometry() 
// Called when user clicks "Edit" in the geometry list.
{
	// Figure out which geometry item to edit.
	int	index = GetSelectedItemIndex();
	if (index < 0) {
		// No item selected, so we don't know what to delete...
		return;
	}
	if (index == 0) {
		// Can't edit null geometry.
		return;
	}

	// Get the geometry object to edit.
	Geometry*	g = GetDocument()->GetGeometry(index);
	if (g == NULL) return;

	// Call edit dialog box.
	GeometryPropertiesDialog d;
	d.SetName(g->GetName());
	d.SetFilename(g->GetFilename()); 
	d.SetComment(g->GetComment());

	if (d.DoModal() != IDCANCEL) {
		// Apply the changes.
		g->SetName(d.GetName());
		g->SetFilename(d.GetFilename());
		g->SetComment(d.GetComment());

		GetDocument()->SetModifiedFlag();
		GetDocument()->UpdateAllViews(NULL);
	}
}


void GeometryListView::OnDeleteGeometry() 
// Called when the user clicks "Delete" button in the geometry list dialog.
{
	// Figure out which geometry item to edit.
	int	index = GetSelectedItemIndex();
	if (index < 0) {
		// No item selected, so we don't know what to delete...
		return;
	}
	if (index == 0) {
		// Can't delete geometry 0 (the null geometry).
		return;
	}

	CTerrainDoc*	doc = GetDocument();

	// Check to see if any objects are using this geometry...
	// bool InUse = doc->GeometryInUse(index);
	// if (InUse) {
	//	put up a dialog, noting that the object is in use, asking if the user's sure;
	//	if (DoModal() == IDCANCEL) {
	//		return;
	//	}
	// }
	// else
	{
		// Make the user confirm.
		ConfirmDeleteDialog	d(doc->GetGeometry(index)->GetName());
		if (d.DoModal() == IDCANCEL) {
			return;
		}
	}

	// Delete the item and update views.
	doc->DeleteGeometry(index);
	doc->SetModifiedFlag();
	doc->UpdateAllViews(NULL);
}


int GeometryListView::GetSelectedItemIndex()
// Returns the index of the geometry item that's selected.
// Returns -1 if it can't find a selected item.
{
	int	ItemCount = m_ListControl.GetItemCount();
	for (int i = 0; i < ItemCount; i++) {
		UINT	state = m_ListControl.GetItemState(i, LVIS_SELECTED | LVIS_FOCUSED);
		if (state & LVIS_FOCUSED) {
			return m_ListControl.GetItemData(i);
		}
	}

	return -1;
}


CTerrainDoc*	GeometryListView::GetDocument()
{
	return GetFrame()->GetDocument();
}

BOOL GeometryListView::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	Update();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void GeometryListView::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CPropertyPage::OnCaptureChanged(pWnd);
}
