@echo off
set CURDIR=%cd%
pushd %temp%
if not exist m1_build md m1_build
cd m1_build
cmake %CURDIR% -G"MinGW Makefiles"
mingw32-make -j2 install
popd
@echo on