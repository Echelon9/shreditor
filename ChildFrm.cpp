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
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "terrain.h"
#include "terraindoc.h"
#include "terrainview.h"
#include "PositioningWindow.h"
#include "GridFormView.h"
#include "PropertyView.h"
#include "ObjectFormView.h"
#include "PolygonFormView.h"
#include "ToolPalette.h"
#include "PropertyView.h"
#include "SolidListView.h"
#include "SelectByAttributeDialog.h"
#include "PlantParametersDialog.h"
#include "OffsetParams.h"


#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SIZE()
	ON_WM_CANCELMODE()
	ON_COMMAND(ID_OBJECT_SELECTBYATTR, OnObjectSelectbyattr)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_OBJECT_PLANTPARAMETERS, OnObjectPlantparameters)
	ON_COMMAND(ID_EXCAVATE, OnExcavate)
	ON_COMMAND(ID_EDIT_OFFSET, OnEditOffset)
	ON_COMMAND(ID_FILE_LOAD_INTO_CURRENT, OnFileLoadIntoCurrent)
	ON_COMMAND(ID_EDIT_GENERATE_TYPEMAPS, OnEditGenerateTypemaps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
//	ObjView = NULL;
//	GeoView = NULL;
	SplittersCreated = FALSE;
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// By turning off the default MFC-defined FWS_ADDTOTITLE style,
	// the framework will use first string in the document template
	// STRINGTABLE resource instead of the document name.
	cs.style &= ~(LONG)FWS_ADDTOTITLE;

	return CMDIChildWnd::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CDocument* pDoc = pContext->m_pCurrentDoc;
	ASSERT(pDoc != NULL);
	Document = dynamic_cast<CTerrainDoc*>(pDoc);

	{
		// The rest of the views go in splitter window panes.

		if (!SplitterWindow.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE)) {
			return FALSE;
		}

		// Figure out how big the parent frame is.

		
		if (!SplitterWindow.CreateView(0, 0, RUNTIME_CLASS(CTerrainView), CSize(500, 500), pContext)) {
			return FALSE;
		}
		
		if (!SplitterWindow2.CreateStatic(&SplitterWindow, 2, 1, WS_CHILD | WS_VISIBLE | WS_BORDER, SplitterWindow.IdFromRowCol(0, 1)))
		{
			return FALSE;
		}
		
		if (!SplitterWindow2.CreateView(0, 0, RUNTIME_CLASS(ToolPalette), CSize(187, 57), pContext)) {
			return FALSE;
		}
		
		if (!SplitterWindow2.CreateView(1, 0, RUNTIME_CLASS(PropertyView), CSize(187, 62), pContext)) {
			return FALSE;
		}
		
		SplittersCreated = TRUE;

		CRect	r;
		GetClientRect(&r);

		ResizePanes(r.right, r.bottom);

		GetGridFormView()->SetFrame(this);
		GetGeometryListView()->SetFrame(this);
		GetObjectFormView()->SetFrame(this);
		GetPolygonFormView()->SetFrame(this);
		GetSolidListView()->SetFrame(this);

		return TRUE;
	}
}


void CChildFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}


CTerrainView*	CChildFrame::GetTerrainView()
// Returns a pointer to the main view.
{
	CTerrainView*	v = dynamic_cast<CTerrainView*>(SplitterWindow.GetPane(0, 0));

	return v;
}


ToolPalette*	CChildFrame::GetToolPalette()
// Returns the tool palette, or NULL.
{
	ToolPalette*	p = dynamic_cast<ToolPalette*>(SplitterWindow2.GetPane(0, 0));

	return p;
}


PropertyView*	CChildFrame::GetPropertyView()
// Returns a pointer to the PropertyView object, which
// contains the various property pages.
{
	return dynamic_cast<PropertyView*>(SplitterWindow2.GetPane(1, 0));
}


PaintParametersView*	CChildFrame::GetPaintParametersView()
// Returns the PaintParametersView, if any.
{
	PropertyView*	v = GetPropertyView();
	if (v) {
		return v->GetPaintParametersView();
	} else {
		return NULL;
	}
}


GridFormView*	CChildFrame::GetGridFormView()
// Returns the GridFormView, if any.
{
	PropertyView*	v = GetPropertyView();
	if (v) {
		return v->GetGridFormView();
	} else {
		return NULL;
	}
}


ObjectFormView*	CChildFrame::GetObjectFormView()
// Returns the ObjectFormView, if any.
{
//	return ObjView;
	PropertyView*	v = GetPropertyView();
	if (v) {
		return v->GetObjectFormView();
	} else {
		return NULL;
	}
}

PolygonFormView*	CChildFrame::GetPolygonFormView()
// Returns the ObjectFormView, if any.
{
	PropertyView*	v = GetPropertyView();
	if (v) {
		return v->GetPolygonFormView();
	} else {
		return NULL;
	}
}

