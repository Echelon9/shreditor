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
#if !defined(AFX_TOOLSHEET_H__CC5FE942_A265_11D2_8C89_00104BC7527F__INCLUDED_)
#define AFX_TOOLSHEET_H__CC5FE942_A265_11D2_8C89_00104BC7527F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolSheet.h : header file
//


#include "GridFormView.h"
#include "ObjectFormView.h"
#include "PolygonFormView.h"
#include "SolidListView.h"
#include "PaintParametersView.h"


/////////////////////////////////////////////////////////////////////////////
// ToolSheet

class ToolSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(ToolSheet)

// Construction
public:
	ToolSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ToolSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	GridFormView	GridPage;
	GeometryListView	GeoListView;
	ObjectFormView	ObjectPage;
	PolygonFormView	PolygonPage;
	SolidListView	SolidListPage;
	PaintParametersView	PaintPage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ToolSheet)
	public:
	protected:
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ToolSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(ToolSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void	Init();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLSHEET_H__CC5FE942_A265_11D2_8C89_00104BC7527F__INCLUDED_)
