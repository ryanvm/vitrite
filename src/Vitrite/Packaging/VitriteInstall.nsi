;
; This is the installation script for Vitrite. Hopefully, I won't screw up again and delete it.
;

Name "Vitrite"

; Filename of the produced installer
OutFile "VitriteInstall.exe"

; Default installation directory
InstallDir "$PROGRAMFILES\Tiny Utilities\Vitrite"

; Make sure the installer isn't corrupt
CRCCheck on

LicenseText "You must agree to this license before installing Vitrite:"
LicenseData "instgpl.txt"

; Use the Windows' colorscheme
InstallColors /windows

; Fancy up our progress bar
InstProgressFlags smooth

DirShow show 	; (make this hide to not let the user change it)
DirText "Select the directory to install Vitrite into:"

UninstallText "This will uninstall Vitrite from your system"

Section "" ; (default section)
  MessageBox MB_ICONEXCLAMATION|MB_OKCANCEL "Warning: If you are already running a version of Vitrite, you need to exit the application and uninstall it before installing a newer version." IDCANCEL Cancelled
  SetOutPath "$INSTDIR"

  ; Copy files
  File "Vitrite.exe"
  File "VitriDLL.dll"
  File "ReadMe.txt"
  File "GPL.txt"

  ; Write registry entries
  WriteRegStr HKEY_LOCAL_MACHINE "SOFTWARE\Tiny Utilities\Vitrite" "" "$INSTDIR"
  WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vitrite" "DisplayName" "Vitrite (remove only)"
  WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Vitrite" "UninstallString" '"$INSTDIR\uninst.exe"'

  ; Create Start Menu shortcuts
  CreateDirectory "$SMPROGRAMS\Tiny Utilities\Vitrite"
  CreateShortCut "$SMPROGRAMS\Tiny Utilities\Vitrite\Vitrite.lnk" "$INSTDIR\Vitrite.exe" 
  CreateShortCut "$SMPROGRAMS\Tiny Utilities\Vitrite\Uninstall.lnk" "$INSTDIR\uninst.exe"

  ; Ask about running on startup
  MessageBox MB_ICONQUESTION|MB_YESNO "Do you want Vitrite to run when Windows starts?" IDNO NoStartup
  CreateShortCut "$SMSTARTUP\Vitrite.lnk" "$INSTDIR\Vitrite.exe"
  NoStartup:

  ; Ask about running Vitrite now
  MessageBox MB_ICONQUESTION|MB_YESNO "Do you want to run Vitrite now?" IDNO NoRun
  Exec "$INSTDIR\Vitrite.exe"
  NoRun:

  ; Ask about viewing the ReadMe
  MessageBox MB_ICONQUESTION|MB_YESNO "Do you want to view the ReadMe now?" IDNO NoView
  Exec "$WINDIR\notepad.exe $INSTDIR\ReadMe.txt"
  NoView:

  ; Create uninstaller
  WriteUninstaller "$INSTDIR\uninst.exe"
  
  Goto End
  
  ; Bailout point
  Cancelled:
  Quit

  End:
SectionEnd ; end of default section

Section Uninstall
  Delete "$INSTDIR\*"
  Delete "$INSTDIR\uninst.exe"
  Delete "$SMPROGRAMS\Tiny Utilities\Vitrite\*"
  Delete "$SMSTARTUP\Vitrite.lnk"
  RMDir "$SMPROGRAMS\Tiny Utilities\Vitrite"
  IfFileExists "$SMPROGRAMS\Tiny Utilities\*\" DontDelete
  RMDIR "$SMPROGRAMS\Tiny Utilities"
  DontDelete:

  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Tiny Utilities\Vitrite"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Vitrite"
  RMDir "$INSTDIR"
SectionEnd ; end of uninstall section

