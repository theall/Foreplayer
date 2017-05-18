@echo off
set CURDIR=%cd%
pushd %temp%
if not gme_build md gme_build
cd gme_build
cmake %CURDIR%\src -G"MinGW Makefiles"
mingw32-make -j2 install
popd
@echo on