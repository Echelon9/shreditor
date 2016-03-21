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
// Solid.h	Class for holding solid info, for Shreditor.


#ifndef SOLID_H
#define SOLID_H


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "terraindoc.h"


class Solid
{
public:
	void Serialize(CArchive& ar, CTerrainDoc* doc);
	Solid();
	~Solid();

	enum SolidType { BOX = 0, CYLINDER = 1 };
	
	const char*	GetName() { return Name; }
	const char*	GetComment() { return Comment; }
	SolidType	GetType() { return Type; }
	float	GetDimension(int index) { return Dim[index]; }

	void	SetName(const char* NewName);
	void	SetComment(const char* NewComment);
	void	SetType(SolidType t) { Type = t; }
	void	SetDimension(int index, float val) { Dim[index] = val; }
	
	Solid*	Next;
	Solid*	Previous;

private:
	char*	Name;
	char*	Comment;

	SolidType	Type;
	float	Dim[3];	// x, y, z / length, height, width for box; radius, height, null for cylinder
};


#endif	// SOLID_H
