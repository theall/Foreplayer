@echo off
if exist Foreplayer*.7z del Foreplayer*.7z

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
7z a -x!*.json Foreplayer.7z *
if %HAS_SIGNTOOL%==1 (
    echo Signing package...
    signtool.exe sign /a Foreplayer*.7z
    signtool.exe timestamp /t http://timestamp.wosign.com Foreplayer*.7z
)
@echo on