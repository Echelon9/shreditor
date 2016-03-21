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
// terrain.cpp : Defines the class behaviors for the application.
//


#include "stdafx.h"

#include <math.h>

#include "terrain.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "terrainDoc.h"
#include "terrainView.h"
//#include "GridView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerrainApp

BEGIN_MESSAGE_MAP(CTerrainApp, CWinApp)
	//{{AFX_MSG_MAP(CTerrainApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainApp construction

CTerrainApp::CTerrainApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	GridDocTemplate = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTerrainApp object

CTerrainApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTerrainApp initialization

BOOL CTerrainApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Slingshot / Soul Ride Settings"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	// We're going to register a document template that's the main view of the
	// document, and then we're going to register additional templates that provide
	// views into different aspects of the document, like an object editing window,
	// list of geometry models, etc.
	// We use some tricks from the MFC Checkbook sample app, to keep the framework
	// from thinking that these additional views are new types of documents that
	// we support.

	// Main view of terrain.
	TerrainViewTemplate = new CMultiDocTemplate(
		IDR_TERRAITYPE,
		RUNTIME_CLASS(CTerrainDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTerrainView));
	AddDocTemplate(TerrainViewTemplate);

//	// Object editing view.
//	ObjectViewTemplate = new CMultiDocTemplate(IDR_OBJECTEDIT,
//			RUNTIME_CLASS(CTerrainDoc),
//			RUNTIME_CLASS(CChildFrame),
//			RUNTIME_CLASS(ObjectFormView));
//	AddDocTemplate(ObjectViewTemplate);

//	// Geometry list view.
//	GeometryListViewTemplate = new CMultiDocTemplate(IDR_GEOMETRYLIST,
//			RUNTIME_CLASS(CTerrainDoc),
//			RUNTIME_CLASS(CChildFrame),
//			RUNTIME_CLASS(GeometryListView));
//	AddDocTemplate(GeometryListViewTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Maximize on start-up.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTerrainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CTerrainApp commands


struct SurfaceTypeInfo {
	int r, g, b;
	char*	Name;
} SurfaceTypeInfo[17] = {
	{ 220, 250, 215, "forest" },
	{ 160, 160, 210, "ice" },
	{ 190, 190, 215, "granular" },
	{ 225, 225, 225, "hardpack" },

	{ 255, 255, 255, "powder" },
	{ 170, 170, 170, "rock" },
	{ 234, 234, 150, "sand" },
	{ 100, 100, 100, "asphalt" },

	{  80, 100, 255, "water" },
	{ 50, 50, 50, "---" },
	{ 50, 50, 50, "---" },
	{ 50, 50, 50, "---" },

	{ 50, 50, 50, "---" },
	{ 50, 50, 50, "---" },
	{ 50, 50, 50, "---" },
	{ 50, 50, 50, "---" },

	{ 50, 50, 50, "---" },
};


COLORREF	TypeColor(unsigned char t)
// Given a type value, returns the RGB color that represents it.
{
//	float	f1 = (t & 15) / 16.0;
//	float	f0 = 1 - f0;

	int	i = t;
	return RGB(SurfaceTypeInfo[i].r,
		SurfaceTypeInfo[i].g,
		SurfaceTypeInfo[i].b);
}


const char*	TypeName(unsigned char t)
// Given a type value, returns a short string which identifies the type.
{
	int	i = t;
	return SurfaceTypeInfo[i].Name;
}


