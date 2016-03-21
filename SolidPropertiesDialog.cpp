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
// SolidPropertiesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "SolidPropertiesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SolidPropertiesDialog dialog


SolidPropertiesDialog::SolidPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SolidPropertiesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(SolidPropertiesDialog)
	m_Comment = _T("");
	m_Name = _T("");
	m_X = 0.0f;
	m_Y = 0.0f;
	m_Z = 0.0f;
	m_Dim0Label = _T("");
	m_Dim1Label = _T("");
	m_Dim2Label = _T("");
	//}}AFX_DATA_INIT
}


void SolidPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SolidPropertiesDialog)
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDX_Text(pDX, IDC_X, m_X);
	DDX_Text(pDX, IDC_Y, m_Y);
	DDX_Text(pDX, IDC_Z, m_Z);
	DDX_Text(pDX, IDC_DIM0, m_Dim0Label);
	DDX_Text(pDX, IDC_DIM1, m_Dim1Label);
	DDX_Text(pDX, IDC_DIM2, m_Dim2Label);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate == 0) {
		// Set button state according to Type.
		CButton*	b = (CButton*) GetDlgItem(IDC_BOX);
		b->SetCheck(Type == Solid::BOX);
		
		b = (CButton*) GetDlgItem(IDC_CYLINDER);
		b->SetCheck(Type == Solid::CYLINDER);
	} else {
		// Query the dialog buttons to set Type.
		Type = Solid::CYLINDER;	// Default.

		CButton* b = (CButton*) GetDlgItem(IDC_BOX);
		if (b->GetCheck() & 1) {
			// "Box" is selected.
			Type = Solid::BOX;
		}
		
		b = (CButton*) GetDlgItem(IDC_CYLINDER);
		if (b->GetCheck() & 1) {
			// "Cylinder" is selected.
			Type = Solid::CYLINDER;
		}
	}

	if (Type == Solid::BOX) {
		// Make sure that dim2 is not grayed...
		CWnd*	c = GetDlgItem(IDC_DIM2);
		if (c) c->ModifyStyle(WS_DISABLED, 0);
		CEdit*	edit = (CEdit*) GetDlgItem(IDC_Z);
		if (edit) {
			edit->ModifyStyle(WS_DISABLED, 0);
			edit->SetReadOnly(FALSE);
		}
	} else {
		// gray out the unused dimension...
		CWnd*	c = GetDlgItem(IDC_DIM2);
		if (c) c->ModifyStyle(0, WS_DISABLED);
		CEdit*	edit = (CEdit*) GetDlgItem(IDC_Z);
		if (edit) {
			edit->ModifyStyle(0, WS_DISABLED);
			edit->SetReadOnly(TRUE);
		}
	}
}


BEGIN_MESSAGE_MAP(SolidPropertiesDialog, CDialog)
	//{{AFX_MSG_MAP(SolidPropertiesDialog)
	ON_BN_CLICKED(IDC_BOX, OnBox)
	ON_BN_CLICKED(IDC_CYLINDER, OnCylinder)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SolidPropertiesDialog message handlers



void SolidPropertiesDialog::OnBox() 
{
	OnTypeButtonClicked();
}


void SolidPropertiesDialog::OnCylinder() 
{
	OnTypeButtonClicked();
}


void	SolidPropertiesDialog::OnTypeButtonClicked()
// Called when any of the type radio-buttons has been clicked.
// Makes sure the dialog labels & state reflect the type.
{
	Solid::SolidType	OldType = Type;
	UpdateData(TRUE);	// Get data from dialog.
	if (OldType != Type) {
		SetDialogState();
	}
}


void	SolidPropertiesDialog::SetDialogState()
// Sets up the captions and controls according to our state.
{
	if (Type == Solid::BOX) {
		m_Dim0Label = "Length (X)";
		m_Dim1Label = "Height (Y)";
		m_Dim2Label = "Width (Z)";

	} else {
		m_Dim0Label = "Radius (XZ)";
		m_Dim1Label = "Height (Y)";
		m_Dim2Label = "unused";

	}

	UpdateData(FALSE);	// Send the data to the dialogs.
}


BOOL SolidPropertiesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDialogState();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void SolidPropertiesDialog::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}
