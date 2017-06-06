;Foreplayer install script
;Written by Bilge Theall

;--------------------------------
;Include Modern UI
!include "MUI2.nsh"

!define VERSION $%FOREPLAYER_VERSION%
!define APP_PATH "."

;--------------------------------
;General
CRCCheck force
XPStyle on
SetCompressor /FINAL /SOLID lzma

!ifndef QT_DIR
!define QT_DIR ${QTDIR}                       ; Qt Installation directory
!endif

!ifndef MINGW_DIR
!define MINGW_DIR ${MINGW}                    ; MinGW Installation directory
!endif

!ifndef V
!define V ${VERSION}                          ; Program version
!endif

!ifndef ARCH
!define ARCH ${ARCH}                          ; Architecture 32 or 64
!endif

!define P "Foreplayer"                             ; Program name
!define P_NORM "foreplayer"                        ; Program name (normalized)
!define PROGRAM "Foreplayer.exe"

!ifndef ROOT_DIR
!define ROOT_DIR "..\.."                      ; Program root directory
!endif

!ifndef BUILD_DIR
!define BUILD_DIR ${FOREPLAYER_BUILD_DIR}          ; Build dir
!endif

!define ADD_REMOVE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Foreplayer"
!define PRODUCT_REG_KEY "Foreplayer"

; ----------- Icon and Bitmap ---------
!define MUI_ICON logo.ico                 ; TODO: find suitable icon
;!define MUI_UNICON uninstall.ico             ; TODO: find suitable icon
!define MUI_HEADERIMAGE
	!define MUI_HEADERIMAGE_BITMAP logo.bmp
	!define MUI_HEADERIMAGE_UNBITMAP logo.bmp
!define MUI_HEADER_TRANSPARENT_TEXT

; ------------- Reserve Files ---------------------
!insertmacro MUI_RESERVEFILE_LANGDLL
    ;Name and file
    Name "Foreplayer${VERSION}"
    OutFile "Foreplayer${VERSION}-Setup.exe"

    ;Default installation folder
    InstallDir "$PROGRAMFILES\Foreplayer"

    ;Get installation folder from registry if available
    InstallDirRegKey HKCU "Software\Foreplayer${VERSION}" ""

    BrandingText /TRIMLEFT "${P_NORM}-${VERSION}"
    
    ;Request application privileges for Windows Vista
    RequestExecutionLevel admin

;--------------------------------
;Interface Settings

    !define MUI_ABORTWARNING
    ;------------- Language Selection Dialog Settings --------------
    !define MUI_LANGDLL_REGISTRY_ROOT "HKCU"
    !define MUI_LANGDLL_REGISTRY_KEY "Software\${P}\${V}"
    !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"
    
;--------------------------------
;Pages

    !insertmacro MUI_PAGE_WELCOME
    !insertmacro MUI_PAGE_LICENSE "LICENSE.GPL"
    !insertmacro MUI_PAGE_COMPONENTS
    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_INSTFILES
        ; These indented statements modify settings for MUI_PAGE_FINISH
        !define MUI_FINISHPAGE_NOAUTOCLOSE
        !define MUI_FINISHPAGE_RUN "$INSTDIR\${P_NORM}.exe"
        !define MUI_FINISHPAGE_RUN_CHECKED
        !define MUI_FINISHPAGE_RUN_TEXT "Run ${P}"
        !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
        !define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\readme.txt"
    !insertmacro MUI_PAGE_FINISH

;-------------- Uninstall Pages -------------
    !insertmacro MUI_UNPAGE_WELCOME
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES
        ; These indented statements modify settings for MUI_UNPAGE_FINISH
        !define MUI_UNFINISHPAGE_NOAUTOCLOSE
    !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages
    !insertmacro MUI_LANGUAGE "English"
    !insertmacro MUI_LANGUAGE "French"
    !insertmacro MUI_LANGUAGE "German"
    !insertmacro MUI_LANGUAGE "Spanish"
    !insertmacro MUI_LANGUAGE "SpanishInternational"
    !insertmacro MUI_LANGUAGE "SimpChinese"
    !insertmacro MUI_LANGUAGE "TradChinese"
    !insertmacro MUI_LANGUAGE "Japanese"
    !insertmacro MUI_LANGUAGE "Korean"
    !insertmacro MUI_LANGUAGE "Italian"
    !insertmacro MUI_LANGUAGE "Dutch"
    !insertmacro MUI_LANGUAGE "Danish"
    

;--------------------------------
;Uninstall
Function checkAlreadyInstalled
	; check for already installed instance
	ClearErrors
	ReadRegStr $R0 HKLM ${ADD_REMOVE} "DisplayVersion"
	StrCmp $R0 "" 0 +2
	Return
	MessageBox MB_YESNO|MB_ICONQUESTION "${P} version $R0 seems \
	to be installed on your system.$\nWould you like to \
	uninstall that version first?" IDYES UnInstall
	Return
	UnInstall:
        ClearErrors
        ReadRegStr $R0 HKLM "${ADD_REMOVE}" "UninstallString"
		DetailPrint "Uninstalling previously installed version"
        ExecWait '$R0 _?=$INSTDIR'
		IfErrors OnError 0
		Return
	OnError:
		MessageBox MB_OK|MB_ICONSTOP "Error while uninstalling \
		previously installed version. Please uninstall it manually \
		and start the installer again."
		Quit
