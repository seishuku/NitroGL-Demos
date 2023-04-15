!define MUI_PRODUCT "Deferred Rendering"
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

OutFile "DeferredInst.exe"

LicenseData "License.txt"

LangString DESC_SecCopyUI ${LANG_ENGLISH} "The main program and data files."
LangString DESC_SecSourceCopyUI ${LANG_ENGLISH} "The program source code (with out EXE and data files)."

InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"

!insertmacro MUI_SYSTEM

Section "Program" SecCopyUI
  SetOutPath "$INSTDIR"

  File "Composite.tfp"
  File "Deferred.exe"
  File "Elephant.3ds"
  File "Fieldstone.tga"
  File "Fieldstone_b.tga"
  File "Generic.tvp"
  File "Grace.tga"
  File "Level.3ds"
  File "MRT.tfp"
  File "Readme.txt"
  File "Wood.tga"
  File "Wood_b.tga"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN
    CreateDirectory "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Deferred Rendering.lnk" "$INSTDIR\Deferred.exe"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

    WriteRegStr HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder" "${MUI_STARTMENU_VARIABLE}"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Source code" SecSourceCopyUI
  SetOutPath "$INSTDIR"

  File "3ds.c"
  File "3ds.h"
  File "dds.c"
  File "Deferred.dsp"
  File "Deferred.dsw"
  File "engine.c"
  File "font.c"
  File "font.h"
  File "hdr.c"
  File "image.c"
  File "image.h"
  File "index_t.c"
  File "index_t.h"
  File "math.c"
  File "math.h"
  File "opengl.c"
  File "opengl.h"
  File "overlay.c"
  File "overlay.h"
  File "tga.c"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN
    CreateDirectory "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Source Code.lnk" "$INSTDIR\"

    WriteRegStr HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder" "${MUI_STARTMENU_VARIABLE}"
  !insertmacro MUI_STARTMENU_WRITE_END

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
  Delete "$INSTDIR\Composite.tfp"
  Delete "$INSTDIR\dds.c"
  Delete "$INSTDIR\Deferred.dsp"
  Delete "$INSTDIR\Deferred.dsw"
  Delete "$INSTDIR\Deferred.exe"
  Delete "$INSTDIR\Elephant.3ds"
  Delete "$INSTDIR\engine.c"
  Delete "$INSTDIR\Fieldstone.tga"
  Delete "$INSTDIR\Fieldstone_b.tga"
  Delete "$INSTDIR\font.c"
  Delete "$INSTDIR\font.h"
  Delete "$INSTDIR\Generic.tvp"
  Delete "$INSTDIR\Grace.tga"
  Delete "$INSTDIR\hdr.c"
  Delete "$INSTDIR\image.c"
  Delete "$INSTDIR\image.h"
  Delete "$INSTDIR\index_t.c"
  Delete "$INSTDIR\index_t.h"
  Delete "$INSTDIR\Level.3ds"
  Delete "$INSTDIR\math.c"
  Delete "$INSTDIR\math.h"
  Delete "$INSTDIR\MRT.tfp"
  Delete "$INSTDIR\opengl.c"
  Delete "$INSTDIR\opengl.h"
  Delete "$INSTDIR\overlay.c"
  Delete "$INSTDIR\overlay.h"
  Delete "$INSTDIR\Readme.txt"
  Delete "$INSTDIR\tga.c"
  Delete "$INSTDIR\Wood.tga"
  Delete "$INSTDIR\Wood_b.tga"
  Delete "$INSTDIR\Uninstall.exe"

  ReadRegStr ${TEMP1} HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  StrCmp ${TEMP1} "" noshortcuts
    Delete "$SMPROGRAMS\${TEMP1}\Deferred Rendering.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Source Code.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${TEMP1}"

noshortcuts:
  RMDir "$INSTDIR"

  DeleteRegValue HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  !insertmacro MUI_UNFINISHHEADER
SectionEnd