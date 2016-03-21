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
// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__DED47ACA_CF2D_11D1_83C5_98E1A892656D__INCLUDED_)
#define AFX_CHILDFRM_H__DED47ACA_CF2D_11D1_83C5_98E1A892656D__INCLUDED_

#include "ObjectFormView.h"	// Added by ClassView
#include "GeometryListView.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CTerrainView;
class GridView;
class GridFormView;
class ObjectFormView;
class PolygonFormView;
class PropertyFormView;
class ToolPalette;
class PositioningWindow;
class PropertyView;
class SolidListView;
class PaintParametersView;


class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
public:
	GeometryListView* GetGeometryListView();

	PropertyView*	GetPropertyView();
	CTerrainView*	GetTerrainView();
	PaintParametersView*	GetPaintParametersView();
	GridFormView*	GetGridFormView();
	ObjectFormView*	GetObjectFormView();
	PolygonFormView*	GetPolygonFormView();
	PropertyFormView*	GetPropertyFormView();
	SolidListView*	GetSolidListView();
	ToolPalette*	GetToolPalette();
	CTerrainDoc*	GetDocument() { return Document; }

	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CTerrainDoc* Document;
	void ResizePanes(int cx, int cy);
	bool SplittersCreated;
//	GeometryListView* GeoView;
//	ObjectFormView* ObjView;
	CSplitterWnd	SplitterWindow;
	CSplitterWnd	SplitterWindow2;

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCancelMode();
	afx_msg void OnObjectSelectbyattr();
	afx_msg void OnEditDelete();
	afx_msg void OnObjectPlantparameters();
	afx_msg void OnExcavate();
	afx_msg void OnEditOffset();
	afx_msg void OnFileLoadIntoCurrent();
	afx_msg void OnEditGenerateTypemaps();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__DED47ACA_CF2D_11D1_83C5_98E1A892656D__INCLUDED_)
