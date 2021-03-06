; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=SelectByAttributeDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "terrain.h"
LastPage=0

ClassCount=31
Class1=CChildFrame
Class2=ExcavateDialog
Class3=GridFormView
Class4=GridPropertyView
Class5=CMainFrame
Class6=ObjectFormView
Class7=PositioningWindow
Class8=CTerrainApp
Class9=CAboutDlg
Class10=CTerrainDoc
Class11=CTerrainView
Class12=ToolPalette

ResourceCount=23
Resource1=IDD_PAINT_PARAMETERS
Resource2=IDD_SOLID_DIALOG
Resource3=IDD_OFFSET_PARAMS
Resource4=IDD_PROP_EDIT_ORIENTATION
Resource5=IDD_EXCAVATE
Resource6=IDD_OBJECTPROPERTIES
Resource7=IDD_GRID_PROCESSING
Class13=GeometryListView
Resource8=IDD_GEOMETRY_DIALOG
Class14=GeometryPropertiesDialog
Resource9=IDD_TOOL
Resource10=IDD_POLYPROPERTIES
Resource11=IDD_CONFIRM_DELETE_INUSE
Resource12=IDD_GEOMETRY_PROPERTIES
Class15=GridPage
Class16=ToolSheet
Class17=PropertyView
Class18=SolidListView
Class19=SolidPropertiesDialog
Class20=ConfirmDeleteDialog
Resource13=IDD_SOLID_PROPERTIES
Class21=SelectByAttributeDialog
Resource14=IDR_TERRAITYPE
Class22=PlantParametersDialog
Resource15=IDD_ABOUTBOX
Resource16=IDD_PROP_EDIT_MULTILINE_STRING
Class23=PropEditInt
Resource17=IDD_CONFIRM_DELETE
Class24=PropEditString
Resource18=IDR_MAINFRAME
Class25=PropEditOrientation
Resource19=IDD_PLANT_PARAMETERS
Class26=OffsetParams
Resource20=IDD_PROP_EDIT_STRING
Class27=PaintParametersView
Class28=CSurfaceTypePalette
Resource21=IDD_FORMVIEW
Class29=PolyProperties
Class30=PolygonFormView
Resource22=IDD_PROP_EDIT_INT
Class31=PropEditMultiLineString
Resource23=IDD_SELECTBYATTR

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
VirtualFilter=mfWC
LastObject=CChildFrame

[CLS:ExcavateDialog]
Type=0
BaseClass=CDialog
HeaderFile=ExcavateDialog.h
ImplementationFile=ExcavateDialog.cpp
LastObject=IDC_BITMAP_NAME
Filter=D
VirtualFilter=dWC

[CLS:GridFormView]
Type=0
BaseClass=CPropertyPage
HeaderFile=Gridformview.h
ImplementationFile=Gridformview.cpp
Filter=D
VirtualFilter=idWC
LastObject=GridFormView

[CLS:GridPropertyView]
Type=0
BaseClass=CDialog
HeaderFile=GridPropertyView.h
ImplementationFile=GridPropertyView.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=ID_EDIT_GENERATE_TYPEMAPS

[CLS:ObjectFormView]
Type=0
HeaderFile=ObjectFormView.h
ImplementationFile=ObjectFormView.cpp
Filter=D
LastObject=IDC_APPLYOBJECTPROPERTIES
BaseClass=CPropertyPage
VirtualFilter=idWC

[CLS:PositioningWindow]
Type=0
BaseClass=CScrollView
HeaderFile=PositioningWindow.h
ImplementationFile=PositioningWindow.cpp

[CLS:CTerrainApp]
Type=0
BaseClass=CWinApp
HeaderFile=terrain.h
ImplementationFile=terrain.cpp
Filter=N
VirtualFilter=AC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=terrain.cpp
ImplementationFile=terrain.cpp
LastObject=CAboutDlg

