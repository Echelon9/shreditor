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
// terrainView.cpp : implementation of the CTerrainView class
//

#include "stdafx.h"
#include "terrain.h"

#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "terrainDoc.h"
#include "terrainView.h"
#include "grid.h"
#include "gridview.h"
#include "gridelem.h"
#include "childfrm.h"
#include "object.h"
#include "objectelem.h"
#include "ObjectFormView.h"
#include "ToolPalette.h"
#include "GridFormView.h"
#include "Polygon.h"
#include "ExcavateDialog.h"
#include "onfirmDeleteDialog.h"


#define PI 3.141592654

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTerrainView

IMPLEMENT_DYNCREATE(CTerrainView, CView)

BEGIN_MESSAGE_MAP(CTerrainView, CView)
	//{{AFX_MSG_MAP(CTerrainView)
	ON_COMMAND(ID_ZOOM_TERRAIN_IN, OnZoomTerrainIn)
	ON_COMMAND(ID_ZOOM_TERRAIN_OUT, OnZoomTerrainOut)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_NEW_GRID, OnNewGrid)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
//	ON_COMMAND(ID_EXCAVATE, OnExcavate)
	ON_COMMAND(ID_ADD_NEW_POLYGON, OnAddNewPolygon)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_CANCELMODE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainView construction/destruction

CTerrainView::CTerrainView()
{
	// TODO: add construction code here

	CurrentAction = NO_ACTION;
	MovingObject = NULL;

	MovingGrid = NULL;

	MovingVertex = NULL;

	SelectedGrid = NULL;
	CaptureMode = NONE;
	CapturePointCount = 0;

	ViewX0 = 0;
	ViewZ0 = 0;
	ViewX1 = 0;
	ViewZ1 = 0;

	ViewCenterX = 3000;
	ViewCenterZ = 3000;

	// jbl 08/19/98:  The optional environment variable BASE_ELEVATION
	// specifies the base amount to add to the elevation (Y) value in
	// the status bar display.  So I can compare the elevation values
	// in Shreditor to the raw values in a USGS dataset.  ShredDEM
	// reports the base elevation when it converts a dataset.
	BaseElevation = 0.0;
	char *p = getenv("BASE_ELEVATION");
	if (p != NULL) {
		BaseElevation = atof(p);
	}

	CursorVisible = false;
	LastCursorLocation = CPoint(0, 0);
	LastCursorRadius = 1;

	ModulationMap = NULL;
	MapName[0] = 0;
}


CTerrainView::~CTerrainView()
{
	if (ModulationMap) delete ModulationMap;
}


BOOL CTerrainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTerrainView drawing


// Scratch space used by ::OnDraw.
const int	TEMP_DIM = 1024;
static float	TempGrid[TEMP_DIM][TEMP_DIM];
static unsigned char	TempTypeGrid[TEMP_DIM][TEMP_DIM];


float*	CTerrainView::GetTempGridArray()
// Return a pointer to our 256x256 float temporary array.
{
	return &TempGrid[0][0];
}


static void	FixTempBoundary(int xsize, int zsize)
// Forces the boundary around (0,0)-(xsize-1, zsize-1) of TempGrid[][] to 0.
{
	int	x, z;
	for (x = 0; x < xsize; x++) {
		TempGrid[0][x] = 0;
		TempGrid[zsize-1][x] = 0;
	}
	for (z = 0; z < zsize; z++) {
		TempGrid[z][0] = 0;
		TempGrid[z][xsize-1] = 0;
	}
}


// Size of the minimum pixel block used for displaying height field.
int	DISPLAY_BLOCK_SIZE = 5;


void CTerrainView::OnDraw(CDC* pDC)
// Draws the terrain view display.
{
	CursorVisible = false;
	
	CTerrainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CChildFrame* parent = GetFrame();

//	// Draw the terrain's name.
//	pDC->TextOut(10, 10, pDoc->GetName(), strlen(pDoc->GetName()));
	// Or set a window title, or something?

	bool	DrawSelectedGrid = false;
	bool	DrawAllGrids = false;
	bool	DrawObjects = false;
	bool	DrawSurface = false;
	ToolPalette*	tools = parent->GetToolPalette();
	DrawSelectedGrid = tools->GetDrawSelectedGrid();
	DrawAllGrids = tools->GetDrawAllGrids();
	DrawObjects = tools->GetDrawObjects();
	DrawSurface = tools->GetDrawSurface();

	// Make sure ViewX0 and ViewZ0 are up to date.
	ComputeOrigin();

	// Figure out the dimensions and block size for what we're drawing.
	int	BlockPixels = DISPLAY_BLOCK_SIZE;
	CRect	Clip;
	pDC->GetClipBox(&Clip);

	int	ZoomLevel = parent->GetToolPalette()->GetZoomLevel();

	int	xsize, zsize;
	
//	// Compute placement.
//	ViewX1 = ViewX0 + ((Clip.right + BlockPixels - 1) << ZoomLevel) / BlockPixels + 1;
//	ViewZ1 = ViewZ0 + ((Clip.bottom + BlockPixels - 1) << ZoomLevel) / BlockPixels + 1;

	// Make sure x1, z1 are within bounds.
	if (ViewX1 > pDoc->GetXSize()) ViewX1 = pDoc->GetXSize();
	if (ViewZ1 > pDoc->GetZSize()) ViewZ1 = pDoc->GetZSize();
	
	xsize = (ViewX1 - ViewX0) >> ZoomLevel;
	zsize = (ViewZ1 - ViewZ0) >> ZoomLevel;
	if (xsize > TEMP_DIM) xsize = TEMP_DIM;
	if (zsize > TEMP_DIM) zsize = TEMP_DIM;

	// Clear the scratch space.
	int i, j;
	for (j = 0; j < zsize; j++) {
		for (i = 0; i < xsize; i++) {
			TempGrid[j][i] = 0;
			TempTypeGrid[j][i] = 0;
		}
	}

	// Sample the quadtree to fill the scratch height grid.
	for (j = 0; j < zsize; j++) {
		for (i = 0; i < xsize; i++) {
			TempGrid[j][i] = pDoc->GetHeight(ViewX0 + (i << ZoomLevel), ViewZ0 + (j << ZoomLevel));
		}
	}
	
	// Add the contributions of each grid into the scratch array.
	GridElem*	e = pDoc->GetGridList();
	if (e) {
		do {
			if (DrawAllGrids ||
			    (DrawSelectedGrid && e->Grid == SelectedGrid))
			{
				e->Grid->PaintIntoBuffer(&TempTypeGrid[0][0], ViewX0, ViewZ0, xsize, zsize, TEMP_DIM, ZoomLevel);
			}
			e = e->Next;
		} while (e != pDoc->GetGridList());
	}

	// Draw tick marks and contour lines.
	float	Interval = 2.0;
	if (parent) {
		Interval = parent->GetToolPalette()->GetContourInterval();
	}
	DrawHeightField(pDC, 0, 0, BlockPixels, &TempGrid[0][0], &TempTypeGrid[0][0], xsize, zsize, TEMP_DIM, Interval);

	//
	// Draw outlines of the grids.  Highlight the selected grid.
	//

	// Draw the grid outlines.
	e = pDoc->GetGridList();
	if (e) {
		do {
			Grid*	g = e->Grid;

			if (g && (DrawAllGrids || (DrawSelectedGrid && g == SelectedGrid))) {
				DrawBox(pDC,
					(((g->GetXOrigin() << g->GetScale()) - ViewX0) * BlockPixels) >> ZoomLevel,
					(((g->GetZOrigin() << g->GetScale()) - ViewZ0) * BlockPixels) >> ZoomLevel,
					(g->GetXSize() * BlockPixels << g->GetScale()) >> ZoomLevel,
					(g->GetZSize() * BlockPixels << g->GetScale()) >> ZoomLevel,
					g == SelectedGrid ? RGB(0, 0, 200) : RGB(0, 0, 0));
			}

			e = e->Next;
		} while (e != pDoc->GetGridList());
	}

	//
	// Draw surface type polygons.
	//

	if (DrawSurface) {
		
		pDC->SetPolyFillMode(WINDING);
		pDC->SetBkMode(TRANSPARENT);
		
//		HBITMAP	SurfacePattern = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_SURFFILL));
//		HBRUSH	SurfaceBrush = CreatePatternBrush(SurfacePattern);
//		HBRUSH	PreviousBrush = (HBRUSH) pDC->SelectObject(SurfaceBrush);
		
		PolygonElem*	p = pDoc->GetPolygonList();
		if (p) {
			do {
				// Draw the polygon.
				DrawPolygon(pDC, p->poly, /* pos, */ ZoomLevel);
				p = p->Next;
			} while (p != pDoc->GetPolygonList());
		}
		
//		pDC->SelectObject(PreviousBrush);
//		::DeleteObject(SurfacePattern);
//		::DeleteObject(SurfaceBrush);
	}

	//
	// Draw the objects.
	//

	if (DrawObjects) {
		ObjectIterator	it;
		pDoc->InitObjectListIterator(&it, ViewX0, ViewZ0, ViewX1, ViewZ1);
		Object*	o;
		while (o = pDoc->GetObjectAndAdvanceIterator(&it)) {
			// Draw the object.
			CPoint p = GetObjectPoint(o);
			
			o->Draw(pDC, p);
		}
	}
}


