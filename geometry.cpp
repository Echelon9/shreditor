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
// Grid.cpp


#include "stdafx.h"

#include "terrain.h"
#include "geometry.h"


Geometry::Geometry()
// Constructor.
{
	Name = NULL;
	Comment = NULL;
	Filename = NULL;
	// index, in use, etc.

	Next = NULL;
	Previous = NULL;
}


Geometry::~Geometry()
// Destructor.
{
	if (Name) delete [] Name;
	if (Comment) delete [] Comment;
	if (Filename) delete [] Filename;
}


void	Geometry::SetName(const char* NewName)
// Sets the name of this geometry.
{
	if (Name) delete [] Name;
	Name = new char[strlen(NewName) + 1];
	strcpy(Name, NewName);
}


void	Geometry::SetComment(const char* NewComment)
// Sets the comment of this geometry.
{
	if (Comment) delete [] Comment;
	Comment = new char[strlen(NewComment) + 1];
	strcpy(Comment, NewComment);
}


void	Geometry::SetFilename(const char* NewFilename)
// Sets the filename of this geometry.
{
	if (Filename) delete [] Filename;
	Filename = new char[strlen(NewFilename) + 1];
	strcpy(Filename, NewFilename);
}


/////////////////////////////////////////////////////////////////////////////
// Geometry serialization

void Geometry::Serialize(CArchive & ar, CTerrainDoc * doc)
{
	if (ar.IsStoring()) {
		// Storing code.

		// Write the name.
		ar.WriteString(Name);
		ar.WriteString("\n");

		// Write the filename.
		ar.WriteString(Filename);
		ar.WriteString("\n");

		// Write the comment.
		ar.WriteString(Comment);
		ar.WriteString("\n");

	} else {
		// Loading code.

		char	temp[80];

		// Read the name.
		ar.ReadString(temp, 80);
		SetName(temp);

		// Read the filename.
		ar.ReadString(temp, 80);
		SetFilename(temp);

		// Read the comment.
		ar.ReadString(temp, 80);
		SetComment(temp);
	}
}
