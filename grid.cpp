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
#include "grid.h"
#include "terraindoc.h"
#include "terrainview.h"


Grid::Grid()
{
	ScaleLevel = 0;
	XOrigin = ZOrigin = 0;

	//xxxx
	XSize = MAX_GRID_SIZE;
	ZSize = MAX_GRID_SIZE;

	TypeData = new unsigned char[XSize * ZSize];

	// Initialize data.
	int	i, j;
//	float*	p = Data;
	unsigned char*	t = TypeData;
	for (j = 0; j < ZSize; j++) {
		for (i = 0; i < XSize; i++) {
//			*p++ = 0;
			*t++ = 0;
		}
	}

	Selected = false;
}


Grid::~Grid()
{
//	if (Data) delete [] Data;
	if (TypeData) delete [] TypeData;
}


//
// Grid serialization
//


void Grid::Serialize(CArchive& ar, int InFileVersion, CTerrainDoc* doc)
{
	if (ar.IsStoring())
	{
		// Storing code.

		// Store the grid parameters.
		ar << XSize;
		ar << ZSize;
		ar << XOrigin;
		ar << ZOrigin;
		ar << ScaleLevel;

		// Store typemap data.
		int	values = XSize * ZSize;
		unsigned char*	t = TypeData;
		int	i;
		for (i = 0; i < values; i++) {
			ar << *t;
			t++;
		}
	}
	else
	{
		// Loading code.

		// First, read and set the grid size.
		int	NewXSize, NewZSize;
		ar >> NewXSize;
		ar >> NewZSize;

		Resize(NewXSize, NewZSize);

		// Read the other parameters.
		ar >> XOrigin;
		ar >> ZOrigin;
		ar >> ScaleLevel;

		int	values = XSize * ZSize;
		int	i;
		if (InFileVersion <= 8) {
			// Older version, w/ height data contained in
			// the grids.  Make a heightmap and add it into
			// the document's quadtree height structure.
			HeightMapInfo	hm;
			hm.XSize = XSize;
			hm.ZSize = ZSize;
			hm.Data = CTerrainView::GetTempGridArray();
			hm.XOrigin = XOrigin << ScaleLevel;
			hm.ZOrigin = ZOrigin << ScaleLevel;
			hm.RowWidth = XSize;
			hm.Scale = ScaleLevel;

			// Load the height samples.
			float*	p = hm.Data;
			for (i = 0; i < values; i++) {
				ar >> *p;
				p++;
			}
			
			doc->AddHeightMap(hm);
		}

		if (InFileVersion >= 8) {
			// Read the typemap.
			if (InFileVersion < 10) {
				// Older version, with types stored scaled up by 4 bits, so
				// un-scale while reading.
				unsigned char*	t = TypeData;
				for (i = 0; i < values; i++) {
					ar >> *t;
					*t >>= 4;	// un-scale.
					t++;
				}
			} else {
				// Read as-is.
				unsigned char*	t = TypeData;
				for (i = 0; i < values; i++) {
					ar >> *t;
					t++;
				}
			}
		} else {
			// Older format lacking typemap.  Just clear out the type data.
			unsigned char*	t = TypeData;
			for (i = 0; i < values; i++) {
				*t++ = 0;
			}
		}
	}
}


#ifdef NOT
static void	AddHeight(float* Buffer, int BufferXSize, int BufferZSize, int Pitch, int x, int z, float sample)
// Adds the sample height at (x, z) to the buffer.  Checks the (x, z) coordinates to make sure they're in
// bounds before adding.
{
	if (x < 0 || x >= BufferXSize || z < 0 || z >= BufferZSize) return;

	Buffer[x + z * Pitch] += sample;
}
#endif // NOT


static void	SetType(unsigned char* Buffer, int BufferXSize, int BufferZSize, int Pitch, int x, int z, unsigned char sample)
// Sets the type value at (x, z) in the buffer.  Checks the (x, z) coordinates to make sure they're in
// bounds before adding.
{
	if (x < 0 || x >= BufferXSize || z < 0 || z >= BufferZSize) return;

	Buffer[x + z * Pitch] = sample;
}


