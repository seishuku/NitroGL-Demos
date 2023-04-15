!define MUI_PRODUCT "Vertex Blend"
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

OutFile "VertexBlendInst.exe"

LicenseData "License.txt"

LangString DESC_SecCopyUI ${LANG_ENGLISH} "The main program and data files."
LangString DESC_SecSourceCopyUI ${LANG_ENGLISH} "The program source code (with out EXE and data files)."

InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"

!insertmacro MUI_SYSTEM

Section "Program" SecCopyUI
  SetOutPath "$INSTDIR"

  File "base.tga"
  File "font.dll"
  File "font.tga"
  File "tga.dll"
  File "VertexBlend.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN
    CreateDirectory "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Vertex Blend.lnk" "$INSTDIR\VertexBlend.exe"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

    WriteRegStr HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder" "${MUI_STARTMENU_VARIABLE}"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Source code" SecSourceCopyUI
  SetOutPath "$INSTDIR"

  File "font.h"
  File "font.lib"
  File "glati.h"
  File "overlay.c"
  File "overlay.h"
  File "tga.h"
  File "tga.lib"
  File "vertexblend.c"
  File "VertexBlend.dsp"
  File "VertexBlend.dsw"

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

!insertmacro MUI_SECTIONS_FINISHHEADER

!insertmacro MUI_FUNCTIONS_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCopyUI} $(DESC_SecCopyUI)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecSourceCopyUI} $(DESC_SecSourceCopyUI)
!insertmacro MUI_FUNCTIONS_DESCRIPTION_END

Section "Uninstall"
  Delete "$INSTDIR\base.tga"
  Delete "$INSTDIR\font.dll"
  Delete "$INSTDIR\font.h"
  Delete "$INSTDIR\font.lib"
  Delete "$INSTDIR\font.tga"
  Delete "$INSTDIR\glati.h"
  Delete "$INSTDIR\overlay.c"
  Delete "$INSTDIR\overlay.h"
  Delete "$INSTDIR\tga.dll"
  Delete "$INSTDIR\tga.h"
  Delete "$INSTDIR\tga.lib"
  Delete "$INSTDIR\vertexblend.c"
  Delete "$INSTDIR\VertexBlend.dsp"
  Delete "$INSTDIR\VertexBlend.dsw"
  Delete "$INSTDIR\VertexBlend.exe"
  Delete "$INSTDIR\Uninstall.exe"

  ReadRegStr ${TEMP1} HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  StrCmp ${TEMP1} "" noshortcuts
    Delete "$SMPROGRAMS\${TEMP1}\Vertex Blend.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${TEMP1}"

noshortcuts:
  RMDir "$INSTDIR"

  DeleteRegValue HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  !insertmacro MUI_UNFINISHHEADER
SectionEnd