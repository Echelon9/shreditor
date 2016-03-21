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
// Polygon.h: interface for the polygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYGON_H__7BF66520_27A8_11D2_83C5_444553540000__INCLUDED_)
#define AFX_POLYGON_H__7BF66520_27A8_11D2_83C5_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "ObjectProperties.h"


class CArchive;
class CTerrainDoc;


class Vertex {
public:
	float	x, z;
	Vertex	*Next, *Previous;
};


class polygon  
{
public:
	polygon();
	polygon(float xcenter, float zcenter, float ApproxSize);
	virtual ~polygon();

	bool	GetSelected() { return Selected; }
	void	SetSelected(bool s) { Selected = s; }

	bool	GetClosed() { return Closed; }
	void	SetClosed(bool c) { Closed = c; }

	int	GetPropertyCount() { return GetPolygonPropertyCount(); }
	PropertyValue*	GetProperty(int i) { return Property[i]; }

	void	Serialize(CArchive& ar, CTerrainDoc* doc);

	Vertex*	GetVertexLoop() { return VertexLoop; }

	int	GetType() { return Type; }
	void	SetType(int t) { Type = t; }

	bool	DeleteVertex(Vertex* v);
	Vertex*	AddVertexAfter(Vertex* v, float x, float z);
	Vertex*	FindNearestVertex(float* Distance, float x, float z);
	Vertex*	FindNearestEdgeVertex(float* Distance, float x, float z);

private:
	void	DeleteVertices();
	
	Vertex*	VertexLoop;
	int	Type;

	bool	Selected;
	bool	Closed;

	PropertyValue**	Property;
};


struct PolygonElem {
	polygon*	poly;
	PolygonElem	*Next, *Previous;
};


#endif // !defined(AFX_SURFACEPOLYGON_H__7BF66520_27A8_11D2_83C5_444553540000__INCLUDED_)
