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
// terrainDoc.cpp : implementation of the CTerrainDoc class
//

#include "stdafx.h"
#include "terrain.h"

#include <math.h>
#include "terrainDoc.h"
#include "grid.h"
#include "gridelem.h"
#include "object.h"
#include "objectelem.h"
#include "Polygon.h"
#include "geometry.h"
#include "solid.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerrainDoc

IMPLEMENT_DYNCREATE(CTerrainDoc, CDocument)

BEGIN_MESSAGE_MAP(CTerrainDoc, CDocument)
	//{{AFX_MSG_MAP(CTerrainDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTerrainDoc construction/destruction


CTerrainDoc::CTerrainDoc()
{
	// Initialize members.
	strcpy(Name, "Untitled");

	Dimension = 65536;
	Pitch = 1;
	GridList = NULL;
	ObjectList = NULL;
	PolygonList = NULL;
	
	GeometryList = NULL;
	GeometryCount = 0;
	EmptyGeometryList();

	SolidList = NULL;
	SolidCount = 0;
	EmptySolidList();

	RootCornerData.Parent = NULL;
	RootCornerData.Square = NULL;
	RootCornerData.ChildIndex = 0;
	RootCornerData.Level = 15;	// 64K dimensions (2 << 15)
	RootCornerData.xorg = 0;
	RootCornerData.zorg = 0;
	RootCornerData.Y[0] = 0;
	RootCornerData.Y[1] = 0;
	RootCornerData.Y[2] = 0;
	RootCornerData.Y[3] = 0;

	// Create default root node.
	RootCornerData.Square = quadsquare::NewQuadSquare(&RootCornerData);

	NodeCount = 1;
	
//	SelectedObject = NULL;
	SelectedPolygon = NULL;

	InFileVersion = 0;

	// Init object buckets.
	int	i, j;
	for (j = 0; j < BUCKET_DIM; j++) {
		for (i = 0; i < BUCKET_DIM; i++) {
			ObjectBucket[j][i] = NULL;
		}
	}
	MiscBucket = NULL;
}


CTerrainDoc::~CTerrainDoc()
{
	Empty();

	delete RootCornerData.Square;
}


void CTerrainDoc::Empty()
// Clear everything.
{
	delete RootCornerData.Square;
	// Create default root node.
//	RootCornerData.Square = new quadsquare(&RootCornerData);
	RootCornerData.Square = quadsquare::NewQuadSquare(&RootCornerData);

	NodeCount = 1;
	
	EmptyGridList();
	EmptyGeometryList();
	EmptyObjectList();
	EmptyPolygonList();
	EmptySolidList();
}


void	CTerrainDoc::EmptyGridList()
// Delete the grid list.
{
	GridElem*	e = GridList;
	if (e) {
		do {
			GridElem*	n = e->Next;
			delete e;
			e = n;
		} while (e != GridList);
	}
	GridList = NULL;
}


void	CTerrainDoc::EmptyObjectList()
// Delete the object list.
{
	int	i, j;
	for (j = 0; j < BUCKET_DIM; j++) {
		for (i = 0; i < BUCKET_DIM; i++) {
			ObjectElem*	List = ObjectBucket[j][i];
			ObjectElem*	e = List;
			if (e) {
				do {
					ObjectElem*	n = e->Next;
					delete e;
					e = n;
				} while (e != List);
			}
			ObjectBucket[j][i] = NULL;
		}
	}

	{
		ObjectElem*	List = MiscBucket;
		ObjectElem*	e = List;
		if (e) {
			do {
				ObjectElem*	n = e->Next;
				delete e;
				e = n;
			} while (e != List);
		}
		MiscBucket = NULL;
	}
}


void	CTerrainDoc::EmptyPolygonList()
// Delete the polygon list.
{
	PolygonElem*	e = PolygonList;
	if (e) {
		do {
			PolygonElem*	n = e->Next;
			delete e;
			e = n;
		} while (e != PolygonList);
	}
	PolygonList = NULL;
}


void	CTerrainDoc::EmptyGeometryList()
// Deletes all geometry.
{
	Geometry*	g = GeometryList;
	if (g) {
		do {
			Geometry*	n = g->Next;
			delete g;
			g = n;
		} while (g != GeometryList);
	}
	GeometryList = NULL;
	GeometryCount = 0;

	// Add the null geometry element.
	g = new Geometry;
	g->SetName("null");
	g->SetComment("null");
	g->SetFilename("null");
	AddGeometry(g);
}


