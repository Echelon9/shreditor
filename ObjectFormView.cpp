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
// ObjectFormView.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "ObjectFormView.h"
#include "ObjectElem.h"
#include "Object.h"
#include "Geometry.h"
#include "Solid.h"
#include "ChildFrm.h"
#include "ObjectProperties.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ObjectFormView


ObjectFormView::ObjectFormView()
	: CPropertyPage(ObjectFormView::IDD)
{
	//{{AFX_DATA_INIT(ObjectFormView)
	m_ObjectX = 0.0f;
	m_ObjectY = 0.0f;
	m_ObjectZ = 0.0f;
	m_PinToGround = FALSE;
	//}}AFX_DATA_INIT

	Frame = NULL;
	SingleSelectedObject = NULL;

	TypeValid = false;
	GeometryValid = false;
	SolidValid = false;
	XValid = false;
	YValid = false;
	ZValid = false;
	PinValid = false;
}

ObjectFormView::~ObjectFormView()
{
}


void ObjectFormView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ObjectFormView)
	DDX_Control(pDX, IDC_PROPERTIES, m_PropertyList);
	DDX_Control(pDX, IDC_COMBO_OBJECTTYPE, m_ObjectType);
	DDX_Control(pDX, IDC_COMBO_OBJECTGEOMETRY, m_ObjectGeometry);
	DDX_Control(pDX, IDC_COMBO_OBJECTSOLID, m_ObjectSolid);
	DDX_Check(pDX, IDC_PINTOGROUND, m_PinToGround);
	//}}AFX_DATA_MAP

	if (XValid) DDX_Text(pDX, IDC_OBJECTX, m_ObjectX);
	if (YValid) DDX_Text(pDX, IDC_OBJECTY, m_ObjectY);
	if (ZValid) DDX_Text(pDX, IDC_OBJECTZ, m_ObjectZ);

	if (pDX->m_bSaveAndValidate == 0) {
		// Sending data to controls.  Empty the control if the field is invalid.

		CEdit*	c;
		if (XValid == false) {
			c = (CEdit*) GetDlgItem(IDC_OBJECTX);
			if (c) {
				c->SetSel(0, -1);
				c->Clear();
			}
		}
		if (YValid == false) {
			c = (CEdit*) GetDlgItem(IDC_OBJECTY);
			if (c) {
				c->SetSel(0, -1);
				c->Clear();
			}
		}
		if (ZValid == false) {
			c = (CEdit*) GetDlgItem(IDC_OBJECTZ);
			if (c) {
				c->SetSel(0, -1);
				c->Clear();
			}
		}

		CButton* b = (CButton*) GetDlgItem(IDC_PINTOGROUND);
		if (b) {
			if (PinValid == false) {
				b->SetButtonStyle(BS_AUTO3STATE);
				b->SetCheck(2);
			} else {
				b->SetButtonStyle(BS_AUTOCHECKBOX);
			}
		}
	}
}


