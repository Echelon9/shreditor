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
#if !defined(AFX_OBJECTFORMVIEW_H__86862B63_FACB_11D1_83C5_444553540000__INCLUDED_)
#define AFX_OBJECTFORMVIEW_H__86862B63_FACB_11D1_83C5_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ObjectFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ObjectFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "terraindoc.h"


class CChildFrame;


class ObjectFormView : public CPropertyPage
{
public:
	ObjectFormView();
	virtual ~ObjectFormView();

// Form Data
public:
	//{{AFX_DATA(ObjectFormView)
	enum { IDD = IDD_OBJECTPROPERTIES };
	CListCtrl	m_PropertyList;
	CComboBox	m_ObjectType;
	CComboBox	m_ObjectGeometry;
	CComboBox	m_ObjectSolid;
	float	m_ObjectX;
	float	m_ObjectY;
	float	m_ObjectZ;
	BOOL	m_PinToGround;
	//}}AFX_DATA

	bool	TypeValid;
	bool	GeometryValid;
	bool	SolidValid;
	bool	XValid;
	bool	YValid;
	bool	ZValid;
	bool	PinValid;
	// ...

// Attributes
public:

// Operations
public:
	void	SetFrame(CChildFrame* f) { Frame = f; }
	CChildFrame*	GetFrame() { return Frame; }
	CTerrainDoc* GetDocument();
	void	Update();

// Overrides
//	BOOL	OnInitDialog();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ObjectFormView)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ObjectFormView)
	afx_msg void OnApplyObjectProperties();
	afx_msg void OnSelchangeComboObjecttype();
	afx_msg void OnChangeObjectx();
	afx_msg void OnChangeObjecty();
	afx_msg void OnChangeObjectz();
	afx_msg void OnSelchangeComboObjectgeometry();
	afx_msg void OnSelchangeComboObjectsolid();
	afx_msg void OnPintoground();
	afx_msg void OnDblclkProperties(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CChildFrame*	Frame;
	Object*	SingleSelectedObject;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTFORMVIEW_H__86862B63_FACB_11D1_83C5_444553540000__INCLUDED_)
