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
// ExcavateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "ExcavateDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ExcavateDialog dialog


ExcavateDialog::ExcavateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ExcavateDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(ExcavateDialog)
	//}}AFX_DATA_INIT
}


void ExcavateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ExcavateDialog)
	DDX_Control(pDX, IDC_BITMAP_NAME, m_BitmapFilename);
	DDX_Control(pDX, IDC_EDGETAPER, m_EdgeTaper);
	DDX_Control(pDX, IDC_SMOOTHINGAMOUNT, m_SmoothingAmount);
	DDX_Control(pDX, IDC_EXCAVATE_WIDTH, m_Radius);
	DDX_Control(pDX, IDC_EXCAVATE_DEPTH, m_Depth);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate) {
		// Send data to the controls.
		char	buf[80];

		sprintf(buf, "%g", Params.Depth);
		if (m_Depth.m_hWnd) m_Depth.SetWindowText(buf);

		sprintf(buf, "%g", Params.Radius);
		if (m_Radius.m_hWnd) m_Radius.SetWindowText(buf);

		m_EdgeTaper.SetRange(1, 10, TRUE);
		m_EdgeTaper.SetPos(Params.EdgeTaper * 2);

		m_SmoothingAmount.SetRange(1, 10, TRUE);
		m_SmoothingAmount.SetPos(Params.Smoothing * 10);

		m_BitmapFilename.SetWindowText(Params.BitmapFilename);
		
		CButton* b = (CButton*) GetDlgItem(IDC_ADDSUBHEIGHT);
		if (Params.Mode == ExcavateParameters::ADDHEIGHT) {
			b->SetCheck(1);
		} else {
			b->SetCheck(0);
		}

		b = (CButton*) GetDlgItem(IDC_SMOOTHING);
		if (Params.Mode == ExcavateParameters::SMOOTHING) {
			b->SetCheck(1);
		} else {
			b->SetCheck(0);
		}

		b = (CButton*) GetDlgItem(IDC_CLONEHEIGHT);
		if (Params.Mode == ExcavateParameters::CLONE) {
			b->SetCheck(1);
		} else {
			b->SetCheck(0);
		}

		b = (CButton*) GetDlgItem(IDC_DERESOLVE);
		if (Params.Mode == ExcavateParameters::DERESOLVE) {
			b->SetCheck(1);
		} else {
			b->SetCheck(0);
		}

		b = (CButton*) GetDlgItem(IDC_BITMAP_MODULATE);
		if (Params.BitmapModulate == true) {
			b->SetCheck(1);
			// un-gray the bitmap filename edit field.
		} else {
			b->SetCheck(0);
			// gray the bitmap filename edit field.
		}
	}

	if (pDX->m_bSaveAndValidate) {
		// Retrieve data from controls.
		char	buf[80];
		m_Depth.GetWindowText(buf, 80);
		Params.Depth = atof(buf);

		m_Radius.GetWindowText(buf, 80);
		Params.Radius = atof(buf);

		Params.EdgeTaper = m_EdgeTaper.GetPos() / 2.0;

		Params.Smoothing = m_SmoothingAmount.GetPos() / 10.0;

		m_BitmapFilename.GetWindowText(Params.BitmapFilename, EXCAVATE_BITMAP_FILENAME_MAXLEN);
		
		CButton* b = (CButton*) GetDlgItem(IDC_ADDSUBHEIGHT);
		if (b->GetCheck() & 1) {
			Params.Mode = ExcavateParameters::ADDHEIGHT;
		}

		b = (CButton*) GetDlgItem(IDC_SMOOTHING);
		if (b->GetCheck() & 1) {
			Params.Mode = ExcavateParameters::SMOOTHING;
		}

		b = (CButton*) GetDlgItem(IDC_CLONEHEIGHT);
		if (b->GetCheck() & 1) {
			Params.Mode = ExcavateParameters::CLONE;
		}

		b = (CButton*) GetDlgItem(IDC_DERESOLVE);
		if (b->GetCheck() & 1) {
			Params.Mode = ExcavateParameters::DERESOLVE;
		}

		b = (CButton*) GetDlgItem(IDC_BITMAP_MODULATE);
		if (b->GetCheck() & 1) {
			Params.BitmapModulate = true;
		} else {
			Params.BitmapModulate = false;
		}
	}
}


BEGIN_MESSAGE_MAP(ExcavateDialog, CDialog)
	//{{AFX_MSG_MAP(ExcavateDialog)
	ON_BN_CLICKED(IDC_BITMAP_BROWSE, OnBitmapBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ExcavateDialog message handlers



const ExcavateParameters&	ExcavateDialog::GetParameters()
// Returns the currently set excavation parameters.
{
	return Params;
}


void	ExcavateDialog::SetParameters(const ExcavateParameters& p)
// Sets the excavate parameters.
{
	Params = p;
}


void ExcavateDialog::OnBitmapBrowse()
// Called when user clicks "..." next to the bitmap name.  Pop up a browse
// dialog, for picking a file.
{
	// Open up a dialog box to let the user pick a file.
	CFileDialog	d(TRUE);

	d.m_ofn.lpstrFilter = "Photoshop bitmaps (.PSD)\0*.PSD\0";
	
	if (d.DoModal() != IDCANCEL) {
		// Get the current values of this dialog box.
		UpdateData(TRUE);

		// Change the filename, and propagate to this dialog.
		strncpy(Params.BitmapFilename, d.GetPathName(), EXCAVATE_BITMAP_FILENAME_MAXLEN);
		Params.BitmapFilename[EXCAVATE_BITMAP_FILENAME_MAXLEN-1] = 0;
		UpdateData(FALSE);
	}
}
