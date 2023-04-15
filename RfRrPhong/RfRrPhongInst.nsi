!define MUI_PRODUCT "Reflect and Refraction (with Chromatic Aberration) and Full Phong lighting"
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

OutFile "RfRrPhongInst.exe"

LicenseData "License.txt"

LangString DESC_SecCopyUI ${LANG_ENGLISH} "The main program and data files."
LangString DESC_SecSourceCopyUI ${LANG_ENGLISH} "The program source code (with out EXE and data files)."

InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"

!insertmacro MUI_SYSTEM

Section "Program" SecCopyUI
  SetOutPath "$INSTDIR"

  File "BumpFloor.tga"
  File "BumpTile.tga"
  File "Engine.exe"
  File "Floor.tga"
  File "Font.tga"
  File "Generic.tvp"
  File "Phong.tfp"
  File "readme.txt"
  File "ReflRefr.tfp"
  File "Room.3ds"
  File "Sphere.3ds"
  File "Teapot.3DS"
  File "Tile.tga"
  File "White.tga"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN
    CreateDirectory "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Reflect and Refraction (with Chromatic Aberration) and Full Phong lighting.lnk" "$INSTDIR\Engine.exe"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

    WriteRegStr HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder" "${MUI_STARTMENU_VARIABLE}"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Source code" SecSourceCopyUI
  SetOutPath "$INSTDIR"

  File "3ds.c"
  File "3ds.h"
  File "camera.c"
  File "camera.h"
  File "Engine.c"
  File "Engine.dsp"
  File "Engine.dsw"
  File "font.c"
  File "font.h"
  File "frustum.c"
  File "frustum.h"
  File "glati.h"
  File "image.c"
  File "image.h"
  File "jpeg.c"
  File "overlay.c"
  File "overlay.h"
  File "quat.c"
  File "quat.h"
  File "tga.c"
  File "wglati.h"

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

!insertmacro MUI_SECTIONS_FINISHHEADER

!insertmacro MUI_FUNCTIONS_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCopyUI} $(DESC_SecCopyUI)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecSourceCopyUI} $(DESC_SecSourceCopyUI)
!insertmacro MUI_FUNCTIONS_DESCRIPTION_END

Section "Uninstall"
  Delete "$INSTDIR\3ds.c"
  Delete "$INSTDIR\3ds.h"
  Delete "$INSTDIR\BumpFloor.tga"
  Delete "$INSTDIR\BumpTile.tga"
  Delete "$INSTDIR\camera.c"
  Delete "$INSTDIR\camera.h"
  Delete "$INSTDIR\Engine.c"
  Delete "$INSTDIR\Engine.dsp"
  Delete "$INSTDIR\Engine.dsw"
  Delete "$INSTDIR\Engine.exe"
  Delete "$INSTDIR\Floor.tga"
  Delete "$INSTDIR\font.c"
  Delete "$INSTDIR\font.h"
  Delete "$INSTDIR\Font.tga"
  Delete "$INSTDIR\frustum.c"
  Delete "$INSTDIR\frustum.h"
  Delete "$INSTDIR\Generic.tvp"
  Delete "$INSTDIR\glati.h"
  Delete "$INSTDIR\image.c"
  Delete "$INSTDIR\image.h"
  Delete "$INSTDIR\jpeg.c"
  Delete "$INSTDIR\overlay.c"
  Delete "$INSTDIR\overlay.h"
  Delete "$INSTDIR\Phong.tfp"
  Delete "$INSTDIR\quat.c"
  Delete "$INSTDIR\quat.h"
  Delete "$INSTDIR\readme.txt"
  Delete "$INSTDIR\ReflRefr.tfp"
  Delete "$INSTDIR\Room.3ds"
  Delete "$INSTDIR\Sphere.3ds"
  Delete "$INSTDIR\Teapot.3DS"
  Delete "$INSTDIR\tga.c"
  Delete "$INSTDIR\Tile.tga"
  Delete "$INSTDIR\wglati.h"
  Delete "$INSTDIR\White.tga"
  Delete "$INSTDIR\Uninstall.exe"

  ReadRegStr ${TEMP1} HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  StrCmp ${TEMP1} "" noshortcuts
    Delete "$SMPROGRAMS\${TEMP1}\Reflect and Refraction (with Chromatic Aberration) and Full Phong lighting.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${TEMP1}"

noshortcuts:
  RMDir "$INSTDIR"

  DeleteRegValue HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  !insertmacro MUI_UNFINISHHEADER
SectionEnd