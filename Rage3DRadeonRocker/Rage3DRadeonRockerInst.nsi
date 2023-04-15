Name "Rage3D Radeon Rocker Vis Plug-in"

OutFile "Rage3DRadeonRockerInst.exe"

InstallDir $PROGRAMFILES\Winamp
InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Winamp" "UninstallString"

DirText "Please select your Winamp path below (you will be able to proceed when Winamp is detected):"
DirShow hide

AutoCloseWindow true
ShowInstDetails nevershow

Function .onVerifyInstDir
!ifndef WINAMP_AUTOINSTALL
  IfFileExists $INSTDIR\Winamp.exe Good
    Abort
  Good:
!endif
FunctionEnd

Function QueryWinampVisPath
  StrCpy $1 $INSTDIR\Plugins

  ReadINIStr $9 $INSTDIR\winamp.ini Winamp VisDir 
  StrCmp $9 "" End
  IfFileExists $9 0 End
    StrCpy $1 $9 ; update dir
  End: 
FunctionEnd

!ifdef WINAMP_AUTOINSTALL
Function GetWinampInstPath
  Push $0
  Push $1
  Push $2
  ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Winamp" "UninstallString"
  StrCmp $0 "" fin
    StrCpy $1 $0 1 0 ; get firstchar
    StrCmp $1 '"' "" getparent 
      StrCpy $0 $0 "" 1
      StrCpy $1 0
      rqloop:
        StrCpy $2 $0 1 $1
        StrCmp $2 '"' rqdone
        StrCmp $2 "" rqdone
        IntOp $1 $1 + 1
        Goto rqloop
      rqdone:
      StrCpy $0 $0 $1
    getparent:
    StrCpy $1 -1
    gploop:
      StrCpy $2 $0 1 $1
      StrCmp $2 "" gpexit
      StrCmp $2 "\" gpexit
      IntOp $1 $1 - 1
      Goto gploop
    gpexit:
    StrCpy $0 $0 $1

    StrCmp $0 "" fin
    IfFileExists $0\winamp.exe fin
      StrCpy $0 ""
  fin:
  Pop $2
  Pop $1
  Exch $0
FunctionEnd

Function MakeSureIGotWinamp
  Call GetWinampInstPath
  Pop $0
  StrCmp $0 "" getwinamp
    Return
  getwinamp:
  StrCpy $1 $TEMP\porearre1.dll 
  StrCpy $2 "$TEMP\Winamp Installer.exe"
  File /oname=$1 nsisdl.dll
  Push http://download.nullsoft.com/winamp/client/winamp281_lite.exe
  Push $2
  CallInstDLL $1 download
  Delete $1
  StrCmp $0 success success
    SetDetailsView show
    DetailPrint "download failed: $0"
    Abort
  success:
    ExecWait '"$2" /S'
    Delete $2
    Call GetWinampInstPath
    Pop $0
    StrCmp $0 "" skip
    StrCpy $INSTDIR $0
  skip:
FunctionEnd

!endif ; WINAMP_AUTOINSTALL
Section "ThisNameIsIgnoredSoWhyBother?"
!ifdef WINAMP_AUTOINSTALL
  Call MakeSureIGotWinamp
!endif

  Call QueryWinampVisPath
  SetOutPath $1

  File "vis_Rage3DRadeonRocker.dll"
  File "background.tga"
  File "base.tga"
  File "bump.tga"
  File "envmap.tga"

  MessageBox MB_YESNO|MB_ICONQUESTION "The plug-in was installed. Would you like to run Winamp now with Rage3D Radeon Rocker as the default plug-in?" IDNO NoWinamp
    WriteINIStr "$INSTDIR\Winamp.ini" "Winamp" "visplugin_name" "vis_Rage3DRadeonRocker.dll"
    WriteINIStr "$INSTDIR\Winamp.ini" "Winamp" "visplugin_num" "0"
    Exec '"$INSTDIR\Winamp.exe"'
  NoWinamp:
SectionEnd
