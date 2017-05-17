@echo off
set CURDIR=%cd%
pushd %temp%
md m1_build
cd m1_build
cmake %CURDIR% -G"MinGW Makefiles"
mingw32-make -j2 install
popd
@echo on