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
// Object.cpp: implementation of the Object class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "terrain.h"
#include "Object.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Object::Object()
{
	Type = 0;
	PinToGround = true;
	Location.x = Location.y = Location.z = 0;
	GeometryReference = 0;
	SolidReference = 0;
	Selected = false;
	Property = NULL;
}


Object::~Object()
{
	// Delete properties.
	if (Property) {
		int	count = GetPropertyCount();
		for (int i = 0; i < count; i++) {
			if (Property[i]) delete Property[i];
		}

		delete [] Property;
	}
}


void	Object::SetType(int NewType)
// Sets the type of the object.  Adjusts the property list if necessary.
{
	if (NewType == Type) return;

	int	i;
	int	OldPropCount = ::GetPropertyCount(Type);
	int	NewPropCount = ::GetPropertyCount(NewType);
	Type = NewType;

	// Delete the old properties.
	for (i = 0; i < OldPropCount; i++) {
		if (Property[i]) delete Property[i];
	}

	// Delete the array, if necessary.
	if (Property) {
		if (OldPropCount != NewPropCount) {
			delete [] Property;
		}
	}

	// Allocate a new array, if necessary.
	if (NewPropCount) {
		Property = new PropertyValue* [NewPropCount];

		// Create the default properties.
		for (i = 0; i < NewPropCount; i++) {
			Property[i] = CreatePropertyValue(GetPropertyType(NewType, i));
		}
	} else {
		Property = NULL;
	}	
}


void	Object::Serialize(CArchive& ar, CTerrainDoc* doc)
// Read & write.
{
	if (ar.IsStoring()) {
		// Write the object info.

		float	YOut = Location.y;

		if (PinToGround) {
			YOut = doc->GetHeight(Location.x, Location.z) + Location.y;
		}

		ar << Type;
		ar << int(PinToGround);
		ar << Location.x;
		ar << YOut;
		ar << Location.z;
		ar << GeometryReference;	// ver >= 2
		ar << SolidReference;	// ver >= 4

		// Write properties.
		// ver >= ....
		int	i;
		int	count = GetPropertyCount();
		for (i = 0; i < count; i++) {
			Property[i]->Serialize(ar);
		}

	} else {
		// Read the object info.

		int	NewType;
		ar >> NewType;
		SetType(NewType);

		int temp;
		ar >> temp;
		PinToGround = temp;
		float	x, y, z;
		ar >> x;
		ar >> y;
		ar >> z;
		SetLocation(doc, x, y, z);
		if (PinToGround) {
			Location.y -= doc->GetHeight(Location.x, Location.z);
		}

		// Read the geometry info, if it's available.
		if (doc->GetInFileVersion() >= 2) {
			ar >> GeometryReference;
		} else {
			GeometryReference = 0;
		}
		
		// Read solid info, if it's available.
		if (doc->GetInFileVersion() >= 4) {
			ar >> SolidReference;
		} else {
			SolidReference = 0;
		}

		// Read property info, if it's available.
		if (doc->GetInFileVersion() >= 5) {
			int	i;
			int	count = GetPropertyCount();
			for (i = 0; i < count; i++) {
				// Earlier versions lack some object properties.
				bool	Read = true;
				if (doc->GetInFileVersion() < 6) {
					if (Type == 2 && strcmp(GetPropertyName(Type, i), "Run Info") == 0) Read = false;
				}

				if (doc->GetInFileVersion() < 7) {
					if (Property[i]->GetType() == PROP_ORIENTATION) Read = false;
				}

				if (doc->GetInFileVersion() < 12) {
					if (Type == 2 && strcmp(GetPropertyName(Type, i), "Par Seconds") == 0) Read = false;
				}

				if (Read) {
					Property[i]->Serialize(ar);
				} else {
					Property[i]->SetDefault();
				}
			}
		} else {
			// No info in file.  Just set default values for properties.
			int	i;
			int	count = GetPropertyCount();
			for (i = 0; i < count; i++) {
				Property[i]->SetDefault();
			}
		}

//		// xxxx hack in the hemlocks.
//		if (Type == 1) {
//			GeometryReference = 1;	// hemlock geometry.
//			SolidReference = 1;	// hemlock solid.
//		}
	}
}


int	Object::GetPropertyCount()
// Returns the number of additional typed properties this object has.
{
	return ::GetPropertyCount(GetType());
}


PropertyValue*	Object::GetProperty(int index)
// Returns the indexed property, if it exists.
{
	if (index >= 0 && index < GetPropertyCount()) {
		return Property[index];
	} else {
		return NULL;
	}
}


#define COLORCOUNT 5
static COLORREF	ObjectColors[COLORCOUNT] = {
	RGB(200, 0, 0),
	RGB(0, 200, 0),
	RGB(0, 0, 200),
	RGB(200, 0, 200),
	RGB(200, 200, 0),
};


void	Object::Draw(CDC* dc, CPoint p)
// Draw a marker for the object.  If highlight is true, then highlights as
// a selected object.
{
	// Pick a color based on the type.
	int	ColorIndex = Type % COLORCOUNT;

//	DrawBox(dc, p.x - 2, p.y - 2, 4, 4, ObjectColors[ColorIndex]);
//	DrawBox(dc, p.x - 3, p.y - 3, 6, 6, ObjectColors[ColorIndex]);
	COLORREF	c = ObjectColors[ColorIndex];
	dc->SetPixelV(p.x, p.y, c);
	dc->SetPixelV(p.x-1, p.y, c);
	dc->SetPixelV(p.x+1, p.y, c);
	dc->SetPixelV(p.x, p.y-1, c);
	dc->SetPixelV(p.x, p.y+1, c);

	// Draw a highlight if this object is selected.
	if (Selected) {
		DrawBox(dc, p.x - 3, p.y - 3, 6, 6, RGB(0, 0, 200));
	}
}


void	Object::GeometryListDeletion(int DeletedIndex)
// This function is called to notify the object that a geometry index
// has been deleted, so adjustments can be made if necessary.  If our
// own geometry index is > the deleted index then we should decrement
// our index, since the corresponding geometry will have its index
// decremented.
{
	if (DeletedIndex == GeometryReference) {
		GeometryReference = 0;
	} else if (DeletedIndex <= GeometryReference) {
		GeometryReference -= 1;
	}
}


void	Object::SolidListDeletion(int DeletedIndex)
// Like GeometryListDeletion(), but for solids.
{
	if (DeletedIndex == SolidReference) {
		SolidReference = 0;
	} else if (DeletedIndex <= SolidReference) {
		SolidReference -= 1;
	}
}

