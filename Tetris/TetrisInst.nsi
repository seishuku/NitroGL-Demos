!define MUI_PRODUCT "Tetris"
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

OutFile "TetrisInst.exe"

LicenseData "License.txt"

LangString DESC_SecCopyUI ${LANG_ENGLISH} "The main program and data files"
LangString DESC_SecSourceCopyUI ${LANG_ENGLISH} "Source code (no data files or EXE)"
LangString DESC_SecOpenALInstUI ${LANG_ENGLISH} "Installs OpenAL"

InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"

!insertmacro MUI_SYSTEM

Section "Program" SecCopyUI
  SetOutPath "$INSTDIR"

  File "Blur.tfp"
  File "Border.3ds"
  File "Composite.tfp"
  File "Cube.3ds"
  File "Diffuse.tfp"
  File "Drop.wav"
  File "font.tga"
  File "galileo.scn"
  File "galileo.tga"
  File "galileo_s.tga"
  File "Generic.tvp"
  File "grace.scn"
  File "grace.tga"
  File "grace_s.tga"
  File "LevelUp.wav"
  File "Lighting.tfp"
  File "Line.wav"
  File "Line4.wav"
  File "Move.wav"
  File "Readme.txt"
  File "Reflect.tfp"
  File "rnl.scn"
  File "rnl.tga"
  File "rnl_s.tga"
  File "Rotate.wav"
  File "Sphere.3ds"
  File "stpeters.scn"
  File "stpeters.tga"
  File "stpeters_s.tga"
  File "Tetris.exe"
  File "uffizi.scn"
  File "uffizi.tga"
  File "uffizi_s.tga"

  ExecShell open "Readme.txt"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN
    CreateDirectory "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Tetris.lnk" "$INSTDIR\Tetris.exe"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Readme.lnk" "$INSTDIR\Readme.txt"
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
  File "Engine.c"
  File "font.c"
  File "font.h"
  File "Game.c"
  File "Game.h"
  File "image.c"
  File "image.h"
  File "index_t.c"
  File "index_t.h"
  File "ivorbiscodec.h"
  File "ivorbisfile.h"
  File "libvorbisidec.a"
  File "Makefile"
  File "math.c"
  File "math.h"
  File "ogg.h"
  File "opengl.c"
  File "opengl.h"
  File "os_types.h"
  File "overlay.c"
  File "overlay.h"
  File "Tetris.dsp"
  File "Tetris.dsw"
  File "tga.c"
  File "vorbisidec.lib"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN
    CreateDirectory "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Source Code.lnk" "$INSTDIR"
    CreateShortCut "$SMPROGRAMS\${MUI_STARTMENU_VARIABLE}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

    WriteRegStr HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder" "${MUI_STARTMENU_VARIABLE}"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "OpenAL" SecOpenALInstUI
  SetOutPath "$TEMP"

  File "OpenALwEAX.exe"
  Exec '$TEMP\OpenALwEAX.exe'
SectionEnd

!insertmacro MUI_SECTIONS_FINISHHEADER

!insertmacro MUI_FUNCTIONS_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCopyUI} $(DESC_SecCopyUI)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecSourceCopyUI} $(DESC_SecSourceCopyUI)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecOpenALInstUI} $(DESC_SecOpenALInstUI)
!insertmacro MUI_FUNCTIONS_DESCRIPTION_END

Section "Uninstall"
  Delete "$INSTDIR\3ds.c"
  Delete "$INSTDIR\3ds.h"
  Delete "$INSTDIR\Blur.tfp"
  Delete "$INSTDIR\Border.3ds"
  Delete "$INSTDIR\Composite.tfp"
  Delete "$INSTDIR\Cube.3ds"
  Delete "$INSTDIR\dds.c"
  Delete "$INSTDIR\Diffuse.tfp"
  Delete "$INSTDIR\Drop.wav"
  Delete "$INSTDIR\Engine.c"
  Delete "$INSTDIR\font.c"
  Delete "$INSTDIR\font.h"
  Delete "$INSTDIR\font.tga"
  Delete "$INSTDIR\galileo.scn"
  Delete "$INSTDIR\galileo.tga"
  Delete "$INSTDIR\galileo_s.tga"
  Delete "$INSTDIR\Game.c"
  Delete "$INSTDIR\Game.h"
  Delete "$INSTDIR\Generic.tvp"
  Delete "$INSTDIR\grace.scn"
  Delete "$INSTDIR\grace.tga"
  Delete "$INSTDIR\grace_s.tga"
  Delete "$INSTDIR\image.c"
  Delete "$INSTDIR\image.h"
  Delete "$INSTDIR\index_t.c"
  Delete "$INSTDIR\index_t.h"
  Delete "$INSTDIR\ivorbiscodec.h"
  Delete "$INSTDIR\ivorbisfile.h"
  Delete "$INSTDIR\LevelUp.wav"
  Delete "$INSTDIR\libvorbisidec.a"
  Delete "$INSTDIR\Lighting.tfp"
  Delete "$INSTDIR\Line.wav"
  Delete "$INSTDIR\Line4.wav"
  Delete "$INSTDIR\Makefile"
  Delete "$INSTDIR\math.c"
  Delete "$INSTDIR\math.h"
  Delete "$INSTDIR\Move.wav"
  Delete "$INSTDIR\ogg.h"
  Delete "$INSTDIR\opengl.c"
  Delete "$INSTDIR\opengl.h"
  Delete "$INSTDIR\os_types.h"
  Delete "$INSTDIR\overlay.c"
  Delete "$INSTDIR\overlay.h"
  Delete "$INSTDIR\Readme.txt"
  Delete "$INSTDIR\Reflect.tfp"
  Delete "$INSTDIR\rnl.scn"
  Delete "$INSTDIR\rnl.tga"
  Delete "$INSTDIR\rnl_s.tga"
  Delete "$INSTDIR\Rotate.wav"
  Delete "$INSTDIR\Sphere.3ds"
  Delete "$INSTDIR\stpeters.scn"
  Delete "$INSTDIR\stpeters.tga"
  Delete "$INSTDIR\stpeters_s.tga"
  Delete "$INSTDIR\Tetris.dsp"
  Delete "$INSTDIR\Tetris.dsw"
  Delete "$INSTDIR\Tetris.exe"
  Delete "$INSTDIR\tga.c"
  Delete "$INSTDIR\uffizi.scn"
  Delete "$INSTDIR\uffizi.tga"
  Delete "$INSTDIR\uffizi_s.tga"
  Delete "$INSTDIR\vorbisidec.lib"

  Delete "$INSTDIR\Uninstall.exe"

  ReadRegStr ${TEMP1} HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  StrCmp ${TEMP1} "" noshortcuts
    Delete "$SMPROGRAMS\${TEMP1}\Tetris.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Readme.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Source Code.lnk"
    Delete "$SMPROGRAMS\${TEMP1}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${TEMP1}"

noshortcuts:
  RMDir "$INSTDIR"

  DeleteRegValue HKCU "Software\${MUI_PRODUCT}" "Start Menu Folder"

  !insertmacro MUI_UNFINISHHEADER
SectionEnd