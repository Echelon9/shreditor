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
#if !defined(AFX_SOLIDLISTVIEW_H__ED3521E0_A327_11D2_8C89_00104BC7527F__INCLUDED_)
#define AFX_SOLIDLISTVIEW_H__ED3521E0_A327_11D2_8C89_00104BC7527F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SolidListView.h : header file
//


class CTerrainDoc;
class CChildFrame;


/////////////////////////////////////////////////////////////////////////////
// SolidListView dialog

class SolidListView : public CPropertyPage
{
	DECLARE_DYNCREATE(SolidListView)

// Construction
public:
	SolidListView();
	~SolidListView();

// Dialog Data
	//{{AFX_DATA(SolidListView)
	enum { IDD = IDD_SOLID_DIALOG };
	CListCtrl	m_SolidList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(SolidListView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(SolidListView)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	afx_msg void OnEditSolid();
	afx_msg void OnAddNewSolid();
	afx_msg void OnDeleteSolid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void Update();
	CChildFrame*	GetFrame() { return Frame; }
	void	SetFrame(CChildFrame* f) { Frame = f; }
	CTerrainDoc*	GetDocument();

private:
	int GetSelectedItemIndex();
	CChildFrame*	Frame;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOLIDLISTVIEW_H__ED3521E0_A327_11D2_8C89_00104BC7527F__INCLUDED_)