BEGIN_MESSAGE_MAP(ObjectFormView, CPropertyPage)
	//{{AFX_MSG_MAP(ObjectFormView)
	ON_BN_CLICKED(IDC_APPLYOBJECTPROPERTIES, OnApplyObjectProperties)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJECTTYPE, OnSelchangeComboObjecttype)
	ON_EN_CHANGE(IDC_OBJECTX, OnChangeObjectx)
	ON_EN_CHANGE(IDC_OBJECTY, OnChangeObjecty)
	ON_EN_CHANGE(IDC_OBJECTZ, OnChangeObjectz)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJECTGEOMETRY, OnSelchangeComboObjectgeometry)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJECTSOLID, OnSelchangeComboObjectsolid)
	ON_BN_CLICKED(IDC_PINTOGROUND, OnPintoground)
	ON_NOTIFY(NM_DBLCLK, IDC_PROPERTIES, OnDblclkProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ObjectFormView diagnostics

#ifdef _DEBUG
void ObjectFormView::AssertValid() const
{
	CPropertyPage::AssertValid();
//	CDialog::AssertValid();
}

void ObjectFormView::Dump(CDumpContext& dc) const
{
	CPropertyPage::Dump(dc);
//	CDialog::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ObjectFormView message handlers

void ObjectFormView::OnApplyObjectProperties() 
// Called when the user presses the Apply button.
{
	// Get the data from the controls into our member variables.
	if (!UpdateData()) {
		return;
	}

	CTerrainDoc* Doc = GetDocument();
	Object*	o = NULL;

	// Apply the new values to all selected objects, if they're different.
	BOOL Changed = FALSE;

	int	Type = m_ObjectType.GetCurSel();
	int	Geo = m_ObjectGeometry.GetCurSel();
	int	Sol = m_ObjectSolid.GetCurSel();

	ObjectIterator	it;
	Doc->InitObjectListIterator(&it);
	while (o = Doc->GetObjectAndAdvanceIterator(&it)) {
//	ObjectElem* e = Doc->GetNextSelectedObject(NULL);
//	for ( ; e; e = Doc->GetNextSelectedObject(e)) {
//		o = e->Object;
		if (o->GetSelected() == false) continue;

		if (TypeValid == true && Type != o->GetType()) {
			o->SetType(Type);
			Changed = TRUE;
		}

		if (XValid == true && m_ObjectX != o->GetX()) {
			o->SetX(Doc, m_ObjectX);
			Changed = TRUE;
		}
		if (YValid == true && m_ObjectY != o->GetY()) {
			o->SetY(m_ObjectY);
			Changed = TRUE;
		}
		if (ZValid == true && m_ObjectZ != o->GetZ()) {
			o->SetZ(Doc, m_ObjectZ);
			Changed = TRUE;
		}

		if (PinValid == true && bool(m_PinToGround) != o->GetReferenceToGround()) {
			o->SetReferenceToGround(bool(m_PinToGround));
			Changed = TRUE;
		}

		if (GeometryValid == true && Geo != o->GetGeometryReference()) {
			o->SetGeometryReference(Geo);
			Changed = TRUE;
		}

		if (SolidValid == true && Sol != o->GetSolidReference()) {
			o->SetSolidReference(Sol);
			Changed = TRUE;
		}
	}

	if (Changed && Doc) {
		// Mark the document as modified, and refresh the view.
		Doc->SetModifiedFlag();
		Doc->UpdateAllViews(NULL);
	}
}


CTerrainDoc* ObjectFormView::GetDocument()
{
	return GetFrame()->GetDocument();
}


void ObjectFormView::Update()
// Called when the document or selection changes.  Grab
//  information from the currently selected object(s).
{
	CTerrainDoc*	doc = GetDocument();
	if (doc == NULL) return;

	// Update the list of available geometry.
	if (m_ObjectGeometry.m_hWnd == NULL) return;
	m_ObjectGeometry.ResetContent();
	int	GeoCount = doc->GetGeometryCount();
	Geometry*	g = doc->GetGeometryList();
	for (int i = 0; i < GeoCount; i++) {
		m_ObjectGeometry.AddString(g->GetName());
		g = g->Next;
	}

	// Update the list of available solids.
	if (m_ObjectSolid.m_hWnd == NULL) return;
	m_ObjectSolid.ResetContent();
	int	SolCount = doc->GetSolidCount();
	Solid*	s = doc->GetSolidList();
	for (i = 0; i < SolCount; i++) {
		m_ObjectSolid.AddString(s->GetName());
		s = s->Next;
	}

	Object*	o = NULL;

	// Find the first selected object.
	ObjectIterator	it;
	doc->InitObjectListIterator(&it);
	Object*	p;
	while (p = doc->GetObjectAndAdvanceIterator(&it)) {
		if (p->GetSelected()) {
			o = p;
			break;
		}
	}

	// If we have a selection, then assume fields are valid.  Compare all other selected objects against
	// the first selected object, and if a field varies, then mark that field
	// as invalid.  The idea is to only show values for the fields that are
	// invariant across the selection.

	SingleSelectedObject = o;
	bool	val = o != NULL ? true : false;
	TypeValid = val;
	GeometryValid = val;
	SolidValid = val;
	XValid = val;
	YValid = val;
	ZValid = val;
	PinValid = val;

	while (p = doc->GetObjectAndAdvanceIterator(&it)) {
		if (p->GetSelected() == false) continue;

		SingleSelectedObject = NULL;

		if (p->GetType() != o->GetType()) TypeValid = false;
		if (p->GetGeometryReference() != o->GetGeometryReference()) GeometryValid = false;
		if (p->GetSolidReference() != o->GetSolidReference()) SolidValid = false;
		if (p->GetX() != o->GetX()) XValid = false;
		if (p->GetY() != o->GetY()) YValid = false;
		if (p->GetZ() != o->GetZ()) ZValid = false;
		if (p->GetReferenceToGround() != o->GetReferenceToGround()) PinValid = false;
	}

	// Initialize fields of the dialog.
	if (TypeValid) m_ObjectType.SetCurSel(o->GetType());
	else m_ObjectType.SetCurSel(-1);

	if (XValid) m_ObjectX = o->GetX();
	else m_ObjectX = 0;

	if (YValid) m_ObjectY = o->GetY();
	else m_ObjectY = 0;

	if (ZValid) m_ObjectZ = o->GetZ();
	else m_ObjectZ = 0;

	if (PinValid) m_PinToGround = o->GetReferenceToGround();
	else m_PinToGround = FALSE;

	if (GeometryValid) m_ObjectGeometry.SetCurSel(o->GetGeometryReference());
	else m_ObjectGeometry.SetCurSel(-1);

	if (SolidValid) m_ObjectSolid.SetCurSel(o->GetSolidReference());
	else m_ObjectSolid.SetCurSel(-1);

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
	if (SingleSelectedObject) {
		o = SingleSelectedObject;
		int	type = 	o->GetType();
		int	count = GetPropertyCount(type);
		list->SetItemCount(count);
		for (int i = 0; i < count; i++) {
			int index = list->InsertItem(i, GetPropertyName(type, i));
			list->SetItemText(index, 1, GetPropertyTypeName(GetPropertyType(type, i)));
			
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


void ObjectFormView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::CalcWindowRect(lpClientRect, nAdjustType);
}


BOOL ObjectFormView::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Set up the object-type list box.
	int	count = GetObjectTypeCount();
	int	i;
	for (i = 0; i < count; i++) {
		m_ObjectType.AddString(GetObjectTypeName(i));
	}

//	CFrameWnd*	f = GetParentFrame();
//	f->MoveWindow(20, 20, 285, 240);

	Update();

	return TRUE;
}


//BOOL ObjectFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
//{
//	// TODO: Add your specialized code here and/or call the base class
//	
//	return CPropertyPage::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
//}

BOOL ObjectFormView::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	Update();

	return CPropertyPage::OnSetActive();
}

BOOL ObjectFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void ObjectFormView::OnSelchangeComboObjecttype() 
// Called when user changes the object type.
{
	TypeValid = true;
}


void ObjectFormView::OnSelchangeComboObjectgeometry() 
// Called when user changes the geometry reference.
{
	GeometryValid = true;
}

void ObjectFormView::OnSelchangeComboObjectsolid() 
// Called when user changed the solid reference.
{
	SolidValid = true;
}

void ObjectFormView::OnChangeObjectx()
{
	char	dummy[80];
	// If the box has been emptied, then mark the field invalid.
	if (GetDlgItem(IDC_OBJECTX)->GetWindowText(dummy, 10) == 0) XValid = false;
	else XValid = true;	// Otherwise, it's valid.
}

void ObjectFormView::OnChangeObjecty() 
{
	char	dummy[80];
	// If the box has been emptied, then mark the field invalid.
	if (GetDlgItem(IDC_OBJECTY)->GetWindowText(dummy, 10) == 0) YValid = false;
	else YValid = true;	// Otherwise, it's valid.
}

void ObjectFormView::OnChangeObjectz() 
{
	char	dummy[80];
	// If the box has been emptied, then mark the field invalid.
	if (GetDlgItem(IDC_OBJECTZ)->GetWindowText(dummy, 10) == 0) ZValid = false;
	else ZValid = true;	// Otherwise, it's valid.
}


void ObjectFormView::OnPintoground() 
{
	CButton*	b = (CButton*) GetDlgItem(IDC_PINTOGROUND);
	if (b) {
		int	c = b->GetCheck();
		if (c == 2) PinValid = false;
		else PinValid = true;
	}
}


void ObjectFormView::OnDblclkProperties(NMHDR* pNMHDR, LRESULT* pResult) 
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
	Object*	o = SingleSelectedObject;
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
