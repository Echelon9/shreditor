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
#if !defined(AFX_EXCAVATEDIALOG_H__86862B62_FACB_11D1_83C5_444553540000__INCLUDED_)
#define AFX_EXCAVATEDIALOG_H__86862B62_FACB_11D1_83C5_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ExcavateDialog.h : header file
//


const int	EXCAVATE_BITMAP_FILENAME_MAXLEN = 1000;


struct ExcavateParameters {
	float	Radius;
	float	EdgeTaper;
	float	Depth;
	float	Smoothing;
	float	ReferenceHeight;
	bool	BitmapModulate;
	char	BitmapFilename[EXCAVATE_BITMAP_FILENAME_MAXLEN];
	enum {
		ADDHEIGHT = 0,
		SMOOTHING,
		CLONE,
		DERESOLVE
	} Mode;

	ExcavateParameters() {
		Radius = 2.5;
		EdgeTaper = 1;
		Depth = 1;
		Smoothing = 1;
		ReferenceHeight = 0;
		BitmapModulate = false;
		BitmapFilename[0] = 0;
		Mode = ADDHEIGHT;
	}
};


/////////////////////////////////////////////////////////////////////////////
// ExcavateDialog dialog

class ExcavateDialog : public CDialog
{
// Construction
public:
	ExcavateDialog(CWnd* pParent = NULL);   // standard constructor

	const ExcavateParameters& GetParameters();
	void	SetParameters(const ExcavateParameters& p);

// Dialog Data
	//{{AFX_DATA(ExcavateDialog)
	enum { IDD = IDD_EXCAVATE };
	CEdit	m_BitmapFilename;
	CSliderCtrl	m_EdgeTaper;
	CSliderCtrl	m_SmoothingAmount;
	CEdit	m_Radius;
	CEdit	m_Depth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ExcavateDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	ExcavateParameters	Params;

	// Generated message map functions
	//{{AFX_MSG(ExcavateDialog)
	afx_msg void OnBitmapBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCAVATEDIALOG_H__86862B62_FACB_11D1_83C5_444553540000__INCLUDED_)
