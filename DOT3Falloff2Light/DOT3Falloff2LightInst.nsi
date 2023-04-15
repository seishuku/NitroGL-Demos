!define MUI_PRODUCT "DOT3 Bump Mapping with 2 Lights"
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

OutFile "DOT3Falloff2LightInst.exe"

LicenseData "License.txt"

LangString DESC_SecCopyUI ${LANG_ENGLISH} "The main program and data files."
LangString DESC_SecSourceCopyUI ${LANG_ENGLISH} "The program source code (with out EXE and data files)."

InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"

!insertmacro MUI_SYSTEM

Section "Program" SecCopyUI
  SetOutPath "$INSTDIR"

  File "base.tga"
  File "bump.tga"
  File "DOT3Falloff2Light.exe"
  File "font.dll"
  File "font.tga"
  File "readme.txt"
  File "room.obj"
  File "tga.dll"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN
    CreateDirectory "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\DOT3 Bump Mapping with 2 Lights.lnk" "$INSTDIR\DOT3Falloff2Light.exe"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

    WriteRegStr HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder" "${MUI_STARTMENU_VARIABLE}"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Source code" SecSourceCopyUI
  SetOutPath "$INSTDIR"

  File "DOT3Falloff2Light.c"
  File "DOT3Falloff2Light.dsp"
  File "DOT3Falloff2Light.dsw"
  File "font.h"
  File "font.lib"
  File "glati.h"
  File "obj.c"
  File "obj.h"
  File "overlay.c"
  File "overlay.h"
  File "tga.h"
  File "tga.lib"

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

!insertmacro MUI_SECTIONS_FINISHHEADER

!insertmacro MUI_FUNCTIONS_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCopyUI} $(DESC_SecCopyUI)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecSourceCopyUI} $(DESC_SecSourceCopyUI)
!insertmacro MUI_FUNCTIONS_DESCRIPTION_END

Section "Uninstall"
  Delete "$INSTDIR\base.tga"
  Delete "$INSTDIR\bump.tga"
  Delete "$INSTDIR\DOT3Falloff2Light.c"
  Delete "$INSTDIR\DOT3Falloff2Light.dsp"
  Delete "$INSTDIR\DOT3Falloff2Light.dsw"
  Delete "$INSTDIR\DOT3Falloff2Light.exe"
  Delete "$INSTDIR\font.dll"
  Delete "$INSTDIR\font.h"
  Delete "$INSTDIR\font.lib"
  Delete "$INSTDIR\font.tga"
  Delete "$INSTDIR\glati.h"
  Delete "$INSTDIR\obj.c"
  Delete "$INSTDIR\obj.h"
  Delete "$INSTDIR\overlay.c"
  Delete "$INSTDIR\overlay.h"
  Delete "$INSTDIR\readme.txt"
  Delete "$INSTDIR\room.obj"
  Delete "$INSTDIR\tga.dll"
  Delete "$INSTDIR\tga.h"
  Delete "$INSTDIR\tga.lib"
  Delete "$INSTDIR\Uninstall.exe"

  ReadRegStr ${TEMP1} HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  StrCmp ${TEMP1} "" noshortcuts
    Delete "$SMPROGRAMS\${TEMP1}\DOT3 Bump Mapping with 2 Lights.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${TEMP1}"

noshortcuts:
  RMDir "$INSTDIR"

  DeleteRegValue HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  !insertmacro MUI_UNFINISHHEADER
SectionEnd