void	CTerrainDoc::EmptySolidList()
// Deletes all solids.
{
	Solid*	s = SolidList;
	if (s) {
		do {
			Solid*	n = s->Next;
			delete s;
			s = n;
		} while (s != SolidList);
	}
	SolidList = NULL;
	SolidCount = 0;

	// Add the null solid element.
	s = new Solid;
	s->SetName("null");
	s->SetComment("null");
	s->SetType(Solid::BOX);
	AddSolid(s);
}


BOOL CTerrainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CTerrainDoc serialization

void CTerrainDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here

		// Header marker.
		ar << -1;

		// File version.  Increments when format changes...
		InFileVersion = 12;
		ar << InFileVersion;

		// Name
		// BlockLevels
		// Pitch

		// Write the height quadtree.
		RootCornerData.Square->Serialize(RootCornerData, ar);	// write 3 heights, four child flags, and recurse on existing children.
		
		//
		// Grids
		//

		// Count the grids.
		int	GridCount = 0;
		GridElem*	e = GetGridList();
		if (e) {
			do {
				GridCount++;
				e = e->Next;
			} while (e != GetGridList());
		}

		// Write the grid count.
		ar << GridCount;

		// Save the grids.
		e = GetGridList();
		if (e) {
			do {
				e->Grid->Serialize(ar, InFileVersion, this);
				e = e->Next;
			} while (e != GetGridList());
		}

		//
		// Geometry
		//

		// Write the geometry count.
		ar << GeometryCount - 1;

		// Write geometry.
		Geometry*	g = GetGeometryList();
		// Don't bother saving the first geometry; it's always null.
		if (g) g = g->Next;
		int	i;
		for (i = 1; i < GeometryCount; i++) {
			g->Serialize(ar, this);
			g = g->Next;
		}

		//
		// Solids
		//

		// Write the solid count.
		ar << SolidCount - 1;

		// Write solids.
		Solid*	s = GetSolidList();
		// Don't save first solid; it's always null.
		if (s) s = s->Next;
		for (i = 1; i < SolidCount; i++) {
			s->Serialize(ar, this);
			s = s->Next;
		}

		//
		// Objects
		//

		// Count the objects.
		int	ObjectCount = 0;
		ObjectIterator	it;
		InitObjectListIterator(&it);
		Object*	o;
		while (o = GetObjectAndAdvanceIterator(&it)) {
			ObjectCount++;
		}

		// Write the object count.
		ar << ObjectCount;

		// Save the objects.
		InitObjectListIterator(&it);
		while (o = GetObjectAndAdvanceIterator(&it)) {
				o->Serialize(ar, this);
		}

		//
		// Surface type polygons
		//

		// Count the polygons.
		int	PolyCount = 0;
		PolygonElem*	p = GetPolygonList();
		if (p) {
			do {
				PolyCount++;
				p = p->Next;
			} while (p != GetPolygonList());
		}
		
		// Write the polygon count.
		ar << PolyCount;
		
		// Save the polygons.
		p = GetPolygonList();
		if (p) {
			do {
				p->poly->Serialize(ar, this);
				p = p->Next;
			} while (p != GetPolygonList());
		}
	}
	else
	{
		// Load.
		
		InFileVersion = 0;

		// Get the grid count.
		int	GridCount;
		ar >> GridCount;

		if (GridCount < 0) {
			// Whoops, that wasn't really the grid count,
			// it was actually the header marker.
			
			// Read header.
			ar >> InFileVersion;
		}

		if (InFileVersion >= 9) {
			// Load quadtree data.
			RootCornerData.Square->Serialize(RootCornerData, ar);
		}
		
		// Load all the grids.
		if (GridCount < 0) {
			ar >> GridCount;
		}
		for (int i = 0; i < GridCount; i++) {
			Grid*	g = new Grid;
			g->Serialize(ar, InFileVersion, this);
			AddGrid(g);
		}

		if (InFileVersion >= 3) {
			// Get the geometry count.
			int	count;
			ar >> count;

			// Load geometry.
			for (i = 0; i < count; i++) {
				Geometry* g = new Geometry;
				g->Serialize(ar, this);
				AddGeometry(g);
			}
		}

		if (InFileVersion >= 4) {
			// Get the solid count.
			int	count;
			ar >> count;

			// Load solids.
			for (i = 0; i < count; i++) {
				Solid*	s = new Solid;
				s->Serialize(ar, this);
				AddSolid(s);
			}
		}

		// Get the object count.
		int	ObjectCount;
		ar >> ObjectCount;

		// Load all the objects.
		for (i = 0; i < ObjectCount; i++) {
			Object*	o = new Object;
			o->Serialize(ar, this);
			AddObject(o);
		}

		// Only try to load polygons if the file has more data.
		ar.Flush();
		const CFile*	file = ar.GetFile();
		if (file->GetPosition() < file->GetLength()) {
			// Get the polygon count.
			int	PolyCount;
			ar >> PolyCount;
			
			// Load all the polygons.
			for (i = 0; i < PolyCount; i++) {
				polygon*	p = new polygon();
				p->Serialize(ar, this);
				AddPolygon(p);
			}
		}

		NodeCount = RootCornerData.Square->CountNodes();
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTerrainDoc diagnostics

#ifdef _DEBUG
void CTerrainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTerrainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTerrainDoc commands

GridElem* CTerrainDoc::GetGridList()
{
	return GridList;
}


//ObjectElem* CTerrainDoc::GetObjectList()
//{
//	return ObjectList;
//}


static int	clamp(int low, int i, int hi)
{
	if (i < low) i = low;
	if (i > hi) i = hi;
	return i;
}


void	CTerrainDoc::InitObjectListIterator(ObjectIterator* it, int x0, int z0, int x1, int z1)
// Returns an iterator that can be used to iterate through all objects
// within the specified x,z region.  Iterate using
// CTerrainDoc::GetObjectAndAdvanceIterator().  Will return all objects
// within the region, plus an indeterminate number outside the region as
// well.  It's a performance optimization.
{
	if (x0 > -1 && z0 > -1 && x1 > -1 && z1 > -1) {
		it->MinX = clamp(0, floor(float(x0) / GetXSize() * BUCKET_DIM), BUCKET_DIM-1);
		it->MinZ = clamp(0, floor(float(z0) / GetZSize() * BUCKET_DIM), BUCKET_DIM-1);
		it->MaxX = clamp(0, floor(float(x1) / GetXSize() * BUCKET_DIM), BUCKET_DIM-1);
		it->MaxZ = clamp(0, floor(float(z1) / GetZSize() * BUCKET_DIM), BUCKET_DIM-1);
	} else {
		it->MinX = it->MinZ = 0;
		it->MaxX = it->MaxZ = BUCKET_DIM - 1;
	}

	// Initialize the element pointer to the first non-empty object bucket.
	it->BucketZ = it->MinZ;
	while (it->BucketZ <= it->MaxZ) {
		it->BucketX = it->MinX;
		while (it->BucketX <= it->MaxX) {
			it->Elem = ObjectBucket[it->BucketZ][it->BucketX];
			if (it->Elem) {
				// OK, the iterator is at the next bucket; ready for the next call to this function.
				return;
			}
			it->BucketX++;
		}
		it->BucketZ++;
	}

	// No more buckets.
	it->Elem = NULL;
}


Object*	CTerrainDoc::GetObjectAndAdvanceIterator(ObjectIterator* it)
// Returns the object associated with the current iterator and advances
// to the next object.  If the iterator is at the end of the objects to
// be iterated, returns NULL.
{
	Object*	ret = NULL;

	if (it->Elem) {
		ret = it->Elem->Object;

		// Advance to the next object in the bucket.
		it->Elem = it->Elem->Next;
		
		if (it->Elem == ObjectBucket[it->BucketZ][it->BucketX]) {
			// Done with this bucket; move to the next one, if any are left.
			it->Elem = NULL;
			it->BucketX++;

			while (it->BucketZ <= it->MaxZ) {
				while (it->BucketX <= it->MaxX) {
					it->Elem = ObjectBucket[it->BucketZ][it->BucketX];
					if (it->Elem) {
						// OK, the iterator is at the next bucket; ready for the next call to this function.
						return ret;
					}
					it->BucketX++;
				}
				it->BucketZ++;
				it->BucketX = it->MinX;
			}
		}
	}

	return ret;
}


PolygonElem*	CTerrainDoc::GetPolygonList()
{
	return PolygonList;
}


char* CTerrainDoc::GetName()
{
	return Name;
}


float	CTerrainDoc::GetPitch()
{
	return Pitch;
}


float	CTerrainDoc::GetHeight(float x, float z)
// Returns the y value at the given x,z coordinates for this terrain.
{
	return RootCornerData.Square->GetHeight(RootCornerData, x, z);
}


void	CTerrainDoc::AddHeightMap(const HeightMapInfo& hm)
// Sums the given height info with our current stored height info.
{
	RootCornerData.Square->AddHeightMap(RootCornerData, hm);
}


int	CTerrainDoc::GetQuadTreeNodeCount()
// Returns the number of nodes in our height quadtree.
{
	return NodeCount;
}


void	CTerrainDoc::CleanupQuadTree(float ThresholdErrorRatio)
// Delete unnecessary nodes from the height quadtree.
{
	RootCornerData.Square->StaticCullData(RootCornerData, ThresholdErrorRatio);
	for (int i = RootCornerData.Level; i >= 0; i--) {
		RootCornerData.Square->DeCrack(RootCornerData, i);	// Crude removal of cracks.
	}
	NodeCount = RootCornerData.Square->CountNodes();
}


void	CTerrainDoc::Deresolve(const HeightMapInfo& hm, int ThresholdScale)
// Deresolves detail in the height data, according to the given
// influence map (values should range from 0 to 1).  Only touches data
// at or below the specified scale level.
{
	int	i;
	for (i = ThresholdScale; i >= 0; i--) {
		RootCornerData.Square->Deresolve(RootCornerData, hm, i);
	}
}


void	CTerrainDoc::AddGrid(Grid* NewGrid)
// Adds a grid to our list.  We want to keep them sorted in large-scale to
// small-scale order, so we'll do that by ensuring grids are inserted in the
// correct spot.
{
	// GridList points into a doubly-linked circular list.
	GridElem*	e = new GridElem;
	e->Grid = NewGrid;

	if (GridList) {
		// Insert before the first existing grid which is less-than-equal-to the new grid.
		GridElem*	Prev = GridList->Previous;
		bool	NewElemAtHead = true;
		do {
			if (NewGrid->Compare(*(Prev->Next->Grid)) <= 0) break;
			NewElemAtHead = false;
			Prev = Prev->Next;
		} while (Prev->Next != GridList);
		
		e->Previous = Prev;
		e->Next = Prev->Next;
		Prev->Next = e;
		e->Next->Previous = e;

		if (NewElemAtHead) GridList = e;
	} else {
		e->Previous = e;
		e->Next = e;
		GridList = e;
	}
}


void	CTerrainDoc::DeleteGrid(Grid* g)
// Deletes the specified grid from our list.
{
	// If we're about to delete the selected grid, then make sure to clear
	// the selection.
//	if (o == SelectedObject) SelectedObject = NULL;

	// GridList points into a doubly-linked circular list.

	GridElem*	DeleteElem = NULL;

	if (GridList) {
		// Look for the link we want to remove.
		GridElem*	e = GridList;
		do {
			if (e->Grid == g) {
				// We've found the link we want to remove.
				DeleteElem = e;
				break;
			}
			e = e->Next;
		} while (e != GridList);

		// Deleting the last element is a special case.
		if (DeleteElem->Next == DeleteElem) {
			GridList = NULL;
			delete DeleteElem;

		} else {
			// Normal case.

			// Make sure we keep a valid link into the list.
			if (DeleteElem == GridList) {
				GridList = GridList->Next;
			}

			// Unlink DeleteElem.
			DeleteElem->Previous->Next = DeleteElem->Next;
			DeleteElem->Next->Previous = DeleteElem->Previous;

			delete DeleteElem;
		}
	}
}


void	CTerrainDoc::OffsetGrids(float XOffset, float ZOffset)
// Moves all grids by the specified offset.  Due to the limitations
// on grid placement, due to alignment with power-of-two coordinates,
// some grids may not actually move by the precise amount specified.
{
	GridElem*	e = GetGridList();
	if (e) {
		do {
			Grid*	g = e->Grid;
			g->SetXOrigin(int((int(g->GetXOrigin()) << g->GetScale()) + XOffset) >> g->GetScale());
			g->SetZOrigin(int((int(g->GetZOrigin()) << g->GetScale()) + ZOffset) >> g->GetScale());
			e = e->Next;
		} while (e != GetGridList());
	}
}


//
// object stuff.
//


ObjectElem**	CTerrainDoc::GetObjectBucket(float x, float z)
// Returns a pointer to the bucket of objects which should contain an object
// with the given x,z coordinates.
{
	int	bx = floor(x / GetXSize() * BUCKET_DIM);
	int	bz = floor(z / GetZSize() * BUCKET_DIM);

	if (bx < 0 || bx >= BUCKET_DIM || bz < 0 || bz >= BUCKET_DIM) {
		return &MiscBucket;
	}

	return &ObjectBucket[bz][bx];
}


ObjectElem*	CTerrainDoc::UnlinkObject(ObjectElem** list, Object* o)
// Look for the object in the given list, and unlink it.  Returns the
// object's link.  If the object can't be found in the given list,
// returns NULL.
{
	ObjectElem*	e = NULL;
	if (*list) {
		e = *list;
		do {
			if (e->Object == o) {
				// We've found the object.  Unlink it from the list.

				// Removing the last element is a special case.
				if (e->Next == e) {
					*list = NULL;
					return e;
				} else {
					// Normal case.
					
					// Make sure we keep a valid link into the list.
					if (e == *list) {
						*list = (*list)->Next;
					}
					
					// Unlink the element.
					e->Previous->Next = e->Next;
					e->Next->Previous = e->Previous;
					
					return e;
				}
			}

			e = e->Next;
		} while (e != *list);
	}

	// Didn't find the object.
	return NULL;
}


void	CTerrainDoc::LinkObject(ObjectElem** list, ObjectElem* elem)
// Links the given object element into the given list.
{
	if (*list) {
		elem->Previous = (*list)->Previous;
		elem->Previous->Next = elem;
		elem->Next = *list;
		elem->Next->Previous = elem;
	} else {
		elem->Previous = elem;
		elem->Next = elem;
		*list = elem;
	}
}

	
void	CTerrainDoc::AddObject(Object* NewObject)
// Adds an object to our list.
{
	// Figure out what bucket this object belongs in, based on its location.
	ObjectElem** bucket = GetObjectBucket(NewObject->GetX(), NewObject->GetZ());

	if (bucket == NULL) {
		// Coordinates are out of bounds or something.
		delete NewObject;
		
//		issue some type of warning????;
		return;
	}
	
	// Bucket pointers point into a doubly-linked circular list.
	ObjectElem*	e = new ObjectElem;
	e->Object = NewObject;
	LinkObject(bucket, e);
}


void CTerrainDoc::FixupObjectBuckets()
//  Goes through all objects and makes sure they're linked to the correct bucket.
{
	ObjectIterator	it;
	InitObjectListIterator(&it);
	Object*	o;
	while (1) {
		ObjectElem**	OldBucket = &ObjectBucket[it.BucketZ][it.BucketX];
		o = GetObjectAndAdvanceIterator(&it);
		if (o == NULL) break;
		ObjectElem**	NewBucket = GetObjectBucket(o->GetX(), o->GetZ());

		if (OldBucket != NewBucket) {
			ObjectElem*	elem = UnlinkObject(OldBucket, o);
			LinkObject(NewBucket, elem);
		}
	}
}


void	CTerrainDoc::MoveObject(Object* o, float NewX, float NewZ)
// Moves the given object to the new x,z coordinates.
{
	// Find the bucket and link where the object originates.
	ObjectElem**	OldBucket = GetObjectBucket(o->GetX(), o->GetZ());
	ObjectElem**	NewBucket = GetObjectBucket(NewX, NewZ);

	if (NewBucket != OldBucket) {
		// Object must change buckets.
		ObjectElem*	elem;
		if (OldBucket) {
			elem = UnlinkObject(OldBucket, o);
		} else {
			elem = new ObjectElem;
			elem->Object = o;
		}

		// Only link the object if we were able to unlink it from somewhere else in the object documents.
		if (elem) {
			if (NewBucket) {
				LinkObject(NewBucket, elem);
			} else {
				// Out of bounds or something.
				delete elem;
				return;
			}
		}
	}
	
	// Set the new coordinates.
	o->Location.x = NewX;
	o->Location.z = NewZ;
}


void	CTerrainDoc::DeleteObject(Object* o)
// Deletes the specified object from our list.
{
	ObjectElem**	bucket = GetObjectBucket(o->GetX(), o->GetZ());
	if (bucket) {
		ObjectElem*	elem = UnlinkObject(bucket, o);
		
		if (elem) {
			delete elem;
		}
	}
}


void	CTerrainDoc::ClearObjectSelection()
// De-selects all objects.
{
	ObjectIterator	it;
	InitObjectListIterator(&it);
	Object*	o;
	while (o = GetObjectAndAdvanceIterator(&it)) {
		o->SetSelected(false);
	}
}


void	CTerrainDoc::OffsetObjects(float XOffset, float ZOffset)
// Move all objects by the specified offset.
{
	ObjectIterator	it;
	InitObjectListIterator(&it);
	Object*	o;
	while (o = GetObjectAndAdvanceIterator(&it)) {
		o->ForceLocation(o->GetX() + XOffset, o->GetY(), o->GetZ() + ZOffset);
	}

	FixupObjectBuckets();
}


//
// Polygon stuff
//


void	CTerrainDoc::AddPolygon(polygon* p)
// Adds a surface-type polygon to our list.
{
	// PolygonList points into a doubly-linked circular list.
	PolygonElem*	e = new PolygonElem;
	e->poly = p;
	e->Next = PolygonList;
	if (PolygonList) {
		e->Previous = PolygonList->Previous;
		e->Previous->Next = e;
		PolygonList->Previous = e;
	} else {
		e->Previous = e;
		e->Next = e;
	}
	PolygonList = e;
}


void	CTerrainDoc::DeletePolygon(polygon* p)
// Deletes the specified polygon from our list.
{
	// If we're about to delete the selected polygon, then make sure to clear
	// the selection.
	if (p == SelectedPolygon) SelectedPolygon = NULL;

	// PolygonList points into a doubly-linked circular list.
	PolygonElem*	DeleteElem = NULL;

	if (PolygonList) {
		// Look for the link we want to remove.
		PolygonElem*	e = PolygonList;
		do {
			if (e->poly == p) {
				// We've found the link we want to remove.
				DeleteElem = e;
				break;
			}
			e = e->Next;
		} while (e != PolygonList);

		// Deleting the last element is a special case.
		if (DeleteElem->Next == DeleteElem) {
			PolygonList = NULL;
			delete DeleteElem;

		} else {
			// Normal case.

			// Make sure we keep a valid link into the list.
			if (DeleteElem == PolygonList) {
				PolygonList = PolygonList->Next;
			}

			// Unlink DeleteElem.
			DeleteElem->Previous->Next = DeleteElem->Next;
			DeleteElem->Next->Previous = DeleteElem->Previous;

			delete DeleteElem;
		}
	}
}


void	CTerrainDoc::ClearPolygonSelection()
// De-selects all polygons.
{
	PolygonElem*	e = PolygonList;
	if (e) {
		do {
			e->poly->SetSelected(false);
			e = e->Next;
		} while (e != PolygonList);
	}
}


void	CTerrainDoc::OffsetPolygons(float XOffset, float ZOffset)
// Move all polygon verts by the specified offset.
{
	PolygonElem*	e = PolygonList;
	if (e) {
		do {
			Vertex*	s = e->poly->GetVertexLoop();
			if (s) {
				Vertex*	v = s;
				do {
					v->x += XOffset;
					v->z += ZOffset;

					v = v->Next;
				} while (s != v);
			}

			e = e->Next;
		} while (e != PolygonList);
	}
}


//
// Geometry stuff
//


Geometry* CTerrainDoc::GetGeometryList()
// Returns the first element of the list of geometry.  NULL if
// the list is empty.
{
	return GeometryList;
}


void CTerrainDoc::AddGeometry(Geometry* g)
// Add to the end of the geometry list.
{
	// GeometryList points into a doubly-linked circular list.

	if (GeometryList) {
		g->Next = GeometryList;
		g->Previous = GeometryList->Previous;
		g->Previous->Next = g;
		GeometryList->Previous = g;
		// g->Index = g->Previous->GetIndex() + 1;
	} else {
		g->Previous = g;
		g->Next = g;
		GeometryList = g;
		// g->Index = 0;
	}

	GeometryCount++;
}


void	CTerrainDoc::DeleteGeometry(int index)
// Deletes the indexed geometry item.  After deleting, goes through the
// object list and notifies each object of the deletion, so they can renumber
// geometry references as necessary.
{
	// Refuse to delete entry 0, which is null geometry.
	if (index == 0) return;

	Geometry*	g = GetGeometry(index);
	if (g) {
		// Splice g out of the list.
		g->Previous->Next = g->Next;
		g->Next->Previous = g->Previous;
		GeometryCount--;
		delete g;
		
		// Go through remainder of the geometry and
		// decrement its index, if needed.
		
//		// Notify objects in object list of the change.
//		ObjectElem*	o = ObjectList;
//		if (o) {
//			do {
		ObjectIterator	it;
		InitObjectListIterator(&it);
		Object*	o;
		while (o = GetObjectAndAdvanceIterator(&it)) {
			o->GeometryListDeletion(index);
		}
//				o = o->Next;
//			} while (o != ObjectList);
//		}

		// Notify the GeometryListView.

	}
}


Geometry*	CTerrainDoc::GetGeometry(int index)
// Returns the indexed geometry element.  Returns NULL if the index
// doesn't exist.
{
	if (index >= GeometryCount) return NULL;

	Geometry*	g = GeometryList;
	while (index--) {
		g = g->Next;
	}

	return g;
}


//
// Solid stuff
//

Solid* CTerrainDoc::GetSolidList()
// Returns the first element of the list of solids.  NULL if
// the list is empty.
{
	return SolidList;
}


void CTerrainDoc::AddSolid(Solid* s)
// Add to the end of the solid list.
{
	// SolidList points into a doubly-linked circular list.

	if (SolidList) {
		s->Next = SolidList;
		s->Previous = SolidList->Previous;
		s->Previous->Next = s;
		SolidList->Previous = s;
	} else {
		s->Previous = s;
		s->Next = s;
		SolidList = s;
	}

	SolidCount++;
}


void	CTerrainDoc::DeleteSolid(int index)
// Deletes the indexed solid item.  After deleting, goes through the
// object list and notifies each object of the deletion, so they can renumber
// solid references as necessary.
{
	// Refuse to delete entry 0, which is null solid.
	if (index == 0) return;

	Solid*	s = GetSolid(index);
	if (s) {
		// Splice s out of the list.
		s->Previous->Next = s->Next;
		s->Next->Previous = s->Previous;
		SolidCount--;
		delete s;
		
		// Notify objects in object list of the change, so they
		// can fix their references.
		ObjectIterator	it;
		InitObjectListIterator(&it);
		Object*	o;
		while (o = GetObjectAndAdvanceIterator(&it)) {
			o->SolidListDeletion(index);
		}
//		ObjectElem*	o = ObjectList;
//		if (o) {
//			do {
//				o->Object->SolidListDeletion(index);
//				o = o->Next;
//			} while (o != ObjectList);
//		}

		// Notify the SolidListView???
	}
}


Solid*	CTerrainDoc::GetSolid(int index)
// Returns the indexed solid element.  Returns NULL if the index
// doesn't exist.
{
	if (index >= SolidCount) return NULL;

	Solid*	s = SolidList;
	while (index--) {
		s = s->Next;
	}

	return s;
}


void CTerrainDoc::DeleteSelectedObjects()
// Delete all selected objects.
{
	ObjectIterator	it;
	InitObjectListIterator(&it);
	Object*	o;
	while (o = GetObjectAndAdvanceIterator(&it)) {
		if (o->GetSelected()) {
			DeleteObject(o);
		}
	}
	
#ifdef NOT
	if (ObjectList) {
		ObjectElem*	Last = ObjectList->Previous;

		// Look for selected objects.
		ObjectElem*	e = ObjectList;
		for (;;) {
			Object*	o = e->Object;
			ObjectElem* next = e->Next;

			if (o->GetSelected()) {
				// Delete this object.

				// Deleting the last element is a special case.
				if (e->Next == e) {
					ObjectList = NULL;
					delete e;
					return;
				} else {
					// Normal case.
					
					// Unlink e.
					e->Previous->Next = e->Next;
					e->Next->Previous = e->Previous;
					
					delete e;
				}
			}

			if (e == Last) {
				ObjectList = next;
				break;
			} else {
				e = next;
			}
		}
	}
#endif // NOT
	
}


void CTerrainDoc::GenerateTypemaps()
// Generate surface type arrays, which are stored in the grid
// typemaps.
{
	GridElem*	e = GetGridList();
	if (e == NULL) return;
	do {
		Grid*	g = e->Grid;

		// Don't generate typemaps for small grids, as those are likely to have been hand-painted.
		if (g->GetScale() >= 6) {
			unsigned char*	t = g->GetTypeData();
			float	step = 1 << g->GetScale();
			
			float	worldz = g->GetZOrigin() * step + step * 0.5;
			for (int z = 0; z < g->GetZSize(); z++) {
				float	worldx = g->GetXOrigin() * step + step * 0.5;
				for (int x = 0; x < g->GetXSize(); x++) {
//					get slope;
//					get height;
//					sample neighborhood?;
					float	y = GetHeight(worldx, worldz);
					float	dx = GetHeight(worldx + step/2, worldz) - GetHeight(worldx - step/2, worldz);
					float	dz = GetHeight(worldx, worldz + step/2) - GetHeight(worldx, worldz - step/2);
					float	normy = step / sqrt(step * step + dx * dx + dz * dz);
					float	alt = y / 2000.0;
					float	exposure = (y - 0.25 *
						(GetHeight(worldx - 100, worldz) +
						GetHeight(worldx + 100, worldz) + 
						GetHeight(worldx, worldz - 100) +
						GetHeight(worldx, worldz + 100))) / 20.0;
					
					unsigned char type = 0;
					
					float	RockProportion = 0;
					float	ForestProportion = 0;
					float	PowderProportion = 0;
					
					float	RockLikelihood = (1 - normy) * 2.2 + alt * 0.2 + exposure * 0.2;
					if (RockLikelihood > 0.51) {
						RockProportion = 1;
					} else {
						if (RockLikelihood < 0.50) RockProportion = 0;
						else RockProportion = (RockLikelihood - 0.50) / 0.01;
						
						float	ForestLikelihood = -0.5 + (1 - alt) * 0.7 + normy * 0.8 + exposure * -0.27;
						if (ForestLikelihood > 0.51) {
							ForestProportion = 1 - RockProportion;
						} else {
							if (ForestLikelihood < 0.50) PowderProportion = 1 - RockProportion;
							else {
								ForestProportion = (1 - RockProportion) * (ForestLikelihood - 0.50) / 0.01;
								PowderProportion = 1 - RockProportion - ForestProportion;
							}
						}
					}
					
					type = (5 * 16) * RockProportion + (0 * 16) * ForestProportion + (4 * 16) * PowderProportion;
					*t++ = type;
					
					worldx += step;
				}
				
				worldz += step;
			}
		}
		
		e = e->Next;
	} while (e != GetGridList());
}


void CTerrainDoc::ReSortGrid(Grid * g)
// If necessary, adjusts the position of the given grid in our internal grid list,
// due to changes in the grid parameters.
{
	// Find the grid element...
	GridElem*	e = GridList;
	if (e) {
		GridElem*	f = NULL;
		do {
			if (e->Grid == g) {
				f = e;
				break;
			}
			e = e->Next;
		} while (e != GridList);

		if (f == NULL) return;	// Can't find the grid... could do a debug assert.

		// See if the grid should be moved...
		for (;;) {
			if (f != GridList && g->Compare(*(f->Previous->Grid)) < 0) {
				// Move backwards.  Just swap grid pointers rather than futzing with next/previous links.
				Grid*	temp = f->Previous->Grid;
				f->Previous->Grid = g;
				f->Grid = temp;
				f = f->Previous;
			} else if (f->Next != GridList && g->Compare(*(f->Next->Grid)) > 0) {
				// Move forwards.
				Grid*	temp = f->Next->Grid;
				f->Next->Grid = g;
				f->Grid = temp;
				f = f->Next;
			} else {
				// We're OK.
				break;
			}
		}
	}
}
