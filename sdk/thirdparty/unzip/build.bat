@echo off
set CURDIR=%cd%
pushd %temp%
if not exist unzip_build md unzip_build
cd unzip_build
cmake %CURDIR% -G"MinGW Makefiles"
mingw32-make install
popd
@echo on