FunctionEnd
;--------------------------------
;Installer Sections
Section "Core" core_section
    Call checkAlreadyInstalled

    CreateDirectory "$INSTDIR\iconengines"
    SetOutPath "$INSTDIR\iconengines"
    File "${APP_PATH}\iconengines\qsvgicon.dll"
    
    CreateDirectory "$INSTDIR\imageformats"
    SetOutPath "$INSTDIR\imageformats"
    File "${APP_PATH}\imageformats\*.dll"
    
    CreateDirectory "$INSTDIR\platforms"
    SetOutPath "$INSTDIR\platforms"
    File "${APP_PATH}\platforms\qwindows.dll"
    
    CreateDirectory "$INSTDIR\translations"
    SetOutPath "$INSTDIR\translations"
    File "${APP_PATH}\translations\qt_*.qm"
    
    CreateDirectory "$INSTDIR\ts"
    SetOutPath "$INSTDIR\ts"
    File "${APP_PATH}\ts\*.qm"
    
    CreateDirectory "$INSTDIR\skins"
    SetOutPath "$INSTDIR\skins"
    File "${APP_PATH}\skins\*.zip"
    
    CreateDirectory "$INSTDIR\plugins"
    SetOutPath "$INSTDIR\plugins"
    File /r /x m1.xml "${APP_PATH}\plugins\*"
    
    SetOutPath "$INSTDIR"
    File "${APP_PATH}\readme.txt"
    File "${APP_PATH}\LICENSE.*"
    File "${APP_PATH}\*.exe"
    File "${APP_PATH}\*.dll"
    CreateShortCut "$DESKTOP\Foreplayer.lnk" "$INSTDIR\Foreplayer.exe"
    
    ;Store installation folder
    WriteRegStr HKCU "Software\Foreplayer" "" $INSTDIR

    ;Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    
    ; Add uninstall information to "Add/Remove Programs"
    WriteRegStr HKLM ${ADD_REMOVE} "DisplayName" ${PROGRAM}
    WriteRegStr HKLM ${ADD_REMOVE} "DisplayIcon" "$INSTDIR\${PROGRAM}"
    WriteRegStr HKLM ${ADD_REMOVE} "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKLM ${ADD_REMOVE} "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
    WriteRegStr HKLM ${ADD_REMOVE} "DisplayVersion" "${VERSION}"
    WriteRegStr HKLM ${ADD_REMOVE} "Publisher" "Bilge Theall"
    ;WriteRegStr HKLM ${ADD_REMOVE} "InstallDate" ""
    WriteRegStr HKLM ${ADD_REMOVE} "URLUpdateInfo" "https://www.foreplayer.org/"
    WriteRegStr HKLM ${ADD_REMOVE} "URLUpdateInfo" "https://www.foreplayer.org/"
    WriteRegStr HKLM ${ADD_REMOVE} "HelpLink" "https://www.foreplayer.org/"
    WriteRegStr HKLM ${ADD_REMOVE} "URLInfoAbout" "https://www.foreplayer.org/"
    WriteRegStr HKLM ${ADD_REMOVE} "URLUpdateInfo" "https://www.foreplayer.org/"
SectionEnd

;--------------------------------
;Descriptions

;--------------------------------
;Uninstaller Section
Section "Uninstall"
    ;ADD YOUR OWN FILES HERE...
    Delete $INSTDIR\*.dll
    Delete $INSTDIR\*.exe
    Delete $INSTDIR\*.txt
    Delete $INSTDIR\LICENSE.*
    Delete $INSTDIR\ScintillaEdit3.dll
    Delete $INSTDIR\iconengines\*.dll
    Delete $INSTDIR\imageformats\*.dll
    Delete $INSTDIR\platforms\*.dll
    Delete $INSTDIR\translations\*.qm
    
    Delete $INSTDIR\ts\*.qm

    Delete $INSTDIR\Uninstall.exe
    Delete $DESKTOP\Foreplayer.lnk
    
    RMDir /r $INSTDIR\ts
    RMDir /r $INSTDIR\iconengines
    RMDir /r $INSTDIR\imageformats
    RMDir /r $INSTDIR\translations
    RMDir /r $INSTDIR\platforms
    RMDir /r $INSTDIR\plugins
    RMDir /r $INSTDIR\skins
    
    DeleteRegKey /ifempty HKCU "Software\Foreplayer"
    DeleteRegKey HKLM ${ADD_REMOVE}
SectionEnd

Function .onInit
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "foreplayer_theall_mutex") i .r1 ?e'
    Pop $R0

    StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "The installer is already running."
    Abort
    
    # set section 'test' as selected and read-only
    IntOp $0 ${SF_SELECTED} | ${SF_RO}
    SectionSetFlags ${core_section} $0

 FunctionEnd


