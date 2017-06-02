@echo off
if exist Foreplayer*.zip del Foreplayer*.zip

set HAS_SIGNTOOL=0
signtool.exe /?>nul
if %errorlevel%==0 (
    echo Signing files...
    signtool.exe sign /a *.exe
    signtool.exe timestamp /t http://timestamp.wosign.com *.exe
)
zip -r Foreplayer.zip ./* -D CopyDependency -x CopyDependency/* CopyDependency.exe *.7z *.zip *.bat *.cmd 7zignore.txt zipignore.txt setting.ini
@echo on