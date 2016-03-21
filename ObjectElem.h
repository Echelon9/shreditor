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
// ObjectElem.h: interface for the ObjectElem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTELEM_H__A61CA102_F9A1_11D1_83C5_444553540000__INCLUDED_)
#define AFX_OBJECTELEM_H__A61CA102_F9A1_11D1_83C5_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "Object.h"


class ObjectElem  
{
public:
	ObjectElem();
	virtual ~ObjectElem();

	Object*	Object;
	ObjectElem*	Next;
	ObjectElem*	Previous;
};


#endif // !defined(AFX_OBJECTELEM_H__A61CA102_F9A1_11D1_83C5_444553540000__INCLUDED_)