[CLS:CTerrainDoc]
Type=0
BaseClass=CDocument
HeaderFile=terrainDoc.h
ImplementationFile=terrainDoc.cpp
LastObject=ID_APP_EXIT

[CLS:CTerrainView]
Type=0
BaseClass=CView
HeaderFile=terrainView.h
ImplementationFile=Terrainview.cpp
Filter=C
VirtualFilter=VWC

[CLS:ToolPalette]
Type=0
BaseClass=CFormView
HeaderFile=ToolPalette.h
ImplementationFile=ToolPalette.cpp
Filter=D
VirtualFilter=VWC
LastObject=IDC_CLEANUP_QUADTREE

[DLG:IDD_EXCAVATE]
Type=1
Class=ExcavateDialog
ControlCount=20
Control1=IDC_EXCAVATE_WIDTH,edit,1350631552
Control2=IDC_ADDSUBHEIGHT,button,1342177289
Control3=IDC_SMOOTHING,button,1342177289
Control4=IDC_CLONEHEIGHT,button,1342177289
Control5=IDC_EXCAVATE_DEPTH,edit,1350631552
Control6=IDC_SMOOTHINGAMOUNT,msctls_trackbar32,1342242840
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDGETAPER,msctls_trackbar32,1342242840
Control16=IDC_STATIC,static,1342308352
Control17=IDC_DERESOLVE,button,1342177289
Control18=IDC_BITMAP_MODULATE,button,1342242819
Control19=IDC_BITMAP_NAME,edit,1350631552
Control20=IDC_BITMAP_BROWSE,button,1342242816

[DLG:IDD_FORMVIEW]
Type=1
Class=GridFormView
ControlCount=14
Control1=IDC_GRIDXSIZE,edit,1350631552
Control2=IDC_STATIC,static,1342308352
Control3=IDC_GRIDZSIZE,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_GRIDSCALE,msctls_trackbar32,1342242824
Control6=IDC_STATIC,static,1342308352
Control7=IDC_GRIDXORIGIN,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_GRIDZORIGIN,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_APPLYGRIDFORM,button,1342242817
Control12=IDC_PSD_EXPORT,button,1342242816
Control13=IDC_PSD_IMPORT,button,1342242816
Control14=IDC_STATIC,button,1342177287

