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
// Solid.cpp


#include "stdafx.h"

#include "terrain.h"
#include "solid.h"


Solid::Solid()
// Constructor.
{
	Name = NULL;
	Comment = NULL;
	Type = BOX;
	for (int i = 0; i < 3; i++) Dim[i] = 0;

	Next = NULL;
	Previous = NULL;
}


Solid::~Solid()
// Destructor.
{
	if (Name) delete [] Name;
	if (Comment) delete [] Comment;
}


void	Solid::SetName(const char* NewName)
// Sets the name of this Solid.
{
	if (Name) delete [] Name;
	Name = new char[strlen(NewName) + 1];
	strcpy(Name, NewName);
}


void	Solid::SetComment(const char* NewComment)
// Sets the comment of this Solid.
{
	if (Comment) delete [] Comment;
	Comment = new char[strlen(NewComment) + 1];
	strcpy(Comment, NewComment);
}


/////////////////////////////////////////////////////////////////////////////
// Solid serialization

void Solid::Serialize(CArchive & ar, CTerrainDoc * doc)
{
	if (ar.IsStoring()) {
		// Storing code.

		// Write the name.
		ar.WriteString(Name);
		ar.WriteString("\n");

		// Write the comment.
		ar.WriteString(Comment);
		ar.WriteString("\n");

		// Write the type.
		ar << BYTE(Type);

		// Write the dimensions.  3 if a box, 2 if a cylinder.
		int	WriteCount = 3;
		if (Type == CYLINDER) WriteCount = 2;

		for (int i = 0; i < WriteCount; i++) {
			ar << Dim[i];
		}
		
	} else {
		// Loading code.

		char	temp[80];

		// Read the name.
		ar.ReadString(temp, 80);
		SetName(temp);

		// Read the comment.
		ar.ReadString(temp, 80);
		SetComment(temp);

		// Read the type.
		BYTE	t;
		ar >> t;
		SetType(SolidType(t));

		// Read the dimensions.
		int	i;
		int	ReadCount = 3;
		if (Type == CYLINDER) ReadCount = 2;

		for (i = 0; i < ReadCount; i++) {
			ar >> Dim[i];
		}

		// Zero the remaining dimensions.
		for ( ; i < 3; i++) {
			Dim[i] = 0;
		}
	}
}
