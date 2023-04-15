# Microsoft Developer Studio Project File - Name="OpenGLDrv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OpenGLDrv - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OpenGLDrv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OpenGLDrv.mak" CFG="OpenGLDrv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OpenGLDrv - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OpenGLDrv - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/UT420/OpenGLDrv/Src", HFMAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OpenGLDrv - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OpenGLDRV_EXPORTS" /YX /FD /c
# ADD CPP /nologo /Zp4 /MD /W4 /WX /vd0 /GX /O2 /Ob2 /I "..\..\Core\Inc" /I "..\..\Engine\Inc" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "UNICODE" /D "_UNICODE" /D ThisPackage=OpenGLDrv /Yu"OpenGLDrv.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ..\..\Core\Lib\Core.lib ..\..\Engine\Lib\Engine.lib user32.lib gdi32.lib /nologo /dll /incremental:yes /machine:I386 /out:"..\..\System\OpenGLDrv.dll"

!ELSEIF  "$(CFG)" == "OpenGLDrv - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OpenGLDRV_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MDd /W4 /WX /Gm /vd0 /GX /ZI /Od /I "..\..\Core\Inc" /I "..\..\Engine\Inc" /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "UNICODE" /D "_UNICODE" /D ThisPackage=OpenGLDrv /Yu"OpenGLDrv.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\Core\Lib\Core.lib ..\..\Engine\Lib\Engine.lib user32.lib gdi32.lib /nologo /dll /debug /machine:I386 /out:"..\..\System\OpenGLDrv.dll" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "OpenGLDrv - Win32 Release"
# Name "OpenGLDrv - Win32 Debug"
# Begin Group "Src"

# PROP Default_Filter "*.cpp;*.h"
# Begin Source File

SOURCE=..\..\Engine\Inc\AActor.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\AAmmo.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\ABrush.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\ACamera.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\ACarcass.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\AGameReplicationInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\AInventory.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\ALevelInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\AMover.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\APawn.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\APickup.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\APlayerPawn.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\APlayerReplicationInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\AWeapon.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\AZoneInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\Core.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\Engine.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\EngineClasses.h
# End Source File
# Begin Source File

SOURCE=.\OpenGL.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenGLDrv.cpp
# ADD CPP /Yc"OpenGLDrv.h"
# End Source File
# Begin Source File

SOURCE=.\OpenGLDrv.h
# End Source File
# Begin Source File

SOURCE=.\OpenGLFuncs.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UExporter.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\ULevelSummary.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnActor.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnAnim.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnArc.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnAudio.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnBits.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnBuild.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnCache.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnCamera.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnCId.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnClass.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnCoreNet.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnCorObj.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnDynBsp.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnGame.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnIn.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnLevel.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnMath.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnMem.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnMesh.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnModel.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnName.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnNames.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnObj.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnObjBas.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnObjVer.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnPlayer.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnPrim.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnReach.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnRender.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnRenderIterator.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnRenderIteratorSupport.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnRenDev.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnScript.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnScrTex.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnSkeletalMesh.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnStack.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnTemplate.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnTex.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnType.h
# End Source File
# Begin Source File

SOURCE=..\..\Engine\Inc\UnURL.h
# End Source File
# Begin Source File

SOURCE=..\..\Core\Inc\UnVcWin32.h
# End Source File
# End Group
# End Target
# End Project
