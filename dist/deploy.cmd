@echo off
if exist Foreplayer*setup.exe del Foreplayer*setup.exe

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
makensis install.nsi
if %HAS_SIGNTOOL%==1 (
    echo Signing package...
    signtool.exe sign /a Foreplayer*setup.exe
    signtool.exe timestamp /t http://timestamp.wosign.com Foreplayer*setup.exe
)
@echo on