@echo off
pushd %temp%
set BUILD_NAME=gme_build
if not exist %BUILD_NAME% md %BUILD_NAME%
cd %BUILD_NAME%
cmake %~dp0\src -G"MinGW Makefiles"
mingw32-make -j2 install
popd
@echo on