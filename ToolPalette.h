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
#if !defined(AFX_TOOLPALETTE_H__FE2499E0_FAF8_11D1_83C5_444553540000__INCLUDED_)
#define AFX_TOOLPALETTE_H__FE2499E0_FAF8_11D1_83C5_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolPalette.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ToolPalette form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "terraindoc.h"
#include "ToolSheet.h"


class ToolPalette : public CFormView
{
protected:
	ToolPalette();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ToolPalette)

// Form Data
public:
	//{{AFX_DATA(ToolPalette)
	enum { IDD = IDD_TOOL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

	struct	PlantParameters {
		bool	EnableSpray;
		float	Radius;
		int	Count;
		float	ErasePercent;

		int	ObjectType;
		int	ObjectGeometry;
		int	ObjectSolid;
		float	ObjectY;
		bool	PinToGround;

		PlantParameters() {
			EnableSpray = false;
			Radius = 1.0;
			Count = 1;
			ErasePercent = 30;
			ObjectType = 1;
			ObjectGeometry = 1;
			ObjectSolid = 1;
			ObjectY = 0;
			PinToGround = true;
		}
	};

// Operations
public:
	CTerrainDoc* GetDocument();

	enum ToolType {
		GRIDTOOL = 0,
		OBJECTTOOL,
		PLANTOBJECTS,
		EXCAVATE,
		PAINTSURFACE,
		VERTEXTOOL,
		HAND
	};
	ToolType	GetCurrentTool();

	float	GetContourInterval();
	int	GetZoomLevel();
	void	SetZoomLevel(int NewLevel);

	bool	GetDrawSelectedGrid();
	bool	GetDrawAllGrids();
	bool	GetDrawObjects();
	bool	GetDrawSurface();

	bool	GetShowCursor();
	
	const PlantParameters&	GetPlantParameters() { return PlantParams; }
	void	SetPlantParameters(const PlantParameters& p) { PlantParams = p; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ToolPalette)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~ToolPalette();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ToolPalette)
	afx_msg void OnToolSelect();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDrawAllGrids();
	afx_msg void OnDrawObjects();
	afx_msg void OnDrawSelectedGrid();
	afx_msg void OnDrawSurface();
	afx_msg void OnCleanupQuadTree();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateSliderNumbers();

	PlantParameters	PlantParams;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLPALETTE_H__FE2499E0_FAF8_11D1_83C5_444553540000__INCLUDED_)