void	Grid::PaintIntoBuffer(unsigned char* Buffer, int BufferXOffset, int BufferZOffset, int BufferXSize, int BufferZSize, int Pitch, int ZoomLevel)
// This function paints this grid's surface types into the specified section
// of the given buffer.  Pitch determines the row pitch of the 2D destination
// buffer.  ZoomLevel is the power-of-two zoom factor for mapping into the destination
// buffer; e.g. at ZoomLevel 0 the destination and source are 1-to-1, at ZoomLevel 1 the
// destination only sees every other source element, at ZoomLevel 2 the dest only
// sees every fourth source element, etc.
{
	int	i, j;
	unsigned char*	RowStart = Buffer;

	// xxxx many opportunities to optimize this!

	int	mag = ScaleLevel - ZoomLevel;
	if (mag <= 0) {
		// Our grid is unit-scaled or minified.  Take the nearest sample (could anti-alias, but it probably doesn't matter).
		for (j = 0; j < BufferZSize; j++, RowStart += Pitch) {
			unsigned char*	p = RowStart;
			for (i = 0; i < BufferXSize; i++, p++) {
				int	x = (i << -mag) - XOrigin + (BufferXOffset >> ScaleLevel);
				int	z = (j << -mag) - ZOrigin + (BufferZOffset >> ScaleLevel);
				
				if (x >= 0 && x < XSize && z >= 0 && z < ZSize) {
					*p = TypeData[z * XSize + x];
				}
			}
		}
	} else {
		// Our data is magnified.  Bilerp between samples.
		int	steps = 1 << mag;
		j = (ZOrigin << mag) - (BufferZOffset >> ZoomLevel);
		int	z = 0;
		if (j < -1) {
			// Skip leading rows if they're outside the dest buffer.
			z = (-1 - j) >> mag;
			j += z << mag;
		}
		for ( ; z < ZSize-1; z++) {
			i = (XOrigin << mag) - (BufferXOffset >> ZoomLevel);
			int	x = 0;
			if (i < -1) {
				// Skip leading columns if they're outside the dest buffer.
				x = (-1 - i) >> mag;
				i += x << mag;
			}
			for ( ; x < XSize-1; x++) {
				// We're going to step down the vertical edges of the block.
				int	sl = TypeData[x + z * XSize] << 16;
				int	sr = TypeData[1 + x + z * XSize] << 16;
				// Steps.
				int	dl = ((TypeData[x + (z+1) * XSize] << 16) - sl) >> mag;
				int	dr = ((TypeData[1 + x + (z + 1) * XSize] << 16) - sr) >> mag;

				for (int k = 0; k < steps; k++) {
					// Lerp across.
					int	s = sl;
					int	ds = (sr - sl) >> mag;
					for (int l = 0; l < steps; l++) {
						SetType(Buffer, BufferXSize, BufferZSize, Pitch, i + l, j + k, (s >> 16) & 255);
						s += ds;
					}
					sl += dl;
					sr += dr;
				}

				i += steps;
				if (i >= BufferXSize) break;
			}

			j += steps;
			if (j >= BufferZSize) break;
		}
	}
}


void	Grid::SetXSize(int NewXSize)
// Changes the x-size of the grid.  Re-allocates and copies the
// grid data.
{
	Resize(NewXSize, GetZSize());
}


void	Grid::SetZSize(int NewZSize)
// Changes the z-size of the grid.  Re-allocates and copies the 
// grid data.
{
	Resize(GetXSize(), NewZSize);
}


void	Grid::SetScale(int NewScale)
// Sets the grid scale, in powers of two.
// Range is limited to [0, 9].
{
	if (NewScale < 0) NewScale = 0;
	if (NewScale > 9) NewScale = 9;

	// Adjust XOrigin and ZOrigin to maintain approximate position in world-coordinates (since
	// XOrigin & ZOrigin are scaled by the scale level).
	int	shift = ScaleLevel - NewScale;
	if (shift < 0) {
		XOrigin >>= -shift;
		ZOrigin >>= -shift;
	} else {
		XOrigin <<= shift;
		ZOrigin <<= shift;
	}

	ScaleLevel = NewScale;
}


