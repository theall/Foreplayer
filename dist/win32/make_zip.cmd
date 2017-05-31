@echo off
if exist Foreplayer*.zip del Foreplayer*.zip

set HAS_SIGNTOOL=0
signtool.exe /?>nul
if %errorlevel%==0 (
    set HAS_SIGNTOOL=1
)
if %HAS_SIGNTOOL%==1 (
    echo Signing files...
    signtool.exe sign /a *.exe
    signtool.exe timestamp /t http://timestamp.wosign.com *.exe
)
zip -r Foreplayer.zip ./* -x *.cmd *.bat
if %HAS_SIGNTOOL%==1 (
    echo Signing package...
    signtool.exe sign /a Foreplayer*.exe
    signtool.exe timestamp /t http://timestamp.wosign.com Foreplayer*.zip
)
@echo on