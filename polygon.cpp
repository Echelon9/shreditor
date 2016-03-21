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
// polygon.cpp: implementation of the polygon class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "terrain.h"
#include "polygon.h"
#include "TerrainDoc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <float.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



polygon::polygon()
// Constructs a new surface polygon, containing no vertices.
{
	VertexLoop = NULL;
	Type = 0;

	Selected = false;
	Closed = false;

	// Create the default properties.
	int	ct = GetPropertyCount();
	Property = new PropertyValue* [ct];
	for (int i = 0; i < ct; i++) {
		Property[i] = CreatePropertyValue(GetPolygonPropertyType(i));
	}
}


polygon::polygon(float xcenter, float zcenter, float size)
// Constructs a new surface polygon.  Initializes it to a diamond shape
// centered around the given coordinates and with an edge length of approximately
// "size".
{
	VertexLoop = NULL;
	Type = 0;

	Selected = false;
	Closed = false;

	// Make the default vertex loop.
	Vertex*	v = AddVertexAfter(GetVertexLoop(), xcenter, zcenter + size);
	v = AddVertexAfter(v, xcenter + size, zcenter);
	v = AddVertexAfter(v, xcenter, zcenter - size);
	v = AddVertexAfter(v, xcenter - size, zcenter);

	// Create the default properties.
	int	ct = GetPropertyCount();
	Property = new PropertyValue* [ct];
	for (int i = 0; i < ct; i++) {
		Property[i] = CreatePropertyValue(GetPolygonPropertyType(i));
	}
}


polygon::~polygon()
// Delete the vertices which make up this polygon.
{
	DeleteVertices();

	// Delete properties.
	if (Property) {
		int	count = GetPropertyCount();
		for (int i = 0; i < count; i++) {
			if (Property[i]) delete Property[i];
		}

		delete [] Property;
	}
}


void	polygon::DeleteVertices()
// Deletes all the vertices of this polygon.
{
	Vertex*	e = GetVertexLoop();
	if (e) {
		Vertex*	v = e;
		do {
			Vertex*	temp = e->Next;
			delete e;
			e = temp;
		} while (v != e);
	}

	VertexLoop = NULL;
}


bool	polygon::DeleteVertex(Vertex* v)
// Looks for the given vertex in this polygon, and deletes it if it finds it.
// Returns true if the polygon now has fewer than 2 vertices; otherwise returns
// false.
{
	// Look for the specified vertex.
	Vertex*	e = VertexLoop;
	if (e) {
		do {
			if (e == v) {
				// This is the vertex we want to delete.

				// Make sure VertexLoop will be valid.
				if (VertexLoop == e) {
					VertexLoop = e->Next;
					if (VertexLoop == e) VertexLoop = NULL;
				}

				// Splice e out of the loop.
				e->Previous->Next = e->Next;
				e->Next->Previous = e->Previous;

				// Free the actual vertex.
				delete e;

				break;
			}

			e = e->Next;
		} while (e != VertexLoop);
	}

	// Count the remaining vertices.
	int	VertexCount = 0;
	e = VertexLoop;
	if (e) {
		do {
			VertexCount++;
			e = e->Next;
		} while (e != VertexLoop);
	}

	// Return true if this polygon is now degenerate.
	if (VertexCount < 2) return true;
	else return false;
}


