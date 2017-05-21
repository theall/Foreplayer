@echo off
pushd %temp%
set BUILD_NAME=gmeplugin_build
if not exist %BUILD_NAME% md %BUILD_NAME%
cd %BUILD_NAME%
cmake %~dp0 -G"MinGW Makefiles"
mingw32-make install
popd
@echo on