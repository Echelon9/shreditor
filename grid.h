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
// Grid.h	Grid data class.


#ifndef GRID_H
#define GRID_H


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


const int	MAX_GRID_SIZE = 1024;

class CTerrainDoc;


class Grid
{
public:
	int Compare(const Grid& g);
	void Resize(int NewXSize, int NewZSize);
	Grid();
	virtual ~Grid();

	void Serialize(CArchive& ar, int InFileVersion, CTerrainDoc* doc);

	bool	GetSelected() { return Selected; }
	void	SetSelected(bool s) { Selected = s; }
//	float*	GetData() { return Data; }
	unsigned char*	GetTypeData() { return TypeData; }
	int	GetXSize() { return XSize; }
	int	GetZSize() { return ZSize; }
	int	GetXOrigin() { return XOrigin; }
	int	GetZOrigin() { return ZOrigin; }
	int	GetScale() { return ScaleLevel; }

	void	SetXOrigin(int NewXOrigin) { XOrigin = NewXOrigin; }
	void	SetZOrigin(int NewZOrigin) { ZOrigin = NewZOrigin; }

	void	SetXSize(int NewXSize);
	void	SetZSize(int NewZSize);

	void	SetScale(int NewScale);

//	void	ScaleData(float factor);

//	void	AddIntoBuffer(float* Buffer, int XOffset, int ZOffset, int XSize, int ZSize, int Pitch, int ZoomLevel);
	void	PaintIntoBuffer(unsigned char* Buffer, int BufferXOffset, int BufferZOffset, int BufferXSize, int BufferZSize, int Pitch, int ZoomLevel);

//	float	GetHeight(float x, float z);
	unsigned char*	GetType(float x, float z);

private:
	int	ScaleLevel;
	int	XOrigin, ZOrigin, XSize, ZSize;
//	float*	Data;
	unsigned char*	TypeData;
	bool	Selected;
};


#endif	// GRID_H
