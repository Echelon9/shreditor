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
// Object.h: interface for the Object class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECT_H__A61CA103_F9A1_11D1_83C5_444553540000__INCLUDED_)
#define AFX_OBJECT_H__A61CA103_F9A1_11D1_83C5_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "TerrainDoc.h"
#include "ObjectProperties.h"


class Object  
{
public:
	Object();
	virtual ~Object();

	void	Serialize(CArchive& ar, CTerrainDoc* doc);

	bool	GetSelected() { return Selected; }
	void	SetSelected(bool s) { Selected = s; }

	void	SetType(int NewType); // { Type = NewType; }
	int	GetType() { return Type; }

	bool	GetReferenceToGround() { return PinToGround; }
	void	SetReferenceToGround(bool ref) { PinToGround = ref; }

	float	GetX() { return Location.x; }
	float	GetY() { return Location.y; }
	float	GetZ() { return Location.z; }

	void	SetLocation(CTerrainDoc* doc, float x, float y, float z)
	{
		Location.y = y;
		doc->MoveObject(this, x, z);
	}

	void	ForceLocation(float x, float y, float z)
	// Move the object, without updating the document buckets.  Must call
	// FixupObjectBuckets() on the document after doing this, to make sure
	// the object is linked in the right place.
	{
		Location.x = x;
		Location.y = y;
		Location.z = z;
	}

	void	SetX(CTerrainDoc* doc, float x) { doc->MoveObject(this, x, Location.z); }
	void	SetY(float y) { Location.y = y; }
	void	SetZ(CTerrainDoc* doc, float z) { doc->MoveObject(this, Location.x, z); }

	int	GetGeometryReference() { return GeometryReference; }
	void	SetGeometryReference(int g) { GeometryReference = g; }
	void	GeometryListDeletion(int DeletedIndex);

	int	GetSolidReference() { return SolidReference; }
	void	SetSolidReference(int s) { SolidReference = s; }
	void	SolidListDeletion(int DeletedIndex);

	int	GetPropertyCount();
	PropertyValue*	GetProperty(int index);

	void	Draw(CDC* dc, CPoint p);

private:
	int	Type;
	PropertyValue**	Property;
	struct {
		float	x, y, z;
	} Location;
	int	GeometryReference;
	int	SolidReference;
	bool	PinToGround;
	bool	Selected;

	friend class CTerrainDoc;
};


#endif // !defined(AFX_OBJECT_H__A61CA103_F9A1_11D1_83C5_444553540000__INCLUDED_)
