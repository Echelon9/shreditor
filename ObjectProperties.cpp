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
// ObjectProperties.cpp	-thatcher 2/23/1999

// For managing object types and their named & typed properties.


#include <stdafx.h>
#include <stdlib.h>

#include "terrain.h"

#include "ObjectProperties.h"
#include "PropEditInt.h"
#include "PropEditString.h"
#include "PropEditMultiLineString.h"
#include "PropEditOrientation.h"


struct ObjectAndPropertyInfo {
	PropertyTypeID	Type;
	char*	Name;
} Info[] = {
	{ PROP_OBJECTTYPE, "null" },

	{ PROP_OBJECTTYPE, "Non-oriented static (tree)" },

	{ PROP_OBJECTTYPE, "User starting location" },
	{ PROP_INT, "Run ID" },
	{ PROP_STRING, "Run Name" },
	{ PROP_STRING, "Run Info" },
	{ PROP_ORIENTATION, "Orientation" },
	{ PROP_INT, "Par Seconds" },

	{ PROP_OBJECTTYPE, "Finish trigger" },
	{ PROP_INT, "Run ID" },
	{ PROP_ORIENTATION, "Orientation" },

	{ PROP_OBJECTTYPE, "Oriented static" },
	{ PROP_ORIENTATION, "Orientation" },

	{ PROP_OBJECTTYPE, "Tree clump" },
	// radius, random seed, tree count, etc.

	{ PROP_OBJECTTYPE, "Boarder test" },

	{ PROP_OBJECTTYPE, "Lift base" },
	{ PROP_INT, "Lift ID" },
	{ PROP_STRING, "Lift Name" },
	{ PROP_ORIENTATION, "Orientation" },

	{ PROP_OBJECTTYPE, "Lift support" },
	{ PROP_INT, "Lift ID" },
	{ PROP_ORIENTATION, "Orientation" },

	{ PROP_OBJECTTYPE, "Lift top" },
	{ PROP_INT, "Lift ID" },
	{ PROP_ORIENTATION, "Orientation" },

	{ PROP_LISTEND, "" }
};


const char*	GetPropertyTypeName(PropertyTypeID type)
// Returns the name of the given property type.
{
	switch (type) {
	default:
		return "<unknown>";
	case PROP_OBJECTTYPE:
		return "<object type>";
	case PROP_LISTEND:
		return "<list end>";
	case PROP_INT:
		return "int";
	case PROP_FLOAT:
		return "float";
	case PROP_STRING:
		return "string";
	case PROP_MULTILINE_STRING:
		return "multiline";
	case PROP_BOOL:
		return "bool";
	case PROP_ORIENTATION:
		return "orientation";
	}
}


const int	MAX_OBJECT_TYPES = 20;
struct TypeInfo {
	int	InfoIndex;
	int	PropertyCount;
} TypeData[MAX_OBJECT_TYPES];
int	ObjectTypeCount = 0;


static struct InitTypeInfo {
	InitTypeInfo() {
		// Scan for object types.
		int	i = 0;
		while (Info[i].Type != PROP_LISTEND) {
			if (Info[i].Type == PROP_OBJECTTYPE) {
				TypeInfo& t = TypeData[ObjectTypeCount++];
				t.InfoIndex = i;
				t.PropertyCount = 0;
				// Count properties.
				i++;
				while (Info[i].Type != PROP_LISTEND && Info[i].Type != PROP_OBJECTTYPE) {
					t.PropertyCount++;
					i++;
				}
				continue;
			}

			i++;
		}
	}
} InitTypeInfo;


int	GetObjectTypeCount()
// Returns the number of object types.
{
	return ObjectTypeCount;
}


const char*	GetObjectTypeName(int index)
// Returns the name of the index'th object type.
{
	if (index >= 0 && index < ObjectTypeCount) {
		// Return the name of this object type.
		return Info[TypeData[index].InfoIndex].Name;
	} else {
		return "<bad index>";
	}
}


int	GetPropertyCount(int ObjectType)
// Returns the number of additional properties attached
// to the specified object type.
{
	if (ObjectType >= 0 && ObjectType < ObjectTypeCount) {
		// Return the # of properties.
		return TypeData[ObjectType].PropertyCount;
	} else {
		return 0;
	}
}


const char*	GetPropertyName(int ObjectType, int PropertyIndex)
// Returns the name of the indexed property of the given object type.
{
	if (ObjectType >= 0 && ObjectType < ObjectTypeCount) {
		TypeInfo&	t = TypeData[ObjectType];
		if (PropertyIndex >= 0 && PropertyIndex < t.PropertyCount) {
			return Info[t.InfoIndex + 1 + PropertyIndex].Name;
		} else {
			return "<bad prop index>";
		}
	} else {
		return "<bad type index>";
	}
}


PropertyTypeID	GetPropertyType(int ObjectType, int PropertyIndex)
// Returns the type of the indexed property of the given object type.
{
	if (ObjectType >= 0 && ObjectType < ObjectTypeCount) {
		TypeInfo&	t = TypeData[ObjectType];
		if (PropertyIndex >= 0 && PropertyIndex < t.PropertyCount) {
			return Info[t.InfoIndex + 1 + PropertyIndex].Type;
		} else {
			return PROP_LISTEND;
		}
	} else {
		return PROP_LISTEND;
	}
}


const int	POLY_PROP_COUNT = 1;
ObjectAndPropertyInfo PolyInfo[] = {
	{ PROP_MULTILINE_STRING, "boundary script" },
};


int	GetPolygonPropertyCount() { return POLY_PROP_COUNT; }
const char*	GetPolygonPropertyName(int PropertyIndex) { return PolyInfo[PropertyIndex].Name; }
PropertyTypeID	GetPolygonPropertyType(int PropertyIndex) { return PolyInfo[PropertyIndex].Type; }


