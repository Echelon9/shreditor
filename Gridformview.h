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
#if !defined(AFX_GRIDFORMVIEW_H__AF90A4E2_E13A_11D1_83C5_AABBB32D6B5D__INCLUDED_)
#define AFX_GRIDFORMVIEW_H__AF90A4E2_E13A_11D1_83C5_AABBB32D6B5D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GridFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GridFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "terraindoc.h"
#include "ChildFrm.h"


class	CChildFrame;


class GridFormView : public CPropertyPage
{
protected:
public://xxxx
	GridFormView();           // protected constructor used by dynamic creation
	virtual ~GridFormView();
//xxxx	DECLARE_DYNCREATE(GridFormView)

// Form Data
public:
	//{{AFX_DATA(GridFormView)
	enum { IDD = IDD_FORMVIEW };
	CSliderCtrl	m_GridScale;
	int		m_GridXSize;
	int		m_GridXOrigin;
	int		m_GridZOrigin;
	int		m_GridZSize;
	//}}AFX_DATA

// Attributes
public:
	CTerrainDoc*	Document;

// Operations
public:
	CChildFrame* Frame;
	void ReadGridValues();
	Grid* GetSelectedGrid();
	CTerrainDoc* GetDocument() { return GetFrame()->GetDocument(); }
	CChildFrame*	GetFrame() { return Frame; }
	void	SetFrame(CChildFrame* c) { Frame = c; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GridFormView)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(GridFormView)
	afx_msg void OnDataChange();
	afx_msg void OnApplyButton();
	afx_msg void OnReadButton();
	afx_msg void OnPsdExport();
	afx_msg void OnPsdImport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDFORMVIEW_H__AF90A4E2_E13A_11D1_83C5_AABBB32D6B5D__INCLUDED_)
