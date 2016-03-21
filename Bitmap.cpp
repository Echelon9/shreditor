// bitmap.cpp	-thatcher 8/19/1998 Copyright Slingshot
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
	
// Some code to read Photoshop 2.5 .PSD files.


#include "stdafx.h"

#include <stdio.h>
#include "bitmap.h"


namespace Bitmap {
;


int	Read16(FILE* fp)
// Reads a two-byte big-endian integer from the given file and returns its value.
// Assumes unsigned.
{
	int	hi = fgetc(fp);
	int	lo = fgetc(fp);
	return lo + (hi << 8);
}


uint32	Read32(FILE* fp)
// Reads a four-byte big-endian integer from the given file and returns its value.
// Assumes unsigned.
{
	uint32	b3 = fgetc(fp);
	uint32	b2 = fgetc(fp);
	uint32	b1 = fgetc(fp);
	uint32	b0 = fgetc(fp);
	return (b3 << 24) + (b2 << 16) + (b1 << 8) + b0;
}


int	Write16(FILE* fp, unsigned short data)
// Writes the data as a two-byte big-endian integer to the given file.
// Returns count of bytes written.
{
	fputc(data >> 8, fp);
	fputc(data & 0x0FF, fp);
	return 2;
}


int	Write32(FILE* fp, unsigned long data)
// Writes the data as a four-byte big-endian integer to the given file.
// Returns count of bytes written.
{
	fputc((data >> 24) & 0x0FF, fp);
	fputc((data >> 16) & 0x0FF, fp);
	fputc((data >> 8) & 0x0FF, fp);
	fputc((data >> 0) & 0x0FF, fp);
	return 4;
}


#ifdef NOT


void	ScanForResolution(float* HRes, float* VRes, FILE* fp, int ByteCount)
// Scans through the next ByteCount bytes of the file, looking for an
// image resource block encoding the image's resolution.  Returns the resolution(s),
// if found, in the pointed-to floats.  Units are in pixels/meter.
{
	while (ByteCount) {
		// Read the image resource block header.
		uint32	Header = Read32(fp);
		if (Header != 0x3842494D /* "8BIM" */) {
			// Format problem.
			Error e; e << "PSDRead: image resource block has unknown signature.";
			throw e;
		}
		int	ID = Read16(fp);

		// Skip the name.
		int	NameLength = fgetc(fp) | 1;	// NameLength must be odd, so that total including size byte is even.
		fseek(fp, NameLength, SEEK_CUR);

		// Get the size of the data block.
		int	DataSize = Read32(fp);
		if (DataSize & 1) {
			DataSize += 1;	// Block size must be even.
		}

		// Account for header size.
		ByteCount -= 11 + NameLength;

		// If this block is a ResolutionInfo structure, then get the resolution.
		if (ID == 0x03ED) {
			// Read ResolutionInfo.
			int	HResFixed = Read32(fp);
			int	junk = Read16(fp);	// display units of hres.
			junk = Read16(fp);	// display units of width.
			int	VResFixed = Read32(fp);
			junk = Read16(fp);	// display units of vres.
			junk = Read16(fp);	// display units of height.

			ByteCount -= DataSize;
			DataSize -= 16;
			// Skip any extra bytes at the end of this block...
			if (DataSize > 0) {
				fseek(fp, DataSize, SEEK_CUR);
			}

			// Need to convert resolution figures from fixed point, pixels/inch to floating point,
			// pixels/meter.
			static const float	InchesPerMeter = 39.4;
			*HRes = HResFixed * (InchesPerMeter / 65536.0);
			*VRes = VResFixed * (InchesPerMeter / 65536.0);
			
		} else {
			// Skip the rest of this block.
			fseek(fp, DataSize, SEEK_CUR);
			ByteCount -= DataSize;
		}
	}
}


bitmap32*	ReadImageData32(const char* filename, float* WidthPtr, float* HeightPtr)
// Reads the image from the specified filename, which must be in Photoshop .PSD format,
// and creates a new 32-bpp image containing the image data.  Creates a new bitmap to
// contain the image, and returns a pointer to the new bitmap.
//
// The image data is in RGBA format, reading by byte with addresses increasing left to right,
// which is sort of a big-endian format.  It's what OpenGL wants.
//
// !!! The caller is responsible for freeing the bitmap.
// Returns NULL if the file can't be read or the image can't be created.
// If the given Width and/or Height parameters are not NULL, then stores the
// width and height of the image in the referenced locations, as encoded in the .PSD
// file but converted so that 1 cm in Photoshop == 1 meter here.
{
	// Open the data file for input.
	FILE*	fp = fopen(filename, "rb");
	if (fp == NULL) {
		return NULL;
	}

	// Check file type signature.
	uint32	sig = Read32(fp);
	if (sig != 0x38425053 /* "8BPS" */) {
		// Not .PSD format.
		fclose(fp);
		return NULL;
	}

	// Check file type version.
	if (Read16(fp) != 1) {
		fclose(fp);
		return NULL;
	}

	// 6 reserved bytes.
	Read32(fp);
	Read16(fp);

	// Read the number of channels (R, G, B, A, etc).
	int	ChannelCount = Read16(fp);
	if (ChannelCount < 0 || ChannelCount > 16) {
		fclose(fp);
		return NULL;
	}

	// Read the rows and columns of the image.
	int	height = Read32(fp);
	int	width = Read32(fp);

	// Make sure the depth is 8 bits. xxxx 16 bits?
	if (Read16(fp) != 8) {
		fclose(fp);
		return NULL;
	}

	// Make sure the color mode is grayscale.
	// Valid options are:
	//   0: Bitmap
	//   1: Grayscale
	//   2: Indexed color
	//   3: RGB color
	//   4: CMYK color
	//   7: Multichannel
	//   8: Duotone
	//   9: Lab color
	if (Read16(fp) != 1) {
		fclose(fp);
		return NULL;
	}

	// Skip the Mode Data.  (It's the palette for indexed color; other info for other modes.)
	int	ModeDataCount = Read32(fp);
	if (ModeDataCount) {
		fseek(fp, ModeDataCount, SEEK_CUR);
	}

	// Process the image resources.  (resolution, pen tool paths, etc)
	int	ResourceDataCount = Read32(fp);
	if (ResourceDataCount) {
		// Read the image resources, looking for the info in the resolution block.
		float	HRes = 1.0;
		float	VRes = 1.0;
		ScanForResolution(&HRes, &VRes, fp, ResourceDataCount);

		// Set returns values for bitmap height/width.
		if (WidthPtr) {
			*WidthPtr = width / HRes * 100.0;	// Compute size in m, then multiply by 100 since we want 1 Photoshop cm == 1 Soul Ride meter.
		}
		if (HeightPtr) {
			// Base the height on the greatest power-of-two
			// that fits in the pixel height.  This is to
			// compensate for additional mip-map data that
			// may be tacked onto the bottom of the image.
			int	HeightBits = height;
			int	i = height;
			for (;;) {
				i >>= 1;
				if (i == 0) break;
				HeightBits++;
			}
			
			*HeightPtr = (1 << HeightBits) / VRes * 100.0;
		}
		
//		fseek(fp, ResourceDataCount, SEEK_CUR);
	}

//	// Skip the image resources.  (resolution, pen tool paths, etc)
//	int	ResourceDataCount = Read32(fp);
//	if (ResourceDataCount) {
//		fseek(fp, ResourceDataCount, SEEK_CUR);
//	}

	// Skip the reserved data.
	int	ReservedDataCount = Read32(fp);
	if (ReservedDataCount) {
		fseek(fp, ReservedDataCount, SEEK_CUR);
	}

	// Find out if the data is compressed.
	// Known values:
	//   0: no compression
	//   1: RLE compressed
	int	Compression = Read16(fp);
	if (Compression > 1) {
		// Unknown compression type.
		fclose(fp);
		return NULL;
	}

//	// Some formatting information about the channels.
//	const struct ChannelInfo {
//		int	Shift, Mask, Default;
//	} Channel[4] = {
//		{ 16, 0x00FF0000, 0 },	// Red.
//		{  8, 0x0000FF00, 0 },	// Green.
//		{  0, 0x000000FF, 0 },	// Blue.
//		{ 24, 0xFF000000, 255 }	// Alpha.
//	};
	// Some formatting information about the channels.
	const struct ChannelInfo {
		int	Shift, Mask, Default;
	} Channel[4] = {
		{  0, 0x000000FF, 0 },	// Red.
		{  8, 0x0000FF00, 0 },	// Green.
		{ 16, 0x00FF0000, 0 },	// Blue.
		{ 24, 0xFF000000, 255 }	// Alpha.
	};

	// Create the destination bitmap.
	bitmap32*	b = new bitmap32(width, height);
	int	PixelCount = height * width;

	// Initialize the data to zero.
	uint32*	p = b->GetData();
	for (int i = 0; i < PixelCount; i++) {
		*p++ = 0;
	}
	
	// Finally, the image data.
	if (Compression) {
		// The RLE-compressed data is preceeded by a 2-byte data count for each row in the data,
		// which we're going to just skip.
		fseek(fp, height * ChannelCount * 2, SEEK_CUR);

		// Read the RLE data by channel.
		for (int channel = 0; channel < 4; channel++) {
			const ChannelInfo&	c = Channel[channel];
			
			uint32*	p = b->GetData();
			if (channel >= ChannelCount) {
				// Fill this channel with default data.
				uint32	def = (c.Default << c.Shift) & c.Mask;
				for (int i = 0; i < PixelCount; i++) {
					*p++ |= def;
				}
			} else {
				// Read the RLE data.
				int	count = 0;
				while (count < PixelCount) {
					int	len = fgetc(fp);
					if (len == 128) {
						// No-op.
					} else if (len < 128) {
						// Copy next len+1 bytes literally.
						len++;
						count += len;
						while (len) {
							*p++ |= ((fgetc(fp) << c.Shift) & c.Mask);
							len--;
						}
					} else if (len > 128) {
						// Next -len+1 bytes in the dest are replicated from next source byte.
						// (Interpret len as a negative 8-bit int.)
						len ^= 0x0FF;
						len += 2;
						uint32	val = (fgetc(fp) << c.Shift) & c.Mask;
						count += len;
						while (len) {
							*p++ |= val;
							len--;
						}
					}
				}
			}
		}
		
	} else {
		// We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
		// where each channel consists of an 8-bit value for each pixel in the image.
		
		// Read the data by channel.
		for (int channel = 0; channel < 4; channel++) {
			const ChannelInfo&	c = Channel[channel];
			
			uint32*	p = b->GetData();
			if (channel > ChannelCount) {
				// Fill this channel with default data.
				uint32	def = (c.Default << c.Shift) & c.Mask;
				for (int i = 0; i < PixelCount; i++) {
					*p++ |= def;
				}
			} else {
				// Read the data.
				int	count = 0;
				for (int i = 0; i < PixelCount; i++) {
					*p++ |= (fgetc(fp) << c.Shift) & c.Mask;
				}
			}
		}
	}
	
	fclose(fp);

	return b;
}


void	WriteImageData32(const char* filename, uint32* RGBAData, int Width, int Height, bool IncludeAlpha)
// Writes a .PSD file containing the given image data.  The data is assumed to
// start with the bottom row, and the red byte is assumed to come first.  (i.e.
// OpenGL's default format).
{
	int	channel, ChannelCount;
	
	// Open the file.
	FILE*	stream = fopen(filename, "wb");
	if (stream == NULL) {
		return;	// Could throw an exception or something.
	}
	
	// Write .PSD signature.
	int res = Write32(stream, 0x38425053);
	if (res != 4) goto io_error;

	// Write file version.
	res = Write16(stream, 1);
	if (res != 2) goto io_error;

	// Write 6 reserved bytes.
	Write32(stream, 0);
	Write16(stream, 0);

	// Write the number of channels: 4 (R, G, B, A) or 3 (R, G, B).
	if (IncludeAlpha) {
		Write16(stream, 4);
	} else {
		Write16(stream, 3);
	}

	// Write the row and column counts.
	Write32(stream, Height);
	Write32(stream, Width);

	// Write the bit depth (8).
	Write16(stream, 8);

	// Write the color mode (RGB).
	Write16(stream, 3);

	// Write the mode data count (0).
	Write32(stream, 0);

	// Write the image resource data count (0).
	Write32(stream, 0);

	// Write the reserved data count (0).
	Write32(stream, 0);

	// Write the compression flag (0 --> not compressed).
	Write16(stream, 0);
	
	// Write image data.

	ChannelCount = IncludeAlpha ? 4 : 3;
	
	for (channel = 0; channel < ChannelCount; channel++) {
		for (int h = 0; h < Height; h++) {
			// Get a row of data.
			uint8*	row = (uint8*) (RGBAData + Width * (Height - 1 - h));
			
			uint8*	p = row + channel;
			int	step = 4;

			// Write out the data.
			for (int w = 0; w < Width; w++) {
				fputc(*p, stream);
				p += step;
			}
		}
	}	

	fclose(stream);
	
	return;

io_error:
	fclose(stream);
	// throw an exception, log an error, or something?
	return;
}


#endif // NOT


bitmap8*	ReadImageData8(const char* filename)
// Reads the image from the specified filename, which must be in Photoshop .PSD format,
// and creates a new 8-bpp image containing the grayscale image data.  Creates a new bitmap to
// contain the image, and returns a pointer to the new bitmap.
//
// !!! The caller is responsible for freeing the bitmap.
// Returns NULL if the file can't be read or the image can't be created.
{
	// Open the data file for input.
	FILE*	fp = fopen(filename, "rb");
	if (fp == NULL) {
		return NULL;
	}

	// Check file type signature.
	uint32	sig = Read32(fp);
	if (sig != 0x38425053 /* "8BPS" */) {
		// Not .PSD format.
		fclose(fp);
		return NULL;
	}

	// Check file type version.
	if (Read16(fp) != 1) {
		fclose(fp);
		return NULL;
	}

	// 6 reserved bytes.
	Read32(fp);
	Read16(fp);

	// Read the number of channels (R, G, B, A, etc).
	int	ChannelCount = Read16(fp);
	if (ChannelCount < 0 || ChannelCount > 16) {
		fclose(fp);
		return NULL;
	}

	// Read the rows and columns of the image.
	int	height = Read32(fp);
	int	width = Read32(fp);

	// Make sure the depth is 8 bits.
	if (Read16(fp) != 8) {
		fclose(fp);
		return NULL;
	}

	// Make sure the color mode is RGB.
	// Valid options are:
	//   0: Bitmap
	//   1: Grayscale
	//   2: Indexed color
	//   3: RGB color
	//   4: CMYK color
	//   7: Multichannel
	//   8: Duotone
	//   9: Lab color
	if (Read16(fp) != 1) {
		fclose(fp);
		return NULL;
	}

	// Skip the Mode Data.  (It's the palette for indexed color; other info for other modes.)
	int	ModeDataCount = Read32(fp);
	if (ModeDataCount) {
		fseek(fp, ModeDataCount, SEEK_CUR);
	}

	// Process the image resources.  (resolution, pen tool paths, etc)
	int	ResourceDataCount = Read32(fp);
	if (ResourceDataCount) {
		fseek(fp, ResourceDataCount, SEEK_CUR);
	}

	// Skip the reserved data.
	int	ReservedDataCount = Read32(fp);
	if (ReservedDataCount) {
		fseek(fp, ReservedDataCount, SEEK_CUR);
	}

	// Find out if the data is compressed.
	// Known values:
	//   0: no compression
	//   1: RLE compressed
	int	Compression = Read16(fp);
	if (Compression > 1) {
		// Unknown compression type.
		fclose(fp);
		return NULL;
	}

#ifdef NOT
//	// Some formatting information about the channels.
//	const struct ChannelInfo {
//		int	Shift, Mask, Default;
//	} Channel[4] = {
//		{ 16, 0x00FF0000, 0 },	// Red.
//		{  8, 0x0000FF00, 0 },	// Green.
//		{  0, 0x000000FF, 0 },	// Blue.
//		{ 24, 0xFF000000, 255 }	// Alpha.
//	};
	// Some formatting information about the channels.
	const struct ChannelInfo {
		int	Shift, Mask, Default;
	} Channel[4] = {
		{  0, 0x000000FF, 0 },	// Red.
		{  8, 0x0000FF00, 0 },	// Green.
		{ 16, 0x00FF0000, 0 },	// Blue.
		{ 24, 0xFF000000, 255 }	// Alpha.
	};
#endif // NOT

	// Create the destination bitmap.
	bitmap8*	b = new bitmap8(width, height);
	int	PixelCount = height * width;

	// Initialize the data to zero.
	uint8*	p = b->GetData();
	for (int i = 0; i < PixelCount; i++) {
		*p++ = 0;
	}
	
	// Finally, the image data.
	if (Compression) {
		// The RLE-compressed data is preceeded by a 2-byte data count for each row in the data,
		// which we're going to just skip.
		fseek(fp, height * ChannelCount * 2, SEEK_CUR);

		// Read the RLE data by channel.
		for (int channel = 0; channel < 1; channel++) {
//			const ChannelInfo&	c = Channel[channel];
			
			uint8*	p = b->GetData();
			if (channel >= ChannelCount) {
				// Fill this channel with default data.
				uint8	def = 0;
				for (int i = 0; i < PixelCount; i++) {
					*p++ = def;
				}
			} else {
				// Read the RLE data.
				int	count = 0;
				while (count < PixelCount) {
					int	len = fgetc(fp);
					if (len == 128) {
						// No-op.
					} else if (len < 128) {
						// Copy next len+1 bytes literally.
						len++;
						count += len;
						while (len) {
							*p++ = fgetc(fp);
							len--;
						}
					} else if (len > 128) {
						// Next -len+1 bytes in the dest are replicated from next source byte.
						// (Interpret len as a negative 8-bit int.)
						len ^= 0x0FF;
						len += 2;
						uint32	val = fgetc(fp);
						count += len;
						while (len) {
							*p++ = val;
							len--;
						}
					}
				}
			}
		}
		
	} else {
		// We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
		// where each channel consists of an 8-bit value for each pixel in the image.
		
		// Read the data by channel.
		for (int channel = 0; channel < 1; channel++) {
//			const ChannelInfo&	c = Channel[channel];
			
			uint8*	p = b->GetData();
			if (channel > ChannelCount) {
				// Fill this channel with default data.
				uint8	def = 0;
				for (int i = 0; i < PixelCount; i++) {
					*p++ = def;
				}
			} else {
				// Read the data.
				int	count = 0;
				for (int i = 0; i < PixelCount; i++) {
					*p++ = fgetc(fp);
				}
			}
		}
	}
	
	fclose(fp);

	return b;
}


void	WriteImageData8(const char* filename, uint8* GrayData, int Width, int Height)
// Writes a .PSD file containing the given image data.  The data is assumed to
// start with the bottom row.
{
	int	channel, ChannelCount;
	
	// Open the file.
	FILE*	stream = fopen(filename, "wb");
	if (stream == NULL) {
		return;	// Could throw an exception or something.
	}
	
	// Write .PSD signature.
	int res = Write32(stream, 0x38425053);
	if (res != 4) goto io_error;

	// Write file version.
	res = Write16(stream, 1);
	if (res != 2) goto io_error;

	// Write 6 reserved bytes.
	Write32(stream, 0);
	Write16(stream, 0);

//	// Write the number of channels: 4 (R, G, B, A) or 3 (R, G, B).
//	if (IncludeAlpha) {
//		Write16(stream, 4);
//	} else {
//		Write16(stream, 3);
//	}
	Write16(stream, 1);

	// Write the row and column counts.
	Write32(stream, Height);
	Write32(stream, Width);

	// Write the bit depth (8).
	Write16(stream, 8);

//	// Write the color mode (RGB).
//	Write16(stream, 3);
	Write16(stream, 1);

	// Write the mode data count (0).
	Write32(stream, 0);

	// Write the image resource data count (0).
	Write32(stream, 0);

	// Write the reserved data count (0).
	Write32(stream, 0);

	// Write the compression flag (0 --> not compressed).
	Write16(stream, 0);
	
	// Write image data.

//	ChannelCount = IncludeAlpha ? 4 : 3;
	ChannelCount = 1;
	
	for (channel = 0; channel < ChannelCount; channel++) {
		for (int h = 0; h < Height; h++) {
			// Get a row of data.
			uint8*	row = (uint8*) (GrayData + Width * h);
			
			uint8*	p = row + channel;
			int	step = 1 /*4*/;

			// Write out the data.
			for (int w = 0; w < Width; w++) {
				fputc(*p, stream);
				p += step;
			}
		}
	}	

	fclose(stream);
	
	return;

io_error:
	fclose(stream);
	// throw an exception, log an error, or something?
	return;
}


// 16-color palette representing our surface types.
static const unsigned char	palette[16][3] = {
	{ 220, 250, 215 },
	{ 160, 160, 210 },
	{ 190, 190, 215 },
	{ 225, 225, 225 },
	
	{ 255, 255, 255 },
	{ 170, 170, 170 },
	{ 234, 234, 150 },
	{ 100, 100, 100 },
	
	{  80, 100, 255 },
	{ 50, 50, 50 },
	{ 50, 50, 50 },
	{ 50, 50, 50 },
	
	{ 50, 50, 50 },
	{ 50, 50, 50 },
	{ 50, 50, 50 },
	{ 50, 50, 50 },
};


uint8	IndexMap[256];	// maps color indices to surface types.


int	iabs(int i) { if (i < 0) return -i; else return i; }


uint8	FindBestType(uint8 r, uint8 g, uint8 b)
// Finds the best surface-type match to the specified color.
{
	int	i;
	int	best = 0;
	int	diff = 10000;

	for (i = 0; i < 16; i++) {
		int	d = iabs(r - palette[i][0]) + iabs(g - palette[i][1]) + iabs(b - palette[i][2]);
		if (d < diff) {
			diff = d;
			best = i;
		}
	}

	return best;
}

	
void	ReadTypeMapFromImage32(const char* filename, uint8* TypeData, int Width, int Height)
// Reads the image from the specified filename, which must be in Photoshop .PSD format,
// and converts the data to type indexes and puts it in the TypeData[][] array.
{
	// Open the data file for input.
	FILE*	fp = fopen(filename, "rb");
	if (fp == NULL) {
		return;
	}

	// Check file type signature.
	uint32	sig = Read32(fp);
	if (sig != 0x38425053 /* "8BPS" */) {
		// Not .PSD format.
		fclose(fp);
		return;
	}

	// Check file type version.
	if (Read16(fp) != 1) {
		fclose(fp);
		return;
	}

	// 6 reserved bytes.
	Read32(fp);
	Read16(fp);

	// Read the number of channels (R, G, B, A, etc).
	int	ChannelCount = Read16(fp);
	if (ChannelCount < 3 || ChannelCount > 16) {
		fclose(fp);
		return;
	}

	// Read the rows and columns of the image.
	int	height = Read32(fp);
	int	width = Read32(fp);

	// Make sure the depth is 8 bits.
	if (Read16(fp) != 8) {
		fclose(fp);
		return;
	}

	// Make sure the color mode is indexed color.
	// Valid options are:
	//   0: Bitmap
	//   1: Grayscale
	//   2: Indexed color
	//   3: RGB color
	//   4: CMYK color
	//   7: Multichannel
	//   8: Duotone
	//   9: Lab color
	if (Read16(fp) != 3) {
		fclose(fp);
		return;
	}

	// Skip palette data.
	int	ModeDataCount = Read32(fp);
	if (ModeDataCount) {
		// Skip data; use default mapping.
		fseek(fp, ModeDataCount, SEEK_CUR);
	}

	// Skip image resources.  (resolution, pen tool paths, etc)
	int	ResourceDataCount = Read32(fp);
	if (ResourceDataCount) {
		fseek(fp, ResourceDataCount, SEEK_CUR);
	}

	// Skip the reserved data.
	int	ReservedDataCount = Read32(fp);
	if (ReservedDataCount) {
		fseek(fp, ReservedDataCount, SEEK_CUR);
	}

	// Find out if the data is compressed.
	// Known values:
	//   0: no compression
	//   1: RLE compressed
	int	Compression = Read16(fp);
	if (Compression > 1) {
		// Unknown compression type.
		fclose(fp);
		return;
	}

	if (height != Height || width != Width) {
		// Size mismatch...
		fclose(fp);
		return;
	}
	
	int	PixelCount = height * width;

	// Allocate a temporary buffer to hold RGB color info.
	uint8*	buf = new uint8[PixelCount * 3];
	
//	// Initialize the data to zero.
//	uint8*	p = b->GetData();
//	for (int i = 0; i < PixelCount; i++) {
//		*p++ = 0;
//	}
	
	// Finally, the image data.
	if (Compression) {
		// The RLE-compressed data is preceeded by a 2-byte data count for each row in the data,
		// which we're going to just skip.
		fseek(fp, height * ChannelCount * 2, SEEK_CUR);

		// Read the RLE data by channel.
		for (int channel = 0; channel < 3; channel++) {
			uint8*	p = buf + channel;
			if (channel >= ChannelCount) {
				// Fill this channel with default data.
				uint8	def = 0;
				for (int i = 0; i < PixelCount; i++) {
					*p = def;
					p += 3;
				}
			} else {
				// Read the RLE data.
				int	count = 0;
				while (count < PixelCount) {
					int	len = fgetc(fp);
					if (len == 128) {
						// No-op.
					} else if (len < 128) {
						// Copy next len+1 bytes literally.
						len++;
						count += len;
						while (len) {
							*p = fgetc(fp);
							p += 3;
							len--;
						}
					} else if (len > 128) {
						// Next -len+1 bytes in the dest are replicated from next source byte.
						// (Interpret len as a negative 8-bit int.)
						len ^= 0x0FF;
						len += 2;
						uint32	val = fgetc(fp);
						count += len;
						while (len) {
							*p = val;
							p += 3;
							len--;
						}
					}
				}
			}
		}
		
	} else {
		// We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
		// where each channel consists of an 8-bit value for each pixel in the image.
		
		// Read the data by channel.
		for (int channel = 0; channel < 3; channel++) {
//			const ChannelInfo&	c = Channel[channel];
			
			uint8*	p = buf + channel;
			if (channel > ChannelCount) {
				// Fill this channel with default data.
				uint8	def = 0;
				for (int i = 0; i < PixelCount; i++) {
					*p = def;
					p += 3;
				}
			} else {
				// Read the data.
				int	count = 0;
				for (int i = 0; i < PixelCount; i++) {
					*p = fgetc(fp);
					p += 3;
				}
			}
		}
	}
	
	fclose(fp);

	// Convert the color data we just loaded into type data...
	uint8*	src = buf;
	uint8*	p = TypeData;
	int	i;
	for (i = 0; i < PixelCount; i++) {
		*p++ = FindBestType(src[0], src[1], src[2]);
		src += 3;
	}

	// Release temp array.
	delete buf;
}


void	WriteImage32FromTypeMap(const char* filename, uint8* IndexData, int Width, int Height)
// Writes a .PSD file containing the given image data.  The data is assumed to
// start with the bottom row.
{
	int	channel, ChannelCount;
	
	// Open the file.
	FILE*	stream = fopen(filename, "wb");
	if (stream == NULL) {
		return;	// Could throw an exception or something.
	}
	
	// Write .PSD signature.
	int res = Write32(stream, 0x38425053);
	if (res != 4) goto io_error;

	// Write file version.
	res = Write16(stream, 1);
	if (res != 2) goto io_error;

	// Write 6 reserved bytes.
	Write32(stream, 0);
	Write16(stream, 0);

	// Write the number of channels: 4 (R, G, B, A) or 3 (R, G, B).
	Write16(stream, 3);

	// Write the row and column counts.
	Write32(stream, Height);
	Write32(stream, Width);

	// Write the bit depth (8).
	Write16(stream, 8);

	// Write the color mode (RGB).
	Write16(stream, 3);

	// Write empty mode data chunk.
	Write32(stream, 0);

	// Write the image resource data count (0).
	Write32(stream, 0);

	// Write the reserved data count (0).
	Write32(stream, 0);

	// Write the compression flag (0 --> not compressed).
	Write16(stream, 0);
	
	// Write image data.

//	ChannelCount = IncludeAlpha ? 4 : 3;
	ChannelCount = 3;
	
	for (channel = 0; channel < ChannelCount; channel++) {
		for (int h = 0; h < Height; h++) {
			// Get a row of data.
			uint8*	row = (uint8*) (IndexData + Width * (h));
			
			uint8*	p = row;
			int	step = 1;

			// Write out the data.
			for (int w = 0; w < Width; w++) {
				fputc(palette[*p][channel], stream);
				p += step;
			}
		}
	}	

	fclose(stream);
	
	return;

io_error:
	fclose(stream);
	// throw an exception, log an error, or something?
	return;
}


}; // end namespace Bitmap

