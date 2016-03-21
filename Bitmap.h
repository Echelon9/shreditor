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
// bitmap.h

// stuff for handling and loading bitmaps.


#ifndef BITMAP_H
#define BITMAP_H


typedef unsigned char uint8;
typedef unsigned int uint32;


// Generic base class for bitmaps.
class bitmap {
public:
	virtual ~bitmap() {}
	int	GetWidth() { return Width; }
	int	GetHeight() { return Height; }
	
protected:
	int	Width, Height;
	bitmap(int w, int h) { Width = w; Height = h; };
};


// class bitmap32 -- for storing 32-bit ARGB textures.

class bitmap32 : public bitmap {
public:
	bitmap32(int w, int h) : bitmap(w, h) { Data = new uint32[Width * Height]; }
	~bitmap32() { delete [] Data; }
	uint32*	GetData() { return Data; }
//	void	ProcessForColorKeyZero();
private:
	uint32*	Data;
};


// class bitmap8 -- for storing 8-bit grayscale bitmaps.
class bitmap8 : public bitmap {
public:
	bitmap8(int w, int h) : bitmap(w, h) { Data = new uint8[Width * Height]; }
	~bitmap8() { delete [] Data; }
	uint8*	GetData() { return Data; }
private:
	uint8*	Data;
};


namespace Bitmap {
	bitmap8*	ReadImageData8(const char* filename);
	void	WriteImageData8(const char* filename, uint8* Data, int Width, int Height);

	void	ReadTypeMapFromImage32(const char* filename, uint8* TypeData, int Width, int Height);
	void	WriteImage32FromTypeMap(const char* filename, uint8* TypeData, int Width, int Height);
	
//	void	ReadImageDataIndexed(const char* filename, uint8* IndexData, int Width, int Height);
//	void	WriteImageDataIndexed(const char* filename, uint8* IndexData, int Width, int Height);
};


#endif // BITMAP_H

