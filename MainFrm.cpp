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
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "terrain.h"
#include "MainFrm.h"
#include "TerrainView.h"
#include "GeometryListView.h"
//#include "ObjectFormView.h"
//#include "PolygonFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_TERRAIN, OnViewTerrain)
	ON_COMMAND(ID_VIEW_GEOMETRY_LIST, OnViewGeometryList)
	ON_COMMAND(ID_VIEW_OBJECT_PROPERTIES, OnViewObjectProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::CreateOrActivateFrame(CDocTemplate * t, CRuntimeClass * view)
// If a check view or book view (specified by pViewClass)
// already exists, then activate the MDI child window containing
// the view.  Otherwise, create a new view for the document.
{
	CMDIChildWnd* pMDIActive = MDIGetActive();
	ASSERT(pMDIActive != NULL);
	CDocument* pDoc = pMDIActive->GetActiveDocument();
	ASSERT(pDoc != NULL);

	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(view))
		{
			pView->GetParentFrame()->ActivateFrame();
			return;
		}
	}

	// Didn't find an existing frame, so open a new one.
	CMDIChildWnd* pNewFrame
		= (CMDIChildWnd*)(t->CreateNewFrame(pDoc, NULL));
	if (pNewFrame == NULL)
		return;     // not created
	ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
	t->InitialUpdateFrame(pNewFrame, pDoc);

#ifdef NOT
//	MDITile(MDITILE_HORIZONTAL);
	pos = pDoc->GetFirstViewPosition();
	while (pos != NULL) {
		pView = pDoc->GetNextView(pos);
		CFrameWnd*	f = pView->GetParentFrame();
		f->MoveWindow(10, 10, 300, 100);	//xxxxx
	}
#endif // NOT
}


void CMainFrame::OnViewTerrain() 
{
	CreateOrActivateFrame(theApp.TerrainViewTemplate,
		RUNTIME_CLASS(CTerrainView));
}


void CMainFrame::OnViewGeometryList() 
{
//	CreateOrActivateFrame(theApp.GeometryListViewTemplate,
//		RUNTIME_CLASS(GeometryListView));
}


void CMainFrame::OnViewObjectProperties() 
{
//	CreateOrActivateFrame(theApp.ObjectViewTemplate,
//		RUNTIME_CLASS(ObjectFormView));
}