void Grid::Resize(int NewXSize, int NewZSize)
// Resizes the grid.  Copies data from the existing grid; fills any
// newly created regions with zeros.
{
	if (NewXSize > MAX_GRID_SIZE) NewXSize = MAX_GRID_SIZE;
	if (NewZSize > MAX_GRID_SIZE) NewZSize = MAX_GRID_SIZE;

	if (NewXSize == GetXSize() && NewZSize == GetZSize()) return;

//	float*	NewData = new float[NewXSize * NewZSize];
	unsigned char*	NewTypeData = new unsigned char[NewXSize * NewZSize];

	// Copy the new data, and insert zeros if the grid enlarged.
	int	xlimit = XSize;
	if (xlimit > NewXSize) xlimit = NewXSize;
	int	zlimit = ZSize;
	if (zlimit > NewZSize) zlimit = NewZSize;

	int	x, z;
	for (z = 0; z < zlimit; z++) {
		// Copy the existing data.
		for (x = 0; x < xlimit; x++) {
//			NewData[x + z * NewXSize] = Data[x + z * XSize];
			NewTypeData[x + z * NewXSize] = TypeData[x + z * XSize];
		}
		// Fill in zeros at the end of the row, if we need to.
		for ( ; x < NewXSize; x++) {
//			NewData[x + z * NewXSize] = 0;
			NewTypeData[x + z * NewXSize] = 0;
		}
	}
	// Fill in any new rows with zeros.
	for ( ; z < NewZSize; z++) {
		for (x = 0; x < NewXSize; x++) {
//			NewData[x + z * NewXSize] = 0;
			NewTypeData[x + z * NewXSize] = 0;
		}
	}

	XSize = NewXSize;
	ZSize = NewZSize;
//	delete [] Data;
//	Data = NewData;
	delete [] TypeData;
	TypeData = NewTypeData;
}


#ifdef NOT


void	Grid::ScaleData(float Factor)
// Scales the grid's data by the given factor.
{
	int	DataCount = GetXSize() * GetZSize();

	for (int i = 0; i < DataCount; i++) {
		Data[i] *= Factor;
	}
}


float	Grid::GetHeight(float x, float z)
// Returns the height contributed by this grid, at the given x,z coordinates.
{
	x -= XOrigin << ScaleLevel;
	z -= ZOrigin << ScaleLevel;

	float	ScaleFactor = 1.0 / (1 << ScaleLevel);
	x *= ScaleFactor;
	z *= ScaleFactor;

	if (x < 0 || x >= XSize || z < 0 || z >= ZSize) return 0;

	int	ix = int(x);
	int	iz = int(z);
	
	x -= ix;
	z -= iz;

	// jbl 08/20/98:  Watch out for Data out of bounds for this grid,
	// which does happen in the +1 expressions at the grid edges.
	//return Data[ix + iz * XSize] * (1 - x) * (1 - z) +
	//	Data[ix + 1 + iz * XSize] * (x) * (1 - z) +
	//	Data[ix + (iz + 1) * XSize] * (1 - x) * (z) +
	//	Data[ix + 1 + (iz + 1) * XSize] * (x) * (z);

	int	Values = XSize * ZSize;
	float	Height = 0;
	int	i;

	i = ix + iz * XSize;
	if (i < Values) Height += Data[i] * (1 - x) * (1 - z);
	i = ix + 1 + iz * XSize;
	if (i < Values) Height += Data[i] * (x) * (1 - z);
	i = ix + (iz + 1) * XSize;
	if (i < Values) Height += Data[i] * (1 - x) * (z);
	i = ix + 1 + (iz + 1) * XSize;
	if (i < Values) Height += Data[i] * (x) * (z);

	return(Height);
}


#endif // NOT


int Grid::Compare(const Grid & g)
// Defines a sorting order on grids.  If *this is "less" than the given g returns
// -1, if the grids are "equal" returns 0, and if *this is "greater" than g returns 1.
// The ordering uses the negative of the scale level as the most significant key,
// followed by the x origin, followed by the z origin.
{
	if (ScaleLevel < g.ScaleLevel) return 1;
	else if (ScaleLevel > g.ScaleLevel) return -1;
	else if (XOrigin < g.XOrigin) return -1;
	else if (XOrigin > g.XOrigin) return 1;
	else if (ZOrigin < g.ZOrigin) return -1;
	else if (ZOrigin > g.ZOrigin) return 1;
	else return 0;
}
