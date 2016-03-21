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
// GeometryPropertiesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "GeometryPropertiesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GeometryPropertiesDialog dialog


GeometryPropertiesDialog::GeometryPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GeometryPropertiesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(GeometryPropertiesDialog)
	m_Name = _T("");
	m_Filename = _T("");
	m_Comment = _T("");
	//}}AFX_DATA_INIT
}


void GeometryPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GeometryPropertiesDialog)
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDX_Text(pDX, IDC_FILENAME, m_Filename);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GeometryPropertiesDialog, CDialog)
	//{{AFX_MSG_MAP(GeometryPropertiesDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GeometryPropertiesDialog message handlers

void GeometryPropertiesDialog::OnBrowse() 
// Called when user presses the "Browse..." button.
{
	// Open up a dialog box to let the user pick a file.
	CFileDialog	d(TRUE);

	if (d.DoModal() != IDCANCEL) {
		// Get the current values of this dialog box.
		UpdateData(TRUE);

		// Change the filename, and propagate to this dialog.
		SetFilename(d.GetFileName());
		UpdateData(FALSE);
	}
}
