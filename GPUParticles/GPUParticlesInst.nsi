!include "MUI2.nsh"

Name "NitroGL GPU Particle System Demo"

OutFile "GPUParticlesInst.exe"

InstallDir "$PROGRAMFILES64\NitroGL GPU Particle System Demo"
InstallDirRegKey HKCU "Software\GPUParticles" ""

RequestExecutionLevel admin

Var StartMenuFolder

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_LICENSE "License.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\NitroGL GPU Particle System Demo"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Section "Program Files" SecProgram
  SetOutPath "$INSTDIR"

  File "Engine.exe"
  File "License.txt"
  File "nitrogl.3ds"
  File "particle.tga"
  File "particlesys_f.glsl"
  File "particlesys_v.glsl"
  File "particle_f.glsl"
  File "particle_g.glsl"
  File "particle_v.glsl"
  File "Readme.txt"

  ExecShell open "Readme.txt"

  WriteRegStr HKCU "Software\GPUParticles" "" $INSTDIR

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\GPU Particles.lnk" "$INSTDIR\Engine.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Readme.lnk" "$INSTDIR\Readme.txt"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Source Code" SecSource
  SetOutPath "$INSTDIR"

  File "3ds.c"
  File "3ds.h"
  File "dds.c"
  File "engine.c"
  File "Engine.sln"
  File "Engine.vcxproj"
  File "Engine.vcxproj.filters"
  File "font.c"
  File "font.h"
  File "fontdata.h"
  File "image.c"
  File "image.h"
  File "License.txt"
  File "Makefile"
  File "math.c"
  File "math.h"
  File "opengl.c"
  File "opengl.h"
  File "tga.c"

  WriteRegStr HKCU "Software\GPUParticles" "" $INSTDIR

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Source Code.lnk" "$INSTDIR"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

LangString DESC_SecProgram ${LANG_ENGLISH} "The main program"
LangString DESC_SecSource ${LANG_ENGLISH} "The program source code"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecProgram} $(DESC_SecProgram)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecSource} $(DESC_SecSource)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"
  Delete "$INSTDIR\3ds.c"
  Delete "$INSTDIR\3ds.h"
  Delete "$INSTDIR\dds.c"
  Delete "$INSTDIR\engine.c"
  Delete "$INSTDIR\Engine.exe"
  Delete "$INSTDIR\Engine.sln"
  Delete "$INSTDIR\Engine.vcxproj"
  Delete "$INSTDIR\Engine.vcxproj.filters"
  Delete "$INSTDIR\font.c"
  Delete "$INSTDIR\font.h"
  Delete "$INSTDIR\fontdata.h"
  Delete "$INSTDIR\image.c"
  Delete "$INSTDIR\image.h"
  Delete "$INSTDIR\License.txt"
  Delete "$INSTDIR\Makefile"
  Delete "$INSTDIR\math.c"
  Delete "$INSTDIR\math.h"
  Delete "$INSTDIR\nitrogl.3ds"
  Delete "$INSTDIR\opengl.c"
  Delete "$INSTDIR\opengl.h"
  Delete "$INSTDIR\particle.tga"
  Delete "$INSTDIR\particlesys_f.glsl"
  Delete "$INSTDIR\particlesys_v.glsl"
  Delete "$INSTDIR\particle_f.glsl"
  Delete "$INSTDIR\particle_g.glsl"
  Delete "$INSTDIR\particle_v.glsl"
  Delete "$INSTDIR\Readme.txt"
  Delete "$INSTDIR\tga.c"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder

  Delete "$SMPROGRAMS\$StartMenuFolder\GPU Particles.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Source Code.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Readme.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey /ifempty HKCU "Software\GPUParticles"
SectionEnd