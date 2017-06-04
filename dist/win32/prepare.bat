@echo off
echo Generating copydependency...
call CopyDependency/build.bat

echo Coping qt dependency libraries...
windeployqt --no-opengl-sw --no-system-d3d-compiler foreplayer.exe

echo Packaging skins...
set CUR_DIR=%cd%
for /f %%i in ('dir /s /b /a:d ..\..\desktop\tools\skins') do pushd %%i & 7z a %CUR_DIR%/skins/%%~ni.zip * & popd

echo Generating translations...
for %%i in (%~dp0\..\..\desktop\app\gui\resource\ts\*.ts) do lrelease %%i -qm ts\%%~ni.qm
@echo on

echo Copying std c++ libraries...
copyDependency exportor.exe
copyDependency foreplayer.exe