void	polygon::Serialize(CArchive& ar, CTerrainDoc* doc)
// Reads/writes this polygon from the given archive.
{
	if (ar.IsStoring()) {
		// Write the polygon info.

		// Write the surface type.
		ar << Type;

		// Count the vertices.
		int	VertexCount = 0;
		Vertex*	v = VertexLoop;
		if (v) {
			do {
				VertexCount++;
				v = v->Next;
			} while (v != VertexLoop);
		}
		
		// Write the vertex count.
		ar << VertexCount;

		// Write the vertex info.
		v = VertexLoop;
		if (v) {
			do {
				ar << v->x;
				ar << v->z;
				v = v->Next;
			} while (v != VertexLoop);
		}

		// Write Closed flag.
		unsigned char	c = Closed ? 1 : 0;
		ar << c;
		
		// Write properties.
		int	i;
		int	count = GetPropertyCount();
		for (i = 0; i < count; i++) {
			Property[i]->Serialize(ar);
		}
		
	} else {
		// Read the polygon info.

		DeleteVertices();
		
		ar >> Type;

		int	VertexCount;
		ar >> VertexCount;

		Vertex*	v = VertexLoop;
		for (int i = 0; i < VertexCount; i++) {
			float	x, z;
			ar >> x;
			ar >> z;
			v = AddVertexAfter(v, x, z);
		}
		
		// Read property info, if it's available.
		if (doc->GetInFileVersion() >= 11) {
			// Read Closed flag
			unsigned char	c;
			ar >> c;
			Closed = (c != 0);
			
			int	i;
			int	count = GetPropertyCount();
			for (i = 0; i < count; i++) {
				Property[i]->Serialize(ar);
			}
		} else {
			// Legacy polygons are closed by default.
			Closed = true;
			
			// No info in file.  Just set default values for properties.
			int	i;
			int	count = GetPropertyCount();
			for (i = 0; i < count; i++) {
				Property[i]->SetDefault();
			}
		}
	}
}


Vertex*	polygon::AddVertexAfter(Vertex* v, float x, float z)
// Inserts a new vertex into this polygon.  Places the new vertex
// after the given vertex.  If v == NULL, then inserts the new vertex
// at an arbitrary place in the loop.
// Returns a pointer to the newly created vertex.
{
	if (v == NULL) {
		v = VertexLoop;
	}

	Vertex*	NewVertex = new Vertex;
	NewVertex->x = x;
	NewVertex->z = z;
	
	if (VertexLoop == NULL) {
		// Start the loop with this vertex.
		NewVertex->Next = NewVertex;
		NewVertex->Previous = NewVertex;
		VertexLoop = NewVertex;
	} else {
		// Find v in the vertex loop.
		Vertex* e = VertexLoop;
		do {
			if (e == v) break;
			e = e->Next;
		} while (e != VertexLoop);

		// Splice the new vertex into the loop.
		NewVertex->Next = e->Next;
		e->Next = NewVertex;
		NewVertex->Previous = e;
		NewVertex->Next->Previous = NewVertex;
	}

	return NewVertex;
}


Vertex* polygon::FindNearestVertex(float* Distance, float x, float z)
// Finds and returns the vertex in this polygon nearest to the specified x,z coordinates.
// Stores the distance to the vertex in *Distance.
// Returns NULL and puts FLT_MAX in *Distance if there are no vertices.
{
	Vertex*	BestVertex = NULL;
	*Distance = FLT_MAX;

	Vertex*	v = VertexLoop;

	if (v) {
		do {
			float	d = sqrt((v->x - x) * (v->x - x) + (v->z - z) * (v->z - z));
			if (d < *Distance) {
				BestVertex = v;
				*Distance = d;
			}
				
			v = v->Next;
		} while (v != VertexLoop);
	}

	return BestVertex;
}


Vertex*	polygon::FindNearestEdgeVertex(float* Distance, float x, float z)
// Finds the nearest edge to the given point, and returns the first vertex of the edge.
// Stores the distance to the edge in *Distance.
// If there's no good candidate edge, returns NULL and puts FLT_MAX in *Distance.
{
	Vertex*	BestEdgeVertex = NULL;
	*Distance = FLT_MAX;

	Vertex*	v = VertexLoop;

	if (v) {
		do {
			float	px = x - v->x;
			float	pz = z - v->z;
			float	ex = v->Next->x - v->x;
			float	ez = v->Next->z - v->z;
			float	dot = px * ex + pz * ez;
			float	pt2 = px * px + pz * pz;
			float	edge2 = ex * ex + ez * ez;

			// Make sure the point is between the endpoints.
			if (edge2 > 0.001 && dot >= 0 && dot <= edge2) {
				// Compute the perpendicular distance from the point to the edge.
				float	d = sqrt(pt2 - (dot * dot) / edge2);
				
				if (d < *Distance) {
					BestEdgeVertex = v;
					*Distance = d;
				}
			}
				
			v = v->Next;
		} while (v != VertexLoop);
	}

	return BestEdgeVertex;
}

