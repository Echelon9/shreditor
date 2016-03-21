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
#if !defined(AFX_SOLIDPROPERTIESDIALOG_H__ED3521E1_A327_11D2_8C89_00104BC7527F__INCLUDED_)
#define AFX_SOLIDPROPERTIESDIALOG_H__ED3521E1_A327_11D2_8C89_00104BC7527F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SolidPropertiesDialog.h : header file
//


#include "Solid.h"


/////////////////////////////////////////////////////////////////////////////
// SolidPropertiesDialog dialog

class SolidPropertiesDialog : public CDialog
{
// Construction
public:
	SolidPropertiesDialog(CWnd* pParent = NULL);   // standard constructor

	void	SetName(const char* Name) { m_Name = Name; }
	void	SetComment(const char* Comment) { m_Comment = Comment; }
	void	SetDimension(int index, float f) { switch (index) { default: case 0: m_X = f; break; case 1: m_Y = f; break; case 2: m_Z = f; break; } };
	void	SetType(Solid::SolidType t) { Type = t; }

	const char*	GetName() { return m_Name; }
	const char*	GetComment() { return m_Comment; }
	float	GetDimension(int index) { switch (index) { default: case 0: return m_X; case 1: return m_Y; case 2: return m_Z; } };
	Solid::SolidType	GetType() { return Type; }

// Dialog Data
	//{{AFX_DATA(SolidPropertiesDialog)
	enum { IDD = IDD_SOLID_PROPERTIES };
	CString	m_Comment;
	CString	m_Name;
	float	m_X;
	float	m_Y;
	float	m_Z;
	CString	m_Dim0Label;
	CString	m_Dim1Label;
	CString	m_Dim2Label;
	//}}AFX_DATA
	Solid::SolidType	Type;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SolidPropertiesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SolidPropertiesDialog)
	afx_msg void OnBox();
	afx_msg void OnCylinder();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void	OnTypeButtonClicked();
	void	SetDialogState();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOLIDPROPERTIESDIALOG_H__ED3521E1_A327_11D2_8C89_00104BC7527F__INCLUDED_)
