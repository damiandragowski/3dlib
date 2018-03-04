# Microsoft Developer Studio Project File - Name="Environ3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Environ3D - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Environ3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Environ3D.mak" CFG="Environ3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Environ3D - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Environ3D - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Environ3D - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x415 /d "NDEBUG"
# ADD RSC /l 0x415 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Environ3D - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x415 /d "_DEBUG"
# ADD RSC /l 0x415 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Environ3D - Win32 Release"
# Name "Environ3D - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Environ3D.cpp
# End Source File
# Begin Source File

SOURCE=.\MainWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDialogBox.cpp
# End Source File
# Begin Source File

SOURCE=.\resoruces.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MainWindow.h
# End Source File
# Begin Source File

SOURCE=.\ParamDialogBox.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\mainicon.ico
# End Source File
# End Group
# Begin Group "3DLib"

# PROP Default_Filter ""
# Begin Group "Source Files No. 1"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\3DLib\Bezier3D.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\BezierPatchC0.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\BSpline.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\BSplinePatch.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\Cursor3D.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\Interpol3D.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\Matrix3D.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\Mesh3D.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\Object3D.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\Point3D.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\Sphere3D.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\Torus3D.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLib\Vector3D.cpp
# End Source File
# End Group
# Begin Group "Header Files No. 2"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\3DLib\Bezier3D.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\BezierPatchC0.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\BSpline.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\BSplinePatch.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\Cursor3D.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\Interpol3D.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\Matrix3D.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\Mesh3D.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\Object3D.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\Point3D.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\Sphere3D.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\Torus3D.h
# End Source File
# Begin Source File

SOURCE=.\3DLib\Vector3D.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