PropertyValue::~PropertyValue() {}


//
// Implementation of different property types.
//


struct PropInt : public PropertyValue {
	int	Value;

	PropInt() { SetDefault(); }
	~PropInt() {}
	PropertyTypeID	GetType() { return PROP_INT; }
	void	SetDefault() { Value = 0; }
	bool	UserEdit()
	// Puts up a dialog so the user can edit the value.
	// Returns true if a change was made.
	{
		PropEditInt d;
		d.SetValue(Value);
		if (d.DoModal() != IDCANCEL) {
			// Apply the changes.
			Value = d.GetValue();
			return true;
		} else {
			return false;
		}
	}
	void	WriteToString(char* buf, int CharLimit) { _itoa(Value, buf, 10); }
	virtual void	Serialize(CArchive& ar) {
		if (ar.IsStoring()) {
			ar << Value;
		} else {
			ar >> Value;
		}
	}
};


struct PropString : public PropertyValue {
	char*	Value;

	PropString() { SetDefault(); }
	~PropString() { if (Value) delete [] Value; }

	PropertyTypeID	GetType() { return PROP_STRING; }

	void	SetDefault() { Value = NULL; }

	void	SetValue(const char* val)
	// Sets a new value for the string.
	{
		if (Value) delete [] Value;
		Value = new char[strlen(val) + 1];
		strcpy(Value, val);
	}

	bool	UserEdit()
	// Puts up a dialog so the user can edit the value.
	// Returns true if a change was made.
	{
		PropEditString d;
		d.SetValue(Value);
		if (d.DoModal() != IDCANCEL) {
			// Apply the changes.
			SetValue(d.GetValue());
			return true;
		} else {
			return false;
		}
	}
	void	WriteToString(char* buf, int CharLimit)
	// Puts the string representation of the value in buf.
	{
		if (Value) {
			strncpy(buf, Value, CharLimit);
			buf[CharLimit-1] = 0;
		} else {
			strncpy(buf, "---", CharLimit);
			buf[CharLimit-1] = 0;
		}
	}
	virtual void	Serialize(CArchive& ar) {
		if (ar.IsStoring()) {
			if (Value) ar.WriteString(Value);
			ar.WriteString("\n");
		} else {
			char	temp[5000];
			ar.ReadString(temp, 5000);
			SetValue(temp);
		}
	}
};


struct PropMultiLineString : public PropertyValue {
	char*	Value;

	PropMultiLineString() { SetDefault(); }
	~PropMultiLineString() { if (Value) delete [] Value; }

	PropertyTypeID	GetType() { return PROP_MULTILINE_STRING; }

	void	SetDefault() { Value = NULL; }

	void	SetValue(const char* val)
	// Sets a new value for the string.
	{
		if (Value) delete [] Value;
		Value = new char[strlen(val) + 1];
		strcpy(Value, val);
	}

	bool	UserEdit()
	// Puts up a dialog so the user can edit the value.
	// Returns true if a change was made.
	{
		PropEditMultiLineString d;
		d.SetValue(Value);
		if (d.DoModal() != IDCANCEL) {
			// Apply the changes.
			SetValue(d.GetValue());
			return true;
		} else {
			return false;
		}
	}
	void	WriteToString(char* buf, int CharLimit)
	// Puts the string representation of the value in buf.
	{
		if (Value) {
			strncpy(buf, Value, CharLimit);
			buf[CharLimit-1] = 0;
		} else {
			strncpy(buf, "---", CharLimit);
			buf[CharLimit-1] = 0;
		}
	}
	virtual void	Serialize(CArchive& ar) {
		if (ar.IsStoring()) {
			if (Value) {
				unsigned short	len = strlen(Value);
				ar << len;
				ar.Write(Value, len);
			} else {
				ar << (unsigned short) 0;
			}
//			ar.WriteString("\n");
		} else {
			unsigned short	len;
			ar >> len;
			if (len) {
				if (Value) delete [] Value;
				Value = new char[len+1];
				ar.Read(Value, len);
				Value[len] = 0;
			} else {
				SetValue("");
			}
		}
	}
};


struct PropOrientation : public PropertyValue {
	float	Yaw;

	PropOrientation() { SetDefault(); }
	~PropOrientation() {}

	PropertyTypeID	GetType() { return PROP_ORIENTATION; }

	void	SetDefault() { Yaw = 0; }

	void	SetValue(float NewYaw)
	// Sets a new value for the orientation.
	{
		Yaw = NewYaw;
	}

	bool	UserEdit()
	// Puts up a dialog so the user can edit the value.
	// Returns true if a change was made.
	{
		PropEditOrientation d;
		d.SetValue(Yaw);
		if (d.DoModal() != IDCANCEL) {
			// Apply the changes.
			SetValue(d.GetValue());
			return true;
		} else {
			return false;
		}
	}
	void	WriteToString(char* buf, int CharLimit)
	// Puts the string representation of the value in buf.
	{
		sprintf(buf, "%g", Yaw);
	}
	virtual void	Serialize(CArchive& ar) {
		if (ar.IsStoring()) {
			ar << Yaw;
		} else {
			ar >> Yaw;
		}
	}
};




PropertyValue*	CreatePropertyValue(PropertyTypeID type)
// Creates a property of the given type, if possible.
{
	if (type == PROP_INT) {
		return new PropInt;
	} else if (type == PROP_STRING) {
		return new PropString;
	} else if (type == PROP_MULTILINE_STRING) {
		return new PropMultiLineString;
	} else if (type == PROP_ORIENTATION) {
		return new PropOrientation;
	} else {
		return NULL;
	}
}



//PropertyValue*	CreatePropertyValue(PropertyTypeID type);