static void	XORCircle(CDC* dc, CPoint p, float radius)
// XOR a circle into the given device context.
{
	int	OldROP = dc->GetROP2();
	dc->SetROP2(R2_XORPEN);
	int	x = p.x - radius * DISPLAY_BLOCK_SIZE;
	int	y = p.y - radius * DISPLAY_BLOCK_SIZE;
	dc->Ellipse(x, y, x + radius * 2 * DISPLAY_BLOCK_SIZE, y + radius * 2 * DISPLAY_BLOCK_SIZE/* , x, p.y, x, p.y+5 */);
	dc->SetROP2(OldROP);
}


void	CTerrainView::ShowToolCursor(CPoint loc, float Radius)
// XOR's a circle onto our window at the specified location.  Keeps
// track of whether a previous circle needs to be erased or not.
{
	CDC*	dc = GetDC();
	if (dc == NULL) return;

	if (CursorVisible) {
		// Erase previous cursor.
		XORCircle(dc, LastCursorLocation, LastCursorRadius);
	}

	// Draw new cursor.
	XORCircle(dc, loc, Radius);

	// Remember what we drew.
	CursorVisible = true;
	LastCursorLocation = loc;
	LastCursorRadius = Radius;
}


void	CTerrainView::HideToolCursor()
// Erase the tool cursor if it's visible.
{
	if (CursorVisible) {
		CDC*	dc = GetDC();
		if (dc == NULL) return;
		
		// Erase previous cursor.
		XORCircle(dc, LastCursorLocation, LastCursorRadius);
		
		CursorVisible = false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTerrainView printing

BOOL CTerrainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTerrainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTerrainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTerrainView diagnostics

#ifdef _DEBUG
void CTerrainView::AssertValid() const
{
	CView::AssertValid();
}

void CTerrainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTerrainDoc* CTerrainView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTerrainDoc)));
	return (CTerrainDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CTerrainView message handlers

//void CTerrainView::OnInitialUpdate() 
//{
//	SetScrollSizes(MM_TEXT, GetDocument()->GetDocumentSize());
//
//	CView::OnInitialUpdate();
//	
//	// TODO: Add your specialized code here and/or call the base class
//	
//}


void CTerrainView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
// Called when the document changes. 
{
	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();

	int	BlockPixels = DISPLAY_BLOCK_SIZE;
	int	XSize = (((GetDocument()->GetXSize() - 1) * BlockPixels) >> ZoomLevel) + 1;
	int	YSize = (((GetDocument()->GetZSize() - 1) * BlockPixels) >> ZoomLevel) + 1;

//	SetScrollSizes(MM_TEXT, CSize(XSize, YSize));

	// Call the base class.
	CView::OnUpdate(pSender, lHint, pHint);
}

void CTerrainView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::CalcWindowRect(lpClientRect, nAdjustType);
}


void CTerrainView::OnZoomTerrainIn()
// Called when the user wants to zoom the view closer.
{
#ifdef NOT
	// TODO: Add your command handler code here
		
	ZoomLevel--;
	if (ZoomLevel < 0) ZoomLevel = 0;

	// Ought to invalidate something so the view gets repainted....
	GetDocument()->UpdateAllViews(NULL);
#endif // NOT
}


void CTerrainView::OnZoomTerrainOut()
// Called when the user wants to zoom the view out.
{
#ifdef NOT
	// TODO: Add your command handler code here
	
	ZoomLevel++;
	if (ZoomLevel > 12) ZoomLevel = 12;	// xxxx

	// Ought to invalidate something so the view gets repainted....
	GetDocument()->UpdateAllViews(NULL);
#endif // NOT
}


void CTerrainView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();

	// Figure out what tool is active.
	ToolPalette::ToolType	t;
	ToolPalette*	pal = GetFrame()->GetToolPalette();
	t = pal->GetCurrentTool();

	switch (t) {
	default:
		return;
		break;

	case ToolPalette::GRIDTOOL:
	{
		// Create a new grid with its origin at the mouse point.
		CTerrainDoc*	d = GetDocument();
		if (d) {
			Grid*	g = new Grid;
			float	x, z;
			ConvertWindowToWorld(point, &x, &z);
			int	scale = g->GetScale();
			g->SetXOrigin(int(x) >> scale);
			g->SetZOrigin(int(z) >> scale);
			d->AddGrid(g);
			SetSelectedGrid(g);
			d->SetModifiedFlag(TRUE);
			d->UpdateAllViews(NULL);	// Refresh views.
		}
		break;
	}

	case ToolPalette::OBJECTTOOL: {
		// Create object at the mouse point.
		GetDocument()->ClearObjectSelection();
		PlantObjects(point, true);
		break;
	}

	case ToolPalette::VERTEXTOOL: {
		// Add a new vertex to a polygon edge, if the cursor is near enough to one.
		AddEdgeVertexAtPoint(point);
		break;
	}

	case ToolPalette::EXCAVATE: {
		return;
		break;
	}

	case ToolPalette::PAINTSURFACE: {
		return;
		break;
	}

	case ToolPalette::HAND: {
		// Zoom in towards the mouse point.
		float	x, z;
		ConvertWindowToWorld(point, &x, &z);
		ViewCenterX = x;
		ViewCenterZ = z;
		pal->SetZoomLevel(pal->GetZoomLevel() - 1);
		break;
	}
	}
	
	// Make sure views get repainted.
	GetDocument()->UpdateAllViews(NULL);
}


void CTerrainView::OnNewGrid() 
// Called when the user presses the New Grid button on the toolbar.
{
	CTerrainDoc*	d = GetDocument();
	if (d) {
		Grid*	g = new Grid;
		d->AddGrid(g);
		SetSelectedGrid(g);
		d->SetModifiedFlag(TRUE);
		d->UpdateAllViews(NULL);	// Refresh views.
	}
}


void CTerrainView::OnAddNewPolygon() 
// Called when the user presses the New Polygon button on the toolbar.
{
	CTerrainDoc*	d = GetDocument();
	if (d) {
		// Create a new polygon.  Position it near the center of the view,
		// and scale it according to the current zoom.
		int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();
		int	size = 30 << ZoomLevel;
		int	x = ViewX0 + size;
		int	z = ViewZ0 + size;
		if (x - size < 0) x = size;
		if (z - size < 0) z = size;

		polygon*	p = new polygon(x, z, size);
		d->AddPolygon(p);
		d->ClearPolygonSelection();
		p->SetSelected(true);
		d->UpdateAllViews(NULL);
	}
}


