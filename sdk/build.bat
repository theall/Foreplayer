@if not "%1"=="sdk_only" (
    @call %~dp0\thirdparty\build.bat
    @call %~dp0\plugins\build.bat
)

@echo off
rem build foreplayerlib
pushd %temp%
set BUILD_NAME=foreplayerlib_build
if not exist %BUILD_NAME% md %BUILD_NAME%
cd %BUILD_NAME%
cmake %~dp0 -G"MinGW Makefiles"

set BUILD_CORES=2
if DEFINED CXX_BUILD_CORES set BUILD_CORES=%CXX_BUILD_CORES%
mingw32-make -j%BUILD_CORES% install
popd

@echo on