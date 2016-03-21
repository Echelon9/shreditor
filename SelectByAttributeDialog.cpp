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
// SelectByAttributeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "SelectByAttributeDialog.h"
#include "Geometry.h"
#include "Solid.h"
#include "ObjectElem.h"
#include "ObjectProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SelectByAttributeDialog dialog


SelectByAttributeDialog::SelectByAttributeDialog(CTerrainDoc* doc, CWnd* pParent /*=NULL*/)
	: CDialog(SelectByAttributeDialog::IDD, pParent)
{
	Document = doc;

	//{{AFX_DATA_INIT(SelectByAttributeDialog)
	m_NotGeometry = FALSE;
	m_NotSolid = FALSE;
	m_NotType = FALSE;
	//}}AFX_DATA_INIT
}


void SelectByAttributeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SelectByAttributeDialog)
	DDX_Control(pDX, IDC_RANDOMMASK, m_RandomMask);
	DDX_Control(pDX, IDC_PIN, m_ObjectPin);
	DDX_Control(pDX, IDC_COMBO_OBJECTTYPE, m_ObjectType);
	DDX_Control(pDX, IDC_COMBO_OBJECTSOLID, m_ObjectSolid);
	DDX_Control(pDX, IDC_COMBO_OBJECTGEOMETRY, m_ObjectGeometry);
	DDX_Check(pDX, IDC_NOTGEOMETRY, m_NotGeometry);
	DDX_Check(pDX, IDC_NOTSOLID, m_NotSolid);
	DDX_Check(pDX, IDC_NOTTYPE, m_NotType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SelectByAttributeDialog, CDialog)
	//{{AFX_MSG_MAP(SelectByAttributeDialog)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_SELECT, OnSelect)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_DESELECT, OnDeselect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SelectByAttributeDialog message handlers

BOOL SelectByAttributeDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Set up the object-type list box.
	m_ObjectType.AddString("*any*");
	int	count = GetObjectTypeCount();
	int	i;
	for (i = 0; i < count; i++) {
		m_ObjectType.AddString(GetObjectTypeName(i));
	}
	m_ObjectType.SetCurSel(0);

	Update();
	
	return TRUE;
}


void SelectByAttributeDialog::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}


void SelectByAttributeDialog::Update()
// Called when the document or selection changes.  Refresh the list of
// geometry and solids.
{
	// Update the list of geometry.
	if (m_ObjectGeometry.m_hWnd == NULL) return;
	m_ObjectGeometry.ResetContent();
	m_ObjectGeometry.AddString("*any*");
	int	GeoCount = Document->GetGeometryCount();
	Geometry*	g = Document->GetGeometryList();
	for (int i = 0; i < GeoCount; i++) {
		m_ObjectGeometry.AddString(g->GetName());
		g = g->Next;
	}
	m_ObjectGeometry.SetCurSel(0);

	// Update the list of solids.
	if (m_ObjectSolid.m_hWnd == NULL) return;
	m_ObjectSolid.ResetContent();
	m_ObjectSolid.AddString("*any*");
	int	SolCount = Document->GetSolidCount();
	Solid*	s = Document->GetSolidList();
	for (i = 0; i < SolCount; i++) {
		m_ObjectSolid.AddString(s->GetName());
		s = s->Next;
	}
	m_ObjectSolid.SetCurSel(0);

	// Set the "Pin" state to indeterminate.
	m_ObjectPin.SetCheck(2);

	// Set the "mask" state to 100%
	m_RandomMask.SetRange(0, 101, TRUE);
	m_RandomMask.SetPos(100);
}


void SelectByAttributeDialog::ProcessByAttribute(bool Select)
// For all objects which match the current attributes, select
// or de-select according to the given value of Select.
{
	// Set up to do the tests.
	int	Type = m_ObjectType.GetCurSel() - 1;
	int	Geo = m_ObjectGeometry.GetCurSel() - 1;
	int	Sol = m_ObjectSolid.GetCurSel() - 1;
	int	Pin = m_ObjectPin.GetCheck();
	int	RandomMask = m_RandomMask.GetPos() * 255 / 100;

	int	NotMask = (m_NotType ? 1 : 0) |
			(m_NotGeometry ? 2 : 0) |
			(m_NotSolid ? 4 : 0);

	// Go through the object list.  Each object that passes the tests
	// will have its selection state set to the value of Select.
//	ObjectElem*	List = Document->GetObjectList();
//	ObjectElem*	e = List;
//	if (e) {
	ObjectIterator	it;
	Document->InitObjectListIterator(&it);
	Object*	o;
	while (o = Document->GetObjectAndAdvanceIterator(&it)) {
		int	test = 0;
		test |= (Type == -1 || o->GetType() == Type) ? 1 : 0;
		test |= (Geo == -1 || o->GetGeometryReference() == Geo) ? 2 : 0;
		test |= (Sol == -1 || o->GetSolidReference() == Sol) ? 4 : 0;
		test |= (Pin == 2 || int(o->GetReferenceToGround()) == Pin) ? 8 : 0;
		
		test ^= NotMask;
		
		if (test == 0x0F) {
			if (RandomMask >= 255 || (RandomMask > ((rand() >> 4) & 0xFF))) {
				o->SetSelected(Select);
			}
		}
//		e = e->Next;
	}

	Document->UpdateAllViews(NULL);
}



void SelectByAttributeDialog::OnClear() 
// Clear object selection.
{
	Document->ClearObjectSelection();
	Document->UpdateAllViews(NULL);
}


void SelectByAttributeDialog::OnSelect() 
// Called when user presses "Select".  Select the objects which
// match the set attributes.
{
	if (!UpdateData()) {
		return;
	}

	ProcessByAttribute(true);
}


void SelectByAttributeDialog::OnDeselect() 
// Deselect objects which match attributes.
{
	if (!UpdateData()) {
		return;
	}

	ProcessByAttribute(false);
}