Grid*	CTerrainView::GetGridAtPoint(CPoint point)
// Return the grid, if any, under the given point in the view.
// If there's more than one grid, returns the smallest candidate.
{
	// Identify the smallest grid under the cursor, and return it.
	Grid*	g = NULL;

	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();
	int	x, z;
	x = ((point.x / DISPLAY_BLOCK_SIZE) << ZoomLevel) + ViewX0;
	z = ((point.y / DISPLAY_BLOCK_SIZE) << ZoomLevel) + ViewZ0;
	
	float	MinArea = FLT_MAX;
	
	// Examine the grid list for the most appropriate grid.
	GridElem*	GridList = GetDocument()->GetGridList();
	GridElem*	e = GridList;
	if (e) {
		do {
			int	scale = e->Grid->GetScale();
			if (x >= e->Grid->GetXOrigin() << scale &&
				x < (e->Grid->GetXOrigin() + e->Grid->GetXSize()) << scale &&
				z >= e->Grid->GetZOrigin() << scale &&
				z < (e->Grid->GetZOrigin() + e->Grid->GetZSize()) << scale)
			{
				float	Area = float(e->Grid->GetXSize()) * float(e->Grid->GetZSize()) * float(1 << (e->Grid->GetScale() << 1));
				if (Area < MinArea) {
					g = e->Grid;
					MinArea = Area;
				}
			}
			
			e = e->Next;
		} while (e != GridList);
	}

	return g;
}


Object* CTerrainView::GetObjectAtPoint(CPoint point)
// Return the object, if any, under the given point in the view.
// Returns NULL if there's no candidate object.
{
//	ObjectElem*	ObjectList = GetDocument()->GetObjectList();
//	ObjectElem*	e = ObjectList;
//
//	if (e) {
//		do {

	float	x0, z0, x1, z1;
	CPoint	p0(point.x - 3, point.y - 3);
	CPoint	p1(point.x + 3, point.y + 3);
	ConvertWindowToWorld(p0, &x0, &z0);
	ConvertWindowToWorld(p1, &x1, &z1);
	
	CTerrainDoc*	doc = GetDocument();
	ObjectIterator	it;
	doc->InitObjectListIterator(&it, x0, z0, x1, z1);
	Object*	o;
	while (o = doc->GetObjectAndAdvanceIterator(&it)) {
		// Check object against point.
		CPoint	p = GetObjectPoint(o);
		int	dx = abs(p.x - point.x);
		int	dy = abs(p.y - point.y);
		if (dx < 3 && dy < 3) {
			return o;
		}
	}

//			e = e->Next;
//		} while (e != ObjectList);
//	}

	return NULL;
}


Vertex*	CTerrainView::GetVertexAtPoint(polygon** Poly, CPoint point)
// Return the vertex, if any, under the given point in the view.
// Returns NULL if there's no good candidate vertex.
// Stores a pointer to the polygon the vertex belongs to in *Poly.  If Poly is NULL,
// does not store the data.
{
	PolygonElem*	PolyList = GetDocument()->GetPolygonList();
	PolygonElem*	e = PolyList;

	float	x, z;
	ConvertWindowToWorld(point, &x, &z);

	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();

	// Find the vertex closest to the point.  Ignore vertices beyond a threshold
	// distance away.

	Vertex*	BestVertex = NULL;
	float	ClosestDistance = 2 * (1 << ZoomLevel);

	if (e) {
		do {
			float	Distance;

			Vertex*	v = e->poly->FindNearestVertex(&Distance, x, z);

			if (v && Distance < ClosestDistance) {
				ClosestDistance = Distance;
				BestVertex = v;
				if (Poly) *Poly = e->poly;
			}

			e = e->Next;
		} while (e != PolyList);
	}

	return BestVertex;
}


void	CTerrainView::AddEdgeVertexAtPoint(CPoint point)
// Finds the polygon edge nearest to the given point, and adds a vertex there
// if the distance is below a threshold.
{
	PolygonElem*	PolyList = GetDocument()->GetPolygonList();
	PolygonElem*	e = PolyList;

	float	x, z;
	ConvertWindowToWorld(point, &x, &z);

	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();

	// Find the edge closest to the point.  Ignore edges beyond a threshold
	// distance away.

	Vertex*	BestEdgeVertex = NULL;
	polygon*	p = NULL;
	float	ClosestDistance = 2 * (1 << ZoomLevel);

	if (e) {
		do {
			float	Distance;

			Vertex*	v = e->poly->FindNearestEdgeVertex(&Distance, x, z);

			if (v && Distance < ClosestDistance) {
				ClosestDistance = Distance;
				BestEdgeVertex = v;
				p = e->poly;
			}

			e = e->Next;
		} while (e != PolyList);
	}

	if (p && BestEdgeVertex) {
		p->AddVertexAfter(BestEdgeVertex, x, z);

		GetDocument()->ClearPolygonSelection();
		p->SetSelected(true);
	}
}


CPoint	CTerrainView::GetObjectPoint(Object* o)
// Returns the location of the given object mapped into this view.
{
	CPoint	p;

	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();

	p.x = int((o->GetX() - ViewX0) * DISPLAY_BLOCK_SIZE) >> ZoomLevel;
	p.y = int((o->GetZ() - ViewZ0) * DISPLAY_BLOCK_SIZE) >> ZoomLevel;

	return p;
}


void CTerrainView::OnLButtonDown(UINT nFlags, CPoint point) 
// Called on left-button down.
{
	// Figure out what tool is active.
	ToolPalette::ToolType	t;
	t = GetFrame()->GetToolPalette()->GetCurrentTool();

	switch (t) {
	default:
		break;

	case ToolPalette::PLANTOBJECTS: {
		PlantObjects(point, true);
		CurrentAction = SPRAYING_OBJECTS;
		SetCapture();
		break;
	}

	case ToolPalette::GRIDTOOL:
	{
		// Select the grid under the mouse point.
		Grid*	g = GetGridAtPoint(point);
		if (g) {
			// Select this grid.
			CChildFrame* parent = GetFrame();
			if (parent) {
				SetSelectedGrid(g);
			}

			// Start moving this grid.
			MovingGrid = g;
			GridStartPoint = point;
			AllowGridMove = false;
			CurrentAction = MOVING_GRID;
		}
		break;
	}

	case ToolPalette::OBJECTTOOL: {
		Object*	o = GetObjectAtPoint(point);
		if (o) {
			if (nFlags & (MK_CONTROL | MK_SHIFT)) {
				if (nFlags & MK_CONTROL) {
					o->SetSelected(!o->GetSelected());
				} else {
					o->SetSelected(true);
				}
				GetDocument()->UpdateAllViews(NULL);
				CurrentAction = NO_ACTION;
			} else if (o->GetSelected()) {
				CurrentAction = MOVING_OBJECTS;
				SetCapture();
				MoveStart = point;
			} else {
				GetDocument()->ClearObjectSelection();
				o->SetSelected(true);
				GetDocument()->UpdateAllViews(NULL);
				CurrentAction = NO_ACTION;
			}
		} else {
			// If there's no object under the mouse then we want to start
			// a rubber-band box.
			BoxAnchor = point;
			BoxCorner = point;
			XORRubberBandBox(BoxAnchor, BoxCorner);
			CurrentAction = RUBBER_BAND_OBJECT_SELECT;
			SetCapture();
		}
		break;
	}

	case ToolPalette::EXCAVATE:
		if (CaptureMode != NONE && CaptureMode != LEFT_BUTTON) {
//			apply the stroke;
		}

		CaptureMode = LEFT_BUTTON;
		CapturePointCount = 1;
		CapturePoint[0] = point;
		CurrentAction = EXCAVATING;

		break;

	case ToolPalette::PAINTSURFACE:
		if (CaptureMode != NONE && CaptureMode != LEFT_BUTTON) {
//			apply the stroke;
		}

		CaptureMode = LEFT_BUTTON;
		CapturePointCount = 1;
		CapturePoint[0] = point;
		CurrentAction = PAINTING_SURFACE;

		break;

	case ToolPalette::VERTEXTOOL: {
		// Move vertex, if there's one near the mouse point.
		polygon*	p = NULL;
		MovingVertex = GetVertexAtPoint(&p, point);
		if (MovingVertex) {
			CurrentAction = MOVING_VERTEX;
			GetDocument()->ClearPolygonSelection();
			p->SetSelected(true);
		}
		
		break;
	}

	case ToolPalette::HAND:
		// Beginning of dragging the view...
		CurrentAction = DRAGGING_VIEW;
		DragLastPoint = point;
		SetCapture();
		break;
	}
}


