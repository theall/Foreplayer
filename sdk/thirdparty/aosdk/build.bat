@echo off
set CURDIR=%cd%
pushd %temp%
if not exist aosdk_build md aosdk_build
cd aosdk_build
cmake %CURDIR% -G"MinGW Makefiles"
mingw32-make -j2 install
popd
@echo on