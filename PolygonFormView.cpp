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
// PolygonFormView.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "PolygonFormView.h"
#include "Object.h"
#include "ChildFrm.h"
#include "ObjectProperties.h"
#include "polygon.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PolygonFormView property page

//IMPLEMENT_DYNCREATE(PolygonFormView, CPropertyPage)

PolygonFormView::PolygonFormView() : CPropertyPage(PolygonFormView::IDD)
{
	//{{AFX_DATA_INIT(PolygonFormView)
	m_Closed = FALSE;
	//}}AFX_DATA_INIT

	Frame = NULL;
	SingleSelectedPolygon = NULL;

	ClosedValid = false;
}

PolygonFormView::~PolygonFormView()
{
}

void PolygonFormView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PolygonFormView)
	DDX_Control(pDX, IDC_PROPERTIES, m_PropertyList);
	DDX_Check(pDX, IDC_CLOSED, m_Closed);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate == 0) {
		// Sending data to controls.  Empty the control if the field is invalid.

		CButton* b = (CButton*) GetDlgItem(IDC_CLOSED);
		if (b) {
			if (ClosedValid == false) {
				b->SetButtonStyle(BS_AUTO3STATE);
				b->SetCheck(2);
			} else {
				b->SetButtonStyle(BS_AUTOCHECKBOX);
			}
		}
	}
}


BEGIN_MESSAGE_MAP(PolygonFormView, CPropertyPage)
	//{{AFX_MSG_MAP(PolygonFormView)
	ON_BN_CLICKED(IDC_APPLYPOLYPROPERTIES, OnApplyPolyProperties)
	ON_BN_CLICKED(IDC_CLOSED, OnClosed)
	ON_NOTIFY(NM_DBLCLK, IDC_PROPERTIES, OnDblclkProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PolygonFormView message handlers

void PolygonFormView::OnApplyPolyProperties() 
// Called when the user presses the Apply button.
{
	// Get the data from the controls into our member variables.
	if (!UpdateData()) {
		return;
	}

	CTerrainDoc* Doc = GetDocument();
	polygon*	p = NULL;

	// Apply the new values to all selected polygons, if they're different.
	BOOL Changed = FALSE;

	PolygonElem*	PolyList = Doc->GetPolygonList();
	
	PolygonElem*	e = PolyList;
	if (e) {
		do {
			p = e->poly;
			e = e->Next;
			
			if (p->GetSelected() == true) {

				if (ClosedValid == true && bool(m_Closed) != p->GetClosed()) {
					p->SetClosed(bool(m_Closed));
					Changed = TRUE;
				}
			}

		} while (e != PolyList);
	}

	if (Changed && Doc) {
		// Mark the document as modified, and refresh the view.
		Doc->SetModifiedFlag();
		Doc->UpdateAllViews(NULL);
	}
}


void PolygonFormView::Update()
// Called when the document or selection changes.  Grab
// information from the currently selected poly(s).
{
	CTerrainDoc*	doc = GetDocument();
	if (doc == NULL) return;

	polygon*	o = NULL;

	PolygonElem*	PolyList = doc->GetPolygonList();
	PolygonElem*	e = PolyList;
	
	// Find the first selected polygon.
	if (e) {
		do {
			polygon*	p = e->poly;
			e = e->Next;
			
			if (p->GetSelected()) {
				o = p;
				break;
			}
		} while (e != PolyList);
	}

	// If we have a selection, then assume fields are valid.  Compare all other selected objects against
	// the first selected object, and if a field varies, then mark that field
	// as invalid.  The idea is to only show values for the fields that are
	// invariant across the selection.

	SingleSelectedPolygon = o;
	bool	val = o != NULL ? true : false;
	ClosedValid = val;

	if (e) {
		while (e != PolyList) {
			polygon*	p = e->poly;
			e = e->Next;
			
			if (p->GetSelected() == false) continue;

			SingleSelectedPolygon = NULL;

			if (p->GetClosed() != o->GetClosed()) ClosedValid = false;
		}
	}

	if (ClosedValid) m_Closed = o->GetClosed();
	else m_Closed = FALSE;

	//
	// Additional Properties list.
	//

	CListCtrl*	list = &m_PropertyList;
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

	// Type.
	col.iSubItem = 1;
	col.pszText = "Type";
	col.cx = 75;
	list->InsertColumn(1, &col);

	// Value.
	col.iSubItem = 2;
	col.pszText = "Value";
	col.cx = 75;
	list->InsertColumn(2, &col);

	// Set the list items.
	list->DeleteAllItems();
	if (SingleSelectedPolygon) {
		o = SingleSelectedPolygon;
		int	count = GetPolygonPropertyCount();

		list->SetItemCount(count);
		for (int i = 0; i < count; i++) {
			int index = list->InsertItem(i, GetPolygonPropertyName(i));
			list->SetItemText(index, 1, GetPropertyTypeName(GetPolygonPropertyType(i)));
			
			char	buf[80];
			PropertyValue*	prop = o->GetProperty(i);
			if (prop) prop->WriteToString(buf, 80);
			else strcpy(buf, "---");

			list->SetItemText(index, 2, buf);
			list->SetItemData(index, i);
		}
	}
	
	// Set the info to the controls.
	UpdateData(FALSE);
}


CTerrainDoc* PolygonFormView::GetDocument()
{
	return GetFrame()->GetDocument();
}


BOOL PolygonFormView::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Update();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL PolygonFormView::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	Update();

	return CPropertyPage::OnSetActive();
}


void PolygonFormView::OnClosed() 
{
	CButton*	b = (CButton*) GetDlgItem(IDC_CLOSED);
	if (b) {
		int	c = b->GetCheck();
		if (c == 2) ClosedValid = false;
		else ClosedValid = true;
	}
}


void PolygonFormView::OnDblclkProperties(NMHDR* pNMHDR, LRESULT* pResult) 
// Called when user double-clicks in the properties list.
// Put up a dialog box to edit the selected property.
{
	// Figure out which property is being clicked on.
	int	index = -1;
	int	ItemCount = m_PropertyList.GetItemCount();
	for (int i = 0; i < ItemCount; i++) {
		UINT	state = m_PropertyList.GetItemState(i, LVIS_SELECTED | LVIS_FOCUSED);
		if (state & LVIS_FOCUSED) {
			index = m_PropertyList.GetItemData(i);
			break;
		}
	}

	// Edit the property.
	polygon*	o = SingleSelectedPolygon;
	if (o) {
		if (index >= 0 && index < o->GetPropertyCount()) {
			bool	Changed = o->GetProperty(index)->UserEdit();
			if (Changed) {
				GetDocument()->SetModifiedFlag();
				GetDocument()->UpdateAllViews(NULL);
			}
		}
	}

	*pResult = 0;
}

