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
// terrainView.h : interface of the CTerrainView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERRAINVIEW_H__DED47ACE_CF2D_11D1_83C5_98E1A892656D__INCLUDED_)
#define AFX_TERRAINVIEW_H__DED47ACE_CF2D_11D1_83C5_98E1A892656D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "grid.h"
#include "TerrainDoc.h"
#include "ExcavateDialog.h"
#include "bitmap.h"


class CChildFrame;
//class PositioningWindow;
class polygon;
class Vertex;


const int	TERRAINVIEW_MAX_CAPTURE_POINTS = 200;


class CTerrainView : public CView
{
protected: // create from serialization only
	CTerrainView();
	DECLARE_DYNCREATE(CTerrainView)

// Attributes
public:
	CTerrainDoc* GetDocument();

	static float*	GetTempGridArray();
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTerrainView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
public:
	void	ComputeDeltaExcavateSmoothing(int xsize, int zsize, float Radius);
	void	ComputeDeltaExcavateClone(int xsize, int zsize);
	void	ComputeDeltaExcavateAddHeight(int xsize, int zsize);
	
	float	GetHeightAtPoint(CPoint& point, int* px = NULL, int* pz = NULL);

	void	SetExcavateParameters(const ExcavateParameters& p) { ExcavateParams = p; }
	const ExcavateParameters&	GetExcavateParameters() { return ExcavateParams; }

	void MoveSelectedObjects(float DeltaX, float DeltaZ);
	void SelectObjectsInBox(const CPoint& anchor, const CPoint& corner, bool Toggle);
	void XORRubberBandBox(const CPoint& anchor, const CPoint& corner);
	void ComputeOrigin();
	CChildFrame* GetFrame();

	Grid*	GetGridAtPoint(CPoint point);

	Object* GetObjectAtPoint(CPoint point);
	CPoint	GetObjectPoint(Object* obj);
	void	PlantObjects(CPoint p, bool Initial);
	void	EraseObjects(CPoint p, bool Initial);
	void	DeleteObject(CPoint p);

	Vertex*	GetVertexAtPoint(polygon** Poly, CPoint point);

	void	AddEdgeVertexAtPoint(CPoint point);

	void	ConvertWindowToWorld(CPoint p, float* px, float* pz);

	Grid*	GetSelectedGrid();
	void	SetSelectedGrid(Grid* g);

	virtual ~CTerrainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void	ShowToolCursor(/*shape,*/ CPoint loc, float Radius);
	void	HideToolCursor();
	
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTerrainView)
	afx_msg void OnZoomTerrainIn();
	afx_msg void OnZoomTerrainOut();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNewGrid();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnExcavate();
	afx_msg void OnAddNewPolygon();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DrawPolygon(CDC* pDC, polygon* p, int ZoomLevel);
	enum CurrentActionType {
		NO_ACTION = 0,
		MOVING_OBJECTS,
		SPRAYING_OBJECTS,
		ERASING_OBJECTS,
		MOVING_GRID,
		EXCAVATING,
		PAINTING_SURFACE,
		MOVING_VERTEX,
		DRAGGING_VIEW,
		RUBBER_BAND_OBJECT_SELECT
	} CurrentAction;

	Object*	MovingObject;
	Grid*	MovingGrid;
	CPoint	GridStartPoint;
	bool	AllowGridMove;

	Vertex*	MovingVertex;

	int	ViewX0, ViewZ0, ViewX1, ViewZ1;
	int	ViewCenterX, ViewCenterZ;
	CPoint	DragLastPoint;

	CPoint	MoveStart;
	CPoint	BoxAnchor, BoxCorner;

	ExcavateParameters	ExcavateParams;
	bitmap8*	ModulationMap;
	char	MapName[EXCAVATE_BITMAP_FILENAME_MAXLEN];

	void ApplyStroke();
	void	ApplyPaintStroke();
	void	DrawStrokeInScratchArray(float Radius, float EdgeTaper);
	void	DrawGridStrokeInScratchArray(Grid* g, float Radius, float EdgeTaper);
	void	ApplyModulation(int xsize, int zsize, bitmap8* ModulationMap, int xoffset, int zoffset);
	
	Grid*	SelectedGrid;
	float	BaseElevation;	// jbl
	
	enum { NONE = 0, LEFT_BUTTON, RIGHT_BUTTON }	CaptureMode;
	int	CapturePointCount;
	POINT	CapturePoint[TERRAINVIEW_MAX_CAPTURE_POINTS];

	// Cursor info.
	bool	CursorVisible;
	CPoint	LastCursorLocation;
	float	LastCursorRadius;
};


#ifndef _DEBUG  // debug version in terrainView.cpp
inline CTerrainDoc* CTerrainView::GetDocument()
   { return (CTerrainDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAINVIEW_H__DED47ACE_CF2D_11D1_83C5_98E1A892656D__INCLUDED_)
