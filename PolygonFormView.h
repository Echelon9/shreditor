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
#if !defined(AFX_POLYGONFORMVIEW_H__36DAC163_7487_11D4_A717_00104BC7527F__INCLUDED_)
#define AFX_POLYGONFORMVIEW_H__36DAC163_7487_11D4_A717_00104BC7527F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PolygonFormView.h : header file
//


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "terraindoc.h"


class CChildFrame;


/////////////////////////////////////////////////////////////////////////////
// PolygonFormView dialog


class PolygonFormView : public CPropertyPage
{
//	DECLARE_DYNCREATE(PolygonFormView)

// Construction
public:
	PolygonFormView();
	~PolygonFormView();

// Dialog Data
	//{{AFX_DATA(PolygonFormView)
	enum { IDD = IDD_POLYPROPERTIES };
	CListCtrl	m_PropertyList;
	BOOL	m_Closed;
	//}}AFX_DATA

	bool	ClosedValid;

// Operations
public:
	void	SetFrame(CChildFrame* f) { Frame = f; }
	CChildFrame*	GetFrame() { return Frame; }
	CTerrainDoc* GetDocument();
	void	Update();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PolygonFormView)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PolygonFormView)
	afx_msg void OnApplyPolyProperties();
	afx_msg void OnClosed();
	afx_msg void OnDblclkProperties(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CChildFrame*	Frame;
	polygon*	SingleSelectedPolygon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POLYGONFORMVIEW_H__36DAC163_7487_11D4_A717_00104BC7527F__INCLUDED_)