[DLG:IDD_OBJECTPROPERTIES]
Type=1
Class=ObjectFormView
ControlCount=17
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_OBJECTTYPE,combobox,1344339971
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_OBJECTX,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_OBJECTY,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_OBJECTZ,edit,1350631552
Control10=IDC_PINTOGROUND,button,1342242819
Control11=IDC_APPLYOBJECTPROPERTIES,button,1342242817
Control12=IDC_STATIC,static,1342308352
Control13=IDC_COMBO_OBJECTGEOMETRY,combobox,1344339971
Control14=IDC_COMBO_OBJECTSOLID,combobox,1344339971
Control15=IDC_STATIC,static,1342308352
Control16=IDC_PROPERTIES,SysListView32,1350631429
Control17=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_TOOL]
Type=1
Class=ToolPalette
ControlCount=22
Control1=IDC_STATIC,static,1342308352
Control2=IDC_GRIDTOOL,button,1342177289
Control3=IDC_EXCAVATE,button,1342177289
Control4=IDC_PAINTSURFACE,button,1342177289
Control5=IDC_PLANTOBJECTS,button,1342177289
Control6=IDC_OBJECTTOOL,button,1342177289
Control7=IDC_VERTEX,button,1342177289
Control8=IDC_HAND,button,1342177289
Control9=IDC_STATIC,static,1342308352
Control10=IDC_DRAW_SELECTED_GRID,button,1342242819
Control11=IDC_DRAW_ALL_GRIDS,button,1342242819
Control12=IDC_DRAW_OBJECTS,button,1342242819
Control13=IDC_DRAW_SURFACE,button,1342242819
Control14=IDC_STATIC,static,1342308352
Control15=IDC_ZOOM,msctls_trackbar32,1342242842
Control16=IDC_ZOOM_NUMBER,edit,1342244992
Control17=IDC_STATIC,static,1342308352
Control18=IDC_CONTOURINTERVAL,msctls_trackbar32,1342242842
Control19=IDC_CONTOUR_NUMBER,edit,1342244992
Control20=IDC_NODE_COUNT,static,1342308352
Control21=IDC_CLEANUP_QUADTREE,button,1342242816
Control22=IDC_SHOW_TOOL_CURSOR,button,1342242819

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_ZOOM_TERRAIN_IN
Command10=ID_ZOOM_TERRAIN_OUT
Command11=ID_NEW_GRID
Command12=ID_ADD_NEW_POLYGON
CommandCount=12

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_TERRAITYPE]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_LOAD_INTO_CURRENT
Command7=ID_FILE_PRINT
Command8=ID_FILE_PRINT_PREVIEW
Command9=ID_FILE_PRINT_SETUP
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_EDIT_UNDO
Command13=ID_EDIT_CUT
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_EDIT_DELETE
Command17=ID_EDIT_OFFSET
Command18=ID_EDIT_GENERATE_TYPEMAPS
Command19=ID_VIEW_TOOLBAR
Command20=ID_VIEW_STATUS_BAR
Command21=ID_EXCAVATE
Command22=ID_NEW_GRID
Command23=ID_GRIDDOUBLEHEIGHT
Command24=ID_GRIDHALVEHEIGHT
Command25=ID_OBJECT_SELECTBYATTR
Command26=ID_OBJECT_PLANTPARAMETERS
Command27=ID_WINDOW_NEW
Command28=ID_WINDOW_CASCADE
Command29=ID_WINDOW_TILE_HORZ
Command30=ID_WINDOW_ARRANGE
Command31=ID_APP_ABOUT
CommandCount=31

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_DELETE
Command9=ID_EDIT_CUT
Command10=ID_NEXT_PANE
Command11=ID_PREV_PANE
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_EDIT_CUT
Command15=ID_EDIT_UNDO
CommandCount=15

[CLS:GeometryListView]
Type=0
HeaderFile=GeometryListView.h
ImplementationFile=GeometryListView.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=GeometryListView
VirtualFilter=VWC

[DLG:IDD_GEOMETRY_DIALOG]
Type=1
Class=GeometryListView
ControlCount=4
Control1=IDC_ADD_NEW_GEOMETRY,button,1342242816
Control2=IDC_DELETE_GEOMETRY,button,1342242816
Control3=IDC_EDIT_GEOMETRY,button,1342242816
Control4=IDC_GEOMETRYLIST,SysListView32,1350631425

[DLG:IDD_GEOMETRY_PROPERTIES]
Type=1
Class=GeometryPropertiesDialog
ControlCount=9
Control1=IDC_NAME,edit,1350631552
Control2=IDC_FILENAME,edit,1350631552
Control3=IDC_BROWSE,button,1342242816
Control4=IDC_COMMENT,edit,1350631552
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[CLS:GeometryPropertiesDialog]
Type=0
HeaderFile=GeometryPropertiesDialog.h
ImplementationFile=GeometryPropertiesDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=GeometryPropertiesDialog

[DLG:IDD_CONFIRM_DELETE]
Type=1
Class=ConfirmDeleteDialog
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_ITEM_NAME,edit,1342244992
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_CONFIRM_DELETE_INUSE]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_GEO_NAME,edit,1342244992
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:GridPage]
Type=0
HeaderFile=GridPage.h
ImplementationFile=GridPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=GridPage

[CLS:ToolSheet]
Type=0
HeaderFile=ToolSheet.h
ImplementationFile=ToolSheet.cpp
BaseClass=CPropertySheet
Filter=W
LastObject=ID_ADD_NEW_POLYGON
VirtualFilter=hWC

[CLS:PropertyView]
Type=0
HeaderFile=PropertyView.h
ImplementationFile=PropertyView.cpp
BaseClass=CView
Filter=C
LastObject=ID_ADD_NEW_POLYGON
VirtualFilter=VWC

