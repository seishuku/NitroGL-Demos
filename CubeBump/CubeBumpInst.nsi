!define MUI_PRODUCT "Cubic Enviroment Bump Mapping"
!define MUI_VERSION ""

!include "${NSISDIR}\Contrib\Modern UI\System.nsh"

!define MUI_LICENSEPAGE
!define MUI_COMPONENTSPAGE
!define MUI_DIRECTORYPAGE
!define MUI_STARTMENUPAGE

!define MUI_ABORTWARNING

!define MUI_UNINSTALLER
!define MUI_UNCONFIRMPAGE

!define TEMP1 $R0

!insertmacro MUI_LANGUAGE "English"

OutFile "CubeBumpInst.exe"

LicenseData "License.txt"

LangString DESC_SecCopyUI ${LANG_ENGLISH} "The main program and data files."
LangString DESC_SecSourceCopyUI ${LANG_ENGLISH} "The program source code (with out EXE and data files)."

InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"

!insertmacro MUI_SYSTEM

Section "Program" SecCopyUI
  SetOutPath "$INSTDIR"

  File "angel.obj"
  File "AngelBase.tga"
  File "AngelBump.tga"
  File "back.tga"
  File "bottom.tga"
  File "chalice.obj"
  File "ChaliceBase.tga"
  File "ChaliceBump.tga"
  File "CrumpleBase.tga"
  File "CrumpleBump.tga"
  File "cube.obj"
  File "Engine.exe"
  File "font.tga"
  File "front.tga"
  File "left.tga"
  File "NitroGLBase.tga"
  File "NitroGLBump.tga"
  File "quad.obj"
  File "readme.txt"
  File "right.tga"
  File "skull.obj"
  File "sphere.obj"
  File "tga.dll"
  File "top.tga"
  File "torus.obj"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN
    CreateDirectory "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Cubic Enviroment Bump Mapping.lnk" "$INSTDIR\Engine.exe"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

    WriteRegStr HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder" "${MUI_STARTMENU_VARIABLE}"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Source code" SecSourceCopyUI
  SetOutPath "$INSTDIR"

  File "Engine.c"
  File "Engine.dsp"
  File "Engine.dsw"
  File "font.c"
  File "font.h"
  File "glati.h"
  File "obj.c"
  File "obj.h"
  File "overlay.c"
  File "overlay.h"
  File "tga.h"
  File "tga.lib"
  File "wglati.h"

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

!insertmacro MUI_SECTIONS_FINISHHEADER

!insertmacro MUI_FUNCTIONS_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCopyUI} $(DESC_SecCopyUI)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecSourceCopyUI} $(DESC_SecSourceCopyUI)
!insertmacro MUI_FUNCTIONS_DESCRIPTION_END

Section "Uninstall"
  Delete "$INSTDIR\angel.obj"
  Delete "$INSTDIR\AngelBase.tga"
  Delete "$INSTDIR\AngelBump.tga"
  Delete "$INSTDIR\back.tga"
  Delete "$INSTDIR\bottom.tga"
  Delete "$INSTDIR\chalice.obj"
  Delete "$INSTDIR\ChaliceBase.tga"
  Delete "$INSTDIR\ChaliceBump.tga"
  Delete "$INSTDIR\CrumpleBase.tga"
  Delete "$INSTDIR\CrumpleBump.tga"
  Delete "$INSTDIR\cube.obj"
  Delete "$INSTDIR\CubeBumpInst.nsi"
  Delete "$INSTDIR\Engine.c"
  Delete "$INSTDIR\Engine.dsp"
  Delete "$INSTDIR\Engine.dsw"
  Delete "$INSTDIR\Engine.exe"
  Delete "$INSTDIR\font.c"
  Delete "$INSTDIR\font.h"
  Delete "$INSTDIR\font.tga"
  Delete "$INSTDIR\front.tga"
  Delete "$INSTDIR\glati.h"
  Delete "$INSTDIR\left.tga"
  Delete "$INSTDIR\License.txt"
  Delete "$INSTDIR\list.txt"
  Delete "$INSTDIR\NitroGLBase.tga"
  Delete "$INSTDIR\NitroGLBump.tga"
  Delete "$INSTDIR\obj.c"
  Delete "$INSTDIR\obj.h"
  Delete "$INSTDIR\overlay.c"
  Delete "$INSTDIR\overlay.h"
  Delete "$INSTDIR\quad.obj"
  Delete "$INSTDIR\readme.txt"
  Delete "$INSTDIR\right.tga"
  Delete "$INSTDIR\skull.obj"
  Delete "$INSTDIR\sphere.obj"
  Delete "$INSTDIR\tga.dll"
  Delete "$INSTDIR\tga.h"
  Delete "$INSTDIR\tga.lib"
  Delete "$INSTDIR\top.tga"
  Delete "$INSTDIR\torus.obj"
  Delete "$INSTDIR\wglati.h"
  Delete "$INSTDIR\Uninstall.exe"

  ReadRegStr ${TEMP1} HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  StrCmp ${TEMP1} "" noshortcuts
    Delete "$SMPROGRAMS\${TEMP1}\Cubic Enviroment Bump Mapping.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${TEMP1}"

noshortcuts:
  RMDir "$INSTDIR"

  DeleteRegValue HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  !insertmacro MUI_UNFINISHHEADER
SectionEnd