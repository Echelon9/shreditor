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
// ObjectProperties.h	-thatcher 2/23/1999

// For keeping track of named & typed object properties.


#ifndef OBJECTPROPERTIES_H
#define OBJECTPROPERTIES_H


class CArchive;


enum PropertyTypeID {
	PROP_LISTEND = 0,
	PROP_OBJECTTYPE,
	PROP_INT,
	PROP_FLOAT,
	PROP_STRING,
	PROP_BOOL,
	PROP_GEOMETRY,
	PROP_SOLID,
	PROP_ANGLE,
	PROP_ORIENTATION,
	PROP_MULTILINE_STRING,
//??	PROP_OBJECT,	// For linking to another object...
};


struct PropertyValue {
	virtual ~PropertyValue();
	virtual PropertyTypeID	GetType() = 0;
	virtual void	SetDefault() = 0;
	virtual bool	UserEdit() = 0;
	virtual void	WriteToString(char* buf, int CharLimit) = 0;
	virtual void	Serialize(CArchive& ar) = 0;
};


PropertyValue*	CreatePropertyValue(PropertyTypeID type);


int	GetObjectTypeCount();
const char*	GetObjectTypeName(int ObjectType);
int	GetPropertyCount(int ObjectType);
const char*	GetPropertyName(int ObjectType, int PropertyIndex);
PropertyTypeID	GetPropertyType(int ObjectType, int PropertyIndex);
const char*	GetPropertyTypeName(PropertyTypeID type);

int	GetPolygonPropertyCount();
const char*	GetPolygonPropertyName(int PropertyIndex);
PropertyTypeID	GetPolygonPropertyType(int PropertyIndex);


#endif // OBJECTPROPERTIES_H