[DLG:IDD_SOLID_PROPERTIES]
Type=1
Class=SolidPropertiesDialog
ControlCount=17
Control1=IDC_NAME,edit,1350631552
Control2=IDC_COMMENT,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_CYLINDER,button,1342177289
Control8=IDC_BOX,button,1342177289
Control9=IDC_X,edit,1350631552
Control10=IDC_SPIN1,msctls_updown32,1342177312
Control11=IDC_DIM1,static,1342308354
Control12=IDC_Y,edit,1350631552
Control13=IDC_SPIN2,msctls_updown32,1342177312
Control14=IDC_DIM0,static,1342308354
Control15=IDC_Z,edit,1350631552
Control16=IDC_SPIN3,msctls_updown32,1342177312
Control17=IDC_DIM2,static,1342308354

[DLG:IDD_SOLID_DIALOG]
Type=1
Class=SolidListView
ControlCount=4
Control1=IDC_ADD_NEW_SOLID,button,1342242816
Control2=IDC_DELETE_SOLID,button,1342242816
Control3=IDC_EDIT_SOLID,button,1342242816
Control4=IDC_SOLIDLIST,SysListView32,1350631425

[CLS:SolidListView]
Type=0
HeaderFile=SolidListView.h
ImplementationFile=SolidListView.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_ADD_NEW_SOLID

[CLS:SolidPropertiesDialog]
Type=0
HeaderFile=SolidPropertiesDialog.h
ImplementationFile=SolidPropertiesDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_DIM0

[CLS:ConfirmDeleteDialog]
Type=0
HeaderFile=onfirmDeleteDialog.h
ImplementationFile=onfirmDeleteDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=ConfirmDeleteDialog
VirtualFilter=dWC

[DLG:IDD_SELECTBYATTR]
Type=1
Class=SelectByAttributeDialog
ControlCount=25
Control1=IDC_STATIC,static,1342308352
Control2=IDC_SELECT,button,1342242816
Control3=IDC_DESELECT,button,1342242816
Control4=IDC_CLEAR,button,1342242816
Control5=IDOK,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_NOTTYPE,button,1342242819
Control8=IDC_STATIC,static,1342308352
Control9=IDC_COMBO_OBJECTTYPE,combobox,1344339971
Control10=IDC_STATIC,static,1342308352
Control11=IDC_NOTGEOMETRY,button,1342242819
Control12=IDC_STATIC,static,1342308352
Control13=IDC_COMBO_OBJECTGEOMETRY,combobox,1344339971
Control14=IDC_STATIC,static,1342308352
Control15=IDC_NOTSOLID,button,1342242819
Control16=IDC_STATIC,static,1342308352
Control17=IDC_COMBO_OBJECTSOLID,combobox,1344339971
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_PIN,button,1342242854
Control22=IDC_RANDOMMASK,msctls_trackbar32,1342242840
Control23=IDC_STATIC,static,1342308353
Control24=IDC_STATIC,static,1342308354
Control25=IDC_STATIC,static,1342308352

[CLS:SelectByAttributeDialog]
Type=0
HeaderFile=SelectByAttributeDialog.h
ImplementationFile=SelectByAttributeDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CLEAR
VirtualFilter=dWC

[DLG:IDD_PLANT_PARAMETERS]
Type=1
Class=PlantParametersDialog
ControlCount=19
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_OBJECTTYPE,combobox,1344339971
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_OBJECTGEOMETRY,combobox,1344339971
Control8=IDC_OBJECTSOLID,combobox,1344339971
Control9=IDC_STATIC,static,1342308352
Control10=IDC_OBJECTY,edit,1350631552
Control11=IDC_PINTOGROUND,button,1342242819
Control12=IDC_STATIC,button,1342177287
Control13=IDC_ENABLESPRAY,button,1342242819
Control14=IDC_STATIC,static,1342308352
Control15=IDC_RADIUS,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_DENSITY,edit,1350631552
Control18=IDC_ERASEPERCENT,edit,1350631552
Control19=IDC_STATIC,static,1342308352

