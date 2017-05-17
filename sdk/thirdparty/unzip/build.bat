@echo off
set CURDIR=%cd%
pushd %temp%
md unzip_build
cd unzip_build
cmake %CURDIR% -G"MinGW Makefiles"
mingw32-make install
popd
@echo on