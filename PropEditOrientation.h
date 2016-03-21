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
#if !defined(AFX_PROPEDITORIENTATION_H__65B35740_CDC1_11D2_8C89_00104BC7527F__INCLUDED_)
#define AFX_PROPEDITORIENTATION_H__65B35740_CDC1_11D2_8C89_00104BC7527F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PropEditOrientation.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PropEditOrientation dialog

class PropEditOrientation : public CDialog
{
// Construction
public:
	PropEditOrientation(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PropEditOrientation)
	enum { IDD = IDD_PROP_EDIT_ORIENTATION };
	float	m_Yaw;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PropEditOrientation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	float	GetValue() { return m_Yaw; }
	void	SetValue(float NewYaw) { m_Yaw = NewYaw; }

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PropEditOrientation)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DrawTickMark(CDC* dc);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPEDITORIENTATION_H__65B35740_CDC1_11D2_8C89_00104BC7527F__INCLUDED_)