void	CTerrainView::ConvertWindowToWorld(CPoint p, float* px, float* pz)
// Converts the given window coordinates to world coordinates and stuffs
// them into the given x and z locations.
{
	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();

	*px = (p.x << ZoomLevel) / DISPLAY_BLOCK_SIZE + ViewX0;
	*pz = (p.y << ZoomLevel) / DISPLAY_BLOCK_SIZE + ViewZ0;
}


void	CTerrainView::PlantObjects(CPoint point, bool Initial)
// Plants an object (or objects) at the specified point in the window.
// Uses the planting parameters stored in the ToolPalette to determine
// the parameters of the object(s), and how many objects to plant.
// Initial should be true if this operation results from the initial mouse-down event.
// If spraying is disabled, and Initial is false, then no planting is done.
{
	// Get parameters.
	ToolPalette::PlantParameters	p;
	p = GetFrame()->GetToolPalette()->GetPlantParameters();

	// Don't plant if this isn't a mouse click and we're not spraying.
	if (p.EnableSpray == false && Initial == false) return;

	CTerrainDoc*	doc = GetDocument();

	if (p.EnableSpray) {
		// Spray a bunch of objects...
		for (int i = 0; i < p.Count; i++) {
			Object*	o = new Object;
			o->SetType(p.ObjectType);
			o->SetGeometryReference(p.ObjectGeometry);
			o->SetSolidReference(p.ObjectSolid);
			float	x, z;
			ConvertWindowToWorld(point, &x, &z);

			// Randomize the location.
			float	theta, r;
			theta = float(rand()) / RAND_MAX * 2 * PI;
			r = float(rand()) / RAND_MAX;
			r = sqrt(r) * p.Radius;
			x += r * cos(theta);
			z += r * sin(theta);
			o->SetLocation(doc, x, p.ObjectY, z);

			o->SetReferenceToGround(p.PinToGround);
			
			// Add the object.
			doc->AddObject(o);
		}
	} else {
		// Plant a single object at the mouse point.
		Object*	o = new Object;
		o->SetType(p.ObjectType);
		o->SetGeometryReference(p.ObjectGeometry);
		o->SetSolidReference(p.ObjectSolid);
		float	x, z;
		ConvertWindowToWorld(point, &x, &z);
		o->SetReferenceToGround(p.PinToGround);
		o->SetLocation(doc, x, p.ObjectY, z);

		// Add the object.
		doc->AddObject(o);
	}

	doc->SetModifiedFlag(true);
//	doc->UpdateAllViews(NULL);	// Refresh views.
}


void	CTerrainView::EraseObjects(CPoint point, bool Initial)
// Erases objects near the specified point in the window.
// Uses the planting parameters stored in the ToolPalette to determine
// the radius of the eraser, and the probability of erasing a given object.
// Initial should be true if this operation results from the initial mouse-down event.
// If spraying is disabled, and Initial is false, then no erasing is done.
{
	// Get parameters.
	ToolPalette::PlantParameters	p;
	p = GetFrame()->GetToolPalette()->GetPlantParameters();

	// Don't erase if this isn't a mouse click and we're not spraying.
	if (p.EnableSpray == false && Initial == false) return;

	CTerrainDoc*	doc = GetDocument();

	if (p.EnableSpray) {
		// Erase nearby objects...
		float	x, z;
		ConvertWindowToWorld(point, &x, &z);

		// Look at objects within the eraser's radius.
		float	radius2 = p.Radius * p.Radius;
		ObjectIterator	it;
		doc->InitObjectListIterator(&it, x - p.Radius, z - p.Radius, x + p.Radius, z + p.Radius);
		Object*	o;

		while (o = doc->GetObjectAndAdvanceIterator(&it)) {
			// Make sure object is within radius.
			float r2 = (x - o->GetX()) * (x - o->GetX()) + (z - o->GetZ()) * (z - o->GetZ());
			if (r2 > radius2) continue;
			
			// Roll the dice to see if we should delete this object.
			if (float(rand()) / RAND_MAX * 100 > p.ErasePercent) continue;

			// Delete the object.
			doc->DeleteObject(o);
		}

		doc->SetModifiedFlag(TRUE);
	} else {
		// Delete a single object at the mouse point.
		DeleteObject(point);
	}
}


void	CTerrainView::DeleteObject(CPoint point)
// Deletes the object, if any, under the specified point in the window.
{
	Object*	o = GetObjectAtPoint(point);

	if (o) {
		CTerrainDoc*	d = GetDocument();
		if (d) {
			d->DeleteObject(o);

			d->SetModifiedFlag(TRUE);

//			// Redraw.
//			d->UpdateAllViews(NULL);	// Refresh views.
		}
	}
}


CChildFrame* CTerrainView::GetFrame()
// Gets our frame.  You can use the frame to get pointers to the various
// views and the tool palette.
{
	return dynamic_cast<CChildFrame*>(GetParent()->GetParent());
}


void CTerrainView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	switch (CurrentAction) {
	default:
	case NO_ACTION:
		CurrentAction = NO_ACTION;
		break;

	case RUBBER_BAND_OBJECT_SELECT: {
		// Erase box.
		XORRubberBandBox(BoxAnchor, BoxCorner);

		bool	Toggle = false;
		if (nFlags & MK_CONTROL) {
			Toggle = true;
		} else if (nFlags & MK_SHIFT) {
			Toggle = false;
		} else {
			// If user doesn't hold CTRL or shift, then clear other selected objects.
			GetDocument()->ClearObjectSelection();
		}

		SelectObjectsInBox(BoxAnchor, point, Toggle);

		GetDocument()->UpdateAllViews(NULL);
		CurrentAction = NO_ACTION;
		break;
	}

	case MOVING_OBJECTS:
		// xxx do some kind of thresholding thing (in OnMouseMove), to prevent accidental movement if mouse hasn't moved far.
		float	x0, z0;
		ConvertWindowToWorld(MoveStart, &x0, &z0);
		float x1, z1;
		ConvertWindowToWorld(point, &x1, &z1);

		MoveSelectedObjects(x1 - x0, z1 - z0);

		GetDocument()->UpdateAllViews(NULL);
		CurrentAction = NO_ACTION;
		break;

	case MOVING_GRID:
		if (MovingGrid && AllowGridMove) {
			float	x0, z0;
			float	dx, dz;
			ConvertWindowToWorld(GridStartPoint, &x0, &z0);
			ConvertWindowToWorld(point, &dx, &dz);
			dx -= x0;
			dz -= z0;
			int	scale = MovingGrid->GetScale();
			MovingGrid->SetXOrigin(MovingGrid->GetXOrigin() + (int(dx) >> scale));
			MovingGrid->SetZOrigin(MovingGrid->GetZOrigin() + (int(dz) >> scale));

			GetDocument()->ReSortGrid(MovingGrid);
			
			SetSelectedGrid(MovingGrid);	// Make sure grid dialog has new values.

			GetDocument()->SetModifiedFlag(TRUE);
		}
		GetDocument()->UpdateAllViews(NULL);
		CurrentAction = NO_ACTION;
		break;

	case EXCAVATING:
		if (CaptureMode == LEFT_BUTTON) {
			ApplyStroke();
			
			// Make sure views get repainted.
			GetDocument()->SetModifiedFlag();
			GetDocument()->UpdateAllViews(NULL);
		}

		CapturePointCount = 0;
		CaptureMode = NONE;

		CurrentAction = NO_ACTION;
		break;

	case PAINTING_SURFACE:
		if (CaptureMode == LEFT_BUTTON) {
			ApplyPaintStroke();
			
			GetDocument()->SetModifiedFlag();
			GetDocument()->UpdateAllViews(NULL);
		}
		
		CapturePointCount = 0;
		CaptureMode = NONE;

		CurrentAction = NO_ACTION;
		break;

	case MOVING_VERTEX:
		if (MovingVertex) {
			float x, z;
			ConvertWindowToWorld(point, &x, &z);
			MovingVertex->x = x;
			MovingVertex->z = z;

			GetDocument()->UpdateAllViews(NULL);
		}
		CurrentAction = NO_ACTION;
		break;

	case DRAGGING_VIEW:
		CurrentAction = NO_ACTION;
		break;

	case SPRAYING_OBJECTS:
		CurrentAction = NO_ACTION;
		GetDocument()->UpdateAllViews(NULL);
		break;
	}

	ReleaseCapture();
}


