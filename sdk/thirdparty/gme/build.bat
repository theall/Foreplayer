@echo off
set CURDIR=%cd%
pushd %temp%
md gme_build
cd gme_build
cmake %CURDIR%\src -G"MinGW Makefiles"
mingw32-make -j2 install
popd
@echo on