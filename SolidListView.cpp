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
// SolidListView.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "SolidListView.h"
#include "ChildFrm.h"
#include "Solid.h"
#include "SolidPropertiesDialog.h"
#include "onfirmDeleteDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SolidListView property page

IMPLEMENT_DYNCREATE(SolidListView, CPropertyPage)

SolidListView::SolidListView() : CPropertyPage(SolidListView::IDD)
{
	//{{AFX_DATA_INIT(SolidListView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

SolidListView::~SolidListView()
{
}

void SolidListView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SolidListView)
	DDX_Control(pDX, IDC_SOLIDLIST, m_SolidList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SolidListView, CPropertyPage)
	//{{AFX_MSG_MAP(SolidListView)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_EDIT_SOLID, OnEditSolid)
	ON_BN_CLICKED(IDC_ADD_NEW_SOLID, OnAddNewSolid)
	ON_BN_CLICKED(IDC_DELETE_SOLID, OnDeleteSolid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SolidListView message handlers


void SolidListView::Update()
// Rebuild the list of solid resources.
{
//	CPropertyPage::OnUpdate(pSender, lHint, pHint);

	// TODO: Add extra initialization here
	CTerrainDoc*	doc = GetDocument();
	Solid*	s = doc->GetSolidList();
	int	SolidCount = doc->GetSolidCount();
	CListCtrl*	list = &m_SolidList;
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

	// Comment.
	col.iSubItem = 1;
	col.pszText = "Comment";
	col.cx = 170;
	list->InsertColumn(1, &col);

	// Set the list items.
	list->DeleteAllItems();
	list->SetItemCount(SolidCount);
	for (int i = 0; i < SolidCount; i++, s = s->Next) {
		if (s == NULL) break;

		int index = list->InsertItem(i, s->GetName());
		list->SetItemText(index, 1, s->GetComment());
		list->SetItemData(index, i);
	}
}


BOOL SolidListView::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	Update();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void SolidListView::OnCancelMode() 
{
	CPropertyPage::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}


CTerrainDoc*	SolidListView::GetDocument()
{
	return GetFrame()->GetDocument();
}


int SolidListView::GetSelectedItemIndex()
// Returns the index of the solid item that's selected.
// Returns -1 if it can't find a selected item.
{
	int	ItemCount = m_SolidList.GetItemCount();
	for (int i = 0; i < ItemCount; i++) {
		UINT	state = m_SolidList.GetItemState(i, LVIS_SELECTED | LVIS_FOCUSED);
		if (state & LVIS_FOCUSED) {
			return m_SolidList.GetItemData(i);
		}
	}

	return -1;
}


void SolidListView::OnEditSolid() 
// Called when user clicks "Edit" in the solid list.
{
	// Figure out which solid item to edit.
	int	index = GetSelectedItemIndex();
	if (index < 0) {
		// No item selected, so we don't know what to delete...
		return;
	}
	if (index == 0) {
		// Can't edit null geometry.
		return;
	}

	// Get the solid object to edit.
	Solid*	s = GetDocument()->GetSolid(index);
	if (s == NULL) return;

	// Call edit dialog box.
	SolidPropertiesDialog d;
	d.SetName(s->GetName());
	d.SetComment(s->GetComment());
	d.SetType(s->GetType());
	int i;
	for (i = 0; i < 3; i++) {
		d.SetDimension(i, s->GetDimension(i));
	}

	if (d.DoModal() != IDCANCEL) {
		// Apply the changes.
		s->SetName(d.GetName());
		s->SetComment(d.GetComment());
		s->SetType(d.GetType());
		for (i = 0; i < 3; i++) {
			s->SetDimension(i, d.GetDimension(i));
		}

		GetDocument()->SetModifiedFlag();
		GetDocument()->UpdateAllViews(NULL);
	}
}


void SolidListView::OnAddNewSolid() 
// Called when user clicks "New Solid" button.
{
	// Call the properties dialog box with some default initial values.
	SolidPropertiesDialog d;
	d.SetName("Untitled");
	d.SetComment("");
	d.SetType(Solid::CYLINDER);
	d.SetDimension(0, 1);
	d.SetDimension(1, 2);
	d.SetDimension(2, 0);

	if (d.DoModal() != IDCANCEL) {
		// Create a new solid item with the new values, and add it to our list.
		Solid*	s = new Solid;

		s->SetName(d.GetName());
		s->SetComment(d.GetComment());
		s->SetType(d.GetType());
		int i;
		for (i = 0; i < 3; i++) {
			s->SetDimension(i, d.GetDimension(i));
		}

		GetDocument()->AddSolid(s);
		GetDocument()->SetModifiedFlag();

		GetDocument()->UpdateAllViews(NULL);
	}
}


void SolidListView::OnDeleteSolid() 
// Called when user presses the button to delete a solid.
{
	// Figure out which solid item to delete.
	int	index = GetSelectedItemIndex();
	if (index < 0) {
		// No item selected, so we don't know what to delete...
		return;
	}
	if (index == 0) {
		// Can't delete solid 0 (the null geometry).
		return;
	}

	CTerrainDoc*	doc = GetDocument();

	// Check to see if any objects are using this solid...
	// bool InUse = doc->SolidInUse(index);
	// if (InUse) {
	//	put up a dialog, noting that the object is in use, asking if the user's sure;
	//	if (DoModal() == IDCANCEL) {
	//		return;
	//	}
	// }
	// else
	{
		// Make the user confirm.
		ConfirmDeleteDialog	d(doc->GetSolid(index)->GetName());
		if (d.DoModal() == IDCANCEL) {
			return;
		}
	}

	// Delete the item and update views.
	doc->DeleteSolid(index);
	doc->SetModifiedFlag();
	doc->UpdateAllViews(NULL);
}
