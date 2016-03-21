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
#if !defined(AFX_GEOMETRYLISTVIEW_H__62A892C0_8066_11D2_8C89_00104BC7527F__INCLUDED_)
#define AFX_GEOMETRYLISTVIEW_H__62A892C0_8066_11D2_8C89_00104BC7527F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GeometryListView.h : header file
//

class CTerrainDoc;
class CChildFrame;


/////////////////////////////////////////////////////////////////////////////
// GeometryListView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class GeometryListView : public CPropertyPage
{
//protected:
public:
	GeometryListView();           // protected constructor used by dynamic creation
	virtual ~GeometryListView();
//	DECLARE_DYNCREATE(GeometryListView)

// Form Data
public:
	//{{AFX_DATA(GeometryListView)
	enum { IDD = IDD_GEOMETRY_DIALOG };
	CButton	m_AddNewGeometry;
	CListCtrl	m_ListControl;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void	Update();
	CTerrainDoc* GetDocument();
	CChildFrame*	GetFrame() { return Frame; }
	void	SetFrame(CChildFrame* f) { Frame = f; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GeometryListView)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(GeometryListView)
	afx_msg void OnAddNewGeometry();
	afx_msg void OnEditGeometry();
	afx_msg void OnDeleteGeometry();
	virtual BOOL OnInitDialog();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int GetSelectedItemIndex();

	CChildFrame*	Frame;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GEOMETRYLISTVIEW_H__62A892C0_8066_11D2_8C89_00104BC7527F__INCLUDED_)