GeometryListView* CChildFrame::GetGeometryListView()
// Returns a pointer to the GeometryListView window.
{
//	return GeoView;
	PropertyView*	v = GetPropertyView();
	if (v) {
		return v->GetGeometryListView();
	} else {
		return NULL;
	}
}


SolidListView*	CChildFrame::GetSolidListView()
// Returns a pointer to the SolidListView window.
{
	PropertyView*	v = GetPropertyView();
	if (v) {
		return v->GetSolidListView();
	} else {
		return NULL;
	}
}


void CChildFrame::RecalcLayout(BOOL bNotify) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMDIChildWnd::RecalcLayout(bNotify);
}


void CChildFrame::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	CMDIChildWnd::CalcWindowRect(lpClientRect, nAdjustType);
	lpClientRect->top = lpClientRect->left = 0;
	lpClientRect->right = 200;
	lpClientRect->bottom = 200;
}


void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	// Resize panes.
	if (SplittersCreated) {
		ResizePanes(cx, cy);
	}

	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}


void CChildFrame::OnCancelMode() 
{
	CMDIChildWnd::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}


void CChildFrame::ResizePanes(int cx, int cy)
// Resizes the panes, given the total size of the frame.
{
	int	TerrainWidth = cx - 268;
	if (TerrainWidth < 50) TerrainWidth = 50;
	
	int	ToolsHeight = 280;
	if (ToolsHeight > cy - 20) ToolsHeight = cy - 20;
	if (ToolsHeight < 0) ToolsHeight = 0;
	
	SplitterWindow.SetColumnInfo(0, TerrainWidth, 0);
	SplitterWindow2.SetRowInfo(0, ToolsHeight, 0);
	SplitterWindow2.SetRowInfo(1, cy - ToolsHeight, 0);
}


void CChildFrame::OnObjectSelectbyattr() 
// For the menu item.  Put up a dialog that lets the user select
// objects according to their attributes.
{
	SelectByAttributeDialog	d(GetDocument());

	d.DoModal();
}


void CChildFrame::OnEditDelete() 
// Called when user chooses "Delete" from Edit menu.
// Delete all selected objects.
{
	GetDocument()->DeleteSelectedObjects();
	GetDocument()->UpdateAllViews(NULL);
}


void CChildFrame::OnObjectPlantparameters() 
// Called when user chooses "Planting Parameters..." from the Object menu.
// Put up the dialog.
{
	PlantParametersDialog	d(GetDocument());

	ToolPalette::PlantParameters	p;
	p = GetToolPalette()->GetPlantParameters();
	d.SetParameters(p);

	if (d.DoModal() == IDOK) {
		d.GetParameters(&p);
		GetToolPalette()->SetPlantParameters(p);
	}
}

void CChildFrame::OnExcavate() 
// Gets called when the user wants to edit the excavator tool properties.
{
	CTerrainView*	view = GetTerrainView();

	ExcavateDialog d;
	d.SetParameters(view->GetExcavateParameters());

	if (d.DoModal() != IDCANCEL) {
		// Apply the brush parameters.
		view->SetExcavateParameters(d.GetParameters());
	}
}


void CChildFrame::OnEditOffset() 
// Prompt the user for offset coordinates, to apply to *everything* in the
// document.
{
	// Put up prompt dialog.
	OffsetParams	d;
	if (d.DoModal() != IDCANCEL) {
		// Now, move everything.
		float	XOffset = d.m_X;
		float	ZOffset = d.m_Z;
		
		CTerrainDoc*	doc = GetDocument();

		if (doc) {
			// move grids
			doc->OffsetGrids(XOffset, ZOffset);
			
			// move polygons (vertices)
			doc->OffsetPolygons(XOffset, ZOffset);

			// move objects
			doc->OffsetObjects(XOffset, ZOffset);

			GetDocument()->UpdateAllViews(NULL);
		}
	}
}


void CChildFrame::OnFileLoadIntoCurrent() 
// Prompt the user for a filename, and then load its contents into the 
// current document.
{
	CFileDialog	d(TRUE);
	if (d.DoModal() == IDCANCEL) return;

	CFile	f;
	if (!f.Open(d.GetPathName(), CFile::modeRead)) {
		::MessageBox(NULL, "Can't open file for input", "Can't Open", MB_OK | MB_ICONWARNING);
		return;
	}

	CArchive	ar(&f, CArchive::load);
	GetDocument()->Serialize(ar);

	f.Close();
}


void CChildFrame::OnEditGenerateTypemaps() 
{
	// put up a dialog box with some options & parameters...


	// Generate type data.
	GetDocument()->GenerateTypemaps();

	GetDocument()->UpdateAllViews(NULL);
}
