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
// onfirmDeleteDialog.cpp : implementation file
//

#include "stdafx.h"
#include "terrain.h"
#include "onfirmDeleteDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ConfirmDeleteDialog dialog


ConfirmDeleteDialog::ConfirmDeleteDialog(const char* ItemName, CWnd* pParent /*=NULL*/)
	: CDialog(ConfirmDeleteDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(ConfirmDeleteDialog)
	m_ItemName = _T("");
	//}}AFX_DATA_INIT

	m_ItemName = ItemName;
}


void ConfirmDeleteDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ConfirmDeleteDialog)
	DDX_Text(pDX, IDC_ITEM_NAME, m_ItemName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ConfirmDeleteDialog, CDialog)
	//{{AFX_MSG_MAP(ConfirmDeleteDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ConfirmDeleteDialog message handlers
