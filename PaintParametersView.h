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
#if !defined(AFX_PAINTPARAMETERSVIEW_H__7304F2A3_0089_11D3_8C89_00104BC7527F__INCLUDED_)
#define AFX_PAINTPARAMETERSVIEW_H__7304F2A3_0089_11D3_8C89_00104BC7527F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PaintParametersView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PaintParametersView dialog

struct PaintParameters {
	float	Radius;
	unsigned char	LeftButtonType, RightButtonType;

	PaintParameters() : Radius(1.5), LeftButtonType(1), RightButtonType(0) {}
};


class PaintParametersView : public CPropertyPage
{
	DECLARE_DYNCREATE(PaintParametersView)

// Construction
public:
	void Update();
	PaintParametersView();
	~PaintParametersView();

	PaintParameters	Params;

// Dialog Data
	//{{AFX_DATA(PaintParametersView)
	enum { IDD = IDD_PAINT_PARAMETERS };
	CStatic	m_Palette;
	float	m_PaintRadius;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PaintParametersView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PaintParametersView)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	afx_msg void OnPaint();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChangePaintRadius();
	afx_msg void OnSetradius();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void	GetPaletteRect(RECT* r);
	int	ClassifyPalettePoint(CPoint p);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAINTPARAMETERSVIEW_H__7304F2A3_0089_11D3_8C89_00104BC7527F__INCLUDED_)
