@echo off
set CURDIR=%cd%
pushd %temp%
if not exist unrar_build md unrar_build
cd unrar_build
cmake %CURDIR% -G"MinGW Makefiles"
mingw32-make -j2 install
popd
@echo on