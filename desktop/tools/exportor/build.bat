@echo off
set cur_path=%~dp0
set project_file=%cur_path%\exportor.pro

if not exist %project_file% echo Project file is not exist. & exit/b

qmake -v>nul
if not %errorlevel%==0 (
	echo qmake is not installed. 
	@echo on
	exit/b
)

set build_path=%temp%\exportor_build

if "%build_path%"=="" (
    echo Use default build path.
    set build_path="build"
)
echo Build path: %build_path%

set exportor_build_path=%build_path%
set dist_path=%cur_path%\..\..\..\dist

if not exist %exportor_build_path% md %exportor_build_path%

pushd %exportor_build_path%

echo Generating makefile...
qmake %project_file% -r
mingw32-make -f Makefile.Release -j2

rem check whether exe file is generated successful
set exportor_exe_file=release\exportor.exe
if exist %exportor_exe_file% copy /y %exportor_exe_file% %cur_path%\..\dist

popd
@echo on