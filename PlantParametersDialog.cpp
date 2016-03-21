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
// PlantParametersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "PlantParametersDialog.h"
#include "Geometry.h"
#include "Solid.h"
#include "ObjectProperties.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PlantParametersDialog dialog


PlantParametersDialog::PlantParametersDialog(CTerrainDoc* doc, CWnd* pParent /*=NULL*/)
	: CDialog(PlantParametersDialog::IDD, pParent)
{
	Document = doc;

	//{{AFX_DATA_INIT(PlantParametersDialog)
	m_Density = 0.0f;
	m_EnableSpray = FALSE;
	m_ObjectY = 0.0f;
	m_PinToGround = FALSE;
	m_Radius = 0.0f;
	m_ErasePercent = 0.0f;
	//}}AFX_DATA_INIT
}


void PlantParametersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PlantParametersDialog)
	DDX_Control(pDX, IDC_OBJECTTYPE, m_ObjectType);
	DDX_Control(pDX, IDC_OBJECTSOLID, m_ObjectSolid);
	DDX_Control(pDX, IDC_OBJECTGEOMETRY, m_ObjectGeometry);
	DDX_Text(pDX, IDC_DENSITY, m_Density);
	DDV_MinMaxFloat(pDX, m_Density, 0.f, 100.f);
	DDX_Check(pDX, IDC_ENABLESPRAY, m_EnableSpray);
	DDX_Text(pDX, IDC_OBJECTY, m_ObjectY);
	DDX_Check(pDX, IDC_PINTOGROUND, m_PinToGround);
	DDX_Text(pDX, IDC_RADIUS, m_Radius);
	DDV_MinMaxFloat(pDX, m_Radius, 0.f, 1000.f);
	DDX_Text(pDX, IDC_ERASEPERCENT, m_ErasePercent);
	DDV_MinMaxFloat(pDX, m_ErasePercent, 0.f, 100.f);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate) {
		InitialObjectType = m_ObjectType.GetCurSel();
		InitialObjectGeometry = m_ObjectGeometry.GetCurSel();
		InitialObjectSolid = m_ObjectSolid.GetCurSel();
	}
}


BEGIN_MESSAGE_MAP(PlantParametersDialog, CDialog)
	//{{AFX_MSG_MAP(PlantParametersDialog)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PlantParametersDialog message handlers


void	PlantParametersDialog::SetParameters(const ToolPalette::PlantParameters& p)
// Sets the initial state of the dialog, prior to DoModal.
{
//	m_ObjectType.SetCurSel(p.ObjectType);
//	m_ObjectGeometry.SetCurSel(p.ObjectGeometry);
//	m_ObjectSolid.SetCurSel(p.ObjectSolid);
	InitialObjectType = p.ObjectType;
	InitialObjectGeometry = p.ObjectGeometry;
	InitialObjectSolid = p.ObjectSolid;

	m_ObjectY = p.ObjectY;
	m_PinToGround = p.PinToGround;
	m_EnableSpray = p.EnableSpray;
	m_Radius = p.Radius;
	m_Density = p.Count;
	m_ErasePercent = p.ErasePercent;
}


void	PlantParametersDialog::GetParameters(ToolPalette::PlantParameters* p)
// Fills the pointed-to structure with the current values from the dialog.
{
	// Get the data from the controls into our member variables.
//	if (!UpdateData()) {
//		return;
//	}

	p->ObjectType = InitialObjectType; // m_ObjectType.GetCurSel();
	p->ObjectGeometry = InitialObjectGeometry; // m_ObjectGeometry.GetCurSel();
	p->ObjectSolid = InitialObjectSolid; // m_ObjectSolid.GetCurSel();
	p->ObjectY = m_ObjectY;
	p->PinToGround = m_PinToGround;
	p->EnableSpray = m_EnableSpray;
	p->Radius = m_Radius;
	p->Count = m_Density;
	p->ErasePercent = m_ErasePercent;
}


BOOL PlantParametersDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Set up the object-type list box.
	int	count = GetObjectTypeCount();
	int	i;
	for (i = 0; i < count; i++) {
		m_ObjectType.AddString(GetObjectTypeName(i));
	}
	m_ObjectType.SetCurSel(InitialObjectType);
	
	CTerrainDoc*	doc = Document;

	// Update the list of available geometry.
	if (m_ObjectGeometry.m_hWnd == NULL) return TRUE;
	m_ObjectGeometry.ResetContent();
	int	GeoCount = doc->GetGeometryCount();
	Geometry*	g = doc->GetGeometryList();
	for (i = 0; i < GeoCount; i++) {
		m_ObjectGeometry.AddString(g->GetName());
		g = g->Next;
	}
	m_ObjectGeometry.SetCurSel(InitialObjectGeometry);

	// Update the list of available solids.
	if (m_ObjectSolid.m_hWnd == NULL) return TRUE;
	m_ObjectSolid.ResetContent();
	int	SolCount = doc->GetSolidCount();
	Solid*	s = doc->GetSolidList();
	for (i = 0; i < SolCount; i++) {
		m_ObjectSolid.AddString(s->GetName());
		s = s->Next;
	}
	m_ObjectSolid.SetCurSel(InitialObjectSolid);

	return TRUE;
}


void PlantParametersDialog::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}
