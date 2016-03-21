# Microsoft Developer Studio Project File - Name="terrain" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=terrain - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "shreditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "shreditor.mak" CFG="terrain - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "terrain - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "terrain - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "terrain - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /GR /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "terrain - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"Debug/shreditor.res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "terrain - Win32 Release"
# Name "terrain - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcavateDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\GeometryListView.cpp
# End Source File
# Begin Source File

SOURCE=.\GeometryPropertiesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\grid.cpp
# End Source File
# Begin Source File

SOURCE=.\GridElem.cpp
# End Source File
# Begin Source File

SOURCE=.\GridFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Object.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectElem.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\OffsetParams.cpp
# End Source File
# Begin Source File

SOURCE=.\onfirmDeleteDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintParametersView.cpp
# End Source File
# Begin Source File

SOURCE=.\PlantParametersDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\polygon.cpp
# End Source File
# Begin Source File

SOURCE=.\PolygonFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\PropEditInt.cpp
# End Source File
# Begin Source File

SOURCE=.\PropEditMultiLineString.cpp
# End Source File
# Begin Source File

SOURCE=.\PropEditOrientation.cpp
# End Source File
# Begin Source File

SOURCE=.\PropEditString.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyView.cpp
# End Source File
# Begin Source File

SOURCE=.\quadtree.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectByAttributeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Solid.cpp
# End Source File
# Begin Source File

SOURCE=.\SolidListView.cpp
# End Source File
# Begin Source File

SOURCE=.\SolidPropertiesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SurfaceTypePalette.cpp
# End Source File
# Begin Source File

SOURCE=.\terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\terrain.rc
# End Source File
# Begin Source File

SOURCE=.\terrainDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\terrainView.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolPalette.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolSheet.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Bitmap.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ExcavateDialog.h
# End Source File
# Begin Source File

SOURCE=.\geometry.h
# End Source File
# Begin Source File

SOURCE=.\GeometryListView.h
# End Source File
# Begin Source File

SOURCE=.\GeometryPropertiesDialog.h
# End Source File
# Begin Source File

SOURCE=.\grid.h
# End Source File
# Begin Source File

SOURCE=.\GridElem.h
# End Source File
# Begin Source File

SOURCE=.\GridFormView.h
# End Source File
# Begin Source File

SOURCE=.\GridView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Object.h
# End Source File
# Begin Source File

SOURCE=.\ObjectElem.h
# End Source File
# Begin Source File

SOURCE=.\ObjectFormView.h
# End Source File
# Begin Source File

SOURCE=.\ObjectProperties.h
# End Source File
# Begin Source File

SOURCE=.\OffsetParams.h
# End Source File
# Begin Source File

SOURCE=.\onfirmDeleteDialog.h
# End Source File
# Begin Source File

SOURCE=.\PaintParametersView.h
# End Source File
# Begin Source File

SOURCE=.\PlantParametersDialog.h
# End Source File
# Begin Source File

SOURCE=.\polygon.h
# End Source File
# Begin Source File

SOURCE=.\PolygonFormView.h
# End Source File
# Begin Source File

SOURCE=.\PropEditInt.h
# End Source File
# Begin Source File

SOURCE=.\PropEditMultiLineString.h
# End Source File
# Begin Source File

SOURCE=.\PropEditOrientation.h
# End Source File
# Begin Source File

SOURCE=.\PropEditString.h
# End Source File
# Begin Source File

SOURCE=.\PropertyView.h
# End Source File
# Begin Source File

SOURCE=.\quadtree.hpp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelectByAttributeDialog.h
# End Source File
# Begin Source File

SOURCE=.\solid.h
# End Source File
# Begin Source File

SOURCE=.\SolidListView.h
# End Source File
# Begin Source File

SOURCE=.\SolidPropertiesDialog.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SurfaceTypePalette.h
# End Source File
# Begin Source File

SOURCE=.\terrain.h
# End Source File
# Begin Source File

SOURCE=.\terrainDoc.h
# End Source File
# Begin Source File

SOURCE=.\terrainView.h
# End Source File
# Begin Source File

SOURCE=.\ToolPalette.h
# End Source File
# Begin Source File

SOURCE=.\ToolSheet.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\surffill.bmp
# End Source File
# Begin Source File

SOURCE=.\res\terrain.ico
# End Source File
# Begin Source File

SOURCE=.\res\terrain.rc2
# End Source File
# Begin Source File

SOURCE=.\res\terrainDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