[CLS:PlantParametersDialog]
Type=0
HeaderFile=PlantParametersDialog.h
ImplementationFile=PlantParametersDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_DENSITY

[DLG:IDD_GRID_PROCESSING]
Type=1
Class=?
ControlCount=25
Control1=IDC_STATIC,button,1342177287
Control2=IDC_BUTTON2,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BUTTON3,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_BUTTON4,button,1342242816
Control8=IDC_EDIT1,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT2,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT3,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT4,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDIT5,edit,1350631552
Control20=IDC_EDIT6,edit,1350631552
Control21=IDC_EDIT7,edit,1350631552
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_BUTTON5,button,1342242816
Control25=IDC_STATIC,button,1342177287

[DLG:IDD_PROP_EDIT_INT]
Type=1
Class=PropEditInt
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_INT,edit,1350631552

[CLS:PropEditInt]
Type=0
HeaderFile=PropEditInt.h
ImplementationFile=PropEditInt.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_INT
VirtualFilter=dWC

[DLG:IDD_PROP_EDIT_STRING]
Type=1
Class=PropEditString
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STRING,edit,1352728644

[CLS:PropEditString]
Type=0
HeaderFile=PropEditString.h
ImplementationFile=PropEditString.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_STRING

[CLS:PropEditOrientation]
Type=0
HeaderFile=PropEditOrientation.h
ImplementationFile=PropEditOrientation.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_CUSTOM1

[DLG:IDD_PROP_EDIT_ORIENTATION]
Type=1
Class=PropEditOrientation
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_YAW,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_SPIN1,msctls_updown32,1342177312
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_OFFSET_PARAMS]
Type=1
Class=OffsetParams
ControlCount=7
Control1=IDC_X,edit,1350631552
Control2=IDC_Z,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[CLS:OffsetParams]
Type=0
HeaderFile=OffsetParams.h
ImplementationFile=OffsetParams.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_X
VirtualFilter=dWC

[DLG:IDD_PAINT_PARAMETERS]
Type=1
Class=PaintParametersView
ControlCount=5
Control1=IDC_PAINT_RADIUS,edit,1350631552
Control2=IDC_STATIC,static,1342308352
Control3=IDC_PALETTE,static,1342177287
Control4=IDC_SETRADIUS,button,1342242816
Control5=IDC_STATIC,static,1342308352

[CLS:PaintParametersView]
Type=0
HeaderFile=PaintParametersView.h
ImplementationFile=PaintParametersView.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_PAINT_RADIUS
VirtualFilter=idWC

[CLS:CSurfaceTypePalette]
Type=0
HeaderFile=SurfaceTypePalette.h
ImplementationFile=SurfaceTypePalette.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=CSurfaceTypePalette

[CLS:PolyProperties]
Type=0
HeaderFile=PolyProperties.h
ImplementationFile=PolyProperties.cpp
BaseClass=CDialog
Filter=D
LastObject=65535

[DLG:IDD_POLYPROPERTIES]
Type=1
Class=PolygonFormView
ControlCount=4
Control1=IDC_CLOSED,button,1342242819
Control2=IDC_APPLYPOLYPROPERTIES,button,1342242817
Control3=IDC_PROPERTIES,SysListView32,1350631429
Control4=65535,static,1342308352

[CLS:PolygonFormView]
Type=0
HeaderFile=PolygonFormView.h
ImplementationFile=PolygonFormView.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=65535

[DLG:IDD_PROP_EDIT_MULTILINE_STRING]
Type=1
Class=PropEditMultiLineString
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STRING,edit,1352732740

[CLS:PropEditMultiLineString]
Type=0
HeaderFile=PropEditMultiLineString.h
ImplementationFile=PropEditMultiLineString.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_STRING

