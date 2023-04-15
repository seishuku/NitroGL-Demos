!define MUI_PRODUCT "3D Texture"
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

OutFile "3DTextureInst.exe"

LicenseData "License.txt"

LangString DESC_SecCopyUI ${LANG_ENGLISH} "The main program and data files."
LangString DESC_SecSourceCopyUI ${LANG_ENGLISH} "The program source code (with out EXE and data files)."

InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"

!insertmacro MUI_SYSTEM

Section "Program" SecCopyUI
  SetOutPath "$INSTDIR"

  File "3DTexture.exe"
  File "font.dll"
  File "font.tga"
  File "readme.txt"
  File "tga.dll"
  File "vol.raw"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN
    CreateDirectory "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\3D Texture.lnk" "$INSTDIR\3DTexture.exe"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

    WriteRegStr HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder" "${MUI_STARTMENU_VARIABLE}"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Source code" SecSourceCopyUI
  SetOutPath "$INSTDIR"

  File "3DTexture.c"
  File "3DTexture.dsp"
  File "3DTexture.dsw"
  File "font.h"
  File "font.lib"
  File "glati.h"
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
  Delete "$INSTDIR\3DTexture.c"
  Delete "$INSTDIR\3DTexture.dsp"
  Delete "$INSTDIR\3DTexture.dsw"
  Delete "$INSTDIR\3DTexture.exe"
  Delete "$INSTDIR\font.dll"
  Delete "$INSTDIR\font.h"
  Delete "$INSTDIR\font.lib"
  Delete "$INSTDIR\font.tga"
  Delete "$INSTDIR\glati.h"
  Delete "$INSTDIR\overlay.c"
  Delete "$INSTDIR\overlay.h"
  Delete "$INSTDIR\readme.txt"
  Delete "$INSTDIR\tga.dll"
  Delete "$INSTDIR\tga.h"
  Delete "$INSTDIR\tga.lib"
  Delete "$INSTDIR\vol.raw"
  Delete "$INSTDIR\Uninstall.exe"

  ReadRegStr ${TEMP1} HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  StrCmp ${TEMP1} "" noshortcuts
    Delete "$SMPROGRAMS\${TEMP1}\3D Texture.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${TEMP1}"

noshortcuts:
  RMDir "$INSTDIR"

  DeleteRegValue HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  !insertmacro MUI_UNFINISHHEADER
SectionEnd