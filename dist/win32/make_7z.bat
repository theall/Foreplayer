@echo off
if exist Foreplayer*.7z del Foreplayer*.7z

signtool.exe /?>nul
if %errorlevel%==0 (
    echo Signing files...
    signtool.exe sign /a *.exe
    signtool.exe timestamp /t http://timestamp.wosign.com *.exe
)
7z a -x@7zignore.txt Foreplayer.7z *
@echo on