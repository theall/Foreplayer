@echo off
set CURDIR=%cd%
pushd %temp%
if not exist foreplayerlib_build md foreplayerlib_build
cd foreplayerlib_build
cmake %CURDIR% -G"MinGW Makefiles"
mingw32-make -j2 install
popd
@echo on