void	DrawHeightField(CDC* pDC, int xorigin, int yorigin, int BlockPixels, float* DataGrid, unsigned char* TypeDataGrid, int xsize, int zsize, int GridPitch, float ContourInterval)
// Renders a height field into the given device context.
{
	// Draw hash marks.
	int	x, y;
	y = yorigin;
	float*	RowStart = DataGrid;
	unsigned char*	TypeRowStart = TypeDataGrid;
	for (int j = 0; j < zsize; j++) {
		float*	p = RowStart;
		unsigned char*	t = TypeRowStart;
		x = xorigin;
		for (int i = 0; i < xsize; i++) {
			// Show surface type behind the contour lines.
			pDC->FillSolidRect(x, y, BlockPixels, BlockPixels, TypeColor(*t));

			// Draw hash dot.
//			COLORREF c = RGB(Gray, Gray, Gray);
//			pDC->SetPixelV(x, y, c);

			p++;
			t++;
			x += BlockPixels;
		}
		RowStart += GridPitch;
		TypeRowStart += GridPitch;
		y += BlockPixels;
	}

	//
	// Draw contour lines.
	//

	float	OneOverInterval = 1.0 / ContourInterval;

	// Setup pens.
	const int	PENCOUNT = 6;
	CPen	pen[PENCOUNT];
	pen[0].CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pen[1].CreatePen(PS_SOLID, 1, RGB(40, 40, 60));
	pen[2].CreatePen(PS_SOLID, 1, RGB(80, 80, 100));
	pen[3].CreatePen(PS_SOLID, 1, RGB(120, 120, 140));
	pen[4].CreatePen(PS_SOLID, 1, RGB(140, 140, 160));
	pen[5].CreatePen(PS_SOLID, 1, RGB(180, 180, 200));

	// Select the new pen into the DC.
	CPen*	OldPen = pDC->SelectObject(&pen[0]);

//	y = z0 * BlockPixels;
	y = yorigin;
	RowStart = DataGrid;
	for (j = 0; j < zsize-1; j++) {
		float*	p = RowStart;
//		x = x0 * BlockPixels;
		x = xorigin;
		for (int i = 0; i < xsize-1; i++) {
			// Draw contour lines.
//			ContourBox(x, y, BlockPixels, 
			float	samp[4];
			samp[0] = *p * OneOverInterval;
			samp[1] = *(p + 1) * OneOverInterval;
			samp[2] = *(p + GridPitch + 1) * OneOverInterval;
			samp[3] = *(p + GridPitch) * OneOverInterval;

			int	min = 100000;
			int	max = -100000;
			int	isamp[4];
			for (int k = 0; k < 4; k++) {
				// Keep samples within reasonable bounds.
				if (samp[k] < -300) samp[k] = -300;
				if (samp[k] > 10000) samp[k] = 10000;

				// Convert to integer.
				isamp[k] = floor(samp[k]);

				// Compute min & max.
				if (isamp[k] < min) min = isamp[k];
				if (isamp[k] > max) max = isamp[k];
			}

			// For each interval crossing within the range of values, draw the
			// appropriate line across the box.
			for (k = min + 1; k <= max; k++) {
				int	EdgePix[4];

				// Choose the appropriate pen.
				pDC->SelectObject(&pen[k % PENCOUNT]);

				// Check each edge to see if this contour
				// crosses it.
				for (int l = 0; l < 4; l++) {
					if ((isamp[(l+3)&3] < k && isamp[l] >= k) ||
					    (isamp[(l+3)&3] >= k && isamp[l] < k))
					{
						float	num = k - samp[(l+3)&3];
						if (l == 0 || l == 3) {
							EdgePix[l] = BlockPixels * (1 - num / (samp[l] - samp[(l+3)&3]));
						} else {
							EdgePix[l] = BlockPixels * num / (samp[l] - samp[(l+3)&3]);
						}
//						EdgePix[l] = BlockPixels >> 1;//xxxxxx
//						if (l > 1) EdgePix[l] = BlockPixels - EdgePix[l];//xxx
					} else {
						EdgePix[l] = -1;
					}
				}

				// Connect the edges.
				for (l = 0; l < 4; l++) {
					if (EdgePix[l] == -1) continue;
					for (int m = l+1; m < 4; m++) {
						if (EdgePix[m] != -1) {
							// Connect edge l with m.
							int	xs, ys;
							if (l & 1) {
								xs = x + EdgePix[l];
								ys = y + (l == 3 ? BlockPixels : 0);
							} else {
								xs = x + (l == 2 ? BlockPixels : 0);
								ys = y + EdgePix[l];
							}

							int	xe, ye;
							if (m & 1) {
								xe = x + EdgePix[m];
								ye = y + (m == 3 ? BlockPixels : 0);
							} else {
								xe = x + (m == 2 ? BlockPixels : 0);
								ye = y + EdgePix[m];
							}

							if (l == 2 && m == 3) {
								pDC->MoveTo(xe, ye);
								pDC->LineTo(xs, ys);
							} else {
								pDC->MoveTo(xs, ys);
								pDC->LineTo(xe, ye);
							}

							if (m > l) l = m;
							break;
						}
					}
				}
			}

			p++;
			x += BlockPixels;
		}
		RowStart += GridPitch;
		y += BlockPixels;
	}

	// delete pens?
	
	// Restore the original pen.
	pDC->SelectObject(OldPen);
}


void	DrawBox(CDC* dc, int x, int y, int xsize, int ysize, COLORREF color)
// Draws a box in the given device context.
{
	CPen	pen;
	pen.CreatePen(PS_SOLID, 1, color);
	// Select the new pen into the DC.
	CPen*	OldPen = dc->SelectObject(&pen);

	POINT	Points[5];
	Points[0].x = x;
	Points[0].y = y;
	Points[1].x = Points[0].x + xsize;
	Points[1].y = Points[0].y;
	Points[2].x = Points[1].x;
	Points[2].y = Points[1].y + ysize;
	Points[3].x = Points[0].x;
	Points[3].y = Points[2].y;
	Points[4] = Points[0];
	
	dc->Polyline(Points, 5);

	dc->SelectObject(OldPen);
}


CDocument* CTerrainApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// CWinApp::OpenDocmentFile creates the first MDI child window
	// for the terrain view.  This override creates the second MDI child
	// window for the object view.

	CTerrainDoc* pDoc = (CTerrainDoc*)CWinApp::OpenDocumentFile(lpszFileName);
	return pDoc;
}


void CTerrainApp::AddToRecentFileList(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWinApp::AddToRecentFileList(lpszPathName);
}



// HACK to link w/ VC7
#define VC7
#ifdef VC7

extern "C"
{
	__declspec(naked)
	void __cdecl _ftol2(void)
	{
		__asm
		{
			sub		esp, 4
			fistp	[DWORD PTR esp]
			pop		eax
			cdq
			ret
		};
	}
}

#endif // VC7
