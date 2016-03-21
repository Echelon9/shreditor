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
// terrain.h : main header file for the TERRAIN application
//

#if !defined(AFX_TERRAIN_H__DED47AC4_CF2D_11D1_83C5_98E1A892656D__INCLUDED_)
#define AFX_TERRAIN_H__DED47AC4_CF2D_11D1_83C5_98E1A892656D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTerrainApp:
// See terrain.cpp for the implementation of this class
//

class CTerrainApp : public CWinApp
{
public:
	CTerrainApp();

	CMultiDocTemplate*	GridDocTemplate;
	CMultiDocTemplate* ObjectViewTemplate;
	CMultiDocTemplate* TerrainViewTemplate;
	CMultiDocTemplate* GeometryListViewTemplate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTerrainApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTerrainApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
};


extern CTerrainApp theApp;


void	DrawHeightField(CDC* pDC, int x0, int z0, int BlockPixels, float* DataGrid, unsigned char* TypeDataGrid, int xsize, int zsize, int GridPitch, float ContourInterval);
void	DrawBox(CDC* dc, int x, int y, int xsize, int ysize, COLORREF color);
COLORREF	TypeColor(unsigned char t);
const char*	TypeName(unsigned char t);


//int	GetObjectTypeCount();
//const char*	GetObjectTypeString(int index);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAIN_H__DED47AC4_CF2D_11D1_83C5_98E1A892656D__INCLUDED_)
