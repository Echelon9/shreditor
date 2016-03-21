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
#if !defined(AFX_GEOMETRYPROPERTIESDIALOG_H__53C76082_8532_11D2_8C89_00104BC7527F__INCLUDED_)
#define AFX_GEOMETRYPROPERTIESDIALOG_H__53C76082_8532_11D2_8C89_00104BC7527F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GeometryPropertiesDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GeometryPropertiesDialog dialog

class GeometryPropertiesDialog : public CDialog
{
// Construction
public:
	GeometryPropertiesDialog(CWnd* pParent = NULL);   // standard constructor

	void	SetName(const char* name) { m_Name = name; }
	void	SetFilename(const char* filename) { m_Filename = filename; }
	void	SetComment(const char* comment) { m_Comment = comment; }

	const char*	GetName() { return m_Name; }
	const char*	GetFilename() { return m_Filename; }
	const char*	GetComment() { return m_Comment; }

// Dialog Data
	//{{AFX_DATA(GeometryPropertiesDialog)
	enum { IDD = IDD_GEOMETRY_PROPERTIES };
	CString	m_Name;
	CString	m_Filename;
	CString	m_Comment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GeometryPropertiesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GeometryPropertiesDialog)
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GEOMETRYPROPERTIESDIALOG_H__53C76082_8532_11D2_8C89_00104BC7527F__INCLUDED_)
