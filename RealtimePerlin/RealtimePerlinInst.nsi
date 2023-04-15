!define MUI_PRODUCT "Realtime Perlin Noise"
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

OutFile "RealtimePerlinInst.exe"

LicenseData "License.txt"

LangString DESC_SecCopyUI ${LANG_ENGLISH} "The main program and data files."
LangString DESC_SecSourceCopyUI ${LANG_ENGLISH} "The program source code (with out EXE and data files)."

InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"

!insertmacro MUI_SYSTEM

Section "Program" SecCopyUI
  SetOutPath "$INSTDIR"

  File "font.dll"
  File "font.tga"
  File "readme.txt"
  File "RealtimePerlin.exe"
  File "tga.dll"
  File "transfer0.tga"
  File "transfer1.tga"
  File "transfer2.tga"
  File "transfer3.tga"
  File "transfer4.tga"
  File "transfer5.tga"
  File "transfer6.tga"
  File "transfer7.tga"
  File "transfer8.tga"
  File "transfer9.tga"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN
    CreateDirectory "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Realtime Perlin Noise.lnk" "$INSTDIR\RealtimePerlin.exe"
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
  File "noise.c"
  File "noise.h"
  File "overlay.c"
  File "overlay.h"
  File "realtimeperlin.c"
  File "RealtimePerlin.dsp"
  File "RealtimePerlin.dsw"
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
  Delete "$INSTDIR\font.dll"
  Delete "$INSTDIR\font.h"
  Delete "$INSTDIR\font.lib"
  Delete "$INSTDIR\font.tga"
  Delete "$INSTDIR\glati.h"
  Delete "$INSTDIR\noise.c"
  Delete "$INSTDIR\noise.h"
  Delete "$INSTDIR\overlay.c"
  Delete "$INSTDIR\overlay.h"
  Delete "$INSTDIR\readme.txt"
  Delete "$INSTDIR\realtimeperlin.c"
  Delete "$INSTDIR\RealtimePerlin.dsp"
  Delete "$INSTDIR\RealtimePerlin.dsw"
  Delete "$INSTDIR\RealtimePerlin.exe"
  Delete "$INSTDIR\tga.dll"
  Delete "$INSTDIR\tga.h"
  Delete "$INSTDIR\tga.lib"
  Delete "$INSTDIR\transfer0.tga"
  Delete "$INSTDIR\transfer1.tga"
  Delete "$INSTDIR\transfer2.tga"
  Delete "$INSTDIR\transfer3.tga"
  Delete "$INSTDIR\transfer4.tga"
  Delete "$INSTDIR\transfer5.tga"
  Delete "$INSTDIR\transfer6.tga"
  Delete "$INSTDIR\transfer7.tga"
  Delete "$INSTDIR\transfer8.tga"
  Delete "$INSTDIR\transfer9.tga"
  Delete "$INSTDIR\Uninstall.exe"

  ReadRegStr ${TEMP1} HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  StrCmp ${TEMP1} "" noshortcuts
    Delete "$SMPROGRAMS\${TEMP1}\Realtime Perlin Noise.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${TEMP1}"

noshortcuts:
  RMDir "$INSTDIR"

  DeleteRegValue HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  !insertmacro MUI_UNFINISHHEADER
SectionEnd