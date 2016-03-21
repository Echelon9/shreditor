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
// terrainDoc.h : interface of the CTerrainDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERRAINDOC_H__DED47ACC_CF2D_11D1_83C5_98E1A892656D__INCLUDED_)
#define AFX_TERRAINDOC_H__DED47ACC_CF2D_11D1_83C5_98E1A892656D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "quadtree.hpp"


class GridElem;
class Grid;
class ObjectElem;
class Object;
class polygon;
class PolygonElem;
class Geometry;
class Solid;


struct ObjectIterator {
	int	MinX, MaxX, MinZ, MaxZ;
	int	BucketX, BucketZ;
	ObjectElem*	Elem;
};


class CTerrainDoc : public CDocument
{
protected:
	CTerrainDoc();
	DECLARE_DYNCREATE(CTerrainDoc)

	void	EmptyGridList();
	void	EmptyObjectList();
	void	EmptyPolygonList();
	void	EmptyGeometryList();
	void	EmptySolidList();

	ObjectElem**	GetObjectBucket(float x, float z);
	
	ObjectElem*	UnlinkObject(ObjectElem** list, Object* o);
	void	LinkObject(ObjectElem** list, ObjectElem* elem);
	
// Attributes
public:
	int	GetXSize() { return Dimension; }
	int	GetZSize() { return Dimension; }
	int	GetInFileVersion() { return InFileVersion; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTerrainDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ReSortGrid(Grid* g);
	void GenerateTypemaps();
	void Empty();
	void FixupObjectBuckets();
	// Geometry.
	void DeleteSelectedObjects();
	int	GetGeometryCount() { return GeometryCount; }
	Geometry* GetGeometry(int index);
	void DeleteGeometry(int index);
	void AddGeometry(Geometry* g);
	Geometry* GetGeometryList();

	// Solids.
	int	GetSolidCount() { return SolidCount; }
	Solid*	GetSolid(int index);
	void	DeleteSolid(int index);
	void	AddSolid(Solid* s);
	Solid*	GetSolidList();

	// Grids.
	void	AddGrid(Grid* NewGrid);
	void	DeleteGrid(Grid* g);
	GridElem*	GetGridList();
	void	OffsetGrids(float XOffset, float ZOffset);

	// Height.
	void	AddHeightMap(const HeightMapInfo& h);
	void	Deresolve(const HeightMapInfo& h, int ThresholdScale);

	int	GetQuadTreeNodeCount();
	void	CleanupQuadTree(float ThresholdErrorRatio);
	
	// Objects.
	void	ClearObjectSelection();
	void	AddObject(Object* NewObject);
	void	DeleteObject(Object* o);
	void	MoveObject(Object* o, float NewX, float NewZ);
	void	OffsetObjects(float XOffset, float ZOffset);
	
	void	InitObjectListIterator(ObjectIterator* it, int x0 = -1, int z0 = -1, int x1 = -1, int z1 = -1);
	Object*	GetObjectAndAdvanceIterator(ObjectIterator* it);

	// Surface polygons.
//	void	SetSelectedPolygon(polygon* p);
//	polygon*	GetSelectedPolygon();
	void	ClearPolygonSelection();
	void	AddPolygon(polygon* p);
	void	DeletePolygon(polygon* p);
	PolygonElem*	GetPolygonList();
	void	OffsetPolygons(float XOffset, float ZOffset);

	char*	GetName();
	float	GetPitch();

	float	GetHeight(float x, float z);

	virtual ~CTerrainDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTerrainDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int InFileVersion;
	char	Name[30];
	int	Dimension;
	float	Pitch;
	GridElem*	GridList;
	ObjectElem*	ObjectList;
	PolygonElem*	PolygonList;
	Geometry*	GeometryList;
	int	GeometryCount;
	Solid*	SolidList;
	int	SolidCount;

	quadcornerdata	RootCornerData;
	int	NodeCount;
	
	polygon*	SelectedPolygon;

#define BUCKET_DIM 512
	ObjectElem*	ObjectBucket[BUCKET_DIM][BUCKET_DIM];
	ObjectElem*	MiscBucket;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAINDOC_H__DED47ACC_CF2D_11D1_83C5_98E1A892656D__INCLUDED_)