void CTerrainView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// Figure out what tool is active.
	ToolPalette::ToolType	t;
	t = GetFrame()->GetToolPalette()->GetCurrentTool();

	switch (t) {
	default:
		break;

	case ToolPalette::GRIDTOOL: {
		// Delete the grid under the mouse...
		Grid* g = GetGridAtPoint(point);
		if (g == SelectedGrid) SelectedGrid = NULL;

		if (g) {
			// Make the user confirm.
			ConfirmDeleteDialog	d("grid");
			if (d.DoModal() == IDOK) {
				GetDocument()->DeleteGrid(g);
				GetDocument()->SetModifiedFlag();
				GetDocument()->UpdateAllViews(NULL);
			}
		}
		break;
	}

	case ToolPalette::OBJECTTOOL:
		// The delete the object under the mouse.
		DeleteObject(point);
		GetDocument()->UpdateAllViews(NULL);
		break;

	case ToolPalette::EXCAVATE:
		if (CaptureMode != NONE && CaptureMode != RIGHT_BUTTON) {
//			apply the stroke;
		}

		if (ExcavateParams.Mode == ExcavateParameters::CLONE) {
			// Get the height under the mouse point.
			ExcavateParams.ReferenceHeight = GetHeightAtPoint(point);
		} else {
			CaptureMode = RIGHT_BUTTON;
			CapturePointCount = 1;
			CapturePoint[0] = point;
			
			CurrentAction = EXCAVATING;
		}

		break;

	case ToolPalette::PAINTSURFACE:
		if (CaptureMode != NONE && CaptureMode != RIGHT_BUTTON) {
//			apply the stroke;
		}

		CaptureMode = RIGHT_BUTTON;
		CapturePointCount = 1;
		CapturePoint[0] = point;
		
		CurrentAction = PAINTING_SURFACE;

		break;

	case ToolPalette::VERTEXTOOL: {
		// Delete the nearest vertex, if one is close enough.
		polygon*	Poly = NULL;
		Vertex*	VertexToDelete = GetVertexAtPoint(&Poly, point);

		if (VertexToDelete && Poly) {
			GetDocument()->ClearPolygonSelection();
			
			// Delete the vertex.
			if (Poly->DeleteVertex(VertexToDelete)) {
				// Eliminate the polygon if it has become degenerate.
				GetDocument()->DeletePolygon(Poly);
			} else {
				// Polygon is still here; select it.
				Poly->SetSelected(true);
			}

			GetDocument()->UpdateAllViews(NULL);
		}
		break;

	}
	
	case ToolPalette::PLANTOBJECTS: {
		// Erase some objects.
		EraseObjects(point, true);
		CurrentAction = ERASING_OBJECTS;
		SetCapture();
		break;
	}

	}
}


void CTerrainView::OnRButtonUp(UINT nFlags, CPoint point) 
{
//	// TODO: Add your message handler code here and/or call default
//	
//	CView::OnRButtonUp(nFlags, point);

//	// Figure out what tool is active.
//	ToolPalette::ToolType	t;
//	t = GetFrame()->GetToolPalette()->GetCurrentTool();
//
//	switch (t) {
	switch (CurrentAction) {
	default:
		CurrentAction = NO_ACTION;
		break;

	case EXCAVATING:
		if (CaptureMode == RIGHT_BUTTON) {
			ApplyStroke();
			
			// Make sure views get repainted.
			GetDocument()->SetModifiedFlag();
			GetDocument()->UpdateAllViews(NULL);
		}
		
		CapturePointCount = 0;
		CaptureMode = NONE;

		CurrentAction = NO_ACTION;

		break;

	case PAINTING_SURFACE:
		if (CaptureMode == RIGHT_BUTTON) {
			ApplyPaintStroke();
			
			// Make sure views get repainted.
			GetDocument()->SetModifiedFlag();
			GetDocument()->UpdateAllViews(NULL);
		}
		
		CapturePointCount = 0;
		CaptureMode = NONE;

		CurrentAction = NO_ACTION;

		break;

	case ERASING_OBJECTS:
		CurrentAction = NO_ACTION;
		GetDocument()->UpdateAllViews(NULL);
		break;
	}

	ReleaseCapture();
}


void CTerrainView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (GetFrame()->GetToolPalette()->GetCurrentTool() == ToolPalette::EXCAVATE &&
	    GetFrame()->GetToolPalette()->GetShowCursor() == true)
	{
		// Show the excavation cursor.
//		while (ShowCursor(FALSE) >= -1) ;
//		while (ShowCursor(TRUE) < -1) ;
		
		ShowToolCursor(point, ExcavateParams.Radius);
	} else {
//		while (ShowCursor(TRUE) < 1) ;
//		while (ShowCursor(FALSE) >= 1) ;
	}
	
	if (CaptureMode != NONE) {
		if (CapturePointCount >= GRIDVIEW_MAX_CAPTURE_POINTS) {
			// We've filled up our stroke storage, so apply this one
			// and start a new one.

			if (CurrentAction == EXCAVATING) ApplyStroke();
			else if (CurrentAction == PAINTING_SURFACE) ApplyPaintStroke();
			GetDocument()->SetModifiedFlag();
			GetDocument()->UpdateAllViews(NULL);

			// Start a new stroke.
			CapturePointCount = 1;
			CapturePoint[0] = point;
		} else {
			CapturePoint[CapturePointCount] = point;
			CapturePointCount++;
		}
	} else if (CurrentAction == DRAGGING_VIEW) {
		if ((nFlags & MK_LBUTTON) == 0) {
			CurrentAction = NO_ACTION;
		} else {
			// Drag the view according to the mouse motion.
			CPoint	delta = point - DragLastPoint;
			DragLastPoint = point;
			int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();
			ViewCenterX += (-delta.x << ZoomLevel) / DISPLAY_BLOCK_SIZE;
			ViewCenterZ += (-delta.y << ZoomLevel) / DISPLAY_BLOCK_SIZE;
			
			// Clamp.
			CTerrainDoc*	doc = GetDocument();
			if (ViewCenterX < 0) ViewCenterX = 0;
			if (ViewCenterX > doc->GetXSize()) ViewCenterX = doc->GetXSize();
			if (ViewCenterZ < 0) ViewCenterZ = 0;
			if (ViewCenterZ > doc->GetZSize()) ViewCenterZ = doc->GetZSize();

			ComputeOrigin();
			
			doc->UpdateAllViews(NULL);
		}
	} else if (CurrentAction == RUBBER_BAND_OBJECT_SELECT) {
		// Erase old box.
		XORRubberBandBox(BoxAnchor, BoxCorner);
		// Move corner.
		BoxCorner = point;
		// Draw new box.
		XORRubberBandBox(BoxAnchor, BoxCorner);

	} else if (CurrentAction == SPRAYING_OBJECTS) {
		PlantObjects(point, false);
	} else if (CurrentAction == ERASING_OBJECTS) {
		EraseObjects(point, false);
	} else if (CurrentAction == MOVING_GRID) {
		// If the mouse moves beyond some threshold distance from the
		// starting point of the drag, then assume the user really does
		// want to move the grid.  Otherwise, we'll assume they don't
		// want to move it.
		if (abs(point.x - GridStartPoint.x) > 10 || abs(point.y - GridStartPoint.y) > 10) {
			AllowGridMove = true;
		}
	}
	
	{
		// jbl 08/18/98:  On a normal non-captured mouse move,
		// show XYZ in the status bar.

		int	x, z;
		float y = GetHeightAtPoint(point, &x, &z) + BaseElevation;
		char s[100];
		sprintf(s, "x=%d, y=%6.1f, z=%d", x, y, z);
		
		// Get the child frame's parent MDI frame, which has
		// access to the status bar via SetMessageText().
		CFrameWnd *topframe = GetFrame()->GetMDIFrame();
		topframe->SetMessageText(s);
	}
}


float	CTerrainView::GetHeightAtPoint(CPoint& point, int* px, int *pz)
// Returns the height of the terrain under the point.
// If px and/or pz are non-NULL, puts the world coordinates of the mouse point
// in *px and/or *pz.
{
	// Figure out the current dimensions and block size for the view.
	int BlockPixels = DISPLAY_BLOCK_SIZE;
	CChildFrame* childframe = GetFrame();
	int ZoomLevel = childframe->GetToolPalette()->GetZoomLevel();

	int x0, z0, xsize, zsize;

	x0 = ViewX0;
	z0 = ViewZ0;

	CTerrainDoc* pDoc = GetDocument();
	xsize = pDoc->GetXSize();
	zsize = pDoc->GetZSize();

	int x = x0 + (point.x << ZoomLevel) / BlockPixels;
	int z = z0 + (point.y << ZoomLevel) / BlockPixels;
	float	y = 0;
	if (x < xsize && z < zsize) {
		y = pDoc->GetHeight(float(x), float(z));
	}

	if (px) *px = x;
	if (pz) *pz = z;

	return y;
}


Grid*	CTerrainView::GetSelectedGrid()
// Returns the currently selected grid.
{
	return SelectedGrid;
}


void	CTerrainView::SetSelectedGrid(Grid* Selected)
// Sets the selected grid, for editing.
{
	SelectedGrid = Selected;

	// Make sure the form view loads its parameters from the new grid.
	CChildFrame* parent = dynamic_cast<CChildFrame*>(GetParent()->GetParent()/*->GetParent()*/);
	if (parent) {
		GridFormView*	fv = parent->GetGridFormView();
		if (fv) {
			fv->ReadGridValues();
		}
	}
}
 
 
static int	clamp(int low, int i, int hi)
{
	if (i < low) i = low;
	if (i > hi) i = hi;
	return i;
}


void	CTerrainView::DrawStrokeInScratchArray(float Radius, float EdgeTaper)
// Using the given parameters as well as the stored stroke sample points, this
// function paints influence coefficients (between 0 and 1) into the scratch array, which
// can then be combined with the terrain height data as desired.
{
	int	i, j;

	float	MaxDist2 = Radius * Radius;

	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();
	int	xsize = (ViewX1 - ViewX0) >> ZoomLevel;
	int	zsize = (ViewZ1 - ViewZ0) >> ZoomLevel;
	if (xsize > TEMP_DIM) xsize = TEMP_DIM;
	if (zsize > TEMP_DIM) zsize = TEMP_DIM;
	
	// Clear the scratch data.
	for (j = 0; j < zsize; j++) {
		for (i = 0; i < xsize; i++) {
			TempGrid[j][i] = 0;
		}
	}

	// Apply max function to points surrounding each stroke sample.
	float	GridBlockPixels = DISPLAY_BLOCK_SIZE;

	int	sample;
	for (sample = 0; sample < CapturePointCount; sample++) {
		float	sx0 = float(CapturePoint[sample].x) / GridBlockPixels;
		float	sz0 = float(CapturePoint[sample].y) / GridBlockPixels;
		float	sx1;
		float	sz1;
		float	dx, dz;
		int	count;

		if (sample < CapturePointCount - 1) {
			sx1 = float(CapturePoint[sample + 1].x) / GridBlockPixels;
			sz1 = float(CapturePoint[sample + 1].y) / GridBlockPixels;
			
			// Interpolate between samples.
			dx = sx1 - sx0;
			dz = sz1 - sz0;
			int idx = abs(int(dx + 1));
			int idz = abs(int(dz + 1));
			count = idx;
			if (count < idz) count = idz;
			if (count <= 0) count = 1;
			dx /= count;
			dz /= count;
		} else {
			// Just the endpoint; make sure we apply it.
			dx = dz = 0;
			count = 1;
		}

		for (i = 0; i < count; i++) {
			int	x0 = clamp(0, int(sx0 + 0.5) - Radius + 1, xsize-1);
			int	z0 = clamp(0, int(sz0 + 0.5) - Radius + 1, zsize-1);
			int	x1 = clamp(x0, int(sx0 + 0.5) + Radius + 1, xsize-1);
			int	z1 = clamp(z0, int(sz0 + 0.5) + Radius + 1, zsize-1);
			
			// For the points near the sample, compute the influence
			// and update the scratch array.
			int	x, z;
			for (z = z0; z <= z1; z++) {
				for (x = x0; x <= x1; x++) {
					float	Distance2 = (sx0 - x) * (sx0 - x) + (sz0 - z) * (sz0 - z);
					float	Influence = 1 - pow(Distance2 / MaxDist2, EdgeTaper);
					
					// Apply max function.
					if (TempGrid[z][x] < Influence) {
						TempGrid[z][x] = Influence;
					}
				}
			}

			sx0 += dx;
			sz0 += dz;
		}
	}
}


void	CTerrainView::DrawGridStrokeInScratchArray(Grid* g, float Radius, float EdgeTaper)
// Using the given parameters as well as the stored stroke sample points, this
// function paints influence coefficients (between 0 and 1) into the scratch array, which
// can then be combined with the specified grid data as desired.
{
	int	i, j;

	float	MaxDist2 = Radius * Radius;

	// Clear the scratch data.
	for (j = 0; j < TEMP_DIM; j++) {
		for (i = 0; i < TEMP_DIM; i++) {
			TempGrid[j][i] = 0;
		}
	}

	// Apply max function to points surrounding each stroke sample.
	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();
	float	GridBlockPixels = float(DISPLAY_BLOCK_SIZE * (1 << g->GetScale())) / (1 << ZoomLevel);
	float	XOffset = int(((g->GetXOrigin() << g->GetScale()) - ViewX0) * DISPLAY_BLOCK_SIZE) >> ZoomLevel;
	float	ZOffset = int(((g->GetZOrigin() << g->GetScale()) - ViewZ0) * DISPLAY_BLOCK_SIZE) >> ZoomLevel;

	int	sample;
	for (sample = 0; sample < CapturePointCount; sample++) {
		float	sx0 = float(CapturePoint[sample].x - XOffset) / GridBlockPixels;
		float	sz0 = float(CapturePoint[sample].y - ZOffset) / GridBlockPixels;
		float	sx1;
		float	sz1;
		float	dx, dz;
		int	count;

		if (sample < CapturePointCount - 1) {
			sx1 = float(CapturePoint[sample + 1].x - XOffset) / GridBlockPixels;
			sz1 = float(CapturePoint[sample + 1].y - ZOffset) / GridBlockPixels;
			
			// Interpolate between samples.
			dx = sx1 - sx0;
			dz = sz1 - sz0;
			int idx = abs(int(dx + 1));
			int idz = abs(int(dz + 1));
			count = idx;
			if (count < idz) count = idz;
			if (count <= 0) count = 1;
			dx /= count;
			dz /= count;
		} else {
			// Just the endpoint; make sure we apply it.
			dx = dz = 0;
			count = 1;
		}

		for (i = 0; i < count; i++) {
			int	x0 = clamp(0, int(sx0 + 0.5) - Radius + 1, g->GetXSize()-1);
			int	z0 = clamp(0, int(sz0 + 0.5) - Radius + 1, g->GetZSize()-1);
			int	x1 = clamp(x0, int(sx0 + 0.5) + Radius + 1, g->GetXSize()-1);
			int	z1 = clamp(z0, int(sz0 + 0.5) + Radius + 1, g->GetZSize()-1);
			
			// For the points near the sample, compute the influence
			// and update the scratch array.
			int	x, z;
			for (z = z0; z <= z1; z++) {
				for (x = x0; x <= x1; x++) {
					float	Distance2 = (sx0 - x) * (sx0 - x) + (sz0 - z) * (sz0 - z);
					float	Influence = 1 - pow(Distance2 / MaxDist2, EdgeTaper);
					
					// Apply max function.
					if (TempGrid[z][x] < Influence) {
						TempGrid[z][x] = Influence;
					}
				}
			}

			sx0 += dx;
			sz0 += dz;
		}
	}
}


