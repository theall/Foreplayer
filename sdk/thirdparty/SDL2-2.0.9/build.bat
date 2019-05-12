@echo off
pushd %temp%
set BUILD_NAME=sdl_build
if not exist %BUILD_NAME% md %BUILD_NAME%
cmake %~dp0 -G"MinGW Makefiles" -B %BUILD_NAME% -DCMAKE_INSTALL_PREFIX=%~dp0\..\..\
cd %BUILD_NAME%

set BUILD_CORES=2
if DEFINED CXX_BUILD_CORES set BUILD_CORES=%CXX_BUILD_CORES%
mingw32-make -j%BUILD_CORES% install
popd
@echo on