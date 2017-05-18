@echo off
set CURDIR=%cd%
pushd %temp%
if not exist fft_build md fft_build
cd fft_build
cmake %CURDIR% -G"MinGW Makefiles"
mingw32-make -j2 install
popd
@echo on