void	CTerrainView::ApplyPaintStroke()
// Applies a recorded stroke to the grid surface type data.
{
	Grid*	g = SelectedGrid;	// Shorthand.
	if (g == NULL) return;

	PaintParameters&	p = GetFrame()->GetPaintParametersView()->Params;
	float	Radius = p.Radius;

	DrawGridStrokeInScratchArray(g, Radius, 10);

	// Paint the surface, using the computed influence data.

	// Determine the type to paint.
	unsigned char	t;
	if (CaptureMode == LEFT_BUTTON) t = p.LeftButtonType;
	else t = p.RightButtonType;

	// Paint.
	int	x, z;
	unsigned char*	TypeData = g->GetTypeData();

	int	xsize = g->GetXSize();
	int	zsize = g->GetZSize();

	unsigned char*	q = TypeData;
	for (z = 0; z < zsize; z++) {
		for (x = 0; x < xsize; x++, q++) {
			if (TempGrid[z][x] > 0.5) {
				*q = t;
			}
		}
	}
}


void	CTerrainView::ComputeDeltaExcavateAddHeight(int xsize, int zsize)
// Computes delta heights in TempGrid[][], from the influence data already
// in the array.
{
	float	Height = 0;
	if (CaptureMode == LEFT_BUTTON) Height += ExcavateParams.Depth;
	if (CaptureMode == RIGHT_BUTTON) Height += -ExcavateParams.Depth;

	int	x, z;
	for (z = 0; z < zsize; z++) {
		for (x = 0; x < xsize; x++) {
			TempGrid[z][x] *= Height;
		}
	}
}


void CTerrainView::ComputeDeltaExcavateClone(int xsize, int zsize)
// Computes delta heights in TempGrid[][] from the influence data already in there,
// so that when added to the existing height grid the influenced results will tend towards
// ExcavateParams.ReferenceHeight.
{
	float	Height = ExcavateParams.ReferenceHeight;
	if (CaptureMode == RIGHT_BUTTON) return;

	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();
	CTerrainDoc*	doc = GetDocument();
	int	x, z;
	for (z = 0; z < zsize; z++) {
		for (x = 0; x < xsize; x++) {
			float	f = TempGrid[z][x];
			float	val = doc->GetHeight((x << ZoomLevel) + ViewX0, (z << ZoomLevel) + ViewZ0);
			TempGrid[z][x] = (f * Height + (1-f) * val) - val;
		}
	}
}


void CTerrainView::ComputeDeltaExcavateSmoothing(int xsize, int zsize, float Radius)
// Computes delta heights in TempGrid[][], from the influence data already
// in the array.  Where the data has influence, compute deltas to smooth it in
// relation to the surrounding data.
{
	float	Coeff = 1.0;
	if (CaptureMode == RIGHT_BUTTON) {
		// Sharpen... deviate away from average value.
		Coeff = -1.0 * ExcavateParams.Smoothing;
	} else {
		// Smooth... deviate towards average value.
		Coeff = 1.0 * ExcavateParams.Smoothing;
	}
	
	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();
	CTerrainDoc*	doc = GetDocument();
	int	x, z;

	for (z = 0; z < zsize; z++) {
		for (x = 0; x < xsize; x++) {
			float	f = TempGrid[z][x];
			if (f > 0) {
				float	sx = ViewX0 + (x << ZoomLevel);
				float	sz = ViewZ0 + (z << ZoomLevel);
				float	avgval = (doc->GetHeight(sx + Radius, sz) +
					doc->GetHeight(sx, sz - Radius) +
					doc->GetHeight(sx - Radius, sz) +
					doc->GetHeight(sx, sz + Radius)) * 0.25;
				float	delt = avgval - doc->GetHeight(sx, sz);
				TempGrid[z][x] = f * delt * Coeff;
			} else {
				TempGrid[z][x] = 0;
			}
		}
	}
}


void	CTerrainView::ApplyModulation(int xsize, int zsize, bitmap8* ModulationMap, int xoffset, int zoffset)
// Given a bitmap of 8-bit modulation values, this function multiplies the
// contents of the TempGrid by the coefficients stored in the given
// modulation map.  The map coordinates are offset by the given
// xoffset and zoffset.
{
	int	x, z;

	int	width = ModulationMap->GetWidth();
	int	height = ModulationMap->GetHeight();

	xoffset %= width;
	zoffset %= height;
	
	int	row = zoffset;
	for (z = 0; z < zsize; z++) {
		uint8*	rowstart = ModulationMap->GetData() + row * ModulationMap->GetWidth();
		int	col = xoffset;
		uint8*	p = rowstart + xoffset;
		for (x = 0; x < xsize; x++) {
			TempGrid[z][x] *= *p / 255.0;
			p++;

			col++;
			if (col >= width) {
				col = 0;
				p = rowstart;
			}
		}
		row++;
		if (row >= height) {
			row = 0;
		}
	}
}


void	CTerrainView::ApplyStroke()
// Applies a recorded stroke to the grid height data.
{
	GetDocument()->SetModifiedFlag();
	
	float	ExcavateRadius = ExcavateParams.Radius;

	// Get the modulation map, if possible.
	if (ExcavateParams.BitmapModulate) {
		if (strcmp(ExcavateParams.BitmapFilename, MapName) == 0) {
			// Map is already loaded.
		} else {
			// Load a new modulation map.
			if (ModulationMap) delete ModulationMap;
			ModulationMap = Bitmap::ReadImageData8(ExcavateParams.BitmapFilename);
			if (ModulationMap) {
				strcpy(MapName, ExcavateParams.BitmapFilename);
			} else {
				// Complain.
				MessageBox("Can't open modulation bitmap file.", NULL, MB_OK);
				
				MapName[0] = 0;
				ExcavateParams.BitmapModulate = false;
			}
		}
	}
	
	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();
	int	xsize = (ViewX1 - ViewX0) >> ZoomLevel;
	int	zsize = (ViewZ1 - ViewZ0) >> ZoomLevel;
	if (xsize > TEMP_DIM) xsize = TEMP_DIM;
	if (zsize > TEMP_DIM) zsize = TEMP_DIM;

	HeightMapInfo	hm;
	hm.Data = &TempGrid[0][0];
	hm.XOrigin = ViewX0;
	hm.ZOrigin = ViewZ0;
	hm.XSize = xsize;
	hm.ZSize = zsize;
	hm.RowWidth = TEMP_DIM;
	hm.Scale = ZoomLevel;
	
	DrawStrokeInScratchArray(ExcavateRadius, ExcavateParams.EdgeTaper);

	// Apply the modulation map.
	if (ExcavateParams.BitmapModulate) {
		ApplyModulation(xsize, zsize, ModulationMap, ViewX0 >> ZoomLevel, ViewZ0 >> ZoomLevel);
	}

	// Combine our computed influence data with the existing height data.

	if (ExcavateParams.Mode == ExcavateParameters::ADDHEIGHT) {
		ComputeDeltaExcavateAddHeight(xsize, zsize);
	} else if (ExcavateParams.Mode == ExcavateParameters::SMOOTHING) {
		if (CaptureMode == LEFT_BUTTON) {
			// Deresolve in the stroke area.
			FixTempBoundary(xsize, zsize);
			GetDocument()->Deresolve(hm, hm.Scale);
		}

		// Now compute deltas to smooth out or sharpen the region.
		ComputeDeltaExcavateSmoothing(xsize, zsize, ExcavateRadius * (1 << ZoomLevel));
		
	} else if (ExcavateParams.Mode == ExcavateParameters::CLONE) {
		// Pin boundary samples to 0.
		FixTempBoundary(xsize, zsize);

		// Deresolve in the region we're about to flatten, to eliminate
		// roughness at higher resolution than our excavation grid.
		GetDocument()->Deresolve(hm, hm.Scale);

		// Compute the changes to make to the existing height dataset.
		ComputeDeltaExcavateClone(xsize, zsize);
	} else if (ExcavateParams.Mode == ExcavateParameters::DERESOLVE) {
		// Pin boundary samples.
		FixTempBoundary(xsize, zsize);

		// Deresolve where the user painted.
		GetDocument()->Deresolve(hm, hm.Scale);

		// Don't do anything else.
		return;
	}

	// Pin the boundary samples to 0.
	FixTempBoundary(xsize, zsize);

	GetDocument()->AddHeightMap(hm);
}


void CTerrainView::DrawPolygon(CDC* pDC, polygon* p, /* PositioningWindow* pos, */ int ZoomLevel)
// Draws the given polygon into the given device context.  Converts the
// polygon's vertices from terrain coordinates to DC coordinates and then uses
// Windows' Polygon function to do the drawing.
{
	const int	MAX_POINTS = 1000;
	static POINT	Temp[MAX_POINTS];
	int	PointCount = 0;

	// Convert the polygon vertices to windows device coordinates.
	Vertex*	s = p->GetVertexLoop();
	if (s) {
		Vertex*	v = s;
		do {
			Temp[PointCount].x = int((v->x - ViewX0) * DISPLAY_BLOCK_SIZE) >> ZoomLevel;
			Temp[PointCount].y = int((v->z - ViewZ0) * DISPLAY_BLOCK_SIZE) >> ZoomLevel;
			PointCount++;
			if (PointCount >= MAX_POINTS-1) break;
			v = v->Next;
		} while (v != s);

		// Close the polygon boundary by adding the first point again at the end.
		if (p->GetClosed()) {
			Temp[PointCount].x = Temp[0].x;
			Temp[PointCount].y = Temp[0].y;
			PointCount++;
		}
	}

	// Draw the polygon.
	if (PointCount > 1) {
		pDC->Polyline(Temp, PointCount);
	}

	// Draw vertices.
	COLORREF	color;
	if (p->GetSelected()) {
		color = RGB(80, 80, 255);
	} else {
		color = RGB(120, 120, 120);
	}
	s = p->GetVertexLoop();
	if (s) {
		Vertex* v = s;
		do {
			int	x = int((v->x - ViewX0) * DISPLAY_BLOCK_SIZE) >> ZoomLevel;
			int	y = int((v->z - ViewZ0) * DISPLAY_BLOCK_SIZE) >> ZoomLevel;

			DrawBox(pDC, x - 2, y - 2, 5, 5, color);

			v = v->Next;
		} while (v != s);
	}
}


void CTerrainView::ComputeOrigin()
// Recomputes ViewX0, ViewZ0 and ViewX1, ViewZ1 based on ViewCenterX & Z
// and the window size and zoom.
{
	int	zoom = GetFrame()->GetToolPalette()->GetZoomLevel();
	int	BlockPixels = DISPLAY_BLOCK_SIZE;
	CRect	Clip;
	GetClientRect(&Clip);
	int	xsize = (Clip.right << zoom) / BlockPixels;
	int	zsize = (Clip.bottom << zoom) / BlockPixels;
	
	CTerrainDoc*	doc = GetDocument();
	ViewX0 = clamp(0, ViewCenterX - (xsize / 2), doc->GetXSize());
	ViewZ0 = clamp(0, ViewCenterZ - (zsize / 2), doc->GetZSize());

	ViewX1 = ViewX0 + (xsize) + 1;
	ViewZ1 = ViewZ0 + (zsize) + 1;
}


void CTerrainView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	int	ZoomLevel = GetFrame()->GetToolPalette()->GetZoomLevel();

	// Figure out what tool is active.
	ToolPalette::ToolType	t;
	ToolPalette*	pal = GetFrame()->GetToolPalette();
	t = pal->GetCurrentTool();

	switch (t) {
	default:
		return;
		break;

	case ToolPalette::HAND: {
		// Zoom out from the mouse point.
		float	x, z;
		ConvertWindowToWorld(point, &x, &z);
		ViewCenterX = x;
		ViewCenterZ = z;
		pal->SetZoomLevel(pal->GetZoomLevel() + 1);
		break;
	}
	}
	
	// Make sure views get repainted.
	GetDocument()->UpdateAllViews(NULL);
}


void CTerrainView::OnCancelMode() 
{
	CView::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}


void CTerrainView::XORRubberBandBox(const CPoint & anchor, const CPoint & corner)
// XOR's the defined box into the window.
{
	CDC*	dc = GetDC();
	if (dc == NULL) return;

	int	x = anchor.x;
	int	xsize = corner.x - x;
	int	y = anchor.y;
	int	ysize = corner.y - y;

	if (xsize < 0) {
		xsize = -xsize;
		x -= xsize;
	}
	if (ysize < 0) {
		ysize = -ysize;
		y -= ysize;
	}

	int	OldROP = dc->GetROP2();
	dc->SetROP2(R2_XORPEN);
	DrawBox(dc, x, y, xsize, ysize, RGB(255, 255, 255));
	dc->SetROP2(OldROP);
}


void CTerrainView::SelectObjectsInBox(const CPoint & anchor, const CPoint & corner, bool Toggle)
// Map the window coordinates to world coordinates, then toggle the
// selection state of each object within the world-coordinate box
// defined by the two points.
{
	float	x0, z0;
	ConvertWindowToWorld(anchor, &x0, &z0);

	float	x1, z1;
	ConvertWindowToWorld(corner, &x1, &z1);

	float	temp;
	if (x1 < x0) {
		temp = x0;
		x0 = x1;
		x1 = temp;
	}
	if (z1 < z0) {
		temp = z0;
		z0 = z1;
		z1 = temp;
	}

	// Go through object list, selecting those that are within the box.

//	ObjectElem*	ObjList = GetDocument()->GetObjectList();
//	ObjectElem*	e = ObjList;
//	if (e) {
//		do {

	CTerrainDoc*	doc = GetDocument();
	ObjectIterator	it;
	doc->InitObjectListIterator(&it, x0, z0, x1, z1);
	Object*	o;
	while (o = doc->GetObjectAndAdvanceIterator(&it)) {
		float	x = o->GetX();
		float	z = o->GetZ();
		if (x >= x0 && x <= x1 && z >= z0 && z <= z1) {
			if (Toggle) {
				o->SetSelected(!o->GetSelected());
			} else {
				o->SetSelected(true);
			}
		}
	}
//			e = e->Next;
//		} while (e != ObjList);
//	}
}


void CTerrainView::MoveSelectedObjects(float DeltaX, float DeltaZ)
// Moves all selected objects by the given deltas.
{
	// Go through object list, applying the delta to selected objects.

	CTerrainDoc*	doc = GetDocument();
	ObjectIterator	it;
	doc->InitObjectListIterator(&it /* , x0, z0, x1, z1 */);
	Object*	o;
	while (o = doc->GetObjectAndAdvanceIterator(&it)) {
		if (o->GetSelected()) {
//			o->SetLocation(doc, o->GetX() + DeltaX, o->GetY(), o->GetZ() + DeltaZ);
			o->ForceLocation(o->GetX() + DeltaX, o->GetY(), o->GetZ() + DeltaZ);
		}
	}
	doc->FixupObjectBuckets();
}


void CTerrainView::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}


