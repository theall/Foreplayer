@echo off
set cur_path=%~dp0
set project_file=%cur_path%foreplayer.pro

if not exist %project_file% echo Project file is not exist. & exit/b

qmake -v>nul
if not %errorlevel%==0 (
	echo qmake is not installed. 
	@echo on
	exit/b
)

set build_path=%temp%\foreplayer_build

if "%build_path%"=="" (
    echo Use default build path.
    set build_path="build"
)
echo Build path: %build_path%

set foreplayer_build_path=%build_path%\foreplayer
set dist_path=%cur_path%..\dist\win32

if not exist %foreplayer_build_path% md %foreplayer_build_path%

pushd %foreplayer_build_path%

echo Generating makefile...
qmake %project_file% -r
mingw32-make -f Makefile.Release -j2

rem check whether exe file is generated successful
set foreplayer_exe_file=release\foreplayer.exe
if exist %foreplayer_exe_file% copy /y %foreplayer_exe_file% %dist_path%

rem install translation files
echo Compiling translation files...
call %~dp0\app\gui\resource\ts\release.cmd
copy /y %cur_path%app\gui\resource\ts\*.qm %dist_path%\ts
popd

rem Build export tool
call %~dp0\tools\build.bat

